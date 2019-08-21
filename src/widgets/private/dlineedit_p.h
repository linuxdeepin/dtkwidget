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

#ifndef DLINEEDIT_P_H
#define DLINEEDIT_P_H

#include "dlineedit.h"
#include "dimagebutton.h"

#include <DObjectPrivate>

#include <QHBoxLayout>

QT_BEGIN_NAMESPACE
class QWidgetAction;
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

class DArrowRectangle;
class DLineEditPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    Q_DECLARE_PUBLIC(DLineEdit)

public:
    DLineEditPrivate(DLineEdit *q);

    void init();

public:
    bool m_isAlert = false;

    DImageButton *m_rightIcon;
    QWidgetAction *m_iconAction;
    DArrowRectangle *tooltip = Q_NULLPTR;

    QWidget *leftWidget;
    QWidget *rightWidget;
};

DWIDGET_END_NAMESPACE

#endif // DLINEEDIT_P_H

