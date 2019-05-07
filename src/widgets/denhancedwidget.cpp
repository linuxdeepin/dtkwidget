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
 * \~chinese \brief DEnhancedWidget::DEnhancedWidget
 * \~chinese QWidget 中只有少量的信号用于通知和窗口相关的属性变化，常用的如 x y width height
 * \~chinese 等属性的改变只能通过继承重写对应的事件才能知晓。DEnhancedWidget 中通过使用事件
 * \~chinese 过滤器监听目标控件的事件，收到对应事件之后转换为自身的信号发射出来，以此来实现对
 * \~chinese QWidget 对象的信号扩展。
 * \~chinese \param w
 * \~chinese \param parent
 * \~chinese \sa DAnchorsBase
 */

/*!
 * \~chinese \property DEnhancedWidget::target
 * \~chinese \brief 要监听的目标控件，每一个 DEnhancedWidget 对象只能关联到一个 QWidget 对象
 * \~chinese 当 target 改变后，所有的信号和旧的 target 都不再有任何关系。
 * \code
 * QWidget *w1 = new QWidget();
 * w1->show();
 * DEnhancedWidget *ew = new DEnhancedWidget(w1);
 * connect(ew, &DEnhancedWidget::xChanged, [] (int x) {
 *    qDebug() << x;
 * });
 * QWidget *w2 = new QWidget();
 * ew.setTarget(w2);
 * w1->move(100, 100);
 * \endcode
 * \~chinese 移动控件 w1 时不会有任何打印
 * \code
 * w2->show();
 * w2->move(100, 100);
 * \endcode
 * \~chinese 移动控件 w2 时会有打印
 * \~chinese 虽然将 target 设置为 w2 之前就已经连接了 x 改变的信号，不过 w2 位置改变时可以收到信号
 * \~chinese \note 可读可写
 */

/*!
 * \~chinese \property DEnhancedWidget::enabled
 * \~chinese \brief 值为 true 时过滤 target 控件的事件发送信号。否则卸载事件过滤器，控件属性
 * \~chinese 改变时也不会发出信号。
 * \~chinese \note 可读可写
 */

/*!
  * \fn void DEnhancedWidget::xChanged(int x)
  * \~chinese 这个信号在目标控件的 x 坐标改变后被发送
  */

/*!
  * \fn void DEnhancedWidget::yChanged(int y)
  * \~chinese 这个信号在目标控件的 y 坐标改变后被发送
  */

/*!
  * \fn void DEnhancedWidget::positionChanged(const QPoint &point)
  * \~chinese 这个信号在目标控件的坐标改变后被发送
  */

/*!
  * \fn void DEnhancedWidget::widthChanged(int width)
  * \~chinese 这个信号在目标控件的宽度改变后被发送
  */

/*!
  * \fn void DEnhancedWidget::heightChanged(int height)
  * \~chinese 这个信号在目标控件的高度改变后被发送
  */

/*!
  * \fn void DEnhancedWidget::sizeChanged(const QSize &size)
  * \~chinese 这个信号在目标控件的大小改变后被发送
  */

/*!
  * \fn void DEnhancedWidget::targetChanged(QWidget *target)
  * \~chinese 这个信号在属性 DEnhancedWidget::target 被改变后发送
  */

/*!
  * \fn void DEnhancedWidget::enabledChanged(bool enabled)
  * \~chinese 这个信号在属性 DEnhancedWidget::enabled 被改变后发送
  */

/*!
  * \fn void DEnhancedWidget::showed()
  * \~chinese 这个信号在目标控件显示后发送
  */

/*!
 * \~chinese \brief DEnhancedWidget::DEnhancedWidget
 * \~chinese \param w 初始时指定的 target 控件
 * \~chinese \param parent 父对象，直接传递给基类
 * \~chinese \sa DEnhancedWidget::target
 * \~chinese \sa QObject::parent
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
 * \brief DEnhancedWidget::setTarget
 * \param target
 * \sa DEnhancedWidget::target
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
 * \brief DEnhancedWidget::setEnabled
 * \param enabled
 * \sa DEnhancedWidget::enabled
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
