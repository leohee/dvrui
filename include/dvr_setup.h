#ifndef DVRMAINSETUPPROPERTY_H
#define DVRMAINSETUPPROPERTY_H

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <QDialog>
#include <QShowEvent>
#include <QSlider>
#include <QTreeWidgetItem>
#include <QListWidgetItem>
#include <QIcon>
#include <QCheckBox>
#include <QRect>
#include <QColor>
#include <QTableWidgetItem>
#include <QSpinBox>
#include <QDateTimeEdit>
#include <QTimer>

#include "dvr_app_api.h"
#include "define.h"
#include "dvr_message.h"
#include "dvr_formatconfirm.h"
/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define SIZE_NET_STR	16
typedef struct {
	///network type static or dhcp
	char type;					// 0: static, 1: dhcp
	///link down or link up
	char state;					// 0: link down, 1: link up
	///ip address
	char ip[SIZE_NET_STR];		// ip address
	///netmask
	char mask[SIZE_NET_STR];	// netmask
	///gateway
	char gate[SIZE_NET_STR];	// gateway
} dvr_net_info_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

namespace Ui {
    class dvr_setup;
}

#include <QThread>
class FormatThread : public QThread
{
private:
    char dir[80];
    char dev[80];
    int status;
    bool bReboot;
public:
    FormatThread();
    void setDir(char* dirPath,char* devPath,int hddstatus);
    void run();
};

class dvr_setup : public QDialog {
    Q_OBJECT
public:
    dvr_setup(QWidget *parent = 0);
    ~dvr_setup();
    virtual void keyPressEvent ( QKeyEvent * event );


//PROPERTY TAB
	///현재 tab index
    int m_iSetupTabIndex;
    ///backup mode, setup mode 구분
    bool m_allTabEnable;
    void SetMainWindowSize();

//DISPLAY
	///one monitor mode 인지 아닌지
    bool m_bOneMonitorDisplay;
    void InitSetupDisplayControls();
    QString SetSequenceIntervalValueText(int iInterval);
    void SetDisplaydata();
    void GetDisplaydata();
    void SetSubOutputEnable();
    void ResotreDisplay();

//CAMERA
	///camera tree items
    QTreeWidgetItem SetupCameraTreeItem[MAX_INTERNAL_CAMERA];
    ///camera icon
    QIcon CameraTreeItemIcon;
    ///현재 선택된 camera tree id
    int  m_iCameraSelectId;
	///motion 설정 table
    QTableWidgetItem m_motionitem[MOTION_H_CELL+MOTION_PAL_INC][MOTION_W_CELL];
    ///motion setting의 clear mode/set mode를 저장
    int  m_iMotionAreaMode;
    ///motion table area width
    int  m_iMotionAreaW;
    ///motion table area height
    int  m_iMotionAreaH;
    ///motion table의 한 block width
    int  m_iMotionBlockW;
    ///motion table의 한 block height
    int  m_iMotionBlockH;
	///motion clear/set에 해당하는 color
    QColor m_MotionBlockColor[2];
    ///motion table이 수정된적 있는지 check 하는 flag
    bool bMotionTableChanged[MAX_INTERNAL_CAMERA];

    void InitSetupCameraControls();
    void SetSetupCameraTreeControl();
    void SetSetupCameraButtonControls(int iChannel);
    QString SetMotionSensitivityValueText(int ivalue);
    void SetCameradata();
    void GetCameradata();
    void ResotreCamera();
    void GetCameraChanneldataToDlgData(int iChannel);

    void SetMotionAreaTable(int iChannel);
    void MotionControlsEnable(int iEnable);

//RECORD
	///현재 선택된 camera tree item id
    int  m_iRecordSelectId;
    ///현재 선택된 camera가 enable되지 않은경우 warning할 camera id
    int  m_iWarnRecordSelectId;
    ///camera tree items
    QTreeWidgetItem SetupRecordTreeItem[MAX_INTERNAL_CAMERA];
    ///record schedule table item
    QTableWidgetItem m_recordscheduleitem[MAX_DAY][MAX_HOUR];
    ///record schedule table header item
    QTableWidgetItem m_recordscheduleheaderitem[MAX_HOUR];
    ///camera icon
    QIcon RecordTreeItemIcon;
    ///현재 설정된 record event type mode
    int  m_iRecordEventTypeMode;
	///record event type mode에 따른 color values
    QColor m_RecordScheduleBlockColor[MAX_REC_SCHEDULE];


    void InitSetupRecordControls();
    void SetSetupRecordTreeControl();
    void SetSetupRecordButtonControls(int iChannel);
    void SetRecorddata();
    void GetRecorddata();
    void ResotreRecord();
    void GetRecordChanneldataToDlgData(int iChannel);
    QString SetRecordFramerateValueText(int ivalue);
    void SetRecordScheduleTable(int iChannel);
    QString SetRecordEventPrevDurationValueText(int ivalue);
    QString SetRecordEventPostDurationValueText(int ivalue);
//EVENT
	///event record link의 현재 event type
    int m_iEventRecordType;
    ///event alram link의 현재 event type
    int m_iEventAlarmType;

	///event-record link table
    int iTempLinkRecord[MAX_INTERNAL_CAMERA][MAX_INTERNAL_CAMERA];
    ///event-alram link table
    int iTempLinkAlarm[MAX_INTERNAL_CAMERA][MAX_ALARM];

	///event-record link event header item
    QTableWidgetItem m_eventrecordeventheaderitem[MAX_INTERNAL_CAMERA];
    ///event-record link record header item
    QTableWidgetItem m_eventrecordrecordheaderitem[MAX_INTERNAL_CAMERA];
    ///event-record link table item
    QTableWidgetItem m_eventrecorditem[MAX_INTERNAL_CAMERA][MAX_INTERNAL_CAMERA];

	///event-alram link event header item
    QTableWidgetItem m_eventalarmeventheaderitem[MAX_INTERNAL_CAMERA];
    ///event-alram link alram header item
    QTableWidgetItem m_eventalarmalarmheaderitem[MAX_ALARM];
    ///event-alram link table item
    QTableWidgetItem m_eventalarmitem[MAX_INTERNAL_CAMERA][MAX_ALARM];

    void InitSetupEventControls();
    void SetEventdata();
    void SetEventRecordTable(int iEventType);
    void SetEventAlarmTable(int iEventType);
    void GetEventdata();
    void ResotreEvent();

//AUDIO
	///현재 선택된 audio input tree id
    int  m_iAudioInSelectId;
    ///audio input tree items
    QTreeWidgetItem SetupAudioInTreeItem[MAX_AUDIOIN];
    ///audio icon
    QIcon AudioInTreeItemIcon;

    void InitSetupAudioControls();
    void SetSetupAudioInTreeControl();
    void SetAudiodata();
    void SetSetupAudioInButtonControls(int iChannel);
    void GetAudiodata();
    void ResotreAudio();
    void GetAudioInChanneldataToDlgData(int iChannel);
    QString SetAudioVolumeValueText(int ivalue);

//DIO
	///현재 선택된 sensor tree id
    int m_iSensorSelectId;
    ///현재 선택된 alram tree id
    int m_iAlarmSelectId;

	///sensor tree items
    QTreeWidgetItem SetupSensorTreeItem[MAX_SENSOR];
    ///sensor icon
    QIcon SensorTreeItemIcon;

	///alram tree items
    QTreeWidgetItem SetupAlarmTreeItem[MAX_ALARM];
    ///alram icon
    QIcon AlarmTreeItemIcon;

    void InitSetupDioControls();

    void SetDiodata();
    void GetDiodata();
    void ResotreDio();

    void SetSetupSensorTreeControl();
    void SetSetupSensorButtonControls(int iSensorId);
    void GetSensorItemdataToDlgData(int iSensorId);

    void SetSetupAlarmTreeControl();
    void SetSetupAlarmButtonControls(int iAlarmId);
    void GetAlarmItemdataToDlgData(int iAlarmId);
    QString SetAlarmDelayTimeValueText(int ivalue);

//NETWORK
    void InitSetupNetworkControls();
    void SetNetworkdata();
    void GetNetworkdata();
    void ResotreNetwork();
    void read_ui_network_data(int devno, dvr_net_info_t *inet);

//STORAGE
	///warning messsage box control
    WarningMessage MsgBox;
    ///format warning message update timer
    QTimer *FormatMsgtimer;
    ///format thread
    FormatThread *pFormatThread;
    ///disk info table items
    QTableWidgetItem m_diskinfotableitem[MAX_DEVLIST][5];
    ///format confirm dialog control
    dvr_formatconfirm *m_pConfirmBox;
    void InitSetupStorageControls();
    void SetStoragedata();
    void GetStoragedata();
    void ResotreStorage();
    void StartFormat();
    void CancelFormat();
    bool CheckFormattable(int idx);

//SYSTEM
	///system user list tree items
    QListWidgetItem SetupUserListItem[MAX_USER];
    ///user icon
    QIcon UserListItemIcon;
    ///현재 선택된 user tree id
    int m_iUserSelectId;
    ///current time update timer
    QTimer *Currenttimer;
    ///current timer flag
    int iCurrentTimerOn;
    ///system time modify flag
    bool m_bChangeSystemTime;

    void InitSetupSystemControls();
    void SetSystemdata();
    void GetSystemdata();
    void ResotreSystem();
    void SetDateFormat(int iDateFormat);
    void UpdateDateTimeCurrent(struct tm *ptm);
    void UpdateDateTimeSet(struct tm *ptm);
    void CurrentTimerStart();
    void CurrentTimerStop();
    void SetSystemUserdata();
    void SetSystemUserListControl();
    void GetSystemUserdata();
    void GetSetupSystemUserListItemdataToUserListdata(int iUser);
    void SetSetupSystemUserButtonControls(int iUser);
    void SystemConfigInit();
    void SystemConfigInitCancel();

//BACKUP
	///backup camera tree items
    QTreeWidgetItem BackupTreeItem[MAX_INTERNAL_CAMERA];
    ///camera icon
    QIcon BackupTreeItemIcon;
    ///현재 선택된 camera tree id
    int  m_ibackupSelectId;
    ///backup info date/time
    struct tm backupinfotm;
    ///record data info
    int iMonthTable[PLAYCALENDARMAXDAYCOUNT];
    ///timetable items
    QTableWidgetItem m_tableitem[1][144];
    ///time table header items
    QTableWidgetItem m_tableheaderitem[PLAYBACKTIMELINE_24];
    ///backup warning message update timer
    QTimer *BackupMsgtimer;

    void InitSetupBackupControls();
    void SetBackupdata();
    void GetBackupdata();
    void ResotreBackup();
    void GetCalendarSelectDay(struct tm * ptm);
    void SetCalendarSelectDay(struct tm * ptm);
    void SetCalendarBoldRecordDate(struct tm * ptm,int iMonthTable[PLAYCALENDARMAXDAYCOUNT]);
    void SetTimeTable();
    void GetTimeTableSelectTime(struct tm * ptm);
    void SetTimeTableUpdate();
    void UpdateBackupInfoDateTime(struct tm *ptm);
    void BackupComplete();


    virtual void showEvent ( QShowEvent * event );
    int get_sys_info(void);
    int get_network_info(void);
    int set_network_info(int devno);
    int get_disk_info(void);

public slots:
    void SaveExitSlot();
    void CancelExitSlot();
    void ApplySetupBtnSlot();
    void ChangeCurrentTabIndex(int index);
    void ChangeOutputSelectSlot(int index);
    void ChangeSequenceIntevalSlot(int value);
    void ChangeSubSequenceIntevalSlot(int value);
   // void on_onemonitorcheckBox_stateChanged(int );

    void SetupCameraTreeItemClicked(QTreeWidgetItem *item, int column);
    void SetupCameraTreeItemDoubleClicked(QTreeWidgetItem *item, int column);
    void ChangeMotionSensitivitySlot(int value);
    void ChangeMotionAreaModeSlot(int index);
    void MotionAreaAllClearModeBtnSlot();
    void MotionAreaAllSetModeBtnSlot();
    void MotionAreaSelectSlot ( int row, int column );
    void on_motionenable_currentIndexChanged(int index);
    void ColorDefaultBtnSlot();
    void CameraTitleVirtualKeyboardBtnSlot();
    void ColorBrightnessVirtualKeyboardBtnSlot();
    void ColorContrastVirtualKeyboardBtnSlot();
    void ColorSaturationVirtualKeyboardBtnSlot();
    void on_applyallchpushButton_clicked();

    void SetupRecordTreeItemClicked(QTreeWidgetItem *item, int column);
    void SetupRecordTreeItemDoubleClicked(QTreeWidgetItem *item, int column);
    void ChangeRecordframerateSlot(int value);
    void ChangeRecordbitrateSlot(int value);
    void ChangeRecordscheduleEventTypeModeSlot(int index);
    void RecordScheduleTableSelectSlot ( int row, int column );
    void RecordScheduleApplyAllBtnSlot();
    void ChangePrevRecordDurationSlot(int value);
    void ChangePostRecordDurationSlot(int value);
    void on_recordapplyallchpushButton_clicked();


    void ChangeEventRecordTypeModeSlot(int index);
    void ChangeEventAlarmTypeModeSlot(int index);
    void EventRecordTableSelectSlot ( int row, int column );
    void EventAlarmTableSelectSlot ( int row, int column );


    void SetupAudioInTreeItemClicked(QTreeWidgetItem *item, int column);
    void SetupAudioInTreeItemDoubleClicked(QTreeWidgetItem *item, int column);
    void ChangeAudioInVolumeSlot(int value);
    void ChangeAudioOutVolumeSlot(int value);

    void SetupSensorTreeItemClicked(QTreeWidgetItem *item, int column);
    void SetupSensorTreeItemDoubleClicked(QTreeWidgetItem *item, int column);

    void SetupAlarmTreeItemClicked(QTreeWidgetItem *item, int column);
    void SetupAlarmTreeItemDoubleClicked(QTreeWidgetItem *item, int column);
    void ChangeAlarmDelayTimeSlot(int value);

    void ChangeNetworkType1SelectSlot(int index);
    void ChangeNetworkType2SelectSlot(int index);

    void NetworkIpaddress1VirtualKeyboardBtnSlot();
    void NetworkSubnetmask1VirtualKeyboardBtnSlot();
    void NetworkGateway1VirtualKeyboardBtnSlot();

    void NetworkIpaddress2VirtualKeyboardBtnSlot();
    void NetworkSubnetmask2VirtualKeyboardBtnSlot();
    void NetworkGateway2VirtualKeyboardBtnSlot();

    void StorageFormatBtnSlot();
    void updateFormatWarn ( );
    void on_pb_disk_refresh_clicked();

    void SystemConfigInitBtnSlot();
    void SystemRebootBtnSlot();
    void SystemDateTimeApplyBtnSlot();
    void CurrentTimerUpdate();
    void SetupSystemUSerItemClicked(QListWidgetItem *item);
    void SetupSystemUSerItemDoubleClicked(QListWidgetItem *item);
    void on_systemdatetimesetdateTimeEdit_editingFinished();

    void SystemUserApplySaveBtnSlot();
    void SystemUserCurrentPasswordVirtualKeyboardBtnSlot();
    void SystemUserConfirmPasswordVirtualKeyboardBtnSlot();

    void SetupBackupTreeItemClicked(QTreeWidgetItem *item, int column);
    void SetupBackupTreeItemDoubleClicked(QTreeWidgetItem *item, int column);
    void BackupStartBtnSlot();
    void BackupInfoCalendarSelectionChangedSlot();
    void BackupInfoCalendarcurrentPageChangedSlot(int year, int month);
    void TimeTableSelectionChangedSlot();
    void SetBackupStartTimeBtnSlot();
    void SetBackupEndTimeBtnSlot();
    void updateBackupWarn ( );
    void on_backupmediacomboBox_currentIndexChanged(int index);
    void on_backuptypecomboBox_currentIndexChanged(int index);


protected:
    void changeEvent(QEvent *e);

private:
    Ui::dvr_setup *ui;

};


#endif
