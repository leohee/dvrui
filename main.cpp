/**
****************************************************************************
* DM8168 Hybrid DVR
* Copyright by UDWorks, Incoporated. All Rights Reserved.
*-----------------------------------------------------------------------------
* @file	main.cpp
* @brief dvrmain의 entry point(main함수)로 dvrmain object 생성
* @author
* @section	MODIFY history
*     - 2011.01.01 : First Created
******************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <QApplication>
#include <QLabel>
//#include <QWSServer>
#include "dvr_defines.h"
#include "dvr_main.h"
#include "predefine.h"
#include "dvr_app_api.h"
#include <QtCore/QTextCodec>
/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
/*static unsigned char str_disp[][8] = {
	"HDMI0", "HDMI1", "VGA", "SD"
};
*/
/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
extern PROFILE  profile;
/*----------------------------------------------------------------------------
 local functions
-----------------------------------------------------------------------------*/
void ReadSettings()
{
    int i=0;

    int readsize=ReadDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));

    if(profile.layout.iMainOutput < DISP_PATH_HDMI0 || profile.layout.iMainOutput > DISP_PATH_HDMI1) readsize=0;
    if(profile.layout.iSubOutput < DISP_PATH_HDMI0 || profile.layout.iSubOutput > DISP_PATH_HDMI1) readsize=0;

    if(profile.layout.iLayoutMode<LAYOUTMODE_1 || profile.layout.iLayoutMode>LAYOUTMODE_16) readsize=0;
    if(profile.layout.iSecondLayoutMode<LAYOUTMODE_1 || profile.layout.iSecondLayoutMode>LAYOUTMODE_16) readsize=0;

    if(profile.sequence.iInterval<1 || profile.sequence.iInterval>60) readsize=0;
    if(profile.secondsequence.iInterval<1 || profile.secondsequence.iInterval>60) readsize=0;

    if(profile.mainosd.iBorderLineOn!=BORDERLINE_ON && profile.mainosd.iBorderLineOn!=BORDERLINE_OFF) readsize=0;
    if(profile.mainosd.iDateFormat<OSD_DATEFORMAT_MMDDYYYY_AP || profile.mainosd.iDateFormat>OSD_DATEFORMAT_YYYYMMDD) readsize=0;

    if(profile.spotdisplayoutput.iResolution!=OUTPUT_RESOLUTION_NTSC && profile.spotdisplayoutput.iResolution!=OUTPUT_RESOLUTION_PAL) readsize=0;
    if(profile.iSpotOutputCh<0 && profile.iSpotOutputCh>=MAX_INTERNAL_CAMERA) readsize=0;

    if(profile.maindisplayoutput.iResolution<OUTPUT_RESOLUTION_1080P || profile.maindisplayoutput.iResolution>OUTPUT_RESOLUTION_576P) readsize=0;
    if(profile.subdisplayoutput.iResolution<OUTPUT_RESOLUTION_1080P && profile.subdisplayoutput.iResolution>OUTPUT_RESOLUTION_576P) readsize=0;

    for(i=0;i<MAX_INTERNAL_CAMERA;i++)
    {
        if(profile.camera[i].iEnable!=1 && profile.camera[i].iEnable!=0) readsize=0;
        if(profile.camera[i].iCameraType!=CAMERA_TYPE_INTERNAL) readsize=0;
        if(profile.camera[i].iCovertOn!=0 && profile.camera[i].iCovertOn!=1) readsize=0;
        if(profile.camera[i].coloradjust.iBrightness<BRIGHTNESS_VALUE_MIN || profile.camera[i].coloradjust.iBrightness>BRIGHTNESS_VALUE_MAX) readsize=0;
        if(profile.camera[i].coloradjust.iContrast<CONTRAST_VALUE_MIN || profile.camera[i].coloradjust.iContrast>CONTRAST_VALUE_MAX) readsize=0;
        if(profile.camera[i].coloradjust.iSaturation<SATURATION_VALUE_MIN || profile.camera[i].coloradjust.iSaturation>SATURATION_VALUE_MAX) readsize=0;

        if(profile.camera[i].strcameraname[MAX_TITLE_CHAR-1]!='\0') readsize=0;

        if(profile.playbackcamera[i].iEnable!=1 && profile.playbackcamera[i].iEnable!=0) readsize=0;

        if(profile.camera[i].ptzinfo.iPtzOn!=0 && profile.camera[i].ptzinfo.iPtzOn!=1) readsize=0;
        if(profile.camera[i].ptzinfo.iPtzdriverIndex!=PTZCTRL_DRX_500 && profile.camera[i].ptzinfo.iPtzdriverIndex!=PTZCTRL_PELCO_D) readsize=0;
        if(profile.camera[i].ptzinfo.iBaudRate<BAUDRATE_1200 || profile.camera[i].ptzinfo.iBaudRate>BAUDRATE_115200) readsize=0;
        if(profile.camera[i].ptzinfo.iDataBit!=DATBIT_8 && profile.camera[i].ptzinfo.iDataBit!=DATBIT_7) readsize=0;
        if(profile.camera[i].ptzinfo.iStopBit!=STOPBIT_1 && profile.camera[i].ptzinfo.iStopBit!=STOPBIT_2) readsize=0;
        if(profile.camera[i].ptzinfo.iParityBit<PARITYBIT_NONE || profile.camera[i].ptzinfo.iParityBit>PARITYBIT_EVEN) readsize=0;

        if(profile.camera[i].camrec.iRecordingOn!=1 && profile.camera[i].camrec.iRecordingOn!=0) readsize=0;
        if(profile.camera[i].camrec.iCodecType<CODECTYPE_H264 || profile.camera[i].camrec.iCodecType>CODECTYPE_MJPEG) readsize=0;
        if(profile.camera[i].camrec.iResolution<RESOLUTION_D1 || profile.camera[i].camrec.iResolution>RESOLUTION_720P) readsize=0;
        if(profile.camera[i].camrec.iFps<0 || profile.camera[i].camrec.iFps>3) readsize=0;
        if(profile.camera[i].camrec.ikbps<500 || profile.camera[i].camrec.ikbps>4000) readsize=0;
        if(profile.camera[i].camrec.iBitrateType!=BITRATE_TYPE_CBR && profile.camera[i].camrec.iBitrateType!=BITRATE_TYPE_VBR) readsize=0;
        if(profile.camera[i].camrec.iIFrameInterval<IFRAME_INTERVAL_30 || profile.camera[i].camrec.iIFrameInterval>IFRAME_INTERVAL_1) readsize=0;
        if(profile.camera[i].camrec.InputVideoSignalType!=INPUT_SIGNALTYPE_NTSC && profile.camera[i].camrec.InputVideoSignalType!=INPUT_SIGNALTYPE_PAL) readsize=0;

        if(profile.camera[i].camrec.iAudioOn!=0 && profile.camera[i].camrec.iAudioOn!=1) readsize=0;
        if(profile.camera[i].camrec.iAudioCodecType!=AUDIO_CODEC_G711 && profile.camera[i].camrec.iAudioCodecType!=AUDIO_CODEC_AAC) readsize=0;
        if(profile.camera[i].camrec.iAudioSamplingType!=AUDIO_SAMPLERATE_8KHZ && profile.camera[i].camrec.iAudioSamplingType!=AUDIO_SAMPLERATE_16KHZ) readsize=0;

        if(profile.camera[i].recordduration.iPrevRecordOn!=0 && profile.camera[i].recordduration.iPrevRecordOn!=1) readsize=0;
        if(profile.camera[i].recordduration.iPrevRecordDuration<1 || profile.camera[i].recordduration.iPrevRecordDuration>10) readsize=0;
        if(profile.camera[i].recordduration.iPostRecordOn!=0 && profile.camera[i].recordduration.iPostRecordOn!=1) readsize=0;
        if(profile.camera[i].recordduration.iPostRecordDuration<1 || profile.camera[i].recordduration.iPostRecordDuration>60) readsize=0;

    }

    for(i=MAX_INTERNAL_CAMERA;i<MAX_CAMERA;i++)
    {
        if(profile.camera[i].iEnable!=1 && profile.camera[i].iEnable!=0) readsize=0;
        if(profile.camera[i].iCameraType!=CAMERA_TYPE_IPNC) readsize=0;
        if(profile.camera[i].iCovertOn!=0 && profile.camera[i].iCovertOn!=1) readsize=0;
        if(profile.camera[i].coloradjust.iBrightness<BRIGHTNESS_VALUE_MIN || profile.camera[i].coloradjust.iBrightness>BRIGHTNESS_VALUE_MAX) readsize=0;
        if(profile.camera[i].coloradjust.iContrast<CONTRAST_VALUE_MIN || profile.camera[i].coloradjust.iContrast>CONTRAST_VALUE_MAX) readsize=0;
        if(profile.camera[i].coloradjust.iSaturation<SATURATION_VALUE_MIN || profile.camera[i].coloradjust.iSaturation>SATURATION_VALUE_MAX) readsize=0;

        if(profile.camera[i].strcameraname[MAX_TITLE_CHAR-1]!='\0') readsize=0;

        if(profile.playbackcamera[i].iEnable!=1 && profile.playbackcamera[i].iEnable!=0) readsize=0;

        if(profile.camera[i].ptzinfo.iPtzOn!=0 && profile.camera[i].ptzinfo.iPtzOn!=1) readsize=0;
        if(profile.camera[i].ptzinfo.iPtzdriverIndex!=PTZCTRL_DRX_500 && profile.camera[i].ptzinfo.iPtzdriverIndex!=PTZCTRL_PELCO_D) readsize=0;
        if(profile.camera[i].ptzinfo.iBaudRate<BAUDRATE_1200 || profile.camera[i].ptzinfo.iBaudRate>BAUDRATE_115200) readsize=0;
        if(profile.camera[i].ptzinfo.iDataBit!=DATBIT_8 && profile.camera[i].ptzinfo.iDataBit!=DATBIT_7) readsize=0;
        if(profile.camera[i].ptzinfo.iStopBit!=STOPBIT_1 && profile.camera[i].ptzinfo.iStopBit!=STOPBIT_2) readsize=0;
        if(profile.camera[i].ptzinfo.iParityBit<PARITYBIT_NONE || profile.camera[i].ptzinfo.iParityBit>PARITYBIT_EVEN) readsize=0;

        if(profile.camera[i].camrec.iRecordingOn!=1 && profile.camera[i].camrec.iRecordingOn!=0) readsize=0;
        if(profile.camera[i].camrec.iCodecType<CODECTYPE_H264 || profile.camera[i].camrec.iCodecType>CODECTYPE_MJPEG) readsize=0;
        if(profile.camera[i].camrec.iResolution<RESOLUTION_D1 || profile.camera[i].camrec.iResolution>RESOLUTION_720P) readsize=0;
        if(profile.camera[i].camrec.iFps<0 || profile.camera[i].camrec.iFps>3) readsize=0;
        if(profile.camera[i].camrec.ikbps<500 || profile.camera[i].camrec.ikbps>4000) readsize=0;
        if(profile.camera[i].camrec.iBitrateType!=BITRATE_TYPE_CBR && profile.camera[i].camrec.iBitrateType!=BITRATE_TYPE_VBR) readsize=0;
        if(profile.camera[i].camrec.iIFrameInterval<IFRAME_INTERVAL_30 || profile.camera[i].camrec.iIFrameInterval>IFRAME_INTERVAL_1) readsize=0;
        if(profile.camera[i].camrec.InputVideoSignalType!=INPUT_SIGNALTYPE_NTSC && profile.camera[i].camrec.InputVideoSignalType!=INPUT_SIGNALTYPE_PAL) readsize=0;

        if(profile.camera[i].camrec.iAudioOn!=0 && profile.camera[i].camrec.iAudioOn!=1) readsize=0;
        if(profile.camera[i].camrec.iAudioCodecType!=AUDIO_CODEC_G711 && profile.camera[i].camrec.iAudioCodecType!=AUDIO_CODEC_AAC) readsize=0;
        if(profile.camera[i].camrec.iAudioSamplingType!=AUDIO_SAMPLERATE_8KHZ && profile.camera[i].camrec.iAudioSamplingType!=AUDIO_SAMPLERATE_16KHZ) readsize=0;

        if(profile.camera[i].recordduration.iPrevRecordOn!=0 && profile.camera[i].recordduration.iPrevRecordOn!=1) readsize=0;
        if(profile.camera[i].recordduration.iPrevRecordDuration<1 || profile.camera[i].recordduration.iPrevRecordDuration>10) readsize=0;
        if(profile.camera[i].recordduration.iPostRecordOn!=0 && profile.camera[i].recordduration.iPostRecordOn!=1) readsize=0;
        if(profile.camera[i].recordduration.iPostRecordDuration<1 || profile.camera[i].recordduration.iPostRecordDuration>60) readsize=0;
    }

    if(profile.audioin.iSampleRate!=AUDIO_SAMPLERATE_8KHZ && profile.audioin.iSampleRate!=AUDIO_SAMPLERATE_16KHZ) readsize=0;
    if(profile.audioin.iVolume<0 || profile.audioin.iVolume>AUDIO_VOLUME_MAX) readsize=0;
    for(i=0;i<MAX_AUDIOIN;i++)
    {
        if(profile.audioin.audioinch[i].iAudioInOn!=0 && profile.audioin.audioinch[i].iAudioInOn!=1) readsize=0;
    }

    if(profile.audioout.iAudioOutOn!=0 && profile.audioout.iAudioOutOn!=1) readsize=0;
    if(profile.audioout.iSelectAudioIn<0 || profile.audioout.iSelectAudioIn>15) readsize=0;
    if(profile.audioout.iVolume<0 || profile.audioout.iVolume>AUDIO_VOLUME_MAX) readsize=0;
    if(profile.audioout.iStereo!=AUDIO_MONO && profile.audioout.iStereo!=AUDIO_STEREO) readsize=0;
    if(profile.audioout.iDevId!=0 && profile.audioout.iDevId!=1) readsize=0;


    for(i=0;i<MAX_SENSOR;i++)
    {
        if(profile.sensor[i].iSensorOn!=SENSOR_DETECT_ON && profile.sensor[i].iSensorOn!=SENSOR_DETECT_OFF) readsize=0;
        if(profile.sensor[i].iSensorType!=SENSOR_TYPE_NO && profile.sensor[i].iSensorType!=SENSOR_TYPE_NC) readsize=0;
    }

    for(i=0;i<MAX_ALARM;i++)
    {
        if(profile.alarm[i].iAlarmOn!=ALARM_DETECT_ON && profile.alarm[i].iAlarmOn!=ALARM_DETECT_OFF) readsize=0;
        if(profile.alarm[i].iAlarmType!=ALARM_TYPE_NO && profile.alarm[i].iAlarmType!=ALARM_TYPE_NC) readsize=0;
        if(profile.alarm[i].iDelayTime<ALARM_DELAYSEC_MIN || profile.alarm[i].iDelayTime>ALARM_DELAYSEC_MAX) readsize=0;
    }

    for(i=0;i<MAX_INTERNAL_CAMERA;i++)
    {
        for(int iday=0;iday<7;iday++)
        {
            for(int ihour=0;ihour<24;ihour++)
            {
                if(profile.camera[i].recschedule.ScheduleTable[iday][ihour] < REC_SCHEDULE_CONTINUOUSRECORD || profile.camera[i].recschedule.ScheduleTable[iday][ihour]>REC_SCHEDULE_NORECORD) readsize=0;
            }
        }

    }

    if(profile.storage.iHddRecylceModeOn!=STORAGE_WRITE_MODE_RECYCLE && profile.storage.iHddRecylceModeOn!=STORAGE_WRITE_MODE_ONCE) readsize=0;
    if(profile.dvrall.iRecordEmergencyOn!=1 && profile.dvrall.iRecordEmergencyOn!=0) readsize=0;
    if(profile.iSelectChannel<0 || profile.iSelectChannel>=16)  readsize=0;
    if(profile.iSecondSelectChannel<0 || profile.iSecondSelectChannel>=16) readsize=0;

    if((readsize==0)||(readsize!=sizeof(PROFILE)))
    {
        DeleteDVRsettingInfo((char*)PROFILEFILNAME);

        memset(&profile,0,sizeof(PROFILE));

        profile.layout.iMainOutput = DISP_PATH_HDMI0;
        profile.layout.iSubOutput = DISP_PATH_HDMI1;
        profile.iSpotOutputCh=0;

        profile.layout.iLayoutMode=LAYOUTMODE_16;
        profile.layout.iSecondLayoutMode=LAYOUTMODE_16;
        profile.iSubOutputEnable = 1;
        profile.sequence.iSequenceModeOn=0;
        profile.secondsequence.iSequenceModeOn=0;
        profile.sequence.iInterval=DEFAULT_SEQUENCE_INTERVAL;
        profile.secondsequence.iInterval=DEFAULT_SEQUENCE_INTERVAL;

        profile.mainosd.iBorderLineOn=BORDERLINE_ON;
        profile.mainosd.iDateFormat=OSD_DATEFORMAT_MMDDYYYY_AP;

        profile.spotdisplayoutput.iResolution=OUTPUT_RESOLUTION_NTSC;
        profile.maindisplayoutput.iResolution=OUTPUT_RESOLUTION_1080P;
        profile.subdisplayoutput.iResolution=OUTPUT_RESOLUTION_1080P;

        for(i=0;i<MAX_INTERNAL_CAMERA;i++)
        {
            profile.camera[i].iEnable=1;
            profile.camera[i].iCameraType=CAMERA_TYPE_INTERNAL;
            profile.camera[i].iCovertOn=0;
            profile.camera[i].coloradjust.iBrightness=BRIGHTNESS_VALUE_DEFAULT;
            profile.camera[i].coloradjust.iContrast=CONTRAST_VALUE_DEFAULT;
            profile.camera[i].coloradjust.iSaturation=SATURATION_VALUE_DEFAULT;

            sprintf(profile.camera[i].strcameraname,"%s%d",STRCAMERANAME,i+1);

            profile.playbackcamera[i].iEnable=1;

            profile.camera[i].ptzinfo.iPtzOn=0;
            profile.camera[i].ptzinfo.iPtzdriverIndex=PTZCTRL_DRX_500;
            profile.camera[i].ptzinfo.iPtzAddress=0;
            profile.camera[i].ptzinfo.iBaudRate=BAUDRATE_9600;
            profile.camera[i].ptzinfo.iDataBit=DATBIT_8;
            profile.camera[i].ptzinfo.iStopBit=STOPBIT_1;
            profile.camera[i].ptzinfo.iParityBit=PARITYBIT_NONE;

            profile.camera[i].camrec.iRecordingOn=1;
            profile.camera[i].camrec.iCodecType=CODECTYPE_H264;
            profile.camera[i].camrec.iResolution=RESOLUTION_D1;
            profile.camera[i].camrec.iFps=3;
            profile.camera[i].camrec.ikbps=1200;
            profile.camera[i].camrec.iBitrateType=BITRATE_TYPE_CBR;
            profile.camera[i].camrec.iIFrameInterval=IFRAME_INTERVAL_30;
            profile.camera[i].camrec.InputVideoSignalType=INPUT_SIGNALTYPE_NTSC;

            profile.camera[i].camrec.iAudioOn=1;
            profile.camera[i].camrec.iAudioCodecType=AUDIO_CODEC_G711;
            profile.camera[i].camrec.iAudioSamplingType=AUDIO_SAMPLERATE_16KHZ;

            profile.camera[i].recordduration.iPrevRecordOn=0;
            profile.camera[i].recordduration.iPrevRecordDuration=PREVRECORDDURATION_SEC_DEFAULT;
            profile.camera[i].recordduration.iPostRecordOn=0;
            profile.camera[i].recordduration.iPostRecordDuration=POSTRECORDDURATION_MINUTE_DEFALUT;

        }

        for(i=MAX_INTERNAL_CAMERA;i<MAX_CAMERA;i++)
        {
            profile.camera[i].iEnable=0;
            profile.camera[i].iCameraType=CAMERA_TYPE_IPNC;
            profile.camera[i].iCovertOn=0;
            profile.camera[i].coloradjust.iBrightness=BRIGHTNESS_VALUE_DEFAULT;
            profile.camera[i].coloradjust.iContrast=CONTRAST_VALUE_DEFAULT;
            profile.camera[i].coloradjust.iSaturation=SATURATION_VALUE_DEFAULT;
            sprintf(profile.camera[i].strcameraname,"%s%d",STRIPNCCAMERANAME,i%MAX_INTERNAL_CAMERA+1);

            profile.playbackcamera[i].iEnable=0;

            profile.camera[i].ptzinfo.iPtzOn=0;
            profile.camera[i].ptzinfo.iPtzdriverIndex=PTZCTRL_DRX_500;
            profile.camera[i].ptzinfo.iPtzAddress=0;
            profile.camera[i].ptzinfo.iBaudRate=BAUDRATE_9600;
            profile.camera[i].ptzinfo.iDataBit=DATBIT_8;
            profile.camera[i].ptzinfo.iStopBit=STOPBIT_1;
            profile.camera[i].ptzinfo.iParityBit=PARITYBIT_NONE;

            profile.camera[i].camrec.iRecordingOn=1;
            profile.camera[i].camrec.iCodecType=CODECTYPE_H264;
            profile.camera[i].camrec.iResolution=RESOLUTION_D1;
            profile.camera[i].camrec.iFps=3;
            profile.camera[i].camrec.ikbps=1200;
            profile.camera[i].camrec.iBitrateType=BITRATE_TYPE_CBR;
            profile.camera[i].camrec.iIFrameInterval=IFRAME_INTERVAL_30;
            profile.camera[i].camrec.InputVideoSignalType=INPUT_SIGNALTYPE_NTSC;

            profile.camera[i].camrec.iAudioOn=0;
            profile.camera[i].camrec.iAudioCodecType=AUDIO_CODEC_G711;
            profile.camera[i].camrec.iAudioSamplingType=AUDIO_SAMPLERATE_8KHZ;

            profile.camera[i].recordduration.iPrevRecordOn=0;
            profile.camera[i].recordduration.iPrevRecordDuration=PREVRECORDDURATION_SEC_DEFAULT;
            profile.camera[i].recordduration.iPostRecordOn=0;
            profile.camera[i].recordduration.iPostRecordDuration=POSTRECORDDURATION_MINUTE_DEFALUT;
        }

        profile.audioin.iSampleRate=AUDIO_SAMPLERATE_16KHZ;
        profile.audioin.iVolume=AUDIO_VOLUME_DEFAULT;
        for(i=0;i<MAX_AUDIOIN;i++)
        {
            profile.audioin.audioinch[i].iAudioInOn=1;
        }

        profile.audioout.iAudioOutOn=1;
        profile.audioout.iSelectAudioIn=0;
        profile.audioout.iVolume=AUDIO_VOLUME_DEFAULT;
        profile.audioout.iStereo=AUDIO_MONO;
        profile.audioout.iDevId=0;


        for(i=0;i<MAX_SENSOR;i++)
        {
            profile.sensor[i].iSensorOn=SENSOR_DETECT_OFF;
            profile.sensor[i].iSensorType=SENSOR_TYPE_NO;
        }

        for(i=0;i<MAX_ALARM;i++)
        {
            profile.alarm[i].iAlarmOn=ALARM_DETECT_OFF;
            profile.alarm[i].iAlarmType=ALARM_TYPE_NO;
            profile.alarm[i].iDelayTime=ALARM_DELAYSEC_DEFAULT;
        }

        for(i=0;i<MAX_INTERNAL_CAMERA;i++)
        {
            for(int iday=0;iday<7;iday++)
            {
                for(int ihour=0;ihour<24;ihour++)
                {
                    profile.camera[i].recschedule.ScheduleTable[iday][ihour] = REC_SCHEDULE_NORECORD;
                }
            }

        }

        profile.storage.iHddRecylceModeOn=STORAGE_WRITE_MODE_RECYCLE;
        profile.dvrall.iRecordEmergencyOn=0;

        profile.net_info1.type=NETWORK_TYPE_STATIC;
        strcpy(profile.net_info1.ipaddr, NETWORK_IPADDRESS_DEFAULT1);
        strcpy(profile.net_info1.netmask, NETWORK_SUBNETMASK_DEFAULT);
        strcpy(profile.net_info1.gateway, NETWORK_GATEWAY_DEFAULT);

        profile.net_info2.type=NETWORK_TYPE_STATIC;
        strcpy(profile.net_info2.ipaddr, NETWORK_IPADDRESS_DEFAULT2);
        strcpy(profile.net_info2.netmask, NETWORK_SUBNETMASK_DEFAULT);
        strcpy(profile.net_info2.gateway, NETWORK_GATEWAY_DEFAULT);

        for(i=0; i<MAX_IPNC_CAMERA; i++)
        {
            strcpy(profile.ipnc[i].IpAddress, NETWORK_IPADDRESS_DEFAULT1);
            profile.ipnc[i].port = 0;
            memset(profile.ipnc[i].id,'\0',sizeof(profile.ipnc[i].id));
            memset(profile.ipnc[i].password,'\0',sizeof(profile.ipnc[i].password));
        }

        profile.iSelectChannel = 0;
        profile.iSecondSelectChannel = 0;

        WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));
    }
}

/*****************************************************************************
* @brief	main function
* @section	DESC Description
*	-
*****************************************************************************/
int main(int argc, char *argv[])
{
    int ntsc=CAM_PAL;

    ReadSettings();

    LIB816x_initSettingParam(&profile);
    LIB816x_startSystem(profile.layout.iMainOutput, profile.layout.iSubOutput, ntsc, DC_MODE_1080P_60);
    QApplication a(argc, argv);
    //QWSServer::setCursorVisible(FALSE);
    //QWSServer::setBackground(QBrush(Qt::black));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));


	QFont font("wenquanyi");
	font.setPointSize(160);
	//QFont::QFont ( const QString & family, int pointSize = -1, int weight = -1, bool italic = false )
      a.setFont(font);
	
    dvrmain Dvrmainwindow;
    Dvrmainwindow.show();
    return a.exec();
}
