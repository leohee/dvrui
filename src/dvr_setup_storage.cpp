/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include "dvr_defines.h"
#include "dvr_setup.h"
#include "ui_dvr_setup.h"
#include "dvr_main.h"
#include "unistd.h"
/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define KB			(1024)
#define MB			(KB*KB)
/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
extern dvrmain *pMainClass;
extern dvr_layout_main *pLayoutClass;
extern dvr_sidebar *pSidebar;
extern dvr_timelinebar_play *pTimebar;
extern dvr_setup *pSetup;

QListWidgetItem disk_list_item[MAX_DEVLIST];
dvr_disk_info_t mp;
int setdiskidx[MAX_DEVLIST];
bool bCheckFormatComplete;
extern int MsgBoxTimerCount;
/*----------------------------------------------------------------------------
 local functions
-----------------------------------------------------------------------------*/
/*!storage tab의 control들을 초기화 */
void dvr_setup::InitSetupStorageControls()
{
    connect(ui->storageformatpushButton, SIGNAL(clicked()), this, SLOT(StorageFormatBtnSlot()));
    FormatMsgtimer=NULL;
    ui->table_disk_info->setColumnWidth(0,184);
    ui->table_disk_info->setColumnWidth(1,90);
    ui->table_disk_info->setColumnWidth(2,140);
    ui->table_disk_info->setColumnWidth(3,140);
    ui->table_disk_info->setColumnWidth(4,140);

    for(int i=0; i<MAX_DEVLIST; i++)
    {
        m_diskinfotableitem[i][0].setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        m_diskinfotableitem[i][0].setFlags(Qt::NoItemFlags);
        m_diskinfotableitem[i][1].setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        m_diskinfotableitem[i][1].setFlags(Qt::NoItemFlags);
        m_diskinfotableitem[i][2].setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        m_diskinfotableitem[i][2].setFlags(Qt::NoItemFlags);
        m_diskinfotableitem[i][3].setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        m_diskinfotableitem[i][3].setFlags(Qt::NoItemFlags);
        m_diskinfotableitem[i][4].setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        m_diskinfotableitem[i][4].setFlags(Qt::NoItemFlags);
    }
}
/*!profile data struct에 따라서 ui에 value setting */
void dvr_setup::SetStoragedata()
{
    ui->storagehddcomboBox->clear();
    for(int i=0;i<mp.cnt;i++)
    {
    	//dev가 CDROM이 아닌경우 hdd combo box에 추가
        if(mp.dev[setdiskidx[i]].state != DISK_CDROM)
            ui->storagehddcomboBox->addItem(mp.dev[setdiskidx[i]].dev);
    }
    //hdd combo box item수가 1보다 작거나 같은경우는 hdd combobox disable 시킴
    if(ui->storagehddcomboBox->count()<=1)
    {
        ui->storagehddcomboBox->setDisabled(true);
        if(ui->storagehddcomboBox->count())
            ui->storagehddcomboBox->setCurrentIndex(0);
    }
    else
    {
    	//2개 이상일경우 hdd combobox를 enable하고 선택 가능하도록 함
        ui->storagehddcomboBox->setEnabled(true);
        ui->storagehddcomboBox->setCurrentIndex(0);
    }
    ui->storagewritemodecomboBox->setCurrentIndex(dlgdata.storage.iHddRecylceModeOn);
}
/*!ui control의 value를 읽고 해당 기능을 적용시킴 */
void dvr_setup::GetStoragedata()
{
    dlgdata.storage.iHddRecylceModeOn=ui->storagewritemodecomboBox->currentIndex();

    if(dlgdata.storage.iHddRecylceModeOn!=profile.storage.iHddRecylceModeOn)
    {
        profile.storage.iHddRecylceModeOn=dlgdata.storage.iHddRecylceModeOn;
        LIB816x_setRecordingType(profile.storage.iHddRecylceModeOn);
    }

}
/*!cancel시 storage관련 변경적용된 내역을 복원시킴 */
void dvr_setup::ResotreStorage()
{
    if(dlgdata.storage.iHddRecylceModeOn!=profile.storage.iHddRecylceModeOn)
    {
        profile.storage.iHddRecylceModeOn=dlgdata.storage.iHddRecylceModeOn;
        LIB816x_setRecordingType(profile.storage.iHddRecylceModeOn);
    }
}

extern int availableHddCount;
/*!현재 연결된 disk info를 read */
int dvr_setup::get_disk_info(void)
{
    char buf[256];
    int i;
    availableHddCount = 0;
#if DVR_API_ENABLE
    LIB816x_disk_info((void *)&mp);
#else
    mp.rfs_type = 0;
    mp.cnt = 4;
    sprintf(mp.dev[0].dev,"/dev/sda1");
    sprintf(mp.dev[0].type,"ext3");
    mp.dev[0].sz_total = 100000000;
    mp.dev[0].sz_used =  40000000;
    mp.dev[0].sz_avail = 60000000;
    mp.dev[0].state = DONE_EXT3;
    mp.rec_used[0] = 1;
    sprintf(mp.dev[1].dev,"/dev/sdb1");
    sprintf(mp.dev[1].type,"ext3");
    mp.dev[1].sz_total = 100000000;
    mp.dev[1].sz_used =  0;
    mp.dev[1].sz_avail = 100000000;
    mp.dev[1].state = DONE_EXT3;
    mp.rec_used[1] = 2;
    sprintf(mp.dev[2].dev,"/dev/sdc1");
    sprintf(mp.dev[2].type,"ext3");
    mp.dev[2].sz_total = 100000000;
    mp.dev[2].sz_used =  0;
    mp.dev[2].sz_avail = 100000000;
    mp.dev[2].state = DONE_EXT3;
    mp.rec_used[2] = 3;
    sprintf(mp.dev[3].dev,"/dev/sdd1");
    sprintf(mp.dev[3].type,"ext3");
    mp.dev[3].sz_total = 100000000;
    mp.dev[3].sz_used =  0;
    mp.dev[3].sz_avail = 100000000;
    mp.dev[3].state = DONE_EXT3;
    mp.rec_used[3] = 4;
#endif

    g_rfs_type = mp.rfs_type;

    for(i=0; i<ui->table_disk_info->rowCount() ; i++)
    {
        ui->table_disk_info->takeItem(i,0);
        ui->table_disk_info->takeItem(i,1);
        ui->table_disk_info->takeItem(i,2);
        ui->table_disk_info->takeItem(i,3);
        ui->table_disk_info->takeItem(i,4);
    }
    int rowcount = mp.cnt>MAX_DEVLIST?MAX_DEVLIST:mp.cnt;
    ui->table_disk_info->setRowCount(rowcount);

    memset(setdiskidx,-1,sizeof(setdiskidx));
    int recusedCount=0;
    int notrecusedCount=0;
    for(int i=0;i<mp.cnt;i++)
    {
        if(mp.rec_used[i])
        {
            setdiskidx[mp.rec_used[i]-1] = i;
            recusedCount++;
            dprintf("i=%d rec_used[i]=%d\n",i,mp.rec_used[i]);
        }
    }
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<(3-i);j++)
        {
            if(setdiskidx[j]==-1)
            {
                int tmp = setdiskidx[j];
                setdiskidx[j]=setdiskidx[j+1];
                setdiskidx[j+1]=tmp;
            }
            else if(setdiskidx[j+1]>0 && mp.rec_used[setdiskidx[j]] > mp.rec_used[setdiskidx[j+1]])
            {
                int tmp = setdiskidx[j];
                setdiskidx[j]=setdiskidx[j+1];
                setdiskidx[j+1]=tmp;
            }
            dprintf("j(%d) setdiskidx[j]:%d setdiskidx[j+1]:%d\n",j,setdiskidx[j],setdiskidx[j+1]);
        }
    }
    for(int i=0;i<mp.cnt;i++)
    {
        if(mp.rec_used[i]==0)
        {
            setdiskidx[recusedCount+notrecusedCount] = i;
            dprintf("recusedCount+notrecusedCount=%d\n",recusedCount+notrecusedCount);
            notrecusedCount++;
        }
        else
        {
            availableHddCount++;
        }
    }

    dprintf("mp.cnt=%d\n",mp.cnt);
    for(i=0;i<MAX_DEVLIST && i<mp.cnt ;i++)
    {
        dprintf("===========dev %d=============\n",i);
        dprintf("dev: %s\n",mp.dev[i].dev);
        dprintf("dir : %s\n",mp.dev[i].dir);
        dprintf("state : %d\n",mp.dev[i].state);
        dprintf("type : %s\n",mp.dev[i].type);
        dprintf("rec used : %d\n",mp.rec_used[i]);
    }
    dprintf("rfs type : %d\n",mp.rfs_type);

    for(i=0; i<mp.cnt && i<MAX_DEVLIST ; i++)
    {
        char str[100];

        sprintf(str,"%s(%d)",mp.dev[setdiskidx[i]].dev,mp.rec_used[setdiskidx[i]]);
        m_diskinfotableitem[i][0].setText(StrToQString(str));
        ui->table_disk_info->setItem(i,0,&m_diskinfotableitem[i][0]);

        m_diskinfotableitem[i][1].setText(StrToQString(mp.dev[setdiskidx[i]].type));
        ui->table_disk_info->setItem(i,1,&m_diskinfotableitem[i][1]);

        sprintf(buf, "%lu", mp.dev[setdiskidx[i]].sz_total);
        m_diskinfotableitem[i][2].setText(StrToQString(buf));
        ui->table_disk_info->setItem(i,2,&m_diskinfotableitem[i][2]);

        sprintf(buf, "%lu", mp.dev[setdiskidx[i]].sz_used);
        m_diskinfotableitem[i][3].setText(StrToQString(buf));
        ui->table_disk_info->setItem(i,3,&m_diskinfotableitem[i][3]);

        sprintf(buf, "%lu", mp.dev[setdiskidx[i]].sz_avail);
        m_diskinfotableitem[i][4].setText(StrToQString(buf));
        ui->table_disk_info->setItem(i,4,&m_diskinfotableitem[i][4]);
    }

    return 0;
}
/*!file system에 따라 format가능 여부 return */
bool dvr_setup::CheckFormattable(int idx)
{
	//현재는 vfat와 linux swap만 false 처리
    if(!strcmp(mp.dev[setdiskidx[idx]].type,"vfat"))
    {
        dprintf("%s : type %s\n",__FUNCTION__,mp.dev[setdiskidx[idx]].type);
        return false;
    }
    else if(!strncmp(mp.dev[setdiskidx[idx]].type,"Linux swap",10))
    {
        dprintf("%s : type %s\n",__FUNCTION__,mp.dev[setdiskidx[idx]].type);
        return false;
    }
    return true;
}
/*!Format start 작업을 실행함 */
void dvr_setup::StartFormat()
{
	int i=0;
	for(i=0; i<MAX_INTERNAL_CAMERA; i++)
		{
			pLayoutClass->QVideoRecIconlayoutChannel[i]->hide();
		}
    pMainClass->BeforeTerminateOperation();
    bCheckFormatComplete=false;
    MsgBox.SetText("Formatting Hard Disk..");
    MsgBox.show();

    pMainClass->bBlockKey=true;
    pMainClass->Dvrmaintimer->stop();

    FormatMsgtimer=new QTimer(this);
    connect(FormatMsgtimer, SIGNAL(timeout()), this, SLOT(updateFormatWarn()));
    pFormatThread=new FormatThread;
    pFormatThread->setDir(mp.dev[setdiskidx[ui->storagehddcomboBox->currentIndex()]].dir, mp.dev[setdiskidx[ui->storagehddcomboBox->currentIndex()]].dev, mp.dev[setdiskidx[ui->storagehddcomboBox->currentIndex()]].state);
    MsgBoxTimerCount=0;
    FormatMsgtimer->start(1000);
    pFormatThread->start();
    char str[100];
    sprintf(str,"HDD Format for %s",mp.dev[setdiskidx[ui->storagehddcomboBox->currentIndex()]].dir);
    pMainClass->EventLogAdd(str,true);
}
/*!format cancle시 호출 */
void dvr_setup::CancelFormat()
{
    setEnabled(true);
}
/*!현재 format중이라는 Message 출력 */
void dvr_setup::updateFormatWarn (  )
{
    MsgBoxTimerCount++;
    MsgBoxTimerCount%=5;
    if(!bCheckFormatComplete)
    {
    	//format 완료가 되지 않은경우 timeout시마다 메세지 출력시킴
        switch(MsgBoxTimerCount)
        {
        case 0:
            MsgBox.SetText("Formatting Hard Disk..");
            break;
        case 1:
            MsgBox.SetText("Formatting Hard Disk....");
            break;
        case 2:
            MsgBox.SetText("Formatting Hard Disk......");
            break;
        case 3:
            MsgBox.SetText("Formatting Hard Disk........");
            break;
        case 4:
            MsgBox.SetText("Formatting Hard Disk..........");
            break;
        }

    }
    else if(bCheckFormatComplete && MsgBoxTimerCount==3)
    {
    	//format 완료시
    	//setup창 enable시키고, recording 상태에 따라서 record start
        MsgBox.hide();
        bCheckFormatComplete=false;
        
        setEnabled(true);
        if(profile.dvrall.iRecordEmergencyOn)
        {
            for(int i=0;i<MAX_INTERNAL_CAMERA;i++)
            {
                if(profile.camera[i].iEnable==1)
                {
                    if(profile.camera[i].camrec.iRecordingOn==1)
                    {
                        int ret;
#if DVR_API_ENABLE
                        ret=LIB816x_setRecChannel(i, 1, LIB_EVENT_REC_EMG, profile.audioin.audioinch[i].iAudioInOn && profile.camera[i].camrec.iAudioOn , profile.camera[i].strcameraname);
#else
                        ret=1;
#endif
                        if(ret)
                        {
                            detectstatus.detectrecording[i].iEmergency = 1;
                            detectstatus.detectrecording[i].iRecordingOn=1;
                        }
                    }
                }
            }
            pLayoutClass->DrawOSDIcon();
        }
        else
        {
            pMainClass->RecScheduleHandler(curtime.currenttime.tm_wday,curtime.currenttime.tm_hour);
        }
        //hdd info refresh
        on_pb_disk_refresh_clicked();
        pMainClass->bBlockKey=false;
        pMainClass->Dvrmaintimer->start(MAINTIMER_STEPVALUE);
        //playback mode시 calendar 및 timebar update
        if(runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
        {
            memset(runtimesettings.playbackinfo.playbacktableinfo.iMonthTable,0,sizeof(runtimesettings.playbackinfo.playbacktableinfo.iMonthTable));
            LIB816x_GetRecDays(curtime.playbacktime,runtimesettings.playbackinfo.playbacktableinfo.iMonthTable);
            pSidebar->setCalendarRecordDate(&curtime.playbacktime,runtimesettings.playbackinfo.playbacktableinfo.iMonthTable);
            pTimebar->SetTimeTableUpdate(true,true);
        }
        //format thread 종료
        delete pFormatThread;
        pFormatThread=NULL;
        FormatMsgtimer->stop();
    }
	if(bCheckFormatComplete)
	{
			pMainClass->ShowNotifyDialog((char*)"Hard Disk Format Complete and System Reboot");
        	pMainClass->EventLogAdd((char*)"HDD Format Complete",true);
			pSetup->SystemRebootBtnSlot();
	}
}
/*!format button click시에 호출 */
void dvr_setup::StorageFormatBtnSlot()
{
    if(ui->storagehddcomboBox->count())
    {
        if(CheckFormattable(ui->storagehddcomboBox->currentIndex()) == false)
        {
            char str[100];
            sprintf(str,"not supported type : %s",mp.dev[setdiskidx[ui->storagehddcomboBox->currentIndex()]].type);
            pMainClass->ShowNotifyDialog(str);
            return;
        }

        if(pFormatThread)
            return;
        if(FormatMsgtimer)
        {
            delete FormatMsgtimer;
            FormatMsgtimer = NULL;
        }

        setEnabled(false);

        //call format confirm dialog
        m_pConfirmBox->SetConfirmMode(CONFIRMMODE_FORMAT);
        m_pConfirmBox->show();
        m_pConfirmBox->setEnabled(true);
        m_pConfirmBox->SetConfirmText(mp.dev[setdiskidx[ui->storagehddcomboBox->currentIndex()]].dev);
    }
}
/*!refresh button click시에 호출 -연결된 disk 정보를 다시 read */
void dvr_setup::on_pb_disk_refresh_clicked()
{
    get_disk_info();

    ui->storagehddcomboBox->clear();
    for(int i=0;i<mp.cnt;i++)
    {
        if(mp.dev[setdiskidx[i]].state != DISK_CDROM)
            ui->storagehddcomboBox->addItem(mp.dev[setdiskidx[i]].dev);
    }
    if(ui->storagehddcomboBox->count()<=1)
    {
        ui->storagehddcomboBox->setDisabled(true);
        if(ui->storagehddcomboBox->count())
            ui->storagehddcomboBox->setCurrentIndex(0);
    }
    else
    {
        ui->storagehddcomboBox->setEnabled(true);
        ui->storagehddcomboBox->setCurrentIndex(0);
    }
    pMainClass->refreshdiskinfo=true;
}
/*!format thread constructor */
FormatThread::FormatThread()
{
    memset(dir,'\0',80);
    memset(dev,'\0',80);
}
/*!format 대상의 device path와 directory path set */
void FormatThread::setDir(char* dirPath,char* devPath,int hddstatus)
{
    sprintf(dir,"%s",dirPath);
    sprintf(dev,"%s",devPath);
    status = hddstatus;
    printf("dir=%s\n dev=%s\n",dir,dev);
}
/*!format 실행 */
void FormatThread::run()
{
    sleep(5);
    LIB816x_HddFormat(dir,dev,status);
    bCheckFormatComplete=true;
    MsgBoxTimerCount=0;
}
