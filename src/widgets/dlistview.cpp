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

#include <QDebug>
#include <QScrollBar>

#include "dboxwidget.h"
#include "dlistview.h"
#include "private/dlistview_p.h"
#include "dflowlayout.h"
#include "dstyleoption.h"
#include "dstyleditemdelegate.h"
#include "dstyle.h"

DWIDGET_BEGIN_NAMESPACE

DVariantListModel::DVariantListModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

int DVariantListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return dataList.count();
}

QVariant DVariantListModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role);

    return dataList.value(index.row());
}

bool DVariantListModel::setData(const QModelIndex &index,
                                const QVariant &value, int role)
{
    if (index.row() >= 0 && index.row() < dataList.size()) {
        dataList.replace(index.row(), value);
        Q_EMIT dataChanged(index, index, QVector<int>() << role);

        return true;
    }

    return false;
}

bool DVariantListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (count < 1 || row < 0 || row > rowCount(parent))
        return false;

    beginInsertRows(QModelIndex(), row, row + count - 1);

    for (int r = 0; r < count; ++r)
        dataList.insert(row, QVariant());

    endInsertRows();

    return true;
}

bool DVariantListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (count <= 0 || row < 0 || (row + count) > rowCount(parent))
        return false;

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    for (int r = 0; r < count; ++r)
        dataList.removeAt(row);

    endRemoveRows();

    return true;
}

DListViewPrivate::DListViewPrivate(DListView *qq) :
    DObjectPrivate(qq)
{

}

DListViewPrivate::~DListViewPrivate()
{

}

void DListViewPrivate::init()
{
    D_Q(DListView);

    q->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    q->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    DStyledItemDelegate *delegate = new DStyledItemDelegate(q);
    q->setItemDelegate(delegate);

    q->setFrameShape(QFrame::NoFrame);

    q->viewport()->setAutoFillBackground(false);
    q->setAutoFillBackground(false);
    q->setBackgroundRole(q->viewport()->backgroundRole());

    // 默认背景类型为圆角背景
    q->setBackgroundType(DStyledItemDelegate::RoundedBackground);
}

void DListViewPrivate::onOrientationChanged()
{
    D_Q(DListView);

    bool isVerticalLayout = (q->orientation() == Qt::Vertical);

    if (headerLayout) {
        headerLayout->setDirection(isVerticalLayout
                                   ? QBoxLayout::TopToBottom
                                   : QBoxLayout::LeftToRight);
        if (isVerticalLayout) {
            headerLayout->resize(q->width(), headerLayout->sizeHint().height());
        } else {
            headerLayout->resize(headerLayout->sizeHint().width(), q->height());
        }

        headerLayout->layout()->update();
    }

    if (footerLayout) {
        footerLayout->setDirection(isVerticalLayout
                                   ? QBoxLayout::TopToBottom
                                   : QBoxLayout::LeftToRight);

        if (isVerticalLayout) {
            footerLayout->resize(q->width(), footerLayout->sizeHint().height());
            footerLayout->move(0, q->height() - footerLayout->height());
        } else {
            footerLayout->resize(footerLayout->sizeHint().width(), q->height());
            footerLayout->move(q->width() - footerLayout->width(), 0);
        }

        footerLayout->layout()->update();
    }
}

// ====================Signals begin====================
/**
 * \~chinese \fn DListView::currentChanged
 * \~chinese \brief 这个信号当当前item发生改变时被调用
 *
 * \~chinese listview会有一个始终表示当前item索引的 QModelIndex 对象，
 * \~chinese 当这个 QModelIndex 对象表示的位置发生改变时这个信号才会被调用，而不是当前item的内容发生改变时。
 * \~chinese 当鼠标单机某一个item或者使用键盘切换item时，
 *
 * \~chinese \param previous 为之前的item的索引对象
 *
 * \~chinese \sa QModelIndex QListView::currentChanged
 */

/**
 * \~chinese \fn DListView::triggerEdit
 * \~chinese \brief 这个信号当有新的item被编辑时被调用
 *
 * \~chinese \param index 为正在编辑的item的索引对象
 *
 * \~chinese \sa QModelIndex QAbstractItemView::EditTrigger
 */
// ====================Signals end====================

/**
 * \~chinese \class DListView
 * \~chinese \brief 一个用于展示一列数据的控件
 *
 * \~chinese DListView 类似与 QListView 属于 Qt's model/view framework 的一个类，常被用来展示一列数据，当数据较多时可以滚动控件以显示跟多内容。
 * \~chinese 但与 QListView 也有不同之处，DListView 提供了顶部控件和底部控件，它们始终显示在listview中，不会因为滚动而不可见，另外还提供了方便编辑
 * \~chinese 数据的方法，如：addItem , addItems , insertItem , takeItem , removeItem , 以及一些开发中常用的信号。
 */

/**
 * \~chinese \brief 获取一个 DListView 实例
 * \~chinese \param parent 被用来作为 DListView 实例的父控件
 */
DListView::DListView(QWidget *parent) :
    QListView(parent),
    DObject(*new DListViewPrivate(this))
{
    d_func()->init();
}

/**
 * \~chinese \brief 获取控件当前的状态
 *
 * \~chinese 控件可以有正在被拖拽，正在被编辑，正在播放动画等状态，详细可以查阅：QAbstractItemView::State
 *
 * \~chinese \return 控件当前的状态
 *
 * \~chinese \sa QAbstractItemView::State
 */
QAbstractItemView::State DListView::state() const
{
    return QListView::state();
}

void DListView::setFlow(QListView::Flow flow)
{
    QListView::setFlow(flow);
}

void DListView::setWrapping(bool enable)
{
    QListView::setWrapping(enable);
}

QSize DListView::viewportSizeHint() const
{
    return sizeAdjustPolicy() == QAbstractScrollArea::AdjustToContents ? QSize(0, 0) : QListView::viewportSizeHint();
}

QSize DListView::minimumSizeHint() const
{
    QSize size = QListView::minimumSizeHint();
    QSize content_size = QListView::contentsSize();

    if (horizontalScrollBarPolicy() == Qt::ScrollBarAlwaysOff) {
        size.setWidth(content_size.width());
    }

    if (verticalScrollBarPolicy() == Qt::ScrollBarAlwaysOff) {
        size.setHeight(content_size.height());
    }

    return size;
}

/*!
 * \~chinese \brief 获取一个顶部控件
 *
 * \~chinese 顶部控件与item一样都会在listview中被显示出来，而且顶部控件会始终在所有item之上，
 * \~chinese 也就是说顶部控件与item不同的地方在于顶部控件始终显示在布局中，而不会因为鼠标滚动不可见。
 * \~chinese 另外顶部控件可以有多个，它们的布局方式(方向)与item的布局方向相同
 *
 * \~chinese \param index 指定要获取的顶部控件的索引
 * \~chinese \return 返回在指定索引处的顶部控件对象
 *
 * \~chinese \note 注意顶部控件并不是像 GridLayout 的表头，表头是始终在水平方向上布局的
 *
 * \~chinese \sa DListView::getFooterWidget DListView::addHeaderWidget DListView::removeHeaderWidget DListView::takeHeaderWidget
 */
QWidget *DListView::getHeaderWidget(int index) const
{
    return d_func()->headerList.value(index);
}

/*!
 * \~chinese \brief 获取一个底部控件
 * \~chinese \param index 指定要获取的底部控件的索引
 * \~chinese \return 返回在指定索引处的底部控件对象
 * \~chinese \sa DListView::getHeaderWidget
 */
QWidget *DListView::getFooterWidget(int index) const
{
    return d_func()->footerList.value(index);
}

/*!
 * \~chinese \brief 判断给定的 QRect 是否与 listview 的item可显示区域有重叠
 *
 * \~chinese listview 的item可显示区域即为 listview 的 viewport ， items只能在 viewport 显示，超出这一区域的 item 将不可见。
 *
 * \~chinese \param rect 要对比的 QRect
 * \~chinese \return 返回两个矩形是否有重叠区域
 * \~chinese \sa DListView::isVisualRect
 */
bool DListView::isActiveRect(const QRect &rect) const
{
//    D_DC(DListView);

    const QRect &area = viewport()->geometry();

    return area.intersects(rect);
}

/*!
 * \~chinese \brief 与 DListView::isVisualRect 相同
 * \~chinese \sa DListView::isVisualRect
 */
bool DListView::isVisualRect(const QRect &rect) const
{
    const QRect &area = viewport()->geometry();

    return area.intersects(rect);
}

/**
 * \~chinese \fn DListView::rowCountChanged
 * \~chinese \sa DListView::count
 */

/**
 * \~chinese \property DListView::count
 * \~chinese \brief 这个属性保存共有多少行数据
 *
 * \~chinese Getter: DListView::count , Signal: DListView::rowCountChanged
 */
int DListView::count() const
{
    return model()->rowCount(rootIndex());
}

/**
 * \~chinese \fn DListView::orientationChanged
 * \~chinese \sa DListView::orientation
 */

/**
 * \~chinese \property DListView::orientation
 * \~chinese \brief 这个属性保存listview中item的布局方式
 *
 * \~chinese Getter: DListView::orientation , Setter: DListView::setOrientation , Signal: DListView::orientationChanged
 *
 * \~chinese \sa Qt::Orientation
 */
Qt::Orientation DListView::orientation() const
{
    bool isVerticalLayout = isWrapping()
                            ? flow() == QListView::LeftToRight
                            : flow() == QListView::TopToBottom;

    return isVerticalLayout ? Qt::Vertical : Qt::Horizontal;
}

/*!
 * \~chinese \brief 设置 DListView 要使用的模型
 *
 * \~chinese 模型用来为 listview 提供数据，以实现数据层与界面层分离的结构, 详细请查阅 Qt's model/view framework
 *
 * \~chinese \param model 模型对象
 * \~chinese \sa QListView::setModel
 */
void DListView::setModel(QAbstractItemModel *model)
{
    QAbstractItemModel *old_model = this->model();

    if (old_model) {
        disconnect(old_model, &QAbstractItemModel::rowsInserted, this, &DListView::rowCountChanged);
        disconnect(old_model, &QAbstractItemModel::rowsRemoved, this, &DListView::rowCountChanged);
    }

    QListView::setModel(model);

    model = this->model();

    if (model) {
        connect(model, &QAbstractItemModel::rowsInserted, this, &DListView::rowCountChanged);
        connect(model, &QAbstractItemModel::rowsRemoved, this, &DListView::rowCountChanged);
    }
}

DStyledItemDelegate::BackgroundType DListView::backgroundType() const
{
    if (DStyledItemDelegate *d = qobject_cast<DStyledItemDelegate *>(itemDelegate())) {
        return d->backgroundType();
    }

    return DStyledItemDelegate::NoBackground;
}

QMargins DListView::itemMargins() const
{
    if (DStyledItemDelegate *d = qobject_cast<DStyledItemDelegate *>(itemDelegate())) {
        return d->margins();
    }

    return QMargins();
}

QSize DListView::itemSize() const
{
    if (DStyledItemDelegate *d = qobject_cast<DStyledItemDelegate *>(itemDelegate())) {
        return d->itemSize();
    }

    return QSize();
}

/*!
 * \~chinese \brief 在列表底部新增一个item
 * \~chinese \param data 要新增的数据
 * \~chinese \return 返回是否新增成功
 */
bool DListView::addItem(const QVariant &data)
{
    return insertItem(count(), data);
}

/*!
 * \~chinese \brief 一次性在列表底部新增多个item
 * \~chinese \param datas 要新增的数据组成的列表
 * \~chinese \return 是否新增成功
 */
bool DListView::addItems(const QVariantList &datas)
{
    return insertItems(count(), datas);
}

/*!
 * \~chinese \brief 在指定行处新增一个item
 * \~chinese \param index 要增加item的行号
 * \~chinese \param data 要增加的item的数据
 * \~chinese \return 是否新增成功
 */
bool DListView::insertItem(int index, const QVariant &data)
{
    if (!model()->insertRow(index))
        return false;

    return model()->setData(model()->index(index, 0, rootIndex()), data);
}

/*!
 * \~chinese \brief 在指定行处新增多个item
 * \~chinese \param index 要增加item的行号
 * \~chinese \param datas 要增加的items的数据组成的列表
 * \~chinese \return 是否新增成功
 */
bool DListView::insertItems(int index, const QVariantList &datas)
{
    if (!model()->insertRows(index, datas.count()))
        return false;

    for (int i = 0; i < datas.count(); ++i)
        model()->setData(model()->index(index + i, 0, rootIndex()), datas.at(i));

    return true;
}

/*!
 * \~chinese \brief 移除指定位置的item
 * \~chinese \param index 要移除的item的行号
 * \~chinese \return 是否移除成功
 */
bool DListView::removeItem(int index)
{
    return model()->removeRow(index);
}

/*!
 * \~chinese \brief 一次移除多个item
 * \~chinese \param index 开始移除item的行号
 * \~chinese \param count 移除从 index 指定的行号开始，移除 count 个item
 * \~chinese \return 返回是否移除成功
 */
bool DListView::removeItems(int index, int count)
{
    return model()->removeRows(index, count);
}

/*!
 * \~chinese \sa DListView::getHeaderWidget
 */
int DListView::addHeaderWidget(QWidget *widget)
{
    D_D(DListView);

    int index = d->headerList.indexOf(widget);

    if (index >= 0)
        return index;

    if (!d->headerLayout) {
        bool isVerticalLayout = (orientation() == Qt::Vertical);

        d->headerLayout = new DBoxWidget(isVerticalLayout
                                         ? QBoxLayout::TopToBottom
                                         : QBoxLayout::LeftToRight, this);

        if (isVerticalLayout)
            d->headerLayout->resize(width(), d->headerLayout->height());
        else
            d->headerLayout->resize(d->headerLayout->width(), height());

        connect(d->headerLayout, &DBoxWidget::sizeChanged,
        this, [this](const QSize & size) {
            bool isVerticalLayout = (orientation() == Qt::Vertical);

            QMargins margins = this->viewportMargins();

            if (isVerticalLayout)
                margins.setTop(size.height());
            else
                margins.setLeft(size.width());

            setViewportMargins(margins);
        });

        d->headerLayout->show();
    }

    d->headerLayout->addWidget(widget);
    d->headerList << widget;

    return d->headerList.count() - 1;
}

/*!
 * \~chinese \sa DListView::getHeaderWidget
 */
void DListView::removeHeaderWidget(int index)
{
    QWidget *widget = takeHeaderWidget(index);

    if (widget)
        widget->deleteLater();
}

/*!
 * \brief 与 DListView::getHeaderWidget 类似，但返回要移除的顶部控件的对象
 * \~chinese \sa DListView::getHeaderWidget
 */
QWidget *DListView::takeHeaderWidget(int index)
{
    D_D(DListView);

    QWidget *widget = d->headerList.takeAt(index);

    d->headerLayout->layout()->removeWidget(widget);

    if (d->headerList.isEmpty()) {
        d->headerLayout->deleteLater();
        d->headerLayout = nullptr;
    }

    return widget;
}

/*!
 * \~chinese \sa DListView::getFooterWidget
 */
int DListView::addFooterWidget(QWidget *widget)
{
    D_D(DListView);

    int index = d->footerList.indexOf(widget);

    if (index >= 0)
        return index;

    if (!d->footerLayout) {
        bool isVerticalLayout = (orientation() == Qt::Vertical);

        d->footerLayout = new DBoxWidget(isVerticalLayout
                                         ? QBoxLayout::TopToBottom
                                         : QBoxLayout::LeftToRight, this);

        if (isVerticalLayout)
            d->footerLayout->resize(width(), d->footerLayout->height());
        else
            d->footerLayout->resize(d->footerLayout->width(), height());

        connect(d->footerLayout, &DBoxWidget::sizeChanged,
        this, [this](const QSize & size) {
            bool isVerticalLayout = (orientation() == Qt::Vertical);

            QMargins margins = this->viewportMargins();

            D_D(DListView);

            if (isVerticalLayout) {
                margins.setBottom(size.height());
                d->footerLayout->move(0, height() - d->footerLayout->height());
            } else {
                margins.setRight(size.width());
                d->footerLayout->move(width() - d->footerLayout->width(), 0);
            }

            setViewportMargins(margins);
        });

        d->footerLayout->show();
    }

    d->footerLayout->addWidget(widget);
    d->footerList << widget;

    return d->footerList.count() - 1;
}

/*!
 * \~chinese \sa DListView::getFooterWidget
 */
void DListView::removeFooterWidget(int index)
{
    QWidget *widget = takeFooterWidget(index);

    if (widget)
        widget->deleteLater();
}

/*!
 * \~chinese \sa DListView::getFooterWidget DListView::takeHeaderWidget
 */
QWidget *DListView::takeFooterWidget(int index)
{
    D_D(DListView);

    QWidget *widget = d->footerList.takeAt(index);

    d->footerLayout->layout()->removeWidget(widget);

    if (d->footerList.isEmpty()) {
        d->footerLayout->deleteLater();
        d->footerLayout = nullptr;
    }

    return widget;
}

/**
 * \~chinese \sa DListView::orientation
 */
void DListView::setOrientation(QListView::Flow flow, bool wrapping)
{
    Qt::Orientation old_orientation = this->orientation();

    setFlow(flow);
    setWrapping(wrapping);

    Qt::Orientation orientation = this->orientation();

    if (old_orientation != orientation) {
        D_D(DListView);

        QMargins margins = viewportMargins();

        if (old_orientation == Qt::Vertical) {
            if (d->headerLayout)
                margins.setTop(0);

            if (d->footerLayout)
                margins.setBottom(0);
        } else {
            if (d->headerLayout)
                margins.setLeft(0);

            if (d->footerLayout)
                margins.setRight(0);
        }

        setViewportMargins(margins);

        d->onOrientationChanged();

        Q_EMIT orientationChanged(orientation);
    }
}

/*!
 * \brief 开始编辑一个item
 * \param index 指定要编辑的item的位置
 */
void DListView::edit(const QModelIndex &index)
{
    QListView::edit(index);
}

void DListView::setBackgroundType(DStyledItemDelegate::BackgroundType backgroundType)
{
    if (DStyledItemDelegate *d = qobject_cast<DStyledItemDelegate *>(itemDelegate())) {
        d->setBackgroundType(backgroundType);

        if (d->backgroundType() == DStyledItemDelegate::RoundedBackground) {
            d->setItemSpacing(10);
        } else if (d->backgroundType() == DStyledItemDelegate::ClipCornerBackground) {
            d->setItemSpacing(1);
        } else {
            d->setItemSpacing(0);
        }
    }
}

void DListView::setItemMargins(const QMargins &itemMargins)
{
    if (DStyledItemDelegate *d = qobject_cast<DStyledItemDelegate *>(itemDelegate())) {
        d->setMargins(itemMargins);
    }
}

void DListView::setItemSize(QSize itemSize)
{
    if (DStyledItemDelegate *d = qobject_cast<DStyledItemDelegate *>(itemDelegate())) {
        return d->setItemSize(itemSize);
    }
}

void DListView::setItemSpacing(int spacing)
{
    if (DStyledItemDelegate *d = qobject_cast<DStyledItemDelegate *>(itemDelegate())) {
        return d->setItemSpacing(spacing);
    }
}

void DListView::setItemRadius(int radius)
{
    if (DStyledItemDelegate *d = qobject_cast<DStyledItemDelegate *>(itemDelegate())) {
        DStyle::setFrameRadius(this, radius);
        update();
    }
}

#if(QT_VERSION < 0x050500)
void DListView::setViewportMargins(int left, int top, int right, int bottom)
{
    D_D(DListView);

    d->left = left;
    d->top = top;
    d->right = right;
    d->bottom = bottom;

    QListView::setViewportMargins(left, top, right, bottom);
}

void DListView::setViewportMargins(const QMargins &margins)
{
    setViewportMargins(margins.left(), margins.top(), margins.right(), margins.bottom());
}

QMargins DListView::viewportMargins() const
{
    D_DC(DListView);

    return QMargins(d->left, d->top, d->right, d->bottom);
}
#endif

void DListView::resizeEvent(QResizeEvent *event)
{
    QListView::resizeEvent(event);

    D_D(DListView);

    bool isVerticalLayout = (orientation() == Qt::Vertical);

    if (d->headerLayout) {
        if (isVerticalLayout)
            d->headerLayout->resize(width(), d->headerLayout->height());
        else
            d->headerLayout->resize(d->headerLayout->width(), height());
    }

    if (d->footerLayout) {
        if (isVerticalLayout) {
            d->footerLayout->resize(width(), d->footerLayout->height());
            d->footerLayout->move(0, height() - d->footerLayout->height());
        } else {
            d->footerLayout->resize(d->footerLayout->width(), height());
            d->footerLayout->move(width() - d->footerLayout->width(), 0);
        }
    }
}

void DListView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    QListView::currentChanged(current, previous);

    Q_EMIT currentChanged(previous);
}

bool DListView::edit(const QModelIndex &index, QAbstractItemView::EditTrigger trigger, QEvent *event)
{
    if (QWidget *w = indexWidget(index)) {
        Qt::ItemFlags flags = model()->flags(index);

        if (((flags & Qt::ItemIsEditable) == 0) || ((flags & Qt::ItemIsEnabled) == 0))
            return false;
        if (state() == QAbstractItemView::EditingState)
            return false;
        if (trigger == QAbstractItemView::AllEditTriggers) // force editing
            return true;
        if ((trigger & editTriggers()) == QAbstractItemView::SelectedClicked
                && !selectionModel()->isSelected(index))
            return false;

        if (trigger & editTriggers()) {
            w->setFocus();

            Q_EMIT triggerEdit(index);

            return true;
        }
    }

    bool tmp = QListView::edit(index, trigger, event);

    if (tmp)
        Q_EMIT triggerEdit(index);

    return tmp;
}

QStyleOptionViewItem DListView::viewOptions() const
{
    QStyleOptionViewItem item = QListView::viewOptions();

    item.showDecorationSelected = true;
    // 列表项不会变为Inactive状态
    item.state |= QStyle::State_Active;

    if (viewMode() == QListView::ListMode) {
        item.decorationAlignment = Qt::AlignVCenter;
    }

    return item;
}

QModelIndex DListView::moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
    switch (cursorAction) {
    case MoveNext: {
        auto curIdx = currentIndex();
        auto curCol = modelColumn();

        auto nextRow = 0;
        if (curIdx.isValid()) {
            nextRow = (curIdx.row() + 1) % model()->rowCount();
        }

        int hiddenCount = 0;
        while (isRowHidden(nextRow)) {
            nextRow = (nextRow + 1) % model()->rowCount();

            //防止所有列都是隐藏的死循环
            ++hiddenCount;
            if (hiddenCount >= count()) {
                return currentIndex();
            }
        }

        return model()->index(nextRow, curCol);
    }
    default:
        break;
    }

    return QListView::moveCursor(cursorAction, modifiers);
}

DWIDGET_END_NAMESPACE

#include "moc_dlistview.cpp"
