#ifndef __ZIPFILE_H_INCLUDED__
#define __ZIPFILE_H_INCLUDED__

#pragma warning (disable:4244)
#pragma warning (disable:4996)

#include <windows.h>
#include "../zlib/unzip.h"

typedef struct ZipEntry
{
    BOOL bDir;
    char szFileName[256];
    char szComment[2048];
    char szModifiedTime[128];
    unsigned int nCompressedSize;
    unsigned int nUnCompressedSize;
    unz64_file_pos pos;
} ZIPENTRY;

typedef struct ZipMsg
{
    char szFileName[256];
    size_t nFileProcessedSize;
    size_t nFileSize;
    int nFileIndex;
    int nFileCount;
} ZIPMSG;

typedef void(*ZIPCALLBACK)(ZIPMSG*);

class ZipFile
{
public:
                ZipFile();
                ~ZipFile();

public:
    BOOL        Open(const char* szFileName);
	BOOL		Open(const void* buffer, unsigned long nSize);
    void        Close();
    BOOL        FindFile(const char* szFileInZipName, __out ZIPENTRY* pEntry) const;
    BOOL        ExtractFile(__in ZIPENTRY* pEntry, const char* szDir, ZIPCALLBACK callback = NULL) const;
    BOOL        ExtractFile(__in ZIPENTRY* pEntry, void* pBuf, size_t nBufSize, ZIPCALLBACK callback = NULL) const;
    BOOL        ExtractAllFiles(const char* szDir, ZIPCALLBACK callback = NULL) const;

protected:
    BOOL        GetCurrentFileInfo(__out ZIPENTRY* pEntry) const;
    BOOL        FindFirstFile(__out ZIPENTRY* pEntry) const;
    BOOL        FindNextFile(__out ZIPENTRY* pEntry) const;
    BOOL        CreateDirectory(const char* szDirPath) const;
    BOOL        IsFileExists(const char* szFilePath) const;
    void        ParseFileName(char* szFileName, const char* szFilePath) const;
    void        ProcessPathToken(char* szNewFilePath, const char* szFilePath) const;
    struct tm*  GetSystemTime() const;

private:
    unzFile m_unzFile;
};

#endif //! __ZIPFILE_H_INCLUDED__
