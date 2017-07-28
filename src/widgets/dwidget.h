/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DWIDGET_H
#define DWIDGET_H

#include <QWidget>
#include <dobject.h>
#include <dtkwidget_global.h>

#include "dx11widget.h"

DWIDGET_BEGIN_NAMESPACE

class DWidgetPrivate;

class LIBDTKWIDGETSHARED_EXPORT DWidget : public DX11Widget
{
    Q_OBJECT
public:
    explicit DWidget(QWidget *parent = 0);
    explicit DWidget(DWidgetPrivate &dd, QWidget *parent = 0);

private:
    D_DECLARE_PRIVATE(DWidget)
    Q_DISABLE_COPY(DWidget)
};

DWIDGET_END_NAMESPACE


#endif // DWIDGET_H
