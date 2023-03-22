/*
 * Copyright 2009 EMBITEL (http://www.embitel.com)
 * 
 * This file is part of Virtual Keyboard Project.
 * 
 * Virtual Keyboard is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation
 * 
 * Virtual Keyboard is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Virtual Keyboard. If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include "WidgetKeyboard.h"
#include "coordinator.h"
#include "dvr_util.h"
#include "dvr_defines.h"
#include <QKeyEvent>
#include <QSignalMapper>
//#include <QSound>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QClipboard>
#include <QList>
/*!constructor*/
WidgetKeyboard::WidgetKeyboard(QWidget *parent) : QWidget(0)
{
    setupUi(this);
    this->setWindowFlags(Qt::Popup);
    m_pParent = parent;
    isCaps = false;
    isShift = false;
    isCtrl = false;
    isAlt = false;
    changeTextCaps(false);
    signalMapper = new QSignalMapper(this);
    allButtons = findChildren<QToolButton *>();
    for (int i=0;i<allButtons.count();i++)
    {
        connect(allButtons.at(i), SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(allButtons.at(i), i);
    }
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(on_btn_clicked(int)));
}
/*!destructor */
WidgetKeyboard::~WidgetKeyboard()
{
    delete signalMapper;
}

#ifdef QT_NO_ACCESSIBILITY
QString WidgetKeyboard::mapkeyAccessibleName(QToolButton *btn)
{
    QString retText;
    char str[15];
    if(btn == this->btnTilt)
    {
        sprintf(str,"0x60");
        retText=StrToQString(str);
    }
    else if(btn == this->btn1)
    {
        sprintf(str,"0x31");
        retText=StrToQString(str);
    }
    else if(btn == this->btn2)
    {
        sprintf(str,"0x32");
        retText=StrToQString(str);
    }
    else if(btn == this->btn3)
    {
        sprintf(str,"0x33");
        retText=StrToQString(str);
    }
    else if(btn == this->btn4)
    {
        sprintf(str,"0x34");
        retText=StrToQString(str);
    }
    else if(btn == this->btn5)
    {
        sprintf(str,"0x35");
        retText=StrToQString(str);
    }
    else if(btn == this->btn6)
    {
        sprintf(str,"0x36");
        retText=StrToQString(str);
    }
    else if(btn == this->btn7)
    {
        sprintf(str,"0x37");
        retText=StrToQString(str);
    }
    else if(btn == this->btn8)
    {
        sprintf(str,"0x38");
        retText=StrToQString(str);
    }
    else if(btn == this->btn9)
    {
        sprintf(str,"0x39");
        retText=StrToQString(str);
    }
    else if(btn == this->btn0)
    {
        sprintf(str,"0x30");
        retText=StrToQString(str);
    }
    else if(btn == this->btnHiphen)
    {
        sprintf(str,"0x2d");
        retText=StrToQString(str);
    }
    else if(btn == this->btnAssign)
    {
        sprintf(str,"13");
        retText=StrToQString(str);
    }
    else if(btn == this->btnBackSpace)
    {
        sprintf(str,"0x01000003");
        retText=StrToQString(str);
    }
    else if(btn == this->btnCaps)
    {
        sprintf(str,"0x01000024");
        retText=StrToQString(str);
    }
    else if(btn == this->btnA)
    {
        sprintf(str,"0x41");
        retText=StrToQString(str);
    }
    else if(btn == this->btnS)
    {
        sprintf(str,"0x53");
        retText=StrToQString(str);
    }
    else if(btn == this->btnD)
    {
        sprintf(str,"0x44");
        retText=StrToQString(str);
    }
    else if(btn == this->btnF)
    {
        sprintf(str,"0x46");
        retText=StrToQString(str);
    }
    else if(btn == this->btnG)
    {
        sprintf(str,"0x47");
        retText=StrToQString(str);
    }
    else if(btn == this->btnH)
    {
        sprintf(str,"0x48");
        retText=StrToQString(str);
    }
    else if(btn == this->btnJ)
    {
        sprintf(str,"0x4a");
        retText=StrToQString(str);
    }
    else if(btn == this->btnK)
    {
        sprintf(str,"0x4b");
        retText=StrToQString(str);
    }
    else if(btn == this->btnL)
    {
        sprintf(str,"0x4c");
        retText=StrToQString(str);
    }
    else if(btn == this->btnSemiColon)
    {
        sprintf(str,"0x3b");
        retText=StrToQString(str);
    }
    else if(btn == this->btnSp)
    {
        sprintf(str,"0x27");
        retText=StrToQString(str);
    }
    else if(btn == this->btnReturn)
    {
        sprintf(str,"0x01000004");
        retText=StrToQString(str);
    }
    else if(btn == this->btnShiftLeft)
    {
        sprintf(str,"0x01000020");
        retText=StrToQString(str);
    }
    else if(btn == this->btnZ)
    {
        sprintf(str,"0x5a");
        retText=StrToQString(str);
    }
    else if(btn == this->btnX)
    {
        sprintf(str,"0x58");
        retText=StrToQString(str);
    }
    else if(btn == this->btnC)
    {
        sprintf(str,"0x43");
        retText=StrToQString(str);
    }
    else if(btn == this->btnV)
    {
        sprintf(str,"0x56");
        retText=StrToQString(str);
    }
    else if(btn == this->btnB)
    {
        sprintf(str,"0x42");
        retText=StrToQString(str);
    }
    else if(btn == this->btnN)
    {
        sprintf(str,"0x4e");
        retText=StrToQString(str);
    }
    else if(btn == this->btnM)
    {
        sprintf(str,"0x4d");
        retText=StrToQString(str);
    }
    else if(btn == this->btnComma)
    {
        sprintf(str,"0x2c");
        retText=StrToQString(str);
    }
    else if(btn == this->btnPeriod)
    {
        sprintf(str,"0x2e");
        retText=StrToQString(str);
    }
    else if(btn == this->btnBcwdSlash)
    {
        sprintf(str,"0x2f");
        retText=StrToQString(str);
    }
    else if(btn == this->btnDel)
    {
        sprintf(str,"0x01000007");
        retText=StrToQString(str);
    }
    else if(btn == this->btnCtrlLeft)
    {
        sprintf(str,"0x01000021");
        retText=StrToQString(str);
    }
    else if(btn == this->btnAltLeft)
    {
        sprintf(str,"0x01000023");
        retText=StrToQString(str);
    }
    else if(btn == this->btnSpace)
    {
        sprintf(str,"0x20");
        retText=StrToQString(str);
    }
    else if(btn == this->btnTab)
    {
        sprintf(str,"0x01000001");
        retText=StrToQString(str);
    }
    else if(btn == this->btnQ)
    {
        sprintf(str,"0x51");
        retText=StrToQString(str);
    }
    else if(btn == this->btnW)
    {
        sprintf(str,"0x57");
        retText=StrToQString(str);
    }
    else if(btn == this->btnE)
    {
        sprintf(str,"0x45");
        retText=StrToQString(str);
    }
    else if(btn == this->btnR)
    {
        sprintf(str,"0x52");
        retText=StrToQString(str);
    }
    else if(btn == this->btnT)
    {
        sprintf(str,"0x54");
        retText=StrToQString(str);
    }
    else if(btn == this->btnY)
    {
        sprintf(str,"0x59");
        retText=StrToQString(str);
    }
    else if(btn == this->btnU)
    {
        sprintf(str,"0x55");
        retText=StrToQString(str);
    }
    else if(btn == this->btnI)
    {
        sprintf(str,"0x49");
        retText=StrToQString(str);
    }
    else if(btn == this->btnO)
    {
        sprintf(str,"0x4f");
        retText=StrToQString(str);
    }
    else if(btn == this->btnP)
    {
        sprintf(str,"0x50");
        retText=StrToQString(str);
    }
    else if(btn == this->btnStartSquare)
    {
        sprintf(str,"0x5b");
        retText=StrToQString(str);
    }
    else if(btn == this->btnCloseSquare)
    {
        sprintf(str,"0x5d");
        retText=StrToQString(str);
    }
    else if(btn == this->btnFwdSlash)
    {
        sprintf(str,"0x5c");
        retText=StrToQString(str);
    }
    else
    {
        dprintf("[mapkeyAccessibleName] Key Not Found\n");
    }
    return retText;
}
#endif
/*!key가 눌렸을 경우 key에 해당하는 key값을 event로 전달*/
void WidgetKeyboard::on_btn_clicked(int btn)
{
    QString strKeyId;
#ifndef QT_NO_ACCESSIBILITY
    strKeyId = allButtons.at(btn)->accessibleName();
#else
    strKeyId = mapkeyAccessibleName(allButtons.at(btn));
#endif
    bool isOk;
    int keyId = strKeyId.toInt(&isOk, 16);
    if (strKeyId.isEmpty() || !isOk)
    {
        printf("[on_btn_clicked] Key Not Found\n");
        return;
    }
    
    //Keys to be handled separately
    if (keyId==Qt::Key_Shift || keyId==Qt::Key_Control || keyId==Qt::Key_Alt)
        return;

    int involvedKeys = 1;
    Qt::KeyboardModifiers Modifier = Qt::NoModifier;
    if (isCtrl)
    {
        Modifier = Modifier | Qt::ControlModifier;
        involvedKeys++;
    }
    if (isShift)
    {
        Modifier = Modifier | Qt::ShiftModifier;
        involvedKeys++;
    }
    if (isAlt)
    {
        Modifier = Modifier | Qt::AltModifier;
        involvedKeys++;
    }    
    
    bool isTextKey = false;

    QString ch = allButtons.at(btn)->text().trimmed();
    if (ch=="&&")
        ch="&";
    else if (keyId==Qt::Key_Space)
        ch = " ";
    else if (checkNotTextKey(keyId))
        ch = QString();
    else
        isTextKey = true;
	    
    QKeyEvent keyEvent(QEvent::KeyPress, keyId, Modifier, ch, false, involvedKeys);
    QApplication::sendEvent(m_pParent->focusWidget(), &keyEvent);
    
    btnShiftLeft->setChecked(false);
    btnCtrlLeft->setChecked(false);
    btnAltLeft->setChecked(false);
}
/*!caps lock key가 눌릴때 caps key flag toggle */
void WidgetKeyboard::on_btnCaps_toggled(bool checked)
{
    changeTextCaps(checked);
    isCaps = checked;
}
/*!shift key가 눌릴때 shift key flag toggle */
void WidgetKeyboard::on_btnShiftLeft_toggled(bool checked)
{
    isShift = checked;
    if (isCaps)
    {
        changeTextShift(checked);
    }
    else
    {
        changeTextShift(checked);
        changeTextCaps(checked);
    }
}

/*!ctrl key가 눌릴때 ctrl key flag toggle */
void WidgetKeyboard::on_btnCtrlLeft_toggled(bool checked)
{
    isCtrl = checked;
}
/*!alt key가 눌릴때 alt key flag toggle */
void WidgetKeyboard::on_btnAltLeft_toggled(bool checked)
{
    isAlt = checked;
}

/*!shift key가 눌렸는지에 따라서 button text 변경 */
void WidgetKeyboard::changeTextShift(bool isShift)
{    
    changeTextCaps(!isShift);
    if (isShift)
    {
        btnTilt->setText(QChar('~'));
        btn1->setText(QChar('!'));
        btn2->setText(QChar('@'));
        btn3->setText(QChar('#'));
        btn4->setText(QChar('$'));
        btn5->setText(QChar('%'));
        btn6->setText(QChar('^'));        
        btn7->setText("&&");
        btn8->setText(QChar('*'));
        btn9->setText(QChar('('));
        btn0->setText(QChar(')'));
        btnHiphen->setText(QChar('_'));
        btnAssign->setText(QChar('+'));
        
        btnStartSquare->setText(QChar('{'));
        btnCloseSquare->setText(QChar('}'));
        btnFwdSlash->setText(QChar('|'));
        
        btnSemiColon->setText(QChar(':'));
        btnSp->setText(QChar('"'));
            
        btnComma->setText(QChar('<'));
        btnPeriod->setText(QChar('>'));
        btnBcwdSlash->setText(QChar('?'));
    }
    else
    {
        btnTilt->setText(QChar('`'));
        btn1->setText(QChar('1'));
        btn2->setText(QChar('2'));
        btn3->setText(QChar('3'));
        btn4->setText(QChar('4'));
        btn5->setText(QChar('5'));
        btn6->setText(QChar('6'));
        btn7->setText(QChar('7'));
        btn8->setText(QChar('8'));
        btn9->setText(QChar('9'));
        btn0->setText(QChar('0'));
        btnHiphen->setText(QChar('-'));
        btnAssign->setText(QChar('='));
            
        btnStartSquare->setText(QChar('['));
        btnCloseSquare->setText(QChar(']'));
        btnFwdSlash->setText(QChar('\\'));
        
        btnSemiColon->setText(QChar(';'));
        btnSp->setText(QChar('\''));
        
        btnComma->setText(QChar(','));
        btnPeriod->setText(QChar('.'));
        btnBcwdSlash->setText(QChar('/'));
    }
}
/*!caps lock에 따라서 button 대소문자 변경 */
void WidgetKeyboard::changeTextCaps(bool isCaps)
{    
    if (isCaps)
    {
        btnQ->setText(QChar('Q'));
        btnW->setText(QChar('W'));
        btnE->setText(QChar('E'));
        btnR->setText(QChar('R'));
        btnT->setText(QChar('T'));
        btnY->setText(QChar('Y'));
        btnU->setText(QChar('U'));
        btnI->setText(QChar('I'));
        btnO->setText(QChar('O'));
        btnP->setText(QChar('P'));

        btnA->setText(QChar('A'));
        btnS->setText(QChar('S'));
        btnD->setText(QChar('D'));
        btnF->setText(QChar('F'));
        btnG->setText(QChar('G'));
        btnH->setText(QChar('H'));
        btnJ->setText(QChar('J'));
        btnK->setText(QChar('K'));
        btnL->setText(QChar('L'));
                
        btnZ->setText(QChar('Z'));
        btnX->setText(QChar('X'));
        btnC->setText(QChar('C'));
        btnV->setText(QChar('V'));
        btnB->setText(QChar('B'));
        btnN->setText(QChar('N'));
        btnM->setText(QChar('M'));        
    }
    else
    {
        btnQ->setText(QChar('q'));
        btnW->setText(QChar('w'));
        btnE->setText(QChar('e'));
        btnR->setText(QChar('r'));
        btnT->setText(QChar('t'));
        btnY->setText(QChar('y'));
        btnU->setText(QChar('u'));
        btnI->setText(QChar('i'));
        btnO->setText(QChar('o'));
        btnP->setText(QChar('p'));

        btnA->setText(QChar('a'));
        btnS->setText(QChar('s'));
        btnD->setText(QChar('d'));
        btnF->setText(QChar('f'));
        btnG->setText(QChar('g'));
        btnH->setText(QChar('h'));
        btnJ->setText(QChar('j'));
        btnK->setText(QChar('k'));
        btnL->setText(QChar('l'));
        
        btnZ->setText(QChar('z'));
        btnX->setText(QChar('x'));
        btnC->setText(QChar('c'));
        btnV->setText(QChar('v'));
        btnB->setText(QChar('b'));
        btnN->setText(QChar('n'));
        btnM->setText(QChar('m'));
    }
}
/*!function key인지 여부를 return */
bool WidgetKeyboard::checkNotTextKey(int keyId)
{
    if (keyId==Qt::Key_Shift || keyId==Qt::Key_Control || keyId==Qt::Key_Tab || keyId==Qt::Key_Escape || keyId==Qt::Key_Return
            || keyId==Qt::Key_Enter || keyId==Qt::Key_CapsLock || keyId==Qt::Key_Delete || keyId==Qt::Key_Alt)
    {
        return true;
    }
    else
        return false;
}



