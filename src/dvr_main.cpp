/*!*****************************************************************************
* DM8168 Hybrid DVR
* Copyright by UDWorks, Incoporated. All Rights Reserved.
*-----------------------------------------------------------------------------
* @file	dvr_main.c
* @brief
* @author
* @section	MODIFY history
*     - 2011.01.01 : First Created
******************************************************************************/

/*!----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <QDir>
#include <QScreen>
//#include <QWSServer>
#include "dvr_defines.h"
#include "dvr_main.h"
#include "ui_dvr_main.h"
#include "WidgetKeyboard.h"

/*!----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*!----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
PROFILE  profile;
PROFILE  dlgdata;
PROFILE  dlgdatatemp;

USER     current_user;
USERLIST userlist;
USERLIST dlguserlist;
DETECTSTATUS detectstatus;

CURENTTIME curtime;
SYSTEMDATETIME systemdatetime;
RUNTIMESETTINGS runtimesettings;
RUNTIMESETTINGS dlgruntimesettings;
RUNTIMESETTINGS dlgruntimesettingstemp;
Coordinate coordinator;

struct tm playbackupdatetime;

int g_rfs_type=0;
QQueue<void*> EvtQ;
QQueue<void*> RecEvtQ;

//static int nChannelIndex = 0;
int availableHddCount = 0;

dvrmain *pMainClass=NULL;
dvr_layout_main *pLayoutClass=NULL;
dvr_sidebar *pSidebar=NULL;
dvr_timelinebar_play *pTimebar=NULL;
dvr_statusbar *pStatusbar = NULL;
dvr_toolbar_main *pToolbar = NULL;
dvr_toolbar_sub *pSubToolbar = NULL;
dvr_setup *pSetup = NULL;
dvr_login *pLoginDlg = NULL;
NotifyDialog *pNotiDlg = NULL;
EventLogDialog *pEvtDlg = NULL;
/*!----------------------------------------------------------------------------
 local functions
-----------------------------------------------------------------------------*/
static void dvrmain_dio_callback(int evt,void* param)
{
    dprintf("DIO Received evt=0x%X, param=%d\n",evt,(int)param);
    //Sensor, Alarm Event수신
    if(evt<MAX_SENSOR)
    {
        dprintf("[DIO] SENSOR %d\n",evt);
        if(profile.sensor[evt].iSensorOn==SENSOR_DETECT_OFF)
            return;
        DvrEvent *qMsg = new DvrEvent;
        qMsg->cmd = EVENT_CMD_SENSOR;
        qMsg->param[0]=evt;
        qMsg->param[1]=0;//(int)param;
        EvtQ.enqueue((void*)qMsg);
    }
    else if(evt<MAX_SENSOR+MAX_ALARM)
    {
        dprintf("[DIO] Alarm %d\n",evt);
        DvrEvent *qMsg = new DvrEvent;
        qMsg->cmd = EVENT_CMD_ALARM;
        qMsg->param[0]=evt-MAX_SENSOR;
        qMsg->param[1]=0;//(int)param;
        EvtQ.enqueue((void*)qMsg);
    }
}
/*!constructor - profile struct를 읽어 liveback mode로 초기화하고 login dialog표시*/
dvrmain::dvrmain(QWidget *parent) :
        QDialog(parent,Qt::CustomizeWindowHint |Qt::FramelessWindowHint),
    ui(new Ui::dvrmain)
{
    pMainClass = this;
    pLayoutClass = &Dvrmainlayout;
    pSidebar = &Dvrmainsidebar;
    pTimebar = &Dvrmainplaybacktimelinebar;
    pStatusbar = &Dvrmainstatusbar;
    pToolbar = &Dvrmaintoolbar;
    pSubToolbar = &Dvrsecondtoolbar;
    pSetup = &Dvrmainsetupproperty;
    pLoginDlg = &Dvrlogin;
    pNotiDlg = &Dvrnotifydialog;
    pEvtDlg = &Dvreventviewer;

    m_systemtimerstarted=false;
    bBlockKey=false;
    refreshdiskinfo=true;

    Dvrmaintimer = NULL;
    Dvrmainlayouttimer=NULL;
    Dvrsublayouttimer=NULL;
    m_bLogin=false;
    m_bDisplayPlaybackTimebar=true;
    m_iPlaybackTimebarUpdateDisableCount=0;
#if	DVR_API_ENABLE
    LIB816x_setDIOCallback((void*)dvrmain_dio_callback);
#endif
	//profile struct에 따라 초기화
    InitProfile(false);
    if(profile.layout.iMainOutput)
    {
    	//display output switch
        SwitchFB(profile.layout.iMainOutput);
    }

	//Timer start
    DvrEvttimer = new QTimer(this);
    DvrMotionChecktimer = new QTimer(this);
    connect(DvrEvttimer, SIGNAL(timeout()), this, SLOT(EvtHandler()));
    connect(DvrMotionChecktimer, SIGNAL(timeout()), this, SLOT(EvtMotionHandler()));
    DvrEvttimer->start(100);
    DvrMotionChecktimer->start(300);

    MainTimerStart();
    if(profile.sequence.iSequenceModeOn)
    {
        MainlayoutTimerStart();
    }
    if(profile.secondsequence.iSequenceModeOn)
    {
        SublayoutTimerStart();
    }

	//각각의 control들 초기화
    ui->setupUi(this);
    virtualKeyBoard = new WidgetKeyboard(this);
    Dvrmainlayout.setParent(this);
    Dvrmaintoolbar.setParent(this);
    Dvrsecondtoolbar.setParent(this);
    Dvrmainstatusbar.setParent(this);
    Dvrmainsidebar.setParent(this);
    Dvrmainplaybacktimelinebar.setParent(this);
    Dvrmainsetupproperty.setParent(this);
    Dvrlogin.setParent(this);
    Dvrnotifydialog.setParent(this);
    Dvreventviewer.setParent(this);
    Dvrmainplaybacktimelinebar.hide();
    Dvrnotifydialog.hide();
    Dvreventviewer.hide();
    Dvrmainsetupproperty.hide();
    Dvrsecondtoolbar.hide();

	//각각의 control들 좌표설정
    coordinator.MainWindow.setX(MAINWINDOWFULL_X);
    coordinator.MainWindow.setY(MAINWINDOWFULL_Y);
    coordinator.MainWindow.setWidth(MAINWINDOWFULL_W);
    coordinator.MainWindow.setHeight(MAINWINDOWFULL_H);
    coordinator.Sidebar.setRect(MAINSIDEBARFULL_X,MAINSIDEBARFULL_Y,MAINSIDEBARFULL_W,MAINSIDEBARFULL_H);
    coordinator.MainToolbar.setRect(MAINTOOLBARFULL_X,MAINTOOLBARFULL_Y,MAINTOOLBARFULL_W,MAINTOOLBARFULL_H);
    coordinator.SecondToolbar.setRect(SUBTOOLBARFULL_X,SUBTOOLBARFULL_Y,SUBTOOLBARFULL_W,SUBTOOLBARFULL_H);
    coordinator.Statusbar.setRect(MAINSTATUSBARFULL_X,MAINSTATUSBARFULL_Y,MAINSTATUSBARFULL_W,MAINSTATUSBARFULL_H);
    coordinator.TimeLinebar.setRect(TIMELINEBARFULL_X,TIMELINEBARFULL_Y,TIMELINEBARFULL_W,TIMELINEBARFULL_H);
    runtimesettings.iLayOutWindowX=LAYOUTWINDOWFULL_X;
    runtimesettings.iLayOutWindowY=LAYOUTWINDOWFULL_Y;
    runtimesettings.iLayOutWindowWidth=LAYOUTWINDOWFULL_W;
    runtimesettings.iLayOutWindowHeight=LAYOUTWINDOWFULL_H;
    setGeometry (APPWINDOW_X,APPWINDOW_Y,APPWINDOW_W,APPWINDOW_H);
    Dvrmainlayout.SetMainWindowSize();
    Dvrmaintoolbar.SetMainWindowSize();
    Dvrsecondtoolbar.SetMainWindowSize();
    Dvrnotifydialog.SetMainWindowSize();
    Dvrmainstatusbar.SetMainWindowSize();
    Dvreventviewer.SetMainWindowSize();
    Dvrmainplaybacktimelinebar.SetMainWindowSize();
    Dvrmainsetupproperty.SetMainWindowSize();
    Dvrmainsidebar.init();

    //Live Mode 설정
    SetAudioChannel(false,profile.iSelectChannel);
    Dvrmainsidebar.setMode();
    Dvrmaintoolbar.SetLivePlayBackBtnSelect(LIB_LIVE_MODE);
    Dvrmainlayout.DrawOSDChannelLive();
	//Layout mode에 따른 Max Page 계산
    Dvrmaintoolbar.MaxPageCalc();
    Dvrsecondtoolbar.MaxPageCalc();

    Dvrmainlayout.SetBorderLineShowHide(profile.mainosd.iBorderLineOn);
    Dvrmainstatusbar.SetDateFormat(profile.mainosd.iDateFormat);

    //1개월 이상된 로그 파일 삭제
    EraseOldEventLog(curtime.currenttime.tm_year, curtime.currenttime.tm_mon, curtime.currenttime.tm_mday);

    LoginStart();
#if DVR_API_ENABLE
    if(profile.net_info1.enable) {
        //RTSP Start
        LIB816x_startRTSP();
    }
    LIB816x_Vdis_start();
#endif
    //QWSServer::setCursorVisible(TRUE);
    QCursor cursor;
    cursor.setPos(coordinator.MainWindow.x() + coordinator.MainWindow.width()/2,coordinator.MainWindow.y()+coordinator.MainWindow.height()/2);
}
/*!destructor */
dvrmain::~dvrmain()
{
    MainTimerStop();
    MainlayoutTimerStop();
    SublayoutTimerStop();
    if(virtualKeyBoard)
        delete virtualKeyBoard;
    delete ui;
}
/*!부팅시 profile에 따른 초기화를 수행하거나 system config init시 profile struct를 Default value로 초기화 */
void dvrmain::InitProfile(bool forceReset)
{
    int i=0;
    int ret=0;
    GetCurrentTime(&curtime.currenttime);
	//user info load
    memset(&current_user,0,sizeof(USER));

    int userlistreadsize=0;
    if(!forceReset)
    {
        userlistreadsize=ReadDVRsettingInfo((char*)USERLISTFILNAME,&userlist,sizeof(USERLIST));
    }

    if((userlistreadsize==0)||(userlistreadsize!=sizeof(USERLIST)))
    {
        DeleteDVRsettingInfo((char*)USERLISTFILNAME);

        memset(&userlist,0,sizeof(USERLIST));
        userlist.usercount=1;
        userlist.userlist[0].iEnable=1;
        userlist.userlist[0].iAccessLevel=USERLEVEL_ADMIN;
        userlist.userlist[0].iAccessContents|=USERACCESS_LIVEMENU;
        userlist.userlist[0].iAccessContents|=USERACCESS_PLAYBACK;
        userlist.userlist[0].iAccessContents|=USERACCESS_MAINMENU;
        userlist.userlist[0].iAccessContents|=USERACCESS_SETUPMENU;
        sprintf(userlist.userlist[0].password,"%s",DEFAULT_PASSWORD);
    }

	//system info load
    if(forceReset)
    {
        DeleteDVRsettingInfo((char*)PROFILEFILNAME);

        memset(&profile,0,sizeof(PROFILE));

        profile.layout.iMainOutput = DISP_PATH_HDMI0;
        profile.layout.iSubOutput = DISP_PATH_HDMI1;
        profile.iSpotOutputCh=0;

        profile.layout.iLayoutMode=LAYOUTMODE_16;
        profile.layout.iSecondLayoutMode=LAYOUTMODE_16;
        profile.iSubOutputEnable = 1;
        profile.sequence.iSequenceModeOn=0;
        profile.secondsequence.iSequenceModeOn=0;
        profile.sequence.iInterval=DEFAULT_SEQUENCE_INTERVAL;
        profile.secondsequence.iInterval=DEFAULT_SEQUENCE_INTERVAL;

        profile.mainosd.iBorderLineOn=BORDERLINE_ON;
        profile.mainosd.iDateFormat=OSD_DATEFORMAT_MMDDYYYY_AP;

        profile.spotdisplayoutput.iResolution=OUTPUT_RESOLUTION_NTSC;
        profile.maindisplayoutput.iResolution=OUTPUT_RESOLUTION_1080P;
        profile.subdisplayoutput.iResolution=OUTPUT_RESOLUTION_1080P;

        for(i=0;i<MAX_INTERNAL_CAMERA;i++)
        {
            profile.camera[i].iEnable=1;
            profile.camera[i].iCameraType=CAMERA_TYPE_INTERNAL;
            profile.camera[i].iCovertOn=0;
            profile.camera[i].coloradjust.iBrightness=BRIGHTNESS_VALUE_DEFAULT;
            profile.camera[i].coloradjust.iContrast=CONTRAST_VALUE_DEFAULT;
            profile.camera[i].coloradjust.iSaturation=SATURATION_VALUE_DEFAULT;

            sprintf(profile.camera[i].strcameraname,"%s%d",STRCAMERANAME,i+1);

            profile.playbackcamera[i].iEnable=1;

            profile.camera[i].ptzinfo.iPtzOn=0;
            profile.camera[i].ptzinfo.iPtzdriverIndex=PTZCTRL_DRX_500;
            profile.camera[i].ptzinfo.iPtzAddress=0;
            profile.camera[i].ptzinfo.iBaudRate=BAUDRATE_9600;
            profile.camera[i].ptzinfo.iDataBit=DATBIT_8;
            profile.camera[i].ptzinfo.iStopBit=STOPBIT_1;
            profile.camera[i].ptzinfo.iParityBit=PARITYBIT_NONE;

            profile.camera[i].camrec.iRecordingOn=1;
            profile.camera[i].camrec.iCodecType=CODECTYPE_H264;
            profile.camera[i].camrec.iResolution=RESOLUTION_D1;
            profile.camera[i].camrec.iFps=3;
            profile.camera[i].camrec.ikbps=1200;
            profile.camera[i].camrec.iBitrateType=BITRATE_TYPE_CBR;
            profile.camera[i].camrec.iIFrameInterval=IFRAME_INTERVAL_30;
            profile.camera[i].camrec.InputVideoSignalType=INPUT_SIGNALTYPE_NTSC;

            profile.camera[i].camrec.iAudioOn=1;
            profile.camera[i].camrec.iAudioCodecType=AUDIO_CODEC_G711;
            profile.camera[i].camrec.iAudioSamplingType=AUDIO_SAMPLERATE_16KHZ;

            profile.camera[i].recordduration.iPrevRecordOn=0;
            profile.camera[i].recordduration.iPrevRecordDuration=PREVRECORDDURATION_SEC_DEFAULT;
            profile.camera[i].recordduration.iPostRecordOn=0;
            profile.camera[i].recordduration.iPostRecordDuration=POSTRECORDDURATION_MINUTE_DEFALUT;

        }

        for(i=MAX_INTERNAL_CAMERA;i<MAX_CAMERA;i++)
        {
            profile.camera[i].iEnable=0;
            profile.camera[i].iCameraType=CAMERA_TYPE_IPNC;
            profile.camera[i].iCovertOn=0;
            profile.camera[i].coloradjust.iBrightness=BRIGHTNESS_VALUE_DEFAULT;
            profile.camera[i].coloradjust.iContrast=CONTRAST_VALUE_DEFAULT;
            profile.camera[i].coloradjust.iSaturation=SATURATION_VALUE_DEFAULT;
            sprintf(profile.camera[i].strcameraname,"%s%d",STRIPNCCAMERANAME,i%MAX_INTERNAL_CAMERA+1);

            profile.playbackcamera[i].iEnable=0;

            profile.camera[i].ptzinfo.iPtzOn=0;
            profile.camera[i].ptzinfo.iPtzdriverIndex=PTZCTRL_DRX_500;
            profile.camera[i].ptzinfo.iPtzAddress=0;
            profile.camera[i].ptzinfo.iBaudRate=BAUDRATE_9600;
            profile.camera[i].ptzinfo.iDataBit=DATBIT_8;
            profile.camera[i].ptzinfo.iStopBit=STOPBIT_1;
            profile.camera[i].ptzinfo.iParityBit=PARITYBIT_NONE;

            profile.camera[i].camrec.iRecordingOn=1;
            profile.camera[i].camrec.iCodecType=CODECTYPE_H264;
            profile.camera[i].camrec.iResolution=RESOLUTION_D1;
            profile.camera[i].camrec.iFps=3;
            profile.camera[i].camrec.ikbps=1200;
            profile.camera[i].camrec.iBitrateType=BITRATE_TYPE_CBR;
            profile.camera[i].camrec.iIFrameInterval=IFRAME_INTERVAL_30;
            profile.camera[i].camrec.InputVideoSignalType=INPUT_SIGNALTYPE_NTSC;

            profile.camera[i].camrec.iAudioOn=0;
            profile.camera[i].camrec.iAudioCodecType=AUDIO_CODEC_G711;
            profile.camera[i].camrec.iAudioSamplingType=AUDIO_SAMPLERATE_8KHZ;

            profile.camera[i].recordduration.iPrevRecordOn=0;
            profile.camera[i].recordduration.iPrevRecordDuration=PREVRECORDDURATION_SEC_DEFAULT;
            profile.camera[i].recordduration.iPostRecordOn=0;
            profile.camera[i].recordduration.iPostRecordDuration=POSTRECORDDURATION_MINUTE_DEFALUT;
        }

        profile.audioin.iSampleRate=AUDIO_SAMPLERATE_16KHZ;
        profile.audioin.iVolume=AUDIO_VOLUME_DEFAULT;
        for(i=0;i<MAX_AUDIOIN;i++)
        {
        profile.audioin.audioinch[i].iAudioInOn=1;

        }

        profile.audioout.iAudioOutOn=1;
        profile.audioout.iSelectAudioIn=0;
        profile.audioout.iVolume=AUDIO_VOLUME_DEFAULT;
        profile.audioout.iStereo=AUDIO_MONO;
        profile.audioout.iDevId=0;


        for(i=0;i<MAX_SENSOR;i++)
        {
            profile.sensor[i].iSensorOn=SENSOR_DETECT_OFF;
            profile.sensor[i].iSensorType=SENSOR_TYPE_NO;
        }

        for(i=0;i<MAX_ALARM;i++)
        {
            profile.alarm[i].iAlarmOn=ALARM_DETECT_OFF;
            profile.alarm[i].iAlarmType=ALARM_TYPE_NO;
            profile.alarm[i].iDelayTime=ALARM_DELAYSEC_DEFAULT;
        }

        for(i=0;i<MAX_INTERNAL_CAMERA;i++)
        {
            for(int iday=0;iday<7;iday++)
            {
                for(int ihour=0;ihour<24;ihour++)
                {
                    profile.camera[i].recschedule.ScheduleTable[iday][ihour] = REC_SCHEDULE_NORECORD;
                }
            }

        }

        profile.storage.iHddRecylceModeOn=STORAGE_WRITE_MODE_RECYCLE;
        profile.dvrall.iRecordEmergencyOn=0;

        profile.net_info1.type=NETWORK_TYPE_STATIC;
        strcpy(profile.net_info1.ipaddr, NETWORK_IPADDRESS_DEFAULT1);
        strcpy(profile.net_info1.netmask, NETWORK_SUBNETMASK_DEFAULT);
        strcpy(profile.net_info1.gateway, NETWORK_GATEWAY_DEFAULT);

        profile.net_info2.type=NETWORK_TYPE_STATIC;
        strcpy(profile.net_info2.ipaddr, NETWORK_IPADDRESS_DEFAULT2);
        strcpy(profile.net_info2.netmask, NETWORK_SUBNETMASK_DEFAULT);
        strcpy(profile.net_info2.gateway, NETWORK_GATEWAY_DEFAULT);

        for(i=0; i<MAX_IPNC_CAMERA; i++)
        {
            strcpy(profile.ipnc[i].IpAddress, NETWORK_IPADDRESS_DEFAULT1);
            profile.ipnc[i].port = 0;
            memset(profile.ipnc[i].id,'\0',sizeof(profile.ipnc[i].id));
            memset(profile.ipnc[i].password,'\0',sizeof(profile.ipnc[i].password));
        }

        profile.iSelectChannel = 0;
        profile.iSecondSelectChannel = 0;

        WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));
    }

    if(profile.layout.iMainOutput == profile.layout.iSubOutput)
    {
        profile.iSubOutputEnable=0;
        if(profile.layout.iMainOutput==DISP_PATH_HDMI0)
        {
            runtimesettings.OutputResolution[DISP_PATH_HDMI0] = profile.maindisplayoutput.iResolution;
            runtimesettings.OutputResolution[DISP_PATH_HDMI1] = profile.subdisplayoutput.iResolution;
        }
        else
        {
            runtimesettings.OutputResolution[DISP_PATH_HDMI1] = profile.maindisplayoutput.iResolution;
            runtimesettings.OutputResolution[DISP_PATH_HDMI0] = profile.subdisplayoutput.iResolution;
        }
    }
    else
    {
        profile.iSubOutputEnable=1;
        runtimesettings.OutputResolution[profile.layout.iMainOutput] = profile.maindisplayoutput.iResolution;
        runtimesettings.OutputResolution[profile.layout.iSubOutput] = profile.subdisplayoutput.iResolution;
    }

    if(profile.layout.iMainOutput == DISP_PATH_HDMI1)
    {
        runtimesettings.iMainSwapEnable = 1;
    }
    else
    {
        runtimesettings.iMainSwapEnable = 0;
    }

    memcpy(&runtimesettings.backupinfo.tmFromBackupTime, &curtime.currenttime, sizeof(struct tm));
    memcpy(&runtimesettings.backupinfo.tmToBackupTime, &curtime.currenttime, sizeof(struct tm));
    for(i=0;i<MAX_CAMERA;i++)
        runtimesettings.backupinfo.iDestChannels[i]=0;
    runtimesettings.backupinfo.iBackupFileType = 0;
    runtimesettings.backupinfo.iBackupMediaType = 0;

    ret = Dvrmainsetupproperty.get_sys_info();
    if(ret)
    {
        strcpy(runtimesettings.versioninfo.FwVer,STRUNKNOWN);
        strcpy(runtimesettings.versioninfo.HwVer,STRUNKNOWN);
        strcpy(runtimesettings.versioninfo.MacAddr1,STRUNKNOWN);
        strcpy(runtimesettings.versioninfo.MacAddr2,STRUNKNOWN);
    }

    Dvrmainsetupproperty.get_disk_info();

    if(g_rfs_type!=RFS_NFS)
    {
        if(profile.net_info1.type==NETWORK_TYPE_STATIC && strcmp(profile.net_info1.ipaddr, NETWORK_IPADDRESS_DEFAULT1))
                Dvrmainsetupproperty.set_network_info(NETWORK_DEV1_NO);
    }
    else {
        Dvrmainsetupproperty.ChangeNetworkType1SelectSlot(NETWORK_TYPE_DHCP);	//# temporary
    }

    if(profile.net_info2.type==NETWORK_TYPE_STATIC && strcmp(profile.net_info2.ipaddr, NETWORK_IPADDRESS_DEFAULT2))
    {
        Dvrmainsetupproperty.set_network_info(NETWORK_DEV2_NO);
    }
    Dvrmainsetupproperty.get_network_info();
    Dvrmainsetupproperty.SetNetworkdata();

    runtimesettings.iLivePlaybackMode=LIB_LIVE_MODE;
    runtimesettings.playbackinfo.playbackmode.iPlaybackMode=PLAYBACKMODE_STOP;
    runtimesettings.playbackinfo.playbackmode.iSpeed=PLAYBACKSPEED_1X;
    runtimesettings.iSetupPropertyOn=SETUPPROPERTY_OFF;
    runtimesettings.iConsoleModeOn=CONSOLEMODE_ON;
    runtimesettings.iSecondConsoleModeOn=SUBCONSOLEMODE_OFF;
    runtimesettings.iPageNum=0;
    runtimesettings.iSecondPageNum=0;
    switch(profile.layout.iLayoutMode)
    {
        case LAYOUTMODE_1:
            runtimesettings.iLayout=LAYOUT_1;
            break;
        case LAYOUTMODE_4:
            runtimesettings.iLayout=LAYOUT_4;
            break;
        case LAYOUTMODE_9:
            runtimesettings.iLayout=LAYOUT_9;
            break;
        case LAYOUTMODE_8:
        case LAYOUTMODE_8_1:
            runtimesettings.iLayout=LAYOUT_8;
            break;
        case LAYOUTMODE_12:
        case LAYOUTMODE_12_1:
            runtimesettings.iLayout=LAYOUT_12;
            break;
        case LAYOUTMODE_14:
            runtimesettings.iLayout=LAYOUT_14;
            break;
        case LAYOUTMODE_16:
            runtimesettings.iLayout=LAYOUT_16;
            break;
    }
    switch(profile.layout.iSecondLayoutMode)
    {
        case LAYOUTMODE_1:
            runtimesettings.iSecondLayout=LAYOUT_1;
            break;
        case LAYOUTMODE_4:
            runtimesettings.iSecondLayout=LAYOUT_4;
            break;
        case LAYOUTMODE_9:
            runtimesettings.iSecondLayout=LAYOUT_9;
            break;
        case LAYOUTMODE_8:
        case LAYOUTMODE_8_1:
            runtimesettings.iSecondLayout=LAYOUT_8;
            break;
        case LAYOUTMODE_12:
        case LAYOUTMODE_12_1:
            runtimesettings.iSecondLayout=LAYOUT_12;
            break;
        case LAYOUTMODE_14:
            runtimesettings.iSecondLayout=LAYOUT_14;
            break;
        case LAYOUTMODE_16:
            runtimesettings.iSecondLayout=LAYOUT_16;
            break;
    }
    runtimesettings.playbacktable.iTimeTableZoomRate=PLAYBACKTIMELINE_ZOOM_24;
    runtimesettings.playbacktable.iTimeTableTimeRate=0;
    runtimesettings.playbacktable.iTimeTablePageChannel=0;
    memset(runtimesettings.chStatus,0,sizeof(runtimesettings.chStatus));
#if DVR_API_ENABLE
    int nChannelCount;
    LIB816x_getSourceStatus(&nChannelCount, runtimesettings.chStatus);
    for(i=0;i<MAX_INTERNAL_CAMERA;i++)
    {
        if(profile.camera[i].iEnable && profile.camera[i].iCovertOn)
    	{
            LIB816x_setCovert(i, profile.camera[i].iCovertOn);
    	}
    }
#endif

    GetBuildDateTime(runtimesettings.versioninfo.BuildDateTime);

    memset(&detectstatus,0,sizeof(DETECTSTATUS));

    if(forceReset==false)
        memcpy(&dlgdata,&profile,sizeof(PROFILE));
    m_iAudioOutChannel = profile.iSelectChannel;
}
/*!login dialog 표시하고 다른 control은 disable시킴 */
void dvrmain::LoginStart()
{
	//login창 표시, 기타 contorl들 disable
    Dvrlogin.setFocus();
    Dvrlogin.DvrLoginShow();
    Dvrmaintoolbar.setEnabled(false);
    Dvrmainsidebar.setEnabled(false);
    Dvrsecondtoolbar.setEnabled(false);
    Dvrmainplaybacktimelinebar.setEnabled(false);
    Dvrmaintoolbar.SetEnableSetupBtn(0);
    m_bLogin=false;

}
/*!login이 된경우 */
void dvrmain::LoginStop()
{
    m_bLogin = true;
    setFocus();
}
/*!logout으로 설정변경 */
void dvrmain::Logout()
{
    EventLogAdd((char*)"Log-out",true);
    m_bLogin = false;
}
/*!Main Timer를 Start 시킴*/
void dvrmain::MainTimerStart()
{
    if(Dvrmaintimer==NULL)
    {
        Dvrmaintimer = new QTimer(this);
        connect(Dvrmaintimer, SIGNAL(timeout()), this, SLOT(MainTimerUpdate()));
        Dvrmaintimer->start(MAINTIMER_STEPVALUE);
    }
}
/*!Main Timer를 종료 시킴*/
void dvrmain::MainTimerStop()
{
    if(Dvrmaintimer)
    {
        Dvrmaintimer->stop();
        delete Dvrmaintimer;
        Dvrmaintimer=NULL;
    }
}
/*!Main laout sequence mode일때 Start시킴*/
void dvrmain::MainlayoutTimerStart()
{
    if(Dvrmainlayouttimer==NULL)
    {
        Dvrmainlayouttimer = new QTimer(this);
        connect(Dvrmainlayouttimer, SIGNAL(timeout()), this, SLOT(MainlayoutTimerUpdate()));
        Dvrmainlayouttimer->start(profile.sequence.iInterval*1000);
    }
}
/*!Main layout sequence mode off일때 종료 시킴*/
void dvrmain::MainlayoutTimerStop()
{
    if(Dvrmainlayouttimer)
    {
        Dvrmainlayouttimer->stop();
        delete Dvrmainlayouttimer;
        Dvrmainlayouttimer=NULL;
    }
}
/*!sub layout sequence mode일때 start 시킴*/
void dvrmain::SublayoutTimerStart()
{
    if(Dvrsublayouttimer == NULL)
    {
        Dvrsublayouttimer = new QTimer(this);
        connect(Dvrsublayouttimer, SIGNAL(timeout()), this, SLOT(SublayoutTimerUpdate()));
        Dvrsublayouttimer->start(profile.secondsequence.iInterval*1000);
    }
}
/*!sub layout sequence mode off일때 종료 시킴*/
void dvrmain::SublayoutTimerStop()
{
    if(Dvrsublayouttimer)
    {
        Dvrsublayouttimer->stop();
        delete Dvrsublayouttimer;
        Dvrsublayouttimer=NULL;
    }
}
/*!system 종료, format 전에 호출되어 playback, record등을 종료시킴*/
void dvrmain::BeforeTerminateOperation()
{
	//system 종료, format시 record, playback을 종료시킴
    //save profile
    WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));

    //stop playback
#if DVR_API_ENABLE
    if(runtimesettings.playbackinfo.playbackmode.iPlaybackMode != PLAYBACKMODE_STOP)
        LIB816x_stopPlayback_x();
#endif

    //stop record
    for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
    {
        if(detectstatus.detectrecording[i].iRecordingOn)
        {
#if DVR_API_ENABLE
            LIB816x_setRecChannel(i, 0, 0, 0 , " ");
#endif
        }
    }
    memset(&detectstatus,0,sizeof(DETECTSTATUS));
}

/*!Main/sub display의 device를 변경할때 QT를 해당 FB로 connect*/
void dvrmain::SwitchFB(int DisplayId)
{
	//display switch시 fb0, fb1을 번갈아가며 연결
    int resolution;
#if DVR_API_ENABLE
/*    QWSServer *server = QWSServer::instance();
    QScreen *screen = QScreen::instance();
    if(DisplayId==0)
    {
        server->enablePainting(false);
        screen->shutdownDevice();
        screen->disconnect();
        screen->connect("/dev/fb0");
        screen->initDevice();
        server->enablePainting(true);
        server->refresh();
    }
    else
    {
        server->enablePainting(false);
        screen->shutdownDevice();
        screen->disconnect();
        screen->connect("/dev/fb1");
        screen->initDevice();
        server->enablePainting(true);
        server->refresh();
    }*/
#endif

}
/*!Emergency Record를 On/Off*/
void dvrmain::EmergencyRecordAll()
{
    int i=0;
    if(profile.dvrall.iRecordEmergencyOn==0)
    {
    	//Emergency Record stop
        EventLogAdd((char*)"Emergency Record Stop",true);
        for(i=0;i<MAX_INTERNAL_CAMERA;i++)
        {
            int ret=0;
            if(profile.camera[i].iEnable==1)
            {
                if(profile.camera[i].camrec.iRecordingOn==1)
                {
                    if(detectstatus.detectrecording[i].iEmergency)
                    {
                        detectstatus.detectrecording[i].iEmergency = 0;
                        detectstatus.detectrecording[i].iRecordingOn=0;
#if DVR_API_ENABLE
                        ret=LIB816x_setRecChannel(i, 0, 0, 0 , " ");
#else
                        ret=1;
#endif
                        if(!ret)
                        {
                            EventLogAdd((char*)"Emergency Record Stop Fail",true);
                            ShowNotifyDialog((char*)"Emergency Record Stop Fail");
                        }
                    }
                }
                else
                {
                    detectstatus.detectrecording[i].iRecordingOn=0;
                }
            }
        }
        //Recording schedule에 따라서 Record 수행
        RecScheduleHandler(curtime.currenttime.tm_wday,curtime.currenttime.tm_hour);
    }
    else
    {
    	//Emergency Record start
        EventLogAdd((char*)"Emergency Record Start",true);
        for(i=0;i<MAX_INTERNAL_CAMERA;i++)
        {
            int ret=0;
            if(profile.camera[i].iEnable==1)
            {
                if(profile.camera[i].camrec.iRecordingOn==1)
                {
                    if(detectstatus.detectrecording[i].iRecordingOn)
                    {
                        detectstatus.detectrecording[i].iNormal=0;
                        detectstatus.detectrecording[i].iMotion=0;
                        detectstatus.detectrecording[i].iSensor=0;
                        detectstatus.detectrecording[i].iVideoloss=0;
                    }
#if DVR_API_ENABLE
                    ret=LIB816x_setRecChannel(i, 1, LIB_EVENT_REC_EMG, profile.audioin.audioinch[i].iAudioInOn && profile.camera[i].camrec.iAudioOn , profile.camera[i].strcameraname);
#else
                    ret=1;
#endif
                    if(ret)
                    {
                        detectstatus.detectrecording[i].iEmergency = 1;
                        detectstatus.detectrecording[i].iRecordingOn=1;
                    }
                    else
                    {
                        EventLogAdd((char*)"Emergency Record Start Fail",true);
                        ShowNotifyDialog((char*)"Emergency Record Start Fail");
                    }
                }
                else
                {
                    detectstatus.detectrecording[i].iRecordingOn=0;
                }
            }
        }
    }
    WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));

    Dvrmainlayout.DrawOSDIcon();
}
/*!Camera별 Recording Schedule을 check하고 그에따른 동작을 수행*/
void dvrmain::RecScheduleHandler(int day,int hour)
{
    if(profile.dvrall.iRecordEmergencyOn)
    {
    	//Emergency Record 중일때 return
        Dvrmainlayout.DrawOSDIcon();
        return;
    }
    char str[100];

    for(int camidx=0;camidx<MAX_INTERNAL_CAMERA;camidx++)
    {
    	//Camera별 Record schedule에 따라서 처리
    	//record event가 다른경우 Record종료후 현재 schedule에 따라서 Recording 작업수행
        int ret=0;
        runtimesettings.curRecSchedule[camidx] = profile.camera[camidx].recschedule.ScheduleTable[day][hour];
        int audflag;
        if(camidx<MAX_AUDIOIN)
        {
            audflag = profile.audioin.audioinch[camidx].iAudioInOn && profile.camera[camidx].camrec.iAudioOn;
        }
        else
        {
            audflag = 0;
        }

        if(runtimesettings.curRecSchedule[camidx] == REC_SCHEDULE_CONTINUOUSRECORD)
        {
            if(profile.camera[camidx].iEnable && profile.camera[camidx].camrec.iRecordingOn)
            {
                if(detectstatus.detectrecording[camidx].iNormal == 0)
                {
#if DVR_API_ENABLE
                    sprintf(str,"Schedule Recording Start for %s",profile.camera[camidx].strcameraname);
                    EventLogAdd(str,true);
                    ret=LIB816x_setRecChannel(camidx, 1, LIB_EVENT_REC_CONT, audflag , profile.camera[camidx].strcameraname);
#else
                    ret=1;
#endif
                    if(ret)
                    {
                        detectstatus.detectrecording[camidx].iNormal = 1;
                        if(detectstatus.detectrecording[camidx].iRecordingOn==0)
                        {
                            detectstatus.detectrecording[camidx].iRecordingOn=1;
                        }
                    }
                    else
                    {
                        sprintf(str,"Schedule Recording Start for %s fail",profile.camera[camidx].strcameraname);
                        EventLogAdd(str,true);
                    }
                }
                detectstatus.detectrecording[camidx].iMotion=0;
                detectstatus.detectrecording[camidx].iSensor=0;
                detectstatus.detectrecording[camidx].iVideoloss=0;
            }
            else
            {
            	detectstatus.detectrecording[camidx].iNormal = 0;
                detectstatus.detectrecording[camidx].iRecordingOn=0;
            }
        }
        else if(runtimesettings.curRecSchedule[camidx] == REC_SCHEDULE_RECORDBYMOTION)
        {
            if(detectstatus.detectrecording[camidx].iNormal
               || detectstatus.detectrecording[camidx].iSensor
               || detectstatus.detectrecording[camidx].iVideoloss)
            {
                if(detectstatus.detectrecording[camidx].iNormal)
                {
                    sprintf(str,"Schedule Recording Stop for %s fail",profile.camera[camidx].strcameraname);
                    detectstatus.detectrecording[camidx].iNormal=0;
                }
                if(detectstatus.detectrecording[camidx].iSensor)
                {
                    sprintf(str,"Senor Recording Stop for %s",profile.camera[camidx].strcameraname);
                    detectstatus.detectrecording[camidx].iSensor=0;
                }
                if(detectstatus.detectrecording[camidx].iVideoloss)
                {
                    sprintf(str,"Video loss Recording Stop for %s",profile.camera[camidx].strcameraname);
                    detectstatus.detectrecording[camidx].iVideoloss=0;
                }

#if DVR_API_ENABLE
                EventLogAdd(str,true);
                ret=LIB816x_setRecChannel(camidx, 0, 0, 0 , " ");
#endif
            }
        }
        else if(runtimesettings.curRecSchedule[camidx] == REC_SCHEDULE_RECORDBYSENSOR)
        {
            if(detectstatus.detectrecording[camidx].iNormal
               || detectstatus.detectrecording[camidx].iMotion
               || detectstatus.detectrecording[camidx].iVideoloss)
            {
                if(detectstatus.detectrecording[camidx].iNormal)
                {
                    sprintf(str,"Schedule Recording Stop for %s",profile.camera[camidx].strcameraname);
                    detectstatus.detectrecording[camidx].iNormal=0;
                }
                if(detectstatus.detectrecording[camidx].iMotion)
                {
                    sprintf(str,"Motion Recording Stop for %s",profile.camera[camidx].strcameraname);
                    detectstatus.detectrecording[camidx].iMotion=0;
                }
                if(detectstatus.detectrecording[camidx].iVideoloss)
                {
                    sprintf(str,"Video loss Recording Stop for %s",profile.camera[camidx].strcameraname);
                    detectstatus.detectrecording[camidx].iVideoloss=0;
                }
#if DVR_API_ENABLE
                EventLogAdd(str,true);
                ret=LIB816x_setRecChannel(camidx, 0, 0, 0 , " ");
#endif
            }
        }
        else if(runtimesettings.curRecSchedule[camidx] == REC_SCHEDULE_RECORDBYVLOSS)
        {
            if(detectstatus.detectrecording[camidx].iNormal
               || detectstatus.detectrecording[camidx].iMotion
               || detectstatus.detectrecording[camidx].iSensor)
            {
                if(detectstatus.detectrecording[camidx].iNormal)
                {
                    sprintf(str,"Schedule Recording Stop for %s",profile.camera[camidx].strcameraname);
                    detectstatus.detectrecording[camidx].iNormal=0;
                }
                if(detectstatus.detectrecording[camidx].iMotion)
                {
                    sprintf(str,"Motion Recording Stop for %s",profile.camera[camidx].strcameraname);
                    detectstatus.detectrecording[camidx].iMotion=0;
                }
                if(detectstatus.detectrecording[camidx].iSensor)
                {
                    sprintf(str,"Sensor Recording Stop for %s",profile.camera[camidx].strcameraname);
                    detectstatus.detectrecording[camidx].iSensor=0;
                }
#if DVR_API_ENABLE
                EventLogAdd(str,true);
                ret=LIB816x_setRecChannel(camidx, 0, 0, 0 , " ");
#endif
            }
        }
        else if(runtimesettings.curRecSchedule[camidx] == REC_SCHEDULE_NORECORD)
        {
            if(detectstatus.detectrecording[camidx].iNormal
               || detectstatus.detectrecording[camidx].iMotion
               || detectstatus.detectrecording[camidx].iSensor
               || detectstatus.detectrecording[camidx].iVideoloss)
            {
                if(detectstatus.detectrecording[camidx].iNormal)
                {
                    sprintf(str,"Schedule Recording Stop for %s",profile.camera[camidx].strcameraname);
                    detectstatus.detectrecording[camidx].iNormal=0;
                }
                if(detectstatus.detectrecording[camidx].iMotion)
                {
                    sprintf(str,"Motion Recording Stop for %s",profile.camera[camidx].strcameraname);
                    detectstatus.detectrecording[camidx].iMotion=0;
                }
                if(detectstatus.detectrecording[camidx].iSensor)
                {
                    sprintf(str,"Sensor Recording Stop for %s",profile.camera[camidx].strcameraname);
                    detectstatus.detectrecording[camidx].iSensor=0;
                }
                if(detectstatus.detectrecording[camidx].iVideoloss)
                {
                    sprintf(str,"Video loss Recording Stop for %s",profile.camera[camidx].strcameraname);
                    detectstatus.detectrecording[camidx].iVideoloss=0;
                }
#if DVR_API_ENABLE
                EventLogAdd(str,true);
                ret=LIB816x_setRecChannel(camidx, 0, 0, 0 , " ");
#endif
            }
        }

        if(detectstatus.detectrecording[camidx].iNormal==0 && detectstatus.detectrecording[camidx].iMotion == 0 && detectstatus.detectrecording[camidx].iSensor == 0 &&
           detectstatus.detectrecording[camidx].iVideoloss==0 && detectstatus.detectrecording[camidx].iEmergency == 0)
        {
            detectstatus.detectrecording[camidx].iRecordingOn=0;
        }
    }
    Dvrmainlayout.DrawOSDIcon();
}
/*!Motion event발생시 그에따른 Recording/Alarm 동작을 수행*/
void dvrmain::MotionEventHandler(int motionidx,int detect)
{
    if(motionidx>=MAX_INTERNAL_CAMERA)
        return;

    char str[100];
    time_t timeval ;

    time(&timeval) ;

    detectstatus.detectmotion[motionidx].iMotionDetectOn=detect;

    //Motion - Record Event
    if(detect)
    {
        for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
        {
            int ret=0;
            //event link, record schedule, record enable에 따라서 Record Start
            if(profile.camera[i].iEnable && profile.camera[i].camrec.iRecordingOn && profile.eventlink.iMotionRecordingLink[motionidx][i] && runtimesettings.curRecSchedule[i] == REC_SCHEDULE_RECORDBYMOTION)
            {
            	//현재 recording 중인경우 post recording timer를 update
                if(runtimesettings.postrecinfo[i].RecReason==LIB_EVENT_REC_MOT)
                {
                    DvrEvent *qMsg = new DvrEvent;
                    qMsg->cmd = EVENT_CMD_POSTREC_CLEAR;
                    qMsg->param[0]=i;
                    RecEvtQ.enqueue((void*)qMsg);
                    runtimesettings.postrecinfo[i].endtime = timeval;
                    runtimesettings.postrecinfo[i].RecReason=0;
                }
                int audflag;
                if(i<MAX_AUDIOIN)
                {
                    audflag = profile.audioin.audioinch[i].iAudioInOn && profile.camera[i].camrec.iAudioOn;
                }
                else
                {
                    audflag = 0;
                }

				//현재 recording 중이 아닌경우 Record start
                if(detectstatus.detectrecording[i].iMotion == 0)
                {
#if DVR_API_ENABLE
                    sprintf(str,"Motion Recording Start for %s",profile.camera[i].strcameraname);
                    EventLogAdd(str,true);
                    ret=LIB816x_setRecChannel(i, 1, LIB_EVENT_REC_MOT, audflag , profile.camera[i].strcameraname);
#else
                    ret=1;
#endif
                    if(ret)
                    {
                        detectstatus.detectrecording[i].iMotion = 1;
                        if(detectstatus.detectrecording[i].iRecordingOn==0)
                            detectstatus.detectrecording[i].iRecordingOn=1;
                    }
                    else
                    {
                        sprintf(str,"Motion Recording Start for %s fail",profile.camera[i].strcameraname);
                        EventLogAdd(str,true);
                    }
                }
            }
            else if(profile.camera[i].camrec.iRecordingOn==0)
            {
                detectstatus.detectrecording[i].iRecordingOn=0;
            }
        }
    }
    else
    {
    	//Motion Event 해제시
        for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
        {
            int ret=0;
            if(detectstatus.detectrecording[i].iMotion)
            {
                bool bFindSet = false;
                for(int iMotion=0;iMotion<MAX_INTERNAL_CAMERA;iMotion++)
                {
                	//다른 camera의 Event가 설정되어있는지 event link에 따라서 검색
                    if(detectstatus.detectmotion[iMotion].iMotionDetectOn && profile.eventlink.iMotionRecordingLink[iMotion][i])
                    {
                        bFindSet = true;
                        break;
                    }
                }
                if(bFindSet == false)
                {
                	//post rec 설정이 된 경우 post record timer update
                	//post rec 설정이 되지 않은 경우 record stop
                    if(detectstatus.detectrecording[i].iMotion == 1 && profile.camera[i].recordduration.iPostRecordOn == 0)
                    {
#if DVR_API_ENABLE
                        sprintf(str,"Motion Recording Stop for %s",profile.camera[i].strcameraname);
                        EventLogAdd(str,true);
                        ret=LIB816x_setRecChannel(i, 0, 0, 0 , " ");
#else
                        ret=1;
#endif
                        if(ret)
                        {
                            detectstatus.detectrecording[i].iMotion = 0;
                        }
                        else
                        {
                            sprintf(str,"Motion Recording Stop for %s fail",profile.camera[i].strcameraname);
                            EventLogAdd(str,true);
                        }
                    }
                    else if(detectstatus.detectrecording[i].iMotion == 1 && profile.eventlink.iMotionRecordingLink[motionidx][i])
                    {
                        runtimesettings.postrecinfo[i].RecReason =  LIB_EVENT_REC_MOT;
                        runtimesettings.postrecinfo[i].endtime = timeval;
                        runtimesettings.postrecinfo[i].duration = profile.camera[i].recordduration.iPostRecordDuration * 60;
                        DvrEvent *qMsg = new DvrEvent;
                        qMsg->cmd = EVENT_CMD_POSTREC_SET;
                        qMsg->param[0]=i;
                        RecEvtQ.enqueue((void*)qMsg);
                    }
                }
            }
            if(detectstatus.detectrecording[i].iNormal == 0 && detectstatus.detectrecording[i].iMotion == 0 && detectstatus.detectrecording[i].iSensor == 0 &&
               detectstatus.detectrecording[i].iVideoloss == 0 && detectstatus.detectrecording[i].iEmergency == 0)
            {
                detectstatus.detectrecording[i].iRecordingOn=0;
            }
        }
    }

    //Motion - Alarm Event
    if(detect)
    {
    	//Event alram link에 따라서 Alarm Set
        for(int i=0;i<MAX_ALARM;i++)
        {
            if(profile.alarm[i].iAlarmOn && profile.eventlink.iMotionAlarmLink[motionidx][i])
            {
                if(detectstatus.detectalarm[i].iAlarmDetectOn==0)
                {
                    detectstatus.detectalarm[i].iAlarmDetectOn=1;
                    Dvrmainsidebar.setAlarmIconStatus(i,1);
                    sprintf(str,"Alarm(%d) set by motion detection event of %s",i,profile.camera[motionidx].strcameraname);
                    EventLogAdd(str,true);
                }
#if DVR_API_ENABLE
                LIB816x_operateAlarm(i,1);
#endif
            }
        }
    }
    Dvrmainlayout.DrawOSDIcon();
}

/*!Sensor Event발생시 그에따른 Recording/Alram 동작을 수행*/
void dvrmain::SensorEventHandler(int sensoridx,int detect)
{
    if(sensoridx>=MAX_SENSOR)
        return;

    char str[100];
    time_t timeval ;

    time(&timeval) ;

    if(detectstatus.detectsensor[sensoridx].iSensorDetectOn != detect)
    {
        detectstatus.detectsensor[sensoridx].iSensorDetectOn=detect;
        Dvrmainsidebar.setSensorIconStatus(sensoridx,detect);
    }

    //Sensor - Record Event
    if(detect)
    {
        for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
        {
        	//event link, record schedule, record enable에 따라서 Record Start
            int ret=0;
            if(profile.camera[i].iEnable && profile.camera[i].camrec.iRecordingOn && profile.eventlink.iSensorRecordingLink[sensoridx][i] && runtimesettings.curRecSchedule[i] == REC_SCHEDULE_RECORDBYSENSOR)
            {
            	//현재 recording 중인경우 post recording timer를 update
                if(runtimesettings.postrecinfo[i].RecReason == LIB_EVENT_REC_SENSOR)
                {
                    runtimesettings.postrecinfo[i].endtime = timeval;
                    DvrEvent *qMsg = new DvrEvent;
                    qMsg->cmd = EVENT_CMD_POSTREC_CLEAR;
                    qMsg->param[0]=i;
                    RecEvtQ.enqueue((void*)qMsg);
                    runtimesettings.postrecinfo[i].RecReason = 0;
                }
				//현재 recording 중이 아닌경우 Record start
                if(detectstatus.detectrecording[i].iSensor == 0)
                {
                    int audflag;
                    if(i<MAX_AUDIOIN)
                    {
                        audflag = profile.audioin.audioinch[i].iAudioInOn && profile.camera[i].camrec.iAudioOn;
                    }
                    else
                    {
                        audflag = 0;
                    }
#if DVR_API_ENABLE
                    sprintf(str,"Sensor Recording Start for %s",profile.camera[i].strcameraname);
                    EventLogAdd(str,true);
                    ret=LIB816x_setRecChannel(i, 1, LIB_EVENT_REC_SENSOR, audflag , profile.camera[i].strcameraname);
#else
                    ret=1;
#endif
                    if(ret)
                    {
                        detectstatus.detectrecording[i].iSensor = 1;
                        if(detectstatus.detectrecording[i].iRecordingOn==0)
                            detectstatus.detectrecording[i].iRecordingOn=1;
                    }
                    else
                    {
                        sprintf(str,"Sensor Recording Start for %s fail",profile.camera[i].strcameraname);
                        EventLogAdd(str,true);
                    }
                }
            }
            else if(profile.camera[i].camrec.iRecordingOn==0)
            {
                detectstatus.detectrecording[i].iRecordingOn=0;
            }
        }
    }
    else
    {
        for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
        {
            int ret=0;
            if(detectstatus.detectrecording[i].iSensor)
            {
                bool bFindSet = false;
                //다른 camera의 Event가 설정되어있는지 event link에 따라서 검색
                for(int iSensor=0;iSensor<MAX_SENSOR;iSensor++)
                {
                    if(detectstatus.detectsensor[iSensor].iSensorDetectOn && profile.eventlink.iSensorRecordingLink[iSensor][i])
                    {
                        bFindSet = true;
                        break;
                    }
                }
                if(bFindSet == false)
                {
                	//post rec 설정이 된 경우 post record timer update
                	//post rec 설정이 되지 않은 경우 record stop
                    if(detectstatus.detectrecording[i].iSensor == 1  && profile.camera[i].recordduration.iPostRecordOn == 0)
                    {
#if DVR_API_ENABLE
                        sprintf(str,"Sensor Recording Stop for %s",profile.camera[i].strcameraname);
                        EventLogAdd(str,true);
                        ret=LIB816x_setRecChannel(i, 0, 0, 0 , " ");
#else
                        ret=1;
#endif
                        if(ret)
                        {
                            detectstatus.detectrecording[i].iSensor = 0;
                        }
                        else
                        {
                            sprintf(str,"Sensor Recording Stop for %s fail",profile.camera[i].strcameraname);
                            EventLogAdd(str,true);
                        }
                    }
                    else if(detectstatus.detectrecording[i].iSensor == 1 && profile.eventlink.iSensorRecordingLink[sensoridx][i])
                    {
                        runtimesettings.postrecinfo[i].RecReason =  LIB_EVENT_REC_SENSOR;
                        runtimesettings.postrecinfo[i].endtime = timeval;
                        runtimesettings.postrecinfo[i].duration = profile.camera[i].recordduration.iPostRecordDuration * 60;
                        DvrEvent *qMsg = new DvrEvent;
                        qMsg->cmd = EVENT_CMD_POSTREC_SET;
                        qMsg->param[0]=i;
                        RecEvtQ.enqueue((void*)qMsg);
                    }
                }
            }
            if(detectstatus.detectrecording[i].iNormal == 0 && detectstatus.detectrecording[i].iMotion == 0 && detectstatus.detectrecording[i].iSensor == 0 &&
               detectstatus.detectrecording[i].iVideoloss == 0 && detectstatus.detectrecording[i].iEmergency == 0)
            {
                detectstatus.detectrecording[i].iRecordingOn=0;
            }
        }
    }

    //Sensor - Alarm Event
    if(detect)
    {
    	//Event alram link에 따라서 Alarm Set
        for(int i=0;i<MAX_ALARM;i++)
        {
            if(profile.alarm[i].iAlarmOn && profile.eventlink.iSensorAlarmLink[sensoridx][i])
            {
                if(detectstatus.detectalarm[i].iAlarmDetectOn==0)
                {
                    detectstatus.detectalarm[i].iAlarmDetectOn=1;
                    Dvrmainsidebar.setAlarmIconStatus(i,1);
                    sprintf(str,"Alarm(%d) set by sensor(%d) event",i,sensoridx);
                    EventLogAdd(str,true);
                }
 #if DVR_API_ENABLE
                 LIB816x_operateAlarm(i,1);
 #endif
            }
        }
    }

    Dvrmainlayout.DrawOSDIcon();
}

/*!Video loss event 발생시 그에 따른 Recording/Alarm 수행*/
void dvrmain::VideoLossEventHandler(int videolossidx,int detect)
{
    if(videolossidx>=MAX_INTERNAL_CAMERA)
        return;

    char str[100];
    time_t timeval ;

    time(&timeval) ;

    detectstatus.detectvideoloss[videolossidx].iVideoLossOn = detect;

    //Video Loss - Record Event
    if(detect)
    {
        for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
        {
        	//event link, record schedule, record enable에 따라서 Record Start
            int ret=0;
            if(profile.camera[i].iEnable && profile.camera[i].camrec.iRecordingOn && profile.eventlink.iVideoLossRecordingLink[videolossidx][i] && runtimesettings.curRecSchedule[i] == REC_SCHEDULE_RECORDBYVLOSS)
            {
            	//현재 recording 중인경우 post recording timer를 update
                if(runtimesettings.postrecinfo[i].RecReason == LIB_EVENT_REC_VLOSS)
                {
                    runtimesettings.postrecinfo[i].endtime = timeval;
                    DvrEvent *qMsg = new DvrEvent;
                    qMsg->cmd = EVENT_CMD_POSTREC_CLEAR;
                    qMsg->param[0]=i;
                    RecEvtQ.enqueue((void*)qMsg);
                    runtimesettings.postrecinfo[i].RecReason =0;
                }
				//현재 recording 중이 아닌경우 Record start
                if(detectstatus.detectrecording[i].iVideoloss == 0)
                {
                    int audflag;
                    if(i<MAX_AUDIOIN)
                    {
                        audflag = profile.audioin.audioinch[i].iAudioInOn && profile.camera[i].camrec.iAudioOn;
                    }
                    else
                    {
                        audflag = 0;
                    }
#if DVR_API_ENABLE
                    sprintf(str,"Video loss Recording Start for %s",profile.camera[i].strcameraname);
                    EventLogAdd(str,true);
                    ret=LIB816x_setRecChannel(i, 1, LIB_EVENT_REC_VLOSS, audflag , profile.camera[i].strcameraname);
#else
                    ret=1;
#endif
                    if(ret)
                    {
                        detectstatus.detectrecording[i].iVideoloss = 1;
                        if(detectstatus.detectrecording[i].iRecordingOn==0)
                            detectstatus.detectrecording[i].iRecordingOn=1;
                    }
                    else
                    {
                        sprintf(str,"Video loss Recording Start for %s fail",profile.camera[i].strcameraname);
                        EventLogAdd(str,true);
                    }
                }
            }
            else if(profile.camera[i].camrec.iRecordingOn==0)
            {
                detectstatus.detectrecording[i].iRecordingOn=0;
            }
        }
    }
    else
    {
        for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
        {
            int ret=0;
            if(detectstatus.detectrecording[i].iVideoloss)
            {
            	//다른 camera의 Event가 설정되어있는지 event link에 따라서 검색
                bool bFindSet = false;
                for(int iVideoloss=0;iVideoloss<MAX_INTERNAL_CAMERA;iVideoloss++)
                {
                    if(detectstatus.detectvideoloss[iVideoloss].iVideoLossOn && profile.eventlink.iVideoLossRecordingLink[iVideoloss][i])
                    {
                        bFindSet = true;
                        break;
                    }
                }
                if(bFindSet == false)
                {
                	//post rec 설정이 된 경우 post record timer update
                	//post rec 설정이 되지 않은 경우 record stop
                    if(detectstatus.detectrecording[i].iVideoloss == 1 && profile.camera[i].recordduration.iPostRecordOn == 0)
                    {
#if DVR_API_ENABLE
                        sprintf(str,"Video loss Recording Stop for %s",profile.camera[i].strcameraname);
                        EventLogAdd(str,true);
                        ret=LIB816x_setRecChannel(i, 0, 0, 0 , " ");
#else
                        ret=1;
#endif
                        if(ret)
                        {
                            detectstatus.detectrecording[i].iVideoloss = 0;
                        }
                        else
                        {
                            sprintf(str,"Video loss Recording Stop for %s fail",profile.camera[i].strcameraname);
                            EventLogAdd(str,true);
                        }
                    }
                    else if(detectstatus.detectrecording[i].iVideoloss == 1  && profile.eventlink.iVideoLossRecordingLink[videolossidx][i])
                    {
                        runtimesettings.postrecinfo[i].RecReason =  LIB_EVENT_REC_VLOSS;
                        runtimesettings.postrecinfo[i].endtime = timeval;
                        runtimesettings.postrecinfo[i].duration = profile.camera[i].recordduration.iPostRecordDuration * 60;
                        DvrEvent *qMsg = new DvrEvent;
                        qMsg->cmd = EVENT_CMD_POSTREC_SET;
                        qMsg->param[0]=i;
                        RecEvtQ.enqueue((void*)qMsg);
                    }
                }
            }
            if(detectstatus.detectrecording[i].iNormal == 0 && detectstatus.detectrecording[i].iMotion == 0 && detectstatus.detectrecording[i].iSensor == 0 &&
               detectstatus.detectrecording[i].iVideoloss == 0 && detectstatus.detectrecording[i].iEmergency == 0)
            {
                detectstatus.detectrecording[i].iRecordingOn=0;
            }
        }
    }

    //Video Loss - Alarm Event
    if(detect)
    {
    	//Event alram link에 따라서 Alarm Set
        for(int i=0;i<MAX_ALARM;i++)
        {
            if(profile.alarm[i].iAlarmOn && profile.eventlink.iVideoLossAlarmLink[videolossidx][i])
            {
                if(detectstatus.detectalarm[i].iAlarmDetectOn==0)
                {
                    detectstatus.detectalarm[i].iAlarmDetectOn=1;
                    Dvrmainsidebar.setAlarmIconStatus(i,1);
                    sprintf(str,"Alarm(%d) set by video loss event of %s",i,profile.camera[videolossidx].strcameraname);
                    EventLogAdd(str,true);
                }
 #if DVR_API_ENABLE
                 LIB816x_operateAlarm(i,1);
 #endif
            }
        }
    }

    Dvrmainlayout.DrawOSDIcon();
}

/*!playback mode에서 timebar의 show/hide를 변경하는 기능 수행*/
void dvrmain::PlaybacktimebarshowOnOff(bool bShowTimebar)
{
	//playback mode일때 timebar show/hide를 변경
    if(m_bDisplayPlaybackTimebar!=bShowTimebar)
    {
        m_bDisplayPlaybackTimebar=bShowTimebar;

        if(m_bDisplayPlaybackTimebar)
        {
            Dvrmainplaybacktimelinebar.show();
        }
        else
        {
            Dvrmainplaybacktimelinebar.hide();
        }
        //timebar가 보이는지 안보이는지 여부에 따라서 status bar의 size를 변경
        Dvrmainstatusbar.ChangePlaybackMainWindowSize(m_bDisplayPlaybackTimebar);
        Dvrmainlayout.setFocus();
    }
}

/*!sub toolbar의 show/hide 기능 수행*/
void dvrmain::SetSubConsoleModeOnOff(int iMode)
{
	//sub toolbar에 대해서 show/hide 변경
    if(iMode)
        Dvrsecondtoolbar.show();
    else
        Dvrsecondtoolbar.hide();
}
/*!event log를 저장/sidebar에 표시하는 역할 수행*/
void dvrmain::EventLogAdd(char *EventStr,bool bShow)
{
    char buf[MAX_CHAR_256];
    sprintf(buf,"%02d:%02d:%02d-%s",curtime.currenttime.tm_hour,curtime.currenttime.tm_min,curtime.currenttime.tm_sec,EventStr);
    if(bShow)
    {
    	//log를 sidebar의 log창에 출력
        Dvrmainsidebar.addEventlogItem(buf);
    }
    //log file에 log 저장..
    FILE *fp = NULL;
    char filename[100];
    sprintf(filename,"%s%d_%02d_%02d.log",EVENTLOGPATH,curtime.currenttime.tm_year,curtime.currenttime.tm_mon+1,curtime.currenttime.tm_mday);
    QString info_path = QDir::currentPath();
    info_path.append(filename);
    fp = fopen(QStringToStr(info_path),"a");
    if(fp)
    {
        fseek(fp,-1,SEEK_END);
        fprintf(fp,"%s\n",buf);
        fclose(fp);
        fp=NULL;
    }
}
/*!error event등이 발생시 pop-up형태로 보여주는 역할 수행*/
void dvrmain::ShowNotifyDialog(char* str)
{
	//event 발생시 해당 text를 pop-up 형식으로 표시
    if(m_bLogin)
    {
        Dvrnotifydialog.SetNotifyText(str);
        Dvrnotifydialog.setFocus();
        Dvrnotifydialog.show();
    }
}
/*!1달 이상의 log file을 삭제하는 역할 수행*/
void dvrmain::EraseOldEventLog(int year,int month,int day)
{
    dprintf("EraseOldEventLog yy:%d mm:%d day:%d\n",year,month,day);
    char buf[100];
    QString info_path = QDir::currentPath();
    info_path.append(EVENTLOGPATH);
    int delyear,delmon,delday;

	//1개월 이상된 log 파일 삭제
    if(month==0)    //Jan.
    {
        delyear = year - 1;
        for(int i=delyear-2;i<delyear;i++)
        {
            sprintf(buf,"rm -f %s%d_*.log",QStringToStr(info_path),i);
            system(buf);
        }
        delmon = 11;    //remove Nov.
        for(int i=0;i<delmon;i++)
        {
            sprintf(buf,"rm -f %s%d_%02d_*.log",QStringToStr(info_path),delyear,i+1);
            system(buf);
        }

        delmon = 11;    //remove Dec
        for(int i=1;i<day;i++)
        {
            sprintf(buf,"rm -f %s%d_%02d_%02d.log",QStringToStr(info_path),delyear,delmon+1,i);
            system(buf);
        }
    }
    else if(month==1)   //Feb
    {
        delyear = year - 1;
        for(int i=delyear-2;i<delyear+1;i++)
        {
            sprintf(buf,"rm -f %s%d_*.log",QStringToStr(info_path),i);
            system(buf);
        }

        delyear = year;
        delmon = 0;
        for(int i=1;i<day;i++)
        {
            sprintf(buf,"rm -f %s%d_%02d_%02d.log",QStringToStr(info_path),delyear,delmon+1,i);
            system(buf);
        }
    }
    else
    {
        delyear = year;
        for(int i=delyear-2;i<delyear;i++)
        {
            sprintf(buf,"rm -f %s%d_*.log",QStringToStr(info_path),i);
            system(buf);
        }
        delmon = month-1;
        for(int i=0;i<delmon;i++)
        {
            sprintf(buf,"rm -f %s%d_%02d_*.log",QStringToStr(info_path),delyear,i+1);
            system(buf);
        }
        delmon = month-1;
        for(int i=1;i<day;i++)
        {
            sprintf(buf,"rm -f %s%d_%02d_%02d.log",QStringToStr(info_path),delyear,delmon+1,i);
            system(buf);
        }
    }
}

/*!virtual keyboard show 기능 수행*/
bool dvrmain::ShowVirtualKeyboard()
{
	//virtual keyboard show/hide 처리
    if(!virtualKeyBoard)
        virtualKeyBoard = new WidgetKeyboard(this);

    virtualKeyBoard->move(LAYOUTWINDOWFULL_X, LAYOUTWINDOWFULL_Y);

    if(virtualKeyBoard->isVisible())
        virtualKeyBoard->hide();
    else
        virtualKeyBoard->show();

    return virtualKeyBoard->isVisible();
}
/*!Audio output channel을 설정*/
void dvrmain::SetAudioChannel(bool bSet, int ch)
{
	//audio output channel을 set
    if(bSet || m_iAudioOutChannel != ch)
    {
        m_iAudioOutChannel = ch;
#if	DVR_API_ENABLE
        LIB816x_setAudioOutput(profile.audioout.iAudioOutOn, profile.audioout.iVolume,ch);
#endif
    }
    WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));
}
/*!Front keypad 입력을 받아 해당 control로 전달*/
void dvrmain::keyPressEvent ( QKeyEvent * event )
{
    int key = event->key();
    dprintf("keyPressEvent key=0x%X\n",key);

    if(key == Qt::Key_Escape)   //power down
    {
        EventLogAdd((char*)"Power off",false);
        BeforeTerminateOperation();
#if	DVR_API_ENABLE
        LIB816x_stopSystem();
#endif
        bBlockKey = true;
        return;
    }

    if(bBlockKey)
        return;

    if(Dvrlogin.isVisible())
    {
        Dvrlogin.keyPressEvent(event );
        return;
    }
    else if(Dvrnotifydialog.isVisible())
    {
        Dvrnotifydialog.keyPressEvent(event );
        return;
    }

    switch(key)
    {
        //Camera Select Keys
        case Qt::Key_1:
        case Qt::Key_2:
        case Qt::Key_3:
        case Qt::Key_4:
        case Qt::Key_5:
        case Qt::Key_6:
        case Qt::Key_7:
        case Qt::Key_8:
        case Qt::Key_9:
        case Qt::Key_A:
        case Qt::Key_B:
        case Qt::Key_C:
        case Qt::Key_D:
        case Qt::Key_E:
        case Qt::Key_F:
        case Qt::Key_G:
           Dvrmainsidebar.setFocus();
           Dvrmainsidebar.keyPressEvent(event);
        break;

        //Function Keys
        case Qt::Key_H:     //LIVE
            if(runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
            {
                Dvrmaintoolbar.setFocus();
                Dvrmaintoolbar.keyPressEvent(event);
            }
            setFocus();
        break;
        case Qt::Key_I:     //SEARCH
            if(runtimesettings.iLivePlaybackMode==LIB_LIVE_MODE)
            {
                Dvrmaintoolbar.setFocus();
                Dvrmaintoolbar.keyPressEvent(event);
            }
            setFocus();
        break;
        case Qt::Key_J:     //SCRMODE
        case Qt::Key_K:     //FULL SCR
        case Qt::Key_L:     //PREV
        case Qt::Key_M:     //NEXT
        case Qt::Key_N:     //SEQ
            Dvrmaintoolbar.setFocus();
            Dvrmaintoolbar.keyPressEvent(event);
            setFocus();
        break;
        case Qt::Key_O:     //RF
        case Qt::Key_P:     //RSTEP
        case Qt::Key_Q:     //PLAYPAUSE
        case Qt::Key_R:     //FSTEP
        case Qt::Key_S:     //FF
        case Qt::Key_T:     //STOP
            if(runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
            {
                Dvrmainsidebar.setFocus();
                Dvrmainsidebar.keyPressEvent(event);
            }
            setFocus();
        break;
        case Qt::Key_U:     //PTZ
            setFocus();
        break;
        case Qt::Key_V:     //REC
            setFocus();
            if(profile.dvrall.iRecordEmergencyOn==0)
                profile.dvrall.iRecordEmergencyOn=1;
            else
                profile.dvrall.iRecordEmergencyOn=0;
            EmergencyRecordAll();
        break;
        case Qt::Key_W:     //SETUP
            Dvrmaintoolbar.setFocus();
            Dvrmaintoolbar.keyPressEvent(event);
            setFocus();
        break;

        //Menu Selection Or PTZ Keys
        case Qt::Key_Up:
        case Qt::Key_Down:
        case Qt::Key_Left:
        case Qt::Key_Right:
            Dvrmainsidebar.setFocus();
            Dvrmainsidebar.keyPressEvent(event);
        break;
        case Qt::Key_Return:
            if(Dvrmaintoolbar.hasFocus())
            {
                Dvrmaintoolbar.keyPressEvent(event);
            }
            else if(Dvrmainsidebar.hasFocus())
            {
                Dvrmainsidebar.keyPressEvent(event);
            }
        break;
        //Dial Input Keys
        case Qt::Key_PageDown:
        case Qt::Key_PageUp:
        case Qt::Key_Home:
        case Qt::Key_F1:
        case Qt::Key_F2:
        case Qt::Key_F3:
        case Qt::Key_F4:
        case Qt::Key_F5:
        case Qt::Key_F6:
        case Qt::Key_Delete:
        case Qt::Key_F7:
        case Qt::Key_F8:
        case Qt::Key_F9:
        case Qt::Key_F10:
        case Qt::Key_F11:
        case Qt::Key_F12:
        case Qt::Key_End:
            Dvrmainsidebar.setFocus();
            Dvrmainsidebar.keyPressEvent(event);
        break;
    }
}
/*!mouse wheel event를 전달받아 playback mode에서 timebar show/hide기능 수행*/
void dvrmain::wheelEvent(QWheelEvent *event)
{
    if(m_bLogin==false)
        return;

    static int degree = 1;
    event->ignore();
	//playback mode에서 wheel up/down에 따라서 timebar show/hide
    if(runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
    {
        int numDegrees = event->delta() ;
        if(numDegrees<0 && degree>0)
        {
            degree = -1;
            DvrEvent *qMsg = new DvrEvent;
            qMsg->cmd = EVENT_CMD_MOUSEWHEEL;
            qMsg->param[0]=numDegrees;
            EvtQ.enqueue((void*)qMsg);
        }
        else if(numDegrees>0 && degree<0)
        {
            degree = 1;
            DvrEvent *qMsg = new DvrEvent;
            qMsg->cmd = EVENT_CMD_MOUSEWHEEL;
            qMsg->param[0]=numDegrees;
            EvtQ.enqueue((void*)qMsg);
        }
    }
}

/*!Main Timer - HDD check, current time,playback time update등 수행*/
void dvrmain::MainTimerUpdate()
{
    int ret=0;
    static double prev_used=0;
    unsigned long total, used=0, sz_total=0;
    static long basketsize=0;
    static long basketcount=0;

    GetCurrentTime(&curtime.currenttime);

	//Video loss check
#if DVR_API_ENABLE
    unsigned int vlossch = LIB816x_getVideoLossDetectStatus();
    for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
    {
        int vlossdetect = (vlossch>>i)&0x01;
        if(vlossdetect != detectstatus.detectvideoloss[i].iVideoLossOn)
        {
            DvrEvent *qMsg = new DvrEvent;
            qMsg->cmd = EVENT_CMD_VIDEOLOSS;
            qMsg->param[0]=i;
            qMsg->param[1]=(int)vlossdetect;
            EvtQ.enqueue((void*)qMsg);
        }
    }
#endif
    if(curtime.currenttime.tm_hour == 0 && curtime.currenttime.tm_min== 0 && curtime.currenttime.tm_sec == 0)
    {
    	//자정에 한달 이상된 log file 삭제
        EraseOldEventLog(curtime.currenttime.tm_year,curtime.currenttime.tm_mon, curtime.currenttime.tm_mday);
    }

    if(bBlockKey)
        return;

#if DVR_API_ENABLE
    if((basketcount==0 || basketsize==0 || refreshdiskinfo==true) && availableHddCount)
    {
        refreshdiskinfo=false;
        if(LIB816x_BasketInfo(&basketcount,&basketsize)==0)
        {
            basketcount=0;
            basketsize=0;
        }
        dprintf("basketsize : %ld\n",basketsize);
        dprintf("basketcount : %ld\n",basketcount);
    }
    else if(refreshdiskinfo || !availableHddCount)
    {
        basketcount=0;
        basketsize=0;
    }
#endif

	//HDD check 및 update
    if(curtime.currenttime.tm_sec%10 == 0 || prev_used==0)
    {
        total = (double)(basketsize*basketcount);

        if(total!=0)
        {
#if DVR_API_ENABLE
            ret=LIB816x_rec_disk_size(&sz_total, &used);
            used /= 1024;
#else
            used = 1000000000;
#endif
            prev_used = used;
            dprintf("\ntotal %ld, used = %ld \n", (long)total, (long)used);
        }
        else    //NO HDD CASE
        {
            #if DVR_API_ENABLE
            if(runtimesettings.playbackinfo.playbackmode.iPlaybackMode != PLAYBACKMODE_STOP )
            {
                LIB816x_stopPlayback_x();
                runtimesettings.playbackinfo.playbackmode.iPlaybackMode = PLAYBACKMODE_STOP;
            }
            #endif

            //stop record
            for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
            {
                if(detectstatus.detectrecording[i].iRecordingOn)
                {
        #if DVR_API_ENABLE
                    LIB816x_setRecChannel(i, 0, 0, 0 , " ");
        #endif
                }
            }
            memset(&detectstatus,0,sizeof(DETECTSTATUS));
            Dvrmainlayout.DrawOSDIcon();
        }
        Dvrmainstatusbar.UpdateHDDInfo(used,total);
    }

    if(m_systemtimerstarted==false)
    {
    	//Recording Start
        EventLogAdd((char*)EVENTLOG_SYSTEM_START,true);
        m_systemtimerstarted=true;
        if(profile.dvrall.iRecordEmergencyOn)
        {
            EmergencyRecordAll();
        }
        else
        {
            RecScheduleHandler(curtime.currenttime.tm_wday,curtime.currenttime.tm_hour);
        }
    }

    if(curtime.currenttime.tm_min==0 && curtime.currenttime.tm_sec == 0)
    {
    	//매 시간마다 record schedule을 check
        RecScheduleHandler(curtime.currenttime.tm_wday,curtime.currenttime.tm_hour);
    }

    if(runtimesettings.iLivePlaybackMode==LIB_LIVE_MODE)
    {
    	//현재 시간 update
        Dvrmainstatusbar.UpdateMainDateTime(&curtime.currenttime);
    }
    else if(runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
    {
    	//playback 시간update , calendar, timebar update
        if(curtime.currenttime.tm_sec==3 || curtime.currenttime.tm_sec==4)
        {
            if(!Dvrmainsetupproperty.isVisible())
                Dvrmainplaybacktimelinebar.SetTimeTableUpdate(false,true);
            if(curtime.currenttime.tm_hour == 0 && curtime.currenttime.tm_min <= 3)
            {
                memset(runtimesettings.playbackinfo.playbacktableinfo.iMonthTable,0,sizeof(runtimesettings.playbackinfo.playbacktableinfo.iMonthTable));
                LIB816x_GetRecDays(curtime.playbacktime,runtimesettings.playbackinfo.playbacktableinfo.iMonthTable);
                Dvrmainsidebar.setCalendarRecordDate(&curtime.playbacktime,runtimesettings.playbackinfo.playbacktableinfo.iMonthTable);
            }
        }

        if(m_iPlaybackTimebarUpdateDisableCount>0)
        {
            m_iPlaybackTimebarUpdateDisableCount--;
        }

        if(runtimesettings.playbackinfo.playbackmode.iPlaybackMode==PLAYBACKMODE_PLAY)
        {
            struct tm playbacktime;
#if DVR_API_ENABLE
            if(LIB816x_getCurPlaybackTime(&playbacktime)==1)
#else
            memcpy(&playbacktime,&curtime.currenttime,sizeof(struct tm));
#endif
            {
                memcpy(&curtime.playbacktime,&playbacktime,sizeof(struct tm));
            }
            if(curtime.playbacktime.tm_min != playbackupdatetime.tm_min || curtime.playbacktime.tm_hour != playbackupdatetime.tm_hour || curtime.playbacktime.tm_mday != playbackupdatetime.tm_mday)
            {
                if(curtime.playbacktime.tm_mday != playbackupdatetime.tm_mday && bDoUpdateCalendar)
                {
                    Dvrmainsidebar.setCalendarSelectDay(&curtime.playbacktime,false);
                }
                else if(bDoUpdateCalendar==false)
            	{
            		bDoUpdateCalendar=true;
            	}
                memcpy(&playbackupdatetime,&curtime.playbacktime,sizeof(struct tm));
                if(m_iPlaybackTimebarUpdateDisableCount<=0)
                {
                    Dvrmainplaybacktimelinebar.SetTimeTableSelectTimeMarking(&curtime.playbacktime);
                    m_iPlaybackTimebarUpdateDisableCount=-1;
                }
            }
            else if(m_iPlaybackTimebarUpdateDisableCount==0)
            {
                Dvrmainplaybacktimelinebar.SetTimeTableSelectTimeMarking(&curtime.playbacktime);
                m_iPlaybackTimebarUpdateDisableCount=-1;
            }
        }
        Dvrmainstatusbar.UpdateMainDateTime(&curtime.playbacktime);
     }
}
/*!Main layout timer - 일정시간후 Main layout next page로 변경 */
void dvrmain::MainlayoutTimerUpdate()
{
    Dvrmaintoolbar.NextPageBtnSlot_Sequence();
}
/*!Sub layout time - 일정시간후 sub layout next page로 변경*/
void dvrmain::SublayoutTimerUpdate()
{
    Dvrsecondtoolbar.NextPageBtnSlot();
}
/*!event수신/처리 app로부터 error 발생을 전달받아 해당 기능 수행*/
void dvrmain::EvtHandler()
{
    while(!EvtQ.isEmpty())
    {
        DvrEvent *qMsg = NULL;
        qMsg = (DvrEvent *)EvtQ.dequeue();
        if(qMsg)
        {
            if(qMsg->cmd == EVENT_CMD_MOTION)
            {
            	int chid = qMsg->param[0];
                int detect = qMsg->param[1];
            	MotionEventHandler(chid,detect);
            }
            else if(qMsg->cmd == EVENT_CMD_SENSOR)
            {
                int sensorid = qMsg->param[0];
                int detect = qMsg->param[1];
                SensorEventHandler(sensorid,detect);
            }
            else if(qMsg->cmd == EVENT_CMD_VIDEOLOSS)
            {
            	int chid = qMsg->param[0];
                int detect = qMsg->param[1];
            	VideoLossEventHandler(chid,detect);
            }
            else if(qMsg->cmd == EVENT_CMD_ALARM)
            {
            	int alarmid = qMsg->param[0];
            	int detect = qMsg->param[1];
            	if(detect == 0)
                {
                    char str[100];
                    detectstatus.detectalarm[alarmid].iAlarmDetectOn=0;
                    Dvrmainsidebar.setAlarmIconStatus(alarmid,0);
                    sprintf(str,"Alarm(%d) reset",alarmid);
                    EventLogAdd(str,true);
                }
            }
            else if(qMsg->cmd == EVENT_CMD_MOUSEWHEEL)
            {
                if(qMsg->param[0]<0 && m_bDisplayPlaybackTimebar)
                {
                    PlaybacktimebarshowOnOff(false);
                }
                else if(qMsg->param[0]>0 && !m_bDisplayPlaybackTimebar)
                {
                    PlaybacktimebarshowOnOff(true);
                }
            }
            delete qMsg;
        }
    }

    // Check pipe from application manager.
#if DVR_API_ENABLE
    int rv;
    DVR_MSG_T msg;

    rv = LIB816x_getDvrMessage(&msg);
    if (0 == rv)
    {
        switch(msg.which)
        {
            case LIB_PLAYER:
                if(runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
                {
                    switch(msg.data)
                    {
                        case LIB_PB_NO_ERR:
                            break;
                        case LIB_PB_START_FAILED:
                            EventLogAdd((char*)"PLAYBACK ERROR : START FAIL",true);
                            Dvrmainsidebar.setPlayBackMode(PLAYBACKMODE_STOP);
                            Dvrmainsidebar.setPlaybackBtnSelect(PLAYBACKMODE_STOP);
                            break;
                        case LIB_PB_JUMP_FAILED:
                            EventLogAdd((char*)"PLAYBACK ERROR : JUMP FAIL",true);
                            Dvrmainsidebar.setPlayBackMode(PLAYBACKMODE_STOP);
                            Dvrmainsidebar.setPlaybackBtnSelect(PLAYBACKMODE_STOP);
                            break;
                        case LIB_PB_ALLOC_FAILED:
                            EventLogAdd((char*)"PLAYBACK ERROR : ALLOC FAIL",true);
                            Dvrmainsidebar.setPlayBackMode(PLAYBACKMODE_STOP);
                            Dvrmainsidebar.setPlaybackBtnSelect(PLAYBACKMODE_STOP);
                            break;
                        case LIB_PB_BASKET_FAILED:
                            EventLogAdd((char*)"PLAYBACK ERROR : BASKET FAIL",true);
                            Dvrmainsidebar.setPlayBackMode(PLAYBACKMODE_STOP);
                            Dvrmainsidebar.setPlaybackBtnSelect(PLAYBACKMODE_STOP);
                            Dvrmainsetupproperty.get_disk_info();
                            break;
                        case LIB_PB_OSAL_FAILED:
                            EventLogAdd((char*)"PLAYBACK ERROR : OSAL FAIL",true);
                            Dvrmainsidebar.setPlayBackMode(PLAYBACKMODE_STOP);
                            Dvrmainsidebar.setPlaybackBtnSelect(PLAYBACKMODE_STOP);
                            break;
                        case LIB_PB_READ_FRAME_FAILED:
                            EventLogAdd((char*)"PLAYBACK ERROR : READ FRAME FAIL",true);
                            Dvrmainsidebar.setPlayBackMode(PLAYBACKMODE_STOP);
                            Dvrmainsidebar.setPlaybackBtnSelect(PLAYBACKMODE_STOP);
                            Dvrmainsetupproperty.get_disk_info();
                            break;
                        case LIB_PB_NOT_ALLOWED:
                            EventLogAdd((char*)"PLAYBACK ERROR : NOT ALLOWED",true);
                            Dvrmainsidebar.setPlayBackMode(PLAYBACKMODE_STOP);
                            Dvrmainsidebar.setPlaybackBtnSelect(PLAYBACKMODE_STOP);
                            break;
                    }
                    if (LIB_PB_READ_FRAME_FAILED == msg.data)
                    {
                        dprintf("Qt: !!!!! Player read error. then playback will be stopped\n",msg.which ,msg.data ,msg.actual_size);
                    }
                }
                break;
            case LIB_RECORDER:
                switch(msg.data)
                {
                    case LIB_WR_NO_ERR:
                        dprintf("RECORDER : NO ERR\n");
                        break;
                    case LIB_WR_OPEN_FAIL:
                        EventLogAdd((char*)"Record error : open fail",true);
                        dprintf("RECORDER ERROR : OPEN FAIL\n");
                        for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
                        {
                            LIB816x_setRecChannel(i, 0, 0, 0, " ");
                            detectstatus.detectrecording[i].iRecordingOn = 0;
                            detectstatus.detectrecording[i].iNormal = 0;
                            detectstatus.detectrecording[i].iMotion = 0;
                            detectstatus.detectrecording[i].iSensor = 0;
                            detectstatus.detectrecording[i].iVideoloss = 0;
                            detectstatus.detectrecording[i].iEmergency = 0;
                        }
                        if(runtimesettings.iLivePlaybackMode==LIB_LIVE_MODE)
                        {
                            Dvrmainlayout.DrawOSDIconLive();
                        }
                        else if(runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
                        {
                            Dvrmainlayout.DrawOSDIconPlayback();
                        }
                        ShowNotifyDialog((char*)"BASKET OPEN FAIL");
                        break;
                    case LIB_WR_REC_FAIL:
                        EventLogAdd((char*)"Record error : record fail",true);
                        dprintf("RECORDER ERROR : REC FAIL\n");
                        for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
                        {
                            LIB816x_setRecChannel(i, 0, 0, 0, " ");
                            detectstatus.detectrecording[i].iRecordingOn = 0;
                            detectstatus.detectrecording[i].iNormal = 0;
                            detectstatus.detectrecording[i].iMotion = 0;
                            detectstatus.detectrecording[i].iSensor = 0;
                            detectstatus.detectrecording[i].iVideoloss = 0;
                            detectstatus.detectrecording[i].iEmergency = 0;
                        }
                        if(runtimesettings.iLivePlaybackMode==LIB_LIVE_MODE)
                        {
                            Dvrmainlayout.DrawOSDIconLive();
                        }
                        else if(runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
                        {
                            Dvrmainlayout.DrawOSDIconPlayback();
                        }
                        ShowNotifyDialog((char*)"RECORDING FAIL");
                        Dvrmainsetupproperty.get_disk_info();
                        break;
                    case LIB_WR_HDD_FULL:
                        EventLogAdd((char*)"Record error : HDD full",true);
                        dprintf("RECORDER ERROR : HDD FULL\n");
                        for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
                        {
                            LIB816x_setRecChannel(i, 0, 0, 0, " ");
                            detectstatus.detectrecording[i].iRecordingOn = 0;
                            detectstatus.detectrecording[i].iNormal = 0;
                            detectstatus.detectrecording[i].iMotion = 0;
                            detectstatus.detectrecording[i].iSensor = 0;
                            detectstatus.detectrecording[i].iVideoloss = 0;
                            detectstatus.detectrecording[i].iEmergency = 0;
                        }
                        if(runtimesettings.iLivePlaybackMode==LIB_LIVE_MODE)
                        {
                            Dvrmainlayout.DrawOSDIconLive();
                        }
                        else if(runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
                        {
                            Dvrmainlayout.DrawOSDIconPlayback();
                        }
                        ShowNotifyDialog((char*)"HDD FULL");
                        break;
                    case LIB_WR_NOT_MOUNTED:
                        dprintf("RECORDER ERROR : NOT MOUNTED\n");
                        for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
                        {
                            LIB816x_setRecChannel(i, 0, 0, 0, " ");
                            detectstatus.detectrecording[i].iRecordingOn = 0;
                            detectstatus.detectrecording[i].iNormal = 0;
                            detectstatus.detectrecording[i].iMotion = 0;
                            detectstatus.detectrecording[i].iSensor = 0;
                            detectstatus.detectrecording[i].iVideoloss = 0;
                            detectstatus.detectrecording[i].iEmergency = 0;
                        }
                        if(runtimesettings.iLivePlaybackMode==LIB_LIVE_MODE)
                        {
                            Dvrmainlayout.DrawOSDIconLive();
                        }
                        else if(runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
                        {
                            Dvrmainlayout.DrawOSDIconPlayback();
                        }
                        Dvrmainsetupproperty.get_disk_info();
                        break;
                }
                break;
            case LIB_MISC:
                break;
            case LIB_BACKUP:
                {
                    Dvrmainsetupproperty.BackupComplete();
                    switch(msg.data)
                    {
                        case LIB_BA_NO_ERR:
                            EventLogAdd((char*)"BACKUP DONE",true);
                            dprintf("BACK UP ERROR : LIB_BA_NO_ERR\n");
                            ShowNotifyDialog((char*)"BACKUP DONE");
                            break;
                        case LIB_BA_START_FAILED:
                            EventLogAdd((char*)"BACKUP ERROR : START FAIL",true);
                            dprintf("BACK UP ERROR : LIB_BA_START_FAILED\n");
                            ShowNotifyDialog((char*)"BACKUP ERROR : START FAIL");
                            break;
                        case LIB_BA_ALLOC_FAILED:
                            EventLogAdd((char*)"BACKUP ERROR : ALLOC FAIL",true);
                            dprintf("BACK UP ERROR : LIB_BA_ALLOC_FAILED\n");
                            ShowNotifyDialog((char*)"BACKUP ERROR : ALLOC FAIL");
                            break;
                        case LIB_BA_BASKET_FAILED:
                            EventLogAdd((char*)"BACKUP ERROR : BASKET FAIL",true);
                            dprintf("BACK UP ERROR : LIB_BA_BASKET_FAILED\n");
                            ShowNotifyDialog((char*)"BACKUP ERROR : BASKET FAIL");
                            break;
                        case LIB_BA_OSAL_FAILED:
                            EventLogAdd((char*)"BACKUP ERROR : OSAL FAIL",true);
                            dprintf("BACK UP ERROR : LIB_BA_OSAL_FAILED\n");
                            ShowNotifyDialog((char*)"BACKUP ERROR : OSAL FAIL");
                            break;
                        case LIB_BA_READ_FRAME_FAILED:
                            EventLogAdd((char*)"BACKUP ERROR : READ FRAME FAIL",true);
                            dprintf("BACK UP ERROR : LIB_BA_READ_FRAME_FAILED\n");
                            ShowNotifyDialog((char*)"BACKUP ERROR : READ FRAME FAIL");
                            break;
                        case LIB_BA_CREATE_AVI_FAILED:
                            EventLogAdd((char*)"BACKUP ERROR : CREATE AVI FAIL",true);
                            dprintf("BACK UP ERROR : LIB_BA_CREATE_AVI_FAILED");
                            ShowNotifyDialog((char*)"BACKUP ERROR : CREATE AVI FAIL");
                            break;
                        case LIB_BA_MAKE_ISO_FAILED:
                            EventLogAdd((char*)"BACKUP ERROR : MAKE ISO FAIL",true);
                            dprintf("BACK UP ERROR : LIB_BA_MAKE_ISO_FAILED\n");
                            ShowNotifyDialog((char*)"BACKUP ERROR : MAKE ISO FAIL");
                            break;
                        case LIB_BA_WRITE_ISO_FAILED:
                            EventLogAdd((char*)"BACKUP ERROR : WRITE ISO FAIL",true);
                            dprintf("BACK UP ERROR : LIB_BA_WRITE_ISO_FAILED\n");
                            ShowNotifyDialog((char*)"BACKUP ERROR : WRITE ISO FAIL");
                            break;
                        case LIB_BA_NOT_ALLOWED:
                            EventLogAdd((char*)"BACKUP ERROR : NOT ALLOWED",true);
                            dprintf("BACK UP ERROR : LIB_BA_NOT_ALLOWED\n");
                            ShowNotifyDialog((char*)"BACKUP ERROR : NOT ALLOWED");
                            break;
                        case LIB_BA_NOT_SUFFICIENT:
                            EventLogAdd((char*)"BACKUP ERROR : NOT SUFFICIENT",true);
                            dprintf("BACK UP ERROR : LIB_BA_NOT_SUFFICIENT\n");
                            ShowNotifyDialog((char*)"BACKUP ERROR : NOT SUFFICIENT STORAGE");
                            break;
                        case LIB_BA_INVALID_MEDIA:
                            EventLogAdd((char*)"BACKUP ERROR : INVALID MEDIA",true);
                            dprintf("BACK UP ERROR : LIB_BA_INVALID_MEDIA\n");
                            ShowNotifyDialog((char*)"BACKUP ERROR : INVALID MEDIA");
                            break;
                        default:
                            dprintf("BACK UP ERROR : msg.data = %d\n",msg.data);
                            break;
                    }
                }
                break;
            default:
                dprintf("Receive DVR Message from %d\n",msg.which);
                break;
        }
    }
#endif
}
/*!Motion event 처리 및 post recording 기능 수행*/
void dvrmain::EvtMotionHandler()
{
	//Motion Event 처리
    static int checkChMask=0;
#if DVR_API_ENABLE
    unsigned int motionch = LIB816x_getMotionDetectStatus();
//printf("EvtMotionHandler:motion status=%d\n",motionch);
    for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
    {
        int motiondetect = (motionch>>i)&0x01;
        if(motiondetect != detectstatus.detectmotion[i].iMotionDetectOn)
        {
        	printf("channle %d motion detected !!!!\n",i);
            MotionEventHandler(i,motiondetect);
        }
    }
#endif
	//Post recording Event 처리
    while(RecEvtQ.count())
    {
        DvrEvent *qMsg = NULL;
        qMsg = (DvrEvent *)RecEvtQ.dequeue();
        if(qMsg)
        {
            if(qMsg->cmd == EVENT_CMD_POSTREC_SET)
            {
                checkChMask |= (1<<qMsg->param[0]);
            }
            else if(qMsg->cmd == EVENT_CMD_POSTREC_CLEAR)
            {
                checkChMask &= (~(1<<qMsg->param[0]));
            }
            delete qMsg;
        }
    }

    if(checkChMask)
    {
        time_t timeval ;
        time(&timeval) ;
        for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
        {
            if((checkChMask>>i)&0x01)
            {
                if(timeval - runtimesettings.postrecinfo[i].endtime >= runtimesettings.postrecinfo[i].duration)
                {
                    int ret=0;
                    char str[100];

                    checkChMask &= (~(1<<i));
                    dprintf("Stop PostRec check %d-th Camera duration:%d\n",i+1,timeval - runtimesettings.postrecinfo[i].endtime);
                    if(runtimesettings.postrecinfo[i].RecReason == LIB_EVENT_REC_MOT)
                    {
                        sprintf(str,"Motion Recording Stop for %s",profile.camera[i].strcameraname);
                        detectstatus.detectrecording[i].iMotion = 0;
                    }
                    else if(runtimesettings.postrecinfo[i].RecReason == LIB_EVENT_REC_SENSOR)
                    {
                        sprintf(str,"Sensor Recording Stop for %s",profile.camera[i].strcameraname);
                        detectstatus.detectrecording[i].iSensor = 0;
                    }
                    else if(runtimesettings.postrecinfo[i].RecReason == LIB_EVENT_REC_VLOSS)
                    {
                        sprintf(str,"Video loss Recording Stop for %s",profile.camera[i].strcameraname);
                        detectstatus.detectrecording[i].iVideoloss = 0;
                    }
#if DVR_API_ENABLE
                    EventLogAdd(str,true);
                    ret=LIB816x_setRecChannel(i, 0, 0, 0 , " ");
#endif
                    if(detectstatus.detectrecording[i].iNormal == 0 && detectstatus.detectrecording[i].iMotion == 0 && detectstatus.detectrecording[i].iSensor == 0 &&
                       detectstatus.detectrecording[i].iVideoloss == 0 && detectstatus.detectrecording[i].iEmergency == 0)
                    {
                        detectstatus.detectrecording[i].iRecordingOn=0;
                    }
                    Dvrmainlayout.DrawOSDIcon();
                }
            }
        }
    }

}

void dvrmain::changeEvent(QEvent *e)
{

    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
