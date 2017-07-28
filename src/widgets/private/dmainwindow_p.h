/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DMAINWINDOW_P_H
#define DMAINWINDOW_P_H

#include <DObjectPrivate>
#include "dtkwidget_global.h"
#include "dmainwindow.h"

DWIDGET_BEGIN_NAMESPACE

class DPlatformWindowHandle;
class DTitlebar;

class DMainWindowPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    DMainWindowPrivate(DMainWindow *qq);

    void init();

    DPlatformWindowHandle *handle = Q_NULLPTR;
    DTitlebar *titlebar = Q_NULLPTR;

private:
    D_DECLARE_PUBLIC(DMainWindow)
};

DWIDGET_END_NAMESPACE

#endif // DMAINWINDOW_P_H
