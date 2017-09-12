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
