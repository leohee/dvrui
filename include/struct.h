#ifndef STRUCT_H
#define STRUCT_H

#include <time.h>
#include <QQueue>
#include "define.h"

//BACKUP
typedef struct
{
	///backup start date/time
    struct tm  tmFromBackupTime;
    ///backup end date/time
    struct tm  tmToBackupTime;
    ///backup channel in avi mode
    int iDestChannels[MAX_CAMERA];
    ///basket or avi
    int iBackupFileType;
    ///CD/DVD or USB
    int iBackupMediaType;
}BACKUPINFO, *PBACKUPINFO;  //BACKUPINFO

// USER
typedef struct
{
	///user enable 여부
    int  iEnable;
    ///user level (ADMIN or USER)
    int  iAccessLevel;
    ///content 조작 권한(LIVE,PLAYBACK,etc)
    unsigned int iAccessContents;
    ///pass word
    char  password[MAX_PASSWORD_CHAR];
}USER,*PUSER;               //USER INFO

typedef struct
{
	///enable user count
    int usercount;
    ///user info
    USER userlist[MAX_USER];
}USERLIST,*PUSERLIST;               //USER LIST INFO

// PLAYBACK
typedef struct
{
	///playback mode (play,stop,..,etc)
    int iPlaybackMode;
    ///play speed(1/4 ~ 32)
    int iSpeed;
 }PLAYBACKMODEINFO,*PPLAYBACKMODEINFO ; //PLAYBACK MODE INFO

typedef struct
{
	///Date별 record data 존재 여부
    int iMonthTable[PLAYCALENDARMAXDAYCOUNT];
    ///camera/time별 record data 존자 여부
    int iHourTable[MAX_CAMERA][PLAYBACKDAYMINUTECOUNT];
}PLAYBACKTABLEINFO,* PPLAYBACKTABLEINFO; //PLAYBACK TABLEINFO

typedef struct
{
	///zoom mode in timebar
    int iTimeTableZoomRate;
    ///time line page number 
    int iTimeTableTimeRate;
    ///channel page number in the timebar
    int iTimeTablePageChannel;
}PLAYBACKTABLE,*PPLAYBACKTABLE;

typedef struct
{
	///playback mode/speed
    PLAYBACKMODEINFO playbackmode;
    ///record data table
    PLAYBACKTABLEINFO playbacktableinfo;
}PLAYBACKINFO,PPLAYBACKINFO;

// SYSTEM
typedef struct
{
	///current date/time
    struct tm  currenttime;
    ///playback date/time
    struct tm  playbacktime;
}CURENTTIME,*PCURENTTIME;

typedef struct
{
	///firmware version
    char FwVer[MAX_CHAR_32];
    ///hardware version
    char HwVer[MAX_CHAR_32];
    ///mac address for eth0
    char MacAddr1[MAX_CHAR_32];
    ///mac address for eth1
    char MacAddr2[MAX_CHAR_32];
    ///build date
    char BuildDateTime[MAX_CHAR_48];
}VERSIONINFO,*PVERSIONINFO;

typedef struct
{
     struct tm systemtime;
}SYSTEMDATETIME,*PSYSTEMDATETIME;

//DETECTSTATUS 
typedef struct
{
	///motion event detected or not
    int iMotionDetectOn;
}DETECTMOTION,*PDETECTMOTION;       //DETECT MOTION INFO

typedef struct
{
	///sensor event detected or not
    int iSensorDetectOn;
}DETECTSENSOR,*PDETECTSENSOR;       //DETECT SENSOR INFO

typedef struct
{
	///video loss event detected or not
    int iVideoLossOn;
}DETECTVIDEOLOSS,*PDETECTVIDEOLOSS;       //DETECT VIDEOLOSS INFO

typedef struct
{
	///recording on/off status
    int iRecordingOn;
    ///record by schedule(continuous record)
    int iNormal;
    ///record by motion detect
    int iMotion;
    ///record by sensor detect
    int iSensor;
    ///record by video loss detect
    int iVideoloss;
    ///record by emergency recording
    int iEmergency;
}DETECTRECORDING,*PDETECTRECORDING;       //DETECT RECORDING INFO

typedef struct
{
	///alram on/off status
    int iAlarmDetectOn;
}DETECTALARM,*PDETECTALARM;         //DETECT ALARM INFO


typedef struct
{
	///motion detect status
  DETECTMOTION detectmotion[MAX_CAMERA];
  ///sensor detect status
  DETECTSENSOR detectsensor[MAX_SENSOR];
  ///video loss detect status
  DETECTVIDEOLOSS detectvideoloss[MAX_CAMERA];
  ///recording on/off status
  DETECTRECORDING detectrecording[MAX_CAMERA];
  ///audio on/off status
  DETECTRECORDING detectaudiorecording[MAX_CAMERA];  
  ///alarm on/off status
  DETECTALARM detectalarm[MAX_ALARM];
  ///playback status
  PLAYBACKMODEINFO playbackmodeinfo[MAX_CAMERA];
}DETECTSTATUS,*PDETECTSTATUS;     //DETECT EVENT INFO

typedef struct
{
	///post recording end time
    time_t endtime;
    ///post record duration
    int duration;
    ///post record reason(motion,sensor,vloss)
    int RecReason;
}POSTRECINFO,*PPOSTRECINFO;

typedef struct
{
	///display output이 swap되었는지 여부
    int  iMainSwapEnable;
    ///Live mode인지 playback mode인지
    int  iLivePlaybackMode;
    ///Setting창이 보이는지 아닌지
    int  iSetupPropertyOn;
    ///전체화면 인지 normal mode화면인지
    int  iConsoleModeOn;
    ///sub toolbar을 보여줄것인지 숨길것인지
    int  iSecondConsoleModeOn;
    ///main display output의 현재 page number
    int  iPageNum;
    ///second display output의 현재 page number
    int  iSecondPageNum;
    ///Main display output의 layout x좌표
    int  iLayOutWindowX;
    ///Main display output의 layout y좌표
    int  iLayOutWindowY;
    ///Main display output의 layout width
    int  iLayOutWindowWidth;
    ///Main display output의 layout height
    int  iLayOutWindowHeight;
    ///Main display output의 layout 개수
    int  iLayout;
    ///second display output의 layout 개수
    int  iSecondLayout;
	///version 정보, MAC addresss등
    VERSIONINFO versioninfo;
    ///backup 설정 정보
    BACKUPINFO backupinfo;
	///현재 playback 정보
    PLAYBACKINFO playbackinfo;
    ///playback timebar 설정값
    PLAYBACKTABLE playbacktable;
    
	///audio mute status
    int AudioMuteOnOff;
    ///current recording schedule value
    int curRecSchedule[MAX_CAMERA];
    ///current post recording status
    POSTRECINFO postrecinfo[MAX_CAMERA];
    ///channel frame size
    SOURCE_CH_STATUS_S chStatus[MAX_CAMERA];
    ///display output resolution 
    int OutputResolution[2];
}RUNTIMESETTINGS,*PRUNTIMESETTINGS;

typedef struct
{
	
/*!EVENT_CMD_MOTION(0)*/
/*!EVENT_CMD_SENSOR(1)*/
/*!EVENT_CMD_VIDEOLOSS(2)*/
/*!EVENT_CMD_ALARM(3)*/
/*!EVENT_CMD_MOUSEWHEEL(4)*/
/*!EVENT_CMD_POSTREC_SET(5)*/
/*!EVENT_CMD_POSTREC_CLEAR(6)*/
    int cmd;
    int param[32];
}DvrEvent;

#include <QRect>

typedef struct
{
	///ui size
    QRect MainWindow; 	
    ///main toolbar coordinate
    QRect MainToolbar; 	
    ///sub toolbar coordinate
    QRect SecondToolbar; 
    ///side bar coordinate
    QRect Sidebar;     	
    ///playback timebar coordinate
    QRect TimeLinebar;	
    ///status bar coordinate
    QRect Statusbar;	
}Coordinate;
/*!----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
extern PROFILE  profile;
extern PROFILE  dlgdata;
extern PROFILE  dlgdatatemp;
extern USER     current_user;
extern USERLIST userlist;
extern USERLIST dlguserlist;
extern DETECTSTATUS detectstatus;

extern CURENTTIME curtime;
extern SYSTEMDATETIME systemdatetime;
extern RUNTIMESETTINGS runtimesettings;
extern RUNTIMESETTINGS dlgruntimesettings;
extern RUNTIMESETTINGS dlgruntimesettingstemp;

extern Coordinate coordinator;
extern dvr_disk_info_t mp;

extern QQueue<void*> EvtQ;

#endif 
