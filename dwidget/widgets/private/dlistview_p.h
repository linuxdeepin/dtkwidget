/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DLISTVIEW_P_H
#define DLISTVIEW_P_H

#include "dobject_p.h"
#include "dlistview.h"

DWIDGET_NAMESPACE_BEGIN

class DListItemCreator;
class DBoxWidget;
class DListViewPrivate : public DObjectPrivate
{
    DListViewPrivate(DListView *qq);
    ~DListViewPrivate();

    void init();

    void onRowsInserted(const QModelIndex & parent, int first, int last);
    void onRowsAboutToBeRemoved(const QModelIndex & parent, int first, int last);
    void onDataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight,
                          const QVector<int> & roles = QVector<int> ());
    void onOrientationChanged();

    void _q_updateIndexWidget();
    void _q_onItemPaint(const QStyleOptionViewItem &option, const QModelIndex &index);

    int cacheBuffer = 0;
    int cacheCount = 0;

    /// Will be updated within the geometry QModelIndex.
    int startIndex = INT_MAX;
    int endIndex = -1;

    /// prevent q::paintEvent and _q_delayUpdateIndexWidget loop calls
    bool indexWidgetUpdated = false;

    /// contain widget QModelIndex row number list.
    QMap<int, QWidget*> indexToWidgetMap;
    DListItemCreator *creator = nullptr;
    DBoxWidget *headerLayout = nullptr;
    DBoxWidget *footerLayout = nullptr;

    QList<QWidget*> headerList;
    QList<QWidget*> footerList;

#if(QT_VERSION < 0x050500)
    int left = 0, top = 0, right = 0, bottom = 0; // viewport margin
#endif

    D_DECLARE_PUBLIC(DListView)
};

DWIDGET_NAMESPACE_END

#endif // DLISTVIEW_P_H

