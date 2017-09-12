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

#ifndef DLISTWIDGET_P_H
#define DLISTWIDGET_P_H

#include "dscrollarea_p.h"
#include "dlistwidget.h"
#include "dboxwidget.h"

class QVBoxLayout;

DWIDGET_BEGIN_NAMESPACE

class DListWidgetPrivate : public DScrollAreaPrivate
{
protected:
    DListWidgetPrivate(DListWidget *qq);
    ~DListWidgetPrivate();

private:
    void init();

    int itemWidth;
    int itemHeight;
    QList<QWidget*> widgetList;
    QList<int> checkedList;
    DListWidget::CheckMode checkMode;
    DBoxWidget *mainWidget;
    int visibleCount;
    QMap<const QWidget*, bool> mapVisible;
    bool checkable;
    bool toggleable;
    bool enableHorizontalScroll;
    bool enableVerticalScroll;

    D_DECLARE_PUBLIC(DListWidget)
};

DWIDGET_END_NAMESPACE

#endif // DLISTWIDGET_P_H

