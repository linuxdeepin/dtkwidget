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

#include "libdui_global.h"
#include "dscrollarea.h"
#include "dobject_p.h"

class QPropertyAnimation;

DWIDGET_NAMESPACE_BEGIN

class DScrollAreaPrivate : public DObjectPrivate
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

DWIDGET_NAMESPACE_END

#endif // DSCROLLAREA_P_H
