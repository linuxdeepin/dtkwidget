/*
 * Copyright (C) 2011 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     sunkang <sunkang_cm@deepin.com>
 *
 * Maintainer: sunkang <sunkang_cm@deepin.com>
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

#ifndef DFRAME_P_H
#define DFRAME_P_H

#include <DStyle>
#include <DFrame>

#include <DObjectPrivate>

DWIDGET_BEGIN_NAMESPACE


class DFramePrivate : public DCORE_NAMESPACE::DObjectPrivate
{
public:
    DFramePrivate(DFrame *qq);
    bool frameRounded;
    DPalette::ColorType backType;

    D_DECLARE_PUBLIC(DFrame)
};

DWIDGET_END_NAMESPACE

#endif // DFRAME_P_H
