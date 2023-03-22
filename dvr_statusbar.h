#ifndef DVRMAINSTATUSBAR_H
#define DVRMAINSTATUSBAR_H

#include <QDialog>


namespace Ui {
    class dvr_statusbar;
}

class dvr_statusbar : public QDialog {
    Q_OBJECT
public:
    dvr_statusbar(QWidget *parent = 0);
    ~dvr_statusbar();
    void SetMainWindowSize();
    void ChangePlaybackMainWindowSize(bool displaytimebar);
    void SetDateTimeEditSize();
    void UpdateMainDateTime(struct tm *ptm);
    void UpdateHDDInfo(double use,double total);
    void SetDateFormat(int iDateFormat);



protected:
    void changeEvent(QEvent *e);

private:
    Ui::dvr_statusbar *ui;
};

#endif 
