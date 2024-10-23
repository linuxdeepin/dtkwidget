// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QDebug>
#include <QScrollBar>

#include "dboxwidget.h"
#include "dlistview.h"
#include "private/dlistview_p.h"
#include "dflowlayout.h"
#include "dstyleoption.h"
#include "dstyleditemdelegate.h"
#include "dstyle.h"

#include <DBounceAnimation>

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
/*!
  @~english
  \fn void DListView::currentChanged(const QModelIndex &previous)
  \brief This signal is called when the current item changes
  
  \details ListView will have a QModelIndex object that always represents the current Item index,
  This signal is called when the location of this QModelIndex object changes, not the content of the current ITEM content changes.
  When a mouse ordered machine to switch item with a keyboard
  
  \param[in] previous It is the index object of the previous item
  
  \sa QModelIndex QListView::currentChanged
 */

/*!
  @~english
  \fn void DListView::triggerEdit(const QModelIndex &index)
  \brief This signal is called when a new item is edited
  
  \param[in] index the indexing object of item that is being edited
  
  \sa QModelIndex QAbstractItemView::EditTrigger
 */
// ====================Signals end====================

/*!
  @~english
  \class Dtk::Widget::DListView
  \inmodule dtkwidget
  \brief A control for displaying a column of data.
  
  \details DListView is similar to QListView a class that belongs to QT's Model/View Framework.
   It is often used to display a column of data. When the data is more, you can roll the control
    to display and multiple contents.
  But it is also different from QLISTView. DListView provides top controls and bottom controls. 
  They are always displayed in ListView, which will not be visible because of rolling.
  The method of data, such as: additem, additems, insertitem, takeItem, removeItem, and some 
  commonly used signals in development.
 */

/*!
  @~english
  \brief Get a dlistView instance
  \param[in] parent Parents controlled by the DListView instance
 */
DListView::DListView(QWidget *parent) :
    QListView(parent),
    DObject(*new DListViewPrivate(this))
{
    d_func()->init();
    if (ENABLE_ANIMATIONS && ENABLE_ANIMATION_SEARCH) {
        auto ani = new DBounceAnimation(this);
        ani->setAnimationTarget(this);
        ani->setAniMationEnable(true);
    }
}

/*!
  @~english
  \brief Get the current state of the control
  
  The control can be dragged, edited by editor, and is playing animation and other states. You can check in detail:QAbstractItemView::State
  
  \return the current state of control
  
  \sa QAbstractItemView::State
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

int DListView::horizontalOffset() const
{
    if (viewMode() != IconMode || flow() != LeftToRight || !isWrapping()
            || (!gridSize().isValid() && !itemSize().isValid())) {
        return QListView::horizontalOffset();
    }

    int width = contentsSize().width();
    int offset = gridSize().isValid() ? 0 : spacing() / 2;

    return -(viewport()->width() - width) / 2 + offset;
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
  @~english
  \brief Get a top control
  
  \details The top control will be displayed in ListView, and the top control will always be above all item.
  That is to say, the difference between the top control and item is that the top control is always displayed in the layout, not because the mouse rolling is not visible.
  In addition, there are multiple top controls. Their layout (direction) is the same as the layout direction of item
  
  \param[in] index Specify the index of the top control to be obtained
  \return Return to the top control object at the designated index
  
  \note Note that the top control is not like the head of GridLayout, and the header is always deployed in the horizontal direction
  
  \sa DListView::getFooterWidget DListView::addHeaderWidget DListView::removeHeaderWidget DListView::takeHeaderWidget
 */
QWidget *DListView::getHeaderWidget(int index) const
{
    return d_func()->headerList.value(index);
}

/*!
  @~english
  \brief Get a bottom control
  \param[in] index Specify the index of the bottom control to be obtained
  \return return to the bottom control object at the specified index
  \sa DListView::getHeaderWidget
 */
QWidget *DListView::getFooterWidget(int index) const
{
    return d_func()->footerList.value(index);
}

/*!
  @~english
  \brief Determine whether the given QRect is overlapped with the item of ListView.
  
  \details The Item of ListView shows the ViewPort of ListView. item can only be displayed in ViewPort. item beyond this area will be invisible.
  
  \param[in] rect QRct to compare
  \return Whether the two rectangles have overlapping areas
  \sa DListView::isVisualRect
 */
bool DListView::isActiveRect(const QRect &rect) const
{
//    D_DC(DListView);

    const QRect &area = viewport()->geometry();

    return area.intersects(rect);
}

/*!
  @~english
  \brief Same as dlistView::IsvisualRect

  \param[in] rect The position rectangle for judging.
  \return Successfully contains rectangular return true，Otherwise, return false.

  \sa DListView::isVisualRect
 */
bool DListView::isVisualRect(const QRect &rect) const
{
    const QRect &area = viewport()->geometry();

    return area.intersects(rect);
}

/*!
  @~english
  \fn void DListView::rowCountChanged()
  \sa DListView::count
 */

/*!
  @~english
  \property DListView::count
  \brief How many lines of data is there in this attribute preservation
  \details Getter: DListView::count , Signal: DListView::rowCountChanged
 */
int DListView::count() const
{
    return model()->rowCount(rootIndex());
}

/*!
  @~english
  \fn void DListView::orientationChanged(Qt::Orientation orientation)

  \param[in] orientation The direction value of the change.

  \sa DListView::orientation
 */

/*!
  @~english
  \property DListView::orientation
  \brief This attribute saves the layout of Item in ListView
  \details Getter: DListView::orientation , Setter: DListView::setOrientation , Signal: DListView::orientationChanged
  \sa Qt::Orientation
 */
Qt::Orientation DListView::orientation() const
{
    bool isVerticalLayout = isWrapping()
                            ? flow() == QListView::LeftToRight
                            : flow() == QListView::TopToBottom;

    return isVerticalLayout ? Qt::Vertical : Qt::Horizontal;
}

/*!
  @~english
  \brief Set the model you want to use dlistView
  
  \details The model is used to provide data for ListView to achieve the structure of the separation of the data layer and the interface layer. For details, please refer to Qt's Model/View Framework
  
  \param[in] model Model object
  \sa QListView::setModel
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
  @~english
  \brief Add an Item at the bottom of the list
  \param[in] data New data to be added
  \return return whether it is successful to add
 */
bool DListView::addItem(const QVariant &data)
{
    return insertItem(count(), data);
}

/*!
  @~english
  \brief Add more item at the bottom of the list at one time
  \param[in] datas List of new data composition
  \return Whether it is successful
 */
bool DListView::addItems(const QVariantList &datas)
{
    return insertItems(count(), datas);
}

/*!
  @~english
  \brief Add a new item at the designated bank
  \param[in] index To increase the line number of Item
  \param[in] data Item data to be added
  \return Whether it is successful
 */
bool DListView::insertItem(int index, const QVariant &data)
{
    if (!model()->insertRow(index))
        return false;

    return model()->setData(model()->index(index, 0, rootIndex()), data);
}

/*!
  @~english
  \brief Add more item at the designated bank
  \param[in] index To increase the line number of Item
  \param[in] datas List of data composition of items data
  \return Whether it is successful
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
  @~english
  \brief Remove the designated position item
  \param[in] index The line number of it to remove
  \return Whether it is remove successfully
 */
bool DListView::removeItem(int index)
{
    return model()->removeRow(index);
}

/*!
  @~english
  \brief Remove multiple item at a time
  \param[in] index Start removing the line number of Item
  \param[in] count Remove from the line number specified by index, remove the county item
  \return Whether to return to remove success
 */
bool DListView::removeItems(int index, int count)
{
    return model()->removeRows(index, count);
}

/*!
  @~english
  \brief This function is used to add top controls.

  Similar to dlistView::GetHeaderWidget, but returns the object of the top control to be removed.
  \param[in] widget Head control instance.

  \return Successfully adds to the index value of adding to dlistview, and the corresponding control has been returned
  The index value.

  \sa DListView::getHeaderWidget
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
  @~english
  \brief This function is used to remove the head control small control.

  \param[in] index Add a small head control in dlistview
  The index value is the return value of dlistView::addheaderWidget.

  \sa DListView::addFooterWidget
 */
void DListView::removeHeaderWidget(int index)
{
    QWidget *widget = takeHeaderWidget(index);

    if (widget)
        widget->deleteLater();
}

/*!
  @~english
  \brief This function is used to remove the small control of the head and return the control.
  \details Similar to Dlistview::GetHeaderWidget, but returns the object of the top control to be removed

  \param[in] index Add a small head control in dlistview
  The index value is the return value of dlistView::addheaderWidget.
  \return Successfully remove the small head control obtained, otherwise return nullptr.

  \sa DListView::getHeaderWidget
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
  @~english
  \brief This function is used to add a small control small control.

  \param[in] widget the instance of control in footer
  \return Successfully add the corresponding index value, if it exists, return
  Corresponding index value.

  \sa DListView::getFooterWidget
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
  @~english
  \brief This function is used to remove the underlying foot-footed control.

  \param[in] index Add to dlistview mid-layer footer control
  Index value，It is the return value of dlistView::addfooterWidget.

  \sa DListView::addFooterWidget
 */
void DListView::removeFooterWidget(int index)
{
    QWidget *widget = takeFooterWidget(index);

    if (widget)
        widget->deleteLater();
}

/*!
  @~english
  \brief Remove the bottom foot control and return to the control.

  \param[in] index Add the index value of the bottom -footed control control in dlistview,
  It is the return value of dlistView::addfooterWidget.

  \sa DListView::getFooterWidget DListView::takeHeaderWidget
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

/*!
  @~english
  \brief This function is used to set the direction of dlistview

  \param[in] flow The direction of dlistview，have QListView::Flow::LeftToRight and
  QListView::Flow::TopToBottom 

  \param[in] wrapping Used to control whether the layout is automatically changed. 
  True indicates that automatic changes are replaced, and False indicates non -automatic bank.

  \sa DListView::orientation
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
  @~english
  \brief Edit a item.

  \param[in] index Specify the location of the Item to edit
 */
void DListView::edit(const QModelIndex &index)
{
    QListView::edit(index);
}

/*!
  @~english
  \brief Set the background color type of item.

  \param[in] backgroundType Background color
 */
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

/*!
  @~english
  \brief Set the content of the item Margin.

  \param[in] itemMargins margin value
 */
void DListView::setItemMargins(const QMargins &itemMargins)
{
    if (DStyledItemDelegate *d = qobject_cast<DStyledItemDelegate *>(itemDelegate())) {
        d->setMargins(itemMargins);
    }
}

/*!
  @~english
  \brief Set the size of Item.

  \param[in] itemSize itemSize
 */
void DListView::setItemSize(QSize itemSize)
{
    if (DStyledItemDelegate *d = qobject_cast<DStyledItemDelegate *>(itemDelegate())) {
        return d->setItemSize(itemSize);
    }
}

/*!
  @~english
  \brief Set the spacing size of Item.

  \param[in] spacing　Spacing value
 */
void DListView::setItemSpacing(int spacing)
{
    if (DStyledItemDelegate *d = qobject_cast<DStyledItemDelegate *>(itemDelegate())) {
        return d->setItemSpacing(spacing);
    }
}

/*!
  @~english
  \brief Set the rounded corner size of Item.
  \param[in] radius Rounded corner size value
 */
void DListView::setItemRadius(int radius)
{
    DStyle::setFrameRadius(this, radius);
    update();
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

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
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
#else
/*!
  \reimp
*/
void DListView::initViewItemOption(QStyleOptionViewItem *option) const
{
    QListView::initViewItemOption(option);
    option->showDecorationSelected = true;
    // 列表项不会变为Inactive状态
    option->state |= QStyle::State_Active;
    if (viewMode() == QListView::ListMode) {
        option->decorationAlignment = Qt::AlignVCenter;
    }
}
#endif

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
