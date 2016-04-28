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

class QVBoxLayout;

class QGraphicsDropShadowEffect;

class DX11Widget;

DWIDGET_BEGIN_NAMESPACE

class DX11WidgetPrivate: public DObjectPrivate
{
    D_DECLARE_PUBLIC(DX11Widget)
public:
    explicit DX11WidgetPrivate(DX11Widget *q);

    void init();

    QSize externSize(const QSize &size) const;
    bool leftPressed;
    bool resizable;

    int                 m_ShadowWidth;
    int                 m_NormalShadowWidth;
    int                 m_Radius;
    int                 m_Border;
    bool                m_MousePressed;
    QPoint              m_LastMousePos;
    Qt::WindowFlags     dwindowFlags;

    QPixmap             m_Background;
    QVBoxLayout         *rootLayout;
    DTitlebar           *titlebar;
    QWidget             *contentWidget;
    QColor              m_backgroundColor;

    QGraphicsDropShadowEffect *m_Shadow;
};


class DWidgetPrivate: public DX11WidgetPrivate
{
    D_DECLARE_PUBLIC(DWidget)

public:
    explicit DWidgetPrivate(DWidget *q);

    bool leftPressed;
    bool resizable;
};

DWIDGET_END_NAMESPACE

#endif // DWDIGET_P_H
