/******************************************************************************
* DM8168 Hybrid DVR
* Copyright by UDWorks, Incoporated. All Rights Reserved.
*-----------------------------------------------------------------------------
* @file	dvr_message.cpp
* @brief
* @author
* @section	MODIFY history
*     - 2011.01.01 : First Created
******************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include "dvr_defines.h"
#include "dvr_message.h"
#include "ui_dvr_message.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 local functions
-----------------------------------------------------------------------------*/
/*!constructor*/
WarningMessage::WarningMessage(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::WarningMessage)
{
    ui->setupUi(this);
}
/*!destructor */
WarningMessage::~WarningMessage()
{
    delete ui;
}

void WarningMessage::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
/*!warning message에 해당하는 text로 update */
void WarningMessage::SetText(const char *instr)
{
    ui->warn_label->setText(QString::fromUtf8(instr));
}
