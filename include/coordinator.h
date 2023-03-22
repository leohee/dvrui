#ifndef COORDINATOR_H
#define COORDINATOR_H

#define     APPWINDOW_X                     0
#define     APPWINDOW_Y                     0
#define     APPWINDOW_W                     1920
#define     APPWINDOW_H                     1080

#define     MAINWINDOWFULL_X                0
#define     MAINWINDOWFULL_Y                0
#define     MAINWINDOWFULL_W                1920
#define     MAINWINDOWFULL_H                1080

#define     MAINTOOLBARFULL_X               0
#define     MAINTOOLBARFULL_Y               0
#define     MAINTOOLBARFULL_W               MAINWINDOWFULL_W
#define     MAINTOOLBARFULL_H               40

#define     MAINSIDEBARFULL_X               0
#define     MAINSIDEBARFULL_Y               MAINTOOLBARFULL_H
#define     MAINSIDEBARFULL_W               200
#define     MAINSIDEBARFULL_H               (MAINWINDOWFULL_H-MAINSIDEBARFULL_Y)

#define     MAINSTATUSBARFULL_X             MAINSIDEBARFULL_W
#define     MAINSTATUSBARFULL_H             40
#define     MAINSTATUSBARFULL_Y             (MAINWINDOWFULL_H-MAINSTATUSBARFULL_H)
#define     MAINSTATUSBARFULL_W             (MAINWINDOWFULL_W-MAINSTATUSBARFULL_X)

#define     SECONDSTATUSBARFULL_W           211
#define     SECONDSTATUSBARFULL_H           32
#define     SECONDSTATUSBARFULL_X           (SECONDWINDOWFULL_W-SECONDSTATUSBARFULL_W)/2
#define     SECONDSTATUSBARFULL_Y           (SECONDWINDOWFULL_Y+SECONDWINDOWFULL_H-SECONDSTATUSBARFULL_H)

#define     TIMELINEBARFULL_X               MAINSIDEBARFULL_W
#define     TIMELINEBARFULL_H               100
#define     TIMELINEBARFULL_Y               (MAINWINDOWFULL_H-MAINTOOLBARFULL_H-TIMELINEBARFULL_H)
#define     TIMELINEBARFULL_W               (MAINWINDOWFULL_W-TIMELINEBARFULL_X)

#define     SUBTOOLBARFULL_W                491
#define     SUBTOOLBARFULL_H                50
#define     SUBTOOLBARFULL_X                (MAINWINDOWFULL_W-SUBTOOLBARFULL_W)
#define     SUBTOOLBARFULL_Y                (MAINWINDOWFULL_H-SUBTOOLBARFULL_H-MAINSTATUSBARFULL_H-TIMELINEBARFULL_H)

#define     LAYOUTWINDOWFULL_X              (MAINSIDEBARFULL_X+MAINSIDEBARFULL_W)
#define     LAYOUTWINDOWFULL_Y              (MAINTOOLBARFULL_Y+MAINTOOLBARFULL_H)
#define     LAYOUTWINDOWFULL_W              (MAINWINDOWFULL_W-LAYOUTWINDOWFULL_X)
#define     LAYOUTWINDOWFULL_H              (MAINWINDOWFULL_H-LAYOUTWINDOWFULL_Y)

#define     LOGINDLG_W                      320
#define     LOGINDLG_H                      200


#define     MARGIN_4                        4
#define     MARGIN_10                       10

#define     OSD_ICON_W                      20
#define     OSD_ICON_H                      20


#define     TIMELINE_X                      120
#define     TIMELINE_Y                      20
#define     TIMELINE_FW                     1440
#define     TIMELINE_SW                     720
#define     TIMELINE_H                      80
#define     TIMELINE_STEPF                  4
#define     TIMELINE_STEPS                  2

#define     TIMELINEHEADER_X                120
#define     TIMELINEHEADER_Y                0
#define     TIMELINEHEADER_H                20

#define     PREVIEW_CHANNEL_W               352
#define     PREVIEW_CHANNEL_HNTSC           240
#define     PREVIEW_CHANNEL_HPAL            288


#define     CAMERATREEICON_SIZE_W           30
#define     CAMERATREEICON_SIZE_H           30

#define     AUDIOTREEICON_SIZE_W            30
#define     AUDIOTREEICON_SIZE_H            30

#define     SENSORTREEICON_SIZE_W           30
#define     SENSORTREEICON_SIZE_H           30

#define     ALARMTREEICON_SIZE_W            30
#define     ALARMTREEICON_SIZE_H            30

#define     USERLISTICON_SIZE_W             30
#define     USERLISTICON_SIZE_H             30

#endif // COORDINATOR_H
