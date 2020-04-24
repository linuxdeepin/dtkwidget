/*
 * Copyright (C) 2017 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
 *
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
#ifndef DICONBUTTON_P_H
#define DICONBUTTON_P_H

#include "dobject_p.h"
#include "diconbutton.h"

DWIDGET_BEGIN_NAMESPACE

class DIconButtonPrivate : public DCORE_NAMESPACE::DObjectPrivate
{
public:
    DIconButtonPrivate(DIconButton *qq);

    bool flat = false;
    qint64 iconType = -1;

    D_DECLARE_PUBLIC(DIconButton)
};

DWIDGET_END_NAMESPACE

#endif // DICONBUTTON_P_H
