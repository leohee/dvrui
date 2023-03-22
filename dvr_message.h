#ifndef WARNINGMESSAGE_H
#define WARNINGMESSAGE_H

#include <QFrame>

namespace Ui {
    class WarningMessage;
}

class WarningMessage : public QFrame {
    Q_OBJECT
public:
    WarningMessage(QWidget *parent = 0);
    ~WarningMessage();
    void SetText(const char *);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::WarningMessage *ui;
};

#endif 
