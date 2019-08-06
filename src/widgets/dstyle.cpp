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

QBrush DStyle::generatedBrush(const QStyleOption *option, const QBrush &base, QPalette::ColorGroup cg, QPalette::ColorRole role) const
{
    return generatedBrush(getState(option), option, base, cg, role);
}

QBrush DStyle::generatedBrush(DStyle::StyleState state, const QStyleOption *option, const QBrush &base, QPalette::ColorGroup cg, QPalette::ColorRole role) const
{
    return generatedBrush(getFlags(option) | state, base, cg, role);
}

QBrush DStyle::generatedBrush(StateFlags flags, const QBrush &base, QPalette::ColorGroup cg, QPalette::ColorRole role) const
{
    Q_UNUSED(flags)
    Q_UNUSED(base)
    Q_UNUSED(cg)
    Q_UNUSED(role)

    return base;
}

QBrush DStyle::generatedBrush(const QStyleOption *option, const QBrush &base, QPalette::ColorGroup cg, DPalette::ColorType type) const
{
    return generatedBrush(getState(option), option, base, cg, type);
}

QBrush DStyle::generatedBrush(DStyle::StyleState state, const QStyleOption *option, const QBrush &base, QPalette::ColorGroup cg, DPalette::ColorType type) const
{
    return generatedBrush(getFlags(option) | state, base, cg, type);
}

QBrush DStyle::generatedBrush(StateFlags flags, const QBrush &base, QPalette::ColorGroup cg, DPalette::ColorType role) const
{
    Q_UNUSED(flags)
    Q_UNUSED(cg)
    Q_UNUSED(role)

    return base;
}

DWIDGET_END_NAMESPACE
