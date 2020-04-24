/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef DSTACKWIDGET_P_H
#define DSTACKWIDGET_P_H

#include "dstackwidget.h"

#include <DObjectPrivate>

class QStackedLayout;

DWIDGET_BEGIN_NAMESPACE

class DAbstractStackWidgetTransitionPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    DAbstractStackWidgetTransitionPrivate(DAbstractStackWidgetTransition *qq);
    ~DAbstractStackWidgetTransitionPrivate();

    QVariantAnimation *animation;
    DAbstractStackWidgetTransition::TransitionInfo info;

    void init();

    D_DECLARE_PUBLIC(DAbstractStackWidgetTransition)
};

class DStackWidgetPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    DStackWidgetPrivate(DStackWidget *qq);
    ~DStackWidgetPrivate();

    int currentIndex;
    QWidget *currentWidget;
    QList<QWidget*> widgetList;
    QList<QWidget*> trashWidgetList;
    DAbstractStackWidgetTransition* transition;

    void init();
    void setCurrentIndex(int index);

    D_DECLARE_PUBLIC(DStackWidget)
};

DWIDGET_END_NAMESPACE

#endif // DSTACKWIDGET_P

