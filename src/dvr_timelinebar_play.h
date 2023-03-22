#ifndef DVRMAINPLAYBACKTIMELINEBAR_H
#define DVRMAINPLAYBACKTIMELINEBAR_H

#include <QDialog>
#include <QColor>
#include <QLabel>
#include <QTableWidgetItem>
#include "define.h"

namespace Ui {
    class dvr_timelinebar_play;
}

class dvr_timelinebar_play : public QDialog {
    Q_OBJECT
public:
    dvr_timelinebar_play(QWidget *parent = 0);
    ~dvr_timelinebar_play();

    void SetMainWindowSize();
        ///timebar table width
    int  m_iTimelineW;
    ///timebar table height
    int  m_iTimelineH;
    ///timebar item width
    int  m_iTimelineStep;
    ///timebar item column
    int  m_iTimelineCol;
    ///timebar item row
    int  m_iTimelineRow;
    ///현재 timebar zoom설정에 대한 band의 개수
    int  m_iTimelineBand;
    ///timebar에서 현재 선택된 위치
    int cursorPosition;
	///timebar의 channel name items
    QLabel           *m_iChannelName[PLAYBACKTIMELINEMAX_ROW];
    ///record event에 따른 color
    QColor           m_itemcolor[MAX_EVENT];
    ///timebar의 time items
    QTableWidgetItem m_tableitem[PLAYBACKTIMELINEMAX_ROW][PLAYBACKTIMELINEMAX_COL];
    ///timebar의 time header items
    QTableWidgetItem m_tableheaderitem[PLAYBACKTIMELINE_24];

    void SetTimeTableColRow(int iRow,int iCol,int iEvent);
    int GetRecordHourInMinuteDataProfile(struct tm  readtime);
    void SetTimeTableUpdate(bool bFullUpdate,bool bReadRecData);
    void UpdateTimeTable();

    void SetTimeTableChannel(int iPageChannel);
    void GetTimeTableSelectTime(struct tm * ptm);
    void SetTimeTableSelectTime(struct tm * ptm);
    void SetTimeTableSelectTimeMarking(struct tm * ptm);
    void GetNextRecordDataTime(struct tm* ptm);

public slots:
    void TimeLineChannelUpBtnSlot();
    void TimeLineChannelDownBtnSlot();
    void TimeLineZoomInBtnSlot();
    void TimeLineZoomOutBtnSlot();
    void TimeLineBandLeftBtnSlot();
    void TimeLineBandRightBtnSlot();
    void ItemSelectionChangedSlot();


protected:
    void changeEvent(QEvent *e);
    virtual void wheelEvent(QWheelEvent *event);

private:
    Ui::dvr_timelinebar_play *ui;

private slots:
    void on_timetable_itemClicked(QTableWidgetItem* item);
};

#endif 
