#include <io.h>
#include <direct.h>
#include <string.h>
#include <vector>
#include <string>
#include <time.h>
#include <assert.h>
#include "ZipFile.h"

ZipFile::ZipFile()
{
    m_unzFile = NULL;
}

ZipFile::~ZipFile()
{
	Close();
}

BOOL ZipFile::Open(const char* szFileName)
{
    m_unzFile = unzOpen64(szFileName);
    return m_unzFile != NULL;
}

BOOL ZipFile::Open(const void* buffer, unsigned long nSize)
{
	m_unzFile = unzOpenBuffer(buffer, nSize);
	return m_unzFile != NULL;
}

void ZipFile::Close()
{
	if(m_unzFile != NULL)
	{
		unzClose(m_unzFile);
		m_unzFile = NULL;
	}
}

BOOL ZipFile::GetCurrentFileInfo(ZIPENTRY* pEntry) const
{
	if(m_unzFile == NULL || pEntry == NULL)
        return FALSE;

    unz_file_info64 unzfi;
    memset(pEntry, 0, sizeof(ZIPENTRY));
    int nRet = unzGetCurrentFileInfo64(m_unzFile,
                                       &unzfi,
                                       pEntry->szFileName,
                                       sizeof(pEntry->szFileName),
                                       NULL,
                                       0,
                                       pEntry->szComment,
                                       sizeof(pEntry->szComment));
    if(nRet != UNZ_OK)
        return FALSE;

    pEntry->nCompressedSize = unzfi.compressed_size;
    pEntry->nUnCompressedSize = unzfi.uncompressed_size;
    sprintf(pEntry->szModifiedTime, "%04d-%02d-%02d %02d:%02d:%02d",
            unzfi.tmu_date.tm_year,
            unzfi.tmu_date.tm_mon,
            unzfi.tmu_date.tm_mday,
            unzfi.tmu_date.tm_hour,
            unzfi.tmu_date.tm_min,
            unzfi.tmu_date.tm_sec);
    int nLen = strlen(pEntry->szFileName);
    pEntry->bDir = (pEntry->szFileName[nLen - 1] == '/') ? TRUE : FALSE;
    unzGetFilePos64(m_unzFile, &pEntry->pos);

    return TRUE;
}

BOOL ZipFile::FindFirstFile(__out ZIPENTRY* pEntry) const
{
	if(m_unzFile == NULL)
		return FALSE;

    if(unzGoToFirstFile(m_unzFile) == UNZ_OK)
    {
        return GetCurrentFileInfo(pEntry);
    }
    else
    {
        return FALSE;
    }
}

BOOL ZipFile::FindNextFile(__out ZIPENTRY* pEntry) const
{
	if(m_unzFile == NULL)
		return FALSE;

    if(unzGoToNextFile(m_unzFile) == UNZ_OK)
    {
        return GetCurrentFileInfo(pEntry);
    }
    else
    {
        return FALSE;
    }
}

BOOL ZipFile::CreateDirectory(const char* szDirPath) const
{
    return (_mkdir(szDirPath) == 0);
}

BOOL ZipFile::IsFileExists(const char* szFilePath) const
{
    return _access(szFilePath, 0) == 0;
}

BOOL ZipFile::FindFile(const char* szFileInZipName, __out ZIPENTRY* pEntry) const
{
    if(m_unzFile == NULL || szFileInZipName == NULL || strlen(szFileInZipName) == 0)
        return FALSE;

    unz64_file_pos unzfp;
    if(unzGetFilePos64(m_unzFile, &unzfp) == UNZ_OK)
    {
        if(unzLocateFile(m_unzFile, szFileInZipName, 0) == UNZ_OK)
        {
            GetCurrentFileInfo(pEntry);
            unzGoToFilePos64(m_unzFile, &unzfp);
            return TRUE;
        }
    }

    return FALSE;
}

BOOL ZipFile::ExtractFile(__in ZIPENTRY* pEntry, const char* szDir, ZIPCALLBACK callback) const
{
	if(m_unzFile == NULL || pEntry == NULL || szDir == NULL || strlen(szDir) == 0 || pEntry->bDir)
        return FALSE;

    char szPath[256] = { 0 };
    ProcessPathToken(szPath, szDir);
    int nLen = strlen(szPath);
    if(szPath[nLen - 1] != '/')
        strcat(szPath, "/");
    if(!IsFileExists(szPath))
        return FALSE;

    char szFileName[256] = { 0 };
    ParseFileName(szFileName, pEntry->szFileName);
    strcat(szPath, szFileName);
    FILE* fp = fopen(szPath, "wb");
    if(fp == NULL)
        return FALSE;

    unz64_file_pos unzfp;
    if(unzGetFilePos64(m_unzFile, &unzfp) != UNZ_OK)
    {
        fclose(fp);
        return FALSE;
    }

    if(unzGoToFilePos64(m_unzFile, &pEntry->pos) != UNZ_OK)
    {
        fclose(fp);
        return FALSE;
    }

    if(unzOpenCurrentFile(m_unzFile) != UNZ_OK)
    {
        fclose(fp);
        return FALSE;
    }

    ZIPMSG zmsg;
    zmsg.nFileCount = 1;
    zmsg.nFileIndex = 1;
    zmsg.nFileSize = pEntry->nUnCompressedSize;
    sprintf(zmsg.szFileName, pEntry->szFileName);

    char temp[4096];
    size_t nTotal = 0;
    int nRead;
    while((nRead = unzReadCurrentFile(m_unzFile, temp, sizeof(temp))))
    {
        fwrite(temp, 1, nRead, fp);
        nTotal += nRead;
        if(callback != NULL)
        {
            zmsg.nFileProcessedSize = nTotal;
            callback(&zmsg);
        }
    }
    fflush(fp);
    fclose(fp);
    unzCloseCurrentFile(m_unzFile);
    unzGoToFilePos64(m_unzFile, &unzfp);

    return (nTotal == pEntry->nUnCompressedSize ? TRUE : FALSE);
}

BOOL ZipFile::ExtractFile(__in ZIPENTRY* pEntry, void* pBuf, size_t nBufSize, ZIPCALLBACK callback) const
{
	if(m_unzFile == NULL || pEntry == NULL || pBuf == NULL || pEntry->bDir)
        return FALSE;

    unz64_file_pos unzfp;
    if(unzGetFilePos64(m_unzFile, &unzfp) == UNZ_OK)
    {
        if(unzGoToFilePos64(m_unzFile, &pEntry->pos) == UNZ_OK)
        {
            if(unzOpenCurrentFile(m_unzFile) == UNZ_OK)
            {
                ZIPMSG zmsg;
                zmsg.nFileCount = 1;
                zmsg.nFileIndex = 1;
                zmsg.nFileSize = pEntry->nUnCompressedSize;
                sprintf(zmsg.szFileName, pEntry->szFileName);

                char temp[4096];
                size_t nTotal = 0;
                int nRead;
                while((nRead = unzReadCurrentFile(m_unzFile, temp, sizeof(temp))))
                {
                    memcpy(static_cast<char*>(pBuf) + nTotal, temp, nRead);
                    nTotal += nRead;
                    if(callback != NULL)
                    {
                        zmsg.nFileProcessedSize = nTotal;
                        callback(&zmsg);
                    }
                }
                unzCloseCurrentFile(m_unzFile);
                unzGoToFilePos64(m_unzFile, &unzfp);
                return (nTotal == pEntry->nUnCompressedSize ? TRUE : FALSE);
            }
        }
    }

    return FALSE;
}

BOOL ZipFile::ExtractAllFiles(const char* szDir, ZIPCALLBACK callback) const
{
	if(m_unzFile == NULL || szDir == NULL || strlen(szDir) == 0)
        return FALSE;

    ZIPMSG zmsg;
    memset(&zmsg, 0, sizeof(ZIPMSG));
    unz_global_info64 unzgi;
    unzGetGlobalInfo64(m_unzFile, &unzgi);
    zmsg.nFileCount = unzgi.number_entry;

    char szPath[256] = { 0 };
    ProcessPathToken(szPath, szDir);
    int nLen = strlen(szPath);
    if(szPath[nLen - 1] != '/')
        strcat(szPath, "/");

    if(!IsFileExists(szPath))
        return FALSE;

    char szFilePath[256] = { 0 };
    ZIPENTRY zi;
    BOOL bRet = FindFirstFile(&zi);
    while(bRet)
    {
        zmsg.nFileIndex++;
        zmsg.nFileSize = zi.nUnCompressedSize;
        sprintf(zmsg.szFileName, zi.szFileName);

        memset(szFilePath, 0, sizeof(szFilePath));
        sprintf(szFilePath, "%s%s", szPath, zi.szFileName);
        if(zi.bDir)
        {
            if(!IsFileExists(szFilePath))
            {
                if(!CreateDirectory(szFilePath))
                    return FALSE;
            }
            if(callback != NULL)
            {
                zmsg.nFileProcessedSize = 0;
                callback(&zmsg);
            }
        }
        else
        {
            FILE* fp = fopen(szFilePath, "wb");
            if(fp == NULL)
                return FALSE;

            if(unzOpenCurrentFile(m_unzFile) != UNZ_OK)
            {
                fclose(fp);
                return FALSE;
            }

            char temp[4096];
            size_t nTotal = 0;
            int nRead;
            while((nRead = unzReadCurrentFile(m_unzFile, temp, sizeof(temp))))
            {
                fwrite(temp, 1, nRead, fp);
                nTotal += nRead;
                if(callback != NULL)
                {
                    zmsg.nFileProcessedSize = nTotal;
                    callback(&zmsg);
                }
            }
            fflush(fp);
            fclose(fp);
            unzCloseCurrentFile(m_unzFile);

            if(nTotal != zi.nUnCompressedSize)
                return FALSE;
        }
        bRet = FindNextFile(&zi);
    }
    return TRUE;
}

void ZipFile::ParseFileName(char* szFileName, const char* szFilePath) const
{
    assert(szFileName != NULL && szFilePath != NULL);
    const char* p = strrchr(szFilePath, '/');
    if(p != NULL)
        strcpy(szFileName, p + 1);
    else
        strcpy(szFileName, szFilePath);
}

void ZipFile::ProcessPathToken(char* szNewFilePath, const char* szFilePath) const
{
    strcpy(szNewFilePath, szFilePath);
    char* p = szNewFilePath;
    while(*p != '\0')
    {
        if(*p == '\\')
            *p = '/';
        p++;
    }
}

struct tm* ZipFile::GetSystemTime() const
{
    time_t nowtime;
    time(&nowtime);
    return localtime(&nowtime);
}
