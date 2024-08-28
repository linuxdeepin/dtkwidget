// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DFLOATINGWIDGET_P_H
#define DFLOATINGWIDGET_P_H

#include <DFloatingWidget>

#include <DObjectPrivate>

QT_BEGIN_NAMESPACE
class QHBoxLayout;
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

class DFloatingWidgetPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    explicit DFloatingWidgetPrivate(DFloatingWidget *qq);
    ~DFloatingWidgetPrivate();

    void init();
    void adjustPalette();

    QHBoxLayout *layout = nullptr;
    DBlurEffectWidget *background = nullptr;
    int framRadius {-1};

    D_DECLARE_PUBLIC(DFloatingWidget)
};

DWIDGET_END_NAMESPACE

#endif // DFLOATINGWIDGET_P_H
