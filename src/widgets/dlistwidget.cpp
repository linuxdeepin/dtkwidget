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

#include <QWidget>
#include <QVBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>

#include "dthememanager.h"
#include "dlistwidget.h"
#include "private/dlistwidget_p.h"

DWIDGET_BEGIN_NAMESPACE

DListWidgetPrivate::DListWidgetPrivate(DListWidget *qq):
        DScrollAreaPrivate(qq),
        itemWidth(-1),
        itemHeight(-1),
        checkMode(DListWidget::Radio),
        mainWidget(new DBoxWidget(QBoxLayout::TopToBottom)),
        visibleCount(0),
        checkable(false),
        toggleable(false),
        enableHorizontalScroll(false),
        enableVerticalScroll(false)
{

}

DListWidgetPrivate::~DListWidgetPrivate()
{

}

void DListWidgetPrivate::init()
{
    Q_Q(DListWidget);

    mainWidget->setObjectName("MainWidget");

    q->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    q->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    q->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    q->setAlignment(Qt::AlignHCenter);
    q->setFrameStyle(QFrame::NoFrame);
    q->setFrameShape(QFrame::NoFrame);
    q->setWidget(mainWidget);

    QObject::connect(mainWidget, &DBoxWidget::sizeChanged, q, &DListWidget::updateSize);
}




/**
 * \~chinese \class DListWidget
 * \~chinese \brief 基于widget的列表控件。

 * \~chinese DListWidget是一个自定义的列表控件，与QListWidget相比它虽然也是基于item的一个列表控件但却更灵活，因为它可以设置任意QWidget类型的对象作为自己的item，同时提供了各种新增，插入，移除item的方法以及信号，另外还支持设置选择模式为单选或多选。
 */

/**
 * \~chinese \enum DListWidget::CheckMode
 * \~chinese \brief 定义了列表的选择模式。

 * \~chinese 其定义了以下两种模式：
 * \~chinese \var DListWidget::CheckMode DListWidget::Radio
 * \~chinese 单选模式。
 * \~chinese \var DListWidget::CheckMode DListWidget::MultipleCheck
 * \~chinese 多选模式。
 */

/**
 * \~chinese \fn DListWidget::checkedChanged
 * \~chinese \brief 某一项的选择状态发生了改变。
 * \~chinese \param index 发生改变的项的位置。
 * \~chinese \param checked 当前是否被选择。
 */

/**
 * \~chinese \brief 获取一个 DListWidget 实例
 *
 * \~chinese \param checkMode 指定列表控件的选择模式，默认为 DListWidget::CheckMode::Radio 。
 * \~chinese \param parent 指定列表控件的父控件。
 */
DListWidget::DListWidget(CheckMode checkMode, QWidget *parent) :
    DScrollArea(*new DListWidgetPrivate(this), parent)
{
    DThemeManager::registerWidget(this);

    d_func()->checkMode = checkMode;
    d_func()->init();
}

/**
 * \~chinese \brief 获取一个 DListWidget 实例
 *
 * \~chinese \param direction 指定列表控件的内容布局方向。
 * \~chinese \param checkMode 指定列表控件的选择模式，默认为 DListWidget::CheckMode::Radio 。
 * \~chinese \param parent 指定列表控件的父控件。
 */
DListWidget::DListWidget(QBoxLayout::Direction direction, DListWidget::CheckMode checkMode, QWidget *parent) :
    DScrollArea(*new DListWidgetPrivate(this), parent)
{
    DThemeManager::registerWidget(this);

    d_func()->mainWidget->setDirection(direction);
    d_func()->checkMode = checkMode;
    d_func()->init();
}

/**
 * \~chinese \brief 新增一个列表项。
 * \~chinese
 * \~chinese \param w 要新增的列表项对象。
 * \~chinese \param a 为要新增的列表项对象指定对齐方式， 默认为 Qt::AlignHCenter 。
 * \~chinese
 * \~chinese \return 返回新增的列表项在整个列表中的位置。
 */
int DListWidget::addWidget(QWidget *w, Qt::Alignment a)
{
    insertWidget(-1, w, a);

    return count()-1;
}

/**
 * \~chinese \brief 一次新增多个列表项。
 * \~chinese
 * \~chinese \param ws 要新增的多个列表项组成的QList。
 * \~chinese \param a 为要新增的多个列表项对象指定对齐方式， 默认为 Qt::AlignHCenter 。
 */
void DListWidget::addWidgets(const QList<QWidget*> &ws, Qt::Alignment a)
{
    insertWidgets(-1, ws, a);
}

/**
 * \~chinese \brief 在指定位置插入一个列表项。
 * \~chinese
 * \~chinese \param index 要插入列表项的位置。
 * \~chinese \param w 要插入的列表项对象。
 * \~chinese \param a 为要插入的列表项对象指定对齐方式， 默认为 Qt::AlignHCenter 。
 */
void DListWidget::insertWidget(int index, QWidget *w, Qt::Alignment a)
{
    Q_D(DListWidget);

    if(w==NULL || d->widgetList.contains(w))
        return;

    if(d->itemWidth>0){
        w->setFixedWidth(d->itemWidth);
    }

    if(d->itemHeight>0){
        w->setFixedHeight(d->itemHeight);
    }

    if(index < 0) {
        d->widgetList << w;
        d->mainWidget->layout()->addWidget(w, 0, a);
    } else {
        d->widgetList.insert(index, w);
        d->mainWidget->layout()->insertWidget(index, w, 0, a);
    }

    d->mapVisible[w] = true;

    setVisibleCount(d->visibleCount + 1);

    Q_EMIT countChanged(count());
}

/**
 * \~chinese \brief 一次在指定位置插入多个列表项。
 * \~chinese
 * \~chinese \param index 要插入列表项的位置。
 * \~chinese \param w 要插入的多个列表项对象组成的QList。
 * \~chinese \param a 为要插入的列表项对象指定对齐方式， 默认为 Qt::AlignHCenter 。
 */
void DListWidget::insertWidgets(int index, const QList<QWidget*> &ws, Qt::Alignment a)
{
    if(index < 0) {
        Q_FOREACH (QWidget *w, ws) {
            insertWidget(-1, w, a);
        }
    } else {
        Q_FOREACH (QWidget *w, ws) {
            insertWidget(index++, w, a);
        }
    }
}

/**
 * \~chinese \brief 设置每一个列表项的宽高。
 * \~chinese
 * \~chinese \param w 指定宽度。
 * \~chinese \param h 指定高度。
 */
void DListWidget::setItemSize(int w, int h)
{
    Q_D(DListWidget);

    d->itemWidth = w;
    d->itemHeight = h;

    if(d->itemWidth <= 0 && d->itemHeight <= 0)
        return;

    Q_FOREACH (QWidget *w, d->widgetList) {
        if(d->itemWidth>0)
            w->setFixedWidth(d->itemWidth);
        if(d->itemHeight>0){
            w->setFixedHeight(d->itemHeight);
        }
    }
}

/**
 * \~chinese \brief 清空当前列表中的所有列表项。
 * \~chinese
 * \~chinese \param isDelete 是否释放列表项对象占用的内存空间。
 */
void DListWidget::clear(bool isDelete)
{
    Q_D(DListWidget);

    for(int i=0;i<count();++i){
        delete d->mainWidget->layout()->takeAt(i);
        d->widgetList[i]->setParent(NULL);
        if(isDelete)
            d->widgetList[i]->deleteLater();
    }

    d->mapVisible.clear();
    d->widgetList.clear();
    d->checkedList.clear();

    resize(0, 0);
    setVisibleCount(0);

    Q_EMIT countChanged(count());
}

/**
 * \~chinese \brief 移除指定位置的列表项。
 * \~chinese
 * \~chinese \param index 指定的位置。
 * \~chinese \param isDelete 是否释放被移除对象占用的内存空间。
 */
void DListWidget::removeWidget(int index, bool isDelete)
{
    Q_D(DListWidget);

    QWidget *w = getWidget(index);

    d->widgetList.removeAt(index);
    delete d->mainWidget->layout()->takeAt(index);
    d->checkedList.removeOne(index);

    if(d->mapVisible.value(w, false)){
        setVisibleCount(d->visibleCount -1);
    }
    d->mapVisible.remove(w);

    w->setParent(NULL);
    if(isDelete)
        w->deleteLater();

    Q_EMIT countChanged(count());
}

/**
 * \~chinese \brief 显示指定位置的列表项。
 * \~chinese
 * \~chinese \param index 指定的位置。
 */
void DListWidget::showWidget(int index)
{
    Q_D(DListWidget);

    QWidget *w = getWidget(index);

    if(!d->mapVisible.value(w, true)){
        w->show();
        d->mapVisible[w] = true;
        setVisibleCount(d->visibleCount+1);
    }
}

/**
 * \~chinese \brief 隐藏指定位置的列表项。
 * \~chinese
 * \~chinese \param index 指定的位置。
 */
void DListWidget::hideWidget(int index)
{
    Q_D(DListWidget);

    QWidget *w = getWidget(index);

    if(d->mapVisible.value(w, false)){
        w->hide();
        d->mapVisible[w] = false;
        setVisibleCount(d->visibleCount-1);
    }
}

/**
 * \~chinese \brief 设置指定位置的列表项的选择状态。
 * \~chinese
 * \~chinese \param index 指定的位置。
 * \~chinese \param checked 为 true 则设置指定的项为被选择状态，反之则反。
 */
void DListWidget::setChecked(int index, bool checked)
{
    Q_D(DListWidget);

    if (!d->checkable || (checked && isChecked(index)) || (!checked && !isChecked(index)))
        return;

    if(d->checkMode == Radio && checked){
        int checkedWidget = firstChecked();
        if(checkedWidget>=0){
            getWidget(checkedWidget)->setProperty("checked", false);
            d->checkedList.removeOne(checkedWidget);
        }
    }

    if(index>=0 && index < count()){
        QWidget *w = getWidget(index);
        w->setProperty("checked", checked);
    }

    if(checked){
        d->checkedList << index;
    }else{
        d->checkedList.removeOne(index);
    }

    if(d->checkMode == Radio && checked){
        Q_EMIT firstCheckedChanged(index);
    }

    Q_EMIT checkedChanged(index, checked);
}

/**
 * \~chinese \sa DListWidget::checkMode
 */
void DListWidget::setCheckMode(DListWidget::CheckMode checkMode)
{
    Q_D(DListWidget);

    d->checkMode = checkMode;
}

/**
 * \~chinese \sa DListWidget::checkable
 */
void DListWidget::setCheckable(bool checkable)
{
    Q_D(DListWidget);

    if (d->checkable == checkable)
        return;

    d->checkable = checkable;
}

/**
 * \~chinese \sa DListWidget::toggleable
 */
void DListWidget::setToggleable(bool enableUncheck)
{
    Q_D(DListWidget);

    if (d->toggleable == enableUncheck)
        return;

    d->toggleable = enableUncheck;
    Q_EMIT toggleableChanged(enableUncheck);
}

/**
 * \~chinese \sa DListWidget::enableHorizontalScroll
 */
void DListWidget::setEnableHorizontalScroll(bool enableHorizontalScroll)
{
    Q_D(DListWidget);

    if (d->enableHorizontalScroll == enableHorizontalScroll)
        return;

    d->enableHorizontalScroll = enableHorizontalScroll;

    updateSize();

    Q_EMIT enableHorizontalScrollChanged(enableHorizontalScroll);
}

/**
 * \~chinese \brief 设置是否允许垂直方向滚动。
 * \~chinese
 * \~chinese \param enableVerticalScroll 为 true 即为允许，反之则反。
 */
void DListWidget::setEnableVerticalScroll(bool enableVerticalScroll)
{
    Q_D(DListWidget);

    if (d->enableVerticalScroll == enableVerticalScroll)
        return;

    d->enableVerticalScroll = enableVerticalScroll;

    updateSize();

    Q_EMIT enableVerticalScrollChanged(enableVerticalScroll);
}

/**
 * \~chinese \sa DListWidget::spacing
 */
void DListWidget::setSpacing(int spacing)
{
    Q_D(DListWidget);

    d->mainWidget->layout()->setSpacing(spacing);
}

/**
 * \~chinese \sa DListWidget::direction
 */
void DListWidget::setDirection(QBoxLayout::Direction direction)
{
    Q_D(DListWidget);

    d->mainWidget->layout()->setDirection(direction);
}

/**
 * \~chinese \fn DListWidget::countChanged
 * \~chinese \brief 当列表项数量发生改变时被调用。
 * \~chinese \param count 当前列表项的数量。
 * \~chinese \sa DListWidget::count
 */

/**
 * \~chinese \property DListWidget::count
 * \~chinese \brief 所有列表项数量。

 * \~chinese Getter: DListWidget::count , Signal: DListWidget::countChanged
 */
int DListWidget::count() const
{
    Q_D(const DListWidget);

    return d->widgetList.count();
}

/**
 * \~chinese \brief 获取指定列表项对象所在的位置。
 * \~chinese
 * \~chinese \param w 指定的列表项对象。
 * \~chinese
 * \~chinese \return 指定对象的位置。
 */
int DListWidget::indexOf(const QWidget* w)
{
    Q_D(DListWidget);

    return d->widgetList.indexOf(const_cast<QWidget*>(w));
}

/**
 * \~chinese \brief 获取指定位置的列表项对象。 
 * \~chinese
 * \~chinese \param index 指定的位置。
 * \~chinese
 * \~chinese \return 指定位置处的列表项对象。
 */
QWidget *DListWidget::getWidget(int index) const
{
    Q_D(const DListWidget);

    return d->widgetList[index];
}

/**
 * \~chinese \fn DListWidget::firstCheckedChanged
 * \~chinese \brief 第一个被选择的列表项的位置改变时被调用。
 * \~chinese \param index 当前第一个被选择的列表项的位置。
 * \~chinese \sa DListWidget::firstChecked
 */

/**
 * \~chinese \property DListWidget::firstChecked
 * \~chinese \brief 第一个被选择的列表项的位置。
 * 
 * \~chinese Getter: DListWidget::firstChecked , Signal: DListWidget::firstCheckedChanged
 */
int DListWidget::firstChecked() const
{
    Q_D(const DListWidget);

    return d->checkedList.count()>0 ? d->checkedList.first() : -1;
}

/**
 * \~chinese \brief 获取所有被选择项的位置组成的列表。
 * \~chinese
 * \~chinese \return 所有被选择项的位置组成的列表。
 */
QList<int> DListWidget::checkedList() const
{
    Q_D(const DListWidget);

    return d->checkedList;
}

/**
 * \~chinese \fn DListWidget::checkableChanged
 * \~chinese \sa DListWidget::checkable
 */

/**
 * \~chinese \property DListWidget::checkable
 * \~chinese \brief 这个属性表示列表控件中的项是否可以被选择。
 * \~chinese Getter: DListWidget::checkable, Setter: DListWidget::setCheckable , Signal: DListWidget::checkableChanged
 */
bool DListWidget::checkable() const
{
    Q_D(const DListWidget);

    return d->checkable;
}

/**
 * \~chinese \fn DListWidget::toggleableChanged 
 * \~chinese \sa DListWidget::toggleable
 */

/**
 * \~chinese \property DListWidget::toggleable
 * \~chinese \brief 这个属性表示列表控件的所有项的选择状态是否可以切换。
 * \~chinese 当设置为 true 时，鼠标点击某一个列表项，会根据当前的列表项的选择状态切换，如果当前是已选择状态则点击后切换到未选择状态，反之则反，此选项默认为 false 。

 * \~chinese Getter: DListWidget::toggleable , Setter: DListWidget::setToggleable , Signal: DListWidget::toggleableChanged
 */
bool DListWidget::toggleable() const
{
    Q_D(const DListWidget);

    return d->toggleable;
}

bool DListWidget::isChecked(int index) const
{
    Q_D(const DListWidget);

    return d->checkedList.contains(index);
}

/**
 * \~chinese \fn DListWidget::visibleCountChanged
 * \~chinese \brief 当前列表中正在显示的所有列表项的数量发生改变时被调用。
 * \~chinese \param visibleCount 当前列表中正在显示的所有列表项的数量。
 * \~chinese \sa DListWidget::visibleCount
 */

/**
 * \~chinese \property DListWidget::visibleCount
 * \~chinese \brief 当前列表中正在显示的所有列表项的数量。
 * \~chinese Getter: DListWidget::visibleCount ,Signal: DListWidget::visibleCountChanged
 */
int DListWidget::visibleCount() const
{
    Q_D(const DListWidget);

    return d->visibleCount;
}

/**
 * \~chinese \property DListWidget::spacing
 * \~chinese \brief 这个属性是列表项之间的间隔大小。
 * \~chinese 使用 DListWidget::setSpacing 设置这个属性。
 */
int DListWidget::spacing() const
{
    Q_D(const DListWidget);

    return d->mainWidget->layout()->spacing();
}

/**
 * \~chinese \property DListWidget::direction
 * \~chinese \brief 这个属性是列表项布局的方向。
 * \~chinese 使用 DListWidget::setDirection 设置这个属性。
 */
QBoxLayout::Direction DListWidget::direction() const
{
    Q_D(const DListWidget);

    return d->mainWidget->direction();
}

/**
 * \~chinese \property DListWidget::checkMode
 * \~chinese \brief 这个属性表示列表控件的选择模式。
 * \~chinese 当属性的值为 DListWidget::CheckMode::Radio 时，列表为单选模式，为 DListWidget::CheckMode::MultipleCheck 时列表为多选模式。
 * \~chinese Getter: DListWidget::checkMode , Setter: DListWidget::setCheckMode
 */
DListWidget::CheckMode DListWidget::checkMode() const
{
    Q_D(const DListWidget);

    return d->checkMode;
}

void DListWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if(!checkable()){
        return DScrollArea::mouseReleaseEvent(e);
    }

    Q_D(DListWidget);

    QWidget *widget = childAt(e->pos());

    while(widget && widget->parentWidget() != d->mainWidget) {
        widget = widget->parentWidget();
    }

    int index = indexOf(widget);
    if(index>=0){
        if(toggleable())
            setChecked(index, !isChecked(index));
        else
            setChecked(index, true);

        Q_EMIT clicked(index);
    }
}

void DListWidget::setVisibleCount(int count)
{
    Q_D(DListWidget);

    if(d->visibleCount == count)
        return;

    d->visibleCount = count;

    Q_EMIT visibleCountChanged(count);
}

void DListWidget::updateSize()
{
    Q_D(DListWidget);

    if(!d->enableHorizontalScroll) {
        if(direction() == QBoxLayout::LeftToRight || direction() == QBoxLayout::RightToLeft)
            setFixedWidth(d->mainWidget->width());
        else
            setMinimumWidth(d->mainWidget->width());
    } else {
        setMinimumWidth(qMin(d->mainWidget->width(), maximumWidth()));
    }

    if(!d->enableVerticalScroll) {
        if(direction() == QBoxLayout::TopToBottom || direction() == QBoxLayout::BottomToTop)
            setFixedHeight(d->mainWidget->height());
        else
            setMinimumHeight(d->mainWidget->height());
    } else {
        setMinimumHeight(qMin(d->mainWidget->height(), maximumHeight()));
    }
}

/**
 * \~chinese \brief 获取所有列表项对象。
 * \~chinese
 * \~chinese \return 所有列表项对象组成的列表。
 */
QList<QWidget*> DListWidget::widgetList() const
{
    Q_D(const DListWidget);

    return d->widgetList;
}

/**
 * \~chinese \brief 获取列表项的宽高。
 * \~chinese
 * \~chinese \return 列表项的宽高。
 */
QSize DListWidget::itemSize() const
{
    Q_D(const DListWidget);

    return QSize(d->itemWidth, d->itemHeight);
}

/**
 * \~chinese \fn DListWidget::enableHorizontalScrollChanged
 * \~chinese \sa DListWidget::enableHorizontalScroll
 */

/**
 * \~chinese \property DListWidget::enableHorizontalScroll
 * \~chinese \brief 这个属性表示列表控件是否允许水平滚动。
 * \~chinese Getter: DListWidget::enableHorizontalScroll , Setter:DListWidget::setEnableHorizontalScroll , Signal: DListWidget::enableHorizontalScrollChanged
 */
bool DListWidget::enableHorizontalScroll() const
{
    Q_D(const DListWidget);

    return d->enableHorizontalScroll;
}

/**
 * \~chinese \fn DListWidget::enableVerticalScrollChanged
 * \~chinese \sa DListWidget::enableVerticalScroll
 */

/**
 * \~chinese \property DListWidget::enableVerticalScroll
 * \~chinese \brief 这个属性表示列表控件是否允许垂直滚动。
 * \~chinese Getter: DListWidget::enableVerticalScroll , Setter:DListWidget::setEnableVerticalScroll , Signal: DListWidget::enableVerticalScrollChanged
 */
bool DListWidget::enableVerticalScroll() const
{
    Q_D(const DListWidget);

    return d->enableVerticalScroll;
}

/**
 * \~chinese \brief 设置列表的内容与边框之间的间隔。
 * \~chinese
 * \~chinese \param left 左边距。
 * \~chinese \param top 上边距。
 * \~chinese \param right 右边距。
 * \~chinese \param bottom 下边距。
 */
void DListWidget::setContentsMargins(int left, int top, int right, int bottom)
{
    Q_D(DListWidget);

    d->mainWidget->layout()->setContentsMargins(left, top, right, bottom);
}

/**
 * \~chinese \brief 这是一个重载函数，设置列表的内容与边框之间的间隔。
 * \~chinese
 * \~chinese \param margins 指定左上右下边距。
 */
void DListWidget::setContentsMargins(const QMargins &margins)
{
    Q_D(DListWidget);

    d->mainWidget->layout()->setContentsMargins(margins);
}

/**
 * \~chinese \brief 获取列表的内容与边框之间的间隔。
 * \~chinese
 * \~chinese \param left 用于保存左边距的指针。
 * \~chinese \param top 用于保存上边距的指针。
 * \~chinese \param right 用于保存右边距的指针。
 * \~chinese \param bottom 用于保存下边距的指针。
 */
void DListWidget::getContentsMargins(int *left, int *top, int *right, int *bottom) const
{
    Q_D(const DListWidget);

    d->mainWidget->layout()->getContentsMargins(left, top, right, bottom);
}

/**
 * \~chinese \brief 获取列表的内容与边框之间的间隔。
 * \~chinese
 * \~chinese \return 列表的内容与边框之间的间隔。
 */
QMargins DListWidget::contentsMargins() const
{
    Q_D(const DListWidget);

    return d->mainWidget->layout()->contentsMargins();
}

/**
 * \~chinese \brief 获取列表所有内容的大小。
 * \~chinese
 * \~chinese \return 列表所有内容的大小。
 */
QRect DListWidget::contentsRect() const
{
    Q_D(const DListWidget);

    return d->mainWidget->layout()->contentsRect();
}

QSize DListWidget::sizeHint() const
{
    return minimumSize();
}

DListWidget::DListWidget(DListWidgetPrivate &dd, QWidget *parent):
    DScrollArea(dd, parent)
{
    DThemeManager::registerWidget(this);

    d_func()->init();
}

DWIDGET_END_NAMESPACE
