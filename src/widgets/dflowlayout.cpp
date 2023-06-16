// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QStyle>
#include <QDebug>

#include "dflowlayout.h"
#include "private/dflowlayout_p.h"

DWIDGET_BEGIN_NAMESPACE

DFlowLayoutPrivate::DFlowLayoutPrivate(DFlowLayout *qq) :
    DObjectPrivate(qq)
{

}

QSize DFlowLayoutPrivate::doLayout(const QRect &rect, bool testOnly) const
{
    D_QC(DFlowLayout);

    if(!testOnly) {
        if(!q->parentWidget() || q->parentWidget()->isHidden())
            return QSize();
    } else if(rect.width() <= 0){
        return QSize();
    }

    int left, top, right, bottom;

    q->getContentsMargins(&left, &top, &right, &bottom);
    QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);

    int x = q->parentWidget()->layoutDirection() == Qt::RightToLeft
            ? effectiveRect.right()
            :effectiveRect.x();
    int y = effectiveRect.y();

    QSize size_hint;

    if(flow == DFlowLayout::Flow::LeftToRight) {
        int maxWidth = 0;
        int lineHeight = 0;

        if(q->parentWidget()->layoutDirection() == Qt::RightToLeft) {
            for (QLayoutItem *item : itemList) {
                if (item->isEmpty())
                    continue;
                // QRect's x2 = x1 + width - 1
                int nextX = x - item->sizeHint().width() - horizontalSpacing + 1;

                if (nextX + horizontalSpacing < effectiveRect.x() && lineHeight > 0) {
                    x = effectiveRect.right();
                    y = y + lineHeight + verticalSpacing;
                    nextX = x - item->sizeHint().width() - horizontalSpacing + 1;
                    lineHeight = 0;
                }

                if (!testOnly) {
                    QRect item_geometry;

                    item_geometry.setSize(item->sizeHint());
                    item_geometry.moveTopRight(QPoint(x, y));
                    item->setGeometry(item_geometry);
                }

                x = nextX;
                // QRect's width = x2 - x1 + 1
                maxWidth = qMax(effectiveRect.right() - nextX - horizontalSpacing + 1, maxWidth);
                lineHeight = qMax(lineHeight, item->sizeHint().height());
            }

            size_hint = QSize(maxWidth, y + lineHeight - rect.y() + bottom);
        } else {
            for (QLayoutItem *item : itemList) {
                if (item->isEmpty())
                    continue;
                // QRect's x2 = x1 + width - 1
                int nextX = x + item->sizeHint().width() + horizontalSpacing - 1;

                if (nextX - horizontalSpacing > effectiveRect.right() && lineHeight > 0) {
                    x = effectiveRect.x();
                    y = y + lineHeight + verticalSpacing;
                    nextX = x + item->sizeHint().width() + horizontalSpacing - 1;
                    lineHeight = 0;
                }

                if (!testOnly)
                    item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));

                x = nextX;
                // QRect's width = x2 - x1 + 1
                maxWidth = qMax(nextX - effectiveRect.x() - horizontalSpacing + 1, maxWidth);
                lineHeight = qMax(lineHeight, item->sizeHint().height());
            }

            size_hint = QSize(maxWidth, y + lineHeight - rect.y() + bottom);
        }
    } else {
        int maxHeight = 0;
        int lineWidth = 0;

        if(q->parentWidget()->layoutDirection() == Qt::RightToLeft) {
            for (QLayoutItem *item : itemList) {
                if (item->isEmpty())
                    continue;
                // QRect's y2 = y1 + height - 1
                int nextY = y + item->sizeHint().height() + verticalSpacing - 1;

                if(nextY - verticalSpacing > effectiveRect.bottom() && lineWidth > 0) {
                    y = effectiveRect.y();
                    x = x - lineWidth - horizontalSpacing;
                    nextY = y + item->sizeHint().height() + verticalSpacing - 1;
                    lineWidth = 0;
                }

                if (!testOnly)
                    item->setGeometry(QRect(QPoint(x - item->sizeHint().width(), y), item->sizeHint()));

                y = nextY;
                // height = y2 - y1 + 1
                maxHeight = qMax(nextY - effectiveRect.y() - verticalSpacing + 1, maxHeight);
                lineWidth = qMax(lineWidth, item->sizeHint().width());
            }

            size_hint = QSize(rect.right() - x + lineWidth + right + 1, maxHeight);
        } else {
            for (QLayoutItem *item : itemList) {
                if (item->isEmpty())
                    continue;
                // QRect's x2 = x1 + width - 1
                int nextY = y + item->sizeHint().height() + verticalSpacing - 1;

                if(nextY - verticalSpacing > effectiveRect.bottom() && lineWidth > 0) {
                    y = effectiveRect.y();
                    x = x + lineWidth + horizontalSpacing;
                    nextY = y + item->sizeHint().height() + verticalSpacing - 1;
                    lineWidth = 0;
                }

                if (!testOnly)
                    item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));

                y = nextY;
                // height = y2 - y1 + 1
                maxHeight = qMax(nextY - effectiveRect.y() - verticalSpacing + 1, maxHeight);
                lineWidth = qMax(lineWidth, item->sizeHint().width());
            }

            size_hint = QSize(x + lineWidth - rect.x() + right, maxHeight);
        }
    }

    if(!testOnly) {
        if(sizeHint != size_hint) {
            sizeHint = size_hint;
            Q_EMIT q->sizeHintChanged(sizeHint);
        }
    }

    return size_hint;
}

/*!
    \class Dtk::Widget::DFlowLayout
    \inmodule dtkwidget

    \brief DFlowLayout 类提供了一个简便的流布局.
    \brief The DFlowLayout class provides a flow list layout.

    \image html dflowlayout.png

    上方图片是一个 DFlowLayout 的典型应用场景。注意示例图中的标题文本并不是 DFlowLayout 的一部分。
    The above image is a classic use case of DFlowLayout. Notably the title text label in
    the example is not a part of DFlowLayout.
*/

DFlowLayout::DFlowLayout(QWidget *parent) :
    QLayout(parent),
    DObject(*new DFlowLayoutPrivate(this))
{

}

DFlowLayout::DFlowLayout() :
    QLayout(),
    DObject(*new DFlowLayoutPrivate(this))
{

}

DFlowLayout::~DFlowLayout()
{
    QLayoutItem *item;

    while ((item = takeAt(0)))
        delete item;
}

/*!
  \brief 向布局中插入一个 QLayoutItem.
  \brief Insert item to layout.
  
  \a index 插入到的位置
  \a item 待插入的 QLayoutItem
  
  \a index to be insert
  \a item the QLayoutItem to be insert
  
  \sa addItem()
 */
void DFlowLayout::insertItem(int index, QLayoutItem *item)
{
    d_func()->itemList.insert(index, item);

    Q_EMIT countChanged(count());
}

/*!
  \brief 向布局中插入一个 QWidget.
  \brief Insert widget to layout.
  
  \a index 插入到的位置
  \a widget 待插入的 QWidget
  
  \a index to be insert
  \a widget the QWidget to be insert
 */
void DFlowLayout::insertWidget(int index, QWidget *widget)
{
    addChildWidget(widget);
    insertItem(index, new QWidgetItemV2(widget));
}

/*!
  \brief 向布局中插入一个 QLayout 子布局.
  \brief Insert a sub-layout to layout.
  
  \a index 插入到的位置
  \a layout 待插入的 QLayout
  
  \a index to be insert
  \a layout the QLayout to be insert
 */
void DFlowLayout::insertLayout(int index, QLayout *layout)
{
    addChildLayout(layout);
    insertItem(index, layout);
}

/*!
  \brief 向布局中插入一个指定大小的占位符（QSpacerItem）.
  \brief Insert a fixed \a size QSpacerItem to layout.
  
  \a index 插入到的位置
  \a size 待插入的 QSpacerItem 的大小
  
  \a index to be insert
  \a size of the QSpacerItem to be insert
 */
void DFlowLayout::insertSpacing(int index, int size)
{
    QSpacerItem *b;

    b = new QSpacerItem(size, 0, QSizePolicy::Fixed, QSizePolicy::Minimum);

    insertSpacerItem(index, b);
}

/*!
  \brief 向布局中插入一个扩充大小的占位符（QSpacerItem）.
  \brief Insert a expanding QSpacerItem to layout.
  
  \a index 插入到的位置
  \a stretch 没有作用的参数
  
  \a index to be insert
  \a stretch Unused argument
 */
void DFlowLayout::insertStretch(int index, int stretch)
{
    Q_UNUSED(stretch);

    QSpacerItem *b;

    b = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    insertSpacerItem(index, b);
}

/*!
  \brief 向布局中插入一个占位符（QSpacerItem）.
  \brief Insert a fixed size QSpacer to layout.
  
  \a index 插入到的位置
  \a spacerItem 待插入的 QSpacerItem
  
  \a index to be insert
  \a spacerItem to be insert
  
  \sa addSpacerItem()
 */
void DFlowLayout::insertSpacerItem(int index, QSpacerItem *spacerItem)
{
    insertItem(index, spacerItem);
}

/*!
  \brief 向布局中追加一个固定大小的占位符（QSpacerItem）.
  \brief Insert a fixed \a size QSpacerItem to layout.
  
  \a size 占位符大小
  \a size of the QSpacerItem to be insert
  
  \sa insertSpacing()
 */
void DFlowLayout::addSpacing(int size)
{
    insertSpacing(count(), size);
}

/*!
  \brief 向布局中追加一个扩充大小的占位符（QSpacerItem）.
  \brief Insert a expanding QSpacerItem to layout.
  
  \a stretch 没有作用的参数
  \a stretch Unused argument.
  
  \sa insertStretch()
 */
void DFlowLayout::addStretch(int stretch)
{
    insertStretch(count(), stretch);
}

/*!
  \brief 向布局中追加一个的占位符（QSpacerItem）.
  \brief Insert a QSpacerItem to layout.
  
  \a spacerItem 待插入的 QSpacerItem
  \a spacerItem to be insert.
  
  \sa insertSpacerItem()
 */
void DFlowLayout::addSpacerItem(QSpacerItem *spacerItem)
{
    insertSpacerItem(count(), spacerItem);
}

/*!
  \brief 向布局中追加一个 QLayoutItem.
  \brief Insert a QLayoutItem to layout.
  
  \a item 待插入的 QLayoutItem
  \a item to be insert.
  
  \sa insertItem()
 */
void DFlowLayout::addItem(QLayoutItem *item)
{
    insertItem(count(), item);
}

/*!
  \brief 是否可以有固定高度.
  \brief check if layout can have a fixed height.

  \return 可以通过高度获取宽度返回 true，否则返回 false.
  
  \sa heightForWidth()
 */
bool DFlowLayout::hasHeightForWidth() const
{
    D_DC(DFlowLayout);

    return d->flow == DFlowLayout::Flow::LeftToRight;
}

/*!
  \brief 根据指定宽度获得对应高度.
  \brief set fixed height.
  \a width 宽度.
  \return 指定高度.
  
  \sa heightForWidth()
 */
int DFlowLayout::heightForWidth(int width) const
{
    D_DC(DFlowLayout);

    QWidget *parentWidget = this->parentWidget();

    if(parentWidget && width == parentWidget->width()) {
        return d->sizeHint.height();
    }

    return d->doLayout(QRect(0, 0, width, 0), true).height();
}

/*!
  \brief 布局内的元素数量.
  \brief item count in layout.
 */
int DFlowLayout::count() const
{
    return d_func()->itemList.count();
}

QLayoutItem *DFlowLayout::itemAt(int index) const
{
    return d_func()->itemList.value(index);
}

/*
  \reimp
 */
QSize DFlowLayout::minimumSize() const
{
    D_DC(DFlowLayout);

    QSize size;

    for (QLayoutItem *item : d->itemList)
        size = size.expandedTo(item->minimumSize());
    auto margin = this->contentsMargins();
    size += QSize(2 * (margin.right() + margin.left()), 2 * (margin.top() + margin.bottom()));

    return size;
}

/*
  \reimp
 */
void DFlowLayout::setGeometry(const QRect &rect)
{
    if(rect == geometry())
        return;

    QLayout::setGeometry(QRect(rect.topLeft(),
                               d_func()->doLayout(rect, false)));
}

/*
  \reimp
 */
QSize DFlowLayout::sizeHint() const
{
    return d_func()->sizeHint;
}

/*!
  \brief 移除并取得 \a index 位置的对应元素.
  \brief take the QLayoutItem at the given \a index.
 */
QLayoutItem *DFlowLayout::takeAt(int index)
{
    D_D(DFlowLayout);

    if (index < 0 || index >= d->itemList.count()) {
        return 0;
    }

    QLayoutItem *item = d->itemList.takeAt(index);

    if (QLayout *l = item->layout()) {
        // sanity check in case the user passed something weird to QObject::setParent()
        if (l->parent() == this)
            l->setParent(0);
    }

    Q_EMIT countChanged(count());

    return item;
}

Qt::Orientations DFlowLayout::expandingDirections() const
{
    switch (d_func()->flow) {
    case DFlowLayout::Flow::LeftToRight:
        return Qt::Horizontal;
    case DFlowLayout::Flow::TopToBottom:
        return Qt::Vertical;
    }
    return QLayout::expandingDirections();
}

/*!
  \brief 取得横横横向的元素疏密度（Spacing）.
  \brief get flow spacing in horizontal direction.
  
  \return 返回水平方向的间距（疏密度）.
  \sa setHorizontalSpacing()
 */
int DFlowLayout::horizontalSpacing() const
{
    return d_func()->horizontalSpacing;
}

/*!
  \brief 取得纵横横向的元素疏密度（Spacing）.
  \brief get flow spacing in vertical direction.

  \return 返回竖直方向的间距（疏密度）.
  \sa setVerticalSpacing()
 */
int DFlowLayout::verticalSpacing() const
{
    return d_func()->verticalSpacing;
}

/*!
  \brief 取得流布局的方向
  \brief get flow direction.

  \return 返回当前流布局的方向.
  \sa setFlow()
 */
DFlowLayout::Flow DFlowLayout::flow() const
{
    D_DC(DFlowLayout);

    return d->flow;
}

/*!
  \brief 设置纵横横向的元素疏密度（Spacing）.
  \brief set flow spacing in horizontal direction.
  
  \a horizontalSpacing 水平间距值

  \sa setSpacing(), setVerticalSpacing()
 */
void DFlowLayout::setHorizontalSpacing(int horizontalSpacing)
{
    D_D(DFlowLayout);

    if(horizontalSpacing == d->horizontalSpacing)
        return;

    d->horizontalSpacing = horizontalSpacing;

    Q_EMIT horizontalSpacingChanged(horizontalSpacing);

    invalidate();
}

/*!
  \brief 设置纵横纵向的元素疏密度（Spacing）
  \brief set flow \a verticalSpacing in vertical direction.

  \a verticalSpacing 竖直方向的间距值.
  
  \sa setSpacing(), setHorizontalSpacing()
 */
void DFlowLayout::setVerticalSpacing(int verticalSpacing)
{
    D_D(DFlowLayout);

    if(verticalSpacing == d->verticalSpacing)
        return;

    d->verticalSpacing = verticalSpacing;

    Q_EMIT verticalSpacingChanged(verticalSpacing);

    invalidate();
}

/*!
  \brief 设置纵横两个方向的元素疏密度（Spacing）
  \brief set flow \a spacing in horizontal and vertical direction.

  \a spacing 两个方向的间距值.
  
  \sa setVerticalSpacing(), setHorizontalSpacing()
 */
void DFlowLayout::setSpacing(int spacing)
{
    setHorizontalSpacing(spacing);
    setVerticalSpacing(spacing);
}

/*!
  \brief 设置布局方向
  \brief set flow \a direction
  
  \sa QListView::Flow, flow()
 */
void DFlowLayout::setFlow(Flow direction)
{
    D_D(DFlowLayout);

    if (d->flow == direction)
        return;

    d->flow = direction;
    Q_EMIT flowChanged(direction);

    invalidate();
}

DWIDGET_END_NAMESPACE
