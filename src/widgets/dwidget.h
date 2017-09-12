/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef DWIDGET_H
#define DWIDGET_H

#include <QWidget>
#include <dobject.h>
#include <dtkwidget_global.h>

#include "dx11widget.h"

DWIDGET_BEGIN_NAMESPACE

class DWidgetPrivate;

class LIBDTKWIDGETSHARED_EXPORT DWidget : public DX11Widget
{
    Q_OBJECT
public:
    explicit DWidget(QWidget *parent = 0);
    explicit DWidget(DWidgetPrivate &dd, QWidget *parent = 0);

private:
    D_DECLARE_PRIVATE(DWidget)
    Q_DISABLE_COPY(DWidget)
};

DWIDGET_END_NAMESPACE


#endif // DWIDGET_H
