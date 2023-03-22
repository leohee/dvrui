#ifndef DEFINE_H
#define DEFINE_H



//USERLIST
#define MAX_USER                                    10
#define USERLEVEL_ADMIN                             0
#define USERLEVEL_USER                              1

#define USERACCESS_LIVEMENU                         0x0001
#define USERACCESS_PLAYBACK                         0x0002
#define USERACCESS_MAINMENU                         0x0004
#define USERACCESS_SETUPMENU                        0x0008

//DISPLAY LAYOUT
#define MAIN_SWAPOFF                                0
#define MAIN_SWAPON                                 1

#define WINDOW_FULLHD                               0
#define WINDOW_SXGA                                 1

#define NORMAL_SCREEN_MODE                          0
#define FULL_SCREEN_MODE                            1

#define CONSOLEMODE_OFF                             0
#define CONSOLEMODE_ON                              1

#define SUBCONSOLEMODE_OFF                          0
#define SUBCONSOLEMODE_ON                           1

#define SPOTLAYOUT_OFF                              0
#define SPOTLAYOUT_ON                               1

#define BORDERLINE_OFF                              0
#define BORDERLINE_ON                               1


#define SETUPPROPERTY_OFF                           0
#define SETUPPROPERTY_ON                            1

#define CAMERA_TYPE_INTERNAL                        0
#define CAMERA_TYPE_IPNC                            1

#define MAX_LAYOUTCHANNEL                           16

#define LAYOUTMODE_1                                1
#define LAYOUTMODE_4                                2
#define LAYOUTMODE_9                                3
#define LAYOUTMODE_8                                4
#define LAYOUTMODE_8_1                              5
#define LAYOUTMODE_12                               6
#define LAYOUTMODE_12_1                             7
#define LAYOUTMODE_14                               8
#define LAYOUTMODE_16                               9

#define LAYOUT_1                                    1
#define LAYOUT_4                                    4
#define LAYOUT_9                                    9
#define LAYOUT_8                                    8
#define LAYOUT_12                                   12
#define LAYOUT_14                                   14
#define LAYOUT_16                                   16

#define MAX_PAGECHANNEL                             4// 6

#define MAINTIMER_STEPVALUE                         1000
#define MAX_EVENTLOG_SHOWCOUNT                      150
#define DEFAULT_SEQUENCE_INTERVAL                   3

//RECORD
#define MAX_REC_SCHEDULE                            5
#define REC_SCHEDULE_CONTINUOUSRECORD               0
#define REC_SCHEDULE_RECORDBYMOTION                 1
#define REC_SCHEDULE_RECORDBYSENSOR                 2
#define REC_SCHEDULE_RECORDBYVLOSS                  3
#define REC_SCHEDULE_NORECORD                       4

#define BITRATE_QUALITY_HIGHEST                     0
#define BITRATE_QUALITY_HIGH                        1
#define BITRATE_QUALITY_MEDIUM                      2
#define BITRATE_QUALITY_LOW                         3
#define BITRATE_QUALITY_LOWEST                      4


//RECORD EVENT
#define MAX_EVENT                                   6
#define EVENT_NORECORD                              0
#define EVENT_RECORDONLY                            1
#define EVENT_RECORDMOTION                          2
#define EVENT_RECORDSENSOR                          3
#define EVENT_RECORDVIDEOLOSS                       4
#define EVENT_RECORDEMERGENCY                       5

//EVENTLINK
#define MAX_EVENTLINK                               3
#define EVENTLINK_MOTION                            0
#define EVENTLINK_SENSOR                            1
#define EVENTLINK_VIDEOLOSS                         2

//STORAGE
#define MAX_STORAGE_DEVICE                          8
#define STORAGE_USE_TYPE_RECYCLE                    0
#define STORAGE_USE_TYPE_ONCE                       1


//PLAYBACK
#define PLAYBACKMODE_STOP                           0
#define PLAYBACKMODE_PLAY                           1
#define PLAYBACKMODE_PUASE                          2
#define PLAYBACKMODE_REVERSE                        3
#define PLAYBACKMODE_REVERSESTEP                    4
#define PLAYBACKMODE_FORWARD                        5
#define PLAYBACKMODE_FORAWRDSTEP                    6

#define MAX_PLAYBACKSPEED                           8
#define PLAYBACKSPEED_1DIVIDE4X                     0
#define PLAYBACKSPEED_1DIVIDE2X                     1
#define PLAYBACKSPEED_1X                            2
#define PLAYBACKSPEED_2X                            3
#define PLAYBACKSPEED_4X                            4
#define PLAYBACKSPEED_8X                            5
#define PLAYBACKSPEED_16X                           6
#define PLAYBACKSPEED_32X                           7

#define PLAYCALENDARMAXDAYCOUNT                     31
#define PLAYBACKDAYMINUTECOUNT                      1440

#define PLAYBACKTIMELINEMAX_COL                     360
#define PLAYBACKTIMELINEMAX_ROW                     4
#define PLAYBACKTIMELINE_24                         24
#define PLAYBACKTIMELINE_12                         12
#define PLAYBACKTIMELINE_6                          6
#define PLAYBACKTIMELINE_ZOOM_24                    1
#define PLAYBACKTIMELINE_ZOOM_12                    2
#define PLAYBACKTIMELINE_ZOOM_6                     4


//SETUP
#define SETUP_DISPLAY                               0
#define SETUP_CAMERA                                1
#define SETUP_RECORD                                2
#define SETUP_EVENT                                 3
#define SETUP_DIO                                   4
#define SETUP_AUDIO                                 5
#define SETUP_STORAGE                               6
#define SETUP_BACKUP                                7
#define SETUP_NETWORK                               8
#define SETUP_SYSYEM                                9
#define SETUP_IPNC                                  10

#define INPUT_SIGNALTYPE_NTSC                       0
#define INPUT_SIGNALTYPE_PAL                        1

#define OSD_DATEFORMAT_MMDDYYYY_AP                  0
#define OSD_DATEFORMAT_DDMMYYYY_AP                  1
#define OSD_DATEFORMAT_YYYYDDMM_AP                  2
#define OSD_DATEFORMAT_YYYYMMDD_AP                  3
#define OSD_DATEFORMAT_MMDDYYYY                     4
#define OSD_DATEFORMAT_DDMMYYYY                     5
#define OSD_DATEFORMAT_YYYYDDMM                     6
#define OSD_DATEFORMAT_YYYYMMDD                     7

//PRIVACYMASK
#define PRIVACYMASK_W_CELL                          22
#define PRIVACYMASK_H_CELL                          15
#define PRIVACYMASK_PAL_INC                         3

#define MAX_PRIVACYMASK_CELL                        MOTION_W_CELL*(PRIVACYMASK_H_CELL+PRIVACYMASK_PAL_INC)

#define PRIVACYMASKAREA_MODE_CLEAR                  0
#define PRIVACYMASKAREA_MODE_SET                    1

//PTZ
#define MAX_BAUDRATE                                7
#define BAUDRATE_1200                               0
#define BAUDRATE_2400                               1
#define BAUDRATE_4800                               2
#define BAUDRATE_9600                               3
#define BAUDRATE_38400                              4
#define BAUDRATE_57600                              5
#define BAUDRATE_115200                             6

#define MAX_DATABIT                                 2
#define DATBIT_7                                    0
#define DATBIT_8                                    1

#define MAX_STOPBIT                                 2
#define STOPBIT_1                                   0
#define STOPBIT_2                                   1

#define MAX_PARITYBIT                               3
#define PARITYBIT_NONE                              0
#define PARITYBIT_ODD                               1
#define PARITYBIT_EVEN                              2

#define PTZCTRL_DRX_500                             0
#define PTZCTRL_PELCO_D                             1

#define PTZ_BAUDRATE_1200                           1200
#define PTZ_BAUDRATE_2400                           2400
#define PTZ_BAUDRATE_4800                           4800
#define PTZ_BAUDRATE_9600                           9600
#define PTZ_BAUDRATE_38400                          38400
#define PTZ_BAUDRATE_57600                          57600
#define PTZ_BAUDRATE_115200                         115200

#define PTZ_DATBIT_7                                7
#define PTZ_DATBIT_8                                8

#define PTZ_STOPBIT_1                               1
#define PTZ_STOPBIT_2                               2

#define PTZ_PARITY_NONE                             0
#define PTZ_PARITY_ODD                              1
#define PTZ_PARITY_EVEN                             2


const int ptzbaudrate_list[MAX_BAUDRATE]={PTZ_BAUDRATE_1200,PTZ_BAUDRATE_2400,PTZ_BAUDRATE_4800,PTZ_BAUDRATE_9600,PTZ_BAUDRATE_38400,PTZ_BAUDRATE_57600,PTZ_BAUDRATE_115200};
const int ptzdatabit_list[MAX_DATABIT]={PTZ_DATBIT_7,PTZ_DATBIT_8};
const int ptzstopbit_list[MAX_STOPBIT]={PTZ_STOPBIT_1,PTZ_STOPBIT_1};
const int ptzparitybit_list[MAX_PARITYBIT]={PTZ_PARITY_NONE,PTZ_PARITY_ODD,PTZ_PARITY_EVEN};

/*----------------------------------------------------------------------------
 NETWORK
-----------------------------------------------------------------------------*/
#define NETWORK_PROTOCOL_RTSP                       0

#define NETWORK_TYPE_STATIC                         0
#define NETWORK_TYPE_DHCP                           1

#define NETWORK_DEV1								"eth0"
#define NETWORK_DEV2								"eth1"
#define NETWORK_DEV1_NO								0
#define NETWORK_DEV2_NO								1

#define NETWORK_IPADDRESS_DEFAULT1                  "0.0.0.0"//"192.168.1.200"
#define NETWORK_IPADDRESS_DEFAULT2                  "0.0.0.0"//"192.168.1.201"

#define NETWORK_SUBNETMASK_DEFAULT                  "255.255.255.0"
#define NETWORK_GATEWAY_DEFAULT                     "192.168.1.1"

#define NETWORK_PORTNUMBER_MIN                      1024
#define NETWORK_PORTNUMBER_MAX                      65535
#define NETWORK_PORTNUMBER_DEFAULT                  7620

/*----------------------------------------------------------------------------
 STORAGE
-----------------------------------------------------------------------------*/
#define STORAGE_WRITE_MODE_RECYCLE                  0
#define STORAGE_WRITE_MODE_ONCE                     1

/*----------------------------------------------------------------------------
 UI Internal Event
-----------------------------------------------------------------------------*/
#define EVENT_CMD_MOTION                            0
#define EVENT_CMD_SENSOR                            1
#define EVENT_CMD_VIDEOLOSS                         2
#define EVENT_CMD_ALARM		                        3
#define EVENT_CMD_MOUSEWHEEL                        4
#define EVENT_CMD_POSTREC_SET                       5
#define EVENT_CMD_POSTREC_CLEAR                     6
#define EVENT_CMD_MAX                               7

// \brief Floor a integer value. 
#define VsysUtils_floor(val, align)  (((val) / (align)) * (align))

// \brief Align a integer value. 
#define VsysUtils_align(val, align)  VsysUtils_floor(((val) + (align)-1), (align))
#define LayoutWidthAlign	8

#endif // DEFINE_H



