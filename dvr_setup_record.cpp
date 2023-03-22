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
extern dvrmain *pMainClass;
/*----------------------------------------------------------------------------
 local functions
-----------------------------------------------------------------------------*/
/*!record tab의 control들을 초기화 */
void dvr_setup::InitSetupRecordControls()
{
    m_iRecordSelectId=0;
    m_iRecordEventTypeMode=0;
    m_RecordScheduleBlockColor[REC_SCHEDULE_CONTINUOUSRECORD]=Qt::red;
    m_RecordScheduleBlockColor[REC_SCHEDULE_RECORDBYMOTION]=Qt::yellow;
    m_RecordScheduleBlockColor[REC_SCHEDULE_RECORDBYSENSOR]=Qt::blue;
    m_RecordScheduleBlockColor[REC_SCHEDULE_RECORDBYVLOSS]=Qt::cyan;//white;
    m_RecordScheduleBlockColor[REC_SCHEDULE_NORECORD]=Qt::lightGray;

    QSize qsize;
    qsize.setWidth(CAMERATREEICON_SIZE_W);
    qsize.setHeight(CAMERATREEICON_SIZE_H);
    RecordTreeItemIcon.addFile(QString::fromUtf8(CAMERA_TREE_CAMERA_ICON), qsize, QIcon::Normal, QIcon::Off);

    ui->recordscheduleheadertableWidget->setRowCount(1);
    ui->recordscheduleheadertableWidget->setColumnCount(MAX_HOUR);
    int icellwidth=ui->recordscheduleheadertableWidget->width()/ui->recordscheduleheadertableWidget->columnCount();

    for(int i=0;i<ui->recordscheduleheadertableWidget->columnCount();i++)
    {
        ui->recordscheduleheadertableWidget->setColumnWidth(i,icellwidth);

        char buf[MAX_CHAR_4];
        sprintf(buf,"%d",i);
        m_recordscheduleheaderitem[i].setText(StrToQString(buf));
        m_recordscheduleheaderitem[i].setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->recordscheduleheadertableWidget->setItem(0,i,&m_recordscheduleheaderitem[i]);
    }
    connect(ui->recordtreeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(SetupRecordTreeItemClicked(QTreeWidgetItem *, int)));
    connect(ui->recordtreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(SetupRecordTreeItemDoubleClicked(QTreeWidgetItem *, int)));

    connect(ui->recordframerateslider, SIGNAL(valueChanged(int)), this, SLOT(ChangeRecordframerateSlot(int)));
    connect(ui->recordbitrateslider, SIGNAL(valueChanged(int)), this, SLOT(ChangeRecordbitrateSlot(int)));
    connect(ui->recordscheduleeventtypecomboBox, SIGNAL(currentIndexChanged (int)), this, SLOT(ChangeRecordscheduleEventTypeModeSlot(int)));
    connect(ui->recordscheduletableWidget, SIGNAL(cellPressed (int,int)), this, SLOT(RecordScheduleTableSelectSlot(int,int)));
    connect(ui->recordscheduletableWidget, SIGNAL(cellEntered (int,int)), this, SLOT(RecordScheduleTableSelectSlot(int,int)));
    connect(ui->recordscheduleapplyallpushButton, SIGNAL(clicked()), this, SLOT(RecordScheduleApplyAllBtnSlot()));
    connect(ui->recordeventprevdurationslider, SIGNAL(valueChanged(int)), this, SLOT(ChangePrevRecordDurationSlot(int)));
    connect(ui->recordeventpostdurationslider, SIGNAL(valueChanged(int)), this, SLOT(ChangePostRecordDurationSlot(int)));
}
/*!profile data struct에 따라서 ui에 value setting */
void dvr_setup::SetRecorddata()
{
    SetSetupRecordTreeControl();
    SetSetupRecordButtonControls(m_iRecordSelectId);
}
/*!ui control의 value를 읽고 해당 기능을 적용시킴 */
void dvr_setup::GetRecorddata()
{
    int aVal[MAX_INTERNAL_CAMERA];
    int aFRVal[MAX_INTERNAL_CAMERA];
    int aIFVal[MAX_INTERNAL_CAMERA];
    int ch=0;
    int frch = 0;
    int iframech =0;

    GetRecordChanneldataToDlgData(m_iRecordSelectId);

    for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
    {
        if(dlgdata.camera[i].camrec.iRecordingOn!=profile.camera[i].camrec.iRecordingOn)
        {
            profile.camera[i].camrec.iRecordingOn=dlgdata.camera[i].camrec.iRecordingOn;
            if(profile.camera[i].camrec.iRecordingOn==0 && detectstatus.detectrecording[i].iRecordingOn)
            {
                int ret=1;
                if(detectstatus.detectrecording[i].iNormal)
                {
                    detectstatus.detectrecording[i].iNormal=0;
                }
                if(detectstatus.detectrecording[i].iMotion)
                {
                    detectstatus.detectrecording[i].iMotion=0;
                }
                if(detectstatus.detectrecording[i].iSensor)
                {
                    detectstatus.detectrecording[i].iSensor=0;
                }
                if(detectstatus.detectrecording[i].iVideoloss)
                {
                    detectstatus.detectrecording[i].iVideoloss=0;
                }
                if(detectstatus.detectrecording[i].iEmergency)
                {
                    detectstatus.detectrecording[i].iEmergency=0;
                }
                detectstatus.detectrecording[i].iRecordingOn=0;
#if DVR_API_ENABLE
                ret=LIB816x_setRecChannel(i, 0, 0, 0 , " ");
#endif
            }
            else if(profile.camera[i].camrec.iRecordingOn==1 && detectstatus.detectrecording[i].iRecordingOn==0 && profile.dvrall.iRecordEmergencyOn==1)
            {
#if DVR_API_ENABLE
                LIB816x_setRecChannel(i, 1, LIB_EVENT_REC_EMG, profile.audioin.audioinch[i].iAudioInOn && profile.camera[i].camrec.iAudioOn , profile.camera[i].strcameraname);
#endif
                detectstatus.detectrecording[i].iEmergency = 1;
                detectstatus.detectrecording[i].iRecordingOn=1;
            }
        }

        if(dlgdata.camera[i].camrec.iIFrameInterval!=profile.camera[i].camrec.iIFrameInterval)
        {
            profile.camera[i].camrec.iIFrameInterval=dlgdata.camera[i].camrec.iIFrameInterval;
            iframech |= (0x01 << i);
            aIFVal[i] = profile.camera[i].camrec.iIFrameInterval;
        }

        if(dlgdata.camera[i].camrec.iBitrateType!=profile.camera[i].camrec.iBitrateType)
        {
            profile.camera[i].camrec.iBitrateType=dlgdata.camera[i].camrec.iBitrateType;
#if	DVR_API_ENABLE
            LIB816x_setBitrateType(i, profile.camera[i].camrec.iBitrateType);
#endif
        }

        if(dlgdata.camera[i].camrec.iFps!=profile.camera[i].camrec.iFps)
        {
            profile.camera[i].camrec.iFps=dlgdata.camera[i].camrec.iFps;
            frch |= (0x01 << i);
            switch(profile.camera[i].camrec.iFps)
            {
                case 0:
                        aFRVal[i] = FRAMERATE_04_03;
                        break;
                case 1:
                        aFRVal[i] = FRAMERATE_08_06;
                        break;
                case 2:
                        aFRVal[i] = FRAMERATE_15_13;
                        break;
                case 3:
                default:
                        aFRVal[i] = FRAMERATE_30_25;
                        break;
            }
        }

        if(dlgdata.camera[i].camrec.ikbps!=profile.camera[i].camrec.ikbps)
        {
            ch |= (0x01 << i);
            aVal[i]= dlgdata.camera[i].camrec.ikbps *1000;
            profile.camera[i].camrec.ikbps=dlgdata.camera[i].camrec.ikbps;

        }

        memcpy(&profile.camera[i].recschedule,&dlgdata.camera[i].recschedule,sizeof(RECSCHEDULE));


        if((dlgdata.camera[i].recordduration.iPrevRecordOn!=profile.camera[i].recordduration.iPrevRecordOn)
            || (dlgdata.camera[i].recordduration.iPrevRecordDuration!=profile.camera[i].recordduration.iPrevRecordDuration))
        {
            profile.camera[i].recordduration.iPrevRecordOn=dlgdata.camera[i].recordduration.iPrevRecordOn;
            profile.camera[i].recordduration.iPrevRecordDuration=dlgdata.camera[i].recordduration.iPrevRecordDuration;
#if	DVR_API_ENABLE
            LIB816x_setRecDuration(i, profile.camera[i].recordduration.iPrevRecordOn, profile.camera[i].recordduration.iPrevRecordDuration);
#endif
        }

        if((dlgdata.camera[i].recordduration.iPostRecordOn!=profile.camera[i].recordduration.iPostRecordOn)
            || (dlgdata.camera[i].recordduration.iPostRecordDuration!=profile.camera[i].recordduration.iPostRecordDuration))
        {
            profile.camera[i].recordduration.iPostRecordOn=dlgdata.camera[i].recordduration.iPostRecordOn;
            profile.camera[i].recordduration.iPostRecordDuration=dlgdata.camera[i].recordduration.iPostRecordDuration;
        }
        if(dlgdata.camera[i].camrec.iAudioOn!=profile.camera[i].camrec.iAudioOn)
        {
            profile.camera[i].camrec.iAudioOn=dlgdata.camera[i].camrec.iAudioOn;
        }
    }
    //bit rate setting for all channel
#if	DVR_API_ENABLE
    if(ch)
        LIB816x_set_encoder_property(LIB_ENC_SET_BITRATE, ch, aVal);
    if(frch)
        LIB816x_set_encoder_property(LIB_ENC_SET_FRAMERATE, frch, aFRVal);
    if(iframech)
        LIB816x_set_encoder_property(LIB_ENC_SET_I_FRAME_INTERVAL, iframech, aIFVal);
#endif
    pMainClass->RecScheduleHandler(curtime.currenttime.tm_wday,curtime.currenttime.tm_hour);
}
/*!cancel시 record관련 변경적용된 내역을 복원시킴 */
void dvr_setup::ResotreRecord()
{
    int aVal[MAX_INTERNAL_CAMERA];
    int aFRVal[MAX_INTERNAL_CAMERA];
    int aIFVal[MAX_INTERNAL_CAMERA];
    int ch=0;
    int frch = 0;
    int iframech =0;

    for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
    {
        if(dlgdata.camera[i].camrec.iRecordingOn!=profile.camera[i].camrec.iRecordingOn)
        {
            profile.camera[i].camrec.iRecordingOn=dlgdata.camera[i].camrec.iRecordingOn;
            if(profile.camera[i].camrec.iRecordingOn==0 && detectstatus.detectrecording[i].iRecordingOn)
            {
                int ret=1;
                if(detectstatus.detectrecording[i].iNormal)
                {
                    detectstatus.detectrecording[i].iNormal=0;
                }
                if(detectstatus.detectrecording[i].iMotion)
                {
                    detectstatus.detectrecording[i].iMotion=0;
                }
                if(detectstatus.detectrecording[i].iSensor)
                {
                    detectstatus.detectrecording[i].iSensor=0;
                }
                if(detectstatus.detectrecording[i].iVideoloss)
                {
                    detectstatus.detectrecording[i].iVideoloss=0;
                }
                if(detectstatus.detectrecording[i].iEmergency)
                {
                    detectstatus.detectrecording[i].iEmergency=0;
                }
                detectstatus.detectrecording[i].iRecordingOn=0;
#if DVR_API_ENABLE
                ret=LIB816x_setRecChannel(i, 0, 0, 0 , " ");
#endif
            }
            else if(profile.camera[i].camrec.iRecordingOn==1 && detectstatus.detectrecording[i].iRecordingOn==0 && profile.dvrall.iRecordEmergencyOn==1)
            {
#if DVR_API_ENABLE
                LIB816x_setRecChannel(i, 1, LIB_EVENT_REC_EMG, profile.audioin.audioinch[i].iAudioInOn && profile.camera[i].camrec.iAudioOn , profile.camera[i].strcameraname);
#endif
                detectstatus.detectrecording[i].iEmergency = 1;
                detectstatus.detectrecording[i].iRecordingOn=1;
            }
        }

        if(dlgdata.camera[i].camrec.iIFrameInterval!=profile.camera[i].camrec.iIFrameInterval)
        {
            profile.camera[i].camrec.iIFrameInterval=dlgdata.camera[i].camrec.iIFrameInterval;
            iframech |= (0x01 << i);
            aIFVal[i] = profile.camera[i].camrec.iIFrameInterval;
        }

        if(dlgdata.camera[i].camrec.iBitrateType!=profile.camera[i].camrec.iBitrateType)
        {
            profile.camera[i].camrec.iBitrateType=dlgdata.camera[i].camrec.iBitrateType;
#if	DVR_API_ENABLE
            LIB816x_setBitrateType(i, profile.camera[i].camrec.iBitrateType);
#endif
        }

        if(dlgdata.camera[i].camrec.iFps!=profile.camera[i].camrec.iFps)
        {
            profile.camera[i].camrec.iFps=dlgdata.camera[i].camrec.iFps;
            frch |= (0x01 << i);
            switch(profile.camera[i].camrec.iFps)
            {
                case 0:
                        aFRVal[i] = FRAMERATE_04_03;
                        break;
                case 1:
                        aFRVal[i] = FRAMERATE_08_06;
                        break;
                case 2:
                        aFRVal[i] = FRAMERATE_15_13;
                        break;
                case 3:
                default:
                        aFRVal[i] = FRAMERATE_30_25;
                        break;
            }
        }

        if(dlgdata.camera[i].camrec.ikbps!=profile.camera[i].camrec.ikbps)
        {
            ch |= (0x01 << i);

            aVal[i]= dlgdata.camera[i].camrec.ikbps *1000;
            profile.camera[i].camrec.ikbps=dlgdata.camera[i].camrec.ikbps;

        }

        memcpy(&profile.camera[i].recschedule,&dlgdata.camera[i].recschedule,sizeof(RECSCHEDULE));


        if((dlgdata.camera[i].recordduration.iPrevRecordOn!=profile.camera[i].recordduration.iPrevRecordOn)
            || (dlgdata.camera[i].recordduration.iPrevRecordDuration!=profile.camera[i].recordduration.iPrevRecordDuration))
        {
            profile.camera[i].recordduration.iPrevRecordOn=dlgdata.camera[i].recordduration.iPrevRecordOn;
            profile.camera[i].recordduration.iPrevRecordDuration=dlgdata.camera[i].recordduration.iPrevRecordDuration;
#if	DVR_API_ENABLE
            LIB816x_setRecDuration(i, profile.camera[i].recordduration.iPrevRecordOn, profile.camera[i].recordduration.iPrevRecordDuration);
#endif
        }

        if((dlgdata.camera[i].recordduration.iPostRecordOn!=profile.camera[i].recordduration.iPostRecordOn)
            || (dlgdata.camera[i].recordduration.iPostRecordDuration!=profile.camera[i].recordduration.iPostRecordDuration))
        {
            profile.camera[i].recordduration.iPostRecordOn=dlgdata.camera[i].recordduration.iPostRecordOn;
            profile.camera[i].recordduration.iPostRecordDuration=dlgdata.camera[i].recordduration.iPostRecordDuration;
        }
        if(dlgdata.camera[i].camrec.iAudioOn!=profile.camera[i].camrec.iAudioOn)
        {
            profile.camera[i].camrec.iAudioOn=dlgdata.camera[i].camrec.iAudioOn;
        }
    }
    //bit rate setting for all channel
#if	DVR_API_ENABLE
    if(ch)
        LIB816x_set_encoder_property(LIB_ENC_SET_BITRATE, ch, aVal);
    if(frch)
        LIB816x_set_encoder_property(LIB_ENC_SET_FRAMERATE, frch, aFRVal);
    if(iframech)
        LIB816x_set_encoder_property(LIB_ENC_SET_I_FRAME_INTERVAL, iframech, aIFVal);
#endif
    pMainClass->RecScheduleHandler(curtime.currenttime.tm_wday,curtime.currenttime.tm_hour);
}
/*!record camera tree에 현재 선택item을 set */
void dvr_setup::SetSetupRecordTreeControl()
{
    ui->recordtreeWidget->setCurrentItem (&SetupRecordTreeItem[m_iRecordSelectId]);
}
/*!현재 선택된 camera에 대해서 ui에 값을 set */
void dvr_setup::SetSetupRecordButtonControls(int iChannel)
{
    ui->recordenable->setCurrentIndex(dlgdata.camera[iChannel].camrec.iRecordingOn);
    ui->recordiframeintervalcomboBox->setCurrentIndex(dlgdata.camera[iChannel].camrec.iIFrameInterval);
    ui->recordbitratetypecomboBox->setCurrentIndex(dlgdata.camera[iChannel].camrec.iBitrateType);

    ui->recordframerateslider->setSliderPosition(dlgdata.camera[iChannel].camrec.iFps);
    ui->recordbitrateslider->setSliderPosition(dlgdata.camera[iChannel].camrec.ikbps);
    ui->recordframeratevalue->setText(SetRecordFramerateValueText(dlgdata.camera[iChannel].camrec.iFps));

    QString qstrvalue;
    char frameratebuf[MAX_CHAR_10];
    int kbps = (dlgdata.camera[iChannel].camrec.ikbps/100)*100;
    sprintf(frameratebuf,"%dkbps",kbps);
    qstrvalue=StrToQString(frameratebuf);
    ui->recordbitratevalue->setText(qstrvalue);

    ui->recordscheduleeventtypecomboBox->setCurrentIndex(m_iRecordEventTypeMode);

    SetRecordScheduleTable(iChannel);

    ui->recordeventprevenable->setCurrentIndex(dlgdata.camera[iChannel].recordduration.iPrevRecordOn);

    ui->recordeventprevdurationslider->setSliderPosition(dlgdata.camera[iChannel].recordduration.iPrevRecordDuration);
    ui->recordeventprevdurationvalue->setText(SetRecordEventPrevDurationValueText(dlgdata.camera[iChannel].recordduration.iPrevRecordDuration));

    ui->recordeventpostenable->setCurrentIndex(dlgdata.camera[iChannel].recordduration.iPostRecordOn);

    ui->recordeventpostdurationslider->setSliderPosition(dlgdata.camera[iChannel].recordduration.iPostRecordDuration);
    ui->recordeventpostdurationvalue->setText(SetRecordEventPostDurationValueText(dlgdata.camera[iChannel].recordduration.iPostRecordDuration));

    ui->recordaudioenable->setCurrentIndex(dlgdata.camera[iChannel].camrec.iAudioOn);

}
/*!pre record duration값을 text로 표시 */
QString dvr_setup::SetRecordEventPrevDurationValueText(int ivalue)
{
    QString qstrvalue;
    char prevdurationbuf[MAX_CHAR_8];
    sprintf(prevdurationbuf,"%d %s",ivalue,STRSECOND);
    qstrvalue=StrToQString(prevdurationbuf);

    return qstrvalue;
}
/*!post record duration값을 text로 표시 */
QString dvr_setup::SetRecordEventPostDurationValueText(int ivalue)
{
    QString qstrvalue;
    char postdurationbuf[MAX_CHAR_8];
    sprintf(postdurationbuf,"%d %s",ivalue,STRMINUTE);
    qstrvalue=StrToQString(postdurationbuf);

    return qstrvalue;
}
/*!frame rate의 설정값을 text로 표시 */
QString dvr_setup::SetRecordFramerateValueText(int ivalue)
{
    QString qstrvalue;
    char frameratebuf[MAX_CHAR_8];
    if(dlgruntimesettings.chStatus[m_iRecordSelectId].frameHeight==240)
    {
    	//NTSC
        switch(ivalue)
        {
        case 0:
            sprintf(frameratebuf,"%d %s",4,STRFPS);
            break;
        case 1:
            sprintf(frameratebuf,"%d %s",8,STRFPS);
            break;
        case 2:
            sprintf(frameratebuf,"%d %s",15,STRFPS);
            break;
        case 3:
        default:
            sprintf(frameratebuf,"%d %s",30,STRFPS);
            break;
        }
    }
    else
    {
    	//PAL
        switch(ivalue)
        {
        case 0:
            sprintf(frameratebuf,"%d %s",3,STRFPS);
            break;
        case 1:
            sprintf(frameratebuf,"%d %s",6,STRFPS);
            break;
        case 2:
            sprintf(frameratebuf,"%d %s",13,STRFPS);
            break;
        case 3:
        default:
            sprintf(frameratebuf,"%d %s",25,STRFPS);
            break;
        }
    }
    qstrvalue=StrToQString(frameratebuf);

    return qstrvalue;
}
/*!선택 channel에 대한 record schedule을 ui에 표시 */
void dvr_setup::SetRecordScheduleTable(int iChannel)
{
    int i=0;
    int j=0;

    for(j=0;j<ui->recordscheduletableWidget->rowCount();j++)
    {
        for(i=0;i<ui->recordscheduletableWidget->columnCount();i++)
        {
              ui->recordscheduletableWidget->takeItem(j,i);
        }
    }

    ui->recordscheduletableWidget->setRowCount(MAX_DAY);
    ui->recordscheduletableWidget->setColumnCount(MAX_HOUR);

    int icellwidth=ui->recordscheduletableWidget->width()/ui->recordscheduletableWidget->columnCount();
    int icellheight=ui->recordscheduletableWidget->height()/ui->recordscheduletableWidget->rowCount();

    for(i=0;i<ui->recordscheduletableWidget->columnCount();i++)
    {
        ui->recordscheduletableWidget->setColumnWidth(i,icellwidth);
    }

    for(i=0;i<ui->recordscheduletableWidget->rowCount();i++)
    {
        ui->recordscheduletableWidget->setRowHeight(i,icellheight);
    }

    for(j=0;j<ui->recordscheduletableWidget->rowCount();j++)
    {
        for(i=0;i<ui->recordscheduletableWidget->columnCount();i++)
        {
            if(dlgdata.camera[iChannel].recschedule.ScheduleTable[j][i]==REC_SCHEDULE_CONTINUOUSRECORD)
            {
                m_recordscheduleitem[j][i].setBackgroundColor(m_RecordScheduleBlockColor[REC_SCHEDULE_CONTINUOUSRECORD]);
            }
            else if(dlgdata.camera[iChannel].recschedule.ScheduleTable[j][i]==REC_SCHEDULE_RECORDBYMOTION)
            {
                m_recordscheduleitem[j][i].setBackgroundColor(m_RecordScheduleBlockColor[REC_SCHEDULE_RECORDBYMOTION]);
            }
            else if(dlgdata.camera[iChannel].recschedule.ScheduleTable[j][i]==REC_SCHEDULE_RECORDBYSENSOR)
            {
                m_recordscheduleitem[j][i].setBackgroundColor(m_RecordScheduleBlockColor[REC_SCHEDULE_RECORDBYSENSOR]);
            }
            else if(dlgdata.camera[iChannel].recschedule.ScheduleTable[j][i]==REC_SCHEDULE_RECORDBYVLOSS)
            {
                m_recordscheduleitem[j][i].setBackgroundColor(m_RecordScheduleBlockColor[REC_SCHEDULE_RECORDBYVLOSS]);
            }
            else if(dlgdata.camera[iChannel].recschedule.ScheduleTable[j][i]==REC_SCHEDULE_NORECORD)
            {
                m_recordscheduleitem[j][i].setBackgroundColor(m_RecordScheduleBlockColor[REC_SCHEDULE_NORECORD]);
            }

            ui->recordscheduletableWidget->setItem(j, i,&m_recordscheduleitem[j][i]);
        }
    }
}
/*!ui의 현재 설정을 읽어 선택된 camera의 해당 data에 setting */
void dvr_setup::GetRecordChanneldataToDlgData(int iChannel)
{
    dlgdata.camera[iChannel].camrec.iRecordingOn=ui->recordenable->currentIndex();
    dlgdata.camera[iChannel].camrec.iIFrameInterval=ui->recordiframeintervalcomboBox->currentIndex();
    dlgdata.camera[iChannel].camrec.iBitrateType=ui->recordbitratetypecomboBox->currentIndex();
    dlgdata.camera[iChannel].camrec.iFps=ui->recordframerateslider->sliderPosition();

    dlgdata.camera[iChannel].camrec.ikbps=(ui->recordbitrateslider->sliderPosition());

    dlgdata.camera[iChannel].recordduration.iPrevRecordOn=ui->recordeventprevenable->currentIndex();

    for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
    {
        dlgdata.camera[i].recordduration.iPrevRecordDuration=ui->recordeventprevdurationslider->sliderPosition();
    }

    dlgdata.camera[iChannel].recordduration.iPostRecordOn=ui->recordeventpostenable->currentIndex();

    dlgdata.camera[iChannel].recordduration.iPostRecordDuration=ui->recordeventpostdurationslider->sliderPosition();

    dlgdata.camera[iChannel].camrec.iAudioOn=ui->recordaudioenable->currentIndex();
}
/*!record schedule event type combo box가 변경시 호출 */
void dvr_setup::ChangeRecordscheduleEventTypeModeSlot(int index)
{
    m_iRecordEventTypeMode=index;
}
/*!camera tree가 click될때 호출 */
void dvr_setup::SetupRecordTreeItemClicked(QTreeWidgetItem *item, int column)
{
    GetRecordChanneldataToDlgData(m_iRecordSelectId);
    m_iRecordSelectId=ui->recordtreeWidget->indexOfTopLevelItem(item);
    SetSetupRecordButtonControls(m_iRecordSelectId);
}
/*!camera tree가 double click될때 호출 */
void dvr_setup::SetupRecordTreeItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    SetupRecordTreeItemClicked(item,column);
}
/*!frame rate slide값이 변경이 될때 호출 */
void dvr_setup::ChangeRecordframerateSlot(int value)
{
    ui->recordframeratevalue->setText(SetRecordFramerateValueText(value));
}
/*!bit rate slide값이 변경이 될때 호출 */
void dvr_setup::ChangeRecordbitrateSlot(int value)
{
    QString qstrvalue;
    int kbps = (value/100)*100;
    char frameratebuf[MAX_CHAR_8];
    sprintf(frameratebuf,"%dkbps",kbps);
    qstrvalue=StrToQString(frameratebuf);
    ui->recordbitratevalue->setText(qstrvalue);
}
/*!record schedule table이 선택이 될때 호출 */
void dvr_setup::RecordScheduleTableSelectSlot ( int row, int column )
{
    dprintf("RecordScheduleTableSelectSlot col=%d,r=%d\n",column,row);
    if(dlgdata.camera[m_iRecordSelectId].iEnable == 0 && m_iWarnRecordSelectId != m_iRecordSelectId)
    {
        m_iWarnRecordSelectId = m_iRecordSelectId;
        char str[50];
        sprintf(str,"%s is disabled",dlgdata.camera[m_iRecordSelectId].strcameraname);
        pMainClass->ShowNotifyDialog(str);
    }
    ui->recordscheduletableWidget->takeItem(row,column);
    if(dlgdata.camera[m_iRecordSelectId].recschedule.ScheduleTable[row][column]==m_iRecordEventTypeMode)
    {
        m_recordscheduleitem[row][column].setBackgroundColor(m_RecordScheduleBlockColor[REC_SCHEDULE_NORECORD]);
        dlgdata.camera[m_iRecordSelectId].recschedule.ScheduleTable[row][column]=REC_SCHEDULE_NORECORD;
    }
    else
    {
        m_recordscheduleitem[row][column].setBackgroundColor(m_RecordScheduleBlockColor[m_iRecordEventTypeMode]);
        dlgdata.camera[m_iRecordSelectId].recschedule.ScheduleTable[row][column]=m_iRecordEventTypeMode;
    }

    ui->recordscheduletableWidget->setItem(row, column,&m_recordscheduleitem[row][column]);
}
/*!apply all button click될때 호출, schedule table을 선택한 event로 모두 변경*/
void dvr_setup::RecordScheduleApplyAllBtnSlot()
{
    int i=0;
    int j=0;

    for(j=0;j<ui->recordscheduletableWidget->rowCount();j++)
    {
        for(i=0;i<ui->recordscheduletableWidget->columnCount();i++)
        {
            ui->recordscheduletableWidget->takeItem(j,i);

            m_recordscheduleitem[j][i].setBackgroundColor(m_RecordScheduleBlockColor[m_iRecordEventTypeMode]);
            dlgdata.camera[m_iRecordSelectId].recschedule.ScheduleTable[j][i]=m_iRecordEventTypeMode;

             ui->recordscheduletableWidget->setItem(j, i,&m_recordscheduleitem[j][i]);
        }
    }
}
/*!pre-recording duration slide가 변경이 될때 호출 */
void dvr_setup::ChangePrevRecordDurationSlot(int value)
{
    ui->recordeventprevdurationvalue->setText(SetRecordEventPrevDurationValueText(value));
}
/*!post-recording duration slide가 변경이 될때 호출 */
void dvr_setup::ChangePostRecordDurationSlot(int value)
{
    ui->recordeventpostdurationvalue->setText(SetRecordEventPostDurationValueText(value));
}
/*!apply all ch button click될때 호출 -record tab 설정을 모든 camera에 대해서 적용*/
void dvr_setup::on_recordapplyallchpushButton_clicked()
{
    CAMREC camrec;
    RECORDDURATION duration;
    GetRecordChanneldataToDlgData(0);
    memcpy(&camrec,&dlgdata.camera[0].camrec,sizeof(CAMREC));
    memcpy(&duration,&dlgdata.camera[0].recordduration,sizeof(RECORDDURATION));
    memcpy(&dlgdata.camera[0].recschedule,&dlgdata.camera[m_iRecordSelectId].recschedule,sizeof(RECSCHEDULE));

    for(int i=1;i<MAX_INTERNAL_CAMERA;i++)
    {
        memcpy(&dlgdata.camera[i].camrec,&camrec,sizeof(CAMREC));
        memcpy(&dlgdata.camera[i].recordduration,&duration,sizeof(RECORDDURATION));
        memcpy(&dlgdata.camera[i].recschedule,&dlgdata.camera[m_iRecordSelectId].recschedule,sizeof(RECSCHEDULE));
    }
}
