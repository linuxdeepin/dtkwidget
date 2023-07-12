// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QEvent>
#include <QResizeEvent>

#include "denhancedwidget.h"

DWIDGET_BEGIN_NAMESPACE

class DEnhancedWidgetPrivate
{
    explicit DEnhancedWidgetPrivate(DEnhancedWidget *qq): q_ptr(qq) {}

    QSize old_size;
    QPoint old_pos;
    QWidget *target = NULL;
    bool enabled = true;

    DEnhancedWidget *q_ptr;

    Q_DECLARE_PUBLIC(DEnhancedWidget)
};

/*!
@~english
  @property Dtk::Widget::DEnhancedWidget::target
  @brief The target widget to be monitored. Each DEnhancedWidget object can only be associated to a QWidget object.
  When target changes, all signals have no more relationship with the old target.
  @code
  QWidget *w1 = new QWidget();
  w1->show();
  DEnhancedWidget *ew = new DEnhancedWidget(w1);
  connect(ew, &DEnhancedWidget::xChanged, [] (int x) {
     qDebug() << x;
  });
  QWidget *w2 = new QWidget();
  ew.setTarget(w2);
  w1->move(100, 100);
  @endcode
  There will be no printing when moving widget w1.
  @code
  w2->show();
  w2->move(100, 100);
  @endcode
  There will be printing when moving widget w2.
  Although the xChanged signal has been connected before the target is set to w2, the signal can be received when the w2 position changes.
  @note read or write
 */

/*!
@~english
  @property Dtk::Widget::DEnhancedWidget::enabled
  @brief If "true", event filter of the target widget sends a signal. Otherwise, uninstall the event filter, signal will not be sent when the widge property is changed.
  @note read or write
 */

/*!
@~english
  @fn void DEnhancedWidget::xChanged(int x)

  @param[in] x the changed x axis coordinate
  The signal is sent when the x axis coordinate changed.
 */

/*!
@~english
  @fn void DEnhancedWidget::yChanged(int y)

  @param[in] y the changed y axis coordinate
  The signal is sent when the y axis coordinate changed.
 */

/*!
@~english
  @fn void DEnhancedWidget::positionChanged(const QPoint &point)

  @param[in] point the changed position coordinates
  The signal is sent when the position coordinates of target widget changed.
 */

/*!
@~english
  @fn void DEnhancedWidget::widthChanged(int width)

  @param[in] width the changed width
  The signal is sent when the width of target widget changed.
 */

/*!
@~english
  @fn void DEnhancedWidget::heightChanged(int height)

  @param[in] height the changed height
  The signal is sent when the height of target widget changed.
 */

/*!
@~english
  @fn void DEnhancedWidget::sizeChanged(const QSize &size)

  @param[in] size the changed size
  The signal is sent when the size of target widget changed.
 */

/*!
@~english
  @fn void DEnhancedWidget::targetChanged(QWidget *target)

  @param[in] target the target widget
  The signal is sent when the property DEnhancedWidget::target changed.
 */

/*!
@~english
  @fn void DEnhancedWidget::enabledChanged(bool enabled)

  @param[in] enabled usable or not
  The signal is sent when the property DEnhancedWidget::enabled changed.
 */

/*!
@~english
  @fn void DEnhancedWidget::showed()
  This signal is sent when the target widget displayed.
 */

/*!
@~english
  @class Dtk::Widget::DEnhancedWidget
  @inmodule dtkwidget

  There are only few signals in Qwidget to notify the properties changes related to the window. The changes in
  commonly used properties such as x, y, width, height can only be known by inheriting the corresponding events.
  DEnhancedWidget monitors events of the target widget by the event filter. After receiving the corresponding event, 
  it is converted into its own signal and then emitted, realizing signal expansion of the Qwidget object.
 */

/*!
@~english
  @brief DEnhancedWidget::DEnhancedWidget 构造函数.

  @param[in] w initially specified target widget
  @param[in] parent pass directly to the base class
  @sa DEnhancedWidget::target
  @sa QObject::parent
 */
DEnhancedWidget::DEnhancedWidget(QWidget *w, QObject *parent):
    QObject(parent),
    d_ptr(new DEnhancedWidgetPrivate(this))
{
    if (w) {
        Q_D(DEnhancedWidget);

        d->target = w;
        w->installEventFilter(this);
    }
}

DEnhancedWidget::~DEnhancedWidget()
{
    delete d_ptr;
}

QWidget *DEnhancedWidget::target() const
{
    Q_D(const DEnhancedWidget);

    return d->target;
}

bool DEnhancedWidget::enabled() const
{
    Q_D(const DEnhancedWidget);

    return d->enabled;
}

/*!
@~english
  @brief Set the target widget and update the event filter when the target widget changes.
  @param[in] target the target widget
  @sa DEnhancedWidget::target
 */
void DEnhancedWidget::setTarget(QWidget *target)
{
    Q_D(DEnhancedWidget);

    if (d->target == target) {
        return;
    }

    if (d->enabled && d->target) {
        d->target->removeEventFilter(this);
    }
    if (d->enabled && target) {
        target->installEventFilter(this);
    }
    d->target = target;
    Q_EMIT targetChanged(target);
}

/*!
@~english
  @brief Set the availability of the widget and install or uninstall the event filter according to the situation.
  @param[in] enabled usable or not
  @sa DEnhancedWidget::enabled
 */
void DEnhancedWidget::setEnabled(bool enabled)
{
    Q_D(DEnhancedWidget);

    if (d->enabled == enabled) {
        return;
    }

    if (d->target) {
        if (enabled) {
            d->target->installEventFilter(this);
        } else {
            d->target->removeEventFilter(this);
        }
    }

    d->enabled = enabled;
    Q_EMIT enabledChanged(enabled);
}

bool DEnhancedWidget::eventFilter(QObject *o, QEvent *e)
{
    Q_D(DEnhancedWidget);

    if (o == d->target) {
        switch (e->type()) {
        case QEvent::Resize: {
            QResizeEvent *event = static_cast<QResizeEvent *>(e);
            if (event) {
                QSize size = event->size();

                if (size.width() != d->old_size.width()) {
                    Q_EMIT widthChanged(size.width());
                }

                if (size.height() != d->old_size.height()) {
                    Q_EMIT heightChanged(size.height());
                }

                if (size != d->old_size) {
                    Q_EMIT sizeChanged(size);
                }

                d->old_size = size;
            }

            break;
        }
        case QEvent::Move: {
            QMoveEvent *event = static_cast<QMoveEvent *>(e);

            if (event) {
                QPoint pos = event->pos();

                if (pos.x() != d->old_pos.x()) {
                    Q_EMIT xChanged(pos.x());
                }

                if (pos.y() != d->old_pos.y()) {
                    Q_EMIT yChanged(pos.y());
                }

                if (pos != d->old_pos) {
                    Q_EMIT positionChanged(pos);
                }

                d->old_pos = pos;
            }

            break;
        }
        case QEvent::Show: {
            Q_EMIT showed();

            break;
        }
        default:
            break;
        }
    }

    return false;
}

DEnhancedWidget::DEnhancedWidget(DEnhancedWidgetPrivate *dd, QWidget *w, QObject *parent):
    QObject(parent),
    d_ptr(dd)
{
    if (w) {
        w->installEventFilter(this);
    }
}

DWIDGET_END_NAMESPACE
