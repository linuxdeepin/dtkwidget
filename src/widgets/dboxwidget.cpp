/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QBoxLayout>
#include <QEvent>
#include <QDebug>
#include <QResizeEvent>

#include "dthememanager.h"
#include "dboxwidget.h"
#include "private/dboxwidget_p.h"

DWIDGET_BEGIN_NAMESPACE


DBoxWidgetPrivate::DBoxWidgetPrivate(DBoxWidget *qq):
    DObjectPrivate(qq),
    layout(new QBoxLayout(QBoxLayout::TopToBottom))
{

}

void DBoxWidgetPrivate::init()
{
    Q_Q(DBoxWidget);

    layout->setMargin(0);
    layout->setSpacing(0);

    q->setLayout(layout);
    q->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

/*!
  \class Dtk::Widget::DBoxWidget
  \inmodule dtkwidget

  \brief The DBoxWidget class provides widget born with QBoxLayout set.
  \brief DBoxWidget提供了一个自适应子控件大小的Widget.
  
  在Qt编程中，使用QBoxLayout设置控件是很常见的，DBoxWidget提供了方便的封装，会根据需要的大小自动
  设置DBoxWidget的宽高。

  Since an widget with QBoxLayout set is very common use case in Qt
  programming, yet very tedious, DBoxWidget is built to ease that pain.
  
  Also, DBoxWidget will calculate the size it needs automatically, so you don't
  need to set width for DHBoxWidget or height for DVBoxLayout.
  
  \sa DHBoxWidget and DVBoxWidget
 */

/*!
  \brief DBoxWidget的构造函数.
  \brief DBoxWidget::DBoxWidget constructs an instance of DBoxWidget.

  \a direction 是设置内部QBoxLayout使用的方向
  \a parent 传递给QFrame的构造函数
  \a direction is the direction used by the internal QBoxLayout.
  \a parent is passed to QFrame constructor.
 */
DBoxWidget::DBoxWidget(QBoxLayout::Direction direction, QWidget *parent):
    QFrame(parent),
    DObject(*new DBoxWidgetPrivate(this))
{
    d_func()->layout->setDirection(direction);
    d_func()->init();
}

/*!
  \property DBoxWidget::direction

  这个属性返回当前QBoxLayout使用的方向
  \brief This property holds the direction of the internal QBoxLayout.

  \return QBoxLayout::Direction 当前的方向
 */
QBoxLayout::Direction DBoxWidget::direction() const
{
    Q_D(const DBoxWidget);

    return d->layout->direction();
}

/*!
  \brief 这个属性会返回当前使用的布局对象
  \brief This property holds the internal layout object.
  
  This property can be used to get the internal layout, so you can set
  some extra properties on the layout to match the custom needs.

  \return QBoxLayout*
 */
QBoxLayout *DBoxWidget::layout() const
{
    Q_D(const DBoxWidget);

    return d->layout;
}

/*!
  \brief 调用QBoxLayout的addWidget方法将QWidget添加到布局中
  \brief DBoxWidget::addWidget adds widget to the internal layout.

  \a widget 要添加的QWidget对象
  \a widget is the widget to be added.
 */
void DBoxWidget::addWidget(QWidget *widget)
{
    layout()->addWidget(widget);
}

/*!
  \brief 设置QBoxLayout当前的方向
  
  \a direction
 */
void DBoxWidget::setDirection(QBoxLayout::Direction direction)
{
    Q_D(DBoxWidget);

    if (d->layout->direction() == direction)
        return;

    d->layout->setDirection(direction);
    Q_EMIT directionChanged(direction);
}

/*!
  \brief 当方向是QBoxLayout::TopToBottom或者QBoxLayout::BottomToTop时，
  固定高度将使用传入的高度，并设置最小宽度为传入的宽度。
  否则将使用传入的宽度当做固定宽度，高度为最小高度。
  \a size
 */
void DBoxWidget::updateSize(const QSize &size)
{
    if(direction() == QBoxLayout::TopToBottom || direction() == QBoxLayout::BottomToTop) {
        setFixedHeight(size.height());

        if(size.width() > minimumWidth())
            setMinimumWidth(qMin(size.width(), maximumWidth()));
    } else {
        setFixedWidth(size.width());

        if(size.height() > minimumHeight())
            setMinimumHeight(qMin(size.height(), maximumHeight()));
    }
}

/*!\reimp */
bool DBoxWidget::event(QEvent *ee)
{
    Q_D(const DBoxWidget);

    if(ee->type() == QEvent::LayoutRequest) {
        if(size() != d->layout->sizeHint()) {
            updateSize(d->layout->sizeHint());
            updateGeometry();
        }
    } else if(ee->type() == QEvent::Resize) {
        Q_EMIT sizeChanged(size());
    } else if(ee->type() == QEvent::ChildAdded) {
        updateSize(d->layout->sizeHint());
    } else if(ee->type() == QEvent::ChildRemoved) {
        updateSize(d->layout->sizeHint());
    } else if(ee->type() == QEvent::Show) {
        updateSize(d->layout->sizeHint());
    }

    return QWidget::event(ee);
}

/*!
  \reimp
  \brief DBoxWidget::sizeHint reimplemented from QWidget::sizeHint().
  \return the recommended size of this widget.
 */
QSize DBoxWidget::sizeHint() const
{
    Q_D(const DBoxWidget);

    return d->layout->sizeHint();
}

/*!
  \class Dtk::Widget::DHBoxWidget
  \inmodule dtkwidget

  \brief The DHBoxWidget class is DBoxWidget with DBoxWidget::direction set to
   QBoxLayout::LeftToRight.
  \brief 是设置成水平方向的DBoxWidget
 */

/*!
  \brief DHBoxWidget的构造函数
  \brief DHBoxWidget::DHBoxWidget constructs an instance of DHBoxWidget.

  \a parent is passed to DBoxWidget constructor.
 */
DHBoxWidget::DHBoxWidget(QWidget *parent):
    DBoxWidget(QBoxLayout::LeftToRight, parent)
{

}

/*!
  \class Dtk::Widget::DVBoxWidget
  \inmodule dtkwidget

  \brief The DVBoxWidget class is DBoxWidget with DBoxWidget::direction set to
   QBoxLayout::TopToBottom.
  \brief 是设置成垂直方向的DBoxWidget
 */

/*!
  \brief DVBoxWidget的构造函数
  \brief DVBoxWidget::DVBoxWidget constructs an instance of DVBoxWidget.

  \a parent is passed to DBoxWidget constructor.
 */
DVBoxWidget::DVBoxWidget(QWidget *parent):
    DBoxWidget(QBoxLayout::TopToBottom, parent)
{

}

DWIDGET_END_NAMESPACE
