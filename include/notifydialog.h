#ifndef NOTIFYDIALOG_H
#define NOTIFYDIALOG_H

#include <QDialog>
#include <QMouseEvent>

namespace Ui {
    class NotifyDialog;
}

class NotifyDialog : public QDialog {
    Q_OBJECT
public:
    NotifyDialog(QWidget *parent = 0);
    ~NotifyDialog();
    void SetNotifyText(char* text);
    void SetMainWindowSize();

    virtual void keyPressEvent ( QKeyEvent * event );

protected:
    void changeEvent(QEvent *e);

private:
    Ui::NotifyDialog *ui;

public slots:
    void on_OkButton_clicked();
};

#endif 
