// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QDebug>

#include "dthememanager.h"
#include "dstackwidget.h"
#include "private/dstackwidget_p.h"

DWIDGET_BEGIN_NAMESPACE

DAbstractStackWidgetTransitionPrivate::DAbstractStackWidgetTransitionPrivate(DAbstractStackWidgetTransition *qq)
    : DObjectPrivate(qq)
    , animation(new QVariantAnimation(qq))
    , info({})
{

}

DAbstractStackWidgetTransitionPrivate::~DAbstractStackWidgetTransitionPrivate()
{

}

void DAbstractStackWidgetTransitionPrivate::init()
{
    Q_Q(DAbstractStackWidgetTransition);

    QObject::connect(animation, &QVariantAnimation::valueChanged, q, &DAbstractStackWidgetTransition::updateVariant);
}

DAbstractStackWidgetTransition::DAbstractStackWidgetTransition(QObject *parent):
    QObject(parent),
    DObject(*new DAbstractStackWidgetTransitionPrivate(this))
{
    d_func()->init();
}

void DAbstractStackWidgetTransition::beginTransition(const TransitionInfo &info)
{
    Q_D(DAbstractStackWidgetTransition);

    d->info = info;
}

QVariantAnimation *DAbstractStackWidgetTransition::animation() const
{
    Q_D(const DAbstractStackWidgetTransition);

    return d->animation;
}

DAbstractStackWidgetTransition::DAbstractStackWidgetTransition(DAbstractStackWidgetTransitionPrivate &dd,
                                                               QObject *parent):
    QObject(parent),
    DObject(dd)
{
    d_func()->init();
}

const DAbstractStackWidgetTransition::TransitionInfo &DAbstractStackWidgetTransition::info() const
{
    Q_D(const DAbstractStackWidgetTransition);

    return d->info;
}

DSlideStackWidgetTransition::DSlideStackWidgetTransition(QObject *parent):
    DAbstractStackWidgetTransition(parent)
{

}

void DSlideStackWidgetTransition::beginTransition(const TransitionInfo &info)
{
    DAbstractStackWidgetTransition::beginTransition(info);

    info.oldWidget->move(0, 0);
    if(info.type == Push) {
        info.newWidget->move(info.stackWidget->width(), 0);
    } else if(info.type == Pop) {
        info.newWidget->move(-info.stackWidget->width(), 0);
    }

    info.oldWidget->show();
    info.newWidget->show();

    animation()->setStartValue(0);
    animation()->setEndValue(-info.newWidget->x());
    animation()->start();
}

void DSlideStackWidgetTransition::updateVariant(const QVariant &variant)
{
    int x = variant.toInt();

    info().oldWidget->move(x, 0);

    if(info().type == Push) {
        info().newWidget->move(info().stackWidget->width() + x, 0);
    } else if(info().type == Pop) {
        info().newWidget->move(x - info().stackWidget->width(), 0);
    }
}

DStackWidgetPrivate::DStackWidgetPrivate(DStackWidget *qq):
    DObjectPrivate(qq)
{

}

DStackWidgetPrivate::~DStackWidgetPrivate()
{

}

void DStackWidgetPrivate::init()
{
    Q_Q(DStackWidget);

    currentIndex = -1;
    currentWidget = nullptr;
    transition = nullptr;
    q->setTransition(new DSlideStackWidgetTransition(q));
}

void DStackWidgetPrivate::setCurrentIndex(int index)
{
    if(index == currentIndex)
        return;

    Q_Q(DStackWidget);

    if(index >= 0 && index < widgetList.count()){
        currentIndex = index;
        currentWidget = widgetList[index];
    } else {
        currentIndex = -1;
        currentWidget = nullptr;
    }

    q->currentIndexChanged(index);
    q->currentWidgetChanged(currentWidget);
}

/*!
  \class Dtk::Widget::DStackWidget
  \inmodule dtkwidget
  \brief 一个可定制切换动画的容器控件.
  
  DStackWidget 在内容展示方面类似于一个使用了 QStackedLayout 布局的控件, 即在同一个位置展示不同的内容, 但 DStackWidget 提供了
  切换内容时的动画效果, 并且可进行自定义动画, 另外 DStackWidget 采用了栈的先入后出理念(不过也并没有完全遵守), 使用 DStackWidget::pushWidget 方法入栈新的内容,
  使用 DStackWidget::popWidget 出栈当前内容并显示下层内容(如果有的话), 还有 DStackWidget::insertWidget 可以指定插入的位置.
  
  如果要自定义动画则需要继承 DAbstractStackWidgetTransition, 具体实现可参考 DSlideStackWidgetTransition 类, DSlideStackWidgetTransition 类是 DStackWidget 使用的默认动画即滑动.
  \image html DStackWidget.gif
 */

/*!
  \fn void DStackWidget::switchWidgetFinished()
  \brief 当切换一次显示的内容后被调用(动画结束后)
 */

/*!
  \brief 构造一个 DStackWidget 实例
  \a parent 为 DStackWidget 实例的父控件
 */
DStackWidget::DStackWidget(QWidget *parent) :
    QWidget(parent),
    DObject(*new DStackWidgetPrivate(this))
{
    d_func()->init();
}

/*!
  \fn void DStackWidget::busyChanged(bool busy)
  \a busy 当前状态是否是过渡动画.

  \sa DStackWidget::busy
 */

/*!
  \property DStackWidget::busy
  \brief 表示当前是否处于过渡动画过程中.

  Getter: DStackWidget::busy , Signal: DStackWidget::busyChanged
 */
bool DStackWidget::busy() const
{
    Q_D(const DStackWidget);

    return d->transition->animation()->state() != QVariantAnimation::Stopped;
}

/*!
  \fn void DStackWidget::depthChanged(int depth)
  \a depth 改变的层级大小.
  \sa DStackWidget::depth
 */

/*!
  \property DStackWidget::depth
  \brief 表示正在管理有多少层内容(从 1 开始计数).

  Getter: DStackWidget::depth , Signal: DStackWidget::depthChanged
 */
int DStackWidget::depth() const
{
    Q_D(const DStackWidget);

    return d->widgetList.count();
}

/*!
  \fn void DStackWidget::currentIndexChanged(int currentIndex)
  \a currentIndex 当前索引.
  \sa DStackWidget::currentIndex
 */

/*!
  \property DStackWidget::currentIndex
  \brief 表示当前内容在栈中的索引位置(从 0 开始计数).

  Getter: DStackWidget::currentIndex , Signal: DStackWidget::currentIndexChanged
 */
int DStackWidget::currentIndex() const
{
    Q_D(const DStackWidget);

    return d->currentIndex;
}

/*!
  \fn DStackWidget::currentWidgetChanged(QWidget* currentWidget)

  \a currentWidget 修改的当前小控件
  \sa DStackWidget::currentWidget
 */

/*!
  \property DStackWidget::currentWidget
  \brief 表示当前内容的对象.

  Getter: DStackWidget::currentWidget , Signal: DStackWidget::currentWidgetChanged
 */
QWidget *DStackWidget::currentWidget() const
{
    Q_D(const DStackWidget);

    return d->currentWidget;
}

/*!
  \property DStackWidget::transition
  \brief 当前使用的动画对象
  
  如果没有调用过这个属性的 setter 方法, 那么将会返回默认使用的动画对象即 DSlideStackWidgetTransition 的一个实例
  Getter: DStackWidget::transition , Setter: DStackWidget::setTransition
 */
DAbstractStackWidgetTransition *DStackWidget::transition() const
{
    Q_D(const DStackWidget);

    return d->transition;
}

/*!
  \property DStackWidget::animationDuration
  \brief 这个属性表示当前动画播放一次所需要的时间.
  
  Getter: DStackWidget::animationDuration , Setter: DStackWidget::setAnimationDuration
 */
int DStackWidget::animationDuration() const
{
    Q_D(const DStackWidget);

    return d->transition->animation()->duration();
}

/*!
  \property DStackWidget::animationType
  \brief 这个属性表示当前动画的类型.
  
  Getter: DStackWidget::animationType , Setter: DStackWidget::setAnimationType
  \sa QEasingCurve::Type
 */
QEasingCurve::Type DStackWidget::animationType() const
{
    Q_D(const DStackWidget);

    return d->transition->animation()->easingCurve().type();
}

/*!
  \brief DStackWidget::pushWidget 入栈一个新的内容控件.

  \a widget 新的内容控件对象
  \a enableTransition 是否使用动画
  \return 此对象在栈中的索引(从 0 开始计数)
  \sa DStackWidget::insertWidget
 */
int DStackWidget::pushWidget(QWidget *widget, bool enableTransition)
{
    insertWidget(depth(), widget, enableTransition);

    return depth() - 1;
}

/*!
  \brief 在指定的位置插入一个新的内容控件.
  
  需要注意的是如果第一个参数 index 不是栈的最后一个位置将不会显示动画
  
  \a index 要插入的索引
  \a widget 要插入的控件
  \a enableTransition 是否使用动画
  \sa DStackWidget::pushWidget
 */
void DStackWidget::insertWidget(int index, QWidget *widget, bool enableTransition)
{
    Q_D(DStackWidget);

    widget->setParent(this);
    d->widgetList.insert(index, widget);

    if(index == this->depth() - 1)
        setCurrentIndex(index, DAbstractStackWidgetTransition::Push, enableTransition);
    else
        d->setCurrentIndex(indexOf(currentWidget()));
}

/*!
  \brief 出栈指定的内容控件.
  
  如果所有参数都没有指定, 那么将只移除当前的内容控件, 一般是栈顶的控件.
  如果指定了参数 widget 那么计数将从控件 widget 在栈中的索引开始往栈顶方向计算移除 count 个内容控件
  
  \a widget 指定要移除的控件
  \a isDelete 是否删除控件对象
  \a count 指定要移除的控件的数量
  \a enableTransition 是否使用动画
 */
void DStackWidget::popWidget(QWidget *widget, bool isDelete, int count, bool enableTransition)
{
    Q_D(DStackWidget);

    int i = widget ? indexOf(widget) : currentIndex();

    if(i < 0 || i >= depth())
        return;

    bool current_widget_deleted = false;

    while(count-- > 0){
        QWidget *tmp_widget = d->widgetList[i];

        if(tmp_widget == currentWidget()) {
            current_widget_deleted = true;
        } else if(isDelete) {
            tmp_widget->deleteLater();
        }

        d->widgetList.removeAt(i);
    }

    if(current_widget_deleted && isDelete){
        if(enableTransition && depth()){
            d->trashWidgetList << d->currentWidget;
        } else if(d->currentWidget) {
            d->currentWidget->deleteLater();
            d->currentWidget = nullptr;
        }
    }

    setCurrentIndex(depth() - 1, DAbstractStackWidgetTransition::Pop, enableTransition && current_widget_deleted);
}

/*!
  \brief 移除所有控件(无动画).
 */
void DStackWidget::clear()
{
    Q_D(DStackWidget);

    qDeleteAll(d->widgetList.begin(), d->widgetList.end());
    d->widgetList.clear();
    d->setCurrentIndex(-1);
}

/*!
  \brief 获取指定控件的索引.

  \a widget 指定的控件
  \return 通过控件找到的索引
 */
int DStackWidget::indexOf(QWidget *widget) const
{
    Q_D(const DStackWidget);

    return d->widgetList.indexOf(widget);
}

/*!
  \brief 通过索引获取控件.

  \a index 指定索引
  \return 通过索引找到的控件
 */
QWidget *DStackWidget::getWidgetByIndex(int index) const
{
    Q_D(const DStackWidget);

    return d->widgetList[index];
}

/*!
  \brief DStackWidget::setTransition
  \a transition
  \sa DStackWidget::transition
 */
void DStackWidget::setTransition(DAbstractStackWidgetTransition *transition)
{
    Q_D(DStackWidget);

    if(d->transition){
        d->transition->deleteLater();
    }

    transition->setParent(this);
    d->transition = transition;

    connect(transition->animation(), &QVariantAnimation::stateChanged,
            this, [this, d](QAbstractAnimation::State newState, QAbstractAnimation::State oldState){
        if(newState == QVariantAnimation::Stopped) {
            busyChanged(false);
            qDeleteAll(d->trashWidgetList);
            d->trashWidgetList.clear();

            Q_EMIT switchWidgetFinished();
        } else if(oldState == QVariantAnimation::Stopped) {
            busyChanged(true);
        }
    });
}

/*!
  \brief DStackWidget::setAnimationDuration
  \a animationDuration
  \sa DStackWidget::animationDuration
 */
void DStackWidget::setAnimationDuration(int animationDuration)
{
    Q_D(DStackWidget);

    d->transition->animation()->setDuration(animationDuration);
}

/*!
  \brief DStackWidget::setAnimationType
  \a animationType
  \sa DStackWidget::animationType
 */
void DStackWidget::setAnimationType(QEasingCurve::Type animationType)
{
    Q_D(DStackWidget);

    d->transition->animation()->setEasingCurve(animationType);
}

DStackWidget::DStackWidget(DStackWidgetPrivate &dd, QWidget *parent):
    QWidget(parent),
    DObject(dd)
{
    d_func()->init();
}

void DStackWidget::setCurrentIndex(int currentIndex, DAbstractStackWidgetTransition::TransitionType type,
                                   bool enableTransition)
{
    Q_D(DStackWidget);

    if(enableTransition && currentWidget() && currentIndex >= 0) {
        DAbstractStackWidgetTransition::TransitionInfo info;
        info.stackWidget = this;
        info.oldWidget = currentWidget();
        info.newWidget = getWidgetByIndex(depth() - 1);
        info.type = type;

        d->setCurrentIndex(currentIndex);
        d->transition->beginTransition(info);
    } else {
        if(currentWidget()) {
            currentWidget()->hide();
        }

        d->setCurrentIndex(currentIndex);

        if(currentWidget()) {
            currentWidget()->move(0, 0);
            currentWidget()->show();
        }

        Q_EMIT switchWidgetFinished();
    }
}

void DStackWidget::setCurrentWidget(QWidget *currentWidget, DAbstractStackWidgetTransition::TransitionType type,
                                    bool enableTransition)
{
    setCurrentIndex(indexOf(currentWidget), type, enableTransition);
}

DWIDGET_END_NAMESPACE
