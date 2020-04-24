/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * Author:     kirigaya <kirigaya@mkacg.com>
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DSWITCHBUTTON_P_H
#define DSWITCHBUTTON_P_H

#include "dswitchbutton.h"

#include <DObjectPrivate>

DWIDGET_BEGIN_NAMESPACE

class DSwitchButtonPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    explicit DSwitchButtonPrivate(DSwitchButton *qq);
    ~DSwitchButtonPrivate();

    void init();

public:
    bool checked = false;
    QColor enabledBackground = qRgba(207, 214, 230, 255);
    QColor disabledBackground = qRgba(207, 214, 230, 255);
    QColor checkedBackground =  qRgba(44, 167, 248, 255);

    double animationStartValue;
    double animationEndValue;

public:
    D_DECLARE_PUBLIC(DSwitchButton)
};

DWIDGET_END_NAMESPACE

#endif // DSWITCHBUTTON_P_H
