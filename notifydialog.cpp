#include "notifydialog.h"
#include "ui_notifydialog.h"
#include "dvr_defines.h"
#include "dvr_main.h"

extern dvrmain *pMainClass;

/*!constructor*/
NotifyDialog::NotifyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NotifyDialog)
{
    ui->setupUi(this);
}
/*!destructor */
NotifyDialog::~NotifyDialog()
{
    delete ui;
}

void NotifyDialog::changeEvent(QEvent *e)
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
/*!error상황등 발생시 edit 창에 notify text set */
void NotifyDialog::SetNotifyText(char* text)
{
    ui->Notifylabel->setText(StrToQString(text));
}
/*!notify dialog 좌표 설정 */
void NotifyDialog::SetMainWindowSize()
{
    int w = width();
    int h = height();
    int x = (coordinator.MainWindow.width()-w)/2;
    int y = (coordinator.MainWindow.height()-h)/2;
    this->setGeometry(x,y,w,h);
}
/*!front key handler */
void NotifyDialog::keyPressEvent ( QKeyEvent * event )
{
    switch(event->key())
    {
        case Qt::Key_Return:
            on_OkButton_clicked();
        break;
        case Qt::Key_Escape:
            pMainClass->keyPressEvent ( event );
        break;
    }
}
/*!ok button click시 호출 */
void NotifyDialog::on_OkButton_clicked()
{
    hide();
}
