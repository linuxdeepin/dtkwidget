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

#include <QBoxLayout>
#include <QEvent>
#include <QDebug>
#include <QResizeEvent>

#include "dthememanager.h"
#include "dboxwidget.h"
#include "private/dboxwidget_p.h"

DWIDGET_BEGIN_NAMESPACE

/*!
    \~english \class DBoxWidget
    \~english \brief The DBoxWidget widget provides a Adaptive child widget size
    when child widget size changed.
*/

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
 * \~english \class DBoxWidget
 * \~english \brief The DBoxWidget class provides widget born with QBoxLayout set.
 *
 * \~english Since an widget with QBoxLayout set is very common use case in Qt
 * \~english programming, yet very tedious, DBoxWidget is built to ease that pain.
 *
 * \~english Also, DBoxWidget will calculate the size it needs automatically, so you don't
 * \~english need to set width for DHBoxWidget or height for DVBoxLayout.
 *
 * \~english \see DHBoxWidget and DVBoxWidget
 */

/*!
 * \~chinese \class DBoxWidget
 * \~chinese \brief DBoxWidget提供了一个自适应子控件大小的Widget.
 *
 * \~chinese 在Qt编程中，使用QBoxLayout设置控件是很常见的，但是是非常令人乏味的.
 * \~chinese DBoxWidget是为了减轻这一痛苦而构建的.
 *
 * \~chinese DBoxWidget会自动计算它所需要的大小，所以你不需要为DBoxWidget设置宽或高.
 */

/*!
 * \~english \brief DBoxWidget::DBoxWidget constructs an instance of DBoxWidget.
 * \~english \param direction is the direction used by the internal QBoxLayout.
 * \~english \param parent is passed to QFrame constructor.
 */

/*!
 * \~chinese \brief DBoxWidget的构造函数
 *
 * \~chinese \param direction 是设置内部QBoxLayout使用的方向
 * \~chinese \param parent 传递给QFrame的构造函数
 */
DBoxWidget::DBoxWidget(QBoxLayout::Direction direction, QWidget *parent):
    QFrame(parent),
    DObject(*new DBoxWidgetPrivate(this))
{
    DThemeManager::registerWidget(this);

    d_func()->layout->setDirection(direction);
    d_func()->init();
}

/*!
 * \~english \property DBoxWidget::direction
 * \~english \brief This property holds the direction of the internal QBoxLayout.
 */

/*!
 * \~chinese \property DBoxWidget::direction
 * \~chinese 这个属性返回当前QBoxLayout使用的方向
 * \return QBoxLayout::Direction 当前的方向
 */
QBoxLayout::Direction DBoxWidget::direction() const
{
    Q_D(const DBoxWidget);

    return d->layout->direction();
}

/*!
 * \~english \property DBoxWidget::layout
 * \~english \brief This property holds the internal layout object.
 *
 * This property can be used to get the internal layout, so you can set
 * some extra properties on the layout to match the custom needs.
 */

/*!
 * \~chinese \property DBoxWidget::layout
 * \~chinese \brief 这个属性会返回当前使用的布局对象
 * \return QBoxLayout*
 */
QBoxLayout *DBoxWidget::layout() const
{
    Q_D(const DBoxWidget);

    return d->layout;
}

/*!
 * \~english \brief DBoxWidget::addWidget adds widget to the internal layout.
 * \~english \param widget is the widget to be added.
 */

/*!
 * \~chinese \brief 添加控件到布局中
 *
 * \param widget 要添加的QWidget对象
 */
void DBoxWidget::addWidget(QWidget *widget)
{
    layout()->addWidget(widget);
}

/*!
 * \~chinese \brief 设置QBoxLayout当前的方向
 *
 * \param direction
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
 * \~chinese \brief 强制更新大小
 *
 * \param size
 */
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
 * \~english \brief DBoxWidget::sizeHint reimplemented from QWidget::sizeHint().
 * \~english \return the recommended size of this widget.
 */

/*!\reimp */
QSize DBoxWidget::sizeHint() const
{
    Q_D(const DBoxWidget);

    return d->layout->sizeHint();
}

/*!
 * \~english \class DHBoxWidget
 * \~english \brief The DHBoxWidget class is DBoxWidget with DBoxWidget::direction set to
 *  QBoxLayout::LeftToRight.
 */

/*!
 * \~english \brief DHBoxWidget::DHBoxWidget constructs an instance of DHBoxWidget.
 * \~english \param parent is passed to DBoxWidget constructor.
 */

/*!
 * \~chinese \class DHBoxWidget
 * \~chinese \brief 是设置成水平方向的DBoxWidget
 */

/*!
 * \~chinese \brief DHBoxWidget的构造函数
 *
 * \param parent
 */
DHBoxWidget::DHBoxWidget(QWidget *parent):
    DBoxWidget(QBoxLayout::LeftToRight, parent)
{

}

/*!
 * \~english \class DVBoxWidget
 * \~english \brief The DVBoxWidget class is DBoxWidget with DBoxWidget::direction set to
 *  QBoxLayout::TopToBottom.
 */

/*!
 * \~english \brief DVBoxWidget::DVBoxWidget constructs an instance of DVBoxWidget.
 * \~english \param parent is passed to DBoxWidget constructor.
 */

/*!
 * \~chinese \class DVoxWidget
 * \~chinese \brief 是设置成垂直方向的DBoxWidget
 */

/*!
 * \~chinese \brief DVBoxWidget的构造函数
 *
 * \param parent
 */
DVBoxWidget::DVBoxWidget(QWidget *parent):
    DBoxWidget(QBoxLayout::TopToBottom, parent)
{

}

DWIDGET_END_NAMESPACE
