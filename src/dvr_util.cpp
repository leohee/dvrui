/******************************************************************************
* DM8168 Hybrid DVR
* Copyright by UDWorks, Incoporated. All Rights Reserved.
*-----------------------------------------------------------------------------
* @file	dvr_util.cpp
* @brief
* @author
* @section	MODIFY history
*     - 2011.01.01 : First Created
******************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include "dvr_defines.h"
#include <time.h>
#include <string.h>
#include <QByteArray>
#include <QDir>
#include "dvr_util.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 local functions
-----------------------------------------------------------------------------*/
void GetCurrentTime(struct tm *tptr)
{
    struct tm tm1 ;
    time_t timeval ;

    time(&timeval) ;

    localtime_r(&timeval, &tm1) ;
    tm1.tm_year += 1900 ;
    memcpy(tptr, &tm1, sizeof(struct tm)) ;
}

QString StrToQString(char * str)
{
    return QString::fromUtf8(str,strlen(str));
}

char * QStringToStr(QString qstr)
{
    QByteArray qba;
    qba=qstr.toUtf8();
    return qba.data();
}

int ReadProfileFile(char *fileName, unsigned char *addr, unsigned int readSize, unsigned int *file_offset, unsigned int *actualReadSize)
{
    int retVal = 1;
    unsigned char  *curAddr;

    unsigned int readDataSize, fileSize, chunkSize=1024*100;
    unsigned int userReadSize;
    unsigned int fileoffset;

    FILE *hndlFile;

    hndlFile = fopen(fileName, "rb");

    if(hndlFile == NULL)
    {
        retVal = 0;
        goto exit;
    }

    fseek(hndlFile, 0L, SEEK_END);
    fileSize = ftell(hndlFile);

    if (file_offset!=NULL)
    {
        if (*file_offset+readSize >= fileSize)
        {
            fclose(hndlFile);
            if(actualReadSize != NULL)
                *actualReadSize = 0;
            return 0;
        }
        fileoffset=*file_offset;
    }
    else
    {
        fileoffset = 0;
    }
    fseek(hndlFile, fileoffset, SEEK_SET);

    if(fileSize != readSize)
    {        
        printf("filesize=%d readSize=%d\n",fileSize,readSize);
        fclose(hndlFile);
        if(actualReadSize != NULL)
            *actualReadSize = 0;
        return 0;
    }

    curAddr = addr;
    fileSize = 0;

    userReadSize = readSize;

    while(1)
    {
        if(userReadSize != 0)
        {
            if(chunkSize > userReadSize)
                chunkSize = userReadSize;
            readDataSize = fread(curAddr, 1, chunkSize, hndlFile);
            fileSize += readDataSize;
            if(chunkSize != readDataSize)
                break;
            if(fileSize>=userReadSize)
                break;
            curAddr += chunkSize;
        }
        else
        {
            readDataSize = fread(curAddr, 1, chunkSize, hndlFile);
            fileSize+=readDataSize;
            if(chunkSize!=readDataSize)
                break;
            curAddr+=chunkSize;
        }
    }

    fclose(hndlFile);
    if(actualReadSize != NULL)
        *actualReadSize = fileSize;
    if (file_offset != NULL)
        *file_offset += fileSize;

exit:
    if(retVal!=1)
    {
        fileSize=0;
    }

    return retVal;
}

int WriteProfileFile(char *fileName, unsigned char *addr, unsigned int size)
{
    int retVal = 1;
    unsigned int writeDataSize;

    bool errorInWriting = FALSE;

    FILE *hndlFile;

    if(size==0)
        return 0;

    hndlFile = fopen(fileName, "wb");

    if(hndlFile == NULL)
    {
        retVal = 0;
        goto exit;
    }

    {
        // write in units of chunkSize
        unsigned int fileSize, chunkSize = 96*1024;
        unsigned char  *curAddr;


        fileSize = size;
        curAddr  = (unsigned char *)addr;

        while(fileSize>0)
        {
            if(fileSize<chunkSize)
            {
                chunkSize = fileSize;
            }
            writeDataSize=0;
            writeDataSize = fwrite(curAddr, 1, chunkSize, hndlFile);
            if(writeDataSize!=chunkSize)
            {
                errorInWriting = true;
                retVal = 0;
                break;
            }
            curAddr += chunkSize;
            fileSize -= chunkSize;
        }
        writeDataSize = size - fileSize;
    }

    fclose(hndlFile);

    exit:
    if(retVal!=1)
        printf("WriteProfileFile(%s) Fail\n", fileName);

    return retVal;
}

int ReadDVRsettingInfo(char *filename, void* pInfo, int infoSize)
{
	QString info_path = QDir::currentPath();
    unsigned int readSize = 0;

    if(pInfo != NULL)
    {
    	info_path.append(filename);
        ReadProfileFile(QStringToStr(info_path),(unsigned char*)pInfo, infoSize, 0, &readSize);
    }
    return readSize;
}

int WriteDVRsettingInfo(char *filename, void* pInfo, int infoSize)
{
	QString info_path = QDir::currentPath();

    if(pInfo != NULL) {
    	info_path.append(filename);
        return WriteProfileFile(QStringToStr(info_path), (unsigned char *)pInfo, infoSize);
    }
    return 0;
}

int DeleteDVRsettingInfo(char *filename)
{
    return remove(filename);
}


void GetBuildDateTime(char * strbuilddatetime)
{
    sprintf(strbuilddatetime,"BUILD DATE=%s, TIME=%s",__DATE__,__TIME__);
}

