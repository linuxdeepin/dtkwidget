/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <qnamespace.h>
#include "dtkwidget_global.h"

QT_BEGIN_NAMESPACE
class QWidget;
class QPoint;
class QMargins;
class QRect;
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

namespace XUtils
{

enum class CornerEdge
{
    kInvalid = 0,
    kTop = 1,
    kRight = 2,
    kBottom = 4,
    kLeft = 8,
    kTopLeft = 1 | 8,
    kTopRight = 1 | 2,
    kBottomLeft = 4 | 8,
    kBottomRight = 4 | 2,
};

enum class XCursorType
{
    kInvalid = -1,
    kArrow = 2,
    kTop = 138,
    kTopRight = 136,
    kRight = 96,
    kBottomRight = 14,
    kBottom = 16,
    kBottomLeft = 12,
    kLeft = 70,
    kTopLeft = 134,
};

D_DECL_DEPRECATED void ChangeWindowMaximizedState(const QWidget *widget, int wm_state);
D_DECL_DEPRECATED CornerEdge GetCornerEdge(const QWidget *widget, int x, int y, const QMargins &margins, int border_width);
D_DECL_DEPRECATED bool UpdateCursorShape(const QWidget *widget, int x, int y, const QMargins &margins, int border_width);
D_DECL_DEPRECATED bool IsCornerEdget(const QWidget *widget, int x, int y, const QMargins &margins, int border_width);
D_DECL_DEPRECATED void MoveResizeWindow(const QWidget *widget, Qt::MouseButton qbutton, int x, int y, const QMargins &margins, int border_width);

D_DECL_DEPRECATED void StartResizing(const QWidget *w, const QPoint &globalPoint, const CornerEdge &ce);
D_DECL_DEPRECATED void SendMoveResizeMessage(const QWidget *widget, Qt::MouseButton qbutton, int action);
D_DECL_DEPRECATED void MoveWindow(const QWidget *widget, Qt::MouseButton qbutton);
D_DECL_DEPRECATED void CancelMoveWindow(const QWidget *widget, Qt::MouseButton qbutton);

D_DECL_DEPRECATED void ResetCursorShape(const QWidget *widget);
D_DECL_DEPRECATED bool SetCursorShape(const QWidget *widget, int cursor_id);
D_DECL_DEPRECATED void ShowFullscreenWindow(const QWidget *widget, bool is_fullscreen);
D_DECL_DEPRECATED void ShowMaximizedWindow(const QWidget *widget);
D_DECL_DEPRECATED void ShowMinimizedWindow(const QWidget *widget, bool minimized);
D_DECL_DEPRECATED void ShowNormalWindow(const QWidget *widget);
D_DECL_DEPRECATED void ToggleMaximizedWindow(const QWidget *widget);
D_DECL_DEPRECATED void SkipTaskbarPager(const QWidget *widget);
D_DECL_DEPRECATED void SetStayOnTop(const QWidget *widget, bool on);
D_DECL_DEPRECATED void SetMouseTransparent(const QWidget *widget, bool on);
D_DECL_DEPRECATED void SetWindowExtents(const QWidget *widget, const QMargins &margins, const int resizeHandlSize);
D_DECL_DEPRECATED void SetWindowExtents(uint wid, const QRect &windowRect, const QMargins &margins, const int resizeHandleSize);
D_DECL_DEPRECATED void PropagateSizeHints(const QWidget *w);
D_DECL_DEPRECATED void DisableResize(const QWidget *w);

}

DWIDGET_END_NAMESPACE
