/*!
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

#ifndef __WIDGETKEYBOARD_H_
#define __WIDGETKEYBOARD_H_

#include <QWidget>
#include "ui_WidgetKeyboard.h"
class QSignalMapper;

class WidgetKeyboard : public QWidget, public Ui::WidgetKeyboard
{
    Q_OBJECT
    
    private:
        QWidget *m_pParent;
        bool isCaps;
        bool isShift;
        bool isCtrl;
        bool isAlt;
        QSignalMapper *signalMapper;
        QList<QToolButton *> allButtons;
        
    public slots:
        void on_btn_clicked(int btn);
        void on_btnCaps_toggled(bool checked);
        void on_btnShiftLeft_toggled(bool checked);
        void on_btnCtrlLeft_toggled(bool checked);
        void on_btnAltLeft_toggled(bool checked);
        void changeTextShift(bool isShift);
        void changeTextCaps(bool isCaps);
        bool checkNotTextKey(int keyId);

    public:
        WidgetKeyboard(QWidget *parent = NULL);
        ~WidgetKeyboard();
#ifdef QT_NO_ACCESSIBILITY
        QString mapkeyAccessibleName(QToolButton *btn);
#endif
};
#endif /*!__WIDGETKEYBOARD_H_*/
