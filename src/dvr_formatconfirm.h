#ifndef DVR_FORMATCONFIRM_H
#define DVR_FORMATCONFIRM_H

#include <QDialog>

#define CONFIRMMODE_FORMAT  0
#define CONFIRMMODE_INIT    1

namespace Ui {
    class dvr_formatconfirm;
}

class dvr_formatconfirm : public QDialog {
    Q_OBJECT
public:
	///Format or Config init
    int iMode;
    dvr_formatconfirm(QWidget *parent = 0);
    ~dvr_formatconfirm();
    ///mode俊 措茄 confirm text
    char confirmtext[100];

    void SetConfirmText(char* text);
    void SetConfirmMode(int mode);
protected:
    void changeEvent(QEvent *e);

private:
    Ui::dvr_formatconfirm *ui;

public slots:
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();
};

#endif 
