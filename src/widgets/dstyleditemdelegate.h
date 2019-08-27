/*
 * Copyright (C) 2017 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
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
#ifndef DSTYLEDITEMDELEGATE_H
#define DSTYLEDITEMDELEGATE_H

#include <dtkwidget_global.h>
#include <DObject>
#include <DPalette>

#include <QAction>
#include <QStyledItemDelegate>
#include <QStandardItem>
#include <QAbstractItemView>

DWIDGET_BEGIN_NAMESPACE

class DViewItemActionPrivate;
class DViewItemAction : public QAction, public DCORE_NAMESPACE::DObject
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DViewItemAction)

public:
    explicit DViewItemAction(Qt::Alignment alignment = Qt::Alignment(), const QSize &iconSize = QSize(),
                             const QSize &maxSize = QSize(), bool clickable = false, QObject *parent = nullptr);

    Qt::Alignment alignment() const;
    QSize iconSize() const;
    QSize maximumSize() const;

    void setTextColorRole(DPalette::ColorType role);
    void setTextColorRole(DPalette::ColorRole role);
    DPalette::ColorType textColorType() const;
    DPalette::ColorRole textColorRole() const;

    void setFontSize(DFontSizeManager::SizeType size);
    QFont font() const;

    bool isClickable() const;

    void setWidget(QWidget *widget);
    QWidget *widget() const;
};
typedef QList<DViewItemAction*> DViewItemActionList;

class DStyledItemDelegatePrivate;
class DStyledItemDelegate : public QStyledItemDelegate, public DCORE_NAMESPACE::DObject
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DStyledItemDelegate)

    Q_PROPERTY(BackgroundType backgroundType READ backgroundType WRITE setBackgroundType)
    Q_PROPERTY(QMargins margins READ margins WRITE setMargins)
    Q_PROPERTY(QSize itemSize READ itemSize WRITE setItemSize)

public:
    enum BackgroundType {
        NoBackground,
        ClipCornerBackground,
        RoundedBackground
    };

    explicit DStyledItemDelegate(QAbstractItemView *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    BackgroundType backgroundType() const;
    QMargins margins() const;
    QSize itemSize() const;

public Q_SLOTS:
    void setBackgroundType(BackgroundType backgroundType);
    void setMargins(const QMargins margins);
    void setItemSize(QSize itemSize);

protected:
    void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const;
    bool eventFilter(QObject *object, QEvent *event) override;
};

class DStandardItem : public QStandardItem
{
public:
    using QStandardItem::QStandardItem;

    void setActionList(Qt::Edge edge, const DViewItemActionList &list);
    DViewItemActionList actionList(Qt::Edge edge) const;

    void setTextActionList(const DViewItemActionList &list);
    DViewItemActionList textActionList() const;
};

DWIDGET_END_NAMESPACE

Q_DECLARE_METATYPE(DTK_WIDGET_NAMESPACE::DViewItemActionList)

#endif // DSTYLEDITEMDELEGATE_H
