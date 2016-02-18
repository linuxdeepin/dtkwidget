/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DLISTVIEW_H
#define DLISTVIEW_H

#include <QStyledItemDelegate>
#include <QListView>

#include "dobject.h"

DWIDGET_BEGIN_NAMESPACE

class DVariantListModel : public QAbstractListModel
{
public:
    explicit DVariantListModel(QObject *parent = 0);

    int rowCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;

private:
    QList<QVariant> dataList;
};

class LIBDTKWIDGETSHARED_EXPORT DListItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit DListItemDelegate(QObject *parent = 0);

    void paint(QPainter *,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const Q_DECL_OVERRIDE
    {Q_EMIT itemPaint(option, index);}

    virtual QWidget *createWidget(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    virtual void setWidgetData(QWidget *widget, const QModelIndex &index) const;

    virtual void updateWidgetGeometry(QWidget *widget,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const;

Q_SIGNALS:
    void itemPaint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class DListViewPrivate;
class LIBDTKWIDGETSHARED_EXPORT DListView : public QListView, public DObject
{
    Q_OBJECT

    /// see qml ListView cacheBuffer property.
    Q_PROPERTY(int cacheBuffer READ cacheBuffer WRITE setCacheBuffer NOTIFY cacheBufferChanged)
    /// item count.
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    /// list layout orientation
    Q_PROPERTY(Qt::Orientation orientation READ orientation NOTIFY orientationChanged)

public:
    explicit DListView(QWidget *parent = 0);

    void setRootIndex(const QModelIndex &index) Q_DECL_OVERRIDE;
    DListItemDelegate *itemDelegate() const;
    void setItemDelegate(DListItemDelegate *delegate);

    inline QWidget *indexWidget(const QModelIndex &index) const
    {return getWidget(index.row());}
    QWidget *getWidget(int index) const;
    QVariant getItemData(int index) const;
    QModelIndex getIndexByWidget(const QWidget *widget) const;

    QWidget *getHeaderWidget(int index) const;
    QWidget *getFooterWidget(int index) const;

    /// return true if rect intersects contentsVisualRect+qMax(cacheBuffer,cacheCount)
    bool isActiveRect(const QRect &rect) const;
    bool isVisualRect(const QRect &rect) const;

    int cacheBuffer() const;
    int count() const;

    Qt::Orientation orientation() const;

public Q_SLOTS:
    void setIndexWidget(const QModelIndex &index, QWidget *widget);

    bool addItem(const QVariant &data);
    bool addItems(const QVariantList &datas);
    bool insertItem(int index, const QVariant &data);
    bool insertItems(int index, const QVariantList &datas);
    bool removeItem(int index);
    bool removeItems(int index, int count);
    void clear();

    int addHeaderWidget(QWidget *widget);
    void removeHeaderWidget(int index);
    QWidget *takeHeaderWidget(int index);
    int addFooterWidget(QWidget *widget);
    void removeFooterWidget(int index);
    QWidget *takeFooterWidget(int index);

    void setOrientation(QListView::Flow flow, bool wrapping);
    void setCacheBuffer(int cacheBuffer);

Q_SIGNALS:
    void cacheBufferChanged(int cacheBuffer);
    void countChanged(int count);
    void orientationChanged(Qt::Orientation orientation);

protected:
#if(QT_VERSION < 0x050500)
    void setViewportMargins(int left, int top, int right, int bottom);
    void setViewportMargins(const QMargins &margins);
    QMargins viewportMargins() const;
#endif

    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    void dataChanged(const QModelIndex &topLeft,
                     const QModelIndex &bottomRight,
                     const QVector<int> &roles = QVector<int>()) Q_DECL_OVERRIDE;
    void rowsInserted(const QModelIndex &parent, int start, int end) Q_DECL_OVERRIDE;
    void rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end) Q_DECL_OVERRIDE;

private:
    void setFlow(QListView::Flow flow);
    void setWrapping(bool enable);

    D_DECLARE_PRIVATE(DListView)
    Q_PRIVATE_SLOT(d_func(), void _q_updateIndexWidget())
    Q_PRIVATE_SLOT(d_func(), void _q_onItemPaint(const QStyleOptionViewItem&, const QModelIndex&))
};

DWIDGET_END_NAMESPACE

#endif // DLISTVIEW_H
