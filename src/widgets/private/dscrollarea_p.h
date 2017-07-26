/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DSCROLLAREA_P_H
#define DSCROLLAREA_P_H

#include "dtkwidget_global.h"
#include "dscrollarea.h"

#include <DObjectPrivate>

class QPropertyAnimation;

DWIDGET_BEGIN_NAMESPACE

class DScrollAreaPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
protected:
    DScrollAreaPrivate(DScrollArea *qq);
    ~DScrollAreaPrivate();

private:
    void init();

    bool autoHideScrollBar;
    DScrollBar *vBar;
    DScrollBar *hBar;
    QPropertyAnimation *scrollBarAnimation;
    QTimer *timer;

    D_DECLARE_PUBLIC(DScrollArea)
};

DWIDGET_END_NAMESPACE

#endif // DSCROLLAREA_P_H
