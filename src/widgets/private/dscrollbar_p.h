/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DSCROLLBAR_P_H
#define DSCROLLBAR_P_H

#include "dscrollbar.h"

#include <DObjectPrivate>

QT_BEGIN_NAMESPACE
class QGraphicsOpacityEffect;
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

class DScrollBarPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    DScrollBarPrivate(DScrollBar *qq);

    void init();
    void setOpacity(qreal opacity);
    void _q_hidden();
    void _q_updateOpacity();

    bool canPaint = true;
    bool autoHide = true;
    qreal opacity = 1;

    QTimer *timer = Q_NULLPTR;
    QTimer *opacityTimer = Q_NULLPTR;
    QGraphicsOpacityEffect *effect = Q_NULLPTR;

    D_DECLARE_PUBLIC(DScrollBar)
};

DWIDGET_END_NAMESPACE

#endif // DSCROLLBAR_P_H
