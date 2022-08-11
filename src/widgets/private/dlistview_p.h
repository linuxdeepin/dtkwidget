// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DLISTVIEW_P_H
#define DLISTVIEW_P_H

#include <DListView>

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

