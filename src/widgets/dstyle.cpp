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

DStyle::DStyle()
{

}

QBrush DStyle::generatedBrush(const QStyleOption *option, const QBrush &base, QPalette::ColorGroup cg, QPalette::ColorRole role) const
{
    StyleState state = SS_NormalState;

    if (option->state.testFlag(State_Sunken)) {
        state = SS_PressState;
    } else if (option->state.testFlag(State_MouseOver)) {
        state = SS_HoverState;
    }

    return generatedBrush(state, option, base, cg, role);
}

QBrush DStyle::generatedBrush(DStyle::StyleState state, const QStyleOption *option, const QBrush &base, QPalette::ColorGroup cg, QPalette::ColorRole role) const
{
    StateFlags flags = state;

    if (option->state.testFlag(State_On)) {
        flags |= SS_CheckedFlag;
    }

    if (option->state.testFlag(State_Selected)) {
        flags |= SS_SelectedFlag;
    }

    if (option->state.testFlag(State_HasFocus)) {
        flags |= SS_FocusFlag;
    }

    return generatedBrush(flags, base, cg, role);
}

QBrush DStyle::generatedBrush(StateFlags flags, const QBrush &base, QPalette::ColorGroup cg, QPalette::ColorRole role) const
{
    Q_UNUSED(cg)
    Q_UNUSED(role)
    QColor color = base.color();

    if (color.isValid()) {
        return base;
    }

    QBrush new_brush = base;

    switch (flags & StyleState_Mask) {
    case SS_HoverState:
        new_brush.setColor(color.lighter(150));
        break;
    case SS_PressState:
        new_brush.setColor(color.darker(150));
        break;
    default:
        break;
    }

    return new_brush;
}

DWIDGET_END_NAMESPACE
