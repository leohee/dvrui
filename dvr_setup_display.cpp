/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include "dvr_defines.h"
#include "dvr_setup.h"
#include "ui_dvr_setup.h"
#include "dvr_main.h"
/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
extern dvrmain *pMainClass;
extern dvr_layout_main *pLayoutClass;
extern dvr_toolbar_main *pToolbar;
extern dvr_statusbar *pStatusbar;
/*----------------------------------------------------------------------------
 local functions
-----------------------------------------------------------------------------*/
/*!display tab의 control들을 초기화 */
void dvr_setup::InitSetupDisplayControls()
{
    connect(ui->outputselectcomboBox, SIGNAL(currentIndexChanged (int)), this, SLOT(ChangeOutputSelectSlot(int)));
    connect(ui->sequenceintervalslider, SIGNAL(valueChanged(int)), this, SLOT(ChangeSequenceIntevalSlot(int)));
    connect(ui->subsequenceintervalslider, SIGNAL(valueChanged(int)), this, SLOT(ChangeSubSequenceIntevalSlot(int)));
}
/*!profile data struct에 따라서 ui에 value setting */
void dvr_setup::SetDisplaydata()
{

//OUTPUT DEVICE
 //   if(dlgdata.layout.iMainOutput==0 && dlgdata.layout.iSubOutput==0)
  //  {
        m_bOneMonitorDisplay=true;
     //   ui->onemonitorcheckBox->setChecked(true);
        ui->outputselectcomboBox->setEnabled(false);
  //  }
  //	ui->suboutputselectEdit->hide();
#if 0 //remove the two output set, and just use one monitor display now
    else
    {
        m_bOneMonitorDisplay=false;
        ui->onemonitorcheckBox->setChecked(false);
        ui->outputselectcomboBox->setEnabled(true);
        ui->outputselectcomboBox->setCurrentIndex(dlgdata.layout.iMainOutput);
        if(dlgdata.layout.iSubOutput==0){
            ui->suboutputselectEdit->setText(QString::fromUtf8("HDMI0/VGA"));
            ui->resolutionselectcomboBox->clear();
            ui->resolutionselectcomboBox->insertItems(0, QStringList()
                                                  << QApplication::translate("dvr_setup", "1080P60", 0, QApplication::UnicodeUTF8)
                                                           << QApplication::translate("dvr_setup", "720P60", 0, QApplication::UnicodeUTF8)
                                                           << QApplication::translate("dvr_setup", "SXGA60", 0, QApplication::UnicodeUTF8)
                                                           << QApplication::translate("dvr_setup", "XGA60", 0, QApplication::UnicodeUTF8)
                                                           << QApplication::translate("dvr_setup", "1080P50", 0, QApplication::UnicodeUTF8)
                                                          );
            ui->resolutionselectcomboBox_2->clear();
            ui->resolutionselectcomboBox_2->insertItems(0, QStringList()
                                                  << QApplication::translate("dvr_setup", "1080P60", 0, QApplication::UnicodeUTF8)
                                                           << QApplication::translate("dvr_setup", "720P60", 0, QApplication::UnicodeUTF8)
                                                           << QApplication::translate("dvr_setup", "SXGA60", 0, QApplication::UnicodeUTF8)
                                                           << QApplication::translate("dvr_setup", "XGA60", 0, QApplication::UnicodeUTF8)
                                                           << QApplication::translate("dvr_setup", "1080P50", 0, QApplication::UnicodeUTF8)
                                                           << QApplication::translate("dvr_setup", "480P", 0, QApplication::UnicodeUTF8)
                                                           << QApplication::translate("dvr_setup", "576P", 0, QApplication::UnicodeUTF8)
                                                          );
        }
        else{
            ui->suboutputselectEdit->setText(QString::fromUtf8("HDMI1"));
            ui->resolutionselectcomboBox->clear();
            ui->resolutionselectcomboBox->insertItems(0, QStringList()
                                                  << QApplication::translate("dvr_setup", "1080P60", 0, QApplication::UnicodeUTF8)
                                                           << QApplication::translate("dvr_setup", "720P60", 0, QApplication::UnicodeUTF8)
                                                           << QApplication::translate("dvr_setup", "SXGA60", 0, QApplication::UnicodeUTF8)
                                                           << QApplication::translate("dvr_setup", "XGA60", 0, QApplication::UnicodeUTF8)
                                                           << QApplication::translate("dvr_setup", "1080P50", 0, QApplication::UnicodeUTF8)
                                                           << QApplication::translate("dvr_setup", "480P", 0, QApplication::UnicodeUTF8)
                                                           << QApplication::translate("dvr_setup", "576P", 0, QApplication::UnicodeUTF8)
                                                          );
            ui->resolutionselectcomboBox_2->clear();
            ui->resolutionselectcomboBox_2->insertItems(0, QStringList()
                                                  << QApplication::translate("dvr_setup", "1080P60", 0, QApplication::UnicodeUTF8)
                                                           << QApplication::translate("dvr_setup", "720P60", 0, QApplication::UnicodeUTF8)
                                                           << QApplication::translate("dvr_setup", "SXGA60", 0, QApplication::UnicodeUTF8)
                                                           << QApplication::translate("dvr_setup", "XGA60", 0, QApplication::UnicodeUTF8)
                                                           << QApplication::translate("dvr_setup", "1080P50", 0, QApplication::UnicodeUTF8)
                                                          );
        }
    }
#endif
    ui->spotchselectcomboBox->setCurrentIndex(dlgdata.iSpotOutputCh);
    ui->resolutionselectcomboBox->setCurrentIndex(dlgdata.maindisplayoutput.iResolution);
    //ui->resolutionselectcomboBox_2->setCurrentIndex(dlgdata.maindisplayoutput.iResolution);//(dlgdata.subdisplayoutput.iResolution);
    ui->spotresolutionselectcomboBox->setCurrentIndex(dlgdata.spotdisplayoutput.iResolution);

    ui->resolutionselectcomboBox->show();
    ui->sequenceintervalslider->setSliderPosition(dlgdata.sequence.iInterval);
    ui->subsequenceintervalslider->setSliderPosition(dlgdata.secondsequence.iInterval);

    ui->sequenceintervalvalue->setText(SetSequenceIntervalValueText(dlgdata.sequence.iInterval));
    ui->subsequenceintervalvalue->setText(SetSequenceIntervalValueText(dlgdata.secondsequence.iInterval));

//OSD
    ui->borderlineselectcomboBox->setCurrentIndex(dlgdata.mainosd.iBorderLineOn);
    ui->maindateformatcomboBox->setCurrentIndex(dlgdata.mainosd.iDateFormat);

 //   if(ui->outputselectcomboBox->currentIndex() == dlgdata.layout.iSubOutput)
  //  {
       // ui->resolutionselectlabel_2->hide();
        //ui->resolutionselectcomboBox_2->hide();
   // }
   /* else
    {
        ui->resolutionselectlabel_2->show();
        ui->resolutionselectcomboBox_2->show();
    }*/
}
/*!ui control의 value를 읽고 해당 기능을 적용시킴 */
void dvr_setup::GetDisplaydata()
{
//OUTPUT DEVICE
    dlgdata.layout.iMainOutput=ui->outputselectcomboBox->currentIndex();
    if(m_bOneMonitorDisplay)
    {
        dlgdata.layout.iSubOutput=0;
    }
    else
    {
        if(dlgdata.layout.iMainOutput==0)
        {
            dlgdata.layout.iSubOutput=1;
        }
        else
        {
            dlgdata.layout.iSubOutput=0;
        }
    }
    dlgdata.iSpotOutputCh = ui->spotchselectcomboBox->currentIndex();

    dlgdata.maindisplayoutput.iResolution=ui->resolutionselectcomboBox->currentIndex();
   // dlgdata.subdisplayoutput.iResolution=dlgdata.maindisplayoutput.iResolution;//ui->resolutionselectcomboBox_2->currentIndex();
    dlgdata.spotdisplayoutput.iResolution=ui->spotresolutionselectcomboBox->currentIndex();

    if(dlgdata.layout.iMainOutput!=profile.layout.iMainOutput ||
        dlgdata.layout.iSubOutput!=profile.layout.iSubOutput)
    {
        profile.layout.iMainOutput=dlgdata.layout.iMainOutput;
        profile.layout.iSubOutput=dlgdata.layout.iSubOutput;
		
		profile.maindisplayoutput.iResolution=dlgdata.maindisplayoutput.iResolution;
		runtimesettings.OutputResolution[profile.layout.iMainOutput]=dlgdata.maindisplayoutput.iResolution;
		
		profile.subdisplayoutput.iResolution=dlgdata.maindisplayoutput.iResolution;//dlgdata.subdisplayoutput.iResolution;
		runtimesettings.OutputResolution[profile.layout.iSubOutput] = dlgdata.maindisplayoutput.iResolution;//dlgdata.subdisplayoutput.iResolution;

        profile.spotdisplayoutput.iResolution=dlgdata.spotdisplayoutput.iResolution;

        int mainResolution;
        switch(profile.maindisplayoutput.iResolution)
        {
	        case OUTPUT_RESOLUTION_1080P:     
	            mainResolution = DC_MODE_1080P_60;
	            break;
	        case OUTPUT_RESOLUTION_720P:     
	            mainResolution = DC_MODE_720P_60;
	            break;
	        case OUTPUT_RESOLUTION_SXGA:     
	            mainResolution = DC_MODE_SXGA_60;
	            break;
	        case OUTPUT_RESOLUTION_XGA:     
	            mainResolution = DC_MODE_XGA_60;
	            break;
	        case OUTPUT_RESOLUTION_1080P50:     
	            mainResolution = DC_MODE_1080P_50;
	            break;
	        case OUTPUT_RESOLUTION_480P:     
	            mainResolution = DC_MODE_480P;
	            break;
	        case OUTPUT_RESOLUTION_576P:     
	            mainResolution = DC_MODE_576P;
	            break;
	        default:
	            mainResolution = DC_MODE_1080P_60;
	            break;
        }

		int subResolution;
		switch(profile.subdisplayoutput.iResolution)
		{
			case OUTPUT_RESOLUTION_1080P:	  
				subResolution = DC_MODE_1080P_60;
				break;
			case OUTPUT_RESOLUTION_720P:	 
				subResolution = DC_MODE_720P_60;
				break;
			case OUTPUT_RESOLUTION_SXGA:	 
				subResolution = DC_MODE_SXGA_60;
				break;
			case OUTPUT_RESOLUTION_XGA: 	
				subResolution = DC_MODE_XGA_60;
				break;
			case OUTPUT_RESOLUTION_1080P50: 	
				subResolution = DC_MODE_1080P_50;
				break;
			case OUTPUT_RESOLUTION_480P:	 
				subResolution = DC_MODE_480P;
				break;
			case OUTPUT_RESOLUTION_576P:	 
				subResolution = DC_MODE_576P;
				break;
			default:
				subResolution = DC_MODE_1080P_60;
				break;
		}

        int spotResolution;
        switch(profile.spotdisplayoutput.iResolution)
        {
            case OUTPUT_RESOLUTION_NTSC:
                spotResolution = DC_MODE_NTSC;
                break;
            case OUTPUT_RESOLUTION_PAL:
                spotResolution = DC_MODE_PAL;
                break;
            default:
                spotResolution = DC_MODE_NTSC;
                break;
        }

#if DVR_API_ENABLE
        LIB816x_setDisplayMainSub(profile.layout.iMainOutput, profile.layout.iSubOutput, DISP_PATH_SD, mainResolution, subResolution, spotResolution);
        pMainClass->SwitchFB(profile.layout.iMainOutput);
#endif
        if(dlgdata.layout.iMainOutput == DISP_PATH_HDMI1)
        {
            if(runtimesettings.iMainSwapEnable==0)
            {
                runtimesettings.iMainSwapEnable = 1;
            }
        }
        else
        {
            if(runtimesettings.iMainSwapEnable==1)
            {
                runtimesettings.iMainSwapEnable = 0;
            }
        }
    }

    if((dlgdata.maindisplayoutput.iResolution!=profile.maindisplayoutput.iResolution ||
        dlgdata.maindisplayoutput.iResolution!=runtimesettings.OutputResolution[profile.layout.iMainOutput])||
       /*(dlgdata.subdisplayoutput.iResolution!=profile.subdisplayoutput.iResolution || 
        dlgdata.subdisplayoutput.iResolution!=runtimesettings.OutputResolution[profile.layout.iSubOutput])||*/
       (dlgdata.spotdisplayoutput.iResolution!=profile.spotdisplayoutput.iResolution))
    {
            
        profile.maindisplayoutput.iResolution=dlgdata.maindisplayoutput.iResolution;
        runtimesettings.OutputResolution[profile.layout.iMainOutput]=dlgdata.maindisplayoutput.iResolution;
        
        profile.subdisplayoutput.iResolution=dlgdata.maindisplayoutput.iResolution;//dlgdata.subdisplayoutput.iResolution;
        runtimesettings.OutputResolution[profile.layout.iSubOutput] = dlgdata.maindisplayoutput.iResolution;//dlgdata.subdisplayoutput.iResolution;
        
        profile.spotdisplayoutput.iResolution=dlgdata.spotdisplayoutput.iResolution;

        int mainResolution;
        switch(profile.maindisplayoutput.iResolution)
        {
            case OUTPUT_RESOLUTION_1080P:     
                mainResolution = DC_MODE_1080P_60;
                break;
            case OUTPUT_RESOLUTION_720P:     
                mainResolution = DC_MODE_720P_60;
                break;
            case OUTPUT_RESOLUTION_SXGA:     
                mainResolution = DC_MODE_SXGA_60;
                break;
            case OUTPUT_RESOLUTION_XGA:     
                mainResolution = DC_MODE_XGA_60;
                break;
            case OUTPUT_RESOLUTION_1080P50:     
                mainResolution = DC_MODE_1080P_50;
                break;
            case OUTPUT_RESOLUTION_480P:     
                mainResolution = DC_MODE_480P;
                break;
            case OUTPUT_RESOLUTION_576P:     
                mainResolution = DC_MODE_576P;
                break;
            default:
                mainResolution = DC_MODE_1080P_60;
                break;
        }

        int subResolution;
        switch(profile.subdisplayoutput.iResolution)
        {
            case OUTPUT_RESOLUTION_1080P:     
                subResolution = DC_MODE_1080P_60;
                break;
            case OUTPUT_RESOLUTION_720P:     
                subResolution = DC_MODE_720P_60;
                break;
            case OUTPUT_RESOLUTION_SXGA:     
                subResolution = DC_MODE_SXGA_60;
                break;
            case OUTPUT_RESOLUTION_XGA:     
                subResolution = DC_MODE_XGA_60;
                break;
            case OUTPUT_RESOLUTION_1080P50:     
                subResolution = DC_MODE_1080P_50;
                break;
            case OUTPUT_RESOLUTION_480P:     
                subResolution = DC_MODE_480P;
                break;
            case OUTPUT_RESOLUTION_576P:     
                subResolution = DC_MODE_576P;
                break;
            default:
                subResolution = DC_MODE_1080P_60;
                break;
        }

        int spotResolution;
        switch(profile.spotdisplayoutput.iResolution)
        {
        case OUTPUT_RESOLUTION_NTSC:
            spotResolution = DC_MODE_NTSC;
            break;
        case OUTPUT_RESOLUTION_PAL:
            spotResolution = DC_MODE_PAL;
            break;
        default:
            spotResolution = DC_MODE_NTSC;
            break;
        }
#if DVR_API_ENABLE
        LIB816x_setDisplayMainSub(profile.layout.iMainOutput, profile.layout.iSubOutput, DISP_PATH_SD, mainResolution, subResolution, spotResolution);
#endif
    }
//printf("GetDisplaydata:profile.maindisplayoutput.iResolution=%d\n",profile.maindisplayoutput.iResolution);
//SPOT OUTPUT

    if(dlgdata.iSpotOutputCh!=profile.iSpotOutputCh)
    {
        profile.iSpotOutputCh=dlgdata.iSpotOutputCh;
#if	DVR_API_ENABLE
        LIB816x_setSpotChannel(profile.iSpotOutputCh);
#endif
    }
    if(profile.layout.iMainOutput==profile.layout.iSubOutput && profile.iSubOutputEnable==1)
    {
        profile.iSubOutputEnable=0;
        SetSubOutputEnable();
    }
    else if(profile.layout.iMainOutput!=profile.layout.iSubOutput && profile.iSubOutputEnable==0)
    {
        profile.iSubOutputEnable=1;
        SetSubOutputEnable();
    }
    if(dlgdata.layout.iSubOutput==0){
       // ui->suboutputselectEdit->setText(QString::fromUtf8("HDMI0/VGA"));
        ui->resolutionselectcomboBox->clear();
        ui->resolutionselectcomboBox->insertItems(0, QStringList()
                                              << QApplication::translate("dvr_setup", "1080P60", 0)
                                                       << QApplication::translate("dvr_setup", "720P60", 0)
                                                       << QApplication::translate("dvr_setup", "SXGA60", 0)
                                                       << QApplication::translate("dvr_setup", "XGA60", 0)
                                                       << QApplication::translate("dvr_setup", "1080P50", 0)
                                                      );
     /*   ui->resolutionselectcomboBox_2->clear();
        ui->resolutionselectcomboBox_2->insertItems(0, QStringList()
                                              << QApplication::translate("dvr_setup", "1080P60", 0, QApplication::UnicodeUTF8)
                                                       << QApplication::translate("dvr_setup", "720P60", 0, QApplication::UnicodeUTF8)
                                                       << QApplication::translate("dvr_setup", "SXGA60", 0, QApplication::UnicodeUTF8)
                                                       << QApplication::translate("dvr_setup", "XGA60", 0, QApplication::UnicodeUTF8)
                                                       << QApplication::translate("dvr_setup", "1080P50", 0, QApplication::UnicodeUTF8)
                                                       << QApplication::translate("dvr_setup", "480P", 0, QApplication::UnicodeUTF8)
                                                       << QApplication::translate("dvr_setup", "576P", 0, QApplication::UnicodeUTF8)
                                                      );*/
    }
    else{
      //  ui->suboutputselectEdit->setText(QString::fromUtf8("HDMI1"));
        ui->resolutionselectcomboBox->clear();
        ui->resolutionselectcomboBox->insertItems(0, QStringList()
                                              << QApplication::translate("dvr_setup", "1080P60", 0)
                                                       << QApplication::translate("dvr_setup", "720P60", 0)
                                                       << QApplication::translate("dvr_setup", "SXGA60", 0)
                                                       << QApplication::translate("dvr_setup", "XGA60", 0)
                                                       << QApplication::translate("dvr_setup", "1080P50", 0)
                                                       << QApplication::translate("dvr_setup", "480P", 0)
                                                       << QApplication::translate("dvr_setup", "576P", 0)
                                                      );
        /*ui->resolutionselectcomboBox_2->clear();
        ui->resolutionselectcomboBox_2->insertItems(0, QStringList()
                                              << QApplication::translate("dvr_setup", "1080P60", 0, QApplication::UnicodeUTF8)
                                                       << QApplication::translate("dvr_setup", "720P60", 0, QApplication::UnicodeUTF8)
                                                       << QApplication::translate("dvr_setup", "SXGA60", 0, QApplication::UnicodeUTF8)
                                                       << QApplication::translate("dvr_setup", "XGA60", 0, QApplication::UnicodeUTF8)
                                                       << QApplication::translate("dvr_setup", "1080P50", 0, QApplication::UnicodeUTF8)
                                                      );*/
    }
//SEQUENCE
    dlgdata.sequence.iInterval=ui->sequenceintervalslider->sliderPosition();
    dlgdata.secondsequence.iInterval=ui->subsequenceintervalslider->sliderPosition();
    if(dlgdata.sequence.iInterval!=profile.sequence.iInterval)
    {
        profile.sequence.iInterval=dlgdata.sequence.iInterval;
    }
    if(dlgdata.secondsequence.iInterval!=profile.secondsequence.iInterval)
    {
        profile.secondsequence.iInterval=dlgdata.secondsequence.iInterval;
    }
//OSD
    dlgdata.mainosd.iBorderLineOn=ui->borderlineselectcomboBox->currentIndex();
    if(dlgdata.mainosd.iBorderLineOn!=profile.mainosd.iBorderLineOn)
    {
        profile.mainosd.iBorderLineOn=dlgdata.mainosd.iBorderLineOn;
        pLayoutClass->SetBorderLineShowHide(profile.mainosd.iBorderLineOn);
    }
    dlgdata.mainosd.iDateFormat=ui->maindateformatcomboBox->currentIndex();
    if(dlgdata.mainosd.iDateFormat!=profile.mainosd.iDateFormat)
    {
       profile.mainosd.iDateFormat=dlgdata.mainosd.iDateFormat;
       pStatusbar->SetDateFormat(profile.mainosd.iDateFormat);
       SetDateFormat(profile.mainosd.iDateFormat);
    }
	pLayoutClass->SetLayoutMode(profile.layout.iLayoutMode);
}

/*!cancel시 display관련 변경적용된 내역을 복원시킴 */
void dvr_setup::ResotreDisplay()
{
    if(dlgdata.layout.iMainOutput!=profile.layout.iMainOutput ||
        dlgdata.layout.iSubOutput!=profile.layout.iSubOutput)
    {
        profile.layout.iMainOutput=dlgdata.layout.iMainOutput;
        profile.layout.iSubOutput=dlgdata.layout.iSubOutput;

		profile.maindisplayoutput.iResolution=dlgdata.maindisplayoutput.iResolution;
		runtimesettings.OutputResolution[profile.layout.iMainOutput]=dlgdata.maindisplayoutput.iResolution;

        profile.subdisplayoutput.iResolution=dlgdata.maindisplayoutput.iResolution;//dlgdata.subdisplayoutput.iResolution;
        runtimesettings.OutputResolution[profile.layout.iSubOutput] =dlgdata.maindisplayoutput.iResolution;// dlgdata.subdisplayoutput.iResolution;

        profile.spotdisplayoutput.iResolution=dlgdata.spotdisplayoutput.iResolution;

		int mainResolution;
		switch(profile.maindisplayoutput.iResolution)
		{
			case OUTPUT_RESOLUTION_1080P:	  
				mainResolution = DC_MODE_1080P_60;
				break;
			case OUTPUT_RESOLUTION_720P:	 
				mainResolution = DC_MODE_720P_60;
				break;
			case OUTPUT_RESOLUTION_SXGA:	 
				mainResolution = DC_MODE_SXGA_60;
				break;
			case OUTPUT_RESOLUTION_XGA: 	
				mainResolution = DC_MODE_XGA_60;
				break;
			case OUTPUT_RESOLUTION_1080P50: 	
				mainResolution = DC_MODE_1080P_50;
				break;
			case OUTPUT_RESOLUTION_480P:	 
				mainResolution = DC_MODE_480P;
				break;
			case OUTPUT_RESOLUTION_576P:	 
				mainResolution = DC_MODE_576P;
				break;
			default:
				mainResolution = DC_MODE_1080P_60;
				break;
		}

		int subResolution;
		switch(profile.maindisplayoutput.iResolution)
		{
			case OUTPUT_RESOLUTION_1080P:	  
				subResolution = DC_MODE_1080P_60;
				break;
			case OUTPUT_RESOLUTION_720P:	 
				subResolution = DC_MODE_720P_60;
				break;
			case OUTPUT_RESOLUTION_SXGA:	 
				subResolution = DC_MODE_SXGA_60;
				break;
			case OUTPUT_RESOLUTION_XGA: 	
				subResolution = DC_MODE_XGA_60;
				break;
			case OUTPUT_RESOLUTION_1080P50: 	
				subResolution = DC_MODE_1080P_50;
				break;
			case OUTPUT_RESOLUTION_480P:	 
				subResolution = DC_MODE_480P;
				break;
			case OUTPUT_RESOLUTION_576P:	 
				subResolution = DC_MODE_576P;
				break;
			default:
				subResolution = DC_MODE_1080P_60;
				break;
		}

        int spotResolution;
        switch(profile.spotdisplayoutput.iResolution)
        {
            case OUTPUT_RESOLUTION_NTSC:
                spotResolution = DC_MODE_NTSC;
                break;
            case OUTPUT_RESOLUTION_PAL:
                spotResolution = DC_MODE_PAL;
                break;
            default:
                spotResolution = DC_MODE_NTSC;
                break;
        }

#if DVR_API_ENABLE
        LIB816x_setDisplayMainSub(profile.layout.iMainOutput, profile.layout.iSubOutput, DISP_PATH_SD, mainResolution, subResolution, spotResolution);
        pMainClass->SwitchFB(profile.layout.iMainOutput);
#endif
        if(dlgdata.layout.iMainOutput == DISP_PATH_HDMI1)
        {
            if(runtimesettings.iMainSwapEnable==0)
            {
                runtimesettings.iMainSwapEnable = 1;
            }
        }
        else
        {
            if(runtimesettings.iMainSwapEnable==1)
            {
                runtimesettings.iMainSwapEnable = 0;
            }
        }
    }

    if((dlgdata.maindisplayoutput.iResolution!=profile.maindisplayoutput.iResolution ||
        dlgdata.maindisplayoutput.iResolution!=runtimesettings.OutputResolution[profile.layout.iMainOutput])||
      /* (dlgdata.subdisplayoutput.iResolution!=profile.subdisplayoutput.iResolution || 
        dlgdata.subdisplayoutput.iResolution!=runtimesettings.OutputResolution[profile.layout.iSubOutput])||*/
       (dlgdata.spotdisplayoutput.iResolution!=profile.spotdisplayoutput.iResolution))
    {
            
        profile.maindisplayoutput.iResolution=dlgdata.maindisplayoutput.iResolution;
        runtimesettings.OutputResolution[profile.layout.iMainOutput]=dlgdata.maindisplayoutput.iResolution;
        
        profile.subdisplayoutput.iResolution=dlgdata.maindisplayoutput.iResolution;//dlgdata.subdisplayoutput.iResolution;
        runtimesettings.OutputResolution[profile.layout.iSubOutput] = dlgdata.maindisplayoutput.iResolution;//dlgdata.subdisplayoutput.iResolution;
        
        profile.spotdisplayoutput.iResolution=dlgdata.spotdisplayoutput.iResolution;

        int mainResolution;
        switch(profile.maindisplayoutput.iResolution)
        {
            case OUTPUT_RESOLUTION_1080P:     
                mainResolution = DC_MODE_1080P_60;
                break;
            case OUTPUT_RESOLUTION_720P:     
                mainResolution = DC_MODE_720P_60;
                break;
            case OUTPUT_RESOLUTION_SXGA:     
                mainResolution = DC_MODE_SXGA_60;
                break;
            case OUTPUT_RESOLUTION_XGA:     
                mainResolution = DC_MODE_XGA_60;
                break;
            case OUTPUT_RESOLUTION_1080P50:     
                mainResolution = DC_MODE_1080P_50;
                break;
            case OUTPUT_RESOLUTION_480P:     
                mainResolution = DC_MODE_480P;
                break;
            case OUTPUT_RESOLUTION_576P:     
                mainResolution = DC_MODE_576P;
                break;
            default:
                mainResolution = DC_MODE_1080P_60;
                break;
        }

        int subResolution;
        switch(profile.subdisplayoutput.iResolution)
        {
            case OUTPUT_RESOLUTION_1080P:     
                subResolution = DC_MODE_1080P_60;
                break;
            case OUTPUT_RESOLUTION_720P:     
                subResolution = DC_MODE_720P_60;
                break;
            case OUTPUT_RESOLUTION_SXGA:     
                subResolution = DC_MODE_SXGA_60;
                break;
            case OUTPUT_RESOLUTION_XGA:     
                subResolution = DC_MODE_XGA_60;
                break;
            case OUTPUT_RESOLUTION_1080P50:     
                subResolution = DC_MODE_1080P_50;
                break;
            case OUTPUT_RESOLUTION_480P:     
                subResolution = DC_MODE_480P;
                break;
            case OUTPUT_RESOLUTION_576P:     
                subResolution = DC_MODE_576P;
                break;
            default:
                subResolution = DC_MODE_1080P_60;
                break;
        }

        int spotResolution;
        switch(profile.spotdisplayoutput.iResolution)
        {
        case OUTPUT_RESOLUTION_NTSC:
            spotResolution = DC_MODE_NTSC;
            break;
        case OUTPUT_RESOLUTION_PAL:
            spotResolution = DC_MODE_PAL;
            break;
        default:
            spotResolution = DC_MODE_NTSC;
            break;
        }
#if DVR_API_ENABLE
        LIB816x_setDisplayMainSub(profile.layout.iMainOutput, profile.layout.iSubOutput, DISP_PATH_SD, mainResolution, subResolution, spotResolution);
#endif
    }

//SPOT OUTPUT
    if(dlgdata.iSpotOutputCh!=profile.iSpotOutputCh)
    {
        profile.iSpotOutputCh=dlgdata.iSpotOutputCh;
#if	DVR_API_ENABLE
        LIB816x_setSpotChannel(profile.iSpotOutputCh);
#endif
    }
    if(profile.layout.iMainOutput==profile.layout.iSubOutput && profile.iSubOutputEnable==1)
    {
        profile.iSubOutputEnable=0;
        SetSubOutputEnable();
    }
    else if(profile.layout.iMainOutput!=profile.layout.iSubOutput && profile.iSubOutputEnable==0)
    {
        profile.iSubOutputEnable=1;
        SetSubOutputEnable();
    }
//SEQUENCE
    if(dlgdata.sequence.iInterval!=profile.sequence.iInterval)
    {
        profile.sequence.iInterval=dlgdata.sequence.iInterval;
    }
    if(dlgdata.secondsequence.iInterval!=profile.secondsequence.iInterval)
    {
        profile.secondsequence.iInterval=dlgdata.secondsequence.iInterval;
    }
//OSD
    dlgdata.mainosd.iBorderLineOn=ui->borderlineselectcomboBox->currentIndex();
    if(dlgdata.mainosd.iBorderLineOn!=profile.mainosd.iBorderLineOn)
    {
        profile.mainosd.iBorderLineOn=dlgdata.mainosd.iBorderLineOn;
        pLayoutClass->SetBorderLineShowHide(profile.mainosd.iBorderLineOn);
    }
    if(dlgdata.mainosd.iDateFormat!=profile.mainosd.iDateFormat)
    {
       profile.mainosd.iDateFormat=dlgdata.mainosd.iDateFormat;
       pStatusbar->SetDateFormat(profile.mainosd.iDateFormat);
       SetDateFormat(profile.mainosd.iDateFormat);
    }
}
/*!sub output enable/disable에 따른 처리수행(one monitor mode) */
void dvr_setup::SetSubOutputEnable()
{
    if(profile.iSubOutputEnable==1)
    {
        pToolbar->SetEnableSubToolbar(1);
        if(runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
        {
            runtimesettings.iPageNum=0;
#if DVR_API_ENABLE
            LIB816x_setCameraLayout(profile.layout.iMainOutput, 0, runtimesettings.iLayout);
#endif
            pLayoutClass->DrawOSDChannelPlayback();
        }
        else
        {
#if DVR_API_ENABLE
            LIB816x_setCameraLayout(profile.layout.iMainOutput, 0, runtimesettings.iLayout);
#endif
            pLayoutClass->DrawOSDChannelLive();
        }
    }
    else
    {
        pToolbar->SetEnableSubToolbar(0);
        if(runtimesettings.iLivePlaybackMode==LIB_PLAYBACK_MODE)
        {
            runtimesettings.iPageNum=0;
#if DVR_API_ENABLE
            LIB816x_setCameraLayout(profile.layout.iMainOutput, 16, runtimesettings.iLayout);
#endif
            pLayoutClass->DrawOSDChannelPlayback();
        }
        else
        {
#if DVR_API_ENABLE
            LIB816x_setCameraLayout(profile.layout.iMainOutput, 0, runtimesettings.iLayout);
#endif
            pLayoutClass->DrawOSDChannelLive();
        }
    }
}
/*!sequence interval 값을 text로 표시 */
QString dvr_setup::SetSequenceIntervalValueText(int iInterval)
{
    QString qstrinterval;
    char intervalbuf[MAX_CHAR_8];
    sprintf(intervalbuf,"%d %s",iInterval,STRSECOND);
    qstrinterval=StrToQString(intervalbuf);

    return qstrinterval;
}

/*!one monitor check box가 변경시 호출 */
/*void dvr_setup::on_onemonitorcheckBox_stateChanged(int )
{
    if(ui->onemonitorcheckBox->checkState())
    {
        m_bOneMonitorDisplay=true;
        ui->outputselectcomboBox->setEnabled(false);
        ui->outputselectcomboBox->setCurrentIndex(0);
        //ui->suboutputselectEdit->setText(QString::fromUtf8("HDMI0/VGA"));
	ui->suboutputselectEdit->hide();
		
        ui->resolutionselectlabel_2->hide();
        ui->resolutionselectcomboBox_2->hide();
    }
    else
    {
        m_bOneMonitorDisplay=false;
        ui->outputselectcomboBox->setEnabled(true);
        ui->outputselectcomboBox->setCurrentIndex(0);
        ui->suboutputselectEdit->setText(QString::fromUtf8("HDMI1"));

        ui->resolutionselectlabel_2->show();
        ui->resolutionselectcomboBox_2->show();
    }
}*/
/*!Main display output combo box가 변경시 호출 */
void dvr_setup::ChangeOutputSelectSlot(int index)
{
   /* if(m_bOneMonitorDisplay==false)
    {
        if(index == 0)
            ui->suboutputselectEdit->setText(QString::fromUtf8("HDMI1"));
        else
            ui->suboutputselectEdit->setText(QString::fromUtf8("HDMI0/VGA"));
    }*/
}
/*!main display sequence interval slide가 변경시 호출 */
void dvr_setup::ChangeSequenceIntevalSlot(int value)
{
    ui->sequenceintervalvalue->setText(SetSequenceIntervalValueText(value));
}
/*!sub display sequence interval slide가 변경시 호출 */
void dvr_setup::ChangeSubSequenceIntevalSlot(int value)
{
    ui->subsequenceintervalvalue->setText(SetSequenceIntervalValueText(value));
}
