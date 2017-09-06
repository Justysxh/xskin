#include <stdio.h>
#include <string.h>

#include "ZipFile.h"

using namespace std;


// zip file callback.
void OnZipCallback(ZIPMSG* pMsg)
{
    printf("%d/%d, %s --- %ld/%ld\n",
           pMsg->nFileIndex, pMsg->nFileCount, pMsg->szFileName, pMsg->nFileProcessedSize, pMsg->nFileSize);
}

int main(int argc, char *argv[])
{
    char* buf = NULL;
    size_t nBufSize = 0;
    // test unzip /////////////////////////////////////////////////////////////
    ZipFile zf;
    if(zf.Open("/home/finch/Desktop/test_zip_add.zip", ZipFile::UNZIP))
    {
        ZIPENTRY zi;
        if(zf.FindFile("dir1/dir2/dir3/file.iso", &zi))
        {
            // test unzip to buffer.
            buf = new char[zi.nUnCompressedSize];
            nBufSize = zi.nUnCompressedSize;
            memset(buf, 0, sizeof(zi.nUnCompressedSize));
            if(zf.ExtractFile(&zi, buf, zi.nUnCompressedSize, OnZipCallback))
            {
                printf("extract to buffer success.\n\n\n");
            }
            else
            {
                printf("fail to extract to buffer.\n\n\n");
            }

            // test unzip to file.
            if(zf.ExtractFile(&zi, "/home/finch/Desktop", OnZipCallback))
            {
                printf("extract to file success.\n\n\n");
            }
            else
            {
                printf("fail to extract to file.\n\n\n");
            }
        }
        else
        {
            printf("fail to find file.\n\n\n");
        }

        // test unzip all files.
        if(zf.ExtractAllFiles("/home/finch/Desktop", OnZipCallback))
        {
            printf("extract all files success.\n\n\n");
        }
        else
        {
            printf("fail to extract all files.\n\n\n");
        }
        zf.Close();
    }
    else
    {
        printf("fail to open zip file.\n\n\n");
    }

    // test zip /////////////////////////////////////////////////////////////
    ZipFile zf2;
    if(zf2.Open("/home/finch/Desktop/test_zip_add.zip", ZipFile::ZIP))
    {
        if(zf2.AddFile("dir1/dir2/dir3/file.iso", "VCP600ENU1.iso", OnZipCallback))
        {
             printf("zip add file success.\n\n\n");
        }
        else
        {
            printf("fail to add file into zip.\n\n\n");
        }
        if(zf2.AddFile("file_buf.iso", buf, nBufSize, OnZipCallback))
        {
            printf("zip add file from buffer success.\n\n\n");
        }
        else
        {
            printf("fail to add file from buffer into zip.\n\n\n");
        }
        zf2.Close();
    }
    else
    {
        printf("fail to create zip file.\n\n\n");
    }
    return 0;
}
