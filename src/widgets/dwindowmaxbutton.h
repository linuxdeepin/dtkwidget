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

#ifndef DWINDOWMAXBUTTON_H
#define DWINDOWMAXBUTTON_H

#include <dobject.h>

#include "dimagebutton.h"
#include "private/dimagebutton_p.h"

DWIDGET_BEGIN_NAMESPACE

class DWindowMaxButtonPrivate;

class LIBDTKWIDGETSHARED_EXPORT DWindowMaxButton : public DImageButton
{
    Q_OBJECT
public:
    DWindowMaxButton(QWidget * parent = 0);

    Q_PROPERTY(bool isMaximized READ isMaximized WRITE setMaximized NOTIFY maximizedChanged)

    bool isMaximized() const;

public Q_SLOTS:
    void setMaximized(bool isMaximized);

Q_SIGNALS:
    void maximizedChanged(bool isMaximized);

private:
    D_DECLARE_PRIVATE(DWindowMaxButton)
};

DWIDGET_END_NAMESPACE

#endif // DWINDOWMAXBUTTON_H
