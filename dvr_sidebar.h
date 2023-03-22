#ifndef DVR_SIDEBAR_H
#define DVR_SIDEBAR_H

#include <QDialog>
#include <QTreeWidgetItem>
#include <QIcon>
#include <QLabel>
#include "define.h"
#include "strdefine.h"

namespace Ui {
    class dvr_sidebar;
}

class dvr_sidebar : public QDialog {
    Q_OBJECT
public:
	///현재 설정 mode (live/playback)
    int m_Mode;
    ///현재 playback mode
    int m_iPlaybackMode;
    ///현재 playback speed
    int m_iPlaybackSpeed;
    ///playback 설정 변경 여부 flag
    int m_iplayonchange;
    ///Calendar page가 변경되었는지 여부 flag
    bool bCalendarPageChange;
    ///Camera tree items
    QTreeWidgetItem CameraTreeItem[MAX_CAMERA];
    ///Camera icon
    QIcon CameraTreeItemIcon;
    ///Sensor On icon
    QLabel *QSensorOnIconLabel[MAX_SENSOR];
    ///Alram On icon
    QLabel *QAlarmOnIconLabel[MAX_ALARM];
    ///dial update timer
    QTimer *Dialtimer;

    dvr_sidebar(QWidget *parent = 0);
    ~dvr_sidebar();
    void init();
    void setMode();
    void setSideBarTreeControl();
    void changeTreeItem(int column,bool bChangeValue);
    void addEventlogItem(char *str);
    void clearEventlog();
    void setSensorIconStatus(int iSensorId,int iSensorDetect);
    void setAlarmIconStatus(int iAlarmId,int iAlarmDetect);
    void setSideBarPtzControls();
    int ptzGetBuadRate(int chid);
    void setCalendarRecordDate(struct tm * ptm,int iMonthTable[PLAYCALENDARMAXDAYCOUNT]);
    void getCalendarSelectDay(struct tm * ptm);
    void setCalendarSelectDay(struct tm * ptm,bool bplaystop);
    QString setPlaySpeedValueText(int ivalue);
    void setPlaybackBtnSelect(int iMode);
    void setPlayBackMode(int iMode);
    void setPlayBackSpeedChange();
    virtual void keyPressEvent ( QKeyEvent * event );

protected:
    void changeEvent(QEvent *e);

private:
    Ui::dvr_sidebar *ui;

public slots:
    void dialTimerUpdate();
    void TreeItemClicked(QTreeWidgetItem *item, int column);
    void TreeItemDoubleClicked(QTreeWidgetItem *item, int column);
    void ptzUpBtnSlot();
    void ptzDownBtnSlot();
    void ptzLeftBtnSlot();
    void ptzRightBtnSlot();
    void ptzZoomInBtnSlot();
    void ptzZoomOutBtnSlot();
    void ptzFocusInBtnSlot();
    void ptzFocusOutBtnSlot();
    void ptzStopSlot();
    void playbackCalendarSelectionChangedSlot();
    void playbackCalendarcurrentPageChangedSlot(int year, int month);
    void searchBtnSlot();
    void playbackReverseBtnSlot();
    void playbackReverseStepBtnSlot();
    void playbackPlayBtnSlot();
    void playbackPauseBtnSlot();
    void playbackStopBtnSlot();
    void playbackForwardStepBtnSlot();
    void playbackForwardBtnSlot();
    void playbackSpeedChangeSlot(int value);
};

#endif 
