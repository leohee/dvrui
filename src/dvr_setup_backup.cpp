/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/

#include "dvr_defines.h"
#include "dvr_setup.h"
#include "ui_dvr_setup.h"
#include "dvr_main.h"
#include <QTextCharFormat>
/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define KB			(1024)
#define MB			(KB*KB)
/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
extern dvrmain *pMainClass;
extern dvr_timelinebar_play *pTimebar;

extern int MsgBoxTimerCount;
/*----------------------------------------------------------------------------
 local functions
-----------------------------------------------------------------------------*/
/*!backup tab의 control들을 초기화 */
void dvr_setup::InitSetupBackupControls()
{
    QSize qsize;
    m_ibackupSelectId=0;

    qsize.setWidth(CAMERATREEICON_SIZE_W);
    qsize.setHeight(CAMERATREEICON_SIZE_H);
    BackupTreeItemIcon.addFile(QString::fromUtf8(CAMERA_TREE_CAMERA_ICON), qsize, QIcon::Normal, QIcon::Off);

    connect(ui->backuptreewidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(SetupBackupTreeItemClicked(QTreeWidgetItem *, int)));
    connect(ui->backuptreewidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(SetupBackupTreeItemDoubleClicked(QTreeWidgetItem *, int)));
    connect(ui->backupstartpushButton, SIGNAL(clicked()), this, SLOT(BackupStartBtnSlot()));
    connect(ui->backupinfocalendar, SIGNAL(selectionChanged()), this, SLOT(BackupInfoCalendarSelectionChangedSlot()));
    connect(ui->backupinfocalendar, SIGNAL(currentPageChanged(int,int)), this, SLOT(BackupInfoCalendarcurrentPageChangedSlot(int,int)));
    connect(ui->timetable, SIGNAL(itemSelectionChanged()), this, SLOT(TimeTableSelectionChangedSlot()));
    connect(ui->backupstarttimepushButton, SIGNAL(clicked()), this, SLOT(SetBackupStartTimeBtnSlot()));
    connect(ui->backupendtimepushButton, SIGNAL(clicked()), this, SLOT(SetBackupEndTimeBtnSlot()));
}
/*!profile data struct에 따라서 ui에 value setting */
void dvr_setup::SetBackupdata()
{
    for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
    {
        if(dlgruntimesettings.backupinfo.iDestChannels[i])
            BackupTreeItem[i].setCheckState(0,Qt::Checked);
        else
            BackupTreeItem[i].setCheckState(0,Qt::Unchecked);
    }
    //backup start date/time에 설정 시간 set 
    struct tm tmcurtime;
    time_t curtime;
    QDateTime qdatetime;
    memcpy(&tmcurtime,&dlgruntimesettings.backupinfo.tmFromBackupTime,sizeof(struct tm));
    tmcurtime.tm_year-=1900;
    curtime = mktime(&tmcurtime);
    qdatetime.setTime_t(curtime);
    ui->backupstartdateTimeEdit->setDateTime(qdatetime);

	//backup end date/time에 설정 시간 set
    memcpy(&tmcurtime,&dlgruntimesettings.backupinfo.tmToBackupTime,sizeof(struct tm));
    tmcurtime.tm_year-=1900;
    curtime = mktime(&tmcurtime);
    qdatetime.setTime_t(curtime);
    ui->backupenddateTimeEdit->setDateTime(qdatetime);

	//backup type, media, select device set
    ui->backuptypecomboBox->setCurrentIndex(dlgruntimesettings.backupinfo.iBackupFileType);
    ui->backupmediacomboBox->setCurrentIndex(dlgruntimesettings.backupinfo.iBackupMediaType);
    if(dlgruntimesettings.backupinfo.iBackupMediaType == 0)
    {
    	//basket type일때 select device hide
        ui->backupselectdevicelabel->hide();
        ui->backupselectdevicecomboBox->hide();
    }
    else
    {
    	//disk info로부터 device를 combo box에 추가
        get_disk_info();
        ui->backupselectdevicecomboBox->clear();
        for(int i=0;i<mp.cnt;i++)
        {
            if((mp.dev[i].state == DONE_FORMAT || mp.dev[i].state == DONE_EXT3) && mp.rec_used[i]==0)
                ui->backupselectdevicecomboBox->addItem(mp.dev[i].dir);
        }
        if(ui->backupselectdevicecomboBox->count()>1)
        {
            ui->backupselectdevicecomboBox->setEnabled(true);
        }
        else
        {
            ui->backupselectdevicecomboBox->setEnabled(false);
        }
        ui->backupselectdevicelabel->show();
        ui->backupselectdevicecomboBox->show();
    }
    ui->backuptreewidget->setCurrentItem(&BackupTreeItem[m_ibackupSelectId]);

	//basket인경우 camera select를 막고, avi인경우만 camera select가 가능
    if(dlgruntimesettings.backupinfo.iBackupFileType == 0)  //Basket
    {
        ui->backuptreewidget->setEnabled(false);
    }
    else            //AVI
    {
        ui->backuptreewidget->setEnabled(true);
    }

	//Calendar 및 time bar set
    SetCalendarSelectDay(&backupinfotm);
    memset(iMonthTable,0,sizeof(iMonthTable));
    LIB816x_GetRecDays(backupinfotm,iMonthTable);
    SetCalendarBoldRecordDate(&backupinfotm,iMonthTable);
    pTimebar->GetRecordHourInMinuteDataProfile(backupinfotm);
    SetTimeTable();
    UpdateBackupInfoDateTime(&backupinfotm);
}
/*!ui control의 value를 읽고 해당 기능을 적용시킴 */
void dvr_setup::GetBackupdata()
{
    for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
    {
        if(runtimesettings.backupinfo.iDestChannels[i] != dlgruntimesettings.backupinfo.iDestChannels[i])
            runtimesettings.backupinfo.iDestChannels[i] = dlgruntimesettings.backupinfo.iDestChannels[i];
    }
	//read backup start time
    QDateTime qdatetime=ui->backupstartdateTimeEdit->dateTime();
    time_t settime;
    settime=qdatetime.toTime_t();
    memcpy(&dlgruntimesettings.backupinfo.tmFromBackupTime,localtime(&settime),sizeof(struct tm));
    dlgruntimesettings.backupinfo.tmFromBackupTime.tm_year+=1900;

	//read backup end time
    qdatetime=ui->backupenddateTimeEdit->dateTime();
    settime=qdatetime.toTime_t();
    memcpy(&dlgruntimesettings.backupinfo.tmToBackupTime,localtime(&settime),sizeof(struct tm));
    dlgruntimesettings.backupinfo.tmToBackupTime.tm_year+=1900;

    memcpy(&runtimesettings.backupinfo.tmFromBackupTime,&dlgruntimesettings.backupinfo.tmFromBackupTime,sizeof(struct tm));
    memcpy(&runtimesettings.backupinfo.tmToBackupTime,&dlgruntimesettings.backupinfo.tmToBackupTime,sizeof(struct tm));

	//read backup type, media
    dlgruntimesettings.backupinfo.iBackupFileType = ui->backuptypecomboBox->currentIndex();
    if(runtimesettings.backupinfo.iBackupFileType != dlgruntimesettings.backupinfo.iBackupFileType)
        runtimesettings.backupinfo.iBackupFileType = dlgruntimesettings.backupinfo.iBackupFileType;

    dlgruntimesettings.backupinfo.iBackupMediaType = ui->backupmediacomboBox->currentIndex();
    if(runtimesettings.backupinfo.iBackupMediaType != dlgruntimesettings.backupinfo.iBackupMediaType)
        runtimesettings.backupinfo.iBackupMediaType = dlgruntimesettings.backupinfo.iBackupMediaType;
}
/*!cancel시 backup관련 변경적용된 내역을 복원시킴 */
void dvr_setup::ResotreBackup()
{
    for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
    {
        if(runtimesettings.backupinfo.iDestChannels[i] != dlgruntimesettings.backupinfo.iDestChannels[i])
            runtimesettings.backupinfo.iDestChannels[i] = dlgruntimesettings.backupinfo.iDestChannels[i];
    }
    memcpy(&runtimesettings.backupinfo.tmFromBackupTime,&dlgruntimesettings.backupinfo.tmFromBackupTime,sizeof(struct tm));
    memcpy(&runtimesettings.backupinfo.tmToBackupTime,&dlgruntimesettings.backupinfo.tmToBackupTime,sizeof(struct tm));

    if(runtimesettings.backupinfo.iBackupFileType != dlgruntimesettings.backupinfo.iBackupFileType)
        runtimesettings.backupinfo.iBackupFileType = dlgruntimesettings.backupinfo.iBackupFileType;

    if(runtimesettings.backupinfo.iBackupMediaType != dlgruntimesettings.backupinfo.iBackupMediaType)
        runtimesettings.backupinfo.iBackupMediaType = dlgruntimesettings.backupinfo.iBackupMediaType;
}
/*!backup calendar에 record status 표시 */
void dvr_setup::SetCalendarBoldRecordDate(struct tm * ptm,int iMonthTable[PLAYCALENDARMAXDAYCOUNT])
{

    QDate qmonthday;
    QTextCharFormat qtextformat;
    qmonthday.setDate(ptm->tm_year,ptm->tm_mon+1,ptm->tm_mday);
    int monthindays=qmonthday.daysInMonth();
	//calendar에서 record data가 있는 날은 red, record data가 없는 날은 gray 표시 
    QBrush brush;
    for(int i=0;i<monthindays;i++)
    {
        qmonthday.setDate(ptm->tm_year,ptm->tm_mon+1,i+1);
        if(iMonthTable[i]!=0)
        {
            brush.setColor(Qt::red);
        }
        else
        {
            brush.setColor(Qt::darkGray);
        }
        qtextformat.setForeground(brush);
        ui->backupinfocalendar->setDateTextFormat (qmonthday, qtextformat );
    }
}
/*!선택한 날자를 calendar에 setting */
void dvr_setup::SetCalendarSelectDay(struct tm * ptm)
{
    QDate qselectday;
    qselectday.setDate(ptm->tm_year,ptm->tm_mon+1,ptm->tm_mday);
    ui->backupinfocalendar->setSelectedDate(qselectday);
}
/*!record status를 timebar에 표시 */
void dvr_setup::SetTimeTable()
{
    int i=0;
	// 1일을 144개 time table로 나눠서 표시 
    for(i=0;i<144;i++)
    {
          ui->timetable->takeItem(0,i);
    }
    for(i=0;i<24;i++)
    {
         ui->timetableheader->takeItem(0,i);
    }

    ui->timetable->setRowCount(1);
    ui->timetable->setRowHeight(0,31);
    ui->timetable->setColumnCount(144);
    for(i=0;i<144;i++)
    {
        ui->timetable->setColumnWidth(i,4);
    }

    ui->timetableheader->setRowCount(1);
    ui->timetableheader->setColumnCount(24);
    for(i=0;i<24;i++)
    {
        ui->timetableheader->setColumnWidth(i,24);
        char buf[MAX_CHAR_4];
        sprintf(buf,"%d",i);
        m_tableheaderitem[i].setText(StrToQString(buf));
        ui->timetableheader->setItem(0,i,&m_tableheaderitem[i]);
    }
	//time table data update 
    SetTimeTableUpdate();
}
/*!record status를 timebar에 표시 */
void dvr_setup::SetTimeTableUpdate()
{
    int filetype;
    QColor           m_itemcolor[2];
	
	//basket인 경우 전체 camera에 대해서
	//avi인경우 선택한 camera에 대해서 표시 
    filetype = ui->backuptypecomboBox->currentIndex();
    if(filetype == 0)   //Basket
        ui->timetablechannel1->setText(StrToQString((char*)"all camera"));
    else
        ui->timetablechannel1->setText(StrToQString(profile.camera[m_ibackupSelectId].strcameraname));

	//녹화 data가 있는 time구간은 red
	//녹화 data가 없는 time구간은 gray 
    m_itemcolor[0]=Qt::lightGray;
    m_itemcolor[1]=Qt::red;

    if(filetype == 0)
    {
        int ievent[144];
        memset(ievent,0,sizeof(ievent));
        for(int i=0;i<144;i++)
        {
            int icountcol=0;

            icountcol=i*10;

            for(int ch=0; ch<MAX_INTERNAL_CAMERA && ievent[i]==0 ; ch++)
            {
                if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[ch][icountcol]!=0)
                    ievent[i]=1;
                else if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[ch][icountcol+1]!=0)
                    ievent[i]=1;
                else if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[ch][icountcol+2]!=0)
                    ievent[i]=1;
                else if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[ch][icountcol+3]!=0)
                    ievent[i]=1;
                else if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[ch][icountcol+4]!=0)
                    ievent[i]=1;
                else if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[ch][icountcol+5]!=0)
                    ievent[i]=1;
                else if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[ch][icountcol+6]!=0)
                    ievent[i]=1;
                else if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[ch][icountcol+7]!=0)
                    ievent[i]=1;
                else if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[ch][icountcol+8]!=0)
                    ievent[i]=1;
                else if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[ch][icountcol+9]!=0)
                    ievent[i]=1;

                if(ievent[i])
                    break;
            }
            ui->timetable->takeItem(0,i);
            m_tableitem[0][i].setBackgroundColor(m_itemcolor[ievent[i]]);
            ui->timetable->setItem(0, i,&m_tableitem[0][i]);
        }
    }
    else
    {
        int ievent[144];
        memset(ievent,0,sizeof(ievent));
        for(int i=0;i<144;i++)
        {
            int icountcol=0;

            icountcol=i*10;

            for(int ch=0; ch<MAX_INTERNAL_CAMERA  && ievent[i]==0 ; ch++)
            {
                if(!dlgruntimesettings.backupinfo.iDestChannels[ch])
                    continue;

                if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[ch][icountcol]!=0)
                    ievent[i]=1;
                else if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[ch][icountcol+1]!=0)
                    ievent[i]=1;
                else if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[ch][icountcol+2]!=0)
                    ievent[i]=1;
                else if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[ch][icountcol+3]!=0)
                    ievent[i]=1;
                else if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[ch][icountcol+4]!=0)
                    ievent[i]=1;
                else if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[ch][icountcol+5]!=0)
                    ievent[i]=1;
                else if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[ch][icountcol+6]!=0)
                    ievent[i]=1;
                else if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[ch][icountcol+7]!=0)
                    ievent[i]=1;
                else if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[ch][icountcol+8]!=0)
                    ievent[i]=1;
                else if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[ch][icountcol+9]!=0)
                    ievent[i]=1;

                if(ievent[i])
                    break;
            }
            ui->timetable->takeItem(0,i);
            m_tableitem[0][i].setBackgroundColor(m_itemcolor[ievent[i]]);
            ui->timetable->setItem(0, i,&m_tableitem[0][i]);
        }
    }
}
/*!calendar에 선택한 date를 얻어오는 기능 수행 */
void dvr_setup::GetCalendarSelectDay(struct tm * ptm)
{
	//Calendar에서 선택한 date를 얻어옴
    QDate qselectday=ui->backupinfocalendar->selectedDate();
    qselectday.getDate(&ptm->tm_year,&ptm->tm_mon,&ptm->tm_mday);
    ptm->tm_mon=ptm->tm_mon-1;
    UpdateBackupInfoDateTime(ptm);
}
/*!timebar에서 선택한 time을 얻어오는 기능 수행 */
void dvr_setup::GetTimeTableSelectTime(struct tm * ptm)
{
	//time bar에서 선택한 시간을 읽음
    int icount=0;
    int icursel=0;
    QList <QTableWidgetItem *> tablelist;
    QTableWidgetItem *selectedtableitem=NULL;
    tablelist=ui->timetable->selectedItems();
    if(tablelist.count())
        selectedtableitem=tablelist.takeFirst();

    icount=0;
    for(int j=0;j<144;j++)
    {
        if(&m_tableitem[0][j]==selectedtableitem)
        {
            break;
        }
        else
            icount++;
    }

    icursel=icount*10;
    ptm->tm_hour=icursel/60;
    ptm->tm_min=icursel%60;
}
/*!선택된 date/time을 edit창에 표시 */
void dvr_setup::UpdateBackupInfoDateTime(struct tm *ptm)
{
	//calendar, timebar에서 선택한 date/time을 보여줌
    struct tm tmcurtime;
    time_t curtime;
    QDateTime qdatetime;
    ptm->tm_sec = 0;
    memcpy(&tmcurtime,ptm,sizeof(struct tm));
    tmcurtime.tm_year=ptm->tm_year-1900;
    curtime = mktime(&tmcurtime);
    qdatetime.setTime_t(curtime);
    ui->backupinfodatetimeedit->setDateTime(qdatetime);
}
/*!backup complete시 호출 */
void dvr_setup::BackupComplete()
{
    if(BackupMsgtimer)
    {
        BackupMsgtimer->stop();
        delete BackupMsgtimer;
    }
    MsgBox.hide();
    setEnabled(true);
    BackupMsgtimer = NULL;
    show();
}
/*!현재 backup중이라는 waring message를 표시 */
void dvr_setup::updateBackupWarn (  )
{
    MsgBoxTimerCount++;
    MsgBoxTimerCount%=5;
    switch(MsgBoxTimerCount)
    {
    case 0:
        MsgBox.SetText("wait for BACKUP processing..");
        break;
    case 1:
        MsgBox.SetText("wait for BACKUP processing....");
        break;
    case 2:
        MsgBox.SetText("wait for BACKUP processing......");
        break;
    case 3:
        MsgBox.SetText("wait for BACKUP processing........");
        break;
    case 4:
        MsgBox.SetText("wait for BACKUP processing..........");
        break;
    }
}
/*!backup camera tree가 click되면 호출 */
void dvr_setup::SetupBackupTreeItemClicked(QTreeWidgetItem *item, int column)
{
    int iselect=0;
    iselect=ui->backuptreewidget->indexOfTopLevelItem(item);

    if(item->checkState(column)==Qt::Unchecked)
    {
        dlgruntimesettings.backupinfo.iDestChannels[iselect] = 0;
    }
    else
    {
        dlgruntimesettings.backupinfo.iDestChannels[iselect] = 1;
    }
    m_ibackupSelectId=iselect;
    SetTimeTableUpdate();
}
/*!backup camera tree가 double click되면 호출 */
void dvr_setup::SetupBackupTreeItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    SetupBackupTreeItemClicked(item,column);
}
/*!backup start버튼이 눌릴때 호출 */
void dvr_setup::BackupStartBtnSlot()
{
    struct tm tmtime;
    time_t starttime,endtime;
    char strMedia[50],strFile[50];

    GetBackupdata();

    //time validation check
    memcpy(&tmtime,&runtimesettings.backupinfo.tmFromBackupTime,sizeof(struct tm));
    tmtime.tm_year-=1900;
    starttime = mktime(&tmtime);
    memcpy(&tmtime,&runtimesettings.backupinfo.tmToBackupTime,sizeof(struct tm));
    tmtime.tm_year-=1900;
    endtime = mktime(&tmtime);

    if(starttime >= endtime)
    {
        pMainClass->ShowNotifyDialog((char*)"check time setting");
        return;
    }

    //camera selection validate check
    if(runtimesettings.backupinfo.iBackupFileType==1)   //AVI
    {
        bool bFind=false;
        for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
        {
            if(runtimesettings.backupinfo.iDestChannels[i])
            {
                bFind=true;
                break;
            }
        }
        if(bFind==false)
        {
            pMainClass->ShowNotifyDialog((char*)"check camera selection");
            return;
        }
    }

    BackupMsgtimer=new QTimer(this);
    connect(BackupMsgtimer, SIGNAL(timeout()), this, SLOT(updateBackupWarn()));
    MsgBoxTimerCount=0;
    MsgBox.SetText("wait for BACKUP processing..");
    MsgBox.show();
    BackupMsgtimer->start(1000);
    setEnabled(false);

    if(runtimesettings.backupinfo.iBackupFileType==0)   //basket
    {
        sprintf(strFile,"File Type : basket");
#if DVR_API_ENABLE
        int chmask=0;
        for(int i=0;i<MAX_INTERNAL_CAMERA; i++)
        {
        	chmask|=(1<<i);
        }
        if(runtimesettings.backupinfo.iBackupMediaType==0)  //CD/DVD
        {
            sprintf(strMedia,"Media Type : CD/DVD");
            char path[128];
            memset(path,'\0',128);
            for(int i=0;i<mp.cnt; i++)
            {
                if(mp.dev[i].state == DISK_CDROM)
                    sprintf(path,"%s",mp.dev[i].dev);
            }
            if(strlen(path)==0)
            {
                get_disk_info();
                for(int i=0;i<mp.cnt; i++)
                {
                    if(mp.dev[i].state == DISK_CDROM)
                        sprintf(path,"%s",mp.dev[i].dev);
                }
            }
            dprintf("backup path = %s\n",path);
            LIB816x_backupToBASKET(runtimesettings.backupinfo.iBackupMediaType, path, chmask, runtimesettings.backupinfo.tmFromBackupTime, runtimesettings.backupinfo.tmToBackupTime) ;
        }
        else
        {
            sprintf(strMedia,"Media Type : USB");
            char path[128];
            memset(path,'\0',128);
            sprintf(path,"%s",QStringToStr(ui->backupselectdevicecomboBox->currentText()));
            dprintf("backup path = %s\n",path);
            LIB816x_backupToBASKET(runtimesettings.backupinfo.iBackupMediaType, path, chmask, runtimesettings.backupinfo.tmFromBackupTime, runtimesettings.backupinfo.tmToBackupTime) ;
        }
        char str[200];
        sprintf(str, "backup start\n\t %s\n\t %s\n \tstart Time:%d.%d.%d %d:%d:%d\n \tend Time:%d.%d.%d %d:%d:%d",strFile,strMedia,
                runtimesettings.backupinfo.tmFromBackupTime.tm_year,runtimesettings.backupinfo.tmFromBackupTime.tm_mon+1,runtimesettings.backupinfo.tmFromBackupTime.tm_mday,
                runtimesettings.backupinfo.tmFromBackupTime.tm_hour, runtimesettings.backupinfo.tmFromBackupTime.tm_min, runtimesettings.backupinfo.tmFromBackupTime.tm_sec,
                runtimesettings.backupinfo.tmToBackupTime.tm_year, runtimesettings.backupinfo.tmToBackupTime.tm_mon+1, runtimesettings.backupinfo.tmToBackupTime.tm_mday,
                runtimesettings.backupinfo.tmToBackupTime.tm_hour, runtimesettings.backupinfo.tmToBackupTime.tm_min, runtimesettings.backupinfo.tmToBackupTime.tm_sec);
        pMainClass->EventLogAdd(str,true);
#endif
    }
    else
    {
        sprintf(strFile,"File Type : AVI");
        int chmask=0;
        for(int i=0;i<MAX_INTERNAL_CAMERA; i++)
        {
            if(runtimesettings.backupinfo.iDestChannels[i])
                chmask|=(1<<i);
        }
#if DVR_API_ENABLE
        if(runtimesettings.backupinfo.iBackupMediaType==0)  //CD/DVD
        {
            sprintf(strMedia,"Media Type : CD/DVD");
            char path[128];
            memset(path,'\0',128);
            for(int i=0;i<mp.cnt; i++)
            {
                if(mp.dev[i].state == DISK_CDROM)
                    sprintf(path,"%s",mp.dev[i].dev);
            }
            if(strlen(path)==0)
            {
                get_disk_info();
                for(int i=0;i<mp.cnt; i++)
                {
                    if(mp.dev[i].state == DISK_CDROM)
                        sprintf(path,"%s",mp.dev[i].dev);
                }
            }
            dprintf("backup path = %s\n",path);
            LIB816x_backupToAVI(runtimesettings.backupinfo.iBackupMediaType, path, chmask, runtimesettings.backupinfo.tmFromBackupTime, runtimesettings.backupinfo.tmToBackupTime) ;
        }
        else    //USB
        {
            sprintf(strMedia,"Media Type : USB");
            char path[128];
            memset(path,'\0',128);
            sprintf(path,"%s",QStringToStr(ui->backupselectdevicecomboBox->currentText()));
            dprintf("backup path = %s\n",path);
            LIB816x_backupToAVI(runtimesettings.backupinfo.iBackupMediaType, path, chmask, runtimesettings.backupinfo.tmFromBackupTime, runtimesettings.backupinfo.tmToBackupTime) ;
        }
        char str[200];
        sprintf(str, "backup start\n\t %s\n\t %s\n \tstart Time:%d.%d.%d %d:%d:%d\n \tend Time:%d.%d.%d %d:%d:%d",strFile,strMedia,
                runtimesettings.backupinfo.tmFromBackupTime.tm_year,runtimesettings.backupinfo.tmFromBackupTime.tm_mon+1,runtimesettings.backupinfo.tmFromBackupTime.tm_mday,
                runtimesettings.backupinfo.tmFromBackupTime.tm_hour, runtimesettings.backupinfo.tmFromBackupTime.tm_min, runtimesettings.backupinfo.tmFromBackupTime.tm_sec,
                runtimesettings.backupinfo.tmToBackupTime.tm_year, runtimesettings.backupinfo.tmToBackupTime.tm_mon+1, runtimesettings.backupinfo.tmToBackupTime.tm_mday,
                runtimesettings.backupinfo.tmToBackupTime.tm_hour, runtimesettings.backupinfo.tmToBackupTime.tm_min, runtimesettings.backupinfo.tmToBackupTime.tm_sec);
        pMainClass->EventLogAdd(str,true);
#endif
    }
    hide();
}
/*!backup media type이 변경될때 호출 */
void dvr_setup::on_backupmediacomboBox_currentIndexChanged(int index)
{
    if(index == 0)
    {
        ui->backupselectdevicelabel->hide();
        ui->backupselectdevicecomboBox->hide();
    }
    else
    {
        get_disk_info();
        ui->backupselectdevicecomboBox->clear();
        for(int i=0;i<mp.cnt;i++)
        {
            if((mp.dev[i].state == DONE_FORMAT || mp.dev[i].state == DONE_EXT3) && mp.rec_used[i]==0)
                ui->backupselectdevicecomboBox->addItem(mp.dev[i].dir);
        }
        if(ui->backupselectdevicecomboBox->count()>1)
        {
            ui->backupselectdevicecomboBox->setEnabled(true);
        }
        else
        {
            ui->backupselectdevicecomboBox->setEnabled(false);
        }
        ui->backupselectdevicelabel->show();
        ui->backupselectdevicecomboBox->show();
    }
}
/*!calendar의 선택값이 변경될때 호출 */
void dvr_setup::BackupInfoCalendarSelectionChangedSlot()
{
    GetCalendarSelectDay(&backupinfotm);
    memset(iMonthTable,0,sizeof(iMonthTable));
    LIB816x_GetRecDays(backupinfotm,iMonthTable);
    SetCalendarBoldRecordDate(&backupinfotm,iMonthTable);
    pTimebar->GetRecordHourInMinuteDataProfile(backupinfotm);
    SetTimeTableUpdate();
}
/*!calendar page가 변경될때 호출 */
void dvr_setup::BackupInfoCalendarcurrentPageChangedSlot(int year, int month)
{
    struct tm tmptm;
    backupinfotm.tm_year=year;
    backupinfotm.tm_mon=month-1;
    GetCalendarSelectDay(&tmptm);
    backupinfotm.tm_mday = tmptm.tm_mday;
    SetCalendarSelectDay(&backupinfotm);
}
/*!timebar 선택이 변경될때 호출 */
void dvr_setup::TimeTableSelectionChangedSlot()
{
    GetTimeTableSelectTime(&backupinfotm);
    UpdateBackupInfoDateTime(&backupinfotm);
}
/*!set backup start time 버튼이 눌릴때 호출 */
void dvr_setup::SetBackupStartTimeBtnSlot()
{
	//set start time이 눌리면 현재 설정된 backup info time을 backup start time에 set
    struct tm tmcurtime;
    time_t curtime;
    QDateTime qdatetime;
    memcpy(&tmcurtime,&backupinfotm,sizeof(struct tm));
    //find first
    int timeidx = tmcurtime.tm_hour*60 + tmcurtime.tm_min;
    if(ui->backuptypecomboBox->currentIndex() == 0)
    {
        bool bfindfirst = false;
        for(int i=0;i<10 && (bfindfirst==false);i++)
        {
            for(int ch=0; ch<MAX_INTERNAL_CAMERA && (bfindfirst==false); ch++)
            {
                if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[ch][timeidx+i])
                {
                    tmcurtime.tm_min = (timeidx+i)%60;
                    bfindfirst = true;
                    break;
                }
            }
        }
    }
    else
    {
        bool bfindfirst = false;
        for(int i=0;i<10 && (bfindfirst==false);i++)
        {
            for(int ch=0; ch<MAX_INTERNAL_CAMERA && (bfindfirst==false); ch++)
            {
                if(!dlgruntimesettings.backupinfo.iDestChannels[ch])
                    continue;

                if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[ch][timeidx+i])
                {
                    tmcurtime.tm_min = (timeidx+i)%60;
                    bfindfirst = true;
                    break;
                }
            }
        }
    }
    tmcurtime.tm_year=backupinfotm.tm_year-1900;
    curtime = mktime(&tmcurtime);
    qdatetime.setTime_t(curtime);
    ui->backupstartdateTimeEdit->setDateTime(qdatetime);
    tmcurtime.tm_year=backupinfotm.tm_year;
    memcpy(&dlgruntimesettings.backupinfo.tmFromBackupTime,&tmcurtime,sizeof(struct tm));
}
/*!set backup end time 버튼이 눌릴때 호출 */
void dvr_setup::SetBackupEndTimeBtnSlot()
{
	//set end time이 눌리면 현재 설정된 backup info time을 backup end time에 set
    struct tm tmcurtime;
    time_t curtime;
    QDateTime qdatetime;
    memcpy(&tmcurtime,&backupinfotm,sizeof(struct tm));
    //find last
    int timeidx = tmcurtime.tm_hour*60 + tmcurtime.tm_min + 10;
    if(ui->backuptypecomboBox->currentIndex() == 0)
    {
        bool bfindfirst = false;
        for(int i=0;i<10 && (bfindfirst==false);i++)
        {
            for(int ch=0; ch<MAX_INTERNAL_CAMERA && (bfindfirst==false); ch++)
            {
                if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[ch][timeidx-i])
                {
                    tmcurtime.tm_hour = (timeidx-i)/60;
                    tmcurtime.tm_min = (timeidx-i)%60;
                    bfindfirst = true;
                    break;
                }
            }
        }
    }
    else
    {
        bool bfindfirst = false;
        for(int i=0;i<10 && (bfindfirst==false);i++)
        {
            for(int ch=0; ch<MAX_INTERNAL_CAMERA && (bfindfirst==false); ch++)
            {
                if(!dlgruntimesettings.backupinfo.iDestChannels[ch])
                    continue;

                if(runtimesettings.playbackinfo.playbacktableinfo.iHourTable[ch][timeidx-i])
                {
                    tmcurtime.tm_hour = (timeidx-i)/60;
                    tmcurtime.tm_min = (timeidx-i)%60;
                    bfindfirst = true;
                    break;
                }
            }
        }
    }
    tmcurtime.tm_year=backupinfotm.tm_year-1900;
    curtime = mktime(&tmcurtime);
    qdatetime.setTime_t(curtime);
    ui->backupenddateTimeEdit->setDateTime(qdatetime);
    tmcurtime.tm_year=backupinfotm.tm_year;
    memcpy(&dlgruntimesettings.backupinfo.tmToBackupTime,&tmcurtime,sizeof(struct tm));
}
/*!backup type의 변경이 있을 경우 호출 */
void dvr_setup::on_backuptypecomboBox_currentIndexChanged(int index)
{
    if(index == 0)  //Basket
    {
        ui->backuptreewidget->setEnabled(false);
    }
    else            //AVI
    {
        ui->backuptreewidget->setEnabled(true);
    }
    SetTimeTableUpdate();
}
