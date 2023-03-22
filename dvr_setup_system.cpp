/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include "dvr_defines.h"
#include "dvr_setup.h"
#include "ui_dvr_setup.h"
#include "dvr_main.h"
#include <unistd.h>

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
extern dvrmain *pMainClass;
extern dvr_layout_main *pLayoutClass;
extern dvr_toolbar_main *pToolbar;
extern dvr_toolbar_sub *pSubToolbar;
extern dvr_sidebar *pSidebar;
extern dvr_timelinebar_play *pTimebar;
extern dvr_statusbar *pStatusbar;

extern SYSTEMDATETIME dlgsystemdatetime;
/*----------------------------------------------------------------------------
 local functions
-----------------------------------------------------------------------------*/
/*!system tab의 control들을 초기화 */
void dvr_setup::InitSetupSystemControls()
{
    iCurrentTimerOn=0;
    connect(ui->systemconfiginitpushButton, SIGNAL(clicked()), this, SLOT(SystemConfigInitBtnSlot()));
    connect(ui->systemrebootpushButton, SIGNAL(clicked()), this, SLOT(SystemRebootBtnSlot()));
    connect(ui->systemdatetimeapplypushButton, SIGNAL(clicked()), this, SLOT(SystemDateTimeApplyBtnSlot()));

    m_iUserSelectId=0;
    QSize qsize;
    qsize.setWidth(USERLISTICON_SIZE_W);
    qsize.setHeight(USERLISTICON_SIZE_H);
    UserListItemIcon.addFile(QString::fromUtf8(USER_LIST_USER_ICON), qsize, QIcon::Normal, QIcon::Off);

    for(int i=0;i<MAX_USER;i++)
    {
        char strusername[MAX_TITLE_CHAR];
        if(i==0)
            sprintf(strusername,"%s",STRADMIN);
        else
            sprintf(strusername,"%s%d",STRUSER,i);

        SetupUserListItem[i].setIcon(UserListItemIcon);
        SetupUserListItem[i].setText(QString::fromUtf8(strusername));
        ui->systemuserlistWidget->insertItem(i,&SetupUserListItem[i]);
    }
    connect(ui->systemuserlistWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(SetupSystemUSerItemClicked(QListWidgetItem *)));
    connect(ui->systemuserlistWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(SetupSystemUSerItemDoubleClicked(QListWidgetItem *)));

    connect(ui->systemuserapplysavepushButton, SIGNAL(clicked()), this, SLOT(SystemUserApplySaveBtnSlot()));
    connect(ui->systemusercurrentpasswordvirtualkeypushButton, SIGNAL(clicked()), this, SLOT(SystemUserCurrentPasswordVirtualKeyboardBtnSlot()));
    connect(ui->systemuserconfirmpasswordvirtualkeypushButton, SIGNAL(clicked()), this, SLOT(SystemUserConfirmPasswordVirtualKeyboardBtnSlot()));

}
/*!profile data struct에 따라서 ui에 value setting */
void dvr_setup::SetSystemdata()
{
     ui->systeminfofwversionvaluelabel->setText(StrToQString(dlgruntimesettings.versioninfo.FwVer));
     ui->systeminfohwversionvaluelabel->setText(StrToQString(dlgruntimesettings.versioninfo.HwVer));
     ui->systeminfomacaddr1valuelabel->setText(StrToQString(dlgruntimesettings.versioninfo.MacAddr1));
     ui->systeminfomacaddr2valuelabel->setText(StrToQString(dlgruntimesettings.versioninfo.MacAddr2));
     ui->systeminfobuilddatetimevaluelabel->setText(StrToQString(dlgruntimesettings.versioninfo.BuildDateTime));

     GetCurrentTime(&dlgsystemdatetime.systemtime);

     UpdateDateTimeCurrent(&curtime.currenttime);
     if(m_bChangeSystemTime==false)
     {
        UpdateDateTimeSet(&dlgsystemdatetime.systemtime);
     }
     iCurrentTimerOn=1;

     SetSystemUserdata();

}
/*!ui control의 value를 읽고 해당 기능을 적용시킴 */
void dvr_setup::GetSystemdata()
{
    memcpy(&systemdatetime.systemtime,&dlgsystemdatetime.systemtime,sizeof(struct tm));
    if(m_bChangeSystemTime)
    {
        SystemDateTimeApplyBtnSlot();
        m_bChangeSystemTime=false;
    }
}
/*!cancel시 system관련 변경적용된 내역을 복원시킴 */
void dvr_setup::ResotreSystem()
{
    memcpy(&systemdatetime.systemtime,&dlgsystemdatetime.systemtime,sizeof(struct tm));
}
/*!system config 초기화 수행 */
void dvr_setup::SystemConfigInit()
{
	//현재 user backup
    USER     temp_user;
    memcpy(&temp_user,&current_user,sizeof(USER));
    //config init(user,profile 초기화)
    ((dvrmain *)(this->parent()))->InitProfile(true);
    WriteDVRsettingInfo((char*)USERLISTFILNAME,&userlist,sizeof(USERLIST));
    WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));
    runtimesettings.iSetupPropertyOn=SETUPPROPERTY_OFF;
    memcpy(&current_user,&temp_user,sizeof(USER));
    profile.iSubOutputEnable=1;
    runtimesettings.iMainSwapEnable = 0;

    pSubToolbar->hide();

    pStatusbar->SetMainWindowSize();

    pTimebar->UpdateTimeTable();
    pTimebar->SetTimeTableChannel(runtimesettings.playbacktable.iTimeTablePageChannel);
    //setup창 hide
    hide();
    if(profile.sequence.iSequenceModeOn)
    {
        pMainClass->MainlayoutTimerStart();
    }
    pToolbar->setEnabled(true);
    if(current_user.iAccessContents & USERACCESS_LIVEMENU)
        pSidebar->setEnabled(true);
    if(current_user.iAccessContents & USERACCESS_MAINMENU)
        pSubToolbar->setEnabled(true);
    if(current_user.iAccessContents & USERACCESS_PLAYBACK)
    {
        pSidebar->setEnabled(true);
        pTimebar->setEnabled(true);
    }
    //live mode set, layout 변경
    pSidebar->setMode();
    pToolbar->MaxPageCalc();
    pSubToolbar->MaxPageCalc();
    pLayoutClass->SetLayoutMode(profile.layout.iLayoutMode);
    pToolbar->SetLayoutModeBtnSelect(profile.layout.iLayoutMode);
    pLayoutClass->DrawOSDChannelLive();
#if DVR_API_ENABLE
    LIB816x_setDisplayLayout(profile.layout.iLayoutMode, 0);
    if(!profile.iSubOutputEnable && runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
    {
        LIB816x_setCameraLayout(profile.layout.iMainOutput, 16, runtimesettings.iLayout);
    }
#endif
    pSubToolbar->SetLayoutModeBtnSelect(profile.layout.iSecondLayoutMode);
#if DVR_API_ENABLE
    LIB816x_setPlaybackDisplayLayout(profile.layout.iSecondLayoutMode, 0);
    if(!profile.iSubOutputEnable && runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
    {
        LIB816x_setCameraLayout(profile.layout.iMainOutput, 16, runtimesettings.iLayout);
    }
#endif
    pLayoutClass->SetBorderLineShowHide(profile.mainosd.iBorderLineOn);
    SetSubOutputEnable();

    //Display
    if((dlgdata.layout.iMainOutput!=profile.layout.iMainOutput || 
     dlgdata.layout.iSubOutput!=profile.layout.iSubOutput)|| 
    (dlgdata.maindisplayoutput.iResolution!=profile.maindisplayoutput.iResolution ||
     dlgdata.maindisplayoutput.iResolution!=runtimesettings.OutputResolution[profile.layout.iMainOutput])/*||
    (dlgdata.subdisplayoutput.iResolution!=profile.subdisplayoutput.iResolution || 
     dlgdata.subdisplayoutput.iResolution!=runtimesettings.OutputResolution[profile.layout.iSubOutput])*/)
    {
        runtimesettings.OutputResolution[profile.layout.iMainOutput]=profile.maindisplayoutput.iResolution;
        runtimesettings.OutputResolution[profile.layout.iSubOutput] = dlgdata.maindisplayoutput.iResolution;//profile.subdisplayoutput.iResolution;
        
        int mainResolution;
        switch(profile.maindisplayoutput.iResolution)
        {
	        case OUTPUT_RESOLUTION_1080P:     
	            mainResolution = DC_MODE_1080P_60;
	            break;
	        case OUTPUT_RESOLUTION_720P:     
	            mainResolution = DC_MODE_720P_60;
	            break;
	        case OUTPUT_RESOLUTION_SXGA:     
	            mainResolution = DC_MODE_SXGA_60;
	            break;
	        case OUTPUT_RESOLUTION_XGA:     
	            mainResolution = DC_MODE_XGA_60;
	            break;
	        case OUTPUT_RESOLUTION_1080P50:     
	            mainResolution = DC_MODE_1080P_50;
	            break;
	        case OUTPUT_RESOLUTION_480P:     
	            mainResolution = DC_MODE_480P;
	            break;
	        case OUTPUT_RESOLUTION_576P:     
	            mainResolution = DC_MODE_576P;
	            break;
	        default:
	            mainResolution = DC_MODE_1080P_60;
	            break;
        }

		int subResolution;
		switch(profile.maindisplayoutput.iResolution)
		{
			case OUTPUT_RESOLUTION_1080P:	  
				subResolution = DC_MODE_1080P_60;
				break;
			case OUTPUT_RESOLUTION_720P:	 
				subResolution = DC_MODE_720P_60;
				break;
			case OUTPUT_RESOLUTION_SXGA:	 
				subResolution = DC_MODE_SXGA_60;
				break;
			case OUTPUT_RESOLUTION_XGA: 	
				subResolution = DC_MODE_XGA_60;
				break;
			case OUTPUT_RESOLUTION_1080P50: 	
				subResolution = DC_MODE_1080P_50;
				break;
			case OUTPUT_RESOLUTION_480P:	 
				subResolution = DC_MODE_480P;
				break;
			case OUTPUT_RESOLUTION_576P:	 
				subResolution = DC_MODE_576P;
				break;
			default:
				subResolution = DC_MODE_1080P_60;
				break;
		}  


        int spotResolution;
        switch(profile.spotdisplayoutput.iResolution)
        {
        case OUTPUT_RESOLUTION_NTSC:
            spotResolution = DC_MODE_NTSC;
            break;
        case OUTPUT_RESOLUTION_PAL:
            spotResolution = DC_MODE_PAL;
            break;
        default:
            spotResolution = DC_MODE_NTSC;
            break;
        }
#if	DVR_API_ENABLE
        LIB816x_setDisplayMainSub(profile.layout.iMainOutput, 
        profile.layout.iSubOutput, DISP_PATH_SD, mainResolution,  
        subResolution, profile.spotdisplayoutput.iResolution);
        pMainClass->SwitchFB(profile.layout.iMainOutput);
#endif
    }

 
    //camera setting
#if DVR_API_ENABLE
    COLORADJUST adjust;
    for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
    {
        if(profile.camera[i].iEnable != dlgdata.camera[i].iEnable)
        {
            LIB816x_setCameraEnable( i, profile.camera[i].iEnable);
        }
        if(profile.camera[i].strcameraname != dlgdata.camera[i].strcameraname)
        {
            LIB816x_changeCamName(i, profile.camera[i].strcameraname);
        }
        if(dlgdata.camera[i].camrec.iResolution!=profile.camera[i].camrec.iResolution)
        {
            LIB816x_setVideoResolution(i, profile.camera[i].camrec.iResolution);
        }

        LIB816x_setMotion(i, profile.camera[i].motion.iMotionOn, profile.camera[i].motion.iSensitivity, profile.camera[i].motion.motiontable);

        adjust.iBrightness = profile.camera[i].coloradjust.iBrightness;
        adjust.iContrast = profile.camera[i].coloradjust.iContrast;
        adjust.iSaturation = profile.camera[i].coloradjust.iSaturation;
        LIB816x_setColorAdjust(i, &adjust);
    }

    //record
    int aVal[MAX_INTERNAL_CAMERA];
    int aFRVal[MAX_INTERNAL_CAMERA];
    int aIFVal[MAX_INTERNAL_CAMERA];
    int ch=0;
    int frch = 0;
    int iframech =0;
    for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
    {
        if(dlgdata.camera[i].camrec.iCodecType!=profile.camera[i].camrec.iCodecType)
        {
            LIB816x_setVideoCodecType(i, profile.camera[i].camrec.iCodecType);
        }

        if(dlgdata.camera[i].camrec.iIFrameInterval!=profile.camera[i].camrec.iIFrameInterval)
        {
            iframech |= (0x01 << i);
            aIFVal[i] = profile.camera[i].camrec.iIFrameInterval;
        }

        if(dlgdata.camera[i].camrec.iBitrateType!=profile.camera[i].camrec.iBitrateType)
        {
            LIB816x_setBitrateType(i, profile.camera[i].camrec.iBitrateType);
        }

        if(dlgdata.camera[i].camrec.iFps!=profile.camera[i].camrec.iFps)
        {
            frch |= (0x01 << i);
            aFRVal[i] = profile.camera[i].camrec.iFps;
        }

        if(dlgdata.camera[i].camrec.ikbps!=profile.camera[i].camrec.ikbps)
        {
            ch |= (0x01 << i);

            aVal[i]= profile.camera[i].camrec.ikbps *1000;
        }

        if((dlgdata.camera[i].recordduration.iPrevRecordOn!=profile.camera[i].recordduration.iPrevRecordOn)
            || (dlgdata.camera[i].recordduration.iPrevRecordDuration!=profile.camera[i].recordduration.iPrevRecordDuration))
        {
            LIB816x_setRecDuration(i, profile.camera[i].recordduration.iPrevRecordOn, profile.camera[i].recordduration.iPrevRecordDuration);
        }

        if(dlgdata.camera[i].camrec.iAudioCodecType!=profile.camera[i].camrec.iAudioCodecType)
        {
            LIB816x_setAudioCodecType(i, profile.camera[i].camrec.iAudioCodecType);
        }
    }
    //bit rate setting for all channel
    if(ch)
        LIB816x_set_encoder_property(LIB_ENC_SET_BITRATE, ch, aVal);
    if(frch)
        LIB816x_set_encoder_property(LIB_ENC_SET_FRAMERATE, frch, aFRVal);
    if(iframech)
        LIB816x_set_encoder_property(LIB_ENC_SET_I_FRAME_INTERVAL, iframech, aIFVal);
    pMainClass->RecScheduleHandler(curtime.currenttime.tm_wday,curtime.currenttime.tm_hour);

    // DIO 
    for(int i=0;i<MAX_SENSOR;i++)
    {
        LIB816x_setSensor(i,profile.sensor[i].iSensorOn,profile.sensor[i].iSensorType);
    }

    for(int i=0;i<MAX_ALARM;i++)
    {
        LIB816x_setAlarm(i,profile.alarm[i].iAlarmOn,profile.alarm[i].iAlarmType,profile.alarm[i].iDelayTime);
    }

    // AUDIO 
    for(int i=0;i<MAX_AUDIOIN;i++)
    {
        if((dlgdata.audioin.audioinch[i].iAudioInOn!=profile.audioin.audioinch[i].iAudioInOn) )
        {
            LIB816x_setAudioInput(i, profile.audioin.audioinch[i].iAudioInOn);
        }
    }

    if(dlgdata.audioin.iSampleRate!=profile.audioin.iSampleRate || (dlgdata.audioin.iVolume!=profile.audioin.iVolume))
    {
        LIB816x_setAudioInputParams(profile.audioin.iSampleRate, profile.audioin.iVolume);
    }

    if(dlgdata.audioout.iDevId != profile.audioout.iDevId)
    {
        LIB816x_setAudioOutputDev(profile.audioout.iDevId);
    }

    if((dlgdata.audioout.iAudioOutOn!=profile.audioout.iAudioOutOn)
        || (dlgdata.audioout.iVolume!=profile.audioout.iVolume)
        || (dlgdata.iSelectChannel != profile.iSelectChannel))
    {
        pMainClass->SetAudioChannel(true, profile.iSelectChannel);
    }
#endif
    memcpy(&dlgdata,&profile,sizeof(PROFILE));
    SetDisplaydata();
    SetCameradata();
    SetRecorddata();
    SetEventdata();
    SetAudiodata();
    SetDiodata();
    SetStoragedata();
    SetSystemdata();
    setEnabled(true);
}
/*!system config 초기화 cancel시 호출 */
void dvr_setup::SystemConfigInitCancel()
{
    setEnabled(true);
}
/*!config init button click시 호출 */
void dvr_setup::SystemConfigInitBtnSlot()
{
    dprintf("SystemConfigInitBtnSlot()\n");
    setEnabled(false);
    // call format confirm dialog 
    m_pConfirmBox->SetConfirmMode(CONFIRMMODE_INIT);
    m_pConfirmBox->show();
    m_pConfirmBox->setEnabled(true);


}
/*!system reboot button click시 호출 */
void dvr_setup::SystemRebootBtnSlot()
{
    dprintf("SystemRebootBtnSlot()\n");
    pMainClass->EventLogAdd((char*)"System reboot",false);
    //stop playback and recording
    pMainClass->BeforeTerminateOperation();
    setEnabled(false);
    sleep(2);
    //send reboot command to front
    LIB816x_systemReboot();
}
/*!system time apply button click시 호출 */
void dvr_setup::SystemDateTimeApplyBtnSlot()
{
    QDateTime qdatetime=ui->systemdatetimesetdateTimeEdit->dateTime();
    time_t settime;
    settime=qdatetime.toTime_t();
    memcpy(&dlgsystemdatetime.systemtime,localtime(&settime),sizeof(struct tm));
    dlgsystemdatetime.systemtime.tm_year=dlgsystemdatetime.systemtime.tm_year+1900;

    char str[100];
    sprintf(str,"system time set %04d.%02d.%02d-%02d:%02d:%02d", dlgsystemdatetime.systemtime.tm_year,
            dlgsystemdatetime.systemtime.tm_mon+1, dlgsystemdatetime.systemtime.tm_mday,
            dlgsystemdatetime.systemtime.tm_hour, dlgsystemdatetime.systemtime.tm_min,
            dlgsystemdatetime.systemtime.tm_sec);
    pMainClass->EventLogAdd(str,true);

    {
        char buf[64];
        sprintf(buf, "/bin/date -s %04d.%02d.%02d-%02d:%02d:%02d", dlgsystemdatetime.systemtime.tm_year,
                        dlgsystemdatetime.systemtime.tm_mon+1, dlgsystemdatetime.systemtime.tm_mday,
                        dlgsystemdatetime.systemtime.tm_hour, dlgsystemdatetime.systemtime.tm_min,
                        dlgsystemdatetime.systemtime.tm_sec);
        system(buf);
        system("/sbin/hwclock -w -f /dev/rtc0");
    }
    m_bChangeSystemTime=false;
    dprintf("SystemDateTimeApplyBtnSlot()\n");
}
/*!update current date/time */
void dvr_setup::UpdateDateTimeCurrent(struct tm *ptm)
{
    struct tm tmcurtime;
    time_t curtime;
    QDateTime qdatetime;
    memcpy(&tmcurtime,ptm,sizeof(struct tm));
    tmcurtime.tm_year=ptm->tm_year-1900;
    curtime = mktime(&tmcurtime);
    qdatetime.setTime_t(curtime);
    ui->systemdatetimecurrentdateTimeEdit->setDateTime(qdatetime);
}
/*!system date/time을 edit창에 setting */
void dvr_setup::UpdateDateTimeSet(struct tm *ptm)
{
    struct tm tmcurtime;
    time_t curtime;
    QDateTime qdatetime;
    memcpy(&tmcurtime,ptm,sizeof(struct tm));
    tmcurtime.tm_year=ptm->tm_year-1900;
    curtime = mktime(&tmcurtime);
    qdatetime.setTime_t(curtime);
    ui->systemdatetimesetdateTimeEdit->setDateTime(qdatetime);
}
/*!current system time update timer start */
void dvr_setup::CurrentTimerStart()
{
     Currenttimer=NULL;
     Currenttimer = new QTimer(this);
     connect(Currenttimer, SIGNAL(timeout()), this, SLOT(CurrentTimerUpdate()));
     Currenttimer->start(MAINTIMER_STEPVALUE);
 }
/*!current system time update time stop */
void dvr_setup::CurrentTimerStop()
{
    if(Currenttimer)
    {
        Currenttimer->stop();
        delete Currenttimer;
    }
}
/*!current system time update */
void dvr_setup::CurrentTimerUpdate()
{
    if(iCurrentTimerOn==1)
        UpdateDateTimeCurrent(&curtime.currenttime);
}

/*!current user에 대하여 ui setting 수행 */
void dvr_setup::SetSystemUserdata()
{
    SetSystemUserListControl();
    //ADMIN만이 user 설정 변경이 가능하도록 
    if(current_user.iAccessLevel==USERLEVEL_ADMIN)
    {
        ui->systemusergroupBox->setEnabled(true);
    }
    else
    {
        ui->systemusergroupBox->setEnabled(false);
    }
}
/*!ui에서 현재 user 설정 정보를 read하고 저장시킴 */
void dvr_setup::GetSystemUserdata()
{
    GetSetupSystemUserListItemdataToUserListdata(m_iUserSelectId);

    int icount=0;
    for(int i=0;i<MAX_USER;i++)
    {
        if(dlguserlist.userlist[i].iEnable==1)
            icount++;
    }
    dlguserlist.usercount=icount;
    memcpy(&userlist,&dlguserlist,sizeof(USERLIST));

    WriteDVRsettingInfo((char*)USERLISTFILNAME,&userlist,sizeof(USERLIST));
}
/*!user tree에 현재 user를 설정 및 user에 대한 data를 ui에 setting */
void dvr_setup::SetSystemUserListControl()
{
    ui->systemuserlistWidget->setCurrentItem (&SetupUserListItem[m_iUserSelectId]);
    SetSetupSystemUserButtonControls(m_iUserSelectId);
}
/*!user tree click시에 호출 */
void dvr_setup::SetupSystemUSerItemClicked(QListWidgetItem *item)
{
    GetSetupSystemUserListItemdataToUserListdata(m_iUserSelectId);
    m_iUserSelectId=ui->systemuserlistWidget->row(item);
    SetSetupSystemUserButtonControls(m_iUserSelectId);
    dprintf("SetupSystemUSerItemClicked(userid=%d)\n",m_iUserSelectId);
}
/*!user tree double click시에 호출 */
void dvr_setup::SetupSystemUSerItemDoubleClicked(QListWidgetItem *item)
{
    SetupSystemUSerItemClicked(item);
}
/*!현재 user에 대한 ui setting을 read */
void dvr_setup::GetSetupSystemUserListItemdataToUserListdata(int iUser)
{
    dlguserlist.userlist[iUser].iEnable=ui->systemuserenable->currentIndex();

    if(iUser==0)
        dlguserlist.userlist[iUser].iAccessLevel=USERLEVEL_ADMIN;
    else
        dlguserlist.userlist[iUser].iAccessLevel=USERLEVEL_USER;

    if(ui->systemuserlevelcontentslivecheckBox->checkState()==Qt::Unchecked)
    {
        dlguserlist.userlist[iUser].iAccessContents&=~USERACCESS_LIVEMENU;
    }
    else if(ui->systemuserlevelcontentslivecheckBox->checkState()==Qt::Checked)
    {
        dlguserlist.userlist[iUser].iAccessContents|=USERACCESS_LIVEMENU;
    }

    if(ui->systemuserlevelcontentsplaybackcheckBox->checkState()==Qt::Unchecked)
    {
        dlguserlist.userlist[iUser].iAccessContents&=~USERACCESS_PLAYBACK;
    }
    else if(ui->systemuserlevelcontentsplaybackcheckBox->checkState()==Qt::Checked)
    {
        dlguserlist.userlist[iUser].iAccessContents|=USERACCESS_PLAYBACK;
    }

    if(ui->systemuserlevelcontentsmainmenucheckBox->checkState()==Qt::Unchecked)
    {
        dlguserlist.userlist[iUser].iAccessContents&=~USERACCESS_MAINMENU;
    }
    else if(ui->systemuserlevelcontentsmainmenucheckBox->checkState()==Qt::Checked)
    {
        dlguserlist.userlist[iUser].iAccessContents|=USERACCESS_MAINMENU;
    }

    if(ui->systemuserlevelcontentssetupcheckBox->checkState()==Qt::Unchecked)
    {
        dlguserlist.userlist[iUser].iAccessContents&=~USERACCESS_SETUPMENU;
    }
    else if(ui->systemuserlevelcontentssetupcheckBox->checkState()==Qt::Checked)
    {
        dlguserlist.userlist[iUser].iAccessContents|=USERACCESS_SETUPMENU;
    }

    char currentpassword[MAX_PASSWORD_CHAR];
    char confirmpassword[MAX_PASSWORD_CHAR];
    memset(currentpassword,0,sizeof(currentpassword));
    memset(confirmpassword,0,sizeof(confirmpassword));

    strcpy(currentpassword,QStringToStr(ui->systemusercurrentpasswordlineEdit->text()));
    strcpy(confirmpassword,QStringToStr(ui->systemuserconfirmpasswordlineEdit->text()));
    if(strcmp(currentpassword,confirmpassword)==0)
    strcpy(dlguserlist.userlist[iUser].password,currentpassword);
}
/*!user에 대한 data를 ui에 setting */
void dvr_setup::SetSetupSystemUserButtonControls(int iUser)
{
    ui->systemuserenable->setCurrentIndex(dlguserlist.userlist[iUser].iEnable);

    if(dlguserlist.userlist[iUser].iAccessContents & USERACCESS_LIVEMENU)
        ui->systemuserlevelcontentslivecheckBox->setCheckState(Qt::Checked);
    else
        ui->systemuserlevelcontentslivecheckBox->setCheckState(Qt::Unchecked);


    if(dlguserlist.userlist[iUser].iAccessContents & USERACCESS_PLAYBACK)
        ui->systemuserlevelcontentsplaybackcheckBox->setCheckState(Qt::Checked);
    else
        ui->systemuserlevelcontentsplaybackcheckBox->setCheckState(Qt::Unchecked);


    if(dlguserlist.userlist[iUser].iAccessContents & USERACCESS_MAINMENU)
        ui->systemuserlevelcontentsmainmenucheckBox->setCheckState(Qt::Checked);
    else
        ui->systemuserlevelcontentsmainmenucheckBox->setCheckState(Qt::Unchecked);


    if(dlguserlist.userlist[iUser].iAccessContents & USERACCESS_SETUPMENU)
        ui->systemuserlevelcontentssetupcheckBox->setCheckState(Qt::Checked);
    else
        ui->systemuserlevelcontentssetupcheckBox->setCheckState(Qt::Unchecked);


    ui->systemusercurrentpasswordlineEdit->setText(QString::fromUtf8(dlguserlist.userlist[iUser].password));
    ui->systemuserconfirmpasswordlineEdit->setText(QString::fromUtf8(""));

    if(iUser==0)
    {
    	//Admin level인경우 - default로 모든 권한을 가짐
        ui->systemuserenable->setEnabled(false);
        ui->systemuserlevelcontentslivecheckBox->setEnabled(false);
        ui->systemuserlevelcontentsplaybackcheckBox->setEnabled(false);
        ui->systemuserlevelcontentsmainmenucheckBox->setEnabled(false);
        ui->systemuserlevelcontentssetupcheckBox->setEnabled(false);

    }
    else
    {
    	//user level인경우 - Content access를 개별적 선택가능
        ui->systemuserenable->setEnabled(true);
        ui->systemuserlevelcontentslivecheckBox->setEnabled(true);
        ui->systemuserlevelcontentsplaybackcheckBox->setEnabled(true);
        ui->systemuserlevelcontentsmainmenucheckBox->setEnabled(true);
        ui->systemuserlevelcontentssetupcheckBox->setEnabled(true);
    }
}
/*!apply change button click시에 호출 */
 void dvr_setup::SystemUserApplySaveBtnSlot()
{
    dprintf("SystemUserApplySaveBtnSlot()\n");
    GetSystemUserdata();
}
/*!change pw virtual keyboard icon click시 호출 */
void dvr_setup::SystemUserCurrentPasswordVirtualKeyboardBtnSlot()
{
    if(pMainClass->ShowVirtualKeyboard())
        ui->systemusercurrentpasswordlineEdit->setFocus();
    dprintf("SystemUserCurrentPasswordVirtualKeyboardBtnSlot()\n");
}
/*!confirm pw virtual keyboard icon click시 호출 */
void dvr_setup::SystemUserConfirmPasswordVirtualKeyboardBtnSlot()
{
    if(pMainClass->ShowVirtualKeyboard())
        ui->systemuserconfirmpasswordlineEdit->setFocus();
    dprintf("SystemUserConfirmPasswordVirtualKeyboardBtnSlot()\n");
}
/*!date/time config time edit창이 변경시에 호출 */
void dvr_setup::on_systemdatetimesetdateTimeEdit_editingFinished()
{
    m_bChangeSystemTime = true;
}
/*!system information read후 ui에 setting */
int dvr_setup::get_sys_info(void)
{
    #if DVR_API_ENABLE
    char mac0[18], mac1[18], hwver[4];

    LIB816x_sys_info(mac0, mac1, hwver);
    strcpy(runtimesettings.versioninfo.FwVer, LIB_MAIN_VERSION);
    strcpy(runtimesettings.versioninfo.HwVer, hwver);
    strcpy(runtimesettings.versioninfo.MacAddr1, mac0);
    strcpy(runtimesettings.versioninfo.MacAddr2, mac1);
    return 0;
    #else
    return 1;
    #endif
}
