/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DLISTWIDGET_P_H
#define DLISTWIDGET_P_H

#include "dscrollarea_p.h"
#include "dlistwidget.h"
#include "dboxwidget.h"

class QVBoxLayout;

DWIDGET_NAMESPACE_BEGIN

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

DWIDGET_NAMESPACE_END

#endif // DLISTWIDGET_P_H

