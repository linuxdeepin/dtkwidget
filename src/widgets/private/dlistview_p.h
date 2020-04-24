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

#ifndef DLISTVIEW_P_H
#define DLISTVIEW_P_H

#include "dlistview.h"

#include <DObjectPrivate>

DWIDGET_BEGIN_NAMESPACE

class DBoxWidget;
class DListViewPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    DListViewPrivate(DListView *qq);
    ~DListViewPrivate();

    void init();

    void onOrientationChanged();

    DBoxWidget *headerLayout = nullptr;
    DBoxWidget *footerLayout = nullptr;

    QList<QWidget*> headerList;
    QList<QWidget*> footerList;

#if(QT_VERSION < 0x050500)
    int left = 0, top = 0, right = 0, bottom = 0; // viewport margin
#endif

    D_DECLARE_PUBLIC(DListView)
};

DWIDGET_END_NAMESPACE

#endif // DLISTVIEW_P_H

