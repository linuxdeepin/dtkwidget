// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DSTACKWIDGET_P_H
#define DSTACKWIDGET_P_H

#include <dstackwidget.h>

#include <DObjectPrivate>

class QStackedLayout;

DWIDGET_BEGIN_NAMESPACE

class DAbstractStackWidgetTransitionPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    explicit DAbstractStackWidgetTransitionPrivate(DAbstractStackWidgetTransition *qq);
    ~DAbstractStackWidgetTransitionPrivate();

    QVariantAnimation *animation;
    DAbstractStackWidgetTransition::TransitionInfo info;

    void init();

    D_DECLARE_PUBLIC(DAbstractStackWidgetTransition)
};

class DStackWidgetPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    explicit DStackWidgetPrivate(DStackWidget *qq);
    ~DStackWidgetPrivate();

    int currentIndex = 0;
    QWidget *currentWidget = nullptr;
    QList<QWidget*> widgetList = {};
    QList<QWidget*> trashWidgetList = {};
    DAbstractStackWidgetTransition *transition = nullptr;

    void init();
    void setCurrentIndex(int index);

    D_DECLARE_PUBLIC(DStackWidget)
};

DWIDGET_END_NAMESPACE

#endif // DSTACKWIDGET_P

