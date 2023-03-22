/******************************************************************************
* DM8168 Hybrid DVR
* Copyright by UDWorks, Incoporated. All Rights Reserved.
*-----------------------------------------------------------------------------
* @file	dvr_login.cpp
* @brief
* @author
* @section	MODIFY history
*     - 2011.01.01 : First Created
******************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include "dvr_defines.h"
#include "dvr_login.h"
#include "ui_dvr_login.h"
#include "dvr_main.h"
/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define LOGINWIN_W	330
#define LOGINWIN_H	190

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
extern dvrmain *pMainClass;
extern dvr_sidebar *pSidebar;
extern dvr_timelinebar_play *pTimebar;
extern dvr_toolbar_main *pToolbar;
extern dvr_toolbar_sub *pSubToolbar;

int *pEnableUserIndex;
/*----------------------------------------------------------------------------
 local functions
-----------------------------------------------------------------------------*/
/*!constructor*/
dvr_login::dvr_login(QWidget *parent) :
	QDialog(parent, Qt::FramelessWindowHint),
    ui(new Ui::dvr_login)
{
    ui->setupUi(this);
    connect(ui->loginpushButton, SIGNAL(clicked()), this, SLOT(LoginBtnSlot()));
    connect(ui->virtualkeypushButton, SIGNAL(clicked()), this, SLOT(VirtualKeyBtnSlot()));
}
/*!destructor */
dvr_login::~dvr_login()
{
    delete ui;
}

void dvr_login::changeEvent(QEvent *e)
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
/*!Login dialog를 보여주고 Enable된 user에대해 login할 수 있도록 한다 */
void dvr_login::DvrLoginShow()
{
    move((coordinator.MainWindow.width()-LOGINWIN_W)/2,(coordinator.MainWindow.height()-LOGINWIN_H)/2);
    ui->accesslevelcomboBox->clear();
    ui->passwordlineEdit->setText("");
    pEnableUserIndex = new int[userlist.usercount];
    //Enable 되어 있는 user를 combo box에 추가
    for(int i=0;i<userlist.usercount;i++)
    {
        if(userlist.userlist[i].iEnable==1)
        {
            pEnableUserIndex[ui->accesslevelcomboBox->count()]=i;
            if(i==0)
                ui->accesslevelcomboBox->addItem("ADMIN");
            else
            {
                char id[10];
                sprintf(id,"USER%d",ui->accesslevelcomboBox->count());
                ui->accesslevelcomboBox->addItem(id);
            }
        }
    }
    show();
    ui->loginfaillabel->setText(StrToQString((char*)""));

}
/*!login이 된경우 login dialog를 지운다 */
void dvr_login::DvrLoginHide()
{
    hide();
    delete pEnableUserIndex;
}
/*!login button을 누르면 password check를 하고 user access level에 따라 control들을 활성화 한다 */
void dvr_login::LoginBtnSlot()
{
    int iaccesslevel=0;
    char curpw[MAX_PASSWORD_CHAR];
    memset(curpw,0,sizeof(curpw));

    int enableuserindex = ui->accesslevelcomboBox->currentIndex();

    if(enableuserindex==0)
        iaccesslevel=0;
    else
        iaccesslevel=1;

    strcpy(curpw,QStringToStr(ui->passwordlineEdit->text()));

    if(userlist.userlist[pEnableUserIndex[enableuserindex]].iAccessLevel==iaccesslevel)
    {
    	//check passwd
        if(strcmp(userlist.userlist[pEnableUserIndex[enableuserindex]].password,curpw)==0)
        {
            memcpy(&current_user,&userlist.userlist[pEnableUserIndex[enableuserindex]],sizeof(USER));
            ui->loginfaillabel->setText(StrToQString((char*)LOGIN_SUCCESS));
            DvrLoginHide();
            if(current_user.iAccessLevel==USERLEVEL_ADMIN)
            {
            	//ADMIN의 경우 모든 권한이 있으므로 모든 control enable
                pToolbar->setEnabled(true);
                pSidebar->setEnabled(true);
                pSubToolbar->setEnabled(true);
                pTimebar->setEnabled(true);
                pToolbar->SetEnableSetupBtn(1);
                pToolbar->SetEnableAllBtn(1);
                pToolbar->SetLayoutModeBtnSelect(profile.layout.iLayoutMode);
                pSubToolbar->SetLayoutModeBtnSelect(profile.layout.iSecondLayoutMode);
            }
            else if(current_user.iAccessLevel==USERLEVEL_USER)
            {
            	//USER의 경우 설정된 권한에 따라 control enable
                if(current_user.iAccessContents & USERACCESS_LIVEMENU)
                {
                    pSidebar->setEnabled(true);
                }
                if(current_user.iAccessContents & USERACCESS_PLAYBACK)
                {
                    pSidebar->setEnabled(true);
                    pTimebar->setEnabled(true);
                }
                if(current_user.iAccessContents & USERACCESS_MAINMENU)
                {
                    pToolbar->setEnabled(true);
                    pSubToolbar->setEnabled(true);
                    pToolbar->SetEnableAllBtn(1);
                    pToolbar->SetLayoutModeBtnSelect(profile.layout.iLayoutMode);
                    pSubToolbar->SetLayoutModeBtnSelect(profile.layout.iSecondLayoutMode);
                }
                else
                {
                    pToolbar->setEnabled(true);
                    pToolbar->SetEnableAllBtn(0);
                }
                if(current_user.iAccessContents & USERACCESS_SETUPMENU)
                {
                    pToolbar->SetEnableSetupBtn(1);
                }
                else
                {
                    pToolbar->SetEnableSetupBtn(0);
                }

            }
            pMainClass->LoginStop();
            if(iaccesslevel == USERLEVEL_ADMIN)
            {
                pMainClass->EventLogAdd((char*)"ADMIN Log-in",true);
            }
            else
            {
                char str[100];
                sprintf(str,"USER%d Log-in",pEnableUserIndex[enableuserindex]);
                pMainClass->EventLogAdd(str,true);
            }
            return;
        }
    }

    ui->loginfaillabel->setText(StrToQString((char*)LOGIN_FAILED));
}
/*!front key handler */
void dvr_login::keyPressEvent ( QKeyEvent * event )
{
    char str[MAX_PASSWORD_CHAR];
    int input=event->key();

    if(input == Qt::Key_Left)
    {
        char tmp[MAX_PASSWORD_CHAR];
        sprintf(tmp,"%s",QStringToStr(ui->passwordlineEdit->text()));
        if(strlen(tmp)!=0)
            tmp[strlen(tmp)-1]='\0';
        sprintf(str,"%s",tmp);
        ui->passwordlineEdit->setText(str);
    }
    else if((input>=Qt::Key_0 && input<=Qt::Key_9)||(input>=Qt::Key_A && input<=Qt::Key_Z))
    {
        sprintf(str,"%s%c",QStringToStr(ui->passwordlineEdit->text()),(char)input);
        ui->passwordlineEdit->setText(str);
    }
    else if(input == Qt::Key_Return)
    {
        LoginBtnSlot();
    }
    else if(input == Qt::Key_Escape)
    {
        pMainClass->keyPressEvent ( event );
    }

}
/*!virtual keyboard를 보여주고 입력을 받을 준비 */
void dvr_login::VirtualKeyBtnSlot()
{
    if(pMainClass->ShowVirtualKeyboard())
        ui->passwordlineEdit->setFocus();
    dprintf("Virtual Keyboard Open Btn\n");
}
