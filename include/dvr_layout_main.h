#ifndef DVRMAINLAYOUT_H
#define DVRMAINLAYOUT_H

#include <QDialog>
#include <QFrame>
#include <QLabel>
#include <QMouseEvent>
#include "define.h"
#include "strdefine.h"
#include "struct.h"

namespace Ui {
    class dvr_layout_main;
}

class dvr_layout_main : public QDialog {
    Q_OBJECT
public:
    dvr_layout_main(QWidget *parent = 0);
    ~dvr_layout_main();
	///Layout Frame
    QFrame *QFramelayoutChannel[MAX_LAYOUTCHANNEL];
    ///camera name 钎矫 Frame
    QLabel *QTitlelayoutChannel[MAX_LAYOUTCHANNEL];
    ///Record Icon
    QLabel *QVideoRecIconlayoutChannel[MAX_LAYOUTCHANNEL];
    ///Audio Icon
    QLabel *QAudioRecIconlayoutChannel[MAX_LAYOUTCHANNEL];
    ///Motion Icon
    QLabel *QMotionIconlayoutChannel[MAX_LAYOUTCHANNEL];
    ///Video loss Icon
    QLabel *QVideoLossIconlayoutChannel[MAX_LAYOUTCHANNEL];
    ///playback Icon
    QLabel *QPlaybackIconlayoutChannel[MAX_LAYOUTCHANNEL];
    ///No Video Frame
    QWidget *QNoVideolayoutChannel[MAX_LAYOUTCHANNEL];
    ///TI logo Icon
    QLabel *QNoVideologoChannel[MAX_LAYOUTCHANNEL];

    void SetMainWindowSize();
    void SetLayoutMode(int iMode);
    void SetLayoutMode_1();
    void SetLayoutMode_4();
    void SetLayoutMode_9();
    void SetLayoutMode_16();
    void SetLayoutMode_8();
    void SetLayoutMode_8_1();
    void SetLayoutMode_12();
    void SetLayoutMode_12_1();
    void SetLayoutMode_14();

    void DrawOSDChannel();
    void DrawOSDIcon();

    void DrawOSDChannelLive();
    void DrawOSDIconLive();
    void DrawOSDChannelPlayback();
    void DrawOSDIconPlayback();

    void SetBorderLineShowHide(int iOnOff);

    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void wheelEvent(QWheelEvent *event);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::dvr_layout_main *ui;
};

#endif 
