/*
 * Copyright (C) 2011 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     wp <wangpeng_cm@deepin.com>
 *
 * Maintainer: wp <wangpeng_cm@deepin.com>
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

#ifndef DLABEL_P_H
#define DLABEL_P_H

#include "dlabel.h"
#include "dstyle.h"

#include <DObjectPrivate>

DWIDGET_BEGIN_NAMESPACE

class DLabelPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    Q_DECLARE_PUBLIC(DLabel)

public:
    DLabelPrivate(DLabel *q);

    void init();
    static Qt::LayoutDirection textDirection(QLabelPrivate *d);
    static QRectF documentRect(QLabelPrivate *d);
    static QRectF layoutRect(QLabelPrivate *d);
    static void ensureTextLayouted(QLabelPrivate *d);

    DPalette::ColorType color = DPalette::NoType;
    Qt::TextElideMode elideMode = Qt::ElideNone;
};

DWIDGET_END_NAMESPACE

#endif // DLABEL_P_H
