/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
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
#ifndef DBUTTONBOX_P_H
#define DBUTTONBOX_P_H

#include "dbuttonbox.h"

#include <DObjectPrivate>

QT_BEGIN_NAMESPACE
class QBoxLayout;
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

class DButtonBoxPrivate : public DCORE_NAMESPACE::DObjectPrivate
{
public:
    DButtonBoxPrivate(DButtonBox *qq);
    void init();

    QButtonGroup *group;
    QBoxLayout *layout;

    D_DECLARE_PUBLIC(DButtonBox)
};

DWIDGET_END_NAMESPACE

#endif // DBUTTONBOX_P_H
