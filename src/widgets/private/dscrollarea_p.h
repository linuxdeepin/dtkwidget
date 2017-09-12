/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
