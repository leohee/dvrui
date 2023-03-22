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
extern dvr_layout_main *pLayoutClass;
extern dvr_toolbar_main *pToolbar;
extern dvr_sidebar *pSidebar;
/*----------------------------------------------------------------------------
 local functions
-----------------------------------------------------------------------------*/
/*!camera tab의 control들을 초기화 */
void dvr_setup::InitSetupCameraControls()
{
    m_iCameraSelectId=0;
    m_iMotionAreaMode=MOTIONAREA_MODE_CLEAR;
    m_MotionBlockColor[MOTIONAREA_MODE_CLEAR]=Qt::darkGray;/*magenta*/
    m_MotionBlockColor[MOTIONAREA_MODE_SET]=Qt::yellow;

    QSize qsize;
    qsize.setWidth(CAMERATREEICON_SIZE_W);
    qsize.setHeight(CAMERATREEICON_SIZE_H);
    CameraTreeItemIcon.addFile(QString::fromUtf8(CAMERA_TREE_CAMERA_ICON), qsize, QIcon::Normal, QIcon::Off);
    QRect qrect;
    qrect=ui->camerapreviewframe->geometry();
    ui->motionareatableWidget->setGeometry(qrect);
    m_iMotionAreaW=qrect.width();
    m_iMotionAreaH=qrect.height();
    m_iMotionBlockW=MOTION_W_CELL;
    m_iMotionBlockH=MOTION_H_CELL;

    connect(ui->cameratreeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(SetupCameraTreeItemClicked(QTreeWidgetItem *, int)));
    connect(ui->cameratreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(SetupCameraTreeItemDoubleClicked(QTreeWidgetItem *, int)));

    connect(ui->motionsensitivityslider, SIGNAL(valueChanged(int)), this, SLOT(ChangeMotionSensitivitySlot(int)));
    connect(ui->motionareamodecomboBox, SIGNAL(currentIndexChanged (int)), this, SLOT(ChangeMotionAreaModeSlot(int)));
    connect(ui->motionareaallclearpushButton, SIGNAL(clicked()), this, SLOT(MotionAreaAllClearModeBtnSlot()));
    connect(ui->motionareaallsetpushButton, SIGNAL(clicked()), this, SLOT(MotionAreaAllSetModeBtnSlot()));
    connect(ui->motionareatableWidget, SIGNAL(cellPressed (int,int)), this, SLOT(MotionAreaSelectSlot(int,int)));
    connect(ui->motionareatableWidget, SIGNAL(cellEntered (int,int)), this, SLOT(MotionAreaSelectSlot(int,int)));

    connect(ui->cameratitlevirtualkeypushButton, SIGNAL(clicked()), this, SLOT(CameraTitleVirtualKeyboardBtnSlot()));
    connect(ui->colorbrightnessvirtualkeypushButton, SIGNAL(clicked()), this, SLOT(ColorBrightnessVirtualKeyboardBtnSlot()));
    connect(ui->colorcontrastvirtualkeypushButton, SIGNAL(clicked()), this, SLOT(ColorContrastVirtualKeyboardBtnSlot()));
    connect(ui->colorsaturationvirtualkeypushButton, SIGNAL(clicked()), this, SLOT(ColorSaturationVirtualKeyboardBtnSlot()));
    connect(ui->colordefaultpushButton, SIGNAL(clicked()), this, SLOT(ColorDefaultBtnSlot()));
    for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
        bMotionTableChanged[i]=false;
}

/*!profile data struct에 따라서 ui에 value setting */
void dvr_setup::SetCameradata()
{
    SetSetupCameraTreeControl();
    SetSetupCameraButtonControls(m_iCameraSelectId);
}
/*!ui control의 value를 읽고 해당 기능을 적용시킴 */
void dvr_setup::GetCameradata()
{
    int iCameraNameChanged=0;
    int iCameraEnableChanged=0;
    COLORADJUST adjust;

    GetCameraChanneldataToDlgData(m_iCameraSelectId);

    for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
    {
        if(dlgdata.camera[i].iEnable!=profile.camera[i].iEnable)
        {
            profile.camera[i].iEnable=dlgdata.camera[i].iEnable;
            iCameraEnableChanged=1;
            //camera가 enable로 변경된경우 emergency record중이라면 record start
            if(profile.dvrall.iRecordEmergencyOn==1)
            {
                if(profile.camera[i].iEnable  && detectstatus.detectrecording[i].iEmergency ==0)
                {
                    detectstatus.detectrecording[i].iEmergency = 1;
                    detectstatus.detectrecording[i].iRecordingOn=1;
#if DVR_API_ENABLE
                    LIB816x_setRecChannel(i, 1, LIB_EVENT_REC_EMG, profile.audioin.audioinch[i].iAudioInOn && profile.camera[i].camrec.iAudioOn , profile.camera[i].strcameraname);
#endif
                }
                else if(profile.camera[i].iEnable==0  && detectstatus.detectrecording[i].iEmergency ==1)
                {
                    detectstatus.detectrecording[i].iEmergency = 0;
                    detectstatus.detectrecording[i].iRecordingOn=0;
#if DVR_API_ENABLE
                    LIB816x_setRecChannel(i, 0, 0, 0, " ");
#endif
                }
            }
#if DVR_API_ENABLE
            LIB816x_setCameraEnable( i, profile.camera[i].iEnable);
#endif
        }
		//camera name이 변경된 경우
        if(strcmp(dlgdata.camera[i].strcameraname,profile.camera[i].strcameraname)!=0)
        {
            strcpy(profile.camera[i].strcameraname,dlgdata.camera[i].strcameraname);
            iCameraNameChanged=1;
#if DVR_API_ENABLE
            LIB816x_changeCamName(i, profile.camera[i].strcameraname);
#endif
        }
		//covert가 변경된 경우
        if(dlgdata.camera[i].iCovertOn!=profile.camera[i].iCovertOn)
        {
            iCameraEnableChanged=1;
            profile.camera[i].iCovertOn=dlgdata.camera[i].iCovertOn;
#if DVR_API_ENABLE
            if(profile.camera[i].iEnable)
            {
                LIB816x_setCovert(i, profile.camera[i].iCovertOn);
            }
#endif
        }
		//resolution이 변경된 경우
        if(dlgdata.camera[i].camrec.iResolution!=profile.camera[i].camrec.iResolution)
        {
            profile.camera[i].camrec.iResolution=dlgdata.camera[i].camrec.iResolution;
#if	DVR_API_ENABLE
            LIB816x_setVideoResolution(i, profile.camera[i].camrec.iResolution);
#endif
        }
		//motion이 변경된 경우
        if(dlgdata.camera[i].motion.iMotionOn!=profile.camera[i].motion.iMotionOn || dlgdata.camera[i].motion.iSensitivity!=profile.camera[i].motion.iSensitivity || bMotionTableChanged[i])
        {
            profile.camera[i].motion.iMotionOn=dlgdata.camera[i].motion.iMotionOn;
            profile.camera[i].motion.iSensitivity=dlgdata.camera[i].motion.iSensitivity;
            if(bMotionTableChanged[i])
            {
                memcpy(profile.camera[i].motion.motiontable,dlgdata.camera[i].motion.motiontable,MAX_MOTION_CELL);
            }
#if DVR_API_ENABLE
            LIB816x_setMotion(i, profile.camera[i].motion.iMotionOn, profile.camera[i].motion.iSensitivity, profile.camera[i].motion.motiontable);
#endif
        }
        bMotionTableChanged[i]=false;
		//color setting값이 변경된 경우
        if(dlgdata.camera[i].coloradjust.iBrightness!=profile.camera[i].coloradjust.iBrightness
           || dlgdata.camera[i].coloradjust.iContrast!=profile.camera[i].coloradjust.iContrast
           || dlgdata.camera[i].coloradjust.iSaturation!=profile.camera[i].coloradjust.iSaturation)
        {
            profile.camera[i].coloradjust.iBrightness=dlgdata.camera[i].coloradjust.iBrightness;
            profile.camera[i].coloradjust.iContrast=dlgdata.camera[i].coloradjust.iContrast;
            profile.camera[i].coloradjust.iSaturation=dlgdata.camera[i].coloradjust.iSaturation;
#if DVR_API_ENABLE
            adjust.iBrightness = profile.camera[i].coloradjust.iBrightness;
            adjust.iContrast = profile.camera[i].coloradjust.iContrast;
            adjust.iSaturation = profile.camera[i].coloradjust.iSaturation;
            LIB816x_setColorAdjust(i, &adjust);
#endif
        }
		//ptz 가 변경된 경우
        if(dlgdata.camera[i].ptzinfo.iPtzOn!=profile.camera[i].ptzinfo.iPtzOn)
        {
            profile.camera[i].ptzinfo.iPtzOn=dlgdata.camera[i].ptzinfo.iPtzOn;
        }

        if(dlgdata.camera[i].ptzinfo.iPtzdriverIndex!=profile.camera[i].ptzinfo.iPtzdriverIndex)
        {
            profile.camera[i].ptzinfo.iPtzdriverIndex=dlgdata.camera[i].ptzinfo.iPtzdriverIndex;
        }

        if(dlgdata.camera[i].ptzinfo.iPtzAddress!=profile.camera[i].ptzinfo.iPtzAddress)
        {
            profile.camera[i].ptzinfo.iPtzAddress=dlgdata.camera[i].ptzinfo.iPtzAddress;
        }

        if(dlgdata.camera[i].ptzinfo.iBaudRate!=profile.camera[i].ptzinfo.iBaudRate)
        {
            profile.camera[i].ptzinfo.iBaudRate=dlgdata.camera[i].ptzinfo.iBaudRate;
        }

        if(dlgdata.camera[i].ptzinfo.iDataBit!=profile.camera[i].ptzinfo.iDataBit)
        {
            profile.camera[i].ptzinfo.iDataBit=dlgdata.camera[i].ptzinfo.iDataBit;
        }

        if(dlgdata.camera[i].ptzinfo.iStopBit!=profile.camera[i].ptzinfo.iStopBit)
        {
            profile.camera[i].ptzinfo.iStopBit=dlgdata.camera[i].ptzinfo.iStopBit;
        }

        if(dlgdata.camera[i].ptzinfo.iParityBit!=profile.camera[i].ptzinfo.iParityBit)
        {
            profile.camera[i].ptzinfo.iParityBit=dlgdata.camera[i].ptzinfo.iParityBit;
        }
    }
	
    if((iCameraNameChanged==1)||(iCameraEnableChanged==1))
    {
    	//camera enable,name이 변경된 경우
    	//side bar tree update
        pSidebar->setSideBarTreeControl();
        if(iCameraEnableChanged==1)
        {
#if DVR_API_ENABLE
            if(!profile.iSubOutputEnable)
            {
                if(runtimesettings.iLivePlaybackMode==LIB_LIVE_MODE)
                {
                    if(runtimesettings.iLayout*runtimesettings.iPageNum<16)
				LIB816x_setCameraLayout_set(profile.layout.iMainOutput, runtimesettings.iLayout*runtimesettings.iPageNum, runtimesettings.iLayout,m_iCameraSelectId);
                        //LIB816x_setCameraLayout(profile.layout.iMainOutput, runtimesettings.iLayout*runtimesettings.iPageNum, runtimesettings.iLayout,m_iCameraSelectId);
                }
                else
                {
                    if(runtimesettings.iLayout*runtimesettings.iPageNum<16)
                        LIB816x_setCameraLayout(profile.layout.iMainOutput, 16+runtimesettings.iLayout*runtimesettings.iPageNum, runtimesettings.iLayout);
                }
            }
            else
            {
                if(runtimesettings.iLayout*runtimesettings.iPageNum<16)
                    LIB816x_setCameraLayout(profile.layout.iMainOutput, runtimesettings.iLayout*runtimesettings.iPageNum, runtimesettings.iLayout);
            }
#endif
            pToolbar->MaxPageCalc();

        }
        //layout update 
        pLayoutClass->DrawOSDChannel();
    }
}

/*!cancel시 camera관련 변경적용된 내역을 복원시킴 */
void dvr_setup::ResotreCamera()
{
    int iCameraNameChanged=0;
    int iCameraEnableChanged=0;

    COLORADJUST adjust;

    for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
    {
        if(dlgdata.camera[i].iEnable!=profile.camera[i].iEnable)
        {
            profile.camera[i].iEnable=dlgdata.camera[i].iEnable;
            iCameraEnableChanged=1;
            if(profile.dvrall.iRecordEmergencyOn==1)
            {
                if(profile.camera[i].iEnable  && detectstatus.detectrecording[i].iEmergency ==0)
                {
                    detectstatus.detectrecording[i].iEmergency = 1;
                    detectstatus.detectrecording[i].iRecordingOn=1;
#if DVR_API_ENABLE
                    LIB816x_setRecChannel(i, 1, LIB_EVENT_REC_EMG, profile.audioin.audioinch[i].iAudioInOn && profile.camera[i].camrec.iAudioOn , profile.camera[i].strcameraname);
#endif
                }
                else if(profile.camera[i].iEnable==0  && detectstatus.detectrecording[i].iEmergency ==1)
                {
                    detectstatus.detectrecording[i].iEmergency = 0;
                    detectstatus.detectrecording[i].iRecordingOn=0;
#if DVR_API_ENABLE
                    LIB816x_setRecChannel(i, 0, 0, 0 ," ");
#endif
                }
            }
#if DVR_API_ENABLE
            LIB816x_setCameraEnable( i, profile.camera[i].iEnable);
#endif
        }

        if(strcmp(dlgdata.camera[i].strcameraname,profile.camera[i].strcameraname)!=0)
        {
            strcpy(profile.camera[i].strcameraname,dlgdata.camera[i].strcameraname);
            iCameraNameChanged=1;
#if DVR_API_ENABLE
            LIB816x_changeCamName(i, profile.camera[i].strcameraname);
#endif
        }

        if(dlgdata.camera[i].iCovertOn!=profile.camera[i].iCovertOn)
        {
            iCameraEnableChanged=1;
            profile.camera[i].iCovertOn=dlgdata.camera[i].iCovertOn;
#if DVR_API_ENABLE
            if(profile.camera[i].iEnable)
            {
                LIB816x_setCovert(i, profile.camera[i].iCovertOn);
            }
#endif
        }

        if(dlgdata.camera[i].camrec.iResolution!=profile.camera[i].camrec.iResolution)
        {
            profile.camera[i].camrec.iResolution=dlgdata.camera[i].camrec.iResolution;
#if	DVR_API_ENABLE
            LIB816x_setVideoResolution(i, profile.camera[i].camrec.iResolution);
#endif
        }

        if(dlgdata.camera[i].motion.iMotionOn!=profile.camera[i].motion.iMotionOn || dlgdata.camera[i].motion.iSensitivity!=profile.camera[i].motion.iSensitivity || bMotionTableChanged[i])
        {
            profile.camera[i].motion.iMotionOn=dlgdata.camera[i].motion.iMotionOn;
            profile.camera[i].motion.iSensitivity=dlgdata.camera[i].motion.iSensitivity;
            if(bMotionTableChanged[i])
            {
                memcpy(profile.camera[i].motion.motiontable,dlgdata.camera[i].motion.motiontable,MAX_MOTION_CELL);
            }
#if DVR_API_ENABLE
            LIB816x_setMotion(i, profile.camera[i].motion.iMotionOn, profile.camera[i].motion.iSensitivity, profile.camera[i].motion.motiontable);
#endif
        }
        bMotionTableChanged[i]=false;

        if(dlgdata.camera[i].coloradjust.iBrightness!=profile.camera[i].coloradjust.iBrightness
           || dlgdata.camera[i].coloradjust.iContrast!=profile.camera[i].coloradjust.iContrast
           || dlgdata.camera[i].coloradjust.iSaturation!=profile.camera[i].coloradjust.iSaturation)
        {
            profile.camera[i].coloradjust.iBrightness=dlgdata.camera[i].coloradjust.iBrightness;
            profile.camera[i].coloradjust.iContrast=dlgdata.camera[i].coloradjust.iContrast;
            profile.camera[i].coloradjust.iSaturation=dlgdata.camera[i].coloradjust.iSaturation;
#if DVR_API_ENABLE
            adjust.iBrightness = profile.camera[i].coloradjust.iBrightness;
            adjust.iContrast = profile.camera[i].coloradjust.iContrast;
            adjust.iSaturation = profile.camera[i].coloradjust.iSaturation;
            LIB816x_setColorAdjust(i, &adjust);
#endif
        }

        if(dlgdata.camera[i].ptzinfo.iPtzOn!=profile.camera[i].ptzinfo.iPtzOn)
        {
            profile.camera[i].ptzinfo.iPtzOn=dlgdata.camera[i].ptzinfo.iPtzOn;
        }

        if(dlgdata.camera[i].ptzinfo.iPtzdriverIndex!=profile.camera[i].ptzinfo.iPtzdriverIndex)
        {
            profile.camera[i].ptzinfo.iPtzdriverIndex=dlgdata.camera[i].ptzinfo.iPtzdriverIndex;
        }

        if(dlgdata.camera[i].ptzinfo.iPtzAddress!=profile.camera[i].ptzinfo.iPtzAddress)
        {
            profile.camera[i].ptzinfo.iPtzAddress=dlgdata.camera[i].ptzinfo.iPtzAddress;
        }

        if(dlgdata.camera[i].ptzinfo.iBaudRate!=profile.camera[i].ptzinfo.iBaudRate)
        {
            profile.camera[i].ptzinfo.iBaudRate=dlgdata.camera[i].ptzinfo.iBaudRate;
        }

        if(dlgdata.camera[i].ptzinfo.iDataBit!=profile.camera[i].ptzinfo.iDataBit)
        {
            profile.camera[i].ptzinfo.iDataBit=dlgdata.camera[i].ptzinfo.iDataBit;
        }

        if(dlgdata.camera[i].ptzinfo.iStopBit!=profile.camera[i].ptzinfo.iStopBit)
        {
            profile.camera[i].ptzinfo.iStopBit=dlgdata.camera[i].ptzinfo.iStopBit;
        }

        if(dlgdata.camera[i].ptzinfo.iParityBit!=profile.camera[i].ptzinfo.iParityBit)
        {
            profile.camera[i].ptzinfo.iParityBit=dlgdata.camera[i].ptzinfo.iParityBit;
        }
    }

    if((iCameraNameChanged==1)||(iCameraEnableChanged==1))
    {
        pSidebar->setSideBarTreeControl();
        if(iCameraEnableChanged==1)
        {
#if DVR_API_ENABLE
            if(!profile.iSubOutputEnable)
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
            else
            {
                if(runtimesettings.iLayout*runtimesettings.iPageNum<16)
                    LIB816x_setCameraLayout(profile.layout.iMainOutput, runtimesettings.iLayout*runtimesettings.iPageNum, runtimesettings.iLayout);
            }
#endif
            pToolbar->MaxPageCalc();

        }
        pLayoutClass->DrawOSDChannel();
    }
}
/*!camera tree에 선택한 camera를 setting */
void dvr_setup::SetSetupCameraTreeControl()
{
    ui->cameratreeWidget->setCurrentItem (&SetupCameraTreeItem[m_iCameraSelectId]);
}
/*!현재 선택한 camera에대한 data를 ui에 setting */
void dvr_setup::SetSetupCameraButtonControls(int iChannel)
{
	//현재 camera data를 ui에 set 
    ui->cameraenable->setCurrentIndex(dlgdata.camera[iChannel].iEnable);
    ui->cameracovert->setCurrentIndex(dlgdata.camera[iChannel].iCovertOn);
    ui->motionenable->setCurrentIndex(dlgdata.camera[iChannel].motion.iMotionOn);
    ui->recordresolutioncomboBox->setCurrentIndex(dlgdata.camera[iChannel].camrec.iResolution);

    ui->cameratitlelineEdit->setText(QString::fromUtf8(dlgdata.camera[iChannel].strcameraname));

    ui->motionsensitivityslider->setSliderPosition(dlgdata.camera[iChannel].motion.iSensitivity);
    ui->motionsensitivityvalue->setText(SetMotionSensitivityValueText(dlgdata.camera[iChannel].motion.iSensitivity));

    MotionControlsEnable(dlgdata.camera[iChannel].motion.iMotionOn);

    ui->colorbrightnessspinBox->setValue(dlgdata.camera[iChannel].coloradjust.iBrightness);
    ui->colorcontrastspinBox->setValue(dlgdata.camera[iChannel].coloradjust.iContrast);
    ui->colorsaturationspinBox->setValue(dlgdata.camera[iChannel].coloradjust.iSaturation);


    if(dlgdata.camera[iChannel].ptzinfo.iPtzOn==0)
        ui->ptzenablecheckBox->setCheckState(Qt::Unchecked);
    else
        ui->ptzenablecheckBox->setCheckState(Qt::Checked);

    ui->ptzdevicecomboBox->setCurrentIndex(dlgdata.camera[iChannel].ptzinfo.iPtzdriverIndex);
    ui->ptzaddressspinBox->setValue(dlgdata.camera[iChannel].ptzinfo.iPtzAddress);

    ui->ptzbaudratecomboBox->setCurrentIndex(dlgdata.camera[iChannel].ptzinfo.iBaudRate);
    ui->ptzdatabitcomboBox->setCurrentIndex(dlgdata.camera[iChannel].ptzinfo.iDataBit);
    ui->ptzstopbitcomboBox->setCurrentIndex(dlgdata.camera[iChannel].ptzinfo.iStopBit);
    ui->ptzparitybitcomboBox->setCurrentIndex(dlgdata.camera[iChannel].ptzinfo.iParityBit);


    QString qsignaltype;

    if(dlgruntimesettings.chStatus[iChannel].frameHeight==240)
    {
        qsignaltype=StrToQString((char*)STRINPUTVIDEOSIGNALTYPE_NTSC);
    }
    else
    {
        qsignaltype=StrToQString((char*)STRINPUTVIDEOSIGNALTYPE_PAL);
    }
    ui->inputsignaltypevalue->setText(qsignaltype);
    SetMotionAreaTable(iChannel);
}
/*!motion의 sensitivity에 대한 값을 text 표시 */
QString dvr_setup::SetMotionSensitivityValueText(int ivalue)
{
    QString qstrvalue;
    char sensitivitybuf[MAX_CHAR_8];
    memset(sensitivitybuf,0,sizeof(sensitivitybuf));
    switch(ivalue)
    {
    case 0:
        sprintf(sensitivitybuf,"%s","Low");
        break;
    case 1:
        sprintf(sensitivitybuf,"%s","Medium");
        break;
    case 2:
        sprintf(sensitivitybuf,"%s","High");
        break;
    }
    qstrvalue=StrToQString(sensitivitybuf);

    return qstrvalue;
}
/*!현재 camera에 대한 motion table을 setting */
void dvr_setup::SetMotionAreaTable(int iChannel)
{
    int i=0;
    int j=0;

    for(j=0;j<ui->motionareatableWidget->rowCount();j++)
    {
        for(i=0;i<ui->motionareatableWidget->columnCount();i++)
        {
              ui->motionareatableWidget->takeItem(j,i);
        }
    }

    ui->motionareatableWidget->setRowCount(m_iMotionBlockH);
    ui->motionareatableWidget->setColumnCount(m_iMotionBlockW);

    for(i=0;i<ui->motionareatableWidget->columnCount();i++)
    {
        ui->motionareatableWidget->setColumnWidth(i,m_iMotionAreaW/m_iMotionBlockW);
    }

    for(i=0;i<ui->motionareatableWidget->rowCount();i++)
    {
        ui->motionareatableWidget->setRowHeight(i,m_iMotionAreaH/m_iMotionBlockH);
    }

    for(j=0;j<ui->motionareatableWidget->rowCount();j++)
    {
        for(i=0;i<ui->motionareatableWidget->columnCount();i++)
        {
            m_motionitem[j][i].setBackgroundColor(QColor(255, 0, 255, 255));
            if(dlgdata.camera[iChannel].motion.motiontable[(j*MOTION_W_CELL)+i]==0)
            {
                m_motionitem[j][i].setText("");
            }
            else
            {
                m_motionitem[j][i].setText("o");
            }
            m_motionitem[j][i].setTextColor(m_MotionBlockColor[m_iMotionAreaMode]);
            ui->motionareatableWidget->setItem(j, i,&m_motionitem[j][i]);
        }
    }
}
/*!motion control의 enable/disable을 변경 */
void dvr_setup::MotionControlsEnable(int iEnable)
{
	//motion enable/disable 선택에 따라 다른 motion control enable/disable하고
	//PIP상에 motion table표시를 show/hide 처리 
    if(iEnable==0)
    {
        ui->motionsensitivityslider->setEnabled(false);
        ui->motionareamodecomboBox->setEnabled(false);
        ui->motionareaallclearpushButton->setEnabled(false);
        ui->motionareaallsetpushButton->setEnabled(false);
        ui->motionareatableWidget->hide();
    }
    else
    {
        ui->motionsensitivityslider->setEnabled(true);
        ui->motionareamodecomboBox->setEnabled(true);
        ui->motionareaallclearpushButton->setEnabled(true);
        ui->motionareaallsetpushButton->setEnabled(true);
        ui->motionareatableWidget->show();
    }
}

/*!현재 선택 channel에 대해 ui value를 읽음 */
void dvr_setup::GetCameraChanneldataToDlgData(int iChannel)
{
	//현재 camera에 대한 ui data를 읽음
    dlgdata.camera[iChannel].iEnable = ui->cameraenable->currentIndex();
    strcpy(dlgdata.camera[iChannel].strcameraname,QStringToStr(ui->cameratitlelineEdit->text()));
    dlgdata.camera[iChannel].iCovertOn = ui->cameracovert->currentIndex();
    dlgdata.camera[iChannel].camrec.iResolution=ui->recordresolutioncomboBox->currentIndex();
    dlgdata.camera[iChannel].motion.iMotionOn = ui->motionenable->currentIndex();

    dlgdata.camera[iChannel].motion.iSensitivity=ui->motionsensitivityslider->sliderPosition();

    dlgdata.camera[iChannel].coloradjust.iBrightness=ui->colorbrightnessspinBox->value();
    dlgdata.camera[iChannel].coloradjust.iContrast=ui->colorcontrastspinBox->value();
    dlgdata.camera[iChannel].coloradjust.iSaturation=ui->colorsaturationspinBox->value();

    if(ui->ptzenablecheckBox->checkState()==Qt::Unchecked)
    {
        dlgdata.camera[iChannel].ptzinfo.iPtzOn=0;
    }
    else if(ui->ptzenablecheckBox->checkState()==Qt::Checked)
    {
        dlgdata.camera[iChannel].ptzinfo.iPtzOn=1;
    }

    dlgdata.camera[iChannel].ptzinfo.iPtzdriverIndex=ui->ptzdevicecomboBox->currentIndex();
    dlgdata.camera[iChannel].ptzinfo.iPtzAddress=ui->ptzaddressspinBox->value();
    dlgdata.camera[iChannel].ptzinfo.iBaudRate=ui->ptzbaudratecomboBox->currentIndex();
    dlgdata.camera[iChannel].ptzinfo.iDataBit=ui->ptzdatabitcomboBox->currentIndex();
    dlgdata.camera[iChannel].ptzinfo.iStopBit=ui->ptzstopbitcomboBox->currentIndex();
    dlgdata.camera[iChannel].ptzinfo.iParityBit=ui->ptzparitybitcomboBox->currentIndex();
}
/*!camera tree item이 click될때 호출 */
void dvr_setup::SetupCameraTreeItemClicked(QTreeWidgetItem *item, int column)
{
	//선택 camera변경시 현재 ui 설정을 읽어 저장하고,
	//변경한 camera의 data로 ui에 set 
    GetCameraChanneldataToDlgData(m_iCameraSelectId);
    m_iCameraSelectId=ui->cameratreeWidget->indexOfTopLevelItem(item);
    SetSetupCameraButtonControls(m_iCameraSelectId);
#if DVR_API_ENABLE
    LIB816x_selCamPropCh(m_iCameraSelectId);
#endif
}
/*!camera tree item이 double click될때 호출 */
void dvr_setup::SetupCameraTreeItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    SetupCameraTreeItemClicked(item,column);
}
/*!motion enable combobox 값이 변경될때 호출 */
void dvr_setup::on_motionenable_currentIndexChanged(int index)
{
    if(index==0)
    {
        MotionControlsEnable(0);
    }
    else
    {
        MotionControlsEnable(1);
    }
}
/*!motion sensitivity slide값이 변경될때 호출 */
void dvr_setup::ChangeMotionSensitivitySlot(int value)
{
    ui->motionsensitivityvalue->setText(SetMotionSensitivityValueText(value));
}
/*!motion area mode combo box가 변경될때 호출 */
void dvr_setup::ChangeMotionAreaModeSlot(int index)
{
    m_iMotionAreaMode=index;
}
/*!motion area mode all clear 버튼이 눌릴때 호출 */
void dvr_setup::MotionAreaAllClearModeBtnSlot()
{
    for(int i=0;i<MAX_MOTION_CELL;i++)
        dlgdata.camera[m_iCameraSelectId].motion.motiontable[i]=0;
    bMotionTableChanged[m_iCameraSelectId]=true;
    SetMotionAreaTable(m_iCameraSelectId);
}
/*!motion area mode all set 버튼이 눌릴때 호출 */
void dvr_setup::MotionAreaAllSetModeBtnSlot()
{
    for(int i=0;i<MAX_MOTION_CELL;i++)
        dlgdata.camera[m_iCameraSelectId].motion.motiontable[i]=1;
    bMotionTableChanged[m_iCameraSelectId]=true;
    SetMotionAreaTable(m_iCameraSelectId);
}
/*!motion table이 click될때 호출 */
void dvr_setup::MotionAreaSelectSlot ( int row, int column )
{
    dprintf("motion block select col=%d,r=%d\n",column,row);
    //Motion table click시 SET,CLEAR mode에 따라서 motion table을 update
    ui->motionareatableWidget->takeItem(row,column);

    if(m_iMotionAreaMode==0)
    {
        m_motionitem[row][column].setText("");
    }
    else
    {
        m_motionitem[row][column].setText("o");
    }
    bMotionTableChanged[m_iCameraSelectId]=true;
    dlgdata.camera[m_iCameraSelectId].motion.motiontable[(row*MOTION_W_CELL)+column]=m_iMotionAreaMode;
    ui->motionareatableWidget->setItem(row, column,&m_motionitem[row][column]);
}
/*!color default 버튼이 눌릴때 호출 */
void dvr_setup::ColorDefaultBtnSlot()
{
    dlgdata.camera[m_iCameraSelectId].coloradjust.iBrightness=BRIGHTNESS_VALUE_DEFAULT;
    dlgdata.camera[m_iCameraSelectId].coloradjust.iContrast=CONTRAST_VALUE_DEFAULT;
    dlgdata.camera[m_iCameraSelectId].coloradjust.iSaturation=SATURATION_VALUE_DEFAULT;

    ui->colorbrightnessspinBox->setValue(dlgdata.camera[m_iCameraSelectId].coloradjust.iBrightness);
    ui->colorcontrastspinBox->setValue(dlgdata.camera[m_iCameraSelectId].coloradjust.iContrast);
    ui->colorsaturationspinBox->setValue(dlgdata.camera[m_iCameraSelectId].coloradjust.iSaturation);
}
/*!apply all ch 버튼이 눌릴때 호출 */
void dvr_setup::on_applyallchpushButton_clicked()
{
	//현재 ui 설정을 읽어, 모든 camera에 동일한 설정을 적용
    dlgdata.camera[0].iEnable = ui->cameraenable->currentIndex();
    dlgdata.camera[0].iCovertOn = ui->cameracovert->currentIndex();
    dlgdata.camera[0].camrec.iResolution=ui->recordresolutioncomboBox->currentIndex();
    dlgdata.camera[0].motion.iMotionOn = ui->motionenable->currentIndex();

    dlgdata.camera[0].motion.iSensitivity=ui->motionsensitivityslider->sliderPosition();
    memcpy(dlgdata.camera[0].motion.motiontable,dlgdata.camera[m_iCameraSelectId].motion.motiontable,MAX_MOTION_CELL);
    bMotionTableChanged[0]=true;

    dlgdata.camera[0].coloradjust.iBrightness=ui->colorbrightnessspinBox->value();
    dlgdata.camera[0].coloradjust.iContrast=ui->colorcontrastspinBox->value();
    dlgdata.camera[0].coloradjust.iSaturation=ui->colorsaturationspinBox->value();

    for(int i=1;i<MAX_INTERNAL_CAMERA;i++)
    {
        dlgdata.camera[i].iEnable = dlgdata.camera[0].iEnable;
        dlgdata.camera[i].iCovertOn = dlgdata.camera[0].iCovertOn;
        dlgdata.camera[i].camrec.iResolution=dlgdata.camera[0].camrec.iResolution;
        dlgdata.camera[i].motion.iMotionOn = dlgdata.camera[0].motion.iMotionOn;

        dlgdata.camera[i].motion.iSensitivity=dlgdata.camera[0].motion.iSensitivity;
        memcpy(dlgdata.camera[i].motion.motiontable,dlgdata.camera[m_iCameraSelectId].motion.motiontable,MAX_MOTION_CELL);
        bMotionTableChanged[i]=true;
        memcpy(&dlgdata.camera[i].coloradjust,&dlgdata.camera[0].coloradjust,sizeof(COLORADJUST));
    }
}
/*!camera name의 keyboard icon click시 호출 */
void dvr_setup::CameraTitleVirtualKeyboardBtnSlot()
{
	//virtual keyboard를 보여주고 camera name 입력 받을 준비를 한다
    if(pMainClass->ShowVirtualKeyboard())
        ui->cameratitlelineEdit->setFocus();
}
/*!color brightness keyboard icon click시 호출 */
void dvr_setup::ColorBrightnessVirtualKeyboardBtnSlot()
{
	//virtual keyboard를 보여주고 brightness 입력 받을 준비를 한다
    if(pMainClass->ShowVirtualKeyboard())
        ui->colorbrightnessspinBox->setFocus();
}
/*!contrast keyboard icon click시 호출 */
void dvr_setup::ColorContrastVirtualKeyboardBtnSlot()
{
	//virtual keyboard를 보여주고 contrast 입력 받을 준비를 한다
    if(pMainClass->ShowVirtualKeyboard())
        ui->colorcontrastspinBox->setFocus();
}
/*!saturation keyboard icon click시 호출 */
void dvr_setup::ColorSaturationVirtualKeyboardBtnSlot()
{
	//virtual keyboard를 보여주고 saturation 입력 받을 준비를 한다
    if(pMainClass->ShowVirtualKeyboard())
        ui->colorsaturationspinBox->setFocus();
}
