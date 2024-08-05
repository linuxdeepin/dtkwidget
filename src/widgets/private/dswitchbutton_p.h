// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DSWITCHBUTTON_P_H
#define DSWITCHBUTTON_P_H

#include <DSwitchButton>
#include <DDciIconPlayer>

#include <DObjectPrivate>

DGUI_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

class DSwitchButtonPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    explicit DSwitchButtonPrivate(DSwitchButton *qq);
    ~DSwitchButtonPrivate();

    void init();

public:
    bool checked = false;
    QColor enabledBackground = qRgba(207, 214, 230, 255);
    QColor disabledBackground = qRgba(207, 214, 230, 255);
    QColor checkedBackground =  qRgba(44, 167, 248, 255);

    double animationStartValue = 0.0;
    double animationEndValue = 0.0;

    DDciIconPlayer player;

public:
    D_DECLARE_PUBLIC(DSwitchButton)
};

DWIDGET_END_NAMESPACE

#endif // DSWITCHBUTTON_P_H
