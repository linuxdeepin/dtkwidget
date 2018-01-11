/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
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

#ifndef DSUGGESTBUTTON_H
#define DSUGGESTBUTTON_H

#include "dtkwidget_global.h"
#include "dobject.h"

#include <QPushButton>

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DSuggestButton : public QPushButton, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
public:
    explicit DSuggestButton(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

DWIDGET_END_NAMESPACE

#endif // DSUGGESTBUTTON_H
