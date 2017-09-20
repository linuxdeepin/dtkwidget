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

/*!
    \class DBoxWidget
    \brief The DBoxWidget widget provides a Adaptive child widget size
    when child widget size changed.

    \inmodule deepin-tool-kit
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
 * \class DBoxWidget
 * \brief The DBoxWidget class provides widget born with QBoxLayout set.
 *
 * Since an widget with QBoxLayout set is very common use case in Qt programming,
 * yet very tedious, DBoxWidget is built to ease that pain.
 *
 * Also, DBoxWidget will calculate the size it needs automatically, so you don't
 * need to set width for DHBoxWidget or height for DVBoxLayout.
 *
 * \see DHBoxWidget and DVBoxWidget
 */

/*!
 * \brief DBoxWidget::DBoxWidget constructs an instance of DBoxWidget.
 * \param direction is the direction used by the internal QBoxLayout.
 * \param parent is passed to QFrame constructor.
 */
DBoxWidget::DBoxWidget(QBoxLayout::Direction direction, QWidget *parent):
    QFrame(parent),
    DObject(*new DBoxWidgetPrivate(this))
{
    D_THEME_INIT_WIDGET(DBoxWidget);

    d_func()->layout->setDirection(direction);
    d_func()->init();
}

/*!
 * \property DBoxWidget::direction
 * \brief This property holds the direction of the internal QBoxLayout.
 */
QBoxLayout::Direction DBoxWidget::direction() const
{
    Q_D(const DBoxWidget);

    return d->layout->direction();
}

/*!
 * \property DBoxWidget::layout
 * \brief This property holds the internal layout object.
 *
 * This property can be used to get the internal layout, so you can set
 * some extra properties on the layout to match the custom needs.
 */
QBoxLayout *DBoxWidget::layout() const
{
    Q_D(const DBoxWidget);

    return d->layout;
}

/*!
 * \brief DBoxWidget::addWidget adds widget to the internal layout.
 * \param widget is the widget to be added.
 */
void DBoxWidget::addWidget(QWidget *widget)
{
    layout()->addWidget(widget);
}

void DBoxWidget::setDirection(QBoxLayout::Direction direction)
{
    Q_D(DBoxWidget);

    if (d->layout->direction() == direction)
        return;

    d->layout->setDirection(direction);
    Q_EMIT directionChanged(direction);
}

void DBoxWidget::updateSize(const QSize &size)
{
    if(direction() == QBoxLayout::TopToBottom || QBoxLayout::BottomToTop) {
        setFixedHeight(size.height());

        if(size.width() > minimumWidth())
            setMinimumWidth(qMin(size.width(), maximumWidth()));
    } else {
        setFixedWidth(size.width());

        if(size.height() > minimumHeight())
            setMinimumHeight(qMin(size.height(), maximumHeight()));
    }
}

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
 * \brief DBoxWidget::sizeHint reimplemented from QWidget::sizeHint().
 * \return the recommended size of this widget.
 */
QSize DBoxWidget::sizeHint() const
{
    Q_D(const DBoxWidget);

    return d->layout->sizeHint();
}

/*!
 * \class DHBoxWidget
 * \brief The DHBoxWidget class is DBoxWidget with DBoxWidget::direction set to
 *  QBoxLayout::LeftToRight.
 */

/*!
 * \brief DHBoxWidget::DHBoxWidget constructs an instance of DHBoxWidget.
 * \param parent is passed to DBoxWidget constructor.
 */
DHBoxWidget::DHBoxWidget(QWidget *parent):
    DBoxWidget(QBoxLayout::LeftToRight, parent)
{

}

/*!
 * \class DVBoxWidget
 * \brief The DVBoxWidget class is DBoxWidget with DBoxWidget::direction set to
 *  QBoxLayout::TopToBottom.
 */

/*!
 * \brief DVBoxWidget::DVBoxWidget constructs an instance of DVBoxWidget.
 * \param parent is passed to DBoxWidget constructor.
 */
DVBoxWidget::DVBoxWidget(QWidget *parent):
    DBoxWidget(QBoxLayout::TopToBottom, parent)
{

}

DWIDGET_END_NAMESPACE
