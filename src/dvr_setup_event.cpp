/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include "dvr_defines.h"
#include "dvr_setup.h"
#include "ui_dvr_setup.h"
#include "dvr_main.h"
/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 local functions
-----------------------------------------------------------------------------*/
/*!event tab의 control들을 초기화 */
void dvr_setup::InitSetupEventControls()
{
    m_iEventRecordType=EVENTLINK_MOTION;
    m_iEventAlarmType=EVENTLINK_MOTION;

    connect(ui->eventrecordtypecomboBox, SIGNAL(currentIndexChanged (int)), this, SLOT(ChangeEventRecordTypeModeSlot(int)));
    connect(ui->eventalarmtypecomboBox, SIGNAL(currentIndexChanged (int)), this, SLOT(ChangeEventAlarmTypeModeSlot(int)));
    connect(ui->eventrecordtableWidget, SIGNAL(cellPressed (int,int)), this, SLOT(EventRecordTableSelectSlot(int,int)));
    connect(ui->eventalarmtableWidget, SIGNAL(cellPressed (int,int)), this, SLOT(EventAlarmTableSelectSlot(int,int)));

    ui->eventrecordrecordheadertableWidget->setRowCount(1);
    ui->eventrecordrecordheadertableWidget->setColumnCount(MAX_INTERNAL_CAMERA);
    int icellwidth=ui->eventrecordrecordheadertableWidget->width()/ui->eventrecordrecordheadertableWidget->columnCount();

    for(int i=0;i<ui->eventrecordrecordheadertableWidget->columnCount();i++)
    {
        ui->eventrecordrecordheadertableWidget->setColumnWidth(i,icellwidth);

        char buf[MAX_CHAR_8];
        sprintf(buf,"%d",i+1);
        m_eventrecordrecordheaderitem[i].setText(StrToQString(buf));
        m_eventrecordrecordheaderitem[i].setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->eventrecordrecordheadertableWidget->setItem(0,i,&m_eventrecordrecordheaderitem[i]);
    }
    ui->eventalarmalarmheadertableWidget->setRowCount(1);
    ui->eventalarmalarmheadertableWidget->setColumnCount(MAX_ALARM);
    icellwidth=ui->eventalarmalarmheadertableWidget->width()/ui->eventalarmalarmheadertableWidget->columnCount();

    for(int i=0;i<ui->eventalarmalarmheadertableWidget->columnCount();i++)
    {
        ui->eventalarmalarmheadertableWidget->setColumnWidth(i,icellwidth);

        char buf[MAX_CHAR_8];
        sprintf(buf,"%s%d",STREVENTLINK_ALARM,i+1);
        m_eventalarmalarmheaderitem[i].setText(StrToQString(buf));
        m_eventalarmalarmheaderitem[i].setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->eventalarmalarmheadertableWidget->setItem(0,i,&m_eventalarmalarmheaderitem[i]);
    }
}
/*!profile data struct에 따라서 ui에 value setting */
void dvr_setup::SetEventdata()
{
    SetEventRecordTable(m_iEventRecordType);
    SetEventAlarmTable(m_iEventAlarmType);
}
/*!ui control의 value를 읽고 해당 기능을 적용시킴 */
void dvr_setup::GetEventdata()
{
    memcpy(&profile.eventlink,&dlgdata.eventlink,sizeof(EVENTLINK));
}
/*!cancel시 event관련 변경적용된 내역을 복원시킴 */
void dvr_setup::ResotreEvent()
{
    memcpy(&profile.eventlink,&dlgdata.eventlink,sizeof(EVENTLINK));
}
/*!event type에 따라 ui record link table에 setting함 */
void dvr_setup::SetEventRecordTable(int iEventType)
{
    int i=0;
    int j=0;
    int icellwidth=0;
    int icellheight=0;
    int iRowcount=0;
    char bufevent[MAX_CHAR_8];


    memset(bufevent,0,sizeof(bufevent));

    for(i=0;i<MAX_INTERNAL_CAMERA;i++)
    {
        memset(iTempLinkRecord[i],0,sizeof(iTempLinkRecord[i]));
    }

    switch(iEventType)
    {
    case EVENTLINK_MOTION:
        iRowcount=MAX_INTERNAL_CAMERA;
        sprintf(bufevent,"%s",STREVENTLINK_MOTION);
        for(j=0;j<iRowcount;j++)
        {
            for(i=0;i<MAX_INTERNAL_CAMERA;i++)
            {
                iTempLinkRecord[j][i]=dlgdata.eventlink.iMotionRecordingLink[j][i];
            }
        }
        break;
    case EVENTLINK_VIDEOLOSS:
        iRowcount=MAX_INTERNAL_CAMERA;
        sprintf(bufevent,"%s",STREVENTLINK_VIDEOLOSS);
        for(j=0;j<iRowcount;j++)
        {
            for(i=0;i<MAX_INTERNAL_CAMERA;i++)
            {
                iTempLinkRecord[j][i]=dlgdata.eventlink.iVideoLossRecordingLink[j][i];
            }
        }
        break;
    case EVENTLINK_SENSOR:
        iRowcount=MAX_SENSOR;
        sprintf(bufevent,"%s",STREVENTLINK_SENSOR);
        for(j=0;j<iRowcount;j++)
        {
            for(i=0;i<MAX_INTERNAL_CAMERA;i++)
            {
                iTempLinkRecord[j][i]=dlgdata.eventlink.iSensorRecordingLink[j][i];
            }
        }
        break;
    default:
        iRowcount=MAX_INTERNAL_CAMERA;
        break;
    }

    for(i=0;i<ui->eventrecordeventheadertableWidget->rowCount();i++)
    {
        ui->eventrecordeventheadertableWidget->takeItem(i,0);
    }
    ui->eventrecordeventheadertableWidget->setRowCount(iRowcount);
    ui->eventrecordeventheadertableWidget->setColumnCount(1);
    icellheight=ui->eventrecordeventheadertableWidget->height()/ui->eventrecordeventheadertableWidget->rowCount();

    for(i=0;i<ui->eventrecordeventheadertableWidget->rowCount();i++)
    {
        ui->eventrecordeventheadertableWidget->setRowHeight(i,icellheight);

        char buf[MAX_CHAR_10];
        sprintf(buf,"%s%d",bufevent,i+1);
        m_eventrecordeventheaderitem[i].setText(StrToQString(buf));
        m_eventrecordeventheaderitem[i].setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->eventrecordeventheadertableWidget->setItem(i,0,&m_eventrecordeventheaderitem[i]);
    }

    for(j=0;j<ui->eventrecordtableWidget->rowCount();j++)
    {
        for(i=0;i<ui->eventrecordtableWidget->columnCount();i++)
        {
              ui->eventrecordtableWidget->takeItem(j,i);
        }
    }

    ui->eventrecordtableWidget->setRowCount(iRowcount);
    ui->eventrecordtableWidget->setColumnCount(MAX_INTERNAL_CAMERA);
    icellwidth=ui->eventrecordtableWidget->width()/ui->eventrecordtableWidget->columnCount();
    icellheight=ui->eventrecordtableWidget->height()/ui->eventrecordtableWidget->rowCount();


    for(i=0;i<ui->eventrecordtableWidget->columnCount();i++)
    {
        ui->eventrecordtableWidget->setColumnWidth(i,icellwidth);
    }
    for(i=0;i<ui->eventrecordtableWidget->rowCount();i++)
    {
        ui->eventrecordtableWidget->setRowHeight(i,icellheight);
    }

    for(j=0;j<ui->eventrecordtableWidget->rowCount();j++)
    {
        for(i=0;i<ui->eventrecordtableWidget->columnCount();i++)
        {

            if(iTempLinkRecord[j][i]==0)
            {
                m_eventrecorditem[j][i].setText(StrToQString((char*)STREVENTLINK_OFF));
                m_eventrecorditem[j][i].setTextColor(Qt::cyan);
            }
            else
            {
                m_eventrecorditem[j][i].setText(StrToQString((char*)STREVENTLINK_ON));
                m_eventrecorditem[j][i].setTextColor(Qt::yellow);
            }
            m_eventrecorditem[j][i].setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            ui->eventrecordtableWidget->setItem(j, i,&m_eventrecorditem[j][i]);
        }
    }
}
/*!event type에 따라 ui alarm link table에 setting함 */
void dvr_setup::SetEventAlarmTable(int iEventType)
{
    int i=0;
    int j=0;
    int icellwidth=0;
    int icellheight=0;
    int iRowcount=0;
    char bufevent[MAX_CHAR_8];

    memset(bufevent,0,sizeof(bufevent));

    for(i=0;i<MAX_INTERNAL_CAMERA;i++)
    {
        memset(iTempLinkAlarm[i],0,sizeof(iTempLinkAlarm[i]));
    }

    switch(iEventType)
    {
    case EVENTLINK_MOTION:
        iRowcount=MAX_INTERNAL_CAMERA;
        sprintf(bufevent,"%s",STREVENTLINK_MOTION);
        for(j=0;j<iRowcount;j++)
        {
            for(i=0;i<MAX_ALARM;i++)
            {
                iTempLinkAlarm[j][i]=dlgdata.eventlink.iMotionAlarmLink[j][i];
            }
        }
        break;
    case EVENTLINK_VIDEOLOSS:
        iRowcount=MAX_INTERNAL_CAMERA;
        sprintf(bufevent,"%s",STREVENTLINK_VIDEOLOSS);
        for(j=0;j<iRowcount;j++)
        {
            for(i=0;i<MAX_ALARM;i++)
            {
                iTempLinkAlarm[j][i]=dlgdata.eventlink.iVideoLossAlarmLink[j][i];
            }
        }
        break;
    case EVENTLINK_SENSOR:
        iRowcount=MAX_SENSOR;
        sprintf(bufevent,"%s",STREVENTLINK_SENSOR);
        for(j=0;j<iRowcount;j++)
        {
            for(i=0;i<MAX_ALARM;i++)
            {
                iTempLinkAlarm[j][i]=dlgdata.eventlink.iSensorAlarmLink[j][i];
            }
        }
        break;
    default:
        iRowcount=MAX_INTERNAL_CAMERA;
        break;
    }

    for(i=0;i<ui->eventalarmeventheadertableWidget->rowCount();i++)
    {
        ui->eventalarmeventheadertableWidget->takeItem(i,0);
    }

    ui->eventalarmeventheadertableWidget->setRowCount(iRowcount);
    ui->eventalarmeventheadertableWidget->setColumnCount(1);
    icellheight=ui->eventalarmeventheadertableWidget->height()/ui->eventalarmeventheadertableWidget->rowCount();

    for(i=0;i<ui->eventalarmeventheadertableWidget->rowCount();i++)
    {
        ui->eventalarmeventheadertableWidget->setRowHeight(i,icellheight);

        char buf[MAX_CHAR_10];
        sprintf(buf,"%s%d",bufevent,i+1);
        m_eventalarmeventheaderitem[i].setText(StrToQString(buf));
        m_eventalarmeventheaderitem[i].setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->eventalarmeventheadertableWidget->setItem(i,0,&m_eventalarmeventheaderitem[i]);
    }

    for(j=0;j<ui->eventalarmtableWidget->rowCount();j++)
    {
        for(i=0;i<ui->eventalarmtableWidget->columnCount();i++)
        {
              ui->eventalarmtableWidget->takeItem(j,i);
        }
    }

    ui->eventalarmtableWidget->setRowCount(iRowcount);
    ui->eventalarmtableWidget->setColumnCount(MAX_ALARM);

    icellwidth=ui->eventalarmtableWidget->width()/ui->eventalarmtableWidget->columnCount();
    icellheight=ui->eventalarmtableWidget->height()/ui->eventalarmtableWidget->rowCount();


    for(i=0;i<ui->eventalarmtableWidget->columnCount();i++)
    {
        ui->eventalarmtableWidget->setColumnWidth(i,icellwidth);
    }
    for(i=0;i<ui->eventalarmtableWidget->rowCount();i++)
    {
        ui->eventalarmtableWidget->setRowHeight(i,icellheight);
    }
    for(j=0;j<ui->eventalarmtableWidget->rowCount();j++)
    {
        for(i=0;i<ui->eventalarmtableWidget->columnCount();i++)
        {

            if(iTempLinkAlarm[j][i]==0)
            {
                m_eventalarmitem[j][i].setText(StrToQString((char*)STREVENTLINK_OFF));
                m_eventalarmitem[j][i].setTextColor(Qt::cyan);
            }
            else
            {
                m_eventalarmitem[j][i].setText(StrToQString((char*)STREVENTLINK_ON));
                m_eventalarmitem[j][i].setTextColor(Qt::yellow);
            }
            m_eventalarmitem[j][i].setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            ui->eventalarmtableWidget->setItem(j, i,&m_eventalarmitem[j][i]);
        }
    }
}
/*!event-record link의 event type combo box가 변경될 때 호출 */
void dvr_setup::ChangeEventRecordTypeModeSlot(int index)
{
    m_iEventRecordType=index;
    SetEventRecordTable(m_iEventRecordType);
}
/*!event-alram link의 event type combo box가 변경될 때 호출 */
void dvr_setup::ChangeEventAlarmTypeModeSlot(int index)
{
    m_iEventAlarmType=index;
    SetEventAlarmTable(m_iEventAlarmType);
}
/*!event-record link의 table이 변경될 때 호출 */
void dvr_setup::EventRecordTableSelectSlot ( int row, int column )
{
    dprintf("EventRecordTableSelectSlot col=%d,r=%d\n",column,row);
    ui->eventrecordtableWidget->takeItem(row,column);

    if(iTempLinkRecord[row][column]==0)
        iTempLinkRecord[row][column]=1;
    else
        iTempLinkRecord[row][column]=0;

    if(iTempLinkRecord[row][column]==0)
    {
        m_eventrecorditem[row][column].setText(StrToQString((char*)STREVENTLINK_OFF));
        m_eventrecorditem[row][column].setTextColor(Qt::cyan);
    }
    else
    {
        m_eventrecorditem[row][column].setText(StrToQString((char*)STREVENTLINK_ON));
        m_eventrecorditem[row][column].setTextColor(Qt::yellow);
    }
    m_eventrecorditem[row][column].setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->eventrecordtableWidget->setItem(row, column,&m_eventrecorditem[row][column]);

    switch(m_iEventRecordType)
    {
    case EVENTLINK_MOTION:
        dlgdata.eventlink.iMotionRecordingLink[row][column]=iTempLinkRecord[row][column];
        break;
    case EVENTLINK_VIDEOLOSS:
       dlgdata.eventlink.iVideoLossRecordingLink[row][column]=iTempLinkRecord[row][column];
        break;
    case EVENTLINK_SENSOR:
        dlgdata.eventlink.iSensorRecordingLink[row][column]=iTempLinkRecord[row][column];
        break;
    default:
        break;
    }
}
/*!event-alram link의 table이 변경될 때 호출 */
void dvr_setup::EventAlarmTableSelectSlot ( int row, int column )
{
    dprintf("EventRecordTableSelectSlot col=%d,r=%d\n",column,row);
    ui->eventalarmtableWidget->takeItem(row,column);

    if(iTempLinkAlarm[row][column]==0)
        iTempLinkAlarm[row][column]=1;
    else
        iTempLinkAlarm[row][column]=0;

    if(iTempLinkAlarm[row][column]==0)
    {
        m_eventalarmitem[row][column].setText(StrToQString((char*)STREVENTLINK_OFF));
        m_eventalarmitem[row][column].setTextColor(Qt::cyan);
    }
    else
    {
        m_eventalarmitem[row][column].setText(StrToQString((char*)STREVENTLINK_ON));
        m_eventalarmitem[row][column].setTextColor(Qt::yellow);
    }
    m_eventalarmitem[row][column].setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->eventalarmtableWidget->setItem(row, column,&m_eventalarmitem[row][column]);

    switch(m_iEventAlarmType)
    {
    case EVENTLINK_MOTION:
        dlgdata.eventlink.iMotionAlarmLink[row][column]=iTempLinkAlarm[row][column];
        break;
    case EVENTLINK_VIDEOLOSS:
       dlgdata.eventlink.iVideoLossAlarmLink[row][column]=iTempLinkAlarm[row][column];
        break;
    case EVENTLINK_SENSOR:
        dlgdata.eventlink.iSensorAlarmLink[row][column]=iTempLinkAlarm[row][column];
        break;
    default:
        break;
    }
}
