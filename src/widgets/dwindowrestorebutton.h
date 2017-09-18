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

#ifndef DWINDOWRESTOREBUTTON_H
#define DWINDOWRESTOREBUTTON_H

#include <QObject>
#include <QString>

#include "dimagebutton.h"

DWIDGET_BEGIN_NAMESPACE

class Q_DECL_DEPRECATED_X("DWindowMaxButton is sufficient representing the two states.") DWindowRestoreButton : public DImageButton
{
    Q_OBJECT
public:
    DWindowRestoreButton(QWidget * parent = 0);
};

DWIDGET_END_NAMESPACE

#endif // DWINDOWRESTOREBUTTON_H
