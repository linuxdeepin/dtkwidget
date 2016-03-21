/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DWDIGET_P_H
#define DWDIGET_P_H

#include "private/dobject_p.h"
#include "dwidget.h"

DWIDGET_BEGIN_NAMESPACE

class DWidgetPrivate: public DObjectPrivate
{
    D_DECLARE_PUBLIC(DWidget)

public:
    explicit DWidgetPrivate(DWidget *q);

    bool leftPressed;
    bool resizable;
};

DWIDGET_END_NAMESPACE

#endif // DWDIGET_P_H
