#ifndef STRDEFINE_H
#define STRDEFINE_H
#include "dvr_app_api.h"
#if DVR_API_ENABLE
#define PROFILEFILNAME			"/bin/dvrapp_cfg/netracfg.ini"
#define USERLISTFILNAME			"/bin/dvrapp_cfg/netrauser.ini"
#define EVENTLOGPATH			"/bin/dvrapp_cfg/"
#else
#define PROFILEFILNAME			"netracfg.ini"
#define USERLISTFILNAME			"netrauser.ini"
#define EVENTLOGPATH			""
#endif
#define STRCAMERANAME			"CAM"
#define STRIPNCCAMERANAME		"IPNC"
#define STRAUDIOINNAME			"AUDIOIN"
#define STRSENSORNAME			"SENSOR"
#define STRALARMNAME			"ALARM"
#define STRSECOND				"SEC"
#define STRMINUTE				"MIN"
#define STRLEVEL				"Level"
#define STRPOS_X				"POS_X"
#define STRPOS_Y				"POS_Y"
#define STRFPS					"FPS"
#define STRVOL					"VOL"
#define STRVOLMUTE				"MUTE"

#define STRUNKNOWN				"UNKNOWN"
#define STRSPEED				"SPEED"

#define STRADMIN				"ADMINISTRATOR"
#define STRUSER					"USER"

#define STRINPUTVIDEOSIGNALTYPE_NTSC	"NTSC"
#define STRINPUTVIDEOSIGNALTYPE_PAL	"PAL"

#define STREVENTLINK_RECORD			"R"
#define STREVENTLINK_ALARM			"#"

#define STREVENTLINK_MOTION			"MOTION"
#define STREVENTLINK_SENSOR			"SENSOR"
#define STREVENTLINK_VIDEOLOSS		"V-LOSS"

#define STREVENTLINK_ON				"O"//"ON"
#define STREVENTLINK_OFF			"X"//"OFF"


#define DEFAULT_PASSWORD			""
#define LOGIN_SUCCESS				"LOGIN SUCCESS"
#define LOGIN_FAILED				"LOGIN FAILED"

#define CAMERA_TREE_CAMERA_ICON		":/skin/tree_camera_icon.bmp"
#define AUDIO_TREE_CAMERA_ICON		":/skin/tree_audio_icon.bmp"
#define SENSOR_TREE_CAMERA_ICON		":/skin/tree_sensor_icon.bmp"
#define ALARM_TREE_CAMERA_ICON		":/skin/tree_alarm_icon.bmp"
#define USER_LIST_USER_ICON			":/skin/list_user_icon.bmp"

#define PLAYBACKMODE_ICON_STOP		":/skin/osd_stop_icon.bmp"
#define PLAYBACKMODE_ICON_PLAY		":/skin/osd_play_icon.bmp"
#define PLAYBACKMODE_ICON_PAUSE		":/skin/osd_pause_icon.bmp"
#define PLAYBACKMODE_ICON_REVERSE	":/skin/osd_reverse_icon.bmp"
#define PLAYBACKMODE_ICON_REVERSESTEP	":/skin/osd_reversestep_icon.bmp"
#define PLAYBACKMODE_ICON_FORWARD		":/skin/osd_forward_icon.bmp"
#define PLAYBACKMODE_ICON_FORWARDSTEP	":/skin/osd_fowardstep_icon.bmp"

#define STRPLAYBACKSPEED_1DIVIDE4X		"1/4X"
#define STRPLAYBACKSPEED_1DIVIDE2X		"1/2X"
#define STRPLAYBACKSPEED_1X				"1X"
#define STRPLAYBACKSPEED_2X				"2X"
#define STRPLAYBACKSPEED_4X				"4X"
#define STRPLAYBACKSPEED_8X				"8X"
#define STRPLAYBACKSPEED_16X			"16X"
#define STRPLAYBACKSPEED_32X			"32X"


#define EVENTLOG_SYSTEM_START			"System Started"

#endif 
