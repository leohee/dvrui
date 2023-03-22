/******************************************************************************
* DM8168 Hybrid DVR
* Copyright by UDWorks, Incoporated. All Rights Reserved.
*-----------------------------------------------------------------------------
* @file	dvr_timelinebar_play.cpp
* @brief
* @author
* @section	MODIFY history
*     - 2011.01.01 : First Created
******************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include "dvr_defines.h"
#include "dvr_timelinebar_play.h"
#include "ui_dvr_timelinebar_play.h"
#include "dvr_main.h"
#include <QList>

/*!----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
extern dvrmain *pMainClass;
/*----------------------------------------------------------------------------
 local functions
-----------------------------------------------------------------------------*/
/*!constructor*/
dvr_timelinebar_play::dvr_timelinebar_play(QWidget *parent) :
    QDialog(parent,Qt::CustomizeWindowHint |Qt::FramelessWindowHint),
    ui(new Ui::dvr_timelinebar_play)
{
	int i=0;
	
    m_iTimelineBand=1;
    m_iTimelineH=TIMELINE_H;
    m_iTimelineCol=PLAYBACKTIMELINEMAX_COL;
    m_iTimelineRow=PLAYBACKTIMELINEMAX_ROW;
    m_iTimelineW=TIMELINE_FW;
    m_iTimelineStep=TIMELINE_STEPF;
    cursorPosition=0;
    m_itemcolor[EVENT_NORECORD]=Qt::lightGray;
    m_itemcolor[EVENT_RECORDONLY]=Qt::red;
    m_itemcolor[EVENT_RECORDMOTION]=Qt::yellow;
    m_itemcolor[EVENT_RECORDSENSOR]=Qt::blue;
    m_itemcolor[EVENT_RECORDVIDEOLOSS]=Qt::cyan;
    m_itemcolor[EVENT_RECORDEMERGENCY]=Qt::black;
    ui->setupUi(this);
    ui->timetablechanneltitle->setGeometry(0,0,120,20);
    ui->timetablechannel1->setGeometry(0,20,120,20);
    ui->timetablechannel2->setGeometry(0,40,120,20);
    ui->timetablechannel3->setGeometry(0,60,120,20);
    ui->timetablechannel4->setGeometry(0,80,120,20);

    ui->timetablechanneltitle->setText(StrToQString((char*)"CAMERA/TIME"));
    ui->timetable->setGeometry(TIMELINE_X,TIMELINE_Y,m_iTimelineW,m_iTimelineH);
    ui->timetableheader->setGeometry(TIMELINEHEADER_X,TIMELINEHEADER_Y,m_iTimelineW,TIMELINEHEADER_H);
    ui->timelinechannelup->setGeometry(MARGIN_10+13+ui->timetable->x()+ui->timetable->width(),ui->timetable->y()-5,ui->timelinechannelup->width(),ui->timelinechannelup->height());
    ui->timelinechanneldown->setGeometry(MARGIN_10+13+ui->timetable->x()+ui->timetable->width(),ui->timetable->y()-5+ui->timetable->height()-ui->timelinechanneldown->height()-MARGIN_4,ui->timelinechanneldown->width(),ui->timelinechanneldown->height());
    ui->timelinezoomin->setGeometry(ui->timelinechannelup->x()+ui->timelinechannelup->width()+MARGIN_10,ui->timetable->y()-5,ui->timelinezoomin->width(),ui->timelinezoomin->height());
    ui->timelinezoomout->setGeometry(ui->timelinezoomin->x()+ui->timelinezoomin->width()+MARGIN_10,ui->timetable->y()-5,ui->timelinezoomout->width(),ui->timelinezoomout->height());
    ui->timelinebandleft->setGeometry(ui->timelinezoomin->x(),ui->timetable->y()+ui->timetable->height()-ui->timelinebandleft->height()-MARGIN_4-5,ui->timelinebandleft->width(),ui->timelinebandleft->height());
    ui->timelinebandright->setGeometry(ui->timelinezoomout->x(),ui->timetable->y()+ui->timetable->height()-ui->timelinebandright->height()-MARGIN_4-5,ui->timelinebandright->width(),ui->timelinebandright->height());

    connect(ui->timelinechannelup, SIGNAL(clicked()), this, SLOT(TimeLineChannelUpBtnSlot()));
    connect(ui->timelinechanneldown, SIGNAL(clicked()), this, SLOT(TimeLineChannelDownBtnSlot()));
    connect(ui->timelinezoomin, SIGNAL(clicked()), this, SLOT(TimeLineZoomInBtnSlot()));
    connect(ui->timelinezoomout, SIGNAL(clicked()), this, SLOT(TimeLineZoomOutBtnSlot()));
    connect(ui->timelinebandleft, SIGNAL(clicked()), this, SLOT(TimeLineBandLeftBtnSlot()));
    connect(ui->timelinebandright, SIGNAL(clicked()), this, SLOT(TimeLineBandRightBtnSlot()));
    connect(ui->timetable, SIGNAL(itemSelectionChanged()), this, SLOT(ItemSelectionChangedSlot()));
    m_iChannelName[0]=ui->timetablechannel1;
    m_iChannelName[1]=ui->timetablechannel2;
    m_iChannelName[2]=ui->timetablechannel3;
    m_iChannelName[3]=ui->timetablechannel4;

    ui->timetable->setRowCount(m_iTimelineRow);
    ui->timetable->setColumnCount(m_iTimelineCol);
    for(i=0;i<m_iTimelineCol;i++)
    {
        ui->timetable->setColumnWidth(i,m_iTimelineStep);
    }
   ui->timetableheader->setRowCount(1);
   ui->timetableheader->setColumnCount(24);
   char buf[MAX_CHAR_4];
   for(i=0;i<24;i++)
   {
        ui->timetableheader->setColumnWidth(i,60);
        sprintf(buf,"%d",i);
        m_tableheaderitem[i].setText(StrToQString(buf));
        ui->timetableheader->setItem(0,i,&m_tableheaderitem[i]);
   }
}
/*!destructor */
dvr_timelinebar_play::~dvr_timelinebar_play()
{
    delete ui;
}

void dvr_timelinebar_play::changeEvent(QEvent *e)
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
void dvr_timelinebar_play::wheelEvent(QWheelEvent *event)
{
    event->ignore();
    if(runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
    {
        pMainClass->wheelEvent(event);
    }
}
/*!timebar의 좌표 설정 */
void dvr_timelinebar_play::SetMainWindowSize()
{
    setGeometry(coordinator.TimeLinebar);
    ui->timelineframe->setGeometry(0,0,width(),height());
}
/*!channel up button click시 호출 */
void dvr_timelinebar_play::TimeLineChannelUpBtnSlot()
{
    if(runtimesettings.playbacktable.iTimeTablePageChannel>0)
        runtimesettings.playbacktable.iTimeTablePageChannel--;
    else
        runtimesettings.playbacktable.iTimeTablePageChannel=MAX_PAGECHANNEL-1;
    SetTimeTableChannel(runtimesettings.playbacktable.iTimeTablePageChannel);
    dprintf("TimeLineChannelUpBtnSlot()\n");
}
/*!channel down button click시 호출 */
void dvr_timelinebar_play::TimeLineChannelDownBtnSlot()
{
    if(runtimesettings.playbacktable.iTimeTablePageChannel<MAX_PAGECHANNEL-1)
        runtimesettings.playbacktable.iTimeTablePageChannel++;
    else
        runtimesettings.playbacktable.iTimeTablePageChannel=0;
    SetTimeTableChannel(runtimesettings.playbacktable.iTimeTablePageChannel);
    dprintf("TimeLineChannelDownBtnSlot()\n");
}
/*!zoom in button click시 호출 */
void dvr_timelinebar_play::TimeLineZoomInBtnSlot()
{
    bool bFindCurrentTime=false;
    if(runtimesettings.playbacktable.iTimeTableZoomRate<PLAYBACKTIMELINE_ZOOM_6)
        runtimesettings.playbacktable.iTimeTableZoomRate=runtimesettings.playbacktable.iTimeTableZoomRate*2;
    else
        return;
    int step = 24/runtimesettings.playbacktable.iTimeTableZoomRate;
    for(int i=0;i<runtimesettings.playbacktable.iTimeTableZoomRate;i++)
    {
        if(step*i<=curtime.playbacktime.tm_hour && step*(i+1)>curtime.playbacktime.tm_hour)
        {
            bFindCurrentTime=true;
            runtimesettings.playbacktable.iTimeTableTimeRate=i;
        }
    }
    if(bFindCurrentTime==false)
        runtimesettings.playbacktable.iTimeTableTimeRate=runtimesettings.playbacktable.iTimeTableTimeRate*2;
    UpdateTimeTable();
    SetTimeTableSelectTime(&curtime.playbacktime);
    dprintf("TimeLineZoomInBtnSlot()\n");
}
/*!zoom out button click시 호출 */
void dvr_timelinebar_play::TimeLineZoomOutBtnSlot()
{
    bool bFindCurrentTime=false;
    if(runtimesettings.playbacktable.iTimeTableZoomRate>PLAYBACKTIMELINE_ZOOM_24)
        runtimesettings.playbacktable.iTimeTableZoomRate=runtimesettings.playbacktable.iTimeTableZoomRate/2;
    else
        return;
    int step = 24/runtimesettings.playbacktable.iTimeTableZoomRate;
    for(int i=0;i<runtimesettings.playbacktable.iTimeTableZoomRate;i++)
    {
        if(step*i<=curtime.playbacktime.tm_hour && step*(i+1)>curtime.playbacktime.tm_hour)
        {
            bFindCurrentTime=true;
            runtimesettings.playbacktable.iTimeTableTimeRate=i;
        }
    }
    if(bFindCurrentTime==false)
        runtimesettings.playbacktable.iTimeTableTimeRate=runtimesettings.playbacktable.iTimeTableTimeRate/2;
    UpdateTimeTable();
    SetTimeTableSelectTime(&curtime.playbacktime);
    dprintf("TimeLineZoomOutBtnSlot()\n");
}
/*!left button click시 호출 */
void dvr_timelinebar_play::TimeLineBandLeftBtnSlot()
{
    if(runtimesettings.playbacktable.iTimeTableTimeRate>0)
        runtimesettings.playbacktable.iTimeTableTimeRate--;
    else
        return;
    UpdateTimeTable();
    pMainClass->m_iPlaybackTimebarUpdateDisableCount=10;
    SetTimeTableSelectTime(&curtime.playbacktime);
    dprintf("TimeLineBandLeftBtnSlot()\n");
}
/*!right button click시 호출 */
void dvr_timelinebar_play::TimeLineBandRightBtnSlot()
{
    if(runtimesettings.playbacktable.iTimeTableTimeRate<runtimesettings.playbacktable.iTimeTableZoomRate-1)
         runtimesettings.playbacktable.iTimeTableTimeRate++;
    else
        return;
    UpdateTimeTable();
    pMainClass->m_iPlaybackTimebarUpdateDisableCount=10;
    SetTimeTableSelectTime(&curtime.playbacktime);
    dprintf("TimeLineBandRightBtnSlot()\n");
}

/*!time table update */
void dvr_timelinebar_play::UpdateTimeTable()
{
	//update timetable 
    int i=0;
    int j=0;

    int colcount;
    for(j=0;j<m_iTimelineRow;j++)
    {
        for(i=0;i<m_iTimelineCol;i++)
        {
              ui->timetable->takeItem(j,i);
        }
    }
    colcount=ui->timetableheader->columnCount();
    for(i=0;i<colcount;i++)
    {
         ui->timetableheader->takeItem(0,i);
    }

    m_iTimelineBand=PLAYBACKTIMELINE_24/runtimesettings.playbacktable.iTimeTableZoomRate;

    for(int icount=0;icount<m_iTimelineBand;icount++)
    {
         char buf[MAX_CHAR_4];
         sprintf(buf,"%d",icount+(m_iTimelineBand*runtimesettings.playbacktable.iTimeTableTimeRate));
         m_tableheaderitem[icount].setText(StrToQString(buf));

         ui->timetableheader->setItem(0,icount*runtimesettings.playbacktable.iTimeTableZoomRate,&m_tableheaderitem[icount]);
    }
    SetTimeTableUpdate(true,false);
}

/*!매분에 대한 recording event 정보를 read */
int dvr_timelinebar_play::GetRecordHourInMinuteDataProfile(struct tm  readtime)
{
	//read record data(hour table)
    int ret=1;
    memset(runtimesettings.playbackinfo.playbacktableinfo.iHourTable,0,sizeof(runtimesettings.playbackinfo.playbacktableinfo.iHourTable));

    for(int i=0;i<MAX_CAMERA;i++)
    {
#if DVR_API_ENABLE
        if(i<MAX_INTERNAL_CAMERA)  
            ret=LIB816x_GetRecHour(i, readtime, runtimesettings.playbackinfo.playbacktableinfo.iHourTable[i]);
#endif
    }
    return ret;
}

/*!recording event에 따라서 timebar에 color 설정 */
void dvr_timelinebar_play::SetTimeTableColRow(int iRow,int iCol,int iEvent)
{
	//set item color by record event
    if((iRow>=ui->timetable->rowCount())||(iCol>=ui->timetable->columnCount())||(iEvent>=MAX_EVENT))
    {
        return;
    }
    ui->timetable->takeItem(iRow,iCol);
    m_tableitem[iRow][iCol].setBackgroundColor(m_itemcolor[iEvent]);
    ui->timetable->setItem(iRow, iCol,&m_tableitem[iRow][iCol]);
}
/*!recording event를 읽어 timetable을 update */
void dvr_timelinebar_play::SetTimeTableUpdate(bool bFullUpdate,bool bReadRecData)
{
    int ret;
    int ColStart=0;
    int ColEnd=PLAYBACKTIMELINEMAX_COL;
    if(!bFullUpdate)
    {
        if((curtime.currenttime.tm_mday != curtime.playbacktime.tm_mday) ||(curtime.currenttime.tm_mon != curtime.playbacktime.tm_mon) || (curtime.currenttime.tm_year != curtime.playbacktime.tm_year))
        {
            ColStart=PLAYBACKTIMELINEMAX_COL-2;
            ColEnd=PLAYBACKTIMELINEMAX_COL;
        }
        else
        {
            int idivide=0;
            if(runtimesettings.playbacktable.iTimeTableZoomRate==PLAYBACKTIMELINE_ZOOM_24)
                 idivide=4;
            else if(runtimesettings.playbacktable.iTimeTableZoomRate==PLAYBACKTIMELINE_ZOOM_12)
                 idivide=2;
            else if(runtimesettings.playbacktable.iTimeTableZoomRate==PLAYBACKTIMELINE_ZOOM_6)
                 idivide=1;
            int icount1=((curtime.currenttime.tm_hour-(m_iTimelineBand*runtimesettings.playbacktable.iTimeTableTimeRate))*60+curtime.currenttime.tm_min)/idivide;
            int icursel=icount1/PLAYBACKTIMELINEMAX_COL;
            if(icursel>1 || icount1<0)
            {
                return;
            }
            icursel=icount1%PLAYBACKTIMELINEMAX_COL;
            if(icount1<PLAYBACKTIMELINEMAX_COL)
                ColStart=icursel-2;
            else
                ColStart=PLAYBACKTIMELINEMAX_COL-2;
            if(ColStart<0)
            {
                if(icount1<PLAYBACKTIMELINEMAX_COL)
                    ColStart=0;
                else
                    ColStart=PLAYBACKTIMELINEMAX_COL-2;
            }
            if(icount1<PLAYBACKTIMELINEMAX_COL && icursel<(PLAYBACKTIMELINEMAX_COL-1))
            {
                ColEnd=icursel;
            }
            else
            {
                ColEnd=PLAYBACKTIMELINEMAX_COL;
            }
        }        
    }
    if(bReadRecData)
        ret=GetRecordHourInMinuteDataProfile(curtime.playbacktime);
    for(int j=0;j<PLAYBACKTIMELINEMAX_ROW;j++)
    {
        for(int i=ColStart;i<ColEnd;i++)
        {
            int ievent=0;
            int icountcol=0;
            int icountrow=0;
            icountrow=(runtimesettings.playbacktable.iTimeTablePageChannel*PLAYBACKTIMELINEMAX_ROW)+j;
            if(runtimesettings.playbacktable.iTimeTableZoomRate==PLAYBACKTIMELINE_ZOOM_24)
            {
                 icountcol=i*4;
                 if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[icountrow][icountcol]!=0)
                     ievent=runtimesettings.playbackinfo.playbacktableinfo.iHourTable[icountrow][icountcol];
                 else if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[icountrow][icountcol+1]!=0)
                     ievent=runtimesettings.playbackinfo.playbacktableinfo.iHourTable[icountrow][icountcol+1];
                 else if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[icountrow][icountcol+2]!=0)
                     ievent=runtimesettings.playbackinfo.playbacktableinfo.iHourTable[icountrow][icountcol+2];
                 else if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[icountrow][icountcol+3]!=0)
                     ievent=runtimesettings.playbackinfo.playbacktableinfo.iHourTable[icountrow][icountcol+3];
            }
            else if(runtimesettings.playbacktable.iTimeTableZoomRate==PLAYBACKTIMELINE_ZOOM_12)
            {
                icountcol=(runtimesettings.playbacktable.iTimeTableTimeRate*60*12)+(i*2);
                if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[icountrow][icountcol]!=0)
                    ievent=runtimesettings.playbackinfo.playbacktableinfo.iHourTable[icountrow][icountcol];
                else if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[icountrow][icountcol+1]!=0)
                    ievent=runtimesettings.playbackinfo.playbacktableinfo.iHourTable[icountrow][icountcol+1];
            }
            else if(runtimesettings.playbacktable.iTimeTableZoomRate==PLAYBACKTIMELINE_ZOOM_6)
            {
                icountcol=(runtimesettings.playbacktable.iTimeTableTimeRate*60*6)+i;
                ievent=runtimesettings.playbackinfo.playbacktableinfo.iHourTable[icountrow][icountcol];
            }
            SetTimeTableColRow(j,i,ievent);
        }
    }
}

/*!time table의 channel을 변경 */
void dvr_timelinebar_play::SetTimeTableChannel(int iPageChannel)
{
    if((iPageChannel<0)||(iPageChannel>=MAX_PAGECHANNEL))
    {
        return;
    }
    //change channel
    int offset=iPageChannel*PLAYBACKTIMELINEMAX_ROW;
    for(int i=0;i<PLAYBACKTIMELINEMAX_ROW;i++)
    {
        int idx = i+offset;
        m_iChannelName[i]->setText(StrToQString(profile.camera[idx].strcameraname));
    }
    //update time table
    SetTimeTableUpdate(true,false);
}
/*!time table의 현재 item 변경이 있을때 호출 */
void dvr_timelinebar_play::ItemSelectionChangedSlot()
{
    if(runtimesettings.playbackinfo.playbackmode.iPlaybackMode!=PLAYBACKMODE_PLAY)
    {
        GetTimeTableSelectTime(&curtime.playbacktime);
    }
}

/*!time table에서 현재 선택된 time을 읽어옴 */
void dvr_timelinebar_play::GetTimeTableSelectTime(struct tm * ptm)
{
	//get select time
    int ifound=0;
    int icount=0;
    int icursel=0;
    QList <QTableWidgetItem *> tablelist;
    QTableWidgetItem *selectedtableitem=NULL;
    tablelist=ui->timetable->selectedItems();
    if(tablelist.count())
        selectedtableitem=tablelist.takeFirst();
    for(int i=0;i<PLAYBACKTIMELINEMAX_ROW;i++)
    {
        icount=0;
        for(int j=0;j<PLAYBACKTIMELINEMAX_COL;j++)
        {
            if(&m_tableitem[i][j]==selectedtableitem)
            {
                ifound=1;
                break;
            }
            else
                icount++;
        }
        if(ifound==1)
            break;
    }
    icursel=(icount+(runtimesettings.playbacktable.iTimeTableTimeRate*PLAYBACKTIMELINEMAX_COL))*(4/runtimesettings.playbacktable.iTimeTableZoomRate);
    cursorPosition=icursel%PLAYBACKTIMELINEMAX_COL;
    ptm->tm_hour=icursel/60;
    ptm->tm_min=icursel%60;

}
/*!입력 time,zoom rate등에 따라 현재 timetable의 선택 item을 update */
void dvr_timelinebar_play::SetTimeTableSelectTime(struct tm * ptm)
{
	//update current position 
    int icount=0;
    int icursel=0;
    int idivide=0;
    if(runtimesettings.playbacktable.iTimeTableZoomRate==PLAYBACKTIMELINE_ZOOM_24)
        idivide=4;
    else if(runtimesettings.playbacktable.iTimeTableZoomRate==PLAYBACKTIMELINE_ZOOM_12)
        idivide=2;
    else if(runtimesettings.playbacktable.iTimeTableZoomRate==PLAYBACKTIMELINE_ZOOM_6)
        idivide=1;
    icount=(ptm->tm_hour*60+ptm->tm_min)/idivide;
    icursel=icount%PLAYBACKTIMELINEMAX_COL;
    if(cursorPosition!=icursel)
    {
        ui->timetable->selectColumn(icursel);
        cursorPosition=icursel;
    }
    if(runtimesettings.playbackinfo.playbackmode.iPlaybackMode==PLAYBACKMODE_STOP)
        GetTimeTableSelectTime(ptm);
}
/*!입력 time으로 현재 timetable의 선택 item,time band를 update(현재 timeband 내에서) */
void dvr_timelinebar_play::SetTimeTableSelectTimeMarking(struct tm * ptm)
{
	//update current position
    int icount1=0;
    int icursel=0;
    int idivide=0;
    if(runtimesettings.playbacktable.iTimeTableZoomRate==PLAYBACKTIMELINE_ZOOM_24)
        idivide=4;
    else if(runtimesettings.playbacktable.iTimeTableZoomRate==PLAYBACKTIMELINE_ZOOM_12)
        idivide=2;
    else if(runtimesettings.playbacktable.iTimeTableZoomRate==PLAYBACKTIMELINE_ZOOM_6)
        idivide=1;
    icount1=(ptm->tm_hour*60+ptm->tm_min)/idivide;
    icursel=icount1%PLAYBACKTIMELINEMAX_COL;
    if(runtimesettings.playbacktable.iTimeTableTimeRate!=(ptm->tm_hour*60+ptm->tm_min)*runtimesettings.playbacktable.iTimeTableZoomRate/(24*60))
    {
        runtimesettings.playbacktable.iTimeTableTimeRate=(ptm->tm_hour*60+ptm->tm_min)*runtimesettings.playbacktable.iTimeTableZoomRate/(24*60);
        UpdateTimeTable();
    }
    if(cursorPosition!=icursel)
    {
        ui->timetable->selectColumn(icursel);
        cursorPosition=icursel;
    }
}
/*!playback start시 record data가 존재하는 next time을 얻어옴 */
void dvr_timelinebar_play::GetNextRecordDataTime(struct tm* ptm)
{
	//search next record time for playback start
    int idx;
    bool bFind=false;
    idx = ptm->tm_hour*60 + ptm->tm_min;
    for(int i=0;i<16;i++)
    {
        if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[i][idx])
        {
            bFind = true;
            break;
        }
    }
    if(bFind == false)
    {
        for(int i=0; i<16; i++)
        {
            for(int j=idx;j<PLAYBACKDAYMINUTECOUNT;j++)
            {
                if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[i][j])
                {
                    bFind = true;
                    idx = j;
                    break;
                }
            }
            if(bFind)
            {
                break;
            }
        }
    }
    ptm->tm_hour = idx / 60;
    ptm->tm_min = idx % 60;
    dprintf("bFind=%d hour = %d min=%d\n",bFind,ptm->tm_hour,ptm->tm_min);
}

/*!time table의 items을 click할경우 호출 (jump동작 수행) */
void dvr_timelinebar_play::on_timetable_itemClicked(QTableWidgetItem* item)
{
	//playback jump when timetable clicked
    if(runtimesettings.playbackinfo.playbackmode.iPlaybackMode==PLAYBACKMODE_PLAY)
    {
        int ret=0;
        GetTimeTableSelectTime(&curtime.playbacktime);
#if DVR_API_ENABLE
        ret=LIB816x_jumpPlayback_x(0xffff, &curtime.playbacktime);
        if (LIB_PB_NO_ERR != ret)
        {
            eprintf("jump fail %02d:%02d -ret:%d\n",curtime.playbacktime.tm_hour,curtime.playbacktime.tm_min,ret);
        }
#endif
    }
}
