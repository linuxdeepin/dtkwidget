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

#ifndef DFLOWLAYOUT_P_H
#define DFLOWLAYOUT_P_H

#include "dflowlayout.h"

#include <DObjectPrivate>

class QLayoutItem;

DWIDGET_BEGIN_NAMESPACE

class DFlowLayoutPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    DFlowLayoutPrivate(DFlowLayout *qq);

    QSize doLayout(const QRect &rect, bool testOnly) const;

    QList<QLayoutItem*> itemList;
    int horizontalSpacing = 0;
    int verticalSpacing = 0;
    mutable QSize sizeHint;
    DFlowLayout::Flow flow = DFlowLayout::Flow::LeftToRight;

    D_DECLARE_PUBLIC(DFlowLayout)
};

DWIDGET_END_NAMESPACE

#endif // DFLOWLAYOUT_P_H

