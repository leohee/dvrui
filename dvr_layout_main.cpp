/******************************************************************************
* DM8168 Hybrid DVR
* Copyright by UDWorks, Incoporated. All Rights Reserved.
*-----------------------------------------------------------------------------
* @file	dvr_layout_main.cpp
* @brief
* @author
* @section	MODIFY history
*     - 2011.01.01 : First Created
******************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include "dvr_defines.h"
#include "dvr_layout_main.h"
#include "ui_dvr_layout_main.h"
#include "dvr_main.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
extern dvrmain *pMainClass;
/*----------------------------------------------------------------------------
 local functions
-----------------------------------------------------------------------------*/
/*!constructor*/
dvr_layout_main::dvr_layout_main(QWidget *parent) :
    QDialog(parent,Qt::CustomizeWindowHint |Qt::FramelessWindowHint),
    ui(new Ui::dvr_layout_main)
{
    ui->setupUi(this);
    QFramelayoutChannel[0]=ui->layoutchannelframe1;
    QFramelayoutChannel[1]=ui->layoutchannelframe2;
    QFramelayoutChannel[2]=ui->layoutchannelframe3;
    QFramelayoutChannel[3]=ui->layoutchannelframe4;
    QFramelayoutChannel[4]=ui->layoutchannelframe5;
    QFramelayoutChannel[5]=ui->layoutchannelframe6;
    QFramelayoutChannel[6]=ui->layoutchannelframe7;
    QFramelayoutChannel[7]=ui->layoutchannelframe8;
    QFramelayoutChannel[8]=ui->layoutchannelframe9;
    QFramelayoutChannel[9]=ui->layoutchannelframe10;
    QFramelayoutChannel[10]=ui->layoutchannelframe11;
    QFramelayoutChannel[11]=ui->layoutchannelframe12;
    QFramelayoutChannel[12]=ui->layoutchannelframe13;
    QFramelayoutChannel[13]=ui->layoutchannelframe14;
    QFramelayoutChannel[14]=ui->layoutchannelframe15;
    QFramelayoutChannel[15]=ui->layoutchannelframe16;

    QTitlelayoutChannel[0]=ui->titlelabel1;
    QTitlelayoutChannel[1]=ui->titlelabel2;
    QTitlelayoutChannel[2]=ui->titlelabel3;
    QTitlelayoutChannel[3]=ui->titlelabel4;
    QTitlelayoutChannel[4]=ui->titlelabel5;
    QTitlelayoutChannel[5]=ui->titlelabel6;
    QTitlelayoutChannel[6]=ui->titlelabel7;
    QTitlelayoutChannel[7]=ui->titlelabel8;
    QTitlelayoutChannel[8]=ui->titlelabel9;
    QTitlelayoutChannel[9]=ui->titlelabel10;
    QTitlelayoutChannel[10]=ui->titlelabel11;
    QTitlelayoutChannel[11]=ui->titlelabel12;
    QTitlelayoutChannel[12]=ui->titlelabel13;
    QTitlelayoutChannel[13]=ui->titlelabel14;
    QTitlelayoutChannel[14]=ui->titlelabel15;
    QTitlelayoutChannel[15]=ui->titlelabel16;

    for(int i=0;i<MAX_LAYOUTCHANNEL;i++)
    {
         QFont font=QTitlelayoutChannel[i]->font();
         QTitlelayoutChannel[i]->setFont(font);
    }

    QVideoRecIconlayoutChannel[0]=ui->videoreciconlabel1;
    QVideoRecIconlayoutChannel[1]=ui->videoreciconlabel2;
    QVideoRecIconlayoutChannel[2]=ui->videoreciconlabel3;
    QVideoRecIconlayoutChannel[3]=ui->videoreciconlabel4;
    QVideoRecIconlayoutChannel[4]=ui->videoreciconlabel5;
    QVideoRecIconlayoutChannel[5]=ui->videoreciconlabel6;
    QVideoRecIconlayoutChannel[6]=ui->videoreciconlabel7;
    QVideoRecIconlayoutChannel[7]=ui->videoreciconlabel8;
    QVideoRecIconlayoutChannel[8]=ui->videoreciconlabel9;
    QVideoRecIconlayoutChannel[9]=ui->videoreciconlabel10;
    QVideoRecIconlayoutChannel[10]=ui->videoreciconlabel11;
    QVideoRecIconlayoutChannel[11]=ui->videoreciconlabel12;
    QVideoRecIconlayoutChannel[12]=ui->videoreciconlabel13;
    QVideoRecIconlayoutChannel[13]=ui->videoreciconlabel14;
    QVideoRecIconlayoutChannel[14]=ui->videoreciconlabel15;
    QVideoRecIconlayoutChannel[15]=ui->videoreciconlabel16;

    QAudioRecIconlayoutChannel[0]=ui->audioreciconlabel1;
    QAudioRecIconlayoutChannel[1]=ui->audioreciconlabel2;
    QAudioRecIconlayoutChannel[2]=ui->audioreciconlabel3;
    QAudioRecIconlayoutChannel[3]=ui->audioreciconlabel4;
    QAudioRecIconlayoutChannel[4]=ui->audioreciconlabel5;
    QAudioRecIconlayoutChannel[5]=ui->audioreciconlabel6;
    QAudioRecIconlayoutChannel[6]=ui->audioreciconlabel7;
    QAudioRecIconlayoutChannel[7]=ui->audioreciconlabel8;
    QAudioRecIconlayoutChannel[8]=ui->audioreciconlabel9;
    QAudioRecIconlayoutChannel[9]=ui->audioreciconlabel10;
    QAudioRecIconlayoutChannel[10]=ui->audioreciconlabel11;
    QAudioRecIconlayoutChannel[11]=ui->audioreciconlabel12;
    QAudioRecIconlayoutChannel[12]=ui->audioreciconlabel13;
    QAudioRecIconlayoutChannel[13]=ui->audioreciconlabel14;
    QAudioRecIconlayoutChannel[14]=ui->audioreciconlabel15;
    QAudioRecIconlayoutChannel[15]=ui->audioreciconlabel16;

    QMotionIconlayoutChannel[0]=ui->motioniconlabel1;
    QMotionIconlayoutChannel[1]=ui->motioniconlabel2;
    QMotionIconlayoutChannel[2]=ui->motioniconlabel3;
    QMotionIconlayoutChannel[3]=ui->motioniconlabel4;
    QMotionIconlayoutChannel[4]=ui->motioniconlabel5;
    QMotionIconlayoutChannel[5]=ui->motioniconlabel6;
    QMotionIconlayoutChannel[6]=ui->motioniconlabel7;
    QMotionIconlayoutChannel[7]=ui->motioniconlabel8;
    QMotionIconlayoutChannel[8]=ui->motioniconlabel9;
    QMotionIconlayoutChannel[9]=ui->motioniconlabel10;
    QMotionIconlayoutChannel[10]=ui->motioniconlabel11;
    QMotionIconlayoutChannel[11]=ui->motioniconlabel12;
    QMotionIconlayoutChannel[12]=ui->motioniconlabel13;
    QMotionIconlayoutChannel[13]=ui->motioniconlabel14;
    QMotionIconlayoutChannel[14]=ui->motioniconlabel15;
    QMotionIconlayoutChannel[15]=ui->motioniconlabel16;

    QVideoLossIconlayoutChannel[0]=ui->videolossiconlabel1;
    QVideoLossIconlayoutChannel[1]=ui->videolossiconlabel2;
    QVideoLossIconlayoutChannel[2]=ui->videolossiconlabel3;
    QVideoLossIconlayoutChannel[3]=ui->videolossiconlabel4;
    QVideoLossIconlayoutChannel[4]=ui->videolossiconlabel5;
    QVideoLossIconlayoutChannel[5]=ui->videolossiconlabel6;
    QVideoLossIconlayoutChannel[6]=ui->videolossiconlabel7;
    QVideoLossIconlayoutChannel[7]=ui->videolossiconlabel8;
    QVideoLossIconlayoutChannel[8]=ui->videolossiconlabel9;
    QVideoLossIconlayoutChannel[9]=ui->videolossiconlabel10;
    QVideoLossIconlayoutChannel[10]=ui->videolossiconlabel11;
    QVideoLossIconlayoutChannel[11]=ui->videolossiconlabel12;
    QVideoLossIconlayoutChannel[12]=ui->videolossiconlabel13;
    QVideoLossIconlayoutChannel[13]=ui->videolossiconlabel14;
    QVideoLossIconlayoutChannel[14]=ui->videolossiconlabel15;
    QVideoLossIconlayoutChannel[15]=ui->videolossiconlabel16;

    QPlaybackIconlayoutChannel[0]=ui->playbackiconlabel1;
    QPlaybackIconlayoutChannel[1]=ui->playbackiconlabel2;
    QPlaybackIconlayoutChannel[2]=ui->playbackiconlabel3;
    QPlaybackIconlayoutChannel[3]=ui->playbackiconlabel4;
    QPlaybackIconlayoutChannel[4]=ui->playbackiconlabel5;
    QPlaybackIconlayoutChannel[5]=ui->playbackiconlabel6;
    QPlaybackIconlayoutChannel[6]=ui->playbackiconlabel7;
    QPlaybackIconlayoutChannel[7]=ui->playbackiconlabel8;
    QPlaybackIconlayoutChannel[8]=ui->playbackiconlabel9;
    QPlaybackIconlayoutChannel[9]=ui->playbackiconlabel10;
    QPlaybackIconlayoutChannel[10]=ui->playbackiconlabel11;
    QPlaybackIconlayoutChannel[11]=ui->playbackiconlabel12;
    QPlaybackIconlayoutChannel[12]=ui->playbackiconlabel13;
    QPlaybackIconlayoutChannel[13]=ui->playbackiconlabel14;
    QPlaybackIconlayoutChannel[14]=ui->playbackiconlabel15;
    QPlaybackIconlayoutChannel[15]=ui->playbackiconlabel16;

    QNoVideologoChannel[0]=ui->dvrmainlogolabel_1;
    QNoVideologoChannel[1]=ui->dvrmainlogolabel_2;
    QNoVideologoChannel[2]=ui->dvrmainlogolabel_3;
    QNoVideologoChannel[3]=ui->dvrmainlogolabel_4;
    QNoVideologoChannel[4]=ui->dvrmainlogolabel_5;
    QNoVideologoChannel[5]=ui->dvrmainlogolabel_6;
    QNoVideologoChannel[6]=ui->dvrmainlogolabel_7;
    QNoVideologoChannel[7]=ui->dvrmainlogolabel_8;
    QNoVideologoChannel[8]=ui->dvrmainlogolabel_9;
    QNoVideologoChannel[9]=ui->dvrmainlogolabel_10;
    QNoVideologoChannel[10]=ui->dvrmainlogolabel_11;
    QNoVideologoChannel[11]=ui->dvrmainlogolabel_12;
    QNoVideologoChannel[12]=ui->dvrmainlogolabel_13;
    QNoVideologoChannel[13]=ui->dvrmainlogolabel_14;
    QNoVideologoChannel[14]=ui->dvrmainlogolabel_15;
    QNoVideologoChannel[15]=ui->dvrmainlogolabel_16;

    QNoVideolayoutChannel[0] = ui->widget_1;
    QNoVideolayoutChannel[1] = ui->widget_2;
    QNoVideolayoutChannel[2] = ui->widget_3;
    QNoVideolayoutChannel[3] = ui->widget_4;
    QNoVideolayoutChannel[4] = ui->widget_5;
    QNoVideolayoutChannel[5] = ui->widget_6;
    QNoVideolayoutChannel[6] = ui->widget_7;
    QNoVideolayoutChannel[7] = ui->widget_8;
    QNoVideolayoutChannel[8] = ui->widget_9;
    QNoVideolayoutChannel[9] = ui->widget_10;
    QNoVideolayoutChannel[10] = ui->widget_11;
    QNoVideolayoutChannel[11] = ui->widget_12;
    QNoVideolayoutChannel[12] = ui->widget_13;
    QNoVideolayoutChannel[13] = ui->widget_14;
    QNoVideolayoutChannel[14] = ui->widget_15;
    QNoVideolayoutChannel[15] = ui->widget_16;

    for(int i=0;i<MAX_LAYOUTCHANNEL;i++)
    {
        QNoVideolayoutChannel[i]->hide();
    }
}

/*!destructor */
dvr_layout_main::~dvr_layout_main()
{
    delete ui;
}

void dvr_layout_main::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void dvr_layout_main::mousePressEvent ( QMouseEvent * event )
{
    if(pMainClass->m_bLogin==false)
        return;
    dprintf("mousePressEvent\n");

    if(event->buttons()==Qt::RightButton)
    {
        dprintf("RightMouseButton clicked\n");
    }
    else if(event->buttons()==Qt::LeftButton)
    {
    }
}
#if 1
void dvr_layout_main::wheelEvent(QWheelEvent *event)
{
    event->ignore();
    if(runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
    {
        pMainClass->wheelEvent(event);
    }

}
#endif
/*! set size */
void dvr_layout_main::SetMainWindowSize()
{
    this->setGeometry (runtimesettings.iLayOutWindowX,runtimesettings.iLayOutWindowY,runtimesettings.iLayOutWindowWidth,runtimesettings.iLayOutWindowHeight);
    SetLayoutMode(profile.layout.iLayoutMode);
}
/*! set layout mode */
void dvr_layout_main::SetLayoutMode(int iMode)
{
	//Layout mode에 따라서 layout 좌표 설정
    switch(iMode)
    {
    case LAYOUTMODE_1:
        SetLayoutMode_1();
    break;
    case LAYOUTMODE_4:
        SetLayoutMode_4();
    break;
    case LAYOUTMODE_9:
        SetLayoutMode_9();
    break;
    case LAYOUTMODE_16:
        SetLayoutMode_16();
    break;
    case LAYOUTMODE_8:
        SetLayoutMode_8();
    break;
    case LAYOUTMODE_8_1:
        SetLayoutMode_8_1();
    break;
    case LAYOUTMODE_12:
        SetLayoutMode_12();
    break;
    case LAYOUTMODE_12_1:
        SetLayoutMode_12_1();
    break;
    case LAYOUTMODE_14:
        SetLayoutMode_14();
    break;
    }

	//icon 좌표 설정
    for(int i=0;i<runtimesettings.iLayout;i++)
    {

        int x=QFramelayoutChannel[i]->width()-MARGIN_10-OSD_ICON_W;
        int y=MARGIN_10;

        QPlaybackIconlayoutChannel[i]->setGeometry(x,y,OSD_ICON_W,OSD_ICON_H);
        x=x-OSD_ICON_W;
        QVideoRecIconlayoutChannel[i]->setGeometry(x,y,OSD_ICON_W,OSD_ICON_H);
        x=x-OSD_ICON_W;
        QAudioRecIconlayoutChannel[i]->setGeometry(x,y,OSD_ICON_W,OSD_ICON_H);
        x=x-OSD_ICON_W;
        QMotionIconlayoutChannel[i]->setGeometry(x,y,OSD_ICON_W,OSD_ICON_H);
        x=x-OSD_ICON_W;
        QVideoLossIconlayoutChannel[i]->setGeometry(x,y,OSD_ICON_W,OSD_ICON_H);

        x=(QFramelayoutChannel[i]->width()-QNoVideologoChannel[i]->width())/2;
        y=(QFramelayoutChannel[i]->height()-QNoVideologoChannel[i]->height())/2;
        if(profile.mainosd.iBorderLineOn)
            QNoVideolayoutChannel[i]->setGeometry(2,2,QFramelayoutChannel[i]->width()-4,QFramelayoutChannel[i]->height()-4);
        else
            QNoVideolayoutChannel[i]->setGeometry(0,0,QFramelayoutChannel[i]->width(),QFramelayoutChannel[i]->height());
        QNoVideologoChannel[i]->setGeometry(x,y,QNoVideologoChannel[i]->width(),QNoVideologoChannel[i]->height());
    }
    
    //보이지 않는 layout을 hide 처리
    for(int i=runtimesettings.iLayout;i<MAX_LAYOUTCHANNEL;i++)
    {
        QFramelayoutChannel[i]->hide();
    }
}

/*! set layout mode to LAYOUTMODE_1 */
void dvr_layout_main::SetLayoutMode_1()
{
    QFramelayoutChannel[0]->setGeometry(0,0,runtimesettings.iLayOutWindowWidth,runtimesettings.iLayOutWindowHeight);
    QFramelayoutChannel[0]->show();

}

/*! set layout mode to LAYOUTMODE_4 */
void dvr_layout_main::SetLayoutMode_4()
{
    for(int i=0;i<LAYOUT_4;i++)
    {
                int w;
                int x;
		if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_1080P)
        {
                        w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/2,LayoutWidthAlign);
                        x=(i%2)*w;
		}
		else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_720P)
		{
			int LayoutWidthAlign1 = 12;
			w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/2 -6,LayoutWidthAlign1);
                        x=(i%2)*w;
		}
                else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_SXGA)
                {
                    int LayoutWidthAlign1 = 12;
                    w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/2 -6,LayoutWidthAlign1);
                    x=(i%2)*w;
                }
                else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_XGA)
                {
                    int LayoutWidthAlign1 = 12;
                    w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/2 -6,LayoutWidthAlign1);
                    w=w -1;
                    x=(i%2)*w;
                }
                else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_480P)
                {
                    int LayoutWidthAlign1 = 12;
                    w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/2 -6,LayoutWidthAlign1);
                    w=w -2;
                    x=(i%2)*w -1;
                }
                else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_576P)
                {
                    int LayoutWidthAlign1 = 12;
                    w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/2 -6,LayoutWidthAlign1);
                    w=w -2;
                    x=(i%2)*w -1;
                }
		else
		{
			w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/2,LayoutWidthAlign);
                        x=(i%2)*w;
		}
        //int w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/2,LayoutWidthAlign);
        int h=runtimesettings.iLayOutWindowHeight/2;

        int y=(i/2)*h;
		
        QFramelayoutChannel[i]->setGeometry(x,y,w,h);
        QFramelayoutChannel[i]->show();
    }


}

/*! set layout mode to LAYOUTMODE_9 */
void dvr_layout_main::SetLayoutMode_9()
{
    for(int i=0;i<LAYOUT_9;i++)
    {
    	int w;
        int x;
		if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_1080P)
                {
        	w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/3,LayoutWidthAlign);
                x=(i%3)*w;
		}
		else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_720P)
		{
			int LayoutWidthAlign1 = 12;
			w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/3 - 6,LayoutWidthAlign1);
                        x=(i%3)*w;
		}
                else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_SXGA)
                {
                    int LayoutWidthAlign1 = 12;
                    w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/3 -6,LayoutWidthAlign1);
                    x=(i%3)*w;
                }
                else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_XGA)
                {
                    int LayoutWidthAlign1 =8 ;
                    w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/3 -2,LayoutWidthAlign1);
                    x=(i%3)*w -2;
                }
                else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_480P)
                {
                    int LayoutWidthAlign1 =6 ;
                    w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/3 -3,LayoutWidthAlign1);
                    w=w+2;
                    x=(i%3)*w;
                }
                else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_576P)
                {
                    int LayoutWidthAlign1 =6 ;
                    w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/3 -3,LayoutWidthAlign1);
                    w=w+2;
                    x=(i%3)*w;
                }
		else
		{
			w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/3,LayoutWidthAlign);
                        x=(i%3)*w;
		}	
        //int w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/3,LayoutWidthAlign);
        int h=runtimesettings.iLayOutWindowHeight/3;

        int y=(i/3)*h;
        if(i%3==2)
    	{
//            w = this->width() - x;
    	}
        QFramelayoutChannel[i]->setGeometry(x,y,w,h);
        QFramelayoutChannel[i]->show();
    }

}

/*! set layout mode to LAYOUTMODE_16 */
void dvr_layout_main::SetLayoutMode_16()
{
    for(int i=0;i<LAYOUT_16;i++)
    {
        int w;
        int x;
        if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_1080P)
        {
            w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/4,LayoutWidthAlign);
            x=(i%4)*w;
        }
        else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_720P)
        {
            int LayoutWidthAlign1 = 10;
            w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/4 - 10 ,LayoutWidthAlign1);
            x=(i%4)*w;
        }
        else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_SXGA)
        {
            int LayoutWidthAlign1 = 10;
            w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/4 -10,LayoutWidthAlign1);
            x=(i%4)*w;
        }
        else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_XGA)
        {
            int LayoutWidthAlign1 = 8;
            w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/4 -8,LayoutWidthAlign1);
            w=w+3;
            x=(i%4)*w -1;
        }
        else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_480P)
        {
            int LayoutWidthAlign1 = 5;
            w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/4 -5,LayoutWidthAlign1);
            w=w;
            x=(i%4)*w -2;
        }
        else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_576P)
        {
            int LayoutWidthAlign1 = 5;
            w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/4 -5,LayoutWidthAlign1);
            w=w;
            x=(i%4)*w -2;
        }
        else
        {
            w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/4,LayoutWidthAlign);
            x=(i%4)*w;
        }
        int h=runtimesettings.iLayOutWindowHeight/4;

        int y=(i/4)*h;
        if(i%4==3)
    	{
//            w = this->width() - x;
    	}
		
        QFramelayoutChannel[i]->setGeometry(x,y,w,h);
        QFramelayoutChannel[i]->show();
    }
}

/*! set layout mode to LAYOUTMODE_8 */
void dvr_layout_main::SetLayoutMode_8()
{
    int w;
    if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_1080P)
    {
        w=VsysUtils_floor((runtimesettings.iLayOutWindowWidth/4),LayoutWidthAlign)*3;
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_720P)
    {
        int LayoutWidthAlign1 = 10;
        w=VsysUtils_floor((runtimesettings.iLayOutWindowWidth/4-10),LayoutWidthAlign1)*3;
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_SXGA)
    {
        int LayoutWidthAlign1 = 10;
        w=VsysUtils_floor((runtimesettings.iLayOutWindowWidth/4-10),LayoutWidthAlign1)*3;
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_XGA)
    {
        int LayoutWidthAlign1 = 10;
        w=VsysUtils_floor((runtimesettings.iLayOutWindowWidth/4-10),LayoutWidthAlign1)*3;
        w=w-4;
    }
    else
    {
        w=VsysUtils_floor((runtimesettings.iLayOutWindowWidth/4),LayoutWidthAlign)*3;
    }

        //int w=VsysUtils_floor((runtimesettings.iLayOutWindowWidth/4),LayoutWidthAlign)*3;
    int h=(runtimesettings.iLayOutWindowHeight/4)*3;
    int x=0;
    int y=0;
    QFramelayoutChannel[0]->setGeometry(x,y,w,h);
    QFramelayoutChannel[0]->show();

	x = w ;
        if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_1080P)
        {
            w = VsysUtils_floor((runtimesettings.iLayOutWindowWidth/4),LayoutWidthAlign) ;
        }
        else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_720P)
        {
            w = VsysUtils_floor((runtimesettings.iLayOutWindowWidth/4),LayoutWidthAlign) ;
        }
        else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_SXGA)
        {
            w = VsysUtils_floor((runtimesettings.iLayOutWindowWidth/4),LayoutWidthAlign) ;
        }
        else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_XGA)
        {
            int LayoutWidthAlign1 = 10;
            w = VsysUtils_floor((runtimesettings.iLayOutWindowWidth/4 -10),LayoutWidthAlign1) ;
            w=w-4;
        }
        else
        {
            w = VsysUtils_floor((runtimesettings.iLayOutWindowWidth/4),LayoutWidthAlign) ;
        }
   // w = VsysUtils_floor((runtimesettings.iLayOutWindowWidth/4),LayoutWidthAlign) ;
    h=runtimesettings.iLayOutWindowHeight/4;

    y=0;
    QFramelayoutChannel[1]->setGeometry(x,y,w,h);
    QFramelayoutChannel[1]->show();

    y=runtimesettings.iLayOutWindowHeight/4;
    QFramelayoutChannel[2]->setGeometry(x,y,w,h);
    QFramelayoutChannel[2]->show();

    y=(runtimesettings.iLayOutWindowHeight/4)*2;
    QFramelayoutChannel[3]->setGeometry(x,y,w,h);
    QFramelayoutChannel[3]->show();

    if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_1080P)
    {
        w=VsysUtils_floor((runtimesettings.iLayOutWindowWidth/4),LayoutWidthAlign);
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_720P)
    {
        int LayoutWidthAlign1 = 10;
        w=VsysUtils_floor((runtimesettings.iLayOutWindowWidth/4-10),LayoutWidthAlign1);
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_SXGA)
    {
        int LayoutWidthAlign1 = 10;
        w=VsysUtils_floor((runtimesettings.iLayOutWindowWidth/4-10),LayoutWidthAlign1);
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_XGA)
    {
        int LayoutWidthAlign1 = 10;
        w=VsysUtils_floor((runtimesettings.iLayOutWindowWidth/4-10),LayoutWidthAlign1);
        w=w-2;
    }
    else
    {
        w=VsysUtils_floor((runtimesettings.iLayOutWindowWidth/4),LayoutWidthAlign);
    }
    //w = VsysUtils_floor((runtimesettings.iLayOutWindowWidth/4),LayoutWidthAlign);
    x=0;
    y=(runtimesettings.iLayOutWindowHeight/4)*3;
    QFramelayoutChannel[4]->setGeometry(x,y,w,h);
    QFramelayoutChannel[4]->show();

    x=w;
    y=(runtimesettings.iLayOutWindowHeight/4)*3;
    QFramelayoutChannel[5]->setGeometry(x,y,w,h);
    QFramelayoutChannel[5]->show();

    x=w*2;
    y=(runtimesettings.iLayOutWindowHeight/4)*3;
    QFramelayoutChannel[6]->setGeometry(x,y,w,h);
    QFramelayoutChannel[6]->show();

    x=w*3;
    y=(runtimesettings.iLayOutWindowHeight/4)*3;
    QFramelayoutChannel[7]->setGeometry(x,y,w,h);
    QFramelayoutChannel[7]->show();

}

/*! set layout mode to LAYOUTMODE_8_1 */
void dvr_layout_main::SetLayoutMode_8_1()
{
    int w;
    if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_1080P)
    {
      w=(runtimesettings.iLayOutWindowWidth/5)*2;
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_720P)
    {
        w=(runtimesettings.iLayOutWindowWidth/5-2)*2;
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_SXGA)
    {
        w=(runtimesettings.iLayOutWindowWidth/5-2)*2;
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_XGA)
    {
        w=(runtimesettings.iLayOutWindowWidth/5)*2;
        w=w-2;
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_480P)
    {
        w=(runtimesettings.iLayOutWindowWidth/5-6)*2;
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_576P)
    {
        w=(runtimesettings.iLayOutWindowWidth/5-6)*2;
    }
    else
    {
        w=(runtimesettings.iLayOutWindowWidth/5)*2;
    }
    //int w=(runtimesettings.iLayOutWindowWidth/5)*2;
    int h=(runtimesettings.iLayOutWindowHeight/4)*2;
    int x=0;
    int y=0;
    QFramelayoutChannel[0]->setGeometry(x,y,w,h);
    QFramelayoutChannel[0]->show();

    x=w;
    y=0;
    QFramelayoutChannel[1]->setGeometry(x,y,w,h);
    QFramelayoutChannel[1]->show();

    x=0;
    y=h;
    QFramelayoutChannel[2]->setGeometry(x,y,w,h);
    QFramelayoutChannel[2]->show();

    x=w;
    y=h;
    QFramelayoutChannel[3]->setGeometry(x,y,w,h);
    QFramelayoutChannel[3]->show();

    if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_1080P)
    {
      w=(runtimesettings.iLayOutWindowWidth/5);
       x=(runtimesettings.iLayOutWindowWidth/5)*4;
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_720P)
    {
        w=(runtimesettings.iLayOutWindowWidth/5-5);
         x=(runtimesettings.iLayOutWindowWidth/5-2)*4;
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_SXGA)
    {
        w=(runtimesettings.iLayOutWindowWidth/5-5);
         x=(runtimesettings.iLayOutWindowWidth/5-2)*4;
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_XGA)
    {
        w=(runtimesettings.iLayOutWindowWidth/5+4);
         x=(runtimesettings.iLayOutWindowWidth/5-1)*4;
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_480P)
    {
        w=(runtimesettings.iLayOutWindowWidth/5+4);
         x=(runtimesettings.iLayOutWindowWidth/5 -6)*4;
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_576P)
    {
        w=(runtimesettings.iLayOutWindowWidth/5+4);
         x=(runtimesettings.iLayOutWindowWidth/5 -6)*4;
    }
    else
    {
        w=(runtimesettings.iLayOutWindowWidth/5);
         x=(runtimesettings.iLayOutWindowWidth/5)*4;
    }
    //w=(runtimesettings.iLayOutWindowWidth/5);
    h=(runtimesettings.iLayOutWindowHeight/4);
   // x=(runtimesettings.iLayOutWindowWidth/5-2)*4;
    y=0;
    QFramelayoutChannel[4]->setGeometry(x,y,w,h);
    QFramelayoutChannel[4]->show();

    y=h;
    QFramelayoutChannel[5]->setGeometry(x,y,w,h);
    QFramelayoutChannel[5]->show();

    y=h*2;
    QFramelayoutChannel[6]->setGeometry(x,y,w,h);
    QFramelayoutChannel[6]->show();

    y=h*3;
    QFramelayoutChannel[7]->setGeometry(x,y,w,h);
    QFramelayoutChannel[7]->show();

}

/*! set layout mode to LAYOUTMODE_12 */
void dvr_layout_main::SetLayoutMode_12()
{
    for(int i=0;i<LAYOUT_12;i++)
    {
        int w;
        if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_1080P)
        {
            w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/4,LayoutWidthAlign);
        }
        else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_720P)
        {
            int LayoutWidthAlign1 = 10;
            w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/4-10,LayoutWidthAlign1);
        }
        else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_SXGA)
        {
            int LayoutWidthAlign1 = 10;
            w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/4-10,LayoutWidthAlign1);
        }
        else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_XGA)
        {
            int LayoutWidthAlign1 = 10;
            w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/4-10,LayoutWidthAlign1);
            w=w-2;
        }
        else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_480P)
        {
            int LayoutWidthAlign1 = 5;
            w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/4 -5,LayoutWidthAlign1);
            w=w;
        }
        else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_576P)
        {
            int LayoutWidthAlign1 = 5;
            w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/4 -5,LayoutWidthAlign1);
            w=w;
        }
        else
        {
           w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/4,LayoutWidthAlign);
        }
        //int w=VsysUtils_floor(runtimesettings.iLayOutWindowWidth/4,LayoutWidthAlign);
        int h=runtimesettings.iLayOutWindowHeight/3;
        int x=(i%4)*w;
        int y=(i/4)*h;
        if(i%4==3)
    	{
//            w = this->width() - x;
    	}
        QFramelayoutChannel[i]->setGeometry(x,y,w,h);
        QFramelayoutChannel[i]->show();
    }

}

/*! set layout mode to LAYOUTMODE_12_1 */
void dvr_layout_main::SetLayoutMode_12_1()
{
    int w;
    if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_1080P)
    {
        w=(runtimesettings.iLayOutWindowWidth/5)*3;
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_720P)
    {
        w=(runtimesettings.iLayOutWindowWidth/5-7)*3;
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_SXGA)
    {
        w=(runtimesettings.iLayOutWindowWidth/5-7)*3;
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_480P)
    {
        w=(runtimesettings.iLayOutWindowWidth/5-7)*3;
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_576P)
    {
        w=(runtimesettings.iLayOutWindowWidth/5-7)*3;
    }
    else
    {
        w=(runtimesettings.iLayOutWindowWidth/5)*3;
    }
    //int w=(runtimesettings.iLayOutWindowWidth/5)*3;
    int h=(runtimesettings.iLayOutWindowHeight/4)*3;
    int x=0;
    int y=0;
    QFramelayoutChannel[0]->setGeometry(x,y,w,h);
    QFramelayoutChannel[0]->show();

    if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_1080P)
    {
        w=(runtimesettings.iLayOutWindowWidth/5);
        h=(runtimesettings.iLayOutWindowHeight/4);

        x=(runtimesettings.iLayOutWindowWidth/5)*3;
        y=0;
        QFramelayoutChannel[1]->setGeometry(x,y,w,h);
        QFramelayoutChannel[1]->show();

        x=(runtimesettings.iLayOutWindowWidth/5)*3+w;
        y=0;
        QFramelayoutChannel[2]->setGeometry(x,y,w,h);
        QFramelayoutChannel[2]->show();

        x=(runtimesettings.iLayOutWindowWidth/5)*3;
        y=h;
        QFramelayoutChannel[3]->setGeometry(x,y,w,h);
        QFramelayoutChannel[3]->show();

        x=(runtimesettings.iLayOutWindowWidth/5)*3+w;
        y=h;
        QFramelayoutChannel[4]->setGeometry(x,y,w,h);
        QFramelayoutChannel[4]->show();


        x=(runtimesettings.iLayOutWindowWidth/5)*3;
        y=h*2;
        QFramelayoutChannel[5]->setGeometry(x,y,w,h);
        QFramelayoutChannel[5]->show();

        x=(runtimesettings.iLayOutWindowWidth/5)*3+w;
        y=h*2;
        QFramelayoutChannel[6]->setGeometry(x,y,w,h);
        QFramelayoutChannel[6]->show();


        x=(runtimesettings.iLayOutWindowWidth/5)*3;
        y=h*3;
        QFramelayoutChannel[10]->setGeometry(x,y,w,h);
        QFramelayoutChannel[10]->show();

        x=(runtimesettings.iLayOutWindowWidth/5)*3+w;
        y=h*3;
        QFramelayoutChannel[11]->setGeometry(x,y,w,h);
        QFramelayoutChannel[11]->show();


        x=(runtimesettings.iLayOutWindowWidth/5)*3-w;
        y=h*3;
        QFramelayoutChannel[9]->setGeometry(x,y,w,h);
        QFramelayoutChannel[9]->show();

        x=(runtimesettings.iLayOutWindowWidth/5)*3-(w*2);
        y=h*3;
        QFramelayoutChannel[8]->setGeometry(x,y,w,h);
        QFramelayoutChannel[8]->show();

        x=(runtimesettings.iLayOutWindowWidth/5)*3-(w*3);
        y=h*3;
        QFramelayoutChannel[7]->setGeometry(x,y,w,h);
        QFramelayoutChannel[7]->show();
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_720P)
    {
        w=(runtimesettings.iLayOutWindowWidth/5-4);
        h=(runtimesettings.iLayOutWindowHeight/4);

        x=(runtimesettings.iLayOutWindowWidth/5-7)*3;
        y=0;
        QFramelayoutChannel[1]->setGeometry(x,y,w,h);
        QFramelayoutChannel[1]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-9)*3+w;
        y=0;
        QFramelayoutChannel[2]->setGeometry(x,y,w,h);
        QFramelayoutChannel[2]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-7)*3;
        y=h;
        QFramelayoutChannel[3]->setGeometry(x,y,w,h);
        QFramelayoutChannel[3]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-9)*3+w;
        y=h;
        QFramelayoutChannel[4]->setGeometry(x,y,w,h);
        QFramelayoutChannel[4]->show();


        x=(runtimesettings.iLayOutWindowWidth/5-7)*3;
        y=h*2;
        QFramelayoutChannel[5]->setGeometry(x,y,w,h);
        QFramelayoutChannel[5]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-9)*3+w;
        y=h*2;
        QFramelayoutChannel[6]->setGeometry(x,y,w,h);
        QFramelayoutChannel[6]->show();


        x=(runtimesettings.iLayOutWindowWidth/5-5)*3;
        y=h*3;
        QFramelayoutChannel[10]->setGeometry(x-7,y,w-3,h);
        QFramelayoutChannel[10]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-5)*3+w;
        y=h*3;
        QFramelayoutChannel[11]->setGeometry(x-9,y,w-3,h);
        QFramelayoutChannel[11]->show();


        x=(runtimesettings.iLayOutWindowWidth/5-5)*3-w;
        y=h*3;
        QFramelayoutChannel[9]->setGeometry(x-4,y,w-3,h);
        QFramelayoutChannel[9]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-5)*3-(w*2);
        y=h*3;
        QFramelayoutChannel[8]->setGeometry(x-1,y,w-3,h);
        QFramelayoutChannel[8]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-5)*3-(w*3);
        y=h*3;
        QFramelayoutChannel[7]->setGeometry(x+1,y,w-3,h);
        QFramelayoutChannel[7]->show();
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_SXGA)
    {
        w=(runtimesettings.iLayOutWindowWidth/5-4);
        h=(runtimesettings.iLayOutWindowHeight/4);

        x=(runtimesettings.iLayOutWindowWidth/5-7)*3;
        y=0;
        QFramelayoutChannel[1]->setGeometry(x,y,w,h);
        QFramelayoutChannel[1]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-9)*3+w;
        y=0;
        QFramelayoutChannel[2]->setGeometry(x,y,w,h);
        QFramelayoutChannel[2]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-7)*3;
        y=h;
        QFramelayoutChannel[3]->setGeometry(x,y,w,h);
        QFramelayoutChannel[3]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-9)*3+w;
        y=h;
        QFramelayoutChannel[4]->setGeometry(x,y,w,h);
        QFramelayoutChannel[4]->show();


        x=(runtimesettings.iLayOutWindowWidth/5-7)*3;
        y=h*2;
        QFramelayoutChannel[5]->setGeometry(x,y,w,h);
        QFramelayoutChannel[5]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-9)*3+w;
        y=h*2;
        QFramelayoutChannel[6]->setGeometry(x,y,w,h);
        QFramelayoutChannel[6]->show();


        x=(runtimesettings.iLayOutWindowWidth/5-5)*3;
        y=h*3;
        QFramelayoutChannel[10]->setGeometry(x-7,y,w-3,h);
        QFramelayoutChannel[10]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-5)*3+w;
        y=h*3;
        QFramelayoutChannel[11]->setGeometry(x-9,y,w-3,h);
        QFramelayoutChannel[11]->show();


        x=(runtimesettings.iLayOutWindowWidth/5-5)*3-w;
        y=h*3;
        QFramelayoutChannel[9]->setGeometry(x-4,y,w-3,h);
        QFramelayoutChannel[9]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-5)*3-(w*2);
        y=h*3;
        QFramelayoutChannel[8]->setGeometry(x-1,y,w-3,h);
        QFramelayoutChannel[8]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-5)*3-(w*3);
        y=h*3;
        QFramelayoutChannel[7]->setGeometry(x+1,y,w-3,h);
        QFramelayoutChannel[7]->show();
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_480P)
    {
        w=(runtimesettings.iLayOutWindowWidth/5-2);
        h=(runtimesettings.iLayOutWindowHeight/4);

        x=(runtimesettings.iLayOutWindowWidth/5-6)*3;
        y=0;
        QFramelayoutChannel[1]->setGeometry(x,y,w,h);
        QFramelayoutChannel[1]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-7)*3+w;
        y=0;
        QFramelayoutChannel[2]->setGeometry(x,y,w,h);
        QFramelayoutChannel[2]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-6)*3;
        y=h;
        QFramelayoutChannel[3]->setGeometry(x,y,w,h);
        QFramelayoutChannel[3]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-7)*3+w;
        y=h;
        QFramelayoutChannel[4]->setGeometry(x,y,w,h);
        QFramelayoutChannel[4]->show();


        x=(runtimesettings.iLayOutWindowWidth/5-6)*3;
        y=h*2;
        QFramelayoutChannel[5]->setGeometry(x,y,w,h);
        QFramelayoutChannel[5]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-7)*3+w;
        y=h*2;
        QFramelayoutChannel[6]->setGeometry(x,y,w,h);
        QFramelayoutChannel[6]->show();


        x=(runtimesettings.iLayOutWindowWidth/5-5)*3;
        y=h*3;
        QFramelayoutChannel[10]->setGeometry(x-6,y,w-3,h);
        QFramelayoutChannel[10]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-5)*3+w;
        y=h*3;
        QFramelayoutChannel[11]->setGeometry(x-7,y,w-3,h);
        QFramelayoutChannel[11]->show();


        x=(runtimesettings.iLayOutWindowWidth/5-5)*3-w;
        y=h*3;
        QFramelayoutChannel[9]->setGeometry(x-4,y,w-3,h);
        QFramelayoutChannel[9]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-5)*3-(w*2);
        y=h*3;
        QFramelayoutChannel[8]->setGeometry(x-1,y,w-3,h);
        QFramelayoutChannel[8]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-5)*3-(w*3);
        y=h*3;
        QFramelayoutChannel[7]->setGeometry(x+1,y,w-3,h);
        QFramelayoutChannel[7]->show();
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_576P)
    {
        w=(runtimesettings.iLayOutWindowWidth/5-2);
        h=(runtimesettings.iLayOutWindowHeight/4);

        x=(runtimesettings.iLayOutWindowWidth/5-6)*3;
        y=0;
        QFramelayoutChannel[1]->setGeometry(x,y,w,h);
        QFramelayoutChannel[1]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-7)*3+w;
        y=0;
        QFramelayoutChannel[2]->setGeometry(x,y,w,h);
        QFramelayoutChannel[2]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-6)*3;
        y=h;
        QFramelayoutChannel[3]->setGeometry(x,y,w,h);
        QFramelayoutChannel[3]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-7)*3+w;
        y=h;
        QFramelayoutChannel[4]->setGeometry(x,y,w,h);
        QFramelayoutChannel[4]->show();


        x=(runtimesettings.iLayOutWindowWidth/5-6)*3;
        y=h*2;
        QFramelayoutChannel[5]->setGeometry(x,y,w,h);
        QFramelayoutChannel[5]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-7)*3+w;
        y=h*2;
        QFramelayoutChannel[6]->setGeometry(x,y,w,h);
        QFramelayoutChannel[6]->show();


        x=(runtimesettings.iLayOutWindowWidth/5-5)*3;
        y=h*3;
        QFramelayoutChannel[10]->setGeometry(x-6,y,w-3,h);
        QFramelayoutChannel[10]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-5)*3+w;
        y=h*3;
        QFramelayoutChannel[11]->setGeometry(x-7,y,w-3,h);
        QFramelayoutChannel[11]->show();


        x=(runtimesettings.iLayOutWindowWidth/5-5)*3-w;
        y=h*3;
        QFramelayoutChannel[9]->setGeometry(x-4,y,w-3,h);
        QFramelayoutChannel[9]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-5)*3-(w*2);
        y=h*3;
        QFramelayoutChannel[8]->setGeometry(x-1,y,w-3,h);
        QFramelayoutChannel[8]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-5)*3-(w*3);
        y=h*3;
        QFramelayoutChannel[7]->setGeometry(x+1,y,w-3,h);
        QFramelayoutChannel[7]->show();
    }
    else
    {
        w=(runtimesettings.iLayOutWindowWidth/5);
        h=(runtimesettings.iLayOutWindowHeight/4);

        x=(runtimesettings.iLayOutWindowWidth/5)*3;
        y=0;
        QFramelayoutChannel[1]->setGeometry(x,y,w,h);
        QFramelayoutChannel[1]->show();

        x=(runtimesettings.iLayOutWindowWidth/5)*3+w;
        y=0;
        QFramelayoutChannel[2]->setGeometry(x,y,w,h);
        QFramelayoutChannel[2]->show();

        x=(runtimesettings.iLayOutWindowWidth/5)*3;
        y=h;
        QFramelayoutChannel[3]->setGeometry(x,y,w,h);
        QFramelayoutChannel[3]->show();

        x=(runtimesettings.iLayOutWindowWidth/5)*3+w;
        y=h;
        QFramelayoutChannel[4]->setGeometry(x,y,w,h);
        QFramelayoutChannel[4]->show();


        x=(runtimesettings.iLayOutWindowWidth/5)*3;
        y=h*2;
        QFramelayoutChannel[5]->setGeometry(x,y,w,h);
        QFramelayoutChannel[5]->show();

        x=(runtimesettings.iLayOutWindowWidth/5)*3+w;
        y=h*2;
        QFramelayoutChannel[6]->setGeometry(x,y,w,h);
        QFramelayoutChannel[6]->show();


        x=(runtimesettings.iLayOutWindowWidth/5)*3;
        y=h*3;
        QFramelayoutChannel[10]->setGeometry(x,y,w,h);
        QFramelayoutChannel[10]->show();

        x=(runtimesettings.iLayOutWindowWidth/5)*3+w;
        y=h*3;
        QFramelayoutChannel[11]->setGeometry(x,y,w,h);
        QFramelayoutChannel[11]->show();


        x=(runtimesettings.iLayOutWindowWidth/5)*3-w;
        y=h*3;
        QFramelayoutChannel[9]->setGeometry(x,y,w,h);
        QFramelayoutChannel[9]->show();

        x=(runtimesettings.iLayOutWindowWidth/5)*3-(w*2);
        y=h*3;
        QFramelayoutChannel[8]->setGeometry(x,y,w,h);
        QFramelayoutChannel[8]->show();

        x=(runtimesettings.iLayOutWindowWidth/5)*3-(w*3);
        y=h*3;
        QFramelayoutChannel[7]->setGeometry(x,y,w,h);
        QFramelayoutChannel[7]->show();
    }

}

/*! set layout mode to LAYOUTMODE_14 */
void dvr_layout_main::SetLayoutMode_14()
{
    int w;
    if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_1080P)
    {
        w=(runtimesettings.iLayOutWindowWidth/5)*2;
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_720P)
    {
        w=(runtimesettings.iLayOutWindowWidth/5-7)*2;
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_SXGA)
    {
        w=(runtimesettings.iLayOutWindowWidth/5-7)*2;
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_480P)
    {
        w=(runtimesettings.iLayOutWindowWidth/5-6)*2;
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_576P)
    {
        w=(runtimesettings.iLayOutWindowWidth/5-6)*2;
    }
    else
    {
        w=(runtimesettings.iLayOutWindowWidth/5)*2;
    }
    //int w=(runtimesettings.iLayOutWindowWidth/5)*2;
    int h=(runtimesettings.iLayOutWindowHeight/4)*2;
    int x=0;
    int y=0;
    QFramelayoutChannel[0]->setGeometry(x,y,w,h);
    QFramelayoutChannel[0]->show();

    x=w;
    y=0;
    QFramelayoutChannel[1]->setGeometry(x,y,w,h);
    QFramelayoutChannel[1]->show();

    if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_1080P)
    {
        w=(runtimesettings.iLayOutWindowWidth/5);
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_720P)
    {
        w=(runtimesettings.iLayOutWindowWidth/5-7);
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_SXGA)
    {
        w=(runtimesettings.iLayOutWindowWidth/5-7);
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_480P)
    {
        w=(runtimesettings.iLayOutWindowWidth/5-5);
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_576P)
    {
        w=(runtimesettings.iLayOutWindowWidth/5-5);
    }
    else
    {
        w=(runtimesettings.iLayOutWindowWidth/5);
    }
    //w=(runtimesettings.iLayOutWindowWidth/5);
    h=(runtimesettings.iLayOutWindowHeight/4);

    x=0;
    y=(runtimesettings.iLayOutWindowHeight/4)*2;
    QFramelayoutChannel[2]->setGeometry(x,y,w,h);
    QFramelayoutChannel[2]->show();

    x=w;
    y=(runtimesettings.iLayOutWindowHeight/4)*2;
    QFramelayoutChannel[3]->setGeometry(x,y,w,h);
    QFramelayoutChannel[3]->show();

    x=w*2;
    y=(runtimesettings.iLayOutWindowHeight/4)*2;
    QFramelayoutChannel[4]->setGeometry(x,y,w,h);
    QFramelayoutChannel[4]->show();

    x=w*3;
    y=(runtimesettings.iLayOutWindowHeight/4)*2;
    QFramelayoutChannel[5]->setGeometry(x,y,w,h);
    QFramelayoutChannel[5]->show();


    x=0;
    y=(runtimesettings.iLayOutWindowHeight/4)*2+h;
    QFramelayoutChannel[6]->setGeometry(x,y,w,h);
    QFramelayoutChannel[6]->show();

    x=w;
    y=(runtimesettings.iLayOutWindowHeight/4)*2+h;
    QFramelayoutChannel[7]->setGeometry(x,y,w,h);
    QFramelayoutChannel[7]->show();

    x=w*2;
    y=(runtimesettings.iLayOutWindowHeight/4)*2+h;
    QFramelayoutChannel[8]->setGeometry(x,y,w,h);
    QFramelayoutChannel[8]->show();

    x=w*3;
    y=(runtimesettings.iLayOutWindowHeight/4)*2+h;
    QFramelayoutChannel[9]->setGeometry(x,y,w,h);
    QFramelayoutChannel[9]->show();

    if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_1080P)
    {
        x=(runtimesettings.iLayOutWindowWidth/5)*4;
        y=0;
        QFramelayoutChannel[10]->setGeometry(x,y,w,h);
        QFramelayoutChannel[10]->show();


        x=(runtimesettings.iLayOutWindowWidth/5)*4;
        y=h;
        QFramelayoutChannel[11]->setGeometry(x,y,w,h);
        QFramelayoutChannel[11]->show();

        x=(runtimesettings.iLayOutWindowWidth/5)*4;
        y=h*2;
        QFramelayoutChannel[12]->setGeometry(x,y,w,h);
        QFramelayoutChannel[12]->show();

        x=(runtimesettings.iLayOutWindowWidth/5)*4;
        y=h*3;
        QFramelayoutChannel[13]->setGeometry(x,y,w,h);
        QFramelayoutChannel[13]->show();
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_720P)
    {
        x=(runtimesettings.iLayOutWindowWidth/5-7)*4;
        y=0;
        QFramelayoutChannel[10]->setGeometry(x,y,w,h);
        QFramelayoutChannel[10]->show();


        x=(runtimesettings.iLayOutWindowWidth/5-7)*4;
        y=h;
        QFramelayoutChannel[11]->setGeometry(x,y,w,h);
        QFramelayoutChannel[11]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-7)*4;
        y=h*2;
        QFramelayoutChannel[12]->setGeometry(x,y,w,h);
        QFramelayoutChannel[12]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-7)*4;
        y=h*3;
        QFramelayoutChannel[13]->setGeometry(x,y,w,h);
        QFramelayoutChannel[13]->show();
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_SXGA)
    {
        x=(runtimesettings.iLayOutWindowWidth/5-7)*4;
        y=0;
        QFramelayoutChannel[10]->setGeometry(x,y,w,h);
        QFramelayoutChannel[10]->show();


        x=(runtimesettings.iLayOutWindowWidth/5-7)*4;
        y=h;
        QFramelayoutChannel[11]->setGeometry(x,y,w,h);
        QFramelayoutChannel[11]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-7)*4;
        y=h*2;
        QFramelayoutChannel[12]->setGeometry(x,y,w,h);
        QFramelayoutChannel[12]->show();

        x=(runtimesettings.iLayOutWindowWidth/5-7)*4;
        y=h*3;
        QFramelayoutChannel[13]->setGeometry(x,y,w,h);
        QFramelayoutChannel[13]->show();
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_480P)
    {
        x=(runtimesettings.iLayOutWindowWidth/5 -5)*4;
        y=0;
        QFramelayoutChannel[10]->setGeometry(x,y,w,h);
        QFramelayoutChannel[10]->show();


        x=(runtimesettings.iLayOutWindowWidth/5 -5)*4;
        y=h;
        QFramelayoutChannel[11]->setGeometry(x,y,w,h);
        QFramelayoutChannel[11]->show();

        x=(runtimesettings.iLayOutWindowWidth/5 -5)*4;
        y=h*2;
        QFramelayoutChannel[12]->setGeometry(x,y,w,h);
        QFramelayoutChannel[12]->show();

        x=(runtimesettings.iLayOutWindowWidth/5 -5)*4;
        y=h*3;
        QFramelayoutChannel[13]->setGeometry(x,y,w,h);
        QFramelayoutChannel[13]->show();
    }
    else if(runtimesettings.OutputResolution[profile.layout.iMainOutput]==OUTPUT_RESOLUTION_576P)
    {
        x=(runtimesettings.iLayOutWindowWidth/5 -5)*4;
        y=0;
        QFramelayoutChannel[10]->setGeometry(x,y,w,h);
        QFramelayoutChannel[10]->show();


        x=(runtimesettings.iLayOutWindowWidth/5 -5)*4;
        y=h;
        QFramelayoutChannel[11]->setGeometry(x,y,w,h);
        QFramelayoutChannel[11]->show();

        x=(runtimesettings.iLayOutWindowWidth/5 -5)*4;
        y=h*2;
        QFramelayoutChannel[12]->setGeometry(x,y,w,h);
        QFramelayoutChannel[12]->show();

        x=(runtimesettings.iLayOutWindowWidth/5 -5)*4;
        y=h*3;
        QFramelayoutChannel[13]->setGeometry(x,y,w,h);
        QFramelayoutChannel[13]->show();
    }
    else
    {
        x=(runtimesettings.iLayOutWindowWidth/5)*4;
        y=0;
        QFramelayoutChannel[10]->setGeometry(x,y,w,h);
        QFramelayoutChannel[10]->show();


        x=(runtimesettings.iLayOutWindowWidth/5)*4;
        y=h;
        QFramelayoutChannel[11]->setGeometry(x,y,w,h);
        QFramelayoutChannel[11]->show();

        x=(runtimesettings.iLayOutWindowWidth/5)*4;
        y=h*2;
        QFramelayoutChannel[12]->setGeometry(x,y,w,h);
        QFramelayoutChannel[12]->show();

        x=(runtimesettings.iLayOutWindowWidth/5)*4;
        y=h*3;
        QFramelayoutChannel[13]->setGeometry(x,y,w,h);
        QFramelayoutChannel[13]->show();
    }
}
/*! channel name, icon, no video처리등 수행 */
void dvr_layout_main::DrawOSDChannel()
{
	//Live/Playback에 따라 ch name, icon등 표시
    if(runtimesettings.iLivePlaybackMode==LIB_LIVE_MODE)
    {
        DrawOSDChannelLive();
    }
    else if(runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
    {
        DrawOSDChannelPlayback();
    }
}

/*! Draw icons */
void dvr_layout_main::DrawOSDIcon()
{
	//Live/Playback에 따라 icon 표시
    if(runtimesettings.iLivePlaybackMode==LIB_LIVE_MODE)
    {
        DrawOSDIconLive();
    }
    else if(runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
    {
        DrawOSDIconPlayback();
    }
}

/*! live mode에서 channel name, icon, no video처리등 수행*/
void dvr_layout_main::DrawOSDChannelLive()
{
    for(int i=0;i<runtimesettings.iLayout;i++)
    {
        int count=i+(runtimesettings.iLayout*runtimesettings.iPageNum);

        if(count<MAX_CAMERA)
        {
        	//Camera name 표시
            QString qstr=StrToQString(profile.camera[count].strcameraname);
            QTitlelayoutChannel[i]->setText(qstr);
            if(profile.camera[count].iEnable && profile.camera[count].iCovertOn==0)
            {
            	//Camera Enable, covert아닌경우 - TI logo hide 
                if(QNoVideolayoutChannel[i]->isVisible())
                {
                    QTitlelayoutChannel[i]->setStyleSheet(QString::fromUtf8("color:rgb(255,255,255); border: rgb(255, 0, 255);"));
                    QNoVideolayoutChannel[i]->hide();
                    QTitlelayoutChannel[i]->show();
                }
            }
            else
            {
            	//Camera disable, covert인경우 - ti logo 표시, icon hide 
                QTitlelayoutChannel[i]->setStyleSheet(QString::fromUtf8("color:rgb(255,255,255); background-color: rgb(0, 0, 0);"));
                QNoVideolayoutChannel[i]->show();
                QTitlelayoutChannel[i]->show();

                QVideoRecIconlayoutChannel[i]->hide();
                QAudioRecIconlayoutChannel[i]->hide();
                QMotionIconlayoutChannel[i]->hide();
                QVideoLossIconlayoutChannel[i]->hide();
                QPlaybackIconlayoutChannel[i]->hide();
            }

        }
        else
        {
        	//존재하지 않는 채널인경우 - ti logo 표시, camera name, icon hide 
            if(!QNoVideolayoutChannel[i]->isVisible())
            {
                QNoVideolayoutChannel[i]->show();
            }
            QTitlelayoutChannel[i]->hide();

            QVideoRecIconlayoutChannel[i]->hide();
            QAudioRecIconlayoutChannel[i]->hide();
            QMotionIconlayoutChannel[i]->hide();
            QVideoLossIconlayoutChannel[i]->hide();
            QPlaybackIconlayoutChannel[i]->hide();
        }
    }
	//상태에 따라 icon 표시
    DrawOSDIconLive();

}

/*! playback mode에서 channel name, icon, no video처리등 수행*/
void dvr_layout_main::DrawOSDChannelPlayback()
{

    for(int i=0;i<runtimesettings.iLayout;i++)
    {
        int count=i+(runtimesettings.iLayout*runtimesettings.iPageNum);

        if(count<MAX_CAMERA)
        {
            int iChEnable;
            if(profile.iSubOutputEnable)
            {
                    iChEnable = profile.camera[count].iEnable  && profile.camera[count].iCovertOn==0;
            }
            else
            {
                    iChEnable = profile.playbackcamera[count].iEnable;
            }
            //Camera name 표시
            QString qstr=StrToQString(profile.camera[count].strcameraname);
            QTitlelayoutChannel[i]->setText(qstr);
            if(iChEnable)
            {
            	//Camera Enable, covert아닌경우 - TI logo hide 
                if(QNoVideolayoutChannel[i]->isVisible())
                {
                    QTitlelayoutChannel[i]->setStyleSheet(QString::fromUtf8("color:rgb(255,255,255); border: rgb(255, 0, 255);"));
                    QNoVideolayoutChannel[i]->hide();
                    QTitlelayoutChannel[i]->show();
                }
            }
            else
            {
            	//Camera disable, covert인경우 - ti logo 표시, icon hide
                QTitlelayoutChannel[i]->setStyleSheet(QString::fromUtf8("color:rgb(255,255,255); background-color: rgb(0, 0, 0);"));
                QNoVideolayoutChannel[i]->show();
                QTitlelayoutChannel[i]->show();

                QVideoRecIconlayoutChannel[i]->hide();
                QAudioRecIconlayoutChannel[i]->hide();
                QMotionIconlayoutChannel[i]->hide();
                QVideoLossIconlayoutChannel[i]->hide();
                QPlaybackIconlayoutChannel[i]->hide();
            }
        }
        else
        {
        	//존재하지 않는 채널인경우 - ti logo 표시, camera name, icon hide
            if(!QNoVideolayoutChannel[i]->isVisible())
            {
                QNoVideolayoutChannel[i]->show();
            }
            QTitlelayoutChannel[i]->hide();

            QVideoRecIconlayoutChannel[i]->hide();
            QAudioRecIconlayoutChannel[i]->hide();
            QMotionIconlayoutChannel[i]->hide();
            QVideoLossIconlayoutChannel[i]->hide();
            QPlaybackIconlayoutChannel[i]->hide();
        }
    }
	//상태에 따라 icon 표시
    DrawOSDIconPlayback();

}

/*!live mode에서 detect status에 따라서 icon을 표시*/
void dvr_layout_main::DrawOSDIconLive()
{
    for(int i=0;i<runtimesettings.iLayout;i++)
    {
        int count=i+(runtimesettings.iLayout*runtimesettings.iPageNum);

        if(count<MAX_CAMERA && profile.camera[count].iEnable)
        {
        	//playback icon hide, 기타 icon은 상태에 따라 show/hide
            QPlaybackIconlayoutChannel[i]->hide();

            if(detectstatus.detectrecording[count].iRecordingOn==1)
                QVideoRecIconlayoutChannel[i]->show();
            else
                QVideoRecIconlayoutChannel[i]->hide();


            if(detectstatus.detectaudiorecording[count].iRecordingOn==1)
                QAudioRecIconlayoutChannel[i]->show();
            else
                QAudioRecIconlayoutChannel[i]->hide();


            if(detectstatus.detectmotion[count].iMotionDetectOn==1)
                QMotionIconlayoutChannel[i]->show();
            else
                QMotionIconlayoutChannel[i]->hide();


            if(detectstatus.detectvideoloss[count].iVideoLossOn==1)
                QVideoLossIconlayoutChannel[i]->show();
            else
                QVideoLossIconlayoutChannel[i]->hide();
        }
    }
}

/*!playback mode에서 playbackmode,detect status에 따라서 icon을 표시*/
void dvr_layout_main::DrawOSDIconPlayback()
{

    for(int i=0;i<runtimesettings.iLayout;i++)
    {
        int count=i+(runtimesettings.iLayout*runtimesettings.iPageNum);
        int iChEnable;
    	if(profile.iSubOutputEnable)
        {
            iChEnable = profile.camera[count].iEnable;
        }
        else
        {
            iChEnable = profile.playbackcamera[count].iEnable;
        }
        if(count<MAX_CAMERA && iChEnable)
        {
        	//playback icon은 playback mode에 따라 표시, 기타 icon은 상태에 따라 show/hide
            QString qstr;
            switch(detectstatus.playbackmodeinfo[count].iPlaybackMode)
            {
                    case PLAYBACKMODE_STOP:
                    qstr=StrToQString((char*)PLAYBACKMODE_ICON_STOP);
                    break;
                    case PLAYBACKMODE_PLAY:
                    qstr=StrToQString((char*)PLAYBACKMODE_ICON_PLAY);
                    break;
                    case PLAYBACKMODE_PUASE:
                    qstr=StrToQString((char*)PLAYBACKMODE_ICON_PAUSE);
                    break;
                    case PLAYBACKMODE_REVERSE:
                    qstr=StrToQString((char*)PLAYBACKMODE_ICON_REVERSE);
                    break;
                    case PLAYBACKMODE_REVERSESTEP:
                    qstr=StrToQString((char*)PLAYBACKMODE_ICON_REVERSESTEP);
                    break;
                    case PLAYBACKMODE_FORWARD:
                    qstr=StrToQString((char*)PLAYBACKMODE_ICON_FORWARD);
                    break;
                    case PLAYBACKMODE_FORAWRDSTEP:
                    qstr=StrToQString((char*)PLAYBACKMODE_ICON_FORWARDSTEP);
                    break;
                    default:
                    break;
            }
            QPlaybackIconlayoutChannel[i]->setPixmap(QPixmap(qstr));
            QPlaybackIconlayoutChannel[i]->show();

            if(detectstatus.detectrecording[count].iRecordingOn==1)
                QVideoRecIconlayoutChannel[i]->show();
            else
                QVideoRecIconlayoutChannel[i]->hide();

            if(detectstatus.detectaudiorecording[count].iRecordingOn==1)
                QAudioRecIconlayoutChannel[i]->show();
            else
                QAudioRecIconlayoutChannel[i]->hide();

            if(detectstatus.detectmotion[count].iMotionDetectOn==1)
                QMotionIconlayoutChannel[i]->show();
            else
                QMotionIconlayoutChannel[i]->hide();

            if(detectstatus.detectvideoloss[count].iVideoLossOn==1)
                QVideoLossIconlayoutChannel[i]->show();
            else
                QVideoLossIconlayoutChannel[i]->hide();
        }
    }
}

/*! border line 설정에 따라서 layout에 border line on/off를 수행*/
void dvr_layout_main::SetBorderLineShowHide(int iOnOff)
{
    for(int i=0;i<MAX_LAYOUTCHANNEL;i++)
    {
        if(iOnOff==0){
            QFramelayoutChannel[i]->setFrameShape(QFrame::NoFrame);
			QFramelayoutChannel[i]->setStyleSheet(QString::fromUtf8("background-color:rgb(255,0,255); border: rgb(255, 0, 255);"));
        	}
        else{
            QFramelayoutChannel[i]->setFrameShape(QFrame::Box);
			QFramelayoutChannel[i]->setStyleSheet(QString::fromUtf8("background-color:rgb(255,0,255); border: 1px solid white;"));
        	}

    }
}


