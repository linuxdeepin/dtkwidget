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

#include <DObjectPrivate>
#include <QPointer>
#include <DFloatingWidget>

DWIDGET_BEGIN_NAMESPACE

class DToolTip;
class DLineEditPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    Q_DECLARE_PUBLIC(DLineEdit)

public:
    DLineEditPrivate(DLineEdit *q);

    void init();

    bool m_isAlert = false;
    QPointer<DToolTip> tooltip;
    QPointer<DFloatingWidget> frame;

    QWidget *leftWidget;
    QWidget *rightWidget;

    QLineEdit *lineEdit;
    QHBoxLayout *hLayout;
};

DWIDGET_END_NAMESPACE

#endif // DLINEEDIT_P_H

