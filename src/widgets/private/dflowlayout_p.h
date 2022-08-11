// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DFLOWLAYOUT_P_H
#define DFLOWLAYOUT_P_H

#include <dflowlayout.h>

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

