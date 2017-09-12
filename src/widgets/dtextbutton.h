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

#ifndef DTEXTBUTTON_H
#define DTEXTBUTTON_H

#include <QPushButton>
#include <QPainter>

#include "dtkwidget_global.h"

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DTextButton : public QPushButton
{
    Q_OBJECT
public:
    DTextButton(const QString & text, QWidget * parent = 0);
    ~DTextButton();
};

DWIDGET_END_NAMESPACE

#endif // DTEXTBUTTON_H
