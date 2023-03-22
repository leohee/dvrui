#include "dvr_formatconfirm.h"
#include "ui_dvr_formatconfirm.h"
#include "dvr_setup.h"
#include "coordinator.h"
#include "strdefine.h"
#include "struct.h"

extern dvr_setup *pSetup;
/*!constructor*/
dvr_formatconfirm::dvr_formatconfirm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dvr_formatconfirm)
{
    ui->setupUi(this);
    iMode = CONFIRMMODE_FORMAT;
}
/*!destructor */
dvr_formatconfirm::~dvr_formatconfirm()
{
    delete ui;
}

void dvr_formatconfirm::changeEvent(QEvent *e)
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
/*!update confirm text*/
void dvr_formatconfirm::SetConfirmText(char* text)
{
    ui->lineEdit->setText(QString::fromUtf8(text));
}

/*!set confirm mode CONFIRMMODE_FORMAT or CONFIRMMODE_INIT*/
void dvr_formatconfirm::SetConfirmMode(int mode)
{
	//CONFIRMMODE_FORMAT - HDD format confirm을 위한 messsage창 표시
	//CONFIRMMODE_INIT - system config init을 위한 messsage창 표시
    setGeometry((APPWINDOW_W-width())/2,(APPWINDOW_H-height())/2,width(),height());
    iMode = mode;
    switch(iMode)
    {
        case CONFIRMMODE_FORMAT:
            ui->label->setText("Do you want to format?");
            ui->label_2->show();
            ui->lineEdit->show();
            break;
        case CONFIRMMODE_INIT:
            ui->label->setText("confirm config init?");
            ui->label_2->hide();
            ui->lineEdit->hide();
            break;
    }
}

/*!excute ok button selected */
void dvr_formatconfirm::on_buttonBox_accepted()
{
	//CONFIRMMODE_FORMAT - HDD format 
	//CONFIRMMODE_INIT - system config init 
    if(iMode == CONFIRMMODE_FORMAT)
    {
        pSetup->StartFormat();
    }
    else if(iMode == CONFIRMMODE_INIT)
    {
        pSetup->SystemConfigInit();
    }
}
/*!excute cancel button selected */
void dvr_formatconfirm::on_buttonBox_rejected()
{
	//CONFIRMMODE_FORMAT - HDD format cancle 
	//CONFIRMMODE_INIT - system config init cancle 
    if(iMode == CONFIRMMODE_FORMAT)
    {
        pSetup->CancelFormat();
    }
    else if(iMode == CONFIRMMODE_INIT)
    {
        pSetup->SystemConfigInitCancel();
    }
}

