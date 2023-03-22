#ifndef DVR_APP_API_H
#define DVR_APP_API_H

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define DVR_API_ENABLE		1

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
#if	DVR_API_ENABLE		//# use libdm816x.so ------------------------
#include "app_manager.h"

#else					//# null function ---------------------------
/*==================================================================*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*                              DEFINE                              */
/*==================================================================*/

#define LIB_MAIN_VERSION		"VER_00.07.00.02"

/*==============================*/
/*   		 FOR DISPLAY 		*/
/*==============================*/
#define LIB_MAIN_SWAPOFF				0
#define LIB_MAIN_SWAPON					1

#define LIB_WINDOW_FULLHD				0
#define LIB_WINDOW_SXGA					1

#define LIB_NORMAL_SCREEN_MODE			0
#define LIB_FULL_SCREEN_MODE			1

#define LIB_LIVE_MODE  					0
#define LIB_PLAYBACK_MODE				1

// Encoder Resolution
#define LIB_ENC_RESOLUTION_704x576		0
#define LIB_ENC_RESOLUTION_704x480		1
#define LIB_ENC_RESOLUTION_352x288		2
#define LIB_ENC_RESOLUTION_352x240		3
#define LIB_ENC_RESOLUTION_176x144		4
#define LIB_ENC_RESOLUTION_176x112		5

#define LIB_LAYOUTMODE_1				1
#define LIB_LAYOUTMODE_4				2
#define LIB_LAYOUTMODE_9				3
#define LIB_LAYOUTMODE_8				4
#define LIB_LAYOUTMODE_8_1				5
#define LIB_LAYOUTMODE_12				6
#define LIB_LAYOUTMODE_12_1				7
#define LIB_LAYOUTMODE_14				8
#define LIB_LAYOUTMODE_16				9
#define LIB_LAYOUTMODE_20				10
#define LIB_LAYOUTMODE_CAMFLOW			11

// Encoder Set Property
#define LIB_ENC_SET_FRAMERATE			12
#define LIB_ENC_SET_BITRATE				13
#define LIB_ENC_SET_RESOLUTION			14
#define LIB_ENC_SET_REQ_KEY_FRAME		15
#define LIB_ENC_SET_I_FRAME_INTERVAL	16

// Encoder Get Property
#define LIB_ENC_GET_FRAMERATE			17
#define LIB_ENC_GET_BITRATE				18
#define LIB_ENC_GET_RESOLUTION			19
#define LIB_ENC_GET_REQ_KEY_STATUS		20
#define LIB_ENC_GET_I_FRAME_INTERVAL	21

// Playback Command Table
#define LIB_PB_START					22
#define LIB_PB_STOP						23
#define LIB_PB_JUMP						24

// Playback Sub Command Table
#define LIB_PB_SUB_CMD_CH_ENABLE		30
#define LIB_PB_SUB_CMD_CH_SET_SPEED		31

// Playback speed steps
#define LIB_PB_SPEED_1X					1
#define LIB_PB_SPEED_2X					2
#define LIB_PB_SPEED_3X					3
#define LIB_PB_SPEED_4X					4
#define LIB_PB_SPEED_8X					5
#define LIB_PB_SPEED_16X				6
#define LIB_PB_SPEED_32X				7
#define LIB_PB_SPEED_0_25X				8
#define LIB_PB_SPEED_0_5X				9

// Playback Error Table
#define LIB_PB_NO_ERR					(0)
#define LIB_PB_START_FAILED				(-1)
#define LIB_PB_JUMP_FAILED				(-2)
#define LIB_PB_ALLOC_FAILED				(-3)
#define LIB_PB_BASKET_FAILED			(-4)
#define LIB_PB_OSAL_FAILED				(-5)
#define LIB_PB_READ_FRAME_FAILED		(-6)
#define LIB_PB_NOT_ALLOWED				(-7)

// Writer Error Type
#define LIB_WR_NO_ERR					(0)
#define LIB_WR_OPEN_FAIL				(-1)
#define LIB_WR_REC_FAIL					(-2)
#define LIB_WR_HDD_FULL					(-3)
#define LIB_WR_NOT_MOUNTED				(-4)

// Backup Error 
#define LIB_BA_NO_ERR                   (0)
#define LIB_BA_START_FAILED             (-1)
#define LIB_BA_ALLOC_FAILED				(-2)
#define LIB_BA_BASKET_FAILED			(-3)
#define LIB_BA_OSAL_FAILED				(-4)
#define LIB_BA_READ_FRAME_FAILED		(-5)
#define LIB_BA_CREATE_AVI_FAILED        (-6)
#define LIB_BA_MAKE_ISO_FAILED			(-7)
#define LIB_BA_WRITE_ISO_FAILED			(-8)
#define LIB_BA_NOT_ALLOWED				(-9)
#define LIB_BA_NOT_SUFFICIENT			(-10)
#define LIB_BA_INVALID_MEDIA			(-11)


#define TYPE_CDROM						0
#define TYPE_USB						1

// CB Request Owner Table
#define LIB_PLAYER						(1)
#define LIB_RECORDER					(2)
#define LIB_MISC						(3)
#define LIB_BACKUP						(4)

// PTZ Ctrl // 应该补充:左上，左下，右上，右下四个方位
#define	PAN_RIGHT									0
#define PAN_LEFT									1
#define TILT_UP										2
#define	TILT_DOWN									3
#define	ZOOM_IN										4
#define	ZOOM_OUT									5
#define	FOCUS_OUT									6
#define	FOCUS_IN									7
#define	PTZ_STOP									8

// DVR HDD INFO 
#define MAX_DVR_DISK	7		
#define MAX_PARTITION	4
#define MAX_DEVLIST		MAX_DVR_DISK*MAX_PARTITION
#define SIZE_DEV_NAME	64
#define SIZE_DIR_NAME	256
#define SIZE_TYP_NAME	128

#define	RFS_OTHER	0
#define	RFS_NFS		1

//COLORADJUST
#define BRIGHTNESS_VALUE_MIN                        0
#define BRIGHTNESS_VALUE_DEFAULT                    128
#define BRIGHTNESS_VALUE_MAX                        255

#define CONTRAST_VALUE_MIN                          0
#define CONTRAST_VALUE_DEFAULT                      128
#define CONTRAST_VALUE_MAX                          255

#define SATURATION_VALUE_MIN                        0
#define SATURATION_VALUE_DEFAULT                    128
#define SATURATION_VALUE_MAX                        255

#define HUE_VALUE_MIN                               -180
#define HUE_VALUE_DEFAULT                           0
#define HUE_VALUE_MAX                               180

#define SHARPNESS_VALUE_MIN                         0
#define SHARPNESS_VALUE_DEFAULT                     4
#define SHARPNESS_VALUE_MAX                         9

#define BITRATE_TYPE_CBR                            0
#define BITRATE_TYPE_VBR                            1

#define IFRAME_INTERVAL_30                          0
#define IFRAME_INTERVAL_15                          1
#define IFRAME_INTERVAL_10                          2
#define IFRAME_INTERVAL_5                           3
#define IFRAME_INTERVAL_1                           4

#define FRAMERATE_30_25								0	//NTSC 30, PAL 25
#define FRAMERATE_15_13								1	//NTSC 15, PAL 13
#define FRAMERATE_08_06								2	//NTSC 8,  PAL 6
#define FRAMERATE_04_03								3	//NTSC 4,  PAL 3
//DIO
#define MAX_SENSOR                                  16
#define SENSOR_DETECT_OFF                           0
#define SENSOR_DETECT_ON                            1

#define SENSOR_TYPE_NO                              0
#define SENSOR_TYPE_NC                              1

#define MAX_ALARM                                   4
#define ALARM_DETECT_OFF                            0
#define ALARM_DETECT_ON                             1

#define ALARM_TYPE_NO                               0
#define ALARM_TYPE_NC                               1

#define ALARM_DELAYSEC_MIN                          1
#define ALARM_DELAYSEC_MAX                          30
#define ALARM_DELAYSEC_DEFAULT                      3

//MOTION DETECT
#define MOTION_W_CELL                               22
#define MOTION_H_CELL                               15
#define MOTION_PAL_INC                              3

#define MAX_MOTION_CELL                             MOTION_W_CELL*(MOTION_H_CELL+MOTION_PAL_INC)

#define MOTIONAREA_MODE_CLEAR                       0
#define MOTIONAREA_MODE_SET                         1

#define CODECTYPE_H264                              0
#define CODECTYPE_MPEG4                             1
#define CODECTYPE_MJPEG                             2

#define RESOLUTION_D1                               0
#define RESOLUTION_CIF                              1
#define RESOLUTION_HALFD1                           2
#define RESOLUTION_QCIF                             3
#define RESOLUTION_720P                             4

//AUDIO
#define MAX_AUDIOIN                                 16

#define AUDIO_CODEC_G711                            0
#define AUDIO_CODEC_AAC                             1

#define AUDIO_SAMPLERATE_8KHZ                       0
#define AUDIO_SAMPLERATE_16KHZ                      1

#define AUDIO_VOLUME_MUTE                           0
#define AUDIO_VOLUME_MAX                            10
#define AUDIO_VOLUME_DEFAULT                        5

#define AUDIO_MONO                                  0
#define AUDIO_STEREO                                1

//CAMREA POSITION
#define CAMERA_POSITION_X_MIN                       0
#define CAMERA_POSITION_X_MAX                       16
#define CAMERA_POSITION_Y_MIN                       0
#define CAMERA_POSITION_Y_MAX                       16
#define CAMERA_POSITION_X_DEFAULT                   0
#define CAMERA_POSITION_Y_DEFAULT                   0
#define CAMERA_POSITION_X_STEP                      2
#define CAMERA_POSITION_Y_STEP                      2

#define PREVRECORDDURATION_SEC_MIN                  1
#define PREVRECORDDURATION_SEC_MAX                  10
#define PREVRECORDDURATION_SEC_DEFAULT              10

#define POSTRECORDDURATION_MINUTE_MIN               1
#define POSTRECORDDURATION_MINUTE_MAX               60
#define POSTRECORDDURATION_MINUTE_DEFALUT           3

#define MAX_DAY                                     7
#define MAX_HOUR                                    24
#define MAX_TITLE_CHAR                              16
#define MAX_CAMERA                                  16//24
#define MAX_INTERNAL_CAMERA                         16
#define MAX_IPNC_CAMERA                             MAX_CAMERA-MAX_INTERNAL_CAMERA
#define MAX_PASSWORD_CHAR                           8
#define MAX_CHAR_4              4
#define MAX_CHAR_8              8
#define MAX_CHAR_10             10
#define MAX_CHAR_16             16
#define MAX_CHAR_20             20
#define MAX_CHAR_24             24
#define MAX_CHAR_32             32
#define MAX_CHAR_48             48
#define MAX_CHAR_256            256
//DISPLAYOUTPUT
#define OUTPUT_RESOLUTION_1080P                     0
#define OUTPUT_RESOLUTION_720P                      1
#define OUTPUT_RESOLUTION_SXGA                      2
#define OUTPUT_RESOLUTION_XGA                       3
#define OUTPUT_RESOLUTION_1080P50                   4
#define OUTPUT_RESOLUTION_480P						5
#define OUTPUT_RESOLUTION_576P						6

#define OUTPUT_RESOLUTION_NTSC                      0
#define OUTPUT_RESOLUTION_PAL                       1

typedef enum {
        DISK_IDLE = 0,
        DISK_CDROM,
        DONE_FDISK,
        DONE_FORMAT,
        DONE_EXT3,
        MAX_STATE
} disk_state_e;

typedef struct {
	///device path
	char dev[SIZE_DEV_NAME];
	///directory path
	char dir[SIZE_DIR_NAME];
	///file system type (fat,ext3, etc)
	char type[SIZE_TYP_NAME];
	///total size
	long sz_total;
	///available size
	long sz_avail;
	///used size
	long sz_used;
	///disk_state_e
	/*!typedef enum {
        DISK_IDLE = 0,
        DISK_CDROM,
        DONE_FDISK,
        DONE_FORMAT,
        DONE_EXT3,
        MAX_STATE
} disk_state_e;*/
	int  state;	
} dvr_dev_info_t;


typedef struct {
	///device count
	int cnt;
	///device info list
	dvr_dev_info_t dev[MAX_DEVLIST];
	/// root file system type (NFS or not)
	int 				rfs_type;
	///flag for record purpose	
	int  rec_used[MAX_DEVLIST];
} dvr_disk_info_t;

#define CAM_NTSC			1
#define CAM_PAL				0

typedef enum {
	DISP_PATH_HDMI0 = 0,
	DISP_PATH_HDMI1,		
	DISP_PATH_VGA,			
	DISP_PATH_SD,
} DisplayPath_e;

typedef enum {
	DC_MODE_1080P_30 = 0,	
	DC_MODE_1080I_60,
	DC_MODE_1080P_60,	
	DC_MODE_720P_60, 		
	DC_MODE_NTSC,			
	DC_MODE_PAL,			
	DC_MODE_XGA_60,			
	DC_MODE_SXGA_60,
	DC_MODE_1080P_50,
	DC_MODE_480P,
	DC_MODE_576P,		
	DC_MAX_MODE
} DisplayDcMode_e;

//DIO
typedef struct
{
	///sensor enable or not
    int iSensorOn;
    ///sensor type(NC, NO)
    int iSensorType;
}SENSOR,*PSENSOR;       //SENSOR INFO

typedef struct
{
	///alram enable or not
    int iAlarmOn;
    ///alram type (NC, NO)
    int iAlarmType;
    ///alram duration time
    int iDelayTime;
}ALARM,*PALARM;         //ALARM INFO

typedef struct
{
	///brightness value (0~255 default:128)
    int iBrightness;
    ///contrast value (0~255 default:128)
    int iContrast;
    ///saturation value (0~255 default:128)
    int iSaturation;
}COLORADJUST,*PCOLORADJUST; //COLORADJUST INFO

typedef struct
{
	///motion enable or not
    int iMotionOn;
    ///motion sensitivity
    int iSensitivity;
    ///motion table
    unsigned char motiontable[MAX_MOTION_CELL];
}MOTION,*PMOTION;   //MOTION DETECTION  INFO


typedef struct
{
	///camera enable or not
    int iEnable;
    ///playback camera enable or not
    int iPlaybackCameraEnable;
    ///camera name
    char strcameraname[16];
    ///color value(brightness,contrast,saturation)
    COLORADJUST coloradjust;
    ///motion information(enable, sensitivity, motion table)
    MOTION motion;
    ///pre-record enable or not
    int iPrevRecordOn;
    ///pre-record duration
    int iPrevRecordDuration;
    ///video codec type (H.264 fixed)
    int iCodecType;
    ///video resolution
    int iResolution;
    ///i-frame interval
    int iIFrameInterval;
    ///bit rate type (CBR or VBR)
    int iBitrateType;
    ///frame rate
    int iFps;
    ///bit rate
    int ikbps;
    ///audio codec type(G.711 fixed)
    int iAudioCodecType;
    ///audio sampling type
    int iAudioSamplingType;
}CAMERAINFO;


//AUDIO
typedef struct
{
	///audio input enable or not
    int iAudioInOn;
    
}AUDIOINCH,*PAUDIOINCH;         //AUDIOCH IN INFO

typedef struct
{
	///audio input enable or not
    AUDIOINCH audioinch[MAX_AUDIOIN];
    ///audio input sample rate (16 KHz fixed)
    int iSampleRate;
    ///audio input volume (0 ~ 8)
    int iVolume;
}AUDIOIN,*PAUDIOIN;  //AUDIO IN INFO



typedef struct
{
	///audio output enable or not
    int iAudioOutOn;
    ///audio output volume ( 0 ~ 10 )
    int iVolume;
    ///audio output channel
    int iSelectAudioIn;
    ///mono or stereo
    int iStereo;
    ///Audio output device(AIC:0,ONCHIPHDMI:1)
    int iDevId;
}AUDIOOUT,*PAUDIOOUT;       //AUDIO OUT INFO

typedef struct
{
        ///Main output device
    int  iMainOutput;
    ///Sub output device
    int  iSubOutput;
    ///Main output layout mode
    int  iLayoutMode;
    ///Sub output layout mode
    int  iSecondLayoutMode;
}LAYOUT,*PLAYOUT;   //LAYOUT INFO
//DISPLAY
typedef struct
{
        ///squence mode on/off
    int iSequenceModeOn;
    ///sequence time interval(sec)
    int iInterval;
}SEQUENCE,*PSEQUENCE;  //SEQUENCE INFO
typedef struct
{
	///layout frame狼 border line 钎矫 咯何
    int  iBorderLineOn;
    ///date/time芒俊辑 date/time俊 措茄 钎矫 屈侥
    int  iDateFormat;
}OSD,*POSD;         //OSD INFO
typedef struct
{
        ///output resolution
    int  iResolution;
}DISPLAYOUTPUT,*PDISPLAYOUTPUT;  //DISPLAY INFO
typedef struct
{
        ///PTZ enable or not
    int iPtzOn;
    ///device (DRX-500, PELCO-D)
    int iPtzdriverIndex;
        ///address
    int iPtzAddress;
    ///Baudrate (1200, 2400, 4800, 9600, 38400, 57600, 115200)
    int iBaudRate;
    ///data bit (7bit, 8bit)
    int iDataBit;
    ///stop bit (1bit, 2bit)
    int iStopBit;
    ///parity bit (NONE,ODD,EVEN)
    int	iParityBit;
}PTZINFO,*PPTZINFO;     // P/T/Z INFO
//RECORD
typedef struct
{
        ///record schedule table(DAY:SUN~SAT HOUR:0~23)
    int ScheduleTable[MAX_DAY][MAX_HOUR];
 }RECSCHEDULE,*PRECSCHEDULE; //RECSCHEDULE TABLE

typedef struct
{
        ///Record enable or not
    int iRecordingOn;
    ///video codec (H.264 fixed)
    int iCodecType;
    ///video resolution (D1, Half D1, CIF)
    int iResolution;
    ///signal type (NTSC or PAL)
    int InputVideoSignalType;
    ///frame rate(4 ~ 30)
    int iFps;
    ///bit rate (500kbps ~ 4000kbps)
    int ikbps;
    ///i-frame interval
    int iIFrameInterval;
    ///Bitrate type(CBR or VBR)
    int iBitrateType;
    ///Audio record enable or not
    int iAudioOn;
    ///Audio codec (G.711 fixed)
    int iAudioCodecType;
    ///audio sampling rate (8KHz or 16KHz)
    int iAudioSamplingType;
 }CAMREC,*PCAMREC;	// RECORDING INFO
typedef struct
{
        ///pre record enable or not
    int iPrevRecordOn;
    ///pre record duration
    int iPrevRecordDuration;
    ///post record enable or not
    int iPostRecordOn;
    ///post record duration
    int iPostRecordDuration;
}RECORDDURATION,*PRECORDDURATION;       //RECORDING DURATION TIME INFO
typedef struct
{
        ///camera enable or not
    int iEnable;
    ///internal or ipnc
    int iCameraType;
    ///covert enable or not
    int iCovertOn;
    ///camera name
    char strcameraname[MAX_TITLE_CHAR];
    ///PTZ setting(enable, device, Baud rate .. etc)
    PTZINFO ptzinfo;
    ///motion setting(enable,sensitivity,motion table)
    MOTION motion;
    ///color setting (brightness, contrast, saturation)
    COLORADJUST coloradjust;
    ///record setting
    CAMREC  camrec;
    ///recording schedule data
    RECSCHEDULE recschedule;
    ///pre-record, post-record setting
    RECORDDURATION recordduration;
}CAMERA,*PCAMERA;   //CAMERA INFO
typedef struct
{
	///playback mode俊辑 camera enable 咯何
    int iEnable;
}PLAYBACKCAMERA,*PPLAYBACKCAMERA;   //PLAYBACKCAMERA INFO
//EVENT
typedef struct
{
        ///motion event and record link
    int iMotionRecordingLink[MAX_INTERNAL_CAMERA][MAX_INTERNAL_CAMERA];
    ///motion event and alarm link
    int iMotionAlarmLink[MAX_INTERNAL_CAMERA][MAX_ALARM];

        ///sensor event and record link
    int iSensorRecordingLink[MAX_SENSOR][MAX_INTERNAL_CAMERA];
    ///sensor event and alarm link
    int iSensorAlarmLink[MAX_SENSOR][MAX_ALARM];

        ///video loss event and record link
    int iVideoLossRecordingLink[MAX_INTERNAL_CAMERA][MAX_INTERNAL_CAMERA];
    ///video loss event and alarm link
    int iVideoLossAlarmLink[MAX_INTERNAL_CAMERA][MAX_ALARM];
}EVENTLINK,*PEVENTLINK;     //EVENTLINK INFO



//STORAGE
typedef struct
{
        ///once or recycle
    int iHddRecylceModeOn;
}STORAGE,*PSTORAGE;
// IPNC
typedef struct
{
    char IpAddress[MAX_CHAR_16];
    int port;
    char  id[MAX_PASSWORD_CHAR];
    char  password[MAX_PASSWORD_CHAR];
}IPNCINFO,*PIPNCINFO;
typedef struct
{
        ///Emergency record On/Off
   int iRecordEmergencyOn;
}DVRALL,*PDVRALL;
//NETWORK
typedef struct
{
        ///enable or not
        char enable;				// 0:link down, 1: link up
        ///static or dhcp
        char type;					// 0:static, 1: dhcp
        ///ip address
    char ipaddr[MAX_CHAR_16];
    ///network mask
    char netmask[MAX_CHAR_16];
    ///gateway
    char gateway[MAX_CHAR_16];
} NETWORKINFO,*PNETWORKINFO;     //NETWORK INFO

//DVR PROFILE 
typedef struct
{
	///layout汲沥 (main/sub output device, layout mode)
    LAYOUT layout;
    ///main display sequence mode time interval
    SEQUENCE sequence;
    ///sub display sequence mode time interval
    SEQUENCE secondsequence;
    ///layout borderline, date format 汲沥
    OSD mainosd;
    ///main output resolution
    DISPLAYOUTPUT maindisplayoutput;
    ///sub output resolution
    DISPLAYOUTPUT subdisplayoutput;
    ///spot output resolution
    DISPLAYOUTPUT spotdisplayoutput;
    ///spot 免仿 channel number
    int iSpotOutputCh;
    ///sub display output阑 荤侩且瘤 咯何(one monitor 汲沥咯何)
    int iSubOutputEnable;
    ///camera包访 汲沥 (enable,motion,color殿)
    CAMERA camera[MAX_CAMERA];
    ///playback mode camera enable 汲沥
    PLAYBACKCAMERA playbackcamera[MAX_CAMERA];
    ///audio input俊 措茄 汲沥
    AUDIOIN audioin;
    ///audio output俊 措茄 汲沥
    AUDIOOUT audioout;
    ///阿 sensor俊 汲沥惑怕
    SENSOR sensor[MAX_SENSOR];
    ///阿 alram狼 汲沥惑怕
    ALARM alarm[MAX_ALARM];
    ///阿 event客 Record, alram俊措茄 link 汲沥
    EVENTLINK eventlink;
    ///Storage device俊 措茄 汲沥
    STORAGE storage;
    ///IPNC 汲沥 沥焊
    IPNCINFO ipnc[MAX_IPNC_CAMERA];
    ///Emergency Recording 汲沥
    DVRALL dvrall;
    ///ETH0俊 措茄 network 汲沥沥焊
    NETWORKINFO net_info1;
    ///ETH1俊 措茄 network 汲沥沥焊
    NETWORKINFO net_info2;
    ///live mode俊辑 急琶等 盲澄(audio output channel)
    int  iSelectChannel;
    ///playback mode俊辑 急琶等 盲澄(audio output channel)
    int  iSecondSelectChannel;
}PROFILE,*PPROFILE;

typedef struct
{
  ///source video width
  int frameWidth;
  ///source video height
  int frameHeight;
} SOURCE_CH_STATUS_S;

/*==============================*/
/*   	 FOR RECORDING	 		*/
/*==============================*/
#define LIB_EVENT_REC_CONT		1
#define LIB_EVENT_REC_MOT		2
#define LIB_EVENT_REC_SENSOR            3
#define LIB_EVENT_REC_VLOSS		4
#define LIB_EVENT_REC_EMG		5

#define LIB816x_initSettingParam(x...)
#define LIB816x_startSystem(x...)
#define LIB816x_setDisplayLayout(x...)
#define LIB816x_setPlaybackDisplayLayout(x...)

#define LIB816x_startRecChannel(x...)
#define LIB816x_startRecChannelAll(x...)
#define LIB816x_stopRecChannel(x...)
#define LIB816x_stopRecChannelAll(x...)
#define LIB816x_HddFormat(x...)
#define LIB816x_startPlaybackAll(x...)
#define LIB816x_stopPlaybackAll(x...)
#define LIB816x_GetRecDays(x...)
#define LIB816x_GetRecHour(x...)
#define LIB816x_getCurPlaybackTime(x...)

#define LIB816x_sys_info(x...)
#define LIB816x_net_info(x...)
#define LIB816x_disk_info(x...)

#define LIB816x_systemReboot(x...)
#define LIB816x_setRecordingType(x...)
#define LIB816x_set_encoder_property(x...)

#endif					

#endif 
