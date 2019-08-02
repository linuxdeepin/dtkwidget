/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
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
#ifndef DSTYLE_H
#define DSTYLE_H

#include <dtkwidget_global.h>

#include <QCommonStyle>

DWIDGET_BEGIN_NAMESPACE

class DStyle : public QCommonStyle
{
    Q_OBJECT

public:
    enum PrimitiveElement {
        PE_ItemBackground = QStyle::PE_CustomBase + 1,          //列表项的背景色
        PE_CustomBase = QStyle::PE_CustomBase + 0xf00000
    };

    enum PixelMetric {
        PM_FocusBorderWidth = QStyle::PM_CustomBase + 1,        //控件焦点状态的边框宽度
        PM_FocusBorderSpacing,                                  //控件内容和border之间的间隔
        PM_FrameRadius,                                         //控件的圆角大小
        PM_ShadowRadius,                                        //控件阴影效果的半径
        PM_ShadowHOffset,                                       //阴影在水平方向的偏移
        PM_ShadowVOffset,                                       //阴影在竖直方向的偏移
        PM_CustomBase = QStyle::PM_CustomBase + 0xf00000
    };

    enum StyleState {
        SS_NormalState = 0x00000000,
        SS_HoverState = 0x00000001,
        SS_PressState = 0x00000002,
        SS_StateCustomBase = 0x000000f0,

        StyleState_Mask = 0x000000ff,
        SS_CheckedFlag = 0x00000100,
        SS_SelectedFlag = 0x00000200,
        SS_FocusFlag = 0x00000400,
        SS_FlagCustomBase = 0xf00000
    };
    Q_DECLARE_FLAGS(StateFlags, StyleState)

    DStyle();

    inline void drawPrimitive(PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w = nullptr) const
    { drawPrimitive(static_cast<QStyle::PrimitiveElement>(pe), opt, p, w); }
    inline int pixelMetric(PixelMetric m, const QStyleOption *opt = nullptr, const QWidget *widget = nullptr) const
    { return pixelMetric(m, opt, widget); }

    // 获取一个加工后的画笔
    QBrush generatedBrush(const QStyleOption *option, const QBrush &base,
                          QPalette::ColorGroup cg = QPalette::Normal,
                          QPalette::ColorRole role = QPalette::NoRole) const;
    QBrush generatedBrush(StyleState state, const QStyleOption *option, const QBrush &base,
                          QPalette::ColorGroup cg = QPalette::Normal,
                          QPalette::ColorRole role = QPalette::NoRole) const;
    virtual QBrush generatedBrush(StateFlags flags, const QBrush &base,
                                  QPalette::ColorGroup cg = QPalette::Normal,
                                  QPalette::ColorRole role = QPalette::NoRole) const;

    using QStyle::drawPrimitive;
    using QStyle::pixelMetric;
};

DWIDGET_END_NAMESPACE

#endif // DSTYLE_H
