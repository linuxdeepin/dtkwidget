/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DIPV4LINEEDIT_P_H
#define DIPV4LINEEDIT_P_H

#include "dlineedit_p.h"
#include "dipv4lineedit.h"
#include "dimagebutton.h"

DWIDGET_BEGIN_NAMESPACE

class DIpv4LineEditPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    Q_DECLARE_PUBLIC(DIpv4LineEdit)

public:
    DIpv4LineEditPrivate(DIpv4LineEdit *parent);

    void init();

    QLabel *getLabel();
    QLineEdit *getEdit();

    void setFocus(bool focus);

    void _q_updateLineEditText();
    void _q_setIpLineEditText(const QString &text);

    QList<QLineEdit*> editList;
    QWidget *editMainWidget;

    bool focus = false;
};

DWIDGET_END_NAMESPACE

#endif // DIPV4LINEEDIT_P_H

