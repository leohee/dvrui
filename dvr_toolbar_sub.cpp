/******************************************************************************
* DM8168 Hybrid DVR
* Copyright by UDWorks, Incoporated. All Rights Reserved.
*-----------------------------------------------------------------------------
* @file	dvr_toolbar_sub.cpp
* @brief
* @author
* @section	MODIFY history
*     - 2011.01.01 : First Created
******************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include "dvr_defines.h"
#include "dvr_toolbar_sub.h"
#include "ui_dvr_toolbar_sub.h"
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
dvr_toolbar_sub::dvr_toolbar_sub(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dvr_toolbar_sub)
{
    ui->setupUi(this);
    PageCount=0;
    connect(ui->prevpage, SIGNAL(clicked()), this, SLOT(PrevPageBtnSlot()));
    connect(ui->nextpage, SIGNAL(clicked()), this, SLOT(NextPageBtnSlot()));

    connect(ui->layoutmode1, SIGNAL(clicked()), this, SLOT(Layout1BtnSlot()));
    connect(ui->layoutmode4, SIGNAL(clicked()), this, SLOT(Layout4BtnSlot()));
    connect(ui->layoutmode9, SIGNAL(clicked()), this, SLOT(Layout9BtnSlot()));
    connect(ui->layoutmode16, SIGNAL(clicked()), this, SLOT(Layout16BtnSlot()));
    connect(ui->layoutmode8, SIGNAL(clicked()), this, SLOT(Layout8BtnSlot()));
    connect(ui->layoutmode8_1, SIGNAL(clicked()), this, SLOT(Layout8_1BtnSlot()));
    connect(ui->layoutmode12, SIGNAL(clicked()), this, SLOT(Layout12BtnSlot()));
    connect(ui->layoutmode12_1, SIGNAL(clicked()), this, SLOT(Layout12_1BtnSlot()));
    connect(ui->layoutmode14, SIGNAL(clicked()), this, SLOT(Layout14BtnSlot()));
    connect(ui->sequencemode, SIGNAL(clicked()), this, SLOT(SequenceModeBtnSlot()));
    ui->layoutmode24->hide();
}
/*!destructor */
dvr_toolbar_sub::~dvr_toolbar_sub()
{
    delete ui;
}

void dvr_toolbar_sub::changeEvent(QEvent *e)
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

/*!sub toolbar 좌표 설정 */
void dvr_toolbar_sub::SetMainWindowSize()
{
    setGeometry(coordinator.SecondToolbar);
}
/*!previous button click시 호출 */
void dvr_toolbar_sub::PrevPageBtnSlot()
{
	//change previou page
    if(runtimesettings.iSecondPageNum>0)
        runtimesettings.iSecondPageNum--;
    else
        runtimesettings.iSecondPageNum=PageCount;

#if DVR_API_ENABLE
    LIB816x_setCameraLayout(profile.layout.iSubOutput, 16+runtimesettings.iSecondLayout*runtimesettings.iSecondPageNum, runtimesettings.iSecondLayout);
#endif
    dprintf("PrevPageBtnSlot()\n");
}
/*!next button click시 호출 */
void dvr_toolbar_sub::NextPageBtnSlot()
{
	//change next page
    if(runtimesettings.iSecondPageNum<PageCount)
        runtimesettings.iSecondPageNum++;
    else
        runtimesettings.iSecondPageNum=0;

#if DVR_API_ENABLE
    LIB816x_setCameraLayout(profile.layout.iSubOutput, 16+runtimesettings.iSecondLayout*runtimesettings.iSecondPageNum, runtimesettings.iSecondLayout);
#endif   
    dprintf("NextPageBtnSlot()\n");
}
/*!현재 layout mode에 대한 max page 계산 */
void dvr_toolbar_sub::MaxPageCalc()
{
	//find max page number by current layout mode
    if((MAX_CAMERA%runtimesettings.iSecondLayout)==0)
        PageCount=MAX_CAMERA/runtimesettings.iSecondLayout-1;
    else
        PageCount=MAX_CAMERA/runtimesettings.iSecondLayout;
}
/*!LAYOUTMODE_1 button click시 호출 */
void dvr_toolbar_sub::Layout1BtnSlot()
{
	//change layout mode to LAYOUTMODE_1
    profile.layout.iSecondLayoutMode=LAYOUTMODE_1;
    runtimesettings.iSecondLayout=LAYOUT_1;
    runtimesettings.iSecondPageNum=0;
   
    MaxPageCalc();
    SetLayoutModeBtnSelect(LAYOUTMODE_1);
#if DVR_API_ENABLE
    LIB816x_setPlaybackDisplayLayout(LAYOUTMODE_1, 0);
    LIB816x_setCameraLayout(profile.layout.iSubOutput, 16+runtimesettings.iSecondLayout*runtimesettings.iSecondPageNum, runtimesettings.iSecondLayout);
#endif

   dprintf("LayoutBtnSlot1()\n");
   WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));
}
/*!LAYOUTMODE_4 button click시 호출 */
void dvr_toolbar_sub::Layout4BtnSlot()
{
	//change layout mode to LAYOUTMODE_4
    profile.layout.iSecondLayoutMode=LAYOUTMODE_4;
    runtimesettings.iSecondLayout=LAYOUT_4;
    runtimesettings.iSecondPageNum=0;
    
    MaxPageCalc();
    SetLayoutModeBtnSelect(LAYOUTMODE_4);
#if DVR_API_ENABLE
    LIB816x_setPlaybackDisplayLayout(LAYOUTMODE_4, 0);
    LIB816x_setCameraLayout(profile.layout.iSubOutput, 16+runtimesettings.iSecondLayout*runtimesettings.iSecondPageNum, runtimesettings.iSecondLayout);
#endif

   dprintf("LayoutBtnSlot4()\n");
   WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));
}
/*!LAYOUTMODE_9 button click시 호출 */
void dvr_toolbar_sub::Layout9BtnSlot()
{
	//change layout mode to LAYOUTMODE_9
    profile.layout.iSecondLayoutMode=LAYOUTMODE_9;
    runtimesettings.iSecondLayout=LAYOUT_9;
    runtimesettings.iSecondPageNum=0;
    
    MaxPageCalc();
    SetLayoutModeBtnSelect(LAYOUTMODE_9);
#if DVR_API_ENABLE
    LIB816x_setPlaybackDisplayLayout(LAYOUTMODE_9, 0);
    LIB816x_setCameraLayout(profile.layout.iSubOutput, 16+runtimesettings.iSecondLayout*runtimesettings.iSecondPageNum, runtimesettings.iSecondLayout);
#endif

   dprintf("LayoutBtnSlot9()\n");
   WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));
}
/*!LAYOUTMODE_16 button click시 호출 */
void dvr_toolbar_sub::Layout16BtnSlot()
{
	//change layout mode to LAYOUTMODE_16
    profile.layout.iSecondLayoutMode=LAYOUTMODE_16;
    runtimesettings.iSecondLayout=LAYOUT_16;
    runtimesettings.iSecondPageNum=0;
    
    MaxPageCalc();
    SetLayoutModeBtnSelect(LAYOUTMODE_16);
#if DVR_API_ENABLE
    LIB816x_setPlaybackDisplayLayout(LAYOUTMODE_16, 0);
#endif

   dprintf("LayoutBtnSlot16()\n");
   WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));
}
/*!LAYOUTMODE_8 button click시 호출 */
void dvr_toolbar_sub::Layout8BtnSlot()
{
	//change layout mode to LAYOUTMODE_8
    profile.layout.iSecondLayoutMode=LAYOUTMODE_8;
    runtimesettings.iSecondLayout=LAYOUT_8;
    runtimesettings.iSecondPageNum=0;
    
    MaxPageCalc();
    SetLayoutModeBtnSelect(LAYOUTMODE_8);
#if DVR_API_ENABLE
    LIB816x_setPlaybackDisplayLayout(LAYOUTMODE_8, 0);
#endif

   dprintf("LayoutBtnSlot8()\n");
   WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));
}
/*!LAYOUTMODE_8_1 button click시 호출 */
void dvr_toolbar_sub::Layout8_1BtnSlot()
{
	//change layout mode to LAYOUTMODE_8_1
    profile.layout.iSecondLayoutMode=LAYOUTMODE_8_1;
    runtimesettings.iSecondLayout=LAYOUT_8;
    runtimesettings.iSecondPageNum=0;
    
    MaxPageCalc();
    SetLayoutModeBtnSelect(LAYOUTMODE_8_1);
#if DVR_API_ENABLE
    LIB816x_setPlaybackDisplayLayout(LAYOUTMODE_8_1, 0);
#endif

   dprintf("LayoutBtnSlot8_1()\n");
   WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));
}
/*!LAYOUTMODE_12 button click시 호출 */
void dvr_toolbar_sub::Layout12BtnSlot()
{
	//change layout mode to LAYOUTMODE_12
    profile.layout.iSecondLayoutMode=LAYOUTMODE_12;
    runtimesettings.iSecondLayout=LAYOUT_12;
    runtimesettings.iSecondPageNum=0;
    
    MaxPageCalc();
    SetLayoutModeBtnSelect(LAYOUTMODE_12);
#if DVR_API_ENABLE
    LIB816x_setPlaybackDisplayLayout(LAYOUTMODE_12, 0);
#endif

   dprintf("LayoutBtnSlot12()\n");
   WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));
}
/*!LAYOUTMODE_12_1 button click시 호출 */
void dvr_toolbar_sub::Layout12_1BtnSlot()
{
	//change layout mode to LAYOUTMODE_12_1
    profile.layout.iSecondLayoutMode=LAYOUTMODE_12_1;
    runtimesettings.iSecondLayout=LAYOUT_12;
    runtimesettings.iSecondPageNum=0;
    
    MaxPageCalc();
    SetLayoutModeBtnSelect(LAYOUTMODE_12_1);
#if DVR_API_ENABLE
    LIB816x_setPlaybackDisplayLayout(LAYOUTMODE_12_1, 0);
#endif

   dprintf("LayoutBtnSlot12_1()\n");
   WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));
}
/*!LAYOUTMODE_14 button click시 호출 */
void dvr_toolbar_sub::Layout14BtnSlot()
{
	//change layout mode to LAYOUTMODE_14
    profile.layout.iSecondLayoutMode=LAYOUTMODE_14;
    runtimesettings.iSecondLayout=LAYOUT_14;
    runtimesettings.iSecondPageNum=0;
    
    MaxPageCalc();
    SetLayoutModeBtnSelect(LAYOUTMODE_14);
#if DVR_API_ENABLE
    LIB816x_setPlaybackDisplayLayout(LAYOUTMODE_14, 0);
#endif

   dprintf("LayoutBtnSlot14()\n");
   WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));
}
/*!sequence mode button click시 호출 */
void dvr_toolbar_sub::SequenceModeBtnSlot()
{
	//toggle sequence mode on/off
    if(profile.secondsequence.iSequenceModeOn==0)
    {
        profile.secondsequence.iSequenceModeOn=1;
        pMainClass->SublayoutTimerStart();
    }
    else
    {
        profile.secondsequence.iSequenceModeOn=0;
        pMainClass->SublayoutTimerStop();
    }
    WriteDVRsettingInfo((char*)PROFILEFILNAME,&profile,sizeof(PROFILE));

    dprintf("SequenceModeBtnSlot()\n");
}

/*!layout mode에 따라서 현재 layout 해당 button은 disable */
void dvr_toolbar_sub::SetLayoutModeBtnSelect(int iMode)
{
	//disable current layout button and enable other layout button
    static int currentSubMode=100;
    switch(currentSubMode)
    {
    case LAYOUTMODE_1:
        ui->layoutmode1->setEnabled(true);
    break;
    case LAYOUTMODE_4:
        ui->layoutmode4->setEnabled(true);
    break;
    case LAYOUTMODE_9:
        ui->layoutmode9->setEnabled(true);
    break;
    case LAYOUTMODE_16:
        ui->layoutmode16->setEnabled(true);
    break;
    case LAYOUTMODE_8:
        ui->layoutmode8->setEnabled(true);
    break;
    case LAYOUTMODE_8_1:
        ui->layoutmode8_1->setEnabled(true);
    break;
    case LAYOUTMODE_12:
        ui->layoutmode12->setEnabled(true);
    break;
    case LAYOUTMODE_12_1:
        ui->layoutmode12_1->setEnabled(true);
    break;
    case LAYOUTMODE_14:
        ui->layoutmode14->setEnabled(true);
    break;
    default:
        ui->layoutmode1->setEnabled(true);
        ui->layoutmode4->setEnabled(true);
        ui->layoutmode9->setEnabled(true);
        ui->layoutmode16->setEnabled(true);
        ui->layoutmode8->setEnabled(true);
        ui->layoutmode8_1->setEnabled(true);
        ui->layoutmode12->setEnabled(true);
        ui->layoutmode12_1->setEnabled(true);
        ui->layoutmode14->setEnabled(true);
    break;
    }

    currentSubMode = iMode;

    switch(iMode)
    {
    case LAYOUTMODE_1:
        ui->layoutmode1->setEnabled(false);
    break;
    case LAYOUTMODE_4:
        ui->layoutmode4->setEnabled(false);
    break;
    case LAYOUTMODE_9:
        ui->layoutmode9->setEnabled(false);
    break;
    case LAYOUTMODE_16:
        ui->layoutmode16->setEnabled(false);
    break;
    case LAYOUTMODE_8:
        ui->layoutmode8->setEnabled(false);
    break;
    case LAYOUTMODE_8_1:
        ui->layoutmode8_1->setEnabled(false);
    break;
    case LAYOUTMODE_12:
        ui->layoutmode12->setEnabled(false);
    break;
    case LAYOUTMODE_12_1:
        ui->layoutmode12_1->setEnabled(false);
    break;
    case LAYOUTMODE_14:
        ui->layoutmode14->setEnabled(false);
    break;
    default:
        ui->layoutmode1->setEnabled(true);
        ui->layoutmode4->setEnabled(true);
        ui->layoutmode9->setEnabled(true);
        ui->layoutmode16->setEnabled(true);
        ui->layoutmode8->setEnabled(true);
        ui->layoutmode8_1->setEnabled(true);
        ui->layoutmode12->setEnabled(true);
        ui->layoutmode12_1->setEnabled(true);
        ui->layoutmode14->setEnabled(true);
    break;
    }
}
