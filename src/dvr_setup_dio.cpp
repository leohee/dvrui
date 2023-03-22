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
/*!dio tab의 control들을 초기화 */
void dvr_setup::InitSetupDioControls()
{
    int i;
    m_iSensorSelectId=0;
    m_iAlarmSelectId=0;

    QSize qsize;
    qsize.setWidth(SENSORTREEICON_SIZE_W);
    qsize.setHeight(SENSORTREEICON_SIZE_H);
    SensorTreeItemIcon.addFile(QString::fromUtf8(SENSOR_TREE_CAMERA_ICON), qsize, QIcon::Normal, QIcon::Off);
    for(i=0;i<MAX_SENSOR;i++)
    {
        char strsensorname[MAX_TITLE_CHAR];
        sprintf(strsensorname,"%s%d",STRSENSORNAME,i+1);
        SetupSensorTreeItem[i].setIcon(0,SensorTreeItemIcon);
        SetupSensorTreeItem[i].setText(0,QString::fromUtf8(strsensorname));
        ui->sensortreeWidget->insertTopLevelItem(i,&SetupSensorTreeItem[i]);
    }
    connect(ui->sensortreeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(SetupSensorTreeItemClicked(QTreeWidgetItem *, int)));
    connect(ui->sensortreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(SetupSensorTreeItemDoubleClicked(QTreeWidgetItem *, int)));

    qsize.setWidth(ALARMTREEICON_SIZE_W);
    qsize.setHeight(ALARMTREEICON_SIZE_H);
    AlarmTreeItemIcon.addFile(QString::fromUtf8(ALARM_TREE_CAMERA_ICON), qsize, QIcon::Normal, QIcon::Off);
    for(i=0;i<MAX_ALARM;i++)
    {
        char stralarmname[MAX_TITLE_CHAR];
        sprintf(stralarmname,"%s%d",STRALARMNAME,i+1);
        SetupAlarmTreeItem[i].setIcon(0,AlarmTreeItemIcon);
        SetupAlarmTreeItem[i].setText(0,QString::fromUtf8(stralarmname));
        ui->alarmtreeWidget->insertTopLevelItem(i,&SetupAlarmTreeItem[i]);
    }
    connect(ui->alarmtreeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(SetupAlarmTreeItemClicked(QTreeWidgetItem *, int)));
    connect(ui->alarmtreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(SetupAlarmTreeItemDoubleClicked(QTreeWidgetItem *, int)));

    connect(ui->alarmdelaytimeslider, SIGNAL(valueChanged(int)), this, SLOT(ChangeAlarmDelayTimeSlot(int)));
}
/*!profile data struct에 따라서 ui에 value setting */
void dvr_setup::SetDiodata()
{
    SetSetupSensorTreeControl();
    SetSetupAlarmTreeControl();
    SetSetupSensorButtonControls(m_iSensorSelectId);
    SetSetupAlarmButtonControls(m_iAlarmSelectId);
}
/*!ui control의 value를 읽고 해당 기능을 적용시킴 */
void dvr_setup::GetDiodata()
{
    int i=0;

    GetSensorItemdataToDlgData(m_iSensorSelectId);
    GetAlarmItemdataToDlgData(m_iAlarmSelectId);

    for(i=0;i<MAX_SENSOR;i++)
    {
        if(dlgdata.sensor[i].iSensorOn!=profile.sensor[i].iSensorOn
           || dlgdata.sensor[i].iSensorType!=profile.sensor[i].iSensorType)
        {
            profile.sensor[i].iSensorOn=dlgdata.sensor[i].iSensorOn;
            profile.sensor[i].iSensorType=dlgdata.sensor[i].iSensorType;
#if DVR_API_ENABLE
            LIB816x_setSensor(i,profile.sensor[i].iSensorOn,profile.sensor[i].iSensorType);
#endif
        }
    }

    for(i=0;i<MAX_ALARM;i++)
    {
        if(dlgdata.alarm[i].iAlarmOn!=profile.alarm[i].iAlarmOn
           || dlgdata.alarm[i].iAlarmType!=profile.alarm[i].iAlarmType
           || dlgdata.alarm[i].iDelayTime!=profile.alarm[i].iDelayTime)
        {
            profile.alarm[i].iAlarmOn=dlgdata.alarm[i].iAlarmOn;
            profile.alarm[i].iAlarmType=dlgdata.alarm[i].iAlarmType;
            profile.alarm[i].iDelayTime=dlgdata.alarm[i].iDelayTime;
#if DVR_API_ENABLE
            LIB816x_setAlarm(i,profile.alarm[i].iAlarmOn,profile.alarm[i].iAlarmType,profile.alarm[i].iDelayTime);
#endif
        }
    }
}
/*!cancel시 dio관련 변경적용된 내역을 복원시킴 */
void dvr_setup::ResotreDio()
{
    int i=0;
    for(i=0;i<MAX_SENSOR;i++)
    {
        if(dlgdata.sensor[i].iSensorOn!=profile.sensor[i].iSensorOn
           || dlgdata.sensor[i].iSensorType!=profile.sensor[i].iSensorType)
        {
            profile.sensor[i].iSensorOn=dlgdata.sensor[i].iSensorOn;
            profile.sensor[i].iSensorType=dlgdata.sensor[i].iSensorType;
#if DVR_API_ENABLE
            LIB816x_setSensor(i,profile.sensor[i].iSensorOn,profile.sensor[i].iSensorType);
#endif
        }
    }

    for(i=0;i<MAX_ALARM;i++)
    {
        if(dlgdata.alarm[i].iAlarmOn!=profile.alarm[i].iAlarmOn
           || dlgdata.alarm[i].iAlarmType!=profile.alarm[i].iAlarmType
           || dlgdata.alarm[i].iDelayTime!=profile.alarm[i].iDelayTime)
        {
            profile.alarm[i].iAlarmOn=dlgdata.alarm[i].iAlarmOn;
            profile.alarm[i].iAlarmType=dlgdata.alarm[i].iAlarmType;
            profile.alarm[i].iDelayTime=dlgdata.alarm[i].iDelayTime;
#if DVR_API_ENABLE
            LIB816x_setAlarm(i,profile.alarm[i].iAlarmOn,profile.alarm[i].iAlarmType,profile.alarm[i].iDelayTime);
#endif
        }
    }
}
/*!sensor tree에 현재 선택item을 set */
void dvr_setup::SetSetupSensorTreeControl()
{
    ui->sensortreeWidget->setCurrentItem (&SetupSensorTreeItem[m_iSensorSelectId]);
}
/*!현재 선택된 sensor에 대해서 ui에 값을 set */
void dvr_setup::SetSetupSensorButtonControls(int iSensorId)
{
    ui->sensorenable->setCurrentIndex(dlgdata.sensor[iSensorId].iSensorOn);
    ui->sensortypecomboBox->setCurrentIndex(dlgdata.sensor[iSensorId].iSensorType);
}
/*!alarm tree에 현재 선택된 item을 set */
void dvr_setup::SetSetupAlarmTreeControl()
{
    ui->alarmtreeWidget->setCurrentItem (&SetupAlarmTreeItem[m_iAlarmSelectId]);
}
/*!현재 선택된 alarm에 대해서 ui에 값을 set */
void dvr_setup::SetSetupAlarmButtonControls(int iAlarmId)
{
    ui->alarmenable->setCurrentIndex(dlgdata.alarm[iAlarmId].iAlarmOn);
    ui->alarmtypecomboBox->setCurrentIndex(dlgdata.alarm[iAlarmId].iAlarmType);
    ui->alarmdelaytimeslider->setSliderPosition(dlgdata.alarm[iAlarmId].iDelayTime);
    ui->alarmdelaytimevalue->setText(SetAlarmDelayTimeValueText(dlgdata.alarm[iAlarmId].iDelayTime));
}
/*!alram delay time을 text로 표시 */
QString dvr_setup::SetAlarmDelayTimeValueText(int ivalue)
{
    QString qstrvalue;
    char alarmdelaytimebuf[MAX_CHAR_8];
    sprintf(alarmdelaytimebuf,"%d %s",ivalue,STRSECOND);
    qstrvalue=StrToQString(alarmdelaytimebuf);

    return qstrvalue;
}
/*!ui로부터 현재 선택된 sensor의 data를 load */
void dvr_setup::GetSensorItemdataToDlgData(int iSensorId)
{
    dlgdata.sensor[iSensorId].iSensorOn=ui->sensorenable->currentIndex();
    dlgdata.sensor[iSensorId].iSensorType=ui->sensortypecomboBox->currentIndex();
}
/*!ui로부터 현재 선택된 alarm의 data를 load */
void dvr_setup::GetAlarmItemdataToDlgData(int iAlarmId)
{
    dlgdata.alarm[iAlarmId].iAlarmOn=ui->alarmenable->currentIndex();
    dlgdata.alarm[iAlarmId].iAlarmType=ui->alarmtypecomboBox->currentIndex();
    dlgdata.alarm[iAlarmId].iDelayTime=ui->alarmdelaytimeslider->sliderPosition();
}
/*!sensor tree가 click될때 호출 */
void dvr_setup::SetupSensorTreeItemClicked(QTreeWidgetItem *item, int column)
{
	//현재 sensor에 ui data를 저장하고 변경하는 sensor의 data로 ui set
    GetSensorItemdataToDlgData(m_iSensorSelectId);
    m_iSensorSelectId=ui->sensortreeWidget->indexOfTopLevelItem(item);
    SetSetupSensorButtonControls(m_iSensorSelectId);
}
/*!sensor tree가 double click될때 호출 */
void dvr_setup::SetupSensorTreeItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    SetupSensorTreeItemClicked(item,column);
}
/*!alram tree가 click될때 호출 */
void dvr_setup::SetupAlarmTreeItemClicked(QTreeWidgetItem *item, int column)
{
	//현재 alarm에 ui data를 저장하고 변경하는 alarm의 data로 ui set
    GetAlarmItemdataToDlgData(m_iAlarmSelectId);
    m_iAlarmSelectId=ui->alarmtreeWidget->indexOfTopLevelItem(item);
    SetSetupAlarmButtonControls(m_iAlarmSelectId);
}
/*!alram tree가 double click될때 호출 */
void dvr_setup::SetupAlarmTreeItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    SetupAlarmTreeItemClicked(item,column);
}
/*!alarm delay time slide가 변경될때 호출 */
void dvr_setup::ChangeAlarmDelayTimeSlot(int value)
{
     ui->alarmdelaytimevalue->setText(SetAlarmDelayTimeValueText(value));
}
