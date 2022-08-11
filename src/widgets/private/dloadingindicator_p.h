// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DLOADINGINDICATOR_P
#define DLOADINGINDICATOR_P

#include <DObjectPrivate>

#include <dloadingindicator.h>

DWIDGET_BEGIN_NAMESPACE

class DLoadingIndicatorPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    DLoadingIndicatorPrivate(DLoadingIndicator *qq);

    void init();
    void setLoadingItem(QGraphicsItem *item);

    QVariantAnimation rotateAni;
    bool loading;
    QWidget *widgetSource = NULL;
    bool smooth = false;
    DLoadingIndicator::RotationDirection direction = DLoadingIndicator::Clockwise;

    D_DECLARE_PUBLIC(DLoadingIndicator)
};

DWIDGET_END_NAMESPACE

#endif // DLOADINGINDICATOR_P

