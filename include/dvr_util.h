#ifndef UTIL_H
#define UTIL_H

#include <QString>
void GetCurrentTime(struct tm *tptr);
QString StrToQString(char * str);
char * QStringToStr(QString qstr);

int ReadProfileFile(char *fileName, unsigned char *addr, unsigned int readSize, unsigned int *file_offset, unsigned int *actualReadSize);
int WriteProfileFile(char *fileName, unsigned char *addr, unsigned int size);

int ReadDVRsettingInfo(char *filename, void* pInfo, int infoSize);
int WriteDVRsettingInfo(char *filename, void* pInfo, int infoSize);
int DeleteDVRsettingInfo(char *filename);

void GetBuildDateTime(char *strbuilddatetime);

#endif 
