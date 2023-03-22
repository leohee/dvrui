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
/*!audio tab의 control들을 초기화 */
void dvr_setup::InitSetupAudioControls()
{
    m_iAudioInSelectId=0;
    QSize qsize;
    qsize.setWidth(AUDIOTREEICON_SIZE_W);
    qsize.setHeight(AUDIOTREEICON_SIZE_H);
    AudioInTreeItemIcon.addFile(QString::fromUtf8(AUDIO_TREE_CAMERA_ICON), qsize, QIcon::Normal, QIcon::Off);

    for(int i=0;i<MAX_AUDIOIN;i++)
    {
        char straudioinname[MAX_TITLE_CHAR];
        sprintf(straudioinname,"%s%d",STRAUDIOINNAME,i+1);
        SetupAudioInTreeItem[i].setIcon(0,AudioInTreeItemIcon);
        SetupAudioInTreeItem[i].setText(0,QString::fromUtf8(straudioinname));
        ui->audiointreeWidget->insertTopLevelItem(i,&SetupAudioInTreeItem[i]);
    }
    connect(ui->audiointreeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(SetupAudioInTreeItemClicked(QTreeWidgetItem *, int)));
    connect(ui->audiointreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(SetupAudioInTreeItemDoubleClicked(QTreeWidgetItem *, int)));

    connect(ui->audioinvolumeslider, SIGNAL(valueChanged(int)), this, SLOT(ChangeAudioInVolumeSlot(int)));
    connect(ui->audiooutvolumeslider, SIGNAL(valueChanged(int)), this, SLOT(ChangeAudioOutVolumeSlot(int)));
}
/*!profile data struct에 따라서 ui에 value setting */
void dvr_setup::SetAudiodata()
{
    SetSetupAudioInTreeControl();
    SetSetupAudioInButtonControls(m_iAudioInSelectId);

    ui->audiooutenable->setCurrentIndex(dlgdata.audioout.iAudioOutOn);
    ui->audiooutvolumeslider->setSliderPosition(dlgdata.audioout.iVolume);
    ui->audiooutvolumevalue->setText(SetAudioVolumeValueText(dlgdata.audioout.iVolume));
    ui->audiooutdev->setCurrentIndex(dlgdata.audioout.iDevId);
}
/*!ui control의 value를 읽고 해당 기능을 적용시킴 */
void dvr_setup::GetAudiodata()
{
    GetAudioInChanneldataToDlgData(m_iAudioInSelectId);
    dlgdata.audioin.iSampleRate=AUDIO_SAMPLERATE_16KHZ;
    dlgdata.audioout.iAudioOutOn=ui->audiooutenable->currentIndex();
    dlgdata.audioout.iVolume=ui->audiooutvolumeslider->sliderPosition();
    dlgdata.audioout.iDevId=ui->audiooutdev->currentIndex();

    for(int i=0;i<MAX_AUDIOIN;i++)
    {
        if((dlgdata.audioin.audioinch[i].iAudioInOn!=profile.audioin.audioinch[i].iAudioInOn) )
        {
            profile.audioin.audioinch[i].iAudioInOn=dlgdata.audioin.audioinch[i].iAudioInOn;
#if DVR_API_ENABLE
            LIB816x_setAudioInput(i, profile.audioin.audioinch[i].iAudioInOn);
#endif
        }
    }

    if(dlgdata.audioin.iSampleRate!=profile.audioin.iSampleRate || (dlgdata.audioin.iVolume!=profile.audioin.iVolume))
    {
        profile.audioin.iSampleRate=dlgdata.audioin.iSampleRate;
        profile.audioin.iVolume=dlgdata.audioin.iVolume;
#if DVR_API_ENABLE
        LIB816x_setAudioInputParams(profile.audioin.iSampleRate, profile.audioin.iVolume);
#endif
    }

    if(dlgdata.audioout.iDevId != profile.audioout.iDevId)
    {
        profile.audioout.iDevId = dlgdata.audioout.iDevId;
#if	DVR_API_ENABLE
        LIB816x_setAudioOutputDev(profile.audioout.iDevId);
#endif
    }

    if((dlgdata.audioout.iAudioOutOn!=profile.audioout.iAudioOutOn)
        || (dlgdata.audioout.iSelectAudioIn!=profile.audioout.iSelectAudioIn)
        || (dlgdata.audioout.iVolume!=profile.audioout.iVolume))
    {
        profile.audioout.iAudioOutOn=dlgdata.audioout.iAudioOutOn;
        profile.audioout.iSelectAudioIn=dlgdata.audioout.iSelectAudioIn;
        profile.audioout.iVolume=dlgdata.audioout.iVolume;
#if	DVR_API_ENABLE
        if(runtimesettings.iLivePlaybackMode==LIB_LIVE_MODE)
            LIB816x_setAudioOutput(profile.audioout.iAudioOutOn, profile.audioout.iVolume, profile.iSelectChannel);
        else
            LIB816x_setAudioOutput(profile.audioout.iAudioOutOn, profile.audioout.iVolume, MAX_CAMERA+profile.iSecondSelectChannel);
#endif
    }
}
/*!cancel시 audio관련 변경적용된 내역을 복원시킴 */
void dvr_setup::ResotreAudio()
{
    for(int i=0;i<MAX_AUDIOIN;i++)
    {
        if((dlgdata.audioin.audioinch[i].iAudioInOn!=profile.audioin.audioinch[i].iAudioInOn) )
        {
            profile.audioin.audioinch[i].iAudioInOn=dlgdata.audioin.audioinch[i].iAudioInOn;
#if DVR_API_ENABLE
            LIB816x_setAudioInput(i, profile.audioin.audioinch[i].iAudioInOn);
#endif
        }
    }

    if(dlgdata.audioin.iSampleRate!=profile.audioin.iSampleRate || (dlgdata.audioin.iVolume!=profile.audioin.iVolume))
    {
        profile.audioin.iSampleRate=dlgdata.audioin.iSampleRate;
        profile.audioin.iVolume=dlgdata.audioin.iVolume;
#if DVR_API_ENABLE
        LIB816x_setAudioInputParams(profile.audioin.iSampleRate, profile.audioin.iVolume);
#endif
    }

    if(dlgdata.audioout.iDevId != profile.audioout.iDevId)
    {
        profile.audioout.iDevId = dlgdata.audioout.iDevId;
#if	DVR_API_ENABLE
        LIB816x_setAudioOutputDev(profile.audioout.iDevId);
#endif
    }

    if((dlgdata.audioout.iAudioOutOn!=profile.audioout.iAudioOutOn)
        || (dlgdata.audioout.iSelectAudioIn!=profile.audioout.iSelectAudioIn)
        || (dlgdata.audioout.iVolume!=profile.audioout.iVolume))
    {
        profile.audioout.iAudioOutOn=dlgdata.audioout.iAudioOutOn;
        profile.audioout.iSelectAudioIn=dlgdata.audioout.iSelectAudioIn;
        profile.audioout.iVolume=dlgdata.audioout.iVolume;
#if	DVR_API_ENABLE
        if(runtimesettings.iLivePlaybackMode==LIB_LIVE_MODE)
            LIB816x_setAudioOutput(profile.audioout.iAudioOutOn, profile.audioout.iVolume, profile.iSelectChannel);
        else
            LIB816x_setAudioOutput(profile.audioout.iAudioOutOn, profile.audioout.iVolume, MAX_CAMERA+profile.iSecondSelectChannel);
#endif
    }
}
/*!audio input tree에서 현재 선택된 input을 설정 */
void dvr_setup::SetSetupAudioInTreeControl()
{
    ui->audiointreeWidget->setCurrentItem (&SetupAudioInTreeItem[m_iAudioInSelectId]);
}
/*!현재 audio input에 대한 data에 따라서 ui를 setting함 */
void dvr_setup::SetSetupAudioInButtonControls(int iChannel)
{
    ui->audioinenable->setCurrentIndex(dlgdata.audioin.audioinch[iChannel].iAudioInOn);
    ui->audioinvolumeslider->setSliderPosition(dlgdata.audioin.iVolume);
    ui->audioinvolumevalue->setText(SetAudioVolumeValueText(dlgdata.audioin.iVolume));
}
/*!ui로부터 현재 audio channel에 대한 value를 읽음 */
void dvr_setup::GetAudioInChanneldataToDlgData(int iChannel)
{
    dlgdata.audioin.audioinch[iChannel].iAudioInOn=ui->audioinenable->currentIndex();
    dlgdata.audioin.iVolume=ui->audioinvolumeslider->sliderPosition();
}
/*!audio volume text표시 */
QString dvr_setup::SetAudioVolumeValueText(int ivalue)
{
    QString qstrvalue;
    char audiovolbuf[MAX_CHAR_8];
    sprintf(audiovolbuf,"%s %d ",STRVOL,ivalue);

    qstrvalue=StrToQString(audiovolbuf);

    return qstrvalue;
}
/*!audio in volume slide가 변경시 호출 */
void dvr_setup::ChangeAudioInVolumeSlot(int value)
{
    ui->audioinvolumevalue->setText(SetAudioVolumeValueText(value));
}
/*!audio out volume slide가 변경시 호출 */
void dvr_setup::ChangeAudioOutVolumeSlot(int value)
{
    ui->audiooutvolumevalue->setText(SetAudioVolumeValueText(value));
}
/*!audio input tree가 click될때 호출 */
void dvr_setup::SetupAudioInTreeItemClicked(QTreeWidgetItem *item, int column)
{
    GetAudioInChanneldataToDlgData(m_iAudioInSelectId);
    m_iAudioInSelectId=ui->audiointreeWidget->indexOfTopLevelItem(item);
    SetSetupAudioInButtonControls(m_iAudioInSelectId);
}
/*!audio input tree가 double click될때 호출 */
void dvr_setup::SetupAudioInTreeItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    SetupAudioInTreeItemClicked(item,column);
}

