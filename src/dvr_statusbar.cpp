/******************************************************************************
* DM8168 Hybrid DVR
* Copyright by UDWorks, Incoporated. All Rights Reserved.
*-----------------------------------------------------------------------------
* @file	dvr_statusbar.cpp
* @brief
* @author
* @section	MODIFY history
*     - 2011.01.01 : First Created
******************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include "dvr_defines.h"
#include "dvr_statusbar.h"
#include "ui_dvr_statusbar.h"
#include <time.h>

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 local functions
-----------------------------------------------------------------------------*/
/*!constructor*/
dvr_statusbar::dvr_statusbar(QWidget *parent) :
    QDialog(parent,Qt::CustomizeWindowHint |Qt::FramelessWindowHint),
    ui(new Ui::dvr_statusbar)
{
    ui->setupUi(this);
}
/*!destructor */
dvr_statusbar::~dvr_statusbar()
{
    delete ui;
}

void dvr_statusbar::changeEvent(QEvent *e)
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

/*!status bar의 좌표 설정 */
void dvr_statusbar::SetMainWindowSize()
{
	//set status bar size in live mode
    setGeometry(coordinator.Statusbar);
    SetDateTimeEditSize();
    setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 255);"));
}
/*!playback mode에서 status bar의 좌표설정 (timebar show여부에 따라 달라짐) */
void dvr_statusbar::ChangePlaybackMainWindowSize(bool displaytimebar)
{
    if(runtimesettings.iLivePlaybackMode!=LIB_PLAYBACK_MODE)
        return;
    //set status bar size in playback mode

    this->setGeometry(coordinator.Statusbar);
    int x=(width()/2)-(ui->dvrmainstatusbardatetimeedit->width()/2)-(ui->DiskInfoLabel->width()/2);
    int y=MARGIN_4;
    int w=ui->dvrmainstatusbardatetimeedit->width();
    int h=ui->dvrmainstatusbardatetimeedit->height();
    ui->dvrmainstatusbardatetimeedit->setGeometry(x,y,w,h);
    if(displaytimebar==false)
    {
    	//timebar hide -> change to small size
        x=x+geometry().left();
        y=y+geometry().top();
        w=w+ui->DiskInfoLabel->width();
        h=h;
        setGeometry(x,y,w,h);
        w=ui->dvrmainstatusbardatetimeedit->width();
        ui->dvrmainstatusbardatetimeedit->setGeometry(0,0,w,h);
        w=ui->DiskInfoLabel->width();
        ui->DiskInfoLabel->setGeometry(ui->dvrmainstatusbardatetimeedit->width(),0,w,h);
        setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 255);"));
    }
    else
    {
    	//timebar show -> change to big size
        SetDateTimeEditSize();
        setStyleSheet(QString::fromUtf8("background-color: rgb(40, 72, 110);"));
    }
}
/*!status bar의 좌표 설정 */
void dvr_statusbar::SetDateTimeEditSize()
{
    int x=(width()/2)-(ui->dvrmainstatusbardatetimeedit->width()/2)-(ui->DiskInfoLabel->width()/2);
    int y=MARGIN_4;
    int w=ui->dvrmainstatusbardatetimeedit->width();
    int h=ui->dvrmainstatusbardatetimeedit->height();
    ui->dvrmainstatusbardatetimeedit->setGeometry(x,y,w,h);
    if(runtimesettings.iLivePlaybackMode==LIB_LIVE_MODE)
    {
        x=x+geometry().left();
        y=y+geometry().top();
        w=w+ui->DiskInfoLabel->width();
        h=h;
        setGeometry(x,y,w,h);
        w=ui->dvrmainstatusbardatetimeedit->width();
        ui->dvrmainstatusbardatetimeedit->setGeometry(0,0,w,h);
        w=ui->DiskInfoLabel->width();
        ui->DiskInfoLabel->setGeometry(ui->dvrmainstatusbardatetimeedit->width(),0,w,h);
    }
    else
    {
        x = x + ui->dvrmainstatusbardatetimeedit->width();
        w=ui->DiskInfoLabel->width();
        ui->DiskInfoLabel->setGeometry(x,y,w,h);
    }
    ui->dvrmainstatusbardatetimeedit->setStyleSheet("background-color: rgb(0, 0, 0); color: rgb(255, 255, 255); border:0");
}

/*!status bar의 date/time update */
void dvr_statusbar::UpdateMainDateTime(struct tm *ptm)
{
	//update current time or playback time
    struct tm tmcurtime;
    time_t curtime;
    QDateTime qdatetime;
    memcpy(&tmcurtime,ptm,sizeof(struct tm));
    tmcurtime.tm_year=ptm->tm_year-1900;
    curtime = mktime(&tmcurtime);
    qdatetime.setTime_t(curtime);
    ui->dvrmainstatusbardatetimeedit->setDateTime(qdatetime);
}

/*!status bar의 HDD 사용상태 update */
void dvr_statusbar::UpdateHDDInfo(double use,double total)
{
	//update hdd usage info
    char text[30];
    if(total != 0)
    {
        int percent = (int)((use * 100) / total);
        sprintf(text,"HDD: %d(%%)",percent);
    }
    else
    {
        sprintf(text,"No HDD");
    }
    ui->DiskInfoLabel->setText(StrToQString(text));
}

/*!status bar의 date/time 표시방법을 변경 */
void dvr_statusbar::SetDateFormat(int iDateFormat)
{
	//change date/time format
    switch(iDateFormat)
    {
        case OSD_DATEFORMAT_MMDDYYYY_AP:
            ui->dvrmainstatusbardatetimeedit->setDisplayFormat(QString::fromUtf8("MM.dd.yyyy AP h:mm:ss"));
            break;
        case OSD_DATEFORMAT_DDMMYYYY_AP:
            ui->dvrmainstatusbardatetimeedit->setDisplayFormat(QString::fromUtf8("dd.MM.yyyy. AP h:mm:ss"));
            break;
        case OSD_DATEFORMAT_YYYYDDMM_AP:
            ui->dvrmainstatusbardatetimeedit->setDisplayFormat(QString::fromUtf8("yyyy.dd.MM. AP h:mm:ss"));
            break;
        case OSD_DATEFORMAT_YYYYMMDD_AP:
            ui->dvrmainstatusbardatetimeedit->setDisplayFormat(QString::fromUtf8("yyyy.MM.dd. AP h:mm:ss"));
            break;
        case OSD_DATEFORMAT_MMDDYYYY:
            ui->dvrmainstatusbardatetimeedit->setDisplayFormat(QString::fromUtf8("MM.dd.yyyy hh:mm:ss"));
            break;
        case OSD_DATEFORMAT_DDMMYYYY:
            ui->dvrmainstatusbardatetimeedit->setDisplayFormat(QString::fromUtf8("dd.MM.yyyy. hh:mm:ss"));
            break;
        case OSD_DATEFORMAT_YYYYDDMM:
            ui->dvrmainstatusbardatetimeedit->setDisplayFormat(QString::fromUtf8("yyyy.dd.MM. hh:mm:ss"));
            break;
        case OSD_DATEFORMAT_YYYYMMDD:
            ui->dvrmainstatusbardatetimeedit->setDisplayFormat(QString::fromUtf8("yyyy.MM.dd. hh:mm:ss"));
            break;
    }
}


