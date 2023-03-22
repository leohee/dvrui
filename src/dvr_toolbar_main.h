#ifndef DVRMAINTOOLBAR_H
#define DVRMAINTOOLBAR_H

#include <QDialog>

namespace Ui {
    class dvr_toolbar_main;
}

class dvr_toolbar_main : public QDialog {
    Q_OBJECT
public:
    dvr_toolbar_main(QWidget *parent = 0);
    ~dvr_toolbar_main();
	///현재 선택된 cursor 위치
    int toolbarcursorPosition;
    ///현재 layout에 대한 최대 page count
    int PageCount;

    void SetMainWindowSize();

    void SetLivePlayBackBtnSelect(int iMode);
    void SetLayoutModeBtnSelect(int iMode);
    void SetEnableSetupBtn(int iEnable);
    void SetEnableAllBtn(int iEnable);
    void SetEnableSubToolbar(int iEnable);

    virtual void keyPressEvent ( QKeyEvent * event );
    void ExeCurrentSelection();
    void ShowCurrentSelection();
    void MaxPageCalc();

public slots:
    void LivebackModeBtnSlot();
    void PlaybackModeBtnSlot();
    void LivebackSubModeBtnSlot();
    void PrevPageBtnSlot();
    void NextPageBtnSlot();
    void NextPageBtnSlot_Sequence();
    void Layout1BtnSlot();
    void Layout4BtnSlot();
    void Layout9BtnSlot();
    void Layout16BtnSlot();
    void Layout8BtnSlot();
    void Layout8_1BtnSlot();
    void Layout12BtnSlot();
    void Layout12_1BtnSlot();
    void Layout14BtnSlot();
    void SequenceModeBtnSlot();
    void AudioOnOffBtnSlot();
    void EventLogBtnSlot();
    void SnapshotBtnSlot();
    void BackupBtnSlot();
    void SetupPropertyCallSlot();
    void EmergencyRecordSlot();
    void LogoutSlot();
protected:
	///toolbar의 변경/선택이 가능한지 여부표시 flag
    bool bBlockenableChange;
    void changeEvent(QEvent *e);

private:
    Ui::dvr_toolbar_main *ui;
};

#endif 
