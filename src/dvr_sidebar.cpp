#include "dvr_sidebar.h"
#include "ui_dvr_sidebar.h"
#include "dvr_defines.h"
#include "dvr_main.h"
#include <QTextCharFormat>

extern dvrmain *pMainClass;
extern dvr_layout_main *pLayoutClass;
extern dvr_timelinebar_play *pTimebar;
extern struct tm playbackupdatetime;

const char *strplaybackspeed[MAX_PLAYBACKSPEED]={
    STRPLAYBACKSPEED_1DIVIDE4X,
    STRPLAYBACKSPEED_1DIVIDE2X,
    STRPLAYBACKSPEED_1X,
    STRPLAYBACKSPEED_2X,
    STRPLAYBACKSPEED_4X,
    STRPLAYBACKSPEED_8X,
    STRPLAYBACKSPEED_16X,
    STRPLAYBACKSPEED_32X
};

int m_iDataBit=-1;
int m_iParityBit=-1;
int m_iStopBit=-1;
int m_iBaudRate=-1;

/*!constructor*/
dvr_sidebar::dvr_sidebar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dvr_sidebar)
{
    int i=0;
    
    m_Mode = LIB_LIVE_MODE;
    m_iPlaybackMode=PLAYBACKMODE_STOP;
    m_iPlaybackSpeed=PLAYBACKSPEED_1X;
    m_iplayonchange=0;
    bCalendarPageChange=false;
    ui->setupUi(this);
    
    QSize qsize;
    qsize.setWidth(CAMERATREEICON_SIZE_W);
    qsize.setHeight(CAMERATREEICON_SIZE_H);
    CameraTreeItemIcon.addFile(QString::fromUtf8(CAMERA_TREE_CAMERA_ICON), qsize, QIcon::Normal, QIcon::Off);

    for(i=0;i<MAX_CAMERA;i++)
    {
        CameraTreeItem[i].setIcon(0,CameraTreeItemIcon);
        CameraTreeItem[i].setText(0,QString::fromUtf8(profile.camera[i].strcameraname));
        CameraTreeItem[i].setCheckState(0,Qt::Unchecked);
        ui->dvrmainsidebartreewidget->insertTopLevelItem(i,&CameraTreeItem[i]);
    }
    connect(ui->dvrmainsidebartreewidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(TreeItemClicked(QTreeWidgetItem *, int)));
    connect(ui->dvrmainsidebartreewidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(TreeItemDoubleClicked(QTreeWidgetItem *, int)));
	ui->fm_logo->setGeometry(0,990,200,51);
    ui->ptzframe->setGeometry(0,830,200,160);
    ui->dioframe->setGeometry(0,690,200,141);
    ui->dvrmainsidebarlistwidget->setGeometry(0,490,200,200);
    ui->dvrmainsidebartreewidget->setGeometry(0,35,200,461);
    ui->label->setGeometry(0,0,200,35);

    ui->playbackframe->setGeometry(0,740,200,250);
    ui->searchframe->setGeometry(0,530,200,210);
    ui->search_btn->setGeometry(0,490,200,40);

    QSensorOnIconLabel[0]=ui->sensoricon1_h;
    QSensorOnIconLabel[1]=ui->sensoricon2_h;
    QSensorOnIconLabel[2]=ui->sensoricon3_h;
    QSensorOnIconLabel[3]=ui->sensoricon4_h;
    QSensorOnIconLabel[4]=ui->sensoricon5_h;
    QSensorOnIconLabel[5]=ui->sensoricon6_h;
    QSensorOnIconLabel[6]=ui->sensoricon7_h;
    QSensorOnIconLabel[7]=ui->sensoricon8_h;
    QSensorOnIconLabel[8]=ui->sensoricon9_h;
    QSensorOnIconLabel[9]=ui->sensoricon10_h;
    QSensorOnIconLabel[10]=ui->sensoricon11_h;
    QSensorOnIconLabel[11]=ui->sensoricon12_h;
    QSensorOnIconLabel[12]=ui->sensoricon13_h;
    QSensorOnIconLabel[13]=ui->sensoricon14_h;
    QSensorOnIconLabel[14]=ui->sensoricon15_h;
    QSensorOnIconLabel[15]=ui->sensoricon16_h;

    QAlarmOnIconLabel[0]=ui->alarmicon1_h;
    QAlarmOnIconLabel[1]=ui->alarmicon2_h;
    QAlarmOnIconLabel[2]=ui->alarmicon3_h;
    QAlarmOnIconLabel[3]=ui->alarmicon4_h;

    for(int i=0;i<MAX_SENSOR;i++)
    {
        QSensorOnIconLabel[i]->hide();
    }
    for(int i=0;i<MAX_ALARM;i++)
    {
        QAlarmOnIconLabel[i]->hide();
    }
    
    connect(ui->ptz_up_btn, SIGNAL(pressed()), this, SLOT(ptzUpBtnSlot()));
    connect(ui->ptz_up_btn, SIGNAL(released()), this, SLOT(ptzStopSlot()));
    connect(ui->ptz_down_btn, SIGNAL(pressed()), this, SLOT(ptzDownBtnSlot()));
    connect(ui->ptz_down_btn, SIGNAL(released()), this, SLOT(ptzStopSlot()));
    connect(ui->ptz_left_btn, SIGNAL(pressed()), this, SLOT(ptzLeftBtnSlot()));
    connect(ui->ptz_left_btn, SIGNAL(released()), this, SLOT(ptzStopSlot()));
    connect(ui->ptz_right_btn, SIGNAL(pressed()), this, SLOT(ptzRightBtnSlot()));
    connect(ui->ptz_right_btn, SIGNAL(released()), this, SLOT(ptzStopSlot()));
    connect(ui->ptz_zoomin_btn, SIGNAL(pressed()), this, SLOT(ptzZoomInBtnSlot()));
    connect(ui->ptz_zoomin_btn, SIGNAL(released()), this, SLOT(ptzStopSlot()));
    connect(ui->ptz_zoomout_btn, SIGNAL(pressed()), this, SLOT(ptzZoomOutBtnSlot()));
    connect(ui->ptz_zoomout_btn, SIGNAL(released()), this, SLOT(ptzStopSlot()));
    connect(ui->ptz_focusin_btn, SIGNAL(pressed()), this, SLOT(ptzFocusInBtnSlot()));
    connect(ui->ptz_focusin_btn, SIGNAL(released()), this, SLOT(ptzStopSlot()));
    connect(ui->ptz_focusout_btn, SIGNAL(pressed()), this, SLOT(ptzFocusOutBtnSlot()));
    connect(ui->ptz_focusout_btn, SIGNAL(released()), this, SLOT(ptzStopSlot()));

    ui->play_pause_btn->hide();
    ui->play_stop_btn->setEnabled(false);
    connect(ui->dvrmainsidebarplaybackcalendar, SIGNAL(selectionChanged()), this, SLOT(playbackCalendarSelectionChangedSlot()));
    connect(ui->dvrmainsidebarplaybackcalendar, SIGNAL(currentPageChanged(int,int)), this, SLOT(playbackCalendarcurrentPageChangedSlot(int,int)));

    connect(ui->search_btn, SIGNAL(clicked()), this, SLOT(searchBtnSlot()));
    connect(ui->play_reverse_btn, SIGNAL(clicked()), this, SLOT(playbackReverseBtnSlot()));
    connect(ui->play_reversestep_btn, SIGNAL(clicked()), this, SLOT(playbackReverseStepBtnSlot()));
    connect(ui->play_play_btn, SIGNAL(clicked()), this, SLOT(playbackPlayBtnSlot()));
    connect(ui->play_pause_btn, SIGNAL(clicked()), this, SLOT(playbackPauseBtnSlot()));
    connect(ui->play_stop_btn, SIGNAL(clicked()), this, SLOT(playbackStopBtnSlot()));
    connect(ui->play_forwardstep_btn, SIGNAL(clicked()), this, SLOT(playbackForwardStepBtnSlot()));
    connect(ui->play_forward_btn, SIGNAL(clicked()), this, SLOT(playbackForwardBtnSlot()));
    connect(ui->playspeeddial, SIGNAL(valueChanged(int)), this, SLOT(playbackSpeedChangeSlot(int)));
}

/*!destructor */
dvr_sidebar::~dvr_sidebar()
{
    if(Dialtimer)
    {
        Dialtimer->stop();
        delete Dialtimer;
        Dialtimer=NULL;
    }
    delete ui;
}

void dvr_sidebar::changeEvent(QEvent *e)
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
/*!playback speed dial timer */
void dvr_sidebar::dialTimerUpdate()
{
    Dialtimer->stop();
    if(ui->playspeeddial->isEnabled())
    {
        setPlayBackSpeedChange();
        if(m_iPlaybackSpeed != ui->playspeeddial->value())
        {
            ui->playspeeddial->setValue(m_iPlaybackSpeed);
        }
    }
}

/*!front key handler */
void dvr_sidebar::keyPressEvent ( QKeyEvent * event )
{
    printf("keyPressEvent key=0x%X\n",event->key());
    switch(event->key())
    {
        case Qt::Key_1:
        case Qt::Key_2:
        case Qt::Key_3:
        case Qt::Key_4:
        case Qt::Key_5:
        case Qt::Key_6:
        case Qt::Key_7:
        case Qt::Key_8:
        case Qt::Key_9:
            if(ui->dvrmainsidebartreewidget->isEnabled())
            {
               changeTreeItem(event->key()-Qt::Key_1,true);
            }
        return;
        case Qt::Key_A:
        case Qt::Key_B:
        case Qt::Key_C:
        case Qt::Key_D:
        case Qt::Key_E:
        case Qt::Key_F:
        case Qt::Key_G:
            if(ui->dvrmainsidebartreewidget->isEnabled())
            {
               changeTreeItem(event->key()-Qt::Key_A+9,true);
            }
        return;
        case Qt::Key_Up:
            if(m_Mode == LIB_LIVE_MODE)
            {
                if(ui->dvrmainsidebartreewidget->isEnabled())
                {
                    if(profile.iSelectChannel<=0)
                         changeTreeItem(0,false);
                    else
                        changeTreeItem(profile.iSelectChannel-1,false);
                }
            }
            else
            {
                if(ui->dvrmainsidebartreewidget->isEnabled())
                {
                    if(profile.iSecondSelectChannel<=0)
                         changeTreeItem(0,false);
                    else
                        changeTreeItem(profile.iSecondSelectChannel-1,false);
                }
            }
        return;
        case Qt::Key_Down:
            if(m_Mode == LIB_LIVE_MODE)
            {
                if(ui->dvrmainsidebartreewidget->isEnabled())
                {
                    if(profile.iSelectChannel>=MAX_CAMERA-1)
                         changeTreeItem(MAX_CAMERA-1,false);
                    else
                        changeTreeItem(profile.iSelectChannel+1,false);
                }
            }
            else
            {
                if(ui->dvrmainsidebartreewidget->isEnabled())
                {
                    if(profile.iSecondSelectChannel>=MAX_CAMERA-1)
                         changeTreeItem(MAX_CAMERA-1,false);
                    else
                        changeTreeItem(profile.iSecondSelectChannel+1,false);
                }
            }
        return;
        case Qt::Key_Return:
            if(ui->dvrmainsidebartreewidget->isEnabled())
                changeTreeItem(profile.iSecondSelectChannel,true);
        return;
        case Qt::Key_O:     //RF
            setFocus();
            if(runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
            {
                setPlayBackMode(PLAYBACKMODE_REVERSE);
				setPlayBackSpeedChange();
    			ui->playspeedlabel->setText(setPlaySpeedValueText(4));
            }
        return;
        case Qt::Key_P:     //RSTEP
            setFocus();
            if(runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
            {
            	if(runtimesettings.playbackinfo.playbackmode.iPlaybackMode == PLAYBACKMODE_PLAY)
        		{
        			setPlayBackMode(PLAYBACKMODE_PUASE);
            	}
                setPlayBackMode(PLAYBACKMODE_REVERSESTEP);
            }
        return;
        case Qt::Key_Q:     //PLAYPAUSE
            setFocus();
            if(runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
            {
//#if 0
                if(runtimesettings.playbackinfo.playbackmode.iPlaybackMode==PLAYBACKMODE_PLAY)
                    setPlayBackMode(PLAYBACKMODE_PUASE);
                else
//#endif
                    setPlayBackMode(PLAYBACKMODE_PLAY);
					ui->playspeedlabel->setText(setPlaySpeedValueText(2));
            }
        return;
        case Qt::Key_R:     //FSTEP
            setFocus();
            if(runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
            {
            	if(runtimesettings.playbackinfo.playbackmode.iPlaybackMode == PLAYBACKMODE_PLAY)
       			{
       				setPlayBackMode(PLAYBACKMODE_PUASE);
            	}
                setPlayBackMode(PLAYBACKMODE_FORAWRDSTEP);
            }
        return;
        case Qt::Key_S:     //FF
            setFocus();
            if(runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
            {
                setPlayBackMode(PLAYBACKMODE_FORWARD);
				setPlayBackSpeedChange();
    			ui->playspeedlabel->setText(setPlaySpeedValueText(4));
            }
        return;
        case Qt::Key_T:     //STOP
            setFocus();
            if(runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
            {
                setPlayBackMode(PLAYBACKMODE_STOP);
            }
        return;
        case Qt::Key_PageDown:
            if(runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
            {
                m_iPlaybackSpeed--;
                if(m_iPlaybackSpeed<=PLAYBACKSPEED_1DIVIDE4X)
                    m_iPlaybackSpeed=PLAYBACKSPEED_1DIVIDE4X;
                ui->playspeeddial->setValue(m_iPlaybackSpeed);
                ui->playspeedlabel->setText(setPlaySpeedValueText(m_iPlaybackSpeed));
                if(Dialtimer)
                {
                    Dialtimer->stop();
                    delete Dialtimer;
                    Dialtimer=NULL;
                }
                Dialtimer = new QTimer(this);
                connect(Dialtimer, SIGNAL(timeout()), this, SLOT(dialTimerUpdate()));
                Dialtimer->start(300);
            }
        return;
        case Qt::Key_PageUp:
            if(runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
            {
                m_iPlaybackSpeed++;
                if(m_iPlaybackSpeed>=PLAYBACKSPEED_32X)
                    m_iPlaybackSpeed=PLAYBACKSPEED_32X;
                ui->playspeeddial->setValue(m_iPlaybackSpeed);
                ui->playspeedlabel->setText(setPlaySpeedValueText(m_iPlaybackSpeed));
                if(Dialtimer)
                {
                    Dialtimer->stop();
                    delete Dialtimer;
                    Dialtimer=NULL;
                }
                Dialtimer = new QTimer(this);
                connect(Dialtimer, SIGNAL(timeout()), this, SLOT(dialTimerUpdate()));
                Dialtimer->start(300);
            }
        return;
        case Qt::Key_Home:  //Right 70
        return;
        case Qt::Key_F1:    //Right 60
        return;
        case Qt::Key_F2:    //Right 50
        return;
        case Qt::Key_F3:    //Right 40
        return;
        case Qt::Key_F4:    //Right 30
        return;
        case Qt::Key_F5:    //Right 20
        return;
        case Qt::Key_F6:    //Right 10
        if(runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
            {
                setPlayBackMode(PLAYBACKMODE_FORWARD);
				setPlayBackSpeedChange();
    			ui->playspeedlabel->setText(setPlaySpeedValueText(4));
            }
        return;
        case Qt::Key_Delete:    //0
        if(runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
            {
//#if 0
                if(runtimesettings.playbackinfo.playbackmode.iPlaybackMode==PLAYBACKMODE_PLAY)
                    setPlayBackMode(PLAYBACKMODE_PUASE);
                else
//#endif
                    setPlayBackMode(PLAYBACKMODE_STOP);
					ui->playspeedlabel->setText(setPlaySpeedValueText(2));
            }
        return;
        case Qt::Key_F7:    //left 10
        if(runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
            {
                setPlayBackMode(PLAYBACKMODE_REVERSE);
				setPlayBackSpeedChange();
    			ui->playspeedlabel->setText(setPlaySpeedValueText(4));
            }
        return;
        case Qt::Key_F8:    //left 20
        return;
        case Qt::Key_F9:    //left 30
        return;
        case Qt::Key_F10:   //left 40
        return;
        case Qt::Key_F11:   //left 50
        return;
        case Qt::Key_F12:   //left 60
        return;
        case Qt::Key_End:   //left 70
        return;
    }

    pMainClass->keyPressEvent(event);
}
/*!좌표 및 camera tree 초기화 */
void dvr_sidebar::init()
{
    this->setGeometry(coordinator.Sidebar);

    setSideBarTreeControl();
    setSideBarPtzControls();
}
/*!Live/Playback mode 전환*/
void dvr_sidebar::setMode()
{
    m_Mode = runtimesettings.iLivePlaybackMode;
    if(m_Mode == LIB_LIVE_MODE)
    {
    	//show log,sensor/alram,ptz control
    	//hide search,calendar,playback control
        ui->label->setText("LIVE");
        ui->dvrmainsidebarlistwidget->show();
        ui->ptzframe->show();
        ui->dioframe->show();

        ui->playbackframe->hide();
        ui->searchframe->hide();
        ui->search_btn->hide();
        //enable/disable each control by user access level
        if(current_user.iAccessLevel==USERLEVEL_USER)
        {
            if(current_user.iAccessContents & USERACCESS_LIVEMENU)
            {
                ui->dvrmainsidebartreewidget->setEnabled(true);
                ui->dvrmainsidebarlistwidget->setEnabled(true);
                ui->ptzframe->setEnabled(true);
            }
            else
            {
                ui->dvrmainsidebartreewidget->setEnabled(false);
                ui->dvrmainsidebarlistwidget->setEnabled(false);
                ui->ptzframe->setEnabled(false);
            }
        }
    }
    else
    {
    	//show search,calendar,playback control
    	//hide log,sensor/alram, ptz control
        ui->label->setText("PLAY");
        ui->dvrmainsidebarlistwidget->hide();
        ui->ptzframe->hide();
        ui->dioframe->hide();

        ui->playbackframe->show();
		ui->play_forward_btn->setEnabled(true);
        ui->searchframe->show();
        ui->search_btn->show();
        //enable/disable each control by user access level
        if(current_user.iAccessLevel==USERLEVEL_USER)
        {
            if(current_user.iAccessContents & USERACCESS_PLAYBACK)
            {
                ui->dvrmainsidebartreewidget->setEnabled(true);
                ui->playbackframe->setEnabled(true);
                ui->searchframe->setEnabled(true);
                ui->search_btn->setEnabled(true);
            }
            else
            {
                ui->dvrmainsidebartreewidget->setEnabled(false);
                ui->playbackframe->setEnabled(false);
                ui->searchframe->setEnabled(false);
                ui->search_btn->setEnabled(false);
            }
        }
    }
	//camera tree change by live/playback mode
    setSideBarTreeControl();
}

//Camera Tree Control 
/*!live/playback mode에 따라서 camera tree setting */
void dvr_sidebar::setSideBarTreeControl()
{
    int i=0;
    for(i=0;i<MAX_CAMERA;i++)
    {
        CameraTreeItem[i].setText(0,QString::fromUtf8(profile.camera[i].strcameraname));
        if(m_Mode == LIB_LIVE_MODE)
        {
        	//check status set by profile.camera[i].iEnable
            if(profile.camera[i].iEnable==0)
            {

                CameraTreeItem[i].setCheckState(0,Qt::Unchecked);
            }
            else
            {
                CameraTreeItem[i].setCheckState(0,Qt::Checked);
            }
        }
        else
        {
        	//check status set by profile.playbackcamera[i].iEnable
            if(profile.playbackcamera[i].iEnable==0)
            {
                CameraTreeItem[i].setCheckState(0,Qt::Unchecked);
            }
            else
            {
                CameraTreeItem[i].setCheckState(0,Qt::Checked);
            }
        }
    }
    //set current selected channel
    if(m_Mode == LIB_LIVE_MODE)
    {
        ui->dvrmainsidebartreewidget->setCurrentItem (&CameraTreeItem[profile.iSelectChannel]);
    }
    else
    {
        ui->dvrmainsidebartreewidget->setCurrentItem (&CameraTreeItem[profile.iSecondSelectChannel]);
    }
}
/*!camera tree가 click될때 호출 */
void dvr_sidebar::TreeItemClicked(QTreeWidgetItem *item, int column)
{

    int iselect=0;
    iselect=ui->dvrmainsidebartreewidget->indexOfTopLevelItem(item);
    if(m_Mode == LIB_LIVE_MODE)
    {
    	//update selected channel 
        profile.iSelectChannel=iselect;
        //change camera enable by chech status
        if(item->checkState(column)==Qt::Unchecked)
        {
            if(profile.camera[iselect].iEnable!=0)
            {
                profile.camera[iselect].iEnable=0;
#if DVR_API_ENABLE
                LIB816x_setCameraEnable( iselect, profile.camera[iselect].iEnable);
#endif
                pLayoutClass->DrawOSDChannelLive();
            }
        }
        else
        {
            if(profile.camera[iselect].iEnable!=1)
            {
                profile.camera[iselect].iEnable=1;
#if DVR_API_ENABLE
                LIB816x_setCameraEnable( iselect, profile.camera[iselect].iEnable);
#endif
                pLayoutClass->DrawOSDChannelLive();
            }
        }
        //change audio selected channel
        pMainClass->SetAudioChannel(false,profile.iSelectChannel);
        //ptz channel update
        setSideBarPtzControls();
    }
    else
    {
    	//update selected channel
        profile.iSecondSelectChannel = iselect;
        //change camera enable by check status
        if(item->checkState(column)==Qt::Unchecked)
        {
            if(profile.playbackcamera[iselect].iEnable!=0)
            {
                profile.playbackcamera[iselect].iEnable=0;
#if DVR_API_ENABLE
                LIB816x_setCameraEnable(MAX_INTERNAL_CAMERA+iselect, profile.playbackcamera[iselect].iEnable);
#endif
                pLayoutClass->DrawOSDChannelPlayback();
            }
        }
        else
        {
            if(profile.playbackcamera[iselect].iEnable!=1)
            {
                profile.playbackcamera[iselect].iEnable=1;
#if DVR_API_ENABLE
                LIB816x_setCameraEnable(MAX_INTERNAL_CAMERA+iselect, profile.playbackcamera[iselect].iEnable);
#endif
                pLayoutClass->DrawOSDChannelPlayback();
            }
        }
        //change audio selected channel
        pMainClass->SetAudioChannel(false,MAX_CAMERA+profile.iSecondSelectChannel);
    }
    WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));
}
/*!camera tree가 double click 될때 호출 */
void dvr_sidebar::TreeItemDoubleClicked(QTreeWidgetItem *item, int column)
{
	//toggle check status
    if(item->checkState(column)==Qt::Unchecked)
    {
        item->setCheckState(column,Qt::Checked);
    }
    else
    {
        item->setCheckState(column,Qt::Unchecked);
    }

    TreeItemClicked(item,column);
}
/*!front key에 의해서 camera tree가 변경될때 호출 */
void dvr_sidebar::changeTreeItem(int column,bool bChangeValue)
{
    int cameraIndex = column;

    if(m_Mode == LIB_LIVE_MODE)
    {
        profile.iSelectChannel=cameraIndex;
        if(bChangeValue)
        {
        	//toggle camera enable
            profile.camera[cameraIndex].iEnable = !profile.camera[cameraIndex].iEnable;
            //update check status
            if(profile.camera[cameraIndex].iEnable==0)
            {
                CameraTreeItem[cameraIndex].setCheckState(0,Qt::Unchecked);
            }
            else
            {
                CameraTreeItem[cameraIndex].setCheckState(0,Qt::Checked);
            }
            WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));
#if DVR_API_ENABLE
            LIB816x_setCameraEnable(cameraIndex, profile.camera[cameraIndex].iEnable);
#endif
            pLayoutClass->DrawOSDChannelLive();
        }
        //update audio channel
        pMainClass->SetAudioChannel(false,profile.iSelectChannel);
    }
    else
    {
        profile.iSecondSelectChannel=cameraIndex;
        if(bChangeValue)
        {
            profile.playbackcamera[cameraIndex].iEnable = !profile.playbackcamera[cameraIndex].iEnable;
            if(profile.playbackcamera[cameraIndex].iEnable==0)
            {
                CameraTreeItem[cameraIndex].setCheckState(0,Qt::Unchecked);
            }
            else
            {
                CameraTreeItem[cameraIndex].setCheckState(0,Qt::Checked);
            }
            WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));
#if DVR_API_ENABLE
            LIB816x_setCameraEnable(MAX_INTERNAL_CAMERA+cameraIndex, profile.playbackcamera[cameraIndex].iEnable);
#endif
            pLayoutClass->DrawOSDChannelPlayback();
        }
        pMainClass->SetAudioChannel(false,MAX_CAMERA+profile.iSecondSelectChannel);
    }
    //update current selected channel
    ui->dvrmainsidebartreewidget->setCurrentItem(&CameraTreeItem[cameraIndex]);
}

//Event Log Control 
/*!sidebar의 log창에 log를 추가 */
void dvr_sidebar::addEventlogItem(char *str)
{
    QString qstr;
    qstr=StrToQString(str);
    int icount=ui->dvrmainsidebarlistwidget->count();
    if(icount>=MAX_EVENTLOG_SHOWCOUNT)
    {
    	//remove oldest log
        ui->dvrmainsidebarlistwidget->removeItemWidget(ui->dvrmainsidebarlistwidget->takeItem(icount-1));
    }
    //add log
    ui->dvrmainsidebarlistwidget->insertItem(0,qstr);
}
/*!sidebar log창의 모든 log를 삭제 */
void dvr_sidebar::clearEventlog()
{
	//erase all log
    ui->dvrmainsidebarlistwidget->clear();
}

//Sensor Alram Control 
/*!sensor의 detect status에 따라서 sensor icon표시 변경 */
void dvr_sidebar::setSensorIconStatus(int iSensorId,int iSensorDetect)
{
    if((iSensorId<0)||(iSensorId>=MAX_SENSOR))
        return;

    if(iSensorDetect==SENSOR_DETECT_OFF)
    {
    	//sensor on icon hide
        QSensorOnIconLabel[iSensorId]->hide();
    }
    else if(iSensorDetect==SENSOR_DETECT_ON)
    {
    	//sensor on icon show
        QSensorOnIconLabel[iSensorId]->show();
    }
}
/*!alram의 detect status에 따라서 alram icon표시 변경 */
void dvr_sidebar::setAlarmIconStatus(int iAlarmId,int iAlarmDetect)
{
    if((iAlarmId<0)||(iAlarmId>=MAX_ALARM))
        return;


    if(iAlarmDetect==ALARM_DETECT_OFF)
    {
    	//alram on icon hide
        QAlarmOnIconLabel[iAlarmId]->hide();
    }
    else if(iAlarmDetect==ALARM_DETECT_ON)
    {
    	//alram on icon show
        QAlarmOnIconLabel[iAlarmId]->show();
    }
}

//PTZ Control 
/*!camera tree의 선택 channel 변경시 ptz에 channel number update */
void dvr_sidebar::setSideBarPtzControls()
{
	//update selected channel number
    char m_ptzchannelnum[5];
    sprintf(m_ptzchannelnum,"%d",profile.iSelectChannel+1);
    QString qstr;
    qstr=StrToQString(m_ptzchannelnum);
    ui->ptz_channel->setText(qstr);
}
/*!get current baud rate */
int dvr_sidebar::ptzGetBuadRate(int chid)
{
    int BaudRate=0;
    switch(profile.camera[chid].ptzinfo.iBaudRate)
    {
    case 0:
        BaudRate = 1200;
        break;
    case 1:
        BaudRate = 2400;
        break;
    case 2:
        BaudRate = 4800;
        break;
    case 3:
        BaudRate = 9600;
        break;
    case 4:
        BaudRate = 38400;
        break;
    case 5:
        BaudRate = 57600;
        break;
    case 6:
        BaudRate = 115200;
        break;
    }
    return BaudRate;
}
/*!ptz up button press인경우 연속 호출 */
void dvr_sidebar::ptzUpBtnSlot()
{
   if(profile.iSelectChannel<MAX_INTERNAL_CAMERA)
   {
       int i=profile.iSelectChannel;
       if(profile.camera[i].iEnable)
       {
#if DVR_API_ENABLE
           if(m_iDataBit != profile.camera[i].ptzinfo.iDataBit
              || m_iParityBit != profile.camera[i].ptzinfo.iParityBit
              || m_iStopBit != profile.camera[i].ptzinfo.iStopBit
              || m_iBaudRate != profile.camera[i].ptzinfo.iBaudRate)
            {
                LIB816x_setPtzSerialInfo(profile.camera[i].ptzinfo.iDataBit+7, profile.camera[i].ptzinfo.iParityBit, profile.camera[i].ptzinfo.iStopBit+1, ptzGetBuadRate(i));
                m_iDataBit = profile.camera[i].ptzinfo.iDataBit;
                m_iParityBit = profile.camera[i].ptzinfo.iParityBit;
                m_iStopBit = profile.camera[i].ptzinfo.iStopBit;
                m_iBaudRate = profile.camera[i].ptzinfo.iBaudRate;
            }
           LIB816x_ptzCtrl(profile.camera[i].ptzinfo.iPtzdriverIndex, profile.camera[i].ptzinfo.iPtzAddress,TILT_UP);
#endif
       }
   }
}
/*!ptz down button press인경우 연속 호출 */
void dvr_sidebar::ptzDownBtnSlot()
{
   if(profile.iSelectChannel<MAX_INTERNAL_CAMERA)
   {
       int i=profile.iSelectChannel;
       if(profile.camera[i].iEnable)
       {
#if DVR_API_ENABLE
           if(m_iDataBit != profile.camera[i].ptzinfo.iDataBit
              || m_iParityBit != profile.camera[i].ptzinfo.iParityBit
              || m_iStopBit != profile.camera[i].ptzinfo.iStopBit
              || m_iBaudRate != profile.camera[i].ptzinfo.iBaudRate)
            {
                LIB816x_setPtzSerialInfo(profile.camera[i].ptzinfo.iDataBit+7, profile.camera[i].ptzinfo.iParityBit, profile.camera[i].ptzinfo.iStopBit+1, ptzGetBuadRate(i));
                m_iDataBit = profile.camera[i].ptzinfo.iDataBit;
                m_iParityBit = profile.camera[i].ptzinfo.iParityBit;
                m_iStopBit = profile.camera[i].ptzinfo.iStopBit;
                m_iBaudRate = profile.camera[i].ptzinfo.iBaudRate;
            }
           LIB816x_ptzCtrl(profile.camera[i].ptzinfo.iPtzdriverIndex, profile.camera[i].ptzinfo.iPtzAddress,TILT_DOWN);
#endif
       }
   }
}
/*!ptz left button press인경우 연속 호출 */
void dvr_sidebar::ptzLeftBtnSlot()
{
   if(profile.iSelectChannel<MAX_INTERNAL_CAMERA)
   {
       int i=profile.iSelectChannel;
       if(profile.camera[i].iEnable)
       {
#if DVR_API_ENABLE
           if(m_iDataBit != profile.camera[i].ptzinfo.iDataBit
              || m_iParityBit != profile.camera[i].ptzinfo.iParityBit
              || m_iStopBit != profile.camera[i].ptzinfo.iStopBit
              || m_iBaudRate != profile.camera[i].ptzinfo.iBaudRate)
            {
                LIB816x_setPtzSerialInfo(profile.camera[i].ptzinfo.iDataBit+7, profile.camera[i].ptzinfo.iParityBit, profile.camera[i].ptzinfo.iStopBit+1, ptzGetBuadRate(i));
                m_iDataBit = profile.camera[i].ptzinfo.iDataBit;
                m_iParityBit = profile.camera[i].ptzinfo.iParityBit;
                m_iStopBit = profile.camera[i].ptzinfo.iStopBit;
                m_iBaudRate = profile.camera[i].ptzinfo.iBaudRate;
            }
           LIB816x_ptzCtrl(profile.camera[i].ptzinfo.iPtzdriverIndex, profile.camera[i].ptzinfo.iPtzAddress,PAN_LEFT);
#endif
       }
   }
}
/*!ptz right button press인경우 연속 호출 */
void dvr_sidebar::ptzRightBtnSlot()
{
   if(profile.iSelectChannel<MAX_INTERNAL_CAMERA)
   {
       int i=profile.iSelectChannel;
       if(profile.camera[i].iEnable)
       {
#if DVR_API_ENABLE
           if(m_iDataBit != profile.camera[i].ptzinfo.iDataBit
              || m_iParityBit != profile.camera[i].ptzinfo.iParityBit
              || m_iStopBit != profile.camera[i].ptzinfo.iStopBit
              || m_iBaudRate != profile.camera[i].ptzinfo.iBaudRate)
            {
                LIB816x_setPtzSerialInfo(profile.camera[i].ptzinfo.iDataBit+7, profile.camera[i].ptzinfo.iParityBit, profile.camera[i].ptzinfo.iStopBit+1, ptzGetBuadRate(i));
                m_iDataBit = profile.camera[i].ptzinfo.iDataBit;
                m_iParityBit = profile.camera[i].ptzinfo.iParityBit;
                m_iStopBit = profile.camera[i].ptzinfo.iStopBit;
                m_iBaudRate = profile.camera[i].ptzinfo.iBaudRate;
            }
           LIB816x_ptzCtrl(profile.camera[i].ptzinfo.iPtzdriverIndex, profile.camera[i].ptzinfo.iPtzAddress,PAN_RIGHT);
#endif
       }
   }
}
/*!ptz zoom in button press인경우 연속 호출 */
void dvr_sidebar::ptzZoomInBtnSlot()
{
   if(profile.iSelectChannel<MAX_INTERNAL_CAMERA)
   {
       int i=profile.iSelectChannel;
       if(profile.camera[i].iEnable)
       {
#if DVR_API_ENABLE
           if(m_iDataBit != profile.camera[i].ptzinfo.iDataBit
              || m_iParityBit != profile.camera[i].ptzinfo.iParityBit
              || m_iStopBit != profile.camera[i].ptzinfo.iStopBit
              || m_iBaudRate != profile.camera[i].ptzinfo.iBaudRate)
            {
                LIB816x_setPtzSerialInfo(profile.camera[i].ptzinfo.iDataBit+7, profile.camera[i].ptzinfo.iParityBit, profile.camera[i].ptzinfo.iStopBit+1, ptzGetBuadRate(i));
                m_iDataBit = profile.camera[i].ptzinfo.iDataBit;
                m_iParityBit = profile.camera[i].ptzinfo.iParityBit;
                m_iStopBit = profile.camera[i].ptzinfo.iStopBit;
                m_iBaudRate = profile.camera[i].ptzinfo.iBaudRate;
            }
           LIB816x_ptzCtrl(profile.camera[i].ptzinfo.iPtzdriverIndex, profile.camera[i].ptzinfo.iPtzAddress,ZOOM_IN);
#endif
       }
   }
}
/*!ptz zoom out button press인경우 연속 호출 */
void dvr_sidebar::ptzZoomOutBtnSlot()
{
   if(profile.iSelectChannel<MAX_INTERNAL_CAMERA)
   {
       int i=profile.iSelectChannel;
       if(profile.camera[i].iEnable)
       {
#if DVR_API_ENABLE
           if(m_iDataBit != profile.camera[i].ptzinfo.iDataBit
              || m_iParityBit != profile.camera[i].ptzinfo.iParityBit
              || m_iStopBit != profile.camera[i].ptzinfo.iStopBit
              || m_iBaudRate != profile.camera[i].ptzinfo.iBaudRate)
            {
                LIB816x_setPtzSerialInfo(profile.camera[i].ptzinfo.iDataBit+7, profile.camera[i].ptzinfo.iParityBit, profile.camera[i].ptzinfo.iStopBit+1, ptzGetBuadRate(i));
                m_iDataBit = profile.camera[i].ptzinfo.iDataBit;
                m_iParityBit = profile.camera[i].ptzinfo.iParityBit;
                m_iStopBit = profile.camera[i].ptzinfo.iStopBit;
                m_iBaudRate = profile.camera[i].ptzinfo.iBaudRate;
            }
           LIB816x_ptzCtrl(profile.camera[i].ptzinfo.iPtzdriverIndex, profile.camera[i].ptzinfo.iPtzAddress,ZOOM_OUT);
#endif
       }
   }
}
/*!ptz focus in button press인경우 연속 호출 */
void dvr_sidebar::ptzFocusInBtnSlot()
{
   if(profile.iSelectChannel<MAX_INTERNAL_CAMERA)
   {
       int i=profile.iSelectChannel;
       if(profile.camera[i].iEnable)
       {
#if DVR_API_ENABLE
           if(m_iDataBit != profile.camera[i].ptzinfo.iDataBit
              || m_iParityBit != profile.camera[i].ptzinfo.iParityBit
              || m_iStopBit != profile.camera[i].ptzinfo.iStopBit
              || m_iBaudRate != profile.camera[i].ptzinfo.iBaudRate)
            {
                LIB816x_setPtzSerialInfo(profile.camera[i].ptzinfo.iDataBit+7, profile.camera[i].ptzinfo.iParityBit, profile.camera[i].ptzinfo.iStopBit+1, ptzGetBuadRate(i));
                m_iDataBit = profile.camera[i].ptzinfo.iDataBit;
                m_iParityBit = profile.camera[i].ptzinfo.iParityBit;
                m_iStopBit = profile.camera[i].ptzinfo.iStopBit;
                m_iBaudRate = profile.camera[i].ptzinfo.iBaudRate;
            }
           LIB816x_ptzCtrl(profile.camera[i].ptzinfo.iPtzdriverIndex, profile.camera[i].ptzinfo.iPtzAddress,FOCUS_IN);
#endif
       }
   }
}
/*!ptz focus out button press인경우 연속 호출 */
void dvr_sidebar::ptzFocusOutBtnSlot()
{
   if(profile.iSelectChannel<MAX_INTERNAL_CAMERA)
   {
       int i=profile.iSelectChannel;
       if(profile.camera[i].iEnable)
       {
#if DVR_API_ENABLE
           if(m_iDataBit != profile.camera[i].ptzinfo.iDataBit
              || m_iParityBit != profile.camera[i].ptzinfo.iParityBit
              || m_iStopBit != profile.camera[i].ptzinfo.iStopBit
              || m_iBaudRate != profile.camera[i].ptzinfo.iBaudRate)
            {
                LIB816x_setPtzSerialInfo(profile.camera[i].ptzinfo.iDataBit+7, profile.camera[i].ptzinfo.iParityBit, profile.camera[i].ptzinfo.iStopBit+1, ptzGetBuadRate(i));
                m_iDataBit = profile.camera[i].ptzinfo.iDataBit;
                m_iParityBit = profile.camera[i].ptzinfo.iParityBit;
                m_iStopBit = profile.camera[i].ptzinfo.iStopBit;
                m_iBaudRate = profile.camera[i].ptzinfo.iBaudRate;
            }
           LIB816x_ptzCtrl(profile.camera[i].ptzinfo.iPtzdriverIndex, profile.camera[i].ptzinfo.iPtzAddress,FOCUS_OUT);
#endif
       }
   }
}
/*!ptz button release인경우 호출 */
void dvr_sidebar::ptzStopSlot()
{
   if(profile.iSelectChannel<MAX_INTERNAL_CAMERA)
   {
       int i=profile.iSelectChannel;
       if(profile.camera[i].iEnable)
       {
#if DVR_API_ENABLE
            LIB816x_ptzCtrl(profile.camera[i].ptzinfo.iPtzdriverIndex, profile.camera[i].ptzinfo.iPtzAddress,PTZ_STOP);
#endif
       }
   }
}

//Playback Calendar Control 
/*!record data에 따라서 녹화된날은 red, 녹화되지 않는 날은 gray로 표시 */
void dvr_sidebar::setCalendarRecordDate(struct tm * ptm,int iMonthTable[PLAYCALENDARMAXDAYCOUNT])
{
    QDate qmonthday;
    QTextCharFormat qtextformat;

    qmonthday.setDate(ptm->tm_year,ptm->tm_mon+1,ptm->tm_mday);
    int monthindays=qmonthday.daysInMonth();
    QBrush brush;
    for(int i=0;i<monthindays;i++)
    {
        qmonthday.setDate(ptm->tm_year,ptm->tm_mon+1,i+1);
        if(iMonthTable[i]!=0)
        {
            brush.setColor(Qt::red);
        }
        else
        {
            brush.setColor(Qt::darkGray);
        }
        qtextformat.setForeground(brush);
        ui->dvrmainsidebarplaybackcalendar->setDateTextFormat (qmonthday, qtextformat );
    }
}
/*!calendar에서 현재 선택된 date를 얻어옴 */
void dvr_sidebar::getCalendarSelectDay(struct tm * ptm)
{
    QDate qselectday=ui->dvrmainsidebarplaybackcalendar->selectedDate();
    qselectday.getDate(&ptm->tm_year,&ptm->tm_mon,&ptm->tm_mday);
    ptm->tm_mon=ptm->tm_mon-1;
}
/*!입력 date로 calendar의 현재 date를 변경 */
void dvr_sidebar::setCalendarSelectDay(struct tm * ptm,bool bplaystop)
{
    QDate qselectday;
    if(bplaystop)
    {
        m_iplayonchange=0;
    }
    else
    {
        m_iplayonchange=1;
    }
    qselectday.setDate(ptm->tm_year,ptm->tm_mon+1,ptm->tm_mday);
    ui->dvrmainsidebarplaybackcalendar->setSelectedDate(qselectday);
}
/*!calendar의 선택된 date가 변경될때 호출 */
void dvr_sidebar::playbackCalendarSelectionChangedSlot()
{
    if(bCalendarPageChange)
    {
        bCalendarPageChange=false;
        return;
    }
    if(m_iplayonchange<=0)
    {
        if(runtimesettings.playbackinfo.playbackmode.iPlaybackMode!=PLAYBACKMODE_STOP)
        {
            setPlayBackMode(PLAYBACKMODE_STOP);
        }

        getCalendarSelectDay(&curtime.playbacktime);
    }
    else
    {
        struct tm tmptime;
        getCalendarSelectDay(&tmptime);
        curtime.playbacktime.tm_year = tmptime.tm_year;
        curtime.playbacktime.tm_mon = tmptime.tm_mon;
        curtime.playbacktime.tm_mday = tmptime.tm_mday;
    }
    //read record info, update calendar
    memset(runtimesettings.playbackinfo.playbacktableinfo.iMonthTable,0,sizeof(runtimesettings.playbackinfo.playbacktableinfo.iMonthTable));
    LIB816x_GetRecDays(curtime.playbacktime,runtimesettings.playbackinfo.playbacktableinfo.iMonthTable);
    setCalendarRecordDate(&curtime.playbacktime,runtimesettings.playbackinfo.playbacktableinfo.iMonthTable);
    //update timebar
    pTimebar->SetTimeTableUpdate(true,true);
    dprintf("PlaybackCalendarSelectionChangedSlot() called %d\n",m_iplayonchange);
    m_iplayonchange=0;
}
/*!calendar page가 변경될때 호출(년도/달이 바뀌는 경우) */
void dvr_sidebar::playbackCalendarcurrentPageChangedSlot(int year, int month)
{
    bCalendarPageChange=true;
    if(m_iplayonchange==0 && (curtime.playbacktime.tm_year!=year || curtime.playbacktime.tm_mon!=month-1))
    {
        struct tm tmptime;
        getCalendarSelectDay(&tmptime);
        curtime.playbacktime.tm_year=year;
        curtime.playbacktime.tm_mon=month-1;
        curtime.playbacktime.tm_mday = tmptime.tm_mday;
        setCalendarSelectDay(&curtime.playbacktime,true);
    }
    else
    {
        struct tm tmptime;
        getCalendarSelectDay(&tmptime);
        curtime.playbacktime.tm_year = tmptime.tm_year;
        curtime.playbacktime.tm_mon = tmptime.tm_mon;
        curtime.playbacktime.tm_mday = tmptime.tm_mday;
        setCalendarSelectDay(&curtime.playbacktime,false);
    }
    //read record info, update calendar
    memset(runtimesettings.playbackinfo.playbacktableinfo.iMonthTable,0,sizeof(runtimesettings.playbackinfo.playbacktableinfo.iMonthTable));
    LIB816x_GetRecDays(curtime.playbacktime,runtimesettings.playbackinfo.playbacktableinfo.iMonthTable);
    setCalendarRecordDate(&curtime.playbacktime,runtimesettings.playbackinfo.playbacktableinfo.iMonthTable);
    //update timebar
    pTimebar->SetTimeTableUpdate(true,true);
    dprintf("PlaybackCalendarcurrentPageChangedSlot() called %d\n",m_iplayonchange);
}
/*!search button click시 호출 */
void dvr_sidebar::searchBtnSlot()
{
     printf("SearchBtnSlot()\n");
}

//Playback Control 
/*!playback speed string 출력 */
QString dvr_sidebar::setPlaySpeedValueText(int ivalue)
{
	//update playback speed string
    QString qstrvalue;
    char speedbuf[MAX_CHAR_16];
    sprintf(speedbuf,"%s = %s",STRSPEED,strplaybackspeed[ivalue]);
    qstrvalue=StrToQString(speedbuf);

    return qstrvalue;
}
/*!playback mode에 따라서 button enable/disable 처리 */
void dvr_sidebar::setPlaybackBtnSelect(int iMode)
{
	//show/hide and enable/disable playback button by playback mode
    m_iPlaybackMode=iMode;

    switch(m_iPlaybackMode)
    {
    case PLAYBACKMODE_STOP :
        ui->play_reverse_btn->setEnabled(true);
        ui->play_reversestep_btn->setEnabled(true);
        ui->play_play_btn->setEnabled(true);
        ui->play_pause_btn->setEnabled(true);
        ui->play_pause_btn->setEnabled(false);
        ui->play_stop_btn->setEnabled(false);
        ui->play_forwardstep_btn->setEnabled(true);
        ui->play_forward_btn->setEnabled(true);
        ui->play_pause_btn->hide();
        ui->playspeeddial->setEnabled(false);
        m_iPlaybackSpeed=PLAYBACKSPEED_1X;
        break;
    case PLAYBACKMODE_PLAY :
        ui->play_reverse_btn->setEnabled(true);
        ui->play_reversestep_btn->setEnabled(true);
        ui->play_play_btn->setEnabled(false);
        ui->play_pause_btn->setEnabled(true);
        ui->play_stop_btn->setEnabled(true);
        ui->play_forwardstep_btn->setEnabled(true);
        ui->play_forward_btn->setEnabled(true);
        ui->play_pause_btn->show();
        ui->playspeeddial->setEnabled(true);	// YOUNG(July 18, 2011)
        m_iPlaybackSpeed=PLAYBACKSPEED_1X;
        break;
    case PLAYBACKMODE_PUASE :
        ui->play_reverse_btn->setEnabled(true);
        ui->play_reversestep_btn->setEnabled(true);
        ui->play_play_btn->setEnabled(true);
        ui->play_pause_btn->setEnabled(false);
        ui->play_stop_btn->setEnabled(true);
        ui->play_forwardstep_btn->setEnabled(true);
        ui->play_forward_btn->setEnabled(true);
        ui->play_pause_btn->hide();
        ui->playspeeddial->setEnabled(false);
        m_iPlaybackSpeed=PLAYBACKSPEED_1X;
        break;
    case PLAYBACKMODE_REVERSE :
        ui->play_reverse_btn->setEnabled(false);
        ui->play_reversestep_btn->setEnabled(true);
        ui->play_play_btn->setEnabled(true);
        ui->play_pause_btn->setEnabled(true);
        ui->play_stop_btn->setEnabled(true);
        ui->play_forwardstep_btn->setEnabled(true);
        ui->play_forward_btn->setEnabled(true);
        ui->play_pause_btn->hide();
        ui->playspeeddial->setEnabled(true);
        break;
    case PLAYBACKMODE_REVERSESTEP :
        ui->play_reverse_btn->setEnabled(true);
        ui->play_reversestep_btn->setEnabled(false);
        ui->play_play_btn->setEnabled(true);
        ui->play_pause_btn->setEnabled(true);
        ui->play_stop_btn->setEnabled(true);
        ui->play_forwardstep_btn->setEnabled(true);
        ui->play_forward_btn->setEnabled(true);
        ui->play_pause_btn->hide();
        ui->playspeeddial->setEnabled(false);
        m_iPlaybackSpeed=PLAYBACKSPEED_1X;
        break;
    case PLAYBACKMODE_FORWARD :
        ui->play_reverse_btn->setEnabled(true);
        ui->play_reversestep_btn->setEnabled(true);
        ui->play_play_btn->setEnabled(true);
        ui->play_pause_btn->setEnabled(true);
        ui->play_stop_btn->setEnabled(true);
        ui->play_forwardstep_btn->setEnabled(true);
        ui->play_forward_btn->setEnabled(false);
        ui->play_pause_btn->hide();
        ui->playspeeddial->setEnabled(true);
        m_iPlaybackSpeed=PLAYBACKSPEED_4X;
        break;
    case PLAYBACKMODE_FORAWRDSTEP :
        ui->play_reverse_btn->setEnabled(true);
        ui->play_reversestep_btn->setEnabled(true);
        ui->play_play_btn->setEnabled(true);
        ui->play_pause_btn->setEnabled(true);
        ui->play_stop_btn->setEnabled(true);
        ui->play_forwardstep_btn->setEnabled(false);
        ui->play_forward_btn->setEnabled(true);
        ui->play_pause_btn->hide();
        ui->playspeeddial->setEnabled(false);
        m_iPlaybackSpeed=PLAYBACKSPEED_1X;
        break;
    }

    if(m_iPlaybackSpeed != ui->playspeeddial->value())
    {
        ui->playspeeddial->setValue(m_iPlaybackSpeed);
    }
}
extern dvr_toolbar_main *pToolbar;
static int ilayout[9]={1,4,9,8,8,12,12,14,16};
/*!playback mode설정/해당API 호출 */
void dvr_sidebar::setPlayBackMode(int iMode)
{
    int ret=LIB_PB_NO_ERR;
    int i=0;
	
    if(runtimesettings.playbackinfo.playbackmode.iPlaybackMode==iMode)
    {
        return;
    }
	//call each playback API by playback mode
    switch(iMode)
    {
    case PLAYBACKMODE_STOP :
#if	DVR_API_ENABLE	
		pToolbar->toolbarcursorPosition=4+profile.layout.iLayoutMode;
		pToolbar->MaxPageCalc();
        	runtimesettings.iLayout=ilayout[profile.layout.iLayoutMode-1];
        	pToolbar->SetLayoutModeBtnSelect(profile.layout.iLayoutMode);
    		pLayoutClass->SetLayoutMode(profile.layout.iLayoutMode);	
		ret=LIB816x_stopPlayback_x();
#endif
        if(LIB_PB_NO_ERR == ret)
        {
            pMainClass->EventLogAdd((char*)"Playback Stop",true);
            memcpy(&playbackupdatetime,&curtime.playbacktime,sizeof(struct tm));
        }
        else
        {
            pMainClass->EventLogAdd((char*)"Playback Stop Fail",true);
            pMainClass->ShowNotifyDialog((char*)"Playback Stop Fail");
        }
        break;
    case PLAYBACKMODE_PLAY :
#if	DVR_API_ENABLE
        if(runtimesettings.playbackinfo.playbackmode.iPlaybackMode == PLAYBACKMODE_PUASE)
            ret = LIB816x_restartPlayback_x(0xffff);
        else
        {
        	pToolbar->toolbarcursorPosition=4+profile.layout.iSecondLayoutMode;			
        	runtimesettings.iLayout=ilayout[profile.layout.iSecondLayoutMode-1];
		pToolbar->MaxPageCalc();
        	pToolbar->SetLayoutModeBtnSelect(profile.layout.iSecondLayoutMode);
    		pLayoutClass->SetLayoutMode(profile.layout.iSecondLayoutMode);
            pTimebar->GetNextRecordDataTime(&curtime.playbacktime);
            ret=LIB816x_startPlayback_x(0xffff, &curtime.playbacktime);			       
        }
#else
        ret=LIB_PB_NO_ERR;
#endif
        if(LIB_PB_NO_ERR == ret)
        {
            pMainClass->bDoUpdateCalendar=false;
            pMainClass->EventLogAdd((char*)"Playback Start",true);
        }
        else
        {
            pMainClass->EventLogAdd((char*)"Playback Start Fail",true);
            pMainClass->ShowNotifyDialog((char*)"Playback Start Fail");
        }
        break;
    case PLAYBACKMODE_PUASE :
#if	DVR_API_ENABLE
        ret=LIB816x_pausePlayback_x();
#else
        ret=LIB_PB_NO_ERR;
#endif
        break;
    case PLAYBACKMODE_REVERSE :
#if	DVR_API_ENABLE
        ret=LIB816x_fastBackward_x(0xffff);
#else
        ret=LIB_PB_NO_ERR;
#endif
        break;
    case PLAYBACKMODE_REVERSESTEP :
#if	DVR_API_ENABLE
        ret=LIB816x_stepPlayback_x(0xffff,1);
#else
        ret=LIB_PB_NO_ERR;
#endif
        iMode = runtimesettings.playbackinfo.playbackmode.iPlaybackMode;
        break;
    case PLAYBACKMODE_FORWARD :
#if	DVR_API_ENABLE
        ret=LIB816x_fastforward_x(0xffff);
#else
        ret=LIB_PB_NO_ERR;
#endif

        break;
    case PLAYBACKMODE_FORAWRDSTEP :
#if	DVR_API_ENABLE
        ret=LIB816x_stepPlayback_x(0xffff,0);
#else
        ret=LIB_PB_NO_ERR;
#endif
        iMode = runtimesettings.playbackinfo.playbackmode.iPlaybackMode;
        break;
    }

    if(LIB_PB_NO_ERR != ret)
    {
        iMode = PLAYBACKMODE_STOP;
        setPlaybackBtnSelect(iMode);
#if	DVR_API_ENABLE
        ret=LIB816x_stopPlayback_x();
#endif
    }
    runtimesettings.playbackinfo.playbackmode.iPlaybackMode=iMode;
    //update playback mode and icon
    for(i=0;i<MAX_INTERNAL_CAMERA;i++)
    {
        detectstatus.playbackmodeinfo[i].iPlaybackMode=iMode;
    }
    pLayoutClass->DrawOSDIconPlayback();
    setPlaybackBtnSelect(iMode);
}
/*!playback speed에 따라서 해당 API 호출 */
void dvr_sidebar::setPlayBackSpeedChange()
{
	//call playback speed change API
    int ret=0, speed;
    if (PLAYBACKMODE_PLAY == runtimesettings.playbackinfo.playbackmode.iPlaybackMode || PLAYBACKMODE_REVERSE == runtimesettings.playbackinfo.playbackmode.iPlaybackMode)
    {
        if(PLAYBACKSPEED_1X == m_iPlaybackSpeed)
            speed = LIB_PB_SPEED_1X;
        else if(PLAYBACKSPEED_2X == m_iPlaybackSpeed)
            speed = LIB_PB_SPEED_2X;
        else if(PLAYBACKSPEED_4X == m_iPlaybackSpeed)
            speed = LIB_PB_SPEED_4X;
        else if(PLAYBACKSPEED_8X == m_iPlaybackSpeed)
            speed = LIB_PB_SPEED_8X;
        else if(PLAYBACKSPEED_16X == m_iPlaybackSpeed)
            speed = LIB_PB_SPEED_16X;
        else if(PLAYBACKSPEED_32X == m_iPlaybackSpeed)
            speed = LIB_PB_SPEED_32X;
        else if (PLAYBACKSPEED_1DIVIDE4X == m_iPlaybackSpeed)
                speed = LIB_PB_SPEED_0_25X;
        else if (PLAYBACKSPEED_1DIVIDE2X == m_iPlaybackSpeed)
                speed = LIB_PB_SPEED_0_5X;
        else
            speed = LIB_PB_SPEED_1X;
#if DVR_API_ENABLE
        ret = LIB816x_setPlaybackProperty_x(LIB_PB_SUB_CMD_CH_SET_SPEED, 0xffff, speed, NULL);
        if (LIB_PB_NO_ERR != ret)
        {
            eprintf("set playback speed ... failed ret %d\n",ret);
        }
        else
        {
            runtimesettings.playbackinfo.playbackmode.iSpeed=m_iPlaybackSpeed;
        }
#endif
    }
}
/*!playback reverse button click시 호출 */
void dvr_sidebar::playbackReverseBtnSlot()
{
    setPlaybackBtnSelect(PLAYBACKMODE_REVERSE);
    setPlayBackMode(PLAYBACKMODE_REVERSE);
    setPlayBackSpeedChange();
    ui->playspeedlabel->setText(setPlaySpeedValueText(4));
     dprintf("PlaybackReverseBtnSlot()\n");
}
/*!playback reverse step button click시 호출 */
void dvr_sidebar::playbackReverseStepBtnSlot()
{
    setPlaybackBtnSelect(PLAYBACKMODE_REVERSESTEP);
    if(runtimesettings.playbackinfo.playbackmode.iPlaybackMode == PLAYBACKMODE_PLAY)
        setPlayBackMode(PLAYBACKMODE_PUASE);
    setPlayBackMode(PLAYBACKMODE_REVERSESTEP);
     dprintf("PlaybackReverseStepBtnSlot()\n");
}
/*!playback play button click시 호출 */
void dvr_sidebar::playbackPlayBtnSlot()
{
    setPlaybackBtnSelect(PLAYBACKMODE_PLAY);
    setPlayBackMode(PLAYBACKMODE_PLAY);
    setPlayBackSpeedChange();
	ui->playspeedlabel->setText(setPlaySpeedValueText(2));
    dprintf("PlaybackPlayBtnSlot()\n");
}
/*!playback pause button click시 호출 */
void dvr_sidebar::playbackPauseBtnSlot()
{
    setPlaybackBtnSelect(PLAYBACKMODE_PUASE);
    setPlayBackMode(PLAYBACKMODE_PUASE);
     dprintf("PlaybackPauseBtnSlot()\n");
}
/*!playback stop button click시 호출 */
void dvr_sidebar::playbackStopBtnSlot()
{
    setPlaybackBtnSelect(PLAYBACKMODE_STOP);
    setPlayBackMode(PLAYBACKMODE_STOP);
	ui->playspeedlabel->setText(setPlaySpeedValueText(2));
     dprintf("PlaybackStopBtnSlot()\n");
}
/*!playback forward step button click시 호출 */
void dvr_sidebar::playbackForwardStepBtnSlot()
{
    setPlaybackBtnSelect(PLAYBACKMODE_FORAWRDSTEP);
    if(runtimesettings.playbackinfo.playbackmode.iPlaybackMode == PLAYBACKMODE_PLAY)
        setPlayBackMode(PLAYBACKMODE_PUASE);
    setPlayBackMode(PLAYBACKMODE_FORAWRDSTEP);
     dprintf("PlaybackForwardStepBtnSlot()\n");
}
/*!playback forward button click시 호출 */
void dvr_sidebar::playbackForwardBtnSlot()
{
    setPlaybackBtnSelect(PLAYBACKMODE_FORWARD);
    setPlayBackMode(PLAYBACKMODE_FORWARD);
     dprintf("PlaybackForwardBtnSlot()\n");
}
/*!playback speed dail 변경시 호출 */
void dvr_sidebar::playbackSpeedChangeSlot(int value)
{
	//change playback speed by dial control
    dprintf("playbackspeed change %d\n",value);
    if(m_iPlaybackMode==PLAYBACKMODE_PLAY)
    {
        m_iPlaybackSpeed=value;
        if(m_iPlaybackSpeed<PLAYBACKSPEED_1DIVIDE4X)
        {
            m_iPlaybackSpeed = PLAYBACKSPEED_1DIVIDE4X;
            ui->playspeeddial->setValue(m_iPlaybackSpeed);
        }
        else if(m_iPlaybackSpeed>PLAYBACKSPEED_32X)
        {
            m_iPlaybackSpeed = PLAYBACKSPEED_32X;
            ui->playspeeddial->setValue(m_iPlaybackSpeed);
        }
    }
    else if(m_iPlaybackMode==PLAYBACKMODE_REVERSE)
    {
        m_iPlaybackSpeed=value;
        if(m_iPlaybackSpeed<PLAYBACKSPEED_1DIVIDE4X)
        {
            m_iPlaybackSpeed = PLAYBACKSPEED_1DIVIDE4X;
            ui->playspeeddial->setValue(m_iPlaybackSpeed);
        }
        else if(m_iPlaybackSpeed>PLAYBACKSPEED_32X)
        {
            m_iPlaybackSpeed = PLAYBACKSPEED_32X;
            ui->playspeeddial->setValue(m_iPlaybackSpeed);
        }
    }
	else if(m_iPlaybackMode==PLAYBACKMODE_FORWARD)
	{
		m_iPlaybackSpeed=value;
		if(m_iPlaybackSpeed<PLAYBACKSPEED_4X)
        {
            m_iPlaybackSpeed = PLAYBACKSPEED_4X;
            ui->playspeeddial->setValue(m_iPlaybackSpeed);
        }
        else if(m_iPlaybackSpeed>PLAYBACKSPEED_32X)
        {
            m_iPlaybackSpeed = PLAYBACKSPEED_32X;
            ui->playspeeddial->setValue(m_iPlaybackSpeed);
        }
	}
    else
    {
        m_iPlaybackSpeed=PLAYBACKSPEED_1X;
		ui->playspeedlabel->setText(setPlaySpeedValueText(2));
    }
    ui->playspeedlabel->setText(setPlaySpeedValueText(m_iPlaybackSpeed));
    setPlayBackSpeedChange();
}
