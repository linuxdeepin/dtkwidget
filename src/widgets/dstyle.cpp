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
#include "dstyle.h"

#include <QStyleOption>

DWIDGET_BEGIN_NAMESPACE

inline static int adjustColorValue(int base, qint8 increment, int max = 255)
{
    return increment > 0 ? (max - base) * increment / 100.0 + base
                         : base * (1 + increment / 100.0);
}

QColor DStyle::adjustColor(const QColor &base,
                           qint8 hueFloat, qint8 saturationFloat, qint8 lightnessFloat,
                           qint8 redFloat, qint8 greenFloat, qint8 blueFloat, qint8 alphaFloat)
{
    // 按HSL格式调整
    int H, S, L;
    base.getHsl(&H, &S, &L);

    H = H > 0 ? adjustColorValue(H, hueFloat, 359) : H;
    S = adjustColorValue(S, saturationFloat);
    L = adjustColorValue(L, lightnessFloat);

    QColor new_color = QColor::fromHsl(H, S, L);

    // 按RGB格式调整
    int A, R, G, B;
    new_color.getRgb(&R, &G, &B, &A);

    A = adjustColorValue(A, alphaFloat);
    R = adjustColorValue(R, redFloat);
    G = adjustColorValue(G, greenFloat);
    B = adjustColorValue(B, blueFloat);

    new_color.setRgb(R, G, B, A);

    return new_color;
}

QColor DStyle::blendColor(const QColor &substrate, const QColor &superstratum)
{
    QColor c2 = superstratum.toRgb();

    if (c2.alpha() >= 255)
        return c2;

    QColor c1 = substrate.toRgb();
    qreal c1_weight = 1 - c2.alphaF();

    int r = c1_weight * c1.red() + c2.alphaF() * c2.red();
    int g = c1_weight * c1.green() + c2.alphaF() * c2.green();
    int b = c1_weight * c1.blue() + c2.alphaF() * c2.blue();

    return QColor(r, g, b, c1.alpha());
}

DStyle::DStyle()
{

}

static DStyle::StyleState getState(const QStyleOption *option)
{
    DStyle::StyleState state = DStyle::SS_NormalState;

    if (option->state.testFlag(DStyle::State_Sunken)) {
        state = DStyle::SS_PressState;
    } else if (option->state.testFlag(DStyle::State_MouseOver)) {
        state = DStyle::SS_HoverState;
    }

    return state;
}

static DStyle::StateFlags getFlags(const QStyleOption *option)
{
    DStyle::StateFlags flags = 0;

    if (option->state.testFlag(DStyle::State_On)) {
        flags |= DStyle::SS_CheckedFlag;
    }

    if (option->state.testFlag(DStyle::State_Selected)) {
        flags |= DStyle::SS_SelectedFlag;
    }

    if (option->state.testFlag(DStyle::State_HasFocus)) {
        flags |= DStyle::SS_FocusFlag;
    }

    return flags;
}

int DStyle::pixelMetric(QStyle::PixelMetric m, const QStyleOption *opt, const QWidget *widget) const
{
    if (static_cast<int>(m) == PM_FrameMargins) {
        int shadow_radius = pixelMetric(PM_ShadowRadius, opt, widget);
        int shadow_xoffset = pixelMetric(PM_ShadowHOffset, opt, widget);
        int shadow_yoffset = pixelMetric(PM_ShadowVOffset, opt, widget);

        int border_width = pixelMetric(PM_FocusBorderWidth, opt, widget);
        int border_spacing = pixelMetric(PM_FocusBorderSpacing, opt, widget);

        int margins = qMax((shadow_radius + qMax(shadow_xoffset, shadow_yoffset)) / 2, border_width + border_spacing);

        return margins;
    }

    return QCommonStyle::pixelMetric(m, opt, widget);
}

QBrush DStyle::generatedBrush(const QStyleOption *option, const QBrush &base, QPalette::ColorGroup cg, QPalette::ColorRole role) const
{
    return generatedBrush(getState(option), option, base, cg, role);
}

QBrush DStyle::generatedBrush(DStyle::StyleState state, const QStyleOption *option, const QBrush &base, QPalette::ColorGroup cg, QPalette::ColorRole role) const
{
    if (auto proxy = qobject_cast<const DStyle*>(this->proxy())) {
        return proxy->generatedBrush(getFlags(option) | state, base, cg, role, option);
    }

    return generatedBrush(getFlags(option) | state, base, cg, role, option);
}

QBrush DStyle::generatedBrush(StateFlags flags, const QBrush &base, QPalette::ColorGroup cg, QPalette::ColorRole role, const QStyleOption *option) const
{
    Q_UNUSED(flags)
    Q_UNUSED(base)
    Q_UNUSED(cg)
    Q_UNUSED(role)
    Q_UNUSED(option)

    return base;
}

QBrush DStyle::generatedBrush(const QStyleOption *option, const QBrush &base, QPalette::ColorGroup cg, DPalette::ColorType type) const
{
    return generatedBrush(getState(option), option, base, cg, type);
}

QBrush DStyle::generatedBrush(DStyle::StyleState state, const QStyleOption *option, const QBrush &base, QPalette::ColorGroup cg, DPalette::ColorType type) const
{
    if (auto proxy = qobject_cast<const DStyle*>(this->proxy())) {
        return proxy->generatedBrush(getFlags(option) | state, base, cg, type, option);
    }

    return generatedBrush(getFlags(option) | state, base, cg, type, option);
}

QBrush DStyle::generatedBrush(StateFlags flags, const QBrush &base, QPalette::ColorGroup cg, DPalette::ColorType role, const QStyleOption *option) const
{
    Q_UNUSED(flags)
    Q_UNUSED(cg)
    Q_UNUSED(role)
    Q_UNUSED(option)

    return base;
}

DWIDGET_END_NAMESPACE
