// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DSTYLEDITEMDELEGATE_H
#define DSTYLEDITEMDELEGATE_H

#include <dtkwidget_global.h>
#include <DObject>
#include <DPalette>
#include <DFontSizeManager>

#include <QAction>
#include <QStyledItemDelegate>
#include <QStandardItem>
#include <QAbstractItemView>

DGUI_BEGIN_NAMESPACE
class DDciIcon;
DGUI_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

class DViewItemActionPrivate;
class DViewItemAction : public QAction, public DCORE_NAMESPACE::DObject
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DViewItemAction)

public:
    explicit DViewItemAction(Qt::Alignment alignment = Qt::Alignment(), const QSize &iconSize = QSize(),
                             const QSize &maxSize = QSize(), bool clickable = false);
    D_DECL_DEPRECATED explicit DViewItemAction(Qt::Alignment alignment, const QSize &iconSize,
                                               const QSize &maxSize, bool clickable, QObject *parent);

    Qt::Alignment alignment() const;
    QSize iconSize() const;
    QSize maximumSize() const;

    QMargins clickAreaMargins() const;
    void setClickAreaMargins(const QMargins &margins);

    void setTextColorRole(DPalette::ColorType role);
    void setTextColorRole(DPalette::ColorRole role);
    DPalette::ColorType textColorType() const;
    DPalette::ColorRole textColorRole() const;

    void setFontSize(DFontSizeManager::SizeType size);
    QFont font() const;

    bool isClickable() const;

    void setWidget(QWidget *widget);
    QWidget *widget() const;

    void setDciIcon(const DDciIcon &dciIcon);
    DDciIcon dciIcon() const;
};
typedef QList<DViewItemAction *> DViewItemActionList;

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
        NoBackground = 0,
        ClipCornerBackground = 1,
        RoundedBackground = 2,
        BackgroundType_Mask = 0xff,
        NoNormalState = 0x100
    };

    explicit DStyledItemDelegate(QAbstractItemView *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

    BackgroundType backgroundType() const;
    QMargins margins() const;
    QSize itemSize() const;
    int spacing() const;

public Q_SLOTS:
    void setBackgroundType(BackgroundType backgroundType);
    void setMargins(const QMargins margins);
    void setItemSize(QSize itemSize);
    void setItemSpacing(int spacing);

protected:
    void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const override;
    bool eventFilter(QObject *object, QEvent *event) override;
};

class DStandardItem : public QStandardItem
{
public:
    using QStandardItem::QStandardItem;
    virtual ~DStandardItem();

    void setActionList(Qt::Edge edge, const DViewItemActionList &list);
    DViewItemActionList actionList(Qt::Edge edge) const;

    void setTextActionList(const DViewItemActionList &list);
    DViewItemActionList textActionList() const;

    void setTextColorRole(DPalette::ColorType role);
    void setTextColorRole(DPalette::ColorRole role);
    DPalette::ColorType textColorType() const;
    DPalette::ColorRole textColorRole() const;

    void setBackgroundRole(DPalette::ColorType role);
    void setBackgroundRole(DPalette::ColorRole role);
    DPalette::ColorType backgroundType() const;
    DPalette::ColorRole backgroundRole() const;

    void setFontSize(DFontSizeManager::SizeType size);
    QFont font() const;

    void setDciIcon(const DDciIcon &dciIcon);
    DDciIcon dciIcon() const;

    virtual QStandardItem *clone() const override;
};

DWIDGET_END_NAMESPACE

Q_DECLARE_METATYPE(DTK_WIDGET_NAMESPACE::DViewItemActionList)

#endif // DSTYLEDITEMDELEGATE_H
