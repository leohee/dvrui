#include "eventlogdialog.h"
#include "ui_eventlogdialog.h"
#include <stdio.h>
#include <QDir>
#include "strdefine.h"
#include "struct.h"
#include "dvr_util.h"
#include "coordinator.h"
#include "dvr_main.h"

extern dvrmain *pMainClass;
extern dvr_sidebar *pSidebar;

/*!constructor*/
EventLogDialog::EventLogDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EventLogDialog)
{
    ui->setupUi(this);
    connect(ui->dvreventlogcalendar, SIGNAL(selectionChanged()), this, SLOT(LogCalendarSelectionChangedSlot()));
    connect(ui->dvreventlogcalendar, SIGNAL(currentPageChanged(int,int)), this, SLOT(LogCalendarcurrentPageChangedSlot(int,int)));
}
/*!destructor */
EventLogDialog::~EventLogDialog()
{
    delete ui;
}

void EventLogDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
/*!front key handler */
void EventLogDialog::keyPressEvent ( QKeyEvent * event )
{
    int input=event->key();

    if(input == Qt::Key_Enter)
    {
        on_closepushButton_clicked();
    }
    else
    {
        pMainClass->keyPressEvent ( event );
    }

}
/*!event log dialog 좌표 설정 */
void EventLogDialog::SetMainWindowSize()
{
    int w = width();
    int h = height();
    int x = (coordinator.MainWindow.width()-w)/2;
    int y = (coordinator.MainWindow.height()-h)/2;
    this->setGeometry(x,y,w,h);

}
/*!입력 date로 calendar의 현재 date를 set */
void EventLogDialog::setdate(struct tm *ptm)
{
	//change current date to selected date
    memcpy(&selecttm,ptm,sizeof(struct tm));
    QDate qselectday;
    qselectday.setDate(ptm->tm_year,ptm->tm_mon+1,ptm->tm_mday);
    ui->dvreventlogcalendar->setSelectedDate(qselectday);
}
/*!선택한 date의 log file을 읽어 edit창에 표시 */
void EventLogDialog::ShowLog()
{
	//find log file for selected date and show
    FILE* fp = NULL;
    char filename[100];
    QString info_path = QDir::currentPath();
    sprintf(filename,"%s%d_%02d_%02d.log",EVENTLOGPATH,selecttm.tm_year,selecttm.tm_mon+1,selecttm.tm_mday);
    info_path.append(filename);
    fp = fopen(QStringToStr(info_path),"r");
    if(fp == NULL)
    {
        char str[100];
        sprintf(str,"no log data for %d-%02d-%02d",selecttm.tm_year,selecttm.tm_mon+1,selecttm.tm_mday);
        ui->logviewtextEdit->setText(StrToQString(str));
        return;
    }
    fseek(fp,0,SEEK_END);
    int logsize = ftell(fp);
    rewind(fp);
    char *logstr = new char[logsize+1];
    memset(logstr,'\0',logsize+1);
    fread(logstr,sizeof(char),logsize,fp);
    ui->logviewtextEdit->setText(StrToQString(logstr));
    fclose(fp);
    fp = NULL;
    delete logstr;

}
/*!calendar의 선택 date가 변경될 때 호출 */
void EventLogDialog::LogCalendarSelectionChangedSlot()
{
	//if change selected date, find log file for selected date and show
    QDate qselectday=ui->dvreventlogcalendar->selectedDate();
    qselectday.getDate(&selecttm.tm_year,&selecttm.tm_mon,&selecttm.tm_mday);
    selecttm.tm_mon=selecttm.tm_mon-1;
    ShowLog();
}
/*!calendar page가 변경시 호출(년도,달 변경시) */
void EventLogDialog::LogCalendarcurrentPageChangedSlot(int year, int month)
{
	//if change selected date, find log file for selected date and show
    QDate qselectday=ui->dvreventlogcalendar->selectedDate();
    qselectday.getDate(&selecttm.tm_year,&selecttm.tm_mon,&selecttm.tm_mday);
    selecttm.tm_year=year;
    selecttm.tm_mon=month-1;
    qselectday.setDate(selecttm.tm_year,selecttm.tm_mon+1,selecttm.tm_mday);
    ui->dvreventlogcalendar->setSelectedDate(qselectday);
}
/*!close button click시 호출 */
void EventLogDialog::on_closepushButton_clicked()
{
	//hide event log dialog
    hide();
}
/*!erase log button click시 호출 */
void EventLogDialog::on_clearpushButton_clicked()
{
	//erase all log file
    char buf[100];
    QString info_path = QDir::currentPath();
    pSidebar->clearEventlog();
    info_path.append(EVENTLOGPATH);
    sprintf(buf,"rm -f %s*.log",QStringToStr(info_path));
    system(buf);
    ShowLog();
}
/*!refresh button click시 호출 */
void EventLogDialog::on_refreshpushButton_clicked()
{
	//update current log
    ShowLog();
}
