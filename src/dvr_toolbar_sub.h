#ifndef DVRSECONDTOOLBAR_H
#define DVRSECONDTOOLBAR_H

#include <QDialog>

namespace Ui {
    class dvr_toolbar_sub;
}

class dvr_toolbar_sub : public QDialog {
    Q_OBJECT
public:
	///현재 layout mode에대한 max page count
	int PageCount;
    dvr_toolbar_sub(QWidget *parent = 0);
    ~dvr_toolbar_sub();

    void SetMainWindowSize();
    void MaxPageCalc();

public slots:

    void PrevPageBtnSlot();
    void NextPageBtnSlot();
    void Layout1BtnSlot();
    void Layout4BtnSlot();
    void Layout9BtnSlot();
    void Layout16BtnSlot();
    void Layout8BtnSlot();
    void Layout8_1BtnSlot();
    void Layout12BtnSlot();
    void Layout12_1BtnSlot();
    void Layout14BtnSlot();
    void SequenceModeBtnSlot();
    void SetLayoutModeBtnSelect(int iMode);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::dvr_toolbar_sub *ui;
};

#endif 
