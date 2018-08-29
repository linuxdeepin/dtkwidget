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

#ifndef DWDIGET_P_H
#define DWDIGET_P_H

#include <DObjectPrivate>
#include "dborderlesswidget.h"
#include "../../platforms/x11/xutil.h"

class QVBoxLayout;

class QGraphicsDropShadowEffect;

class DX11Widget;

DWIDGET_BEGIN_NAMESPACE

class DBorderlessWidgetPrivate: public DTK_CORE_NAMESPACE::DObjectPrivate
{
    D_DECLARE_PUBLIC(DBorderlessWidget)
public:
    explicit DBorderlessWidgetPrivate(DBorderlessWidget *q);

    void init();

    QSize externSize(const QSize &size) const;
    QMargins externMargins() const;
    int externWidth() const;
    void updateContentsMargins();

    void _q_onTitleBarMousePressed(Qt::MouseButtons buttons) const;

    bool leftPressed;
    bool resizable;

    int                 m_ShadowWidth;
    int                 m_NormalShadowWidth;
    int                 m_Radius;
    int                 m_Border;
    int                 m_ResizeHandleWidth;
    bool                m_MousePressed;
    QPoint              m_LastMousePos;
    Qt::WindowFlags     dwindowFlags;
    DBorderlessWidget::DecorationFlags decorationFlags;
    QColor              m_backgroundColor;
    QColor              shadowColor;
    QPoint              shadowOffset;

    QPixmap             m_Background;
    QPixmap             shadowPixmap;
    QWidget             *windowWidget     = nullptr;
    QVBoxLayout         *rootLayout     = nullptr;
    DTitlebar           *titlebar       = nullptr;
    QWidget             *contentWidget  = nullptr;

    XUtils::CornerEdge resizingCornerEdge = XUtils::CornerEdge::kInvalid;
};



DWIDGET_END_NAMESPACE

#endif // DWDIGET_P_H
