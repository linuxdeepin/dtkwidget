/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DPUSHBUTTON_P_H
#define DPUSHBUTTON_P_H

#include "dpushbutton.h"
#include <DObjectPrivate>

DWIDGET_BEGIN_NAMESPACE

class DPushButtonPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    DPushButtonPrivate(DPushButton *qq);

    QIcon normalIcon;
    QIcon hoverIcon;
    QIcon pressIcon;

    QString normalIconPath;
    QString hoverIconPath;
    QString pressIconPath;

    D_DECLARE_PUBLIC(DPushButton)
};

DWIDGET_END_NAMESPACE

#endif // DPUSHBUTTON_P_H
