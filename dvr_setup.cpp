/******************************************************************************
* DM8168 Hybrid DVR
* Copyright by UDWorks, Incoporated. All Rights Reserved.
*-----------------------------------------------------------------------------
* @file	dvr_setup.cpp
* @brief
* @author
* @section	MODIFY history
*     - 2011.01.01 : First Created
******************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include "dvr_defines.h"
#include "dvr_setup.h"
#include "ui_dvr_setup.h"
#include "dvr_main.h"
#include "time.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define KB			(1024)
#define MB			(KB*KB)
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

SYSTEMDATETIME dlgsystemdatetime;
int MsgBoxTimerCount;
/*----------------------------------------------------------------------------
 local functions
-----------------------------------------------------------------------------*/
/*!constructor*/
dvr_setup::dvr_setup(QWidget *parent) :
        QDialog(parent,Qt::CustomizeWindowHint |Qt::FramelessWindowHint),
    ui(new Ui::dvr_setup)
{
    pFormatThread=NULL;
    FormatMsgtimer=NULL;
    BackupMsgtimer=NULL;
    m_allTabEnable=true;
    m_bOneMonitorDisplay = false;

    ui->setupUi(this);
    MsgBox.setParent(this);
    MsgBox.hide();
    m_pConfirmBox=NULL;

    connect(ui->pb_ok, SIGNAL(clicked()), this, SLOT(SaveExitSlot()));
    connect(ui->pb_cancel, SIGNAL(clicked()), this, SLOT(CancelExitSlot()));
    connect(ui->pb_apply, SIGNAL(clicked()), this, SLOT(ApplySetupBtnSlot()));
    connect(ui->tabsetup, SIGNAL(currentChanged(int)), this, SLOT(ChangeCurrentTabIndex(int)));
    InitSetupDisplayControls();
    InitSetupCameraControls();
    InitSetupRecordControls();
    InitSetupEventControls();
    InitSetupAudioControls();
    InitSetupDioControls();
    InitSetupNetworkControls();
    InitSetupStorageControls();
    InitSetupSystemControls();
#if 0
    InitSetupIPNCControls();
#endif
    InitSetupBackupControls();

    m_bChangeSystemTime=false;
    m_iWarnRecordSelectId = MAX_CAMERA;
}
/*!destructor */
dvr_setup::~dvr_setup()
{
    CurrentTimerStop();
    if(m_pConfirmBox)
        delete m_pConfirmBox;
    m_pConfirmBox=NULL;
    delete ui;
}
/*!front key handler */
void dvr_setup::keyPressEvent ( QKeyEvent * event )
{
    int input=event->key();

    if(input != Qt::Key_Escape)
    {
    }
    else
    {
        pMainClass->keyPressEvent ( event );
    }

}
void dvr_setup::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
/*!show event 발생시 tab 변경 처리 */
void dvr_setup::showEvent ( QShowEvent * event )
{
    if(event)
    {
        dprintf("Setup Dlg Show Event\n");
        ui->tabsetup->setCurrentIndex(m_iSetupTabIndex);
        ChangeCurrentTabIndex(m_iSetupTabIndex);
    }
}
/*!size setting */
void dvr_setup::SetMainWindowSize()
{
    setGeometry((coordinator.MainWindow.width()-width())/2,coordinator.MainWindow.y()+(coordinator.MainWindow.height()-height())/2,width(),height());
    MsgBox.setGeometry ((width()-360)/2,(height()-100)/2,360,100);
}
/*!tab 변경시 호출 기존tab의 data를 read하고 변경tab의 data를 set */
void dvr_setup::ChangeCurrentTabIndex(int index)
{
   dprintf("ChangeCurrentTabIndex=%d\n",index);
    QRect qrect;

    if(m_allTabEnable==false && index!=SETUP_BACKUP)
    {
        //Backup 인경우 다른 tab으로 변경하지 않도록 막음
        printf("set currunt idx SETUP BACKUP\n");
        ui->tabsetup->setCurrentIndex(SETUP_BACKUP);
        m_iSetupTabIndex=SETUP_BACKUP;
        return;
    }

    //먼저 현재tab의 setting을 읽어옴
    switch(m_iSetupTabIndex)
    {
    case SETUP_DISPLAY:
        dlgdata.layout.iMainOutput=ui->outputselectcomboBox->currentIndex();
        if(m_bOneMonitorDisplay)
        {
            dlgdata.layout.iSubOutput=0;
        }
        else
        {
            if(dlgdata.layout.iMainOutput)
                dlgdata.layout.iSubOutput=0;
            else
                dlgdata.layout.iSubOutput=1;
        }
        dlgdata.maindisplayoutput.iResolution=ui->resolutionselectcomboBox->currentIndex();
        //dlgdata.subdisplayoutput.iResolution=ui->resolutionselectcomboBox_2->currentIndex();
        dlgdata.spotdisplayoutput.iResolution=ui->spotresolutionselectcomboBox->currentIndex();
        dlgdata.iSpotOutputCh = ui->spotchselectcomboBox->currentIndex();
        dlgdata.sequence.iInterval=ui->sequenceintervalslider->sliderPosition();
        dlgdata.secondsequence.iInterval=ui->subsequenceintervalslider->sliderPosition();
        dlgdata.mainosd.iBorderLineOn=ui->borderlineselectcomboBox->currentIndex();
        dlgdata.mainosd.iDateFormat=ui->maindateformatcomboBox->currentIndex();
       break;
    case SETUP_CAMERA:
       GetCameraChanneldataToDlgData(m_iCameraSelectId);
#if DVR_API_ENABLE
        if(m_iSetupTabIndex==SETUP_CAMERA)
        {
            LIB816x_endCamProperty();
            if(profile.camera[m_iCameraSelectId].iEnable)
            {
                LIB816x_setCovert(m_iCameraSelectId, profile.camera[m_iCameraSelectId].iCovertOn);
            }
        }
#endif
       break;
    case SETUP_RECORD:
       GetRecordChanneldataToDlgData(m_iRecordSelectId);
       break;
    case SETUP_EVENT:
       break;
    case SETUP_DIO:
       GetSensorItemdataToDlgData(m_iSensorSelectId);
       GetAlarmItemdataToDlgData(m_iAlarmSelectId);
       break;
    case SETUP_AUDIO:
       GetAudioInChanneldataToDlgData(m_iAudioInSelectId);
       dlgdata.audioin.iSampleRate=AUDIO_SAMPLERATE_16KHZ;
       dlgdata.audioout.iAudioOutOn=ui->audiooutenable->currentIndex();
       dlgdata.audioout.iVolume=ui->audiooutvolumeslider->sliderPosition();
       break;
    case SETUP_STORAGE:
       if(m_pConfirmBox)
       {
           delete m_pConfirmBox;
           m_pConfirmBox=NULL;
       }
       break;
    case SETUP_NETWORK:
       {
           dvr_net_info_t inet;

           read_ui_network_data(NETWORK_DEV1_NO, &inet);
           if(g_rfs_type!=RFS_NFS)
           {
               if(inet.type != dlgdata.net_info1.type) {
                   dlgdata.net_info1.type = inet.type;
               }

               if(inet.type == NETWORK_TYPE_STATIC)
               {
                   strcpy(dlgdata.net_info1.ipaddr, inet.ip);
                   strcpy(dlgdata.net_info1.netmask, inet.mask);
                   strcpy(dlgdata.net_info1.gateway, inet.gate);
               }
           }

           read_ui_network_data(NETWORK_DEV2_NO, &inet);
           if(inet.type != dlgdata.net_info2.type) {
               dlgdata.net_info2.type = inet.type;
           }

           if(inet.type == NETWORK_TYPE_STATIC)
           {
               strcpy(dlgdata.net_info2.ipaddr, inet.ip);
               strcpy(dlgdata.net_info2.netmask, inet.mask);
               strcpy(dlgdata.net_info2.gateway, inet.gate);
           }
       }
       break;
    case SETUP_SYSYEM:
       GetSetupSystemUserListItemdataToUserListdata(m_iUserSelectId);
       CurrentTimerStop();
       if(m_pConfirmBox)
       {
           delete m_pConfirmBox;
           m_pConfirmBox=NULL;
       }
       break;
    case SETUP_IPNC:
       break;
    case SETUP_BACKUP:
       break;
    }

    //변경할 tab의 ui setting을 하고 tab 변경..
    switch(index)
    {
    case SETUP_DISPLAY:
        m_iSetupTabIndex=SETUP_DISPLAY;
        SetDisplaydata();
       break;
    case SETUP_CAMERA:
       m_iSetupTabIndex=SETUP_CAMERA;
       for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
       {
           char strcameraname[MAX_TITLE_CHAR];
           memset(strcameraname,0,sizeof(strcameraname));
           sprintf(strcameraname,"%s",profile.camera[i].strcameraname);
           SetupCameraTreeItem[i].setIcon(0,CameraTreeItemIcon);
           SetupCameraTreeItem[i].setText(0,QString::fromUtf8(strcameraname));
           ui->cameratreeWidget->insertTopLevelItem(i,&SetupCameraTreeItem[i]);
       }
       SetCameradata();
#if DVR_API_ENABLE       
       qrect=ui->camerapreviewframe->geometry();
       switch(profile.maindisplayoutput.iResolution)
       {
       case OUTPUT_RESOLUTION_1080P:
           LIB816x_startCamProperty(m_iCameraSelectId, this->x()+qrect.x()-3, this->y()+qrect.y()+26, qrect.width()+1, qrect.height());
           break;
	   case OUTPUT_RESOLUTION_1080P50:
           LIB816x_startCamProperty(m_iCameraSelectId, this->x()+qrect.x()-3, this->y()+qrect.y()+26, qrect.width()+1, qrect.height());
           break;

       case OUTPUT_RESOLUTION_720P:
           LIB816x_startCamProperty(m_iCameraSelectId, (this->x()+qrect.x())*1280/1920-4, (this->y()+qrect.y())*720/1080+19, qrect.width()*1280/1920+1, qrect.height()*720/1080);
           break;
       case OUTPUT_RESOLUTION_SXGA:
           LIB816x_startCamProperty(m_iCameraSelectId, (this->x()+qrect.x())*1280/1920-4, (this->y()+qrect.y())*1024/1080+26, qrect.width()*1280/1920+1, qrect.height()*1024/1080);
           break;
       case OUTPUT_RESOLUTION_XGA:
           LIB816x_startCamProperty(m_iCameraSelectId, (this->x()+qrect.x())*1024/1920-3, (this->y()+qrect.y())*768/1080+20, qrect.width()*1024/1920+1, qrect.height()*768/1080);
           break;

       case OUTPUT_RESOLUTION_480P:
           LIB816x_startCamProperty(m_iCameraSelectId, (this->x()+qrect.x())*704/1920-5, (this->y()+qrect.y())*480/1080+10, qrect.width()*704/1920+1, qrect.height()*480/1080 + 4);
           break;

       case OUTPUT_RESOLUTION_576P:
           LIB816x_startCamProperty(m_iCameraSelectId, (this->x()+qrect.x())*704/1920-5, (this->y()+qrect.y())*576/1080+10, qrect.width()*704/1920+1, qrect.height()*576/1080 + 6);
           break;

       }
#endif       
       break;
    case SETUP_RECORD:
       m_iSetupTabIndex=SETUP_RECORD;
       for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
       {
           char strcameraname[MAX_TITLE_CHAR];
           memset(strcameraname,0,sizeof(strcameraname));
           sprintf(strcameraname,"%s",profile.camera[i].strcameraname);
           SetupRecordTreeItem[i].setIcon(0,RecordTreeItemIcon);
           SetupRecordTreeItem[i].setText(0,QString::fromUtf8(strcameraname));
           ui->recordtreeWidget->insertTopLevelItem(i,&SetupRecordTreeItem[i]);
       }
       SetRecorddata();
       break;
    case SETUP_EVENT:
       m_iSetupTabIndex=SETUP_EVENT;
       SetEventdata();
       break;
    case SETUP_DIO:
       m_iSetupTabIndex=SETUP_DIO;
       SetDiodata();
       break;
    case SETUP_AUDIO:
       m_iSetupTabIndex=SETUP_AUDIO;
       SetAudiodata();
       break;
    case SETUP_STORAGE:
       m_iSetupTabIndex=SETUP_STORAGE;
       if(m_pConfirmBox==NULL)
       {
           m_pConfirmBox = new dvr_formatconfirm;
           m_pConfirmBox->hide();
           m_pConfirmBox->setParent((QWidget*)this->parent());
       }
       on_pb_disk_refresh_clicked();
       SetStoragedata();
       break;
    case SETUP_NETWORK:
       m_iSetupTabIndex=SETUP_NETWORK;
       SetNetworkdata();
       break;
    case SETUP_SYSYEM:
       if(m_pConfirmBox==NULL)
       {
           m_pConfirmBox = new dvr_formatconfirm;
           m_pConfirmBox->hide();
           m_pConfirmBox->setParent((QWidget*)this->parent());
       }
       CurrentTimerStart();
       m_iSetupTabIndex=SETUP_SYSYEM;
       SetDateFormat(dlgdata.mainosd.iDateFormat);
       SetSystemdata();
       break;
    case SETUP_IPNC:
#if 0
       m_iSetupTabIndex=SETUP_IPNC;
       m_iIpncSelectId=0;
       for(int i=0;i<MAX_IPNC_CAMERA;i++)
       {
           char strcameraname[MAX_TITLE_CHAR];
           memset(strcameraname,0,sizeof(strcameraname));
           sprintf(strcameraname,"%s",profile.camera[i+MAX_INTERNAL_CAMERA].strcameraname);
           SetupIpncTreeItem[i].setIcon(0,IpncTreeItemIcon);
           SetupIpncTreeItem[i].setText(0,QString::fromUtf8(strcameraname));
           ui->ipnctreeWidget->insertTopLevelItem(i,&SetupIpncTreeItem[i]);
       }
       SetIPNCdata();
#endif
       break;
    case SETUP_BACKUP:
       SetDateFormat(dlgdata.mainosd.iDateFormat);
       m_iSetupTabIndex=SETUP_BACKUP;
       m_ibackupSelectId=0;
       for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
       {

           BackupTreeItem[i].setIcon(0,BackupTreeItemIcon);
           BackupTreeItem[i].setText(0,QString::fromUtf8(profile.camera[i].strcameraname));
           if(dlgruntimesettings.backupinfo.iDestChannels[i])
               BackupTreeItem[i].setCheckState(0,Qt::Checked);
           else
               BackupTreeItem[i].setCheckState(0,Qt::Unchecked);
           ui->backuptreewidget->insertTopLevelItem(i,&BackupTreeItem[i]);
       }
       memcpy(&backupinfotm,&curtime.currenttime,sizeof(struct tm));
       UpdateBackupInfoDateTime(&backupinfotm);
       SetBackupdata();
       break;
    }
}

/*!ok button click시 호출 ui setting을 read 하여 적용후 setup종료시킴*/
void dvr_setup::SaveExitSlot()
{
    dprintf("SetupProperty SaveExit\n");
    if(m_allTabEnable)
    {
        if(FormatMsgtimer)
        {
            FormatMsgtimer->stop();
            delete FormatMsgtimer;
            FormatMsgtimer=0;
        }
        if(m_pConfirmBox)
        {
            delete m_pConfirmBox;
            m_pConfirmBox=NULL;
        }

        if(m_iSetupTabIndex == SETUP_CAMERA)
        {
    #if DVR_API_ENABLE
            LIB816x_endCamProperty();
            if(profile.camera[m_iCameraSelectId].iEnable)
            {
                LIB816x_setCovert(m_iCameraSelectId, profile.camera[m_iCameraSelectId].iCovertOn);
            }
    #endif
        }
		//모든 tab에 대해 ui data를 읽어옴
        GetDisplaydata();
        SetDisplaydata();
        GetCameradata();
        GetRecorddata();
        GetEventdata();
        GetDiodata();
        GetAudiodata();
        GetStoragedata();
        GetNetworkdata();
        GetSystemdata();
        GetBackupdata();
    #if 0
        GetIPNCdata();
    #endif
    }

    iCurrentTimerOn=0;

    runtimesettings.iSetupPropertyOn=SETUPPROPERTY_OFF;

    MsgBox.hide();
	//profile struct를 netracfg.ini file에 저장
    WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));
    m_bChangeSystemTime=false;

	//setting창 hide, user access level에 따라서 각각 control enable
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
    pSidebar->setMode();
    if(runtimesettings.iSecondConsoleModeOn)
    {
        pSubToolbar->show();
    }
    if(runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
    {
        pTimebar->SetTimeTableUpdate(true,true);
    }

}
/*!cancle button click시 호출 변경된 data를 복구하고 setup 종료시킴 */
void dvr_setup::CancelExitSlot()
{
    if(m_allTabEnable)
    {
        if(FormatMsgtimer)
        {
            FormatMsgtimer->stop();
            delete FormatMsgtimer;
            FormatMsgtimer=0;
        }
        if(m_pConfirmBox)
        {
            delete m_pConfirmBox;
            m_pConfirmBox=NULL;
        }
    #if DVR_API_ENABLE
        if(m_iSetupTabIndex==SETUP_CAMERA)
        {
            LIB816x_endCamProperty();
            if(profile.camera[m_iCameraSelectId].iEnable)
            {
                LIB816x_setCovert(m_iCameraSelectId, profile.camera[m_iCameraSelectId].iCovertOn);
            }
        }
    #endif
    	//temp data로부터 이전값 복구
        memcpy(&dlgdata,&dlgdatatemp,sizeof(PROFILE));
        memcpy(&dlgruntimesettings,&dlgruntimesettingstemp,sizeof(RUNTIMESETTINGS));
        ResotreDisplay();
        ResotreCamera();
        ResotreRecord();
        ResotreEvent();
        ResotreDio();
        ResotreAudio();
        ResotreStorage();
        ResotreNetwork();
        ResotreSystem();
        ResotreBackup();
        memset(&dlgdatatemp,0,sizeof(PROFILE));
        memset(&dlgruntimesettingstemp,0,sizeof(RUNTIMESETTINGS));
 		
 		//ui에 다시 setting 하여줌
        memcpy(&dlgdata,&profile,sizeof(PROFILE));
        SetDisplaydata();
        SetCameradata();
        SetRecorddata();
        SetEventdata();
        SetAudiodata();
        SetDiodata();
        SetNetworkdata();
        SetStoragedata();
        SetSystemdata();
        SetBackupdata();
    }
    iCurrentTimerOn=0;
    dprintf("SetupProperty CancelExit\n");
    MsgBox.hide();

	//setting창 hide, user access level에 따라서 각각 control enable
    m_bChangeSystemTime=false;
    runtimesettings.iSetupPropertyOn=SETUPPROPERTY_OFF;
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
    pSidebar->setMode();
    if(runtimesettings.iSecondConsoleModeOn)
    {
        pSubToolbar->show();
    }
    if(runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
    {
        pTimebar->SetTimeTableUpdate(true,true);
    }
}
/*!apply button click시 호출 현재tab에 변경사항을 적용 */
void dvr_setup::ApplySetupBtnSlot()
{
    if(m_allTabEnable==false)
    {
        return;
    }
	//현재 tab에대한 ui data를 읽어서 적용
    switch(m_iSetupTabIndex)
    {
    case SETUP_DISPLAY:
        GetDisplaydata();
        SetDisplaydata();
    break;
    case SETUP_CAMERA:
        GetCameradata();
        for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
        {
            char strcameraname[MAX_TITLE_CHAR];
            memset(strcameraname,0,sizeof(strcameraname));
            sprintf(strcameraname,"%s",profile.camera[i].strcameraname);
            SetupCameraTreeItem[i].setIcon(0,CameraTreeItemIcon);
            SetupCameraTreeItem[i].setText(0,QString::fromUtf8(strcameraname));
            ui->cameratreeWidget->insertTopLevelItem(i,&SetupCameraTreeItem[i]);
        }
//#if DVR_API_ENABLE
//    LIB816x_selCamPropCh(m_iCameraSelectId);
//#endif
    break;
    case SETUP_RECORD:
        GetRecorddata();
    break;
    case SETUP_EVENT:
        GetEventdata();
    break;
    case SETUP_DIO:
        GetDiodata();
    break;
    case SETUP_AUDIO:
        GetAudiodata();
    break;
    case SETUP_STORAGE:
        GetStoragedata();
    break;
    case SETUP_NETWORK:
        GetNetworkdata();
    break;
    case SETUP_SYSYEM:
        GetSystemdata();
    break;
    case SETUP_IPNC:
#if 0
        GetIPNCdata();
#endif
       break;
    case SETUP_BACKUP:
        GetBackupdata();
       break;
    }
    dprintf("ApplySetupBtnSlot\n");
}

/*!date/time format변경을 UI상에 적용 */
void dvr_setup::SetDateFormat(int iDateFormat)
{
	//설정한 date format에 따라서 setting 내의 time창에 적용시킴
    switch(iDateFormat)
    {
        case OSD_DATEFORMAT_MMDDYYYY_AP:
            ui->systemdatetimecurrentdateTimeEdit->setDisplayFormat(QString::fromUtf8("MM.dd.yyyy. AP hh:mm:ss"));
            ui->systemdatetimesetdateTimeEdit->setDisplayFormat(QString::fromUtf8("MM.dd.yyyy. AP hh:mm:ss"));
            ui->backupstartdateTimeEdit->setDisplayFormat(QString::fromUtf8("MM.dd.yyyy. AP hh:mm:ss"));
            ui->backupenddateTimeEdit->setDisplayFormat(QString::fromUtf8("MM.dd.yyyy. AP hh:mm:ss"));
            ui->backupinfodatetimeedit->setDisplayFormat(QString::fromUtf8("MM.dd.yyyy. AP hh:mm:ss"));
            break;
        case OSD_DATEFORMAT_DDMMYYYY_AP:
            ui->systemdatetimecurrentdateTimeEdit->setDisplayFormat(QString::fromUtf8("dd.MM.yyyy. AP hh:mm:ss"));
            ui->systemdatetimesetdateTimeEdit->setDisplayFormat(QString::fromUtf8("dd.MM.yyyy. AP hh:mm:ss"));
            ui->backupstartdateTimeEdit->setDisplayFormat(QString::fromUtf8("dd.MM.yyyy. AP hh:mm:ss"));
            ui->backupenddateTimeEdit->setDisplayFormat(QString::fromUtf8("dd.MM.yyyy. AP hh:mm:ss"));
            ui->backupinfodatetimeedit->setDisplayFormat(QString::fromUtf8("dd.MM.yyyy. AP hh:mm:ss")); //not work
            break;
        case OSD_DATEFORMAT_YYYYDDMM_AP:
            ui->systemdatetimecurrentdateTimeEdit->setDisplayFormat(QString::fromUtf8("yyyy.dd.MM. AP hh:mm:ss"));
            ui->systemdatetimesetdateTimeEdit->setDisplayFormat(QString::fromUtf8("yyyy.dd.MM. AP hh:mm:ss"));
            ui->backupstartdateTimeEdit->setDisplayFormat(QString::fromUtf8("yyyy.dd.MM. AP hh:mm:ss"));
            ui->backupenddateTimeEdit->setDisplayFormat(QString::fromUtf8("yyyy.dd.MM. AP hh:mm:ss"));
            ui->backupinfodatetimeedit->setDisplayFormat(QString::fromUtf8("yyyy.dd.MM. AP hh:mm:ss"));
            break;
        case OSD_DATEFORMAT_YYYYMMDD_AP:
            ui->systemdatetimecurrentdateTimeEdit->setDisplayFormat(QString::fromUtf8("yyyy.MM.dd. AP hh:mm:ss"));
            ui->systemdatetimesetdateTimeEdit->setDisplayFormat(QString::fromUtf8("yyyy.MM.dd. AP hh:mm:ss"));
            ui->backupstartdateTimeEdit->setDisplayFormat(QString::fromUtf8("yyyy.MM.dd. AP hh:mm:ss"));
            ui->backupenddateTimeEdit->setDisplayFormat(QString::fromUtf8("yyyy.MM.dd. AP hh:mm:ss"));
            ui->backupinfodatetimeedit->setDisplayFormat(QString::fromUtf8("yyyy.MM.dd. AP hh:mm:ss"));
            break;
        case OSD_DATEFORMAT_MMDDYYYY:
            ui->systemdatetimecurrentdateTimeEdit->setDisplayFormat(QString::fromUtf8("MM.dd.yyyy. hh:mm:ss"));
            ui->systemdatetimesetdateTimeEdit->setDisplayFormat(QString::fromUtf8("MM.dd.yyyy. hh:mm:ss"));
            ui->backupstartdateTimeEdit->setDisplayFormat(QString::fromUtf8("MM.dd.yyyy. hh:mm:ss"));
            ui->backupenddateTimeEdit->setDisplayFormat(QString::fromUtf8("MM.dd.yyyy. hh:mm:ss"));
            ui->backupinfodatetimeedit->setDisplayFormat(QString::fromUtf8("MM.dd.yyyy. hh:mm:ss"));
            break;
        case OSD_DATEFORMAT_DDMMYYYY:
            ui->systemdatetimecurrentdateTimeEdit->setDisplayFormat(QString::fromUtf8("dd.MM.yyyy. hh:mm:ss"));
            ui->systemdatetimesetdateTimeEdit->setDisplayFormat(QString::fromUtf8("dd.MM.yyyy. hh:mm:ss"));
            ui->backupstartdateTimeEdit->setDisplayFormat(QString::fromUtf8("dd.MM.yyyy. hh:mm:ss"));
            ui->backupenddateTimeEdit->setDisplayFormat(QString::fromUtf8("dd.MM.yyyy. hh:mm:ss"));
            ui->backupinfodatetimeedit->setDisplayFormat(QString::fromUtf8("dd.MM.yyyy. hh:mm:ss"));
            break;
        case OSD_DATEFORMAT_YYYYDDMM:
            ui->systemdatetimecurrentdateTimeEdit->setDisplayFormat(QString::fromUtf8("yyyy.dd.MM. hh:mm:ss"));
            ui->systemdatetimesetdateTimeEdit->setDisplayFormat(QString::fromUtf8("yyyy.dd.MM. hh:mm:ss"));
            ui->backupstartdateTimeEdit->setDisplayFormat(QString::fromUtf8("yyyy.dd.MM. hh:mm:ss"));
            ui->backupenddateTimeEdit->setDisplayFormat(QString::fromUtf8("yyyy.dd.MM. hh:mm:ss"));
            ui->backupinfodatetimeedit->setDisplayFormat(QString::fromUtf8("yyyy.dd.MM. hh:mm:ss"));
            break;
        case OSD_DATEFORMAT_YYYYMMDD:
            ui->systemdatetimecurrentdateTimeEdit->setDisplayFormat(QString::fromUtf8("yyyy.MM.dd. hh:mm:ss"));
            ui->systemdatetimesetdateTimeEdit->setDisplayFormat(QString::fromUtf8("yyyy.MM.dd. hh:mm:ss"));
            ui->backupstartdateTimeEdit->setDisplayFormat(QString::fromUtf8("yyyy.MM.dd. hh:mm:ss"));
            ui->backupenddateTimeEdit->setDisplayFormat(QString::fromUtf8("yyyy.MM.dd. hh:mm:ss"));
            ui->backupinfodatetimeedit->setDisplayFormat(QString::fromUtf8("yyyy.MM.dd. hh:mm:ss"));
            break;
    }
}
