/******************************************************************************
* DM8168 Hybrid DVR
* Copyright by UDWorks, Incoporated. All Rights Reserved.
*-----------------------------------------------------------------------------
* @file	dvr_toolbar_main.cpp
* @brief
* @author
* @section	MODIFY history
*     - 2011.01.01 : First Created
******************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include "dvr_defines.h"
#include "dvr_toolbar_main.h"
#include "ui_dvr_toolbar_main.h"
#include "dvr_main.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
extern dvrmain *pMainClass;
extern dvr_layout_main *pLayoutClass;
extern dvr_sidebar *pSidebar;
extern dvr_timelinebar_play *pTimebar;
extern dvr_statusbar *pStatusbar;
extern dvr_toolbar_sub *pSubToolbar;
extern EventLogDialog *pEvtDlg;
extern dvr_setup *pSetup;
extern dvr_login *pLoginDlg;
/*----------------------------------------------------------------------------
 local functions
-----------------------------------------------------------------------------*/
/*!constructor*/
dvr_toolbar_main::dvr_toolbar_main(QWidget *parent) :
   QDialog(parent,Qt::CustomizeWindowHint |Qt::FramelessWindowHint),
    ui(new Ui::dvr_toolbar_main)
{
    ui->setupUi(this);
    PageCount=0;
    SetMainWindowSize();
    connect(ui->livebackmode, SIGNAL(clicked()), this, SLOT(LivebackModeBtnSlot()));
    connect(ui->playbackmode, SIGNAL(clicked()), this, SLOT(PlaybackModeBtnSlot()));
    connect(ui->livebacksubmode, SIGNAL(clicked()), this, SLOT(LivebackSubModeBtnSlot()));


    connect(ui->prevpage, SIGNAL(clicked()), this, SLOT(PrevPageBtnSlot()));
    connect(ui->nextpage, SIGNAL(clicked()), this, SLOT(NextPageBtnSlot()));

    connect(ui->layoutmode1, SIGNAL(clicked()), this, SLOT(Layout1BtnSlot()));
    connect(ui->layoutmode4, SIGNAL(clicked()), this, SLOT(Layout4BtnSlot()));
    connect(ui->layoutmode9, SIGNAL(clicked()), this, SLOT(Layout9BtnSlot()));
    connect(ui->layoutmode16, SIGNAL(clicked()), this, SLOT(Layout16BtnSlot()));

    connect(ui->layoutmode8, SIGNAL(clicked()), this, SLOT(Layout8BtnSlot()));
    connect(ui->layoutmode8_1, SIGNAL(clicked()), this, SLOT(Layout8_1BtnSlot()));
    connect(ui->layoutmode12, SIGNAL(clicked()), this, SLOT(Layout12BtnSlot()));
    connect(ui->layoutmode12_1, SIGNAL(clicked()), this, SLOT(Layout12_1BtnSlot()));
    connect(ui->layoutmode14, SIGNAL(clicked()), this, SLOT(Layout14BtnSlot()));

    connect(ui->sequencemode, SIGNAL(clicked()), this, SLOT(SequenceModeBtnSlot()));
    connect(ui->audioonoff, SIGNAL(clicked()), this, SLOT(AudioOnOffBtnSlot()));
    connect(ui->eventlog, SIGNAL(clicked()), this, SLOT(EventLogBtnSlot()));
    connect(ui->snapshot, SIGNAL(clicked()), this, SLOT(SnapshotBtnSlot()));
    connect(ui->backup, SIGNAL(clicked()), this, SLOT(BackupBtnSlot()));

    connect(ui->setupproperty, SIGNAL(clicked()), this, SLOT(SetupPropertyCallSlot()));
    connect(ui->emergencyrecord, SIGNAL(clicked()), this, SLOT(EmergencyRecordSlot()));
    connect(ui->logout, SIGNAL(clicked()), this, SLOT(LogoutSlot()));

    toolbarcursorPosition=0;
    ShowCurrentSelection();
    bBlockenableChange=true;
    ui->layoutcamflow->hide();
    ui->layoutmode24->hide();
    ui->snapshot->hide();
}
/*!destructor */
dvr_toolbar_main::~dvr_toolbar_main()
{
    delete ui;
}

void dvr_toolbar_main::changeEvent(QEvent *e)
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
/*!front key handling */
void dvr_toolbar_main::keyPressEvent ( QKeyEvent * event )
{
    dprintf("keyPressEvent key=0x%X\n",event->key());
    switch(event->key())
    {
        case Qt::Key_H:     //LIVE
            if(ui->livebackmode->isEnabled() && runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
            {
            	//change to live mode
                runtimesettings.iLivePlaybackMode=LIB_LIVE_MODE;
                SetLivePlayBackBtnSelect(LIB_LIVE_MODE);
                pSidebar->setMode();
                pTimebar->hide();
            #if DVR_API_ENABLE
                if(!profile.iSubOutputEnable)
                {
                    if(runtimesettings.iLayout*runtimesettings.iPageNum<16)
                        LIB816x_setCameraLayout(profile.layout.iMainOutput, runtimesettings.iLayout*runtimesettings.iPageNum, runtimesettings.iLayout);
                }
            #endif
                pLayoutClass->DrawOSDChannelLive();
                pStatusbar->SetMainWindowSize();
            }
        return;
        case Qt::Key_I:     //SEARCH
            if(ui->playbackmode->isEnabled() && runtimesettings.iLivePlaybackMode==LIB_LIVE_MODE)
            {
            	//change to playback mode
                runtimesettings.iLivePlaybackMode=LIB_PLAYBACK_MODE;
                SetLivePlayBackBtnSelect(LIB_PLAYBACK_MODE);
                if(runtimesettings.playbackinfo.playbackmode.iPlaybackMode == PLAYBACKMODE_STOP)
                {
                    memcpy(&curtime.playbacktime,&curtime.currenttime,sizeof(struct tm));
                    curtime.playbacktime.tm_sec=0;
                }
                memset(runtimesettings.playbackinfo.playbacktableinfo.iMonthTable,0,sizeof(runtimesettings.playbackinfo.playbacktableinfo.iMonthTable));
                LIB816x_GetRecDays(curtime.playbacktime,runtimesettings.playbackinfo.playbacktableinfo.iMonthTable);
                pSidebar->setCalendarRecordDate(&curtime.playbacktime,runtimesettings.playbackinfo.playbacktableinfo.iMonthTable);
                pSidebar->setMode();
                pSidebar->setCalendarSelectDay(&curtime.playbacktime,true);

                pTimebar->SetTimeTableChannel(0);
                pTimebar->SetTimeTableSelectTimeMarking(&curtime.playbacktime);
                pTimebar->SetTimeTableUpdate(true,true);
                pTimebar->show();

            #if DVR_API_ENABLE
                if(!profile.iSubOutputEnable)
                {
                    if(runtimesettings.iLayout*runtimesettings.iPageNum<16)
                        LIB816x_setCameraLayout(profile.layout.iMainOutput, 16+runtimesettings.iLayout*runtimesettings.iPageNum, runtimesettings.iLayout);
                }
            #endif
                pLayoutClass->DrawOSDChannelPlayback();
                pStatusbar->ChangePlaybackMainWindowSize(true);
            }
        return;
        case Qt::Key_J:     //SCRMODE
        	//change next layout mode
            if(ui->layoutmode1->isEnabled() || ui->layoutmode4->isEnabled())
            {
                switch(profile.layout.iLayoutMode)
                {
                case LAYOUTMODE_1:
                    profile.layout.iLayoutMode=LAYOUTMODE_4;
                    runtimesettings.iLayout=LAYOUT_4;
                    runtimesettings.iPageNum=0;
                    break;
                case LAYOUTMODE_4:
                    profile.layout.iLayoutMode=LAYOUTMODE_9;
                    runtimesettings.iLayout=LAYOUT_9;
                    runtimesettings.iPageNum=0;
                    break;
                case LAYOUTMODE_9:
                    profile.layout.iLayoutMode=LAYOUTMODE_16;
                    runtimesettings.iLayout=LAYOUT_16;
                    runtimesettings.iPageNum=0;
                    break;
                case LAYOUTMODE_8:
                    profile.layout.iLayoutMode=LAYOUTMODE_8_1;
                    runtimesettings.iLayout=LAYOUT_8;
                    runtimesettings.iPageNum=0;
                    break;
                case LAYOUTMODE_8_1:
                    profile.layout.iLayoutMode=LAYOUTMODE_12;
                    runtimesettings.iLayout=LAYOUT_12;
                    runtimesettings.iPageNum=0;
                    break;
                case LAYOUTMODE_12:
                    profile.layout.iLayoutMode=LAYOUTMODE_12_1;
                    runtimesettings.iLayout=LAYOUT_12;
                    runtimesettings.iPageNum=0;
                    break;
                case LAYOUTMODE_12_1:
                    profile.layout.iLayoutMode=LAYOUTMODE_14;
                    runtimesettings.iLayout=LAYOUT_14;
                    runtimesettings.iPageNum=0;
                    break;
                case LAYOUTMODE_14:
                    profile.layout.iLayoutMode=LAYOUTMODE_1;
                    runtimesettings.iLayout=LAYOUT_1;
                    runtimesettings.iPageNum=0;
                    break;
                case LAYOUTMODE_16:
                    profile.layout.iLayoutMode=LAYOUTMODE_8;
                    runtimesettings.iLayout=LAYOUT_8;
                    runtimesettings.iPageNum=0;
                    break;
                }
                SetLayoutModeBtnSelect(profile.layout.iLayoutMode);
                pLayoutClass->SetLayoutMode(profile.layout.iLayoutMode);
                pLayoutClass->DrawOSDChannel();
#if DVR_API_ENABLE
                LIB816x_setDisplayLayout(profile.layout.iLayoutMode, 0);
                if(!profile.iSubOutputEnable && runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
                {
                    LIB816x_setCameraLayout(profile.layout.iMainOutput, 16, runtimesettings.iLayout);
                }
#endif
                WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));
            }
        return;
        case Qt::Key_K:     //FULL SCR
        	//change full screen or normal screen (not implemented)
        return;
        case Qt::Key_L:     //PREV
            if(ui->prevpage->isEnabled())
            {
                    //change previous page
                int count=0;
                int enablecount=0;
                for(int i=0;i<MAX_CAMERA;i++)
                {
                    if(profile.camera[i].iEnable)
                        enablecount++;
                }
                if((enablecount%runtimesettings.iLayout)==0)
                count=enablecount/runtimesettings.iLayout-1;
                else
                count=enablecount/runtimesettings.iLayout;

                if(runtimesettings.iPageNum>0)
                  runtimesettings.iPageNum--;
                else
                  runtimesettings.iPageNum=count;

                pLayoutClass->DrawOSDChannel();
#if DVR_API_ENABLE
                LIB816x_setDisplayLayout(profile.layout.iLayoutMode, 0);
                if(!profile.iSubOutputEnable && runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
                {
                    LIB816x_setCameraLayout(profile.layout.iMainOutput, 16, runtimesettings.iLayout);
                }
#endif
            }
        return;
        case Qt::Key_M:     //NEXT
            if(ui->nextpage->isEnabled())
            {
                    //change next page
                int count=0;
                int enablecount=0;
                for(int i=0;i<MAX_CAMERA;i++)
                {
                    if(profile.camera[i].iEnable)
                        enablecount++;
                }
                if((enablecount%runtimesettings.iLayout)==0)
                count=enablecount/runtimesettings.iLayout-1;
                else
                count=enablecount/runtimesettings.iLayout;

                if(runtimesettings.iPageNum<count)
                  runtimesettings.iPageNum++;
                else
                  runtimesettings.iPageNum=0;

               pLayoutClass->DrawOSDChannel();
#if DVR_API_ENABLE
                LIB816x_setDisplayLayout(profile.layout.iLayoutMode, 0);
                if(!profile.iSubOutputEnable && runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
                {
                    LIB816x_setCameraLayout(profile.layout.iMainOutput, 16, runtimesettings.iLayout);
                }
#endif
            }
        return;
        case Qt::Key_N:     //SEQ
        	//toggle sequence mode on/off
            if(ui->sequencemode->isEnabled())
            {
                if(profile.sequence.iSequenceModeOn==0)
                {
                    profile.sequence.iSequenceModeOn=1;
                    pMainClass->MainlayoutTimerStart();
                }
                else
                {
                    profile.sequence.iSequenceModeOn=0;
                    pMainClass->MainlayoutTimerStop();
                }
                WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));
            }
        return;
        case Qt::Key_W:     //SETUP
            if(ui->setupproperty->isEnabled())
            {
                toolbarcursorPosition=18;
                ShowCurrentSelection();
                ExeCurrentSelection();
            }
            return;
        case Qt::Key_Left:
            toolbarcursorPosition--;
            if(toolbarcursorPosition<0)
                toolbarcursorPosition=0;
            ShowCurrentSelection();
        return;
        case Qt::Key_Right:
            toolbarcursorPosition++;
            if(toolbarcursorPosition>=22)
                toolbarcursorPosition=21;
            ShowCurrentSelection();
        return;
        case Qt::Key_Return:
            ExeCurrentSelection();
        return;
    }

    pMainClass->keyPressEvent(event);
}
/*!front key 입력시 현재 선택된 항목을 highlight 시킴*/
void dvr_toolbar_main::ShowCurrentSelection()
{
    QRect rect;
    //display current selected item

    switch(toolbarcursorPosition)
    {
    case 0:     //LIVE
        rect=ui->livebackmode->geometry();
        break;
    case 1:     //PLAYBACK
        rect=ui->playbackmode->geometry();
        break;
    case 2:     //SUB
        rect=ui->livebacksubmode->geometry();
        break;
    case 3:     //PREV
        rect=ui->prevpage->geometry();
        break;
    case 4:     //NEXT
        rect=ui->nextpage->geometry();
        break;
    case 5:     //SCREEN MODE 1
        rect=ui->layoutmode1->geometry();
        break;
    case 6:     //SCREEN MODE 4
        rect=ui->layoutmode4->geometry();
        break;
    case 7:     //SCREEN MODE 9
        rect=ui->layoutmode9->geometry();
        break;
    case 8:     //SCREEN MODE 16
        rect=ui->layoutmode16->geometry();
        break;
    case 9:    //SCREEN MODE 8
        rect=ui->layoutmode8->geometry();
        break;
    case 10:    //SCREEN MODE 8-1
        rect=ui->layoutmode8_1->geometry();
        break;
    case 11:    //SCREEN MODE 12
        rect=ui->layoutmode12->geometry();
        break;
    case 12:    //SCREEN MODE 12-1
        rect=ui->layoutmode12_1->geometry();
        break;
    case 13:    //SCREEN MODE 14
        rect=ui->layoutmode14->geometry();
        break;
    case 14:    //SEQ
        rect=ui->sequencemode->geometry();
        break;
    case 15:    //AUDIO
        rect=ui->audioonoff->geometry();
        break;
    case 16:    //EVENT LOG
        rect=ui->eventlog->geometry();
        break;
    case 17:    //BACKUP
        rect=ui->backup->geometry();
        break;
    case 18:    //SETUP
        rect=ui->setupproperty->geometry();
        break;
    case 19:    //REC
        rect=ui->emergencyrecord->geometry();
        break;
    case 20:
        rect = ui->logout->geometry();
        break;
    default:
        break;
    }
    ui->toolbarcursor->setGeometry(rect.left()-1,rect.top()-1,rect.width()+2,rect.height()+2);
}
/*!front key 입력시 현재 cursor에 대한 실행 */
void dvr_toolbar_main::ExeCurrentSelection()
{
	//excute current selected item
    switch(toolbarcursorPosition)
    {
    case 0:     //LIVE
        if(ui->livebackmode->isEnabled())
            LivebackModeBtnSlot();
        break;
    case 1:     //PLAYBACK
        if(ui->playbackmode->isEnabled())
            PlaybackModeBtnSlot();
        break;
    case 2:     //SUB
        if(ui->livebacksubmode->isEnabled())
            LivebackSubModeBtnSlot();
        break;
    case 3:     //PREV
        if(ui->prevpage->isEnabled())
            PrevPageBtnSlot();
        break;
    case 4:     //NEXT
        if(ui->nextpage->isEnabled())
            NextPageBtnSlot();
        break;
    case 5:     //SCREEN MODE 1
        if(ui->layoutmode1->isEnabled())
            Layout1BtnSlot();
        break;
    case 6:     //SCREEN MODE 4
        if(ui->layoutmode4->isEnabled())
            Layout4BtnSlot();
        break;
    case 7:     //SCREEN MODE 9
        if(ui->layoutmode9->isEnabled())
            Layout9BtnSlot();
        break;
    case 8:     //SCREEN MODE 16
        if(ui->layoutmode16->isEnabled())
            Layout16BtnSlot();
        break;
    case 9:    //SCREEN MODE 8
        if(ui->layoutmode8->isEnabled())
            Layout8BtnSlot();
        break;
    case 10:    //SCREEN MODE 8-1
        if(ui->layoutmode8_1->isEnabled())
            Layout8_1BtnSlot();
        break;
    case 11:    //SCREEN MODE 12
        if(ui->layoutmode12->isEnabled())
            Layout12BtnSlot();
        break;
    case 12:    //SCREEN MODE 12-1
        if(ui->layoutmode12_1->isEnabled())
            Layout12_1BtnSlot();
        break;
    case 13:    //SCREEN MODE 14
        if(ui->layoutmode14->isEnabled())
            Layout14BtnSlot();
        break;
    case 14:    //SEQ
        if(ui->sequencemode->isEnabled())
            SequenceModeBtnSlot();
        break;
    case 15:    //AUDIO
        if(ui->audioonoff->isEnabled())
            AudioOnOffBtnSlot();
        break;
    case 16:    //EVENT LOG
        if(ui->eventlog->isEnabled())
            EventLogBtnSlot();
        break;
    case 17:    //BACKUP
        if(ui->backup->isEnabled())
            BackupBtnSlot();
        break;
    case 18:    //SETUP
        if(ui->setupproperty->isEnabled())
            SetupPropertyCallSlot();
        break;
    case 19:    //REC
        if(ui->emergencyrecord->isEnabled())
            EmergencyRecordSlot();
        break;
    case 20:
        if(ui->logout->isEnabled())
            LogoutSlot();
        break;
    default:
        break;
    }
}
/*!main toolbar 좌표 설정 */
void dvr_toolbar_main::SetMainWindowSize()
{
    setGeometry(coordinator.MainToolbar);
    ui->logout->setGeometry(coordinator.MainToolbar.width()-40,4,32,32);
    ShowCurrentSelection();
}
/*!setup button에 대한 enable/disable 처리 */
void dvr_toolbar_main::SetEnableSetupBtn(int iEnable)
{
	//change setup button enable or disable
    if(iEnable==0)
        ui->setupproperty->setEnabled(false);
    else
        ui->setupproperty->setEnabled(true);
}
/*!모든 button에 대해서 enable/disable 처리 */
void dvr_toolbar_main::SetEnableAllBtn(int iEnable)
{
	//change all buttons enable or disable
    if(iEnable==0)
    {
        bBlockenableChange=false;
        ui->livebacksubmode->setEnabled(false);
        ui->prevpage->setEnabled(false);
        ui->nextpage->setEnabled(false);
        ui->layoutcamflow->setEnabled(false);
        ui->layoutmode1->setEnabled(false);
        ui->layoutmode4->setEnabled(false);
        ui->layoutmode9->setEnabled(false);
        ui->layoutmode16->setEnabled(false);
        ui->layoutmode8->setEnabled(false);
        ui->layoutmode8_1->setEnabled(false);
        ui->layoutmode12->setEnabled(false);
        ui->layoutmode12_1->setEnabled(false);
        ui->layoutmode14->setEnabled(false);
        ui->layoutmode24->setEnabled(false);
        ui->sequencemode->setEnabled(false);
        ui->audioonoff->setEnabled(false);
        ui->eventlog->setEnabled(false);
        ui->snapshot->setEnabled(false);
        ui->backup->setEnabled(false);
        ui->setupproperty->setEnabled(false);
        ui->emergencyrecord->setEnabled(false);
    }
    else
    {
        bBlockenableChange=true;
        if(profile.iSubOutputEnable)
            ui->livebacksubmode->setEnabled(true);
        else
            ui->livebacksubmode->setEnabled(false);
        ui->prevpage->setEnabled(true);
        ui->nextpage->setEnabled(true);
        ui->layoutmode1->setEnabled(true);
        ui->layoutmode4->setEnabled(true);
        ui->layoutmode9->setEnabled(true);
        ui->layoutmode16->setEnabled(true);
        ui->layoutmode8->setEnabled(true);
        ui->layoutmode8_1->setEnabled(true);
        ui->layoutmode12->setEnabled(true);
        ui->layoutmode12_1->setEnabled(true);
        ui->layoutmode14->setEnabled(true);
        ui->sequencemode->setEnabled(true);
        ui->audioonoff->setEnabled(true);
        ui->eventlog->setEnabled(true);
        ui->backup->setEnabled(true);
        ui->setupproperty->setEnabled(true);
        ui->emergencyrecord->setEnabled(true);
    }
}
/*!sub toolbar button에 대한 enable/disable 처리 */
void dvr_toolbar_main::SetEnableSubToolbar(int iEnable)
{
	//change sub toolbar button enable/disable
    if(iEnable==1)
        ui->livebacksubmode->setEnabled(true);
    else
        ui->livebacksubmode->setEnabled(false);
}
/*!live mode button click시 호출 */
void dvr_toolbar_main::LivebackModeBtnSlot()
{
	//set live mode
    toolbarcursorPosition=0;
    runtimesettings.iLivePlaybackMode=LIB_LIVE_MODE;
    SetLivePlayBackBtnSelect(LIB_LIVE_MODE);
    pSidebar->setMode();
    pTimebar->hide();
#if DVR_API_ENABLE
	pSidebar->setPlaybackBtnSelect(PLAYBACKMODE_STOP);
      pSidebar->setPlayBackMode(PLAYBACKMODE_STOP);
#endif
    pLayoutClass->DrawOSDChannelLive();
    pStatusbar->SetMainWindowSize();
    dprintf("LivebackModeBtnSlot()\n");
    ShowCurrentSelection();
}
/*!playback mode button click시 호출 */
void dvr_toolbar_main::PlaybackModeBtnSlot()
{
	//set playback mode
    toolbarcursorPosition=1;
    runtimesettings.iLivePlaybackMode=LIB_PLAYBACK_MODE;
    SetLivePlayBackBtnSelect(LIB_PLAYBACK_MODE);
    if(runtimesettings.playbackinfo.playbackmode.iPlaybackMode == PLAYBACKMODE_STOP)
    {
        memcpy(&curtime.playbacktime,&curtime.currenttime,sizeof(struct tm));
        curtime.playbacktime.tm_sec=0;
    }
    memset(runtimesettings.playbackinfo.playbacktableinfo.iMonthTable,0,sizeof(runtimesettings.playbackinfo.playbacktableinfo.iMonthTable));
    LIB816x_GetRecDays(curtime.playbacktime,runtimesettings.playbackinfo.playbacktableinfo.iMonthTable);
    pSidebar->setCalendarRecordDate(&curtime.playbacktime,runtimesettings.playbackinfo.playbacktableinfo.iMonthTable);
    pSidebar->setMode();
    pSidebar->setCalendarSelectDay(&curtime.playbacktime,true);

    pTimebar->SetTimeTableChannel(0);
    pTimebar->SetTimeTableSelectTimeMarking(&curtime.playbacktime);
    pTimebar->SetTimeTableUpdate(true,true);
    pTimebar->show();

//#if DVR_API_ENABLE
//#endif
    pLayoutClass->DrawOSDChannelPlayback();
    pStatusbar->ChangePlaybackMainWindowSize(true);
    dprintf("PlaybackModeBtnSlot()\n");
    ShowCurrentSelection();
}
/*!sub toolbar button click시 호출 */
void dvr_toolbar_main::LivebackSubModeBtnSlot()
{
	//show/hide sub toolbar
    toolbarcursorPosition=2;
    if(runtimesettings.iSecondConsoleModeOn==SUBCONSOLEMODE_OFF)
    {
        runtimesettings.iSecondConsoleModeOn=SUBCONSOLEMODE_ON;
        pSubToolbar->show();
    }
    else
    {
        runtimesettings.iSecondConsoleModeOn=SUBCONSOLEMODE_OFF;
        pSubToolbar->hide();
    }
    dprintf("LivebackSubModeBtnSlot()\n");
    ShowCurrentSelection();
}
/*!previous button click시 호출 */
void dvr_toolbar_main::PrevPageBtnSlot()
{
	//change to previous page
    toolbarcursorPosition=3;

    if(runtimesettings.iPageNum>0)
        runtimesettings.iPageNum--;
    else
        runtimesettings.iPageNum=PageCount;
    pLayoutClass->DrawOSDChannel();

    dprintf("PrevPageBtnSlot()\n");
    ShowCurrentSelection();
#if DVR_API_ENABLE
    /*if(profile.iSubOutputEnable)
    {
        LIB816x_setCameraLayout(profile.layout.iMainOutput, runtimesettings.iLayout*runtimesettings.iPageNum, runtimesettings.iLayout);
    }
    else*/
    {
        if(runtimesettings.iLivePlaybackMode==LIB_LIVE_MODE)
        {
            if(runtimesettings.iLayout*runtimesettings.iPageNum<16)
                LIB816x_setCameraLayout(profile.layout.iMainOutput, runtimesettings.iLayout*runtimesettings.iPageNum, runtimesettings.iLayout);
        }
        else
        {
            if(runtimesettings.iLayout*runtimesettings.iPageNum<16)
                LIB816x_setCameraLayout(profile.layout.iMainOutput, 16+runtimesettings.iLayout*runtimesettings.iPageNum, runtimesettings.iLayout);
        }
    }
#endif
}
/*!next button click시 호출 */
void dvr_toolbar_main::NextPageBtnSlot()
{
	//change to next page
    toolbarcursorPosition=4;

    if(runtimesettings.iPageNum<PageCount)
        runtimesettings.iPageNum++;
    else
        runtimesettings.iPageNum=0;
    pLayoutClass->DrawOSDChannel();
    dprintf("NextPageBtnSlot()\n");
    ShowCurrentSelection();
#if DVR_API_ENABLE
   /* if(profile.iSubOutputEnable)
    {
        LIB816x_setCameraLayout(profile.layout.iMainOutput, runtimesettings.iLayout*runtimesettings.iPageNum, runtimesettings.iLayout);
    }
    else*/
    {
        if(runtimesettings.iLivePlaybackMode==LIB_LIVE_MODE)
        {
            if(runtimesettings.iLayout*runtimesettings.iPageNum<16)
                LIB816x_setCameraLayout(profile.layout.iMainOutput, runtimesettings.iLayout*runtimesettings.iPageNum, runtimesettings.iLayout);
        }
        else
        {
            if(runtimesettings.iLayout*runtimesettings.iPageNum<16)
                LIB816x_setCameraLayout(profile.layout.iMainOutput, 16+runtimesettings.iLayout*runtimesettings.iPageNum, runtimesettings.iLayout);
        }
    }
#endif
}
/*!sequence mode일때 일정한 시간마다 호출 */
void dvr_toolbar_main::NextPageBtnSlot_Sequence()
{
	//change next page in sequence mode
    if(runtimesettings.iPageNum<PageCount)
        runtimesettings.iPageNum++;
    else
        runtimesettings.iPageNum=0;
    pLayoutClass->DrawOSDChannel();
    dprintf("NextPageBtnSlot()\n");
#if DVR_API_ENABLE
   /* if(profile.iSubOutputEnable)
    {
    	LIB816x_setCameraLayout(profile.layout.iMainOutput, runtimesettings.iLayout*runtimesettings.iPageNum, runtimesettings.iLayout);
    }
    else*/
    {
        if(runtimesettings.iLivePlaybackMode==LIB_LIVE_MODE)
        {
            if(runtimesettings.iLayout*runtimesettings.iPageNum<16)
                LIB816x_setCameraLayout(profile.layout.iMainOutput, runtimesettings.iLayout*runtimesettings.iPageNum, runtimesettings.iLayout);
        }
        else
        {
            if(runtimesettings.iLayout*runtimesettings.iPageNum<16)
                LIB816x_setCameraLayout(profile.layout.iMainOutput, 16+runtimesettings.iLayout*runtimesettings.iPageNum, runtimesettings.iLayout);
        }
    }
#endif
}
/*!현재 layout mode에 대해서 Max page를 계산 */
void dvr_toolbar_main::MaxPageCalc()
{
	//find max page number by current layout mode
    if((MAX_CAMERA%runtimesettings.iLayout)==0)
        PageCount=MAX_CAMERA/runtimesettings.iLayout-1;
    else
        PageCount=MAX_CAMERA/runtimesettings.iLayout;
}
/*!LAYOUTMODE_1 button click시 호출 */
void dvr_toolbar_main::Layout1BtnSlot()
{
	//change layout mode to LAYOUTMODE_1
    toolbarcursorPosition=5;

   //profile.layout.iLayoutMode=LAYOUTMODE_1;
   runtimesettings.iLayout=LAYOUT_1;
   runtimesettings.iPageNum=0;
   
   MaxPageCalc();
   SetLayoutModeBtnSelect(LAYOUTMODE_1);
   pLayoutClass->SetLayoutMode(LAYOUTMODE_1);
   pLayoutClass->DrawOSDChannel();
#if DVR_API_ENABLE
       if(runtimesettings.iLivePlaybackMode == LIB_LIVE_MODE)	
       {
    		LIB816x_setDisplayLayout(LAYOUTMODE_1, 0);
		profile.layout.iLayoutMode=LAYOUTMODE_1;
       }
   	else
   	{
	  	LIB816x_setPlaybackDisplayLayout(LAYOUTMODE_1, 0);
		profile.layout.iSecondLayoutMode=LAYOUTMODE_1;
   	}
    /*LIB816x_setDisplayLayout(LAYOUTMODE_1, 0);
    if(!profile.iSubOutputEnable && runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
    {
        LIB816x_setCameraLayout(profile.layout.iMainOutput, 16, runtimesettings.iLayout);
    }*/
#endif
   dprintf("LayoutBtnSlot1()\n");
   ShowCurrentSelection();
   WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));

}
/*!LAYOUTMODE_4 button click시 호출 */
void dvr_toolbar_main::Layout4BtnSlot()
{
	//change layout mode to LAYOUTMODE_4
    toolbarcursorPosition=6;

    //profile.layout.iLayoutMode=LAYOUTMODE_4;
    runtimesettings.iLayout=LAYOUT_4;
    runtimesettings.iPageNum=0;
    
    MaxPageCalc();
    SetLayoutModeBtnSelect(LAYOUTMODE_4);
    pLayoutClass->SetLayoutMode(LAYOUTMODE_4);
    pLayoutClass->DrawOSDChannel();
#if DVR_API_ENABLE
        if(runtimesettings.iLivePlaybackMode == LIB_LIVE_MODE)	
       {
    		LIB816x_setDisplayLayout(LAYOUTMODE_4, 0);
		profile.layout.iLayoutMode=LAYOUTMODE_4;
       }
   	else
   	{
	  	LIB816x_setPlaybackDisplayLayout(LAYOUTMODE_4, 0);
		profile.layout.iSecondLayoutMode=LAYOUTMODE_4;
   	}
    /*LIB816x_setDisplayLayout(LAYOUTMODE_4, 0);
    if(!profile.iSubOutputEnable && runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
    {
        LIB816x_setCameraLayout(profile.layout.iMainOutput, 16, runtimesettings.iLayout);
    }*/
#endif
   dprintf("LayoutBtnSlot4()\n");
   ShowCurrentSelection();
   WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));
}
/*!LAYOUTMODE_9 button click시 호출 */
void dvr_toolbar_main::Layout9BtnSlot()
{
	//change layout mode to LAYOUTMODE_9
    toolbarcursorPosition=7;

    //profile.layout.iLayoutMode=LAYOUTMODE_9;
    runtimesettings.iLayout=LAYOUT_9;
    runtimesettings.iPageNum=0;
    
    MaxPageCalc();
    SetLayoutModeBtnSelect(LAYOUTMODE_9);
    pLayoutClass->SetLayoutMode(LAYOUTMODE_9);
    pLayoutClass->DrawOSDChannel();
#if DVR_API_ENABLE
       if(runtimesettings.iLivePlaybackMode == LIB_LIVE_MODE)	
       {
    		LIB816x_setDisplayLayout(LAYOUTMODE_9, 0);
		profile.layout.iLayoutMode=LAYOUTMODE_9;
       }
   	else
   	{
	  	LIB816x_setPlaybackDisplayLayout(LAYOUTMODE_9, 0);
		profile.layout.iSecondLayoutMode=LAYOUTMODE_9;
   	}
#endif
   dprintf("LayoutBtnSlot9()\n");
   ShowCurrentSelection();
   WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));
}
/*!LAYOUTMODE_16 button click시 호출 */
void dvr_toolbar_main::Layout16BtnSlot()
{
	//change layout mode to LAYOUTMODE_16
    toolbarcursorPosition=8;
	
    //profile.layout.iLayoutMode=LAYOUTMODE_16;
    runtimesettings.iLayout=LAYOUT_16;
    runtimesettings.iPageNum=0;
    
    MaxPageCalc();
    SetLayoutModeBtnSelect(LAYOUTMODE_16);
    pLayoutClass->SetLayoutMode(LAYOUTMODE_16);
    pLayoutClass->DrawOSDChannel();
#if DVR_API_ENABLE
       if(runtimesettings.iLivePlaybackMode == LIB_LIVE_MODE)	
       {
    		LIB816x_setDisplayLayout(LAYOUTMODE_16, 0);
		profile.layout.iLayoutMode=LAYOUTMODE_16;
       }
   	else
   	{
	  	LIB816x_setPlaybackDisplayLayout(LAYOUTMODE_16, 0);
		profile.layout.iSecondLayoutMode=LAYOUTMODE_16;
   	}
#endif
   dprintf("LayoutBtnSlot16()\n");
   ShowCurrentSelection();
   WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));
}
/*!LAYOUTMODE_8 button click시 호출 */
void dvr_toolbar_main::Layout8BtnSlot()
{
	//change layout mode to LAYOUTMODE_8
    toolbarcursorPosition=9;

    //profile.layout.iLayoutMode=LAYOUTMODE_8;
    runtimesettings.iLayout=LAYOUT_8;
    runtimesettings.iPageNum=0;
    
    MaxPageCalc();
    SetLayoutModeBtnSelect(LAYOUTMODE_8);
    pLayoutClass->SetLayoutMode(LAYOUTMODE_8);
    pLayoutClass->DrawOSDChannel();
#if DVR_API_ENABLE
       if(runtimesettings.iLivePlaybackMode == LIB_LIVE_MODE)	
       {
    		LIB816x_setDisplayLayout(LAYOUTMODE_8, 0);
		profile.layout.iLayoutMode=LAYOUTMODE_8;
       }
   	else
   	{
	  	LIB816x_setPlaybackDisplayLayout(LAYOUTMODE_8, 0);
		profile.layout.iSecondLayoutMode=LAYOUTMODE_8;
   	}
#endif
   dprintf("LayoutBtnSlot8()\n");
   ShowCurrentSelection();
   WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));
}
/*!LAYOUTMODE_8_1 button click시 호출 */
void dvr_toolbar_main::Layout8_1BtnSlot()
{
	//change layout mode to LAYOUTMODE_8_1
    toolbarcursorPosition=10;

    //profile.layout.iLayoutMode=LAYOUTMODE_8_1;
    runtimesettings.iLayout=LAYOUT_8;
    runtimesettings.iPageNum=0;
    
    MaxPageCalc();
    SetLayoutModeBtnSelect(LAYOUTMODE_8_1);
    pLayoutClass->SetLayoutMode(LAYOUTMODE_8_1);
    pLayoutClass->DrawOSDChannel();
#if DVR_API_ENABLE
      if(runtimesettings.iLivePlaybackMode == LIB_LIVE_MODE)	
       {
    		LIB816x_setDisplayLayout(LAYOUTMODE_8_1, 0);
		profile.layout.iLayoutMode=LAYOUTMODE_8_1;
       }
   	else
   	{
	  	LIB816x_setPlaybackDisplayLayout(LAYOUTMODE_8_1, 0);
		profile.layout.iSecondLayoutMode=LAYOUTMODE_8_1;
   	}
   /*LIB816x_setDisplayLayout(LAYOUTMODE_8_1, 0);
    if(!profile.iSubOutputEnable && runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
    {
        LIB816x_setCameraLayout(profile.layout.iMainOutput, 16, runtimesettings.iLayout);
    }*/
#endif
   dprintf("LayoutBtnSlot8_1()\n");
   ShowCurrentSelection();
   WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));
}
/*!LAYOUTMODE_12 button click시 호출 */
void dvr_toolbar_main::Layout12BtnSlot()
{
	//change layout mode to LAYOUTMODE_12
    toolbarcursorPosition=11;

    //profile.layout.iLayoutMode=LAYOUTMODE_12;
    runtimesettings.iLayout=LAYOUT_12;
    runtimesettings.iPageNum=0;
    
    MaxPageCalc();
    SetLayoutModeBtnSelect(LAYOUTMODE_12);
    pLayoutClass->SetLayoutMode(LAYOUTMODE_12);
    pLayoutClass->DrawOSDChannel();
#if DVR_API_ENABLE
       if(runtimesettings.iLivePlaybackMode == LIB_LIVE_MODE)	
       {
    		LIB816x_setDisplayLayout(LAYOUTMODE_12, 0);
		profile.layout.iLayoutMode=LAYOUTMODE_12;
       }
   	else
   	{
	  	LIB816x_setPlaybackDisplayLayout(LAYOUTMODE_12, 0);
		profile.layout.iSecondLayoutMode=LAYOUTMODE_12;
   	}
   /* LIB816x_setDisplayLayout(LAYOUTMODE_12, 0);
    if(!profile.iSubOutputEnable && runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
    {
        LIB816x_setCameraLayout(profile.layout.iMainOutput, 16, runtimesettings.iLayout);
    }*/
#endif
   dprintf("LayoutBtnSlot12()\n");
   ShowCurrentSelection();
   WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));
}
/*!LAYOUTMODE_12_1 button click시 호출 */
void dvr_toolbar_main::Layout12_1BtnSlot()
{
	//change layout mode to LAYOUTMODE_12_1
    toolbarcursorPosition=12;

    //profile.layout.iLayoutMode=LAYOUTMODE_12_1;
    runtimesettings.iLayout=LAYOUT_12;
    runtimesettings.iPageNum=0;
    
    MaxPageCalc();
    SetLayoutModeBtnSelect(LAYOUTMODE_12_1);
    pLayoutClass->SetLayoutMode(LAYOUTMODE_12_1);
    pLayoutClass->DrawOSDChannel();
#if DVR_API_ENABLE
	if(runtimesettings.iLivePlaybackMode == LIB_LIVE_MODE)	
       {
    		LIB816x_setDisplayLayout(LAYOUTMODE_12_1, 0);
		profile.layout.iLayoutMode=LAYOUTMODE_12_1;
       }
   	else
   	{
	  	LIB816x_setPlaybackDisplayLayout(LAYOUTMODE_12_1, 0);
		profile.layout.iSecondLayoutMode=LAYOUTMODE_12_1;
   	}
/*
    LIB816x_setDisplayLayout(LAYOUTMODE_12_1, 0);
    if(!profile.iSubOutputEnable && runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
    {
        LIB816x_setCameraLayout(profile.layout.iMainOutput, 16, runtimesettings.iLayout);
    }*/
#endif
   dprintf("LayoutBtnSlot12_1()\n");
   ShowCurrentSelection();
   WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));
}
/*!LAYOUTMODE_14 button click시 호출 */
void dvr_toolbar_main::Layout14BtnSlot()
{
	//change layout mode to LAYOUTMODE_14
    toolbarcursorPosition=13;

    //profile.layout.iLayoutMode=LAYOUTMODE_14;
    runtimesettings.iLayout=LAYOUT_14;
    runtimesettings.iPageNum=0;
    
    MaxPageCalc();
    SetLayoutModeBtnSelect(LAYOUTMODE_14);
    pLayoutClass->SetLayoutMode(LAYOUTMODE_14);
    pLayoutClass->DrawOSDChannel();
#if DVR_API_ENABLE
      if(runtimesettings.iLivePlaybackMode == LIB_LIVE_MODE)	
       {
    		LIB816x_setDisplayLayout(LAYOUTMODE_14, 0);
		profile.layout.iLayoutMode=LAYOUTMODE_14;
       }
   	else
   	{
	  	LIB816x_setPlaybackDisplayLayout(LAYOUTMODE_14, 0);
		profile.layout.iSecondLayoutMode=LAYOUTMODE_14;
   	}
    /*LIB816x_setDisplayLayout(LAYOUTMODE_14, 0);
    if(!profile.iSubOutputEnable && runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
    {
        LIB816x_setCameraLayout(profile.layout.iMainOutput, 16, runtimesettings.iLayout);
    }*/
#endif
   dprintf("LayoutBtnSlot14()\n");
   ShowCurrentSelection();
   WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));
}
/*!sequence mode button click시 호출 */
void dvr_toolbar_main::SequenceModeBtnSlot()
{
	//toggle sequence mode on/off
    toolbarcursorPosition=14;
    if(profile.sequence.iSequenceModeOn==0)
    {
        profile.sequence.iSequenceModeOn=1;
        pMainClass->MainlayoutTimerStart();
    }
    else
    {
        profile.sequence.iSequenceModeOn=0;
        pMainClass->MainlayoutTimerStop();
    }

    dprintf("SequenceModeBtnSlot()\n");
    ShowCurrentSelection();
    WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));
}
/*!audio button click시 호출 */
void dvr_toolbar_main::AudioOnOffBtnSlot()
{
	//toggle audio on/off
    toolbarcursorPosition=15;
    dprintf("AudioOnOffBtnSlot()\n");
    if(runtimesettings.AudioMuteOnOff)
    {
        runtimesettings.AudioMuteOnOff = 0;
    }
    else
    {
        runtimesettings.AudioMuteOnOff = 1;
    }
#if	DVR_API_ENABLE
    if(runtimesettings.AudioMuteOnOff)
    {
        LIB816x_setAudioOutput(profile.audioout.iAudioOutOn, AUDIO_VOLUME_MUTE, profile.audioout.iSelectAudioIn);
    }
    else
    {
        LIB816x_setAudioOutput(profile.audioout.iAudioOutOn, profile.audioout.iVolume, profile.audioout.iSelectAudioIn);
    }
#endif
    		ShowCurrentSelection();
}
/*!event log button click시 호출 */
void dvr_toolbar_main::EventLogBtnSlot()
{
	if(runtimesettings.iLivePlaybackMode == LIB_PLAYBACK_MODE)
		return;
		//show event log dialog
	    toolbarcursorPosition=16;
	    dprintf("EventLogBtnSlot()\n");
	    pEvtDlg->setdate(&curtime.currenttime);
	    pEvtDlg->show();
	    pEvtDlg->ShowLog();
	    ShowCurrentSelection();
	
}
/*!snapshot button click시 호출 */
void dvr_toolbar_main::SnapshotBtnSlot()
{
	if(runtimesettings.iLivePlaybackMode == LIB_PLAYBACK_MODE)
		return;
		//not implemented
	    toolbarcursorPosition=18;
	    dprintf("SnapshotBtnSlot()\n");
	    ShowCurrentSelection();
}
/*!backup button click시 호출 */
void dvr_toolbar_main::BackupBtnSlot()
{
	if(runtimesettings.iLivePlaybackMode == LIB_PLAYBACK_MODE)
		return;
		//show backup dialog
	    toolbarcursorPosition=17;
	    dprintf("BackupBtnSlot()\n");
	    ShowCurrentSelection();

	    pSetup->m_iSetupTabIndex=SETUP_BACKUP;
	    runtimesettings.iSetupPropertyOn=SETUPPROPERTY_ON;
	    pSetup->m_allTabEnable=false;
	    setEnabled(false);
	    pSidebar->setEnabled(false);
	    pSubToolbar->hide();
	    pTimebar->setEnabled(false);
	    memcpy(&dlgdata,&profile,sizeof(PROFILE));
	    memcpy(&dlgdatatemp,&profile,sizeof(PROFILE));
	    memcpy(&dlgruntimesettings,&runtimesettings,sizeof(RUNTIMESETTINGS));
	    memcpy(&dlgruntimesettingstemp,&runtimesettings,sizeof(RUNTIMESETTINGS));
	    pSetup->show();
	
}
/*!setup button click시 호출 */
void dvr_toolbar_main::SetupPropertyCallSlot()
{
	if(runtimesettings.iLivePlaybackMode == LIB_PLAYBACK_MODE)
		return;
	//show setup dialog
    toolbarcursorPosition=18;
    runtimesettings.iSetupPropertyOn=SETUPPROPERTY_ON;
    static bool bFirstSetData = false;
    pSetup->m_allTabEnable=true;
    if(profile.sequence.iSequenceModeOn)
    {
        pMainClass->MainlayoutTimerStop();
    }
    pSetup->m_iSetupTabIndex=0;
    setEnabled(false);
    pSidebar->setEnabled(false);
    pSubToolbar->hide();
    pTimebar->setEnabled(false);
    memcpy(&dlgdata,&profile,sizeof(PROFILE));
    memcpy(&dlgdatatemp,&profile,sizeof(PROFILE));
    memcpy(&dlgruntimesettings,&runtimesettings,sizeof(RUNTIMESETTINGS));
    memcpy(&dlgruntimesettingstemp,&runtimesettings,sizeof(RUNTIMESETTINGS));
    memcpy(&dlguserlist,&userlist,sizeof(USERLIST));
    if(bFirstSetData==false)
    {
        pSetup->SetDisplaydata();
        pSetup->SetCameradata();
        pSetup->SetRecorddata();
        pSetup->SetEventdata();
        pSetup->SetDiodata();
        pSetup->SetAudiodata();
        pSetup->SetNetworkdata();
        pSetup->SetSystemdata();
        bFirstSetData=true;
    }
    pSetup->show();
    dprintf("SetupPropertyCallSlot()\n");
    ShowCurrentSelection();
}
/*!emergency record button click시 호출 */
void dvr_toolbar_main::EmergencyRecordSlot()
{
	if(runtimesettings.iLivePlaybackMode == LIB_PLAYBACK_MODE)
		return;
	//toggle emergency record on/off
    toolbarcursorPosition=19;
    if(profile.dvrall.iRecordEmergencyOn==0)
        profile.dvrall.iRecordEmergencyOn=1;
    else
        profile.dvrall.iRecordEmergencyOn=0;
    pMainClass->EmergencyRecordAll();
    dprintf("EmergencyRecordSlot()\n");

    ShowCurrentSelection();
}
/*!logout button click시 호출 */
void dvr_toolbar_main::LogoutSlot()
{
	//log out - show login dialog
    toolbarcursorPosition=20;
    dprintf("LogoutSlot()\n");

    pLoginDlg->setFocus();
    pLoginDlg->DvrLoginShow();
    setEnabled(false);
    pSidebar->setEnabled(false);
    runtimesettings.iSecondConsoleModeOn=SUBCONSOLEMODE_OFF;
    pSubToolbar->hide();
    pTimebar->setEnabled(false);
    SetEnableSetupBtn(0);
    pMainClass->Logout();
    ShowCurrentSelection();
}
/*!live/playback mode에 따라서 live/playback button을 enable/disable */
void dvr_toolbar_main::SetLivePlayBackBtnSelect(int iMode)
{
	//enable/disable live,playback button
    switch(iMode)
    {
    case LIB_LIVE_MODE :
        ui->livebackmode->setEnabled(false);
        ui->playbackmode->setEnabled(true);
    break;
    case LIB_PLAYBACK_MODE:
        ui->livebackmode->setEnabled(true);
        ui->playbackmode->setEnabled(false);
    break;
    }
}
/*!layout mode에 따라서 해당 mode의 button을 disable */
void dvr_toolbar_main::SetLayoutModeBtnSelect(int iMode)
{
    static int currentMode=100;
    if(bBlockenableChange==false)
        return;
	//disable current layout button and enable other layout button
    switch(currentMode)
    {
    case LAYOUTMODE_1:
        ui->layoutmode1->setEnabled(true);
    break;
    case LAYOUTMODE_4:
        ui->layoutmode4->setEnabled(true);
    break;
    case LAYOUTMODE_9:
        ui->layoutmode9->setEnabled(true);
    break;
    case LAYOUTMODE_16:
        ui->layoutmode16->setEnabled(true);
    break;
    case LAYOUTMODE_8:
        ui->layoutmode8->setEnabled(true);
    break;
    case LAYOUTMODE_8_1:
        ui->layoutmode8_1->setEnabled(true);
    break;
    case LAYOUTMODE_12:
        ui->layoutmode12->setEnabled(true);
    break;
    case LAYOUTMODE_12_1:
        ui->layoutmode12_1->setEnabled(true);
    break;
    case LAYOUTMODE_14:
        ui->layoutmode14->setEnabled(true);
    break;
    default:
        ui->layoutmode1->setEnabled(true);
        ui->layoutmode4->setEnabled(true);
        ui->layoutmode9->setEnabled(true);
        ui->layoutmode16->setEnabled(true);
        ui->layoutmode8->setEnabled(true);
        ui->layoutmode8_1->setEnabled(true);
        ui->layoutmode12->setEnabled(true);
        ui->layoutmode12_1->setEnabled(true);
        ui->layoutmode14->setEnabled(true);
    break;
    }

    currentMode = iMode;

    switch(iMode)
    {
    case LAYOUTMODE_1:
        ui->layoutmode1->setEnabled(false);
    break;
    case LAYOUTMODE_4:
        ui->layoutmode4->setEnabled(false);
    break;
    case LAYOUTMODE_9:
        ui->layoutmode9->setEnabled(false);
    break;
    case LAYOUTMODE_16:
        ui->layoutmode16->setEnabled(false);
    break;
    case LAYOUTMODE_8:
        ui->layoutmode8->setEnabled(false);
    break;
    case LAYOUTMODE_8_1:
        ui->layoutmode8_1->setEnabled(false);
    break;
    case LAYOUTMODE_12:
        ui->layoutmode12->setEnabled(false);
    break;
    case LAYOUTMODE_12_1:
        ui->layoutmode12_1->setEnabled(false);
    break;
    case LAYOUTMODE_14:
        ui->layoutmode14->setEnabled(false);
    break;
    default:
        ui->layoutmode1->setEnabled(true);
        ui->layoutmode4->setEnabled(true);
        ui->layoutmode9->setEnabled(true);
        ui->layoutmode16->setEnabled(true);
        ui->layoutmode8->setEnabled(true);
        ui->layoutmode8_1->setEnabled(true);
        ui->layoutmode12->setEnabled(true);
        ui->layoutmode12_1->setEnabled(true);
        ui->layoutmode14->setEnabled(true);
    break;
    }
}

