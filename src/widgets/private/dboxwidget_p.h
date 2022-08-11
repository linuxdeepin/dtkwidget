// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DBOXWIDGET_P_H
#define DBOXWIDGET_P_H

#include <dboxwidget.h>

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

