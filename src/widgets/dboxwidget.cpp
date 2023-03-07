// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
@~english
  @class Dtk::Widget::DBoxWidget
  \inmodule dtkwidget

  @brief The DBoxWidget class provides widget born with QBoxLayout set.

  Since an widget with QBoxLayout set is very common use case in Qt
  programming, yet very tedious, DBoxWidget is built to ease that pain.
  
  Also, DBoxWidget will calculate the size it needs automatically, so you don't
  need to set width for DHBoxWidget or height for DVBoxLayout.
  
  @sa DHBoxWidget and DVBoxWidget
 */

/*!
@~english
  @brief DBoxWidget::DBoxWidget constructs an instance of DBoxWidget.

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
@~english
  @property DBoxWidget::direction

  @brief This property holds the direction of the internal QBoxLayout.

  @return QBoxLayout::Direction Current direction
 */
QBoxLayout::Direction DBoxWidget::direction() const
{
    Q_D(const DBoxWidget);

    return d->layout->direction();
}

/*!
@~english
  @brief This property holds the internal layout object.
  
  This property can be used to get the internal layout, so you can set
  some extra properties on the layout to match the custom needs.

  @return QBoxLayout*
 */
QBoxLayout *DBoxWidget::layout() const
{
    Q_D(const DBoxWidget);

    return d->layout;
}

/*!
@~english
  @brief DBoxWidget::addWidget adds widget to the internal layout.

  \a widget is the widget to be added.
 */
void DBoxWidget::addWidget(QWidget *widget)
{
    layout()->addWidget(widget);
}

/*!
@~english
  @brief Sets the current direction of QBoxLayout
  
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
@~english
  @brief When the direction is QBoxLayout::TopToBottom or QBoxLayout::BottomToTop, 
  the fixed height will use the passed height and set the minimum width to the passed width.
  Otherwise, the passed width is used as the fixed width and the height is the minimum height.
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
@~english
  \reimp
  @brief DBoxWidget::sizeHint reimplemented from QWidget::sizeHint().
  @return the recommended size of this widget.
 */
QSize DBoxWidget::sizeHint() const
{
    Q_D(const DBoxWidget);

    return d->layout->sizeHint();
}

/*!
@~english
  @class Dtk::Widget::DHBoxWidget
  \inmodule dtkwidget

  @brief The DHBoxWidget class is DBoxWidget with DBoxWidget::direction set to
   QBoxLayout::LeftToRight.
 */

/*!
@~english
  @brief DHBoxWidget::DHBoxWidget constructs an instance of DHBoxWidget.

  \a parent is passed to DBoxWidget constructor.
 */
DHBoxWidget::DHBoxWidget(QWidget *parent):
    DBoxWidget(QBoxLayout::LeftToRight, parent)
{

}

/*!
@~english
  @class Dtk::Widget::DVBoxWidget
  \inmodule dtkwidget

  @brief The DVBoxWidget class is DBoxWidget with DBoxWidget::direction set to
   QBoxLayout::TopToBottom.
 */

/*!
@~english
  @brief DVBoxWidget::DVBoxWidget constructs an instance of DVBoxWidget.

  \a parent is passed to DBoxWidget constructor.
 */
DVBoxWidget::DVBoxWidget(QWidget *parent):
    DBoxWidget(QBoxLayout::TopToBottom, parent)
{

}

DWIDGET_END_NAMESPACE
