/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef DBOXWIDGET_P_H
#define DBOXWIDGET_P_H

#include "dboxwidget.h"

#include <DObjectPrivate>

class QBoxLayout;

DWIDGET_BEGIN_NAMESPACE

class DBoxWidgetPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    DBoxWidgetPrivate(DBoxWidget *qq);

    QBoxLayout *layout;

    void init();

    Q_DECLARE_PUBLIC(DBoxWidget)
};

DWIDGET_END_NAMESPACE

#endif // DBOXWIDGET_P_H

