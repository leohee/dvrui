#ifndef EVENTLOGDIALOG_H
#define EVENTLOGDIALOG_H

#include <QDialog>
#include <time.h>

namespace Ui {
    class EventLogDialog;
}

class EventLogDialog : public QDialog {
    Q_OBJECT
public:
    EventLogDialog(QWidget *parent = 0);
    ~EventLogDialog();
    ///현재 선택된 date 저장
    struct tm selecttm;

    void SetMainWindowSize();
    void ShowLog();
    void setdate(struct tm *ptm);
    void keyPressEvent ( QKeyEvent * event );

protected:
    void changeEvent(QEvent *e);

private:
    Ui::EventLogDialog *ui;

public slots:
    void on_refreshpushButton_clicked();
    void on_clearpushButton_clicked();
    void LogCalendarSelectionChangedSlot();
    void LogCalendarcurrentPageChangedSlot(int year, int month);
    void on_closepushButton_clicked();
};

#endif 
