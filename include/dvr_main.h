#ifndef DVRMAIN_H
#define DVRMAIN_H

#include <QDialog>
#include <QTimer>
#include <QMouseEvent>
#include <QCursor>

#include "dvr_toolbar_main.h"
#include "dvr_statusbar.h"
#include "dvr_sidebar.h"
#include "dvr_layout_main.h"
#include "dvr_timelinebar_play.h"
#include "dvr_setup.h"
#include "dvr_toolbar_sub.h"
#include "dvr_login.h"
#include "notifydialog.h"
#include "eventlogdialog.h"

namespace Ui {
    class dvrmain;
}

class WidgetKeyboard;
class dvrmain : public QDialog {
    Q_OBJECT
public:
	///login check flag
    bool m_bLogin;
    ///playback timebar display flag
    bool m_bDisplayPlaybackTimebar;
    ///key event block flag
    bool bBlockKey;
    ///disk info refresh flag
    bool refreshdiskinfo;
    ///calendar update flag
    bool bDoUpdateCalendar;
    ///main layout control
    dvr_layout_main Dvrmainlayout;
    ///main toolbar control
    dvr_toolbar_main Dvrmaintoolbar;
    ///status bar control
    dvr_statusbar Dvrmainstatusbar;
    ///side bar control
    dvr_sidebar Dvrmainsidebar;
    ///time bar control
    dvr_timelinebar_play Dvrmainplaybacktimelinebar;
    ///setup control
    dvr_setup Dvrmainsetupproperty;
    ///sub toolbar control
    dvr_toolbar_sub Dvrsecondtoolbar;
    ///error notify dialog control
    NotifyDialog Dvrnotifydialog;
    ///event log dialog control
    EventLogDialog  Dvreventviewer;
    ///login dialog control
    dvr_login Dvrlogin;
    ///Main timer
    QTimer *Dvrmaintimer;
    ///Main layout sequence mode timer
    QTimer *Dvrmainlayouttimer;
    ///Sub layout sequence mode timer
    QTimer *Dvrsublayouttimer;
    ///Event check timer
    QTimer *DvrEvttimer;
    ///motion event check & post rec timer
    QTimer *DvrMotionChecktimer;
    ///timebar조작등 발생시에 timebar update를 잠시 막기 위한 flag
    int m_iPlaybackTimebarUpdateDisableCount;
    ///audio channel number
    int m_iAudioOutChannel;

    //dvrmain internal
    dvrmain(QWidget *parent = 0);
    ~dvrmain();
    void InitProfile(bool forceReset);
    void LoginStart();
    void LoginStop();
    void Logout();
    void MainTimerStart();
    void MainTimerStop();
    void MainlayoutTimerStart();
    void MainlayoutTimerStop();
    void SublayoutTimerStart();
    void SublayoutTimerStop();
    void BeforeTerminateOperation();
    void SwitchFB(int DisplayId);

    //recording related
    void EmergencyRecordAll();
    void RecScheduleHandler(int day,int hour);
    void MotionEventHandler(int motionidx,int detectstatus);
    void SensorEventHandler(int sensoridx,int detectstatus);
    void VideoLossEventHandler(int videolossidx,int detectstatus);

    //playback related
    void PlaybacktimebarshowOnOff(bool bShowTimebar);

    //display related
    void SetSubConsoleModeOnOff(int iMode);

    //event log
    void EventLogAdd(char *EventStr,bool bShow);
    void ShowNotifyDialog(char* str);
    void EraseOldEventLog(int year,int month,int day);

    //etc
    bool ShowVirtualKeyboard();
    void SetAudioChannel(bool bSet,int ch);
    virtual void keyPressEvent ( QKeyEvent * event );
    virtual void wheelEvent(QWheelEvent *event);

public slots:
    void MainTimerUpdate();
    void MainlayoutTimerUpdate();
    void SublayoutTimerUpdate();
    void EvtHandler();
    void EvtMotionHandler();
protected:
    void changeEvent(QEvent *e);
    WidgetKeyboard *virtualKeyBoard;

private:
    Ui::dvrmain *ui;
    bool m_systemtimerstarted;
};


#endif 
