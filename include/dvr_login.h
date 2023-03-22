#ifndef DVRLOGIN_H
#define DVRLOGIN_H

#include <QDialog>
#include <QFrame>
namespace Ui {
    class dvr_login;
}

class dvr_login : public QDialog {
    Q_OBJECT
public:
    dvr_login(QWidget *parent = 0);
    ~dvr_login();

    void DvrLoginShow();
    void DvrLoginHide();

    virtual void keyPressEvent ( QKeyEvent * event );
public slots:
    void LoginBtnSlot();
    void VirtualKeyBtnSlot();


protected:
    void changeEvent(QEvent *e);

private:
    Ui::dvr_login *ui;
};

#endif 
