// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
#include "private/dbounceanimation_p.h"
#include <QPropertyAnimation>
#include <QEvent>
#include <QDebug>
#include <QAbstractScrollArea>
#include <QScrollBar>
#include <QWheelEvent>
#include <QTimer>

DBounceAnimationPrivate::DBounceAnimationPrivate(DBounceAnimation *qq)
    : DObjectPrivate (qq)
    , m_animation(nullptr)
    , m_animationTarget(nullptr)
    , m_deltaSum(0)
{
}

DBounceAnimation::DBounceAnimation(QObject *parent)
    : QObject(parent)
    , DObject(*new DBounceAnimationPrivate(this))
{
}

void DBounceAnimation::setAnimationTarget(QAbstractScrollArea *w)
{
    D_D(DBounceAnimation);
    if (!w)
        return;

    if (d->m_animationTarget == w)
        return;

    d->m_animationTarget = w;
}

void DBounceAnimation::setAniMationEnable(bool enable)
{
    D_D(DBounceAnimation);
    enable ? d->m_animationTarget->installEventFilter(this)
           : d->m_animationTarget->removeEventFilter(this);
}

bool DBounceAnimation::eventFilter(QObject *o, QEvent *e)
{
    D_D(DBounceAnimation);
    if (e->type() == QEvent::Wheel) {
        if (auto absscroll = dynamic_cast<QAbstractScrollArea *>(o)) {
            if (auto wheelEvent = dynamic_cast<QWheelEvent *>(e)) {
                if (absscroll->verticalScrollBar()->value() <= 0 || absscroll->verticalScrollBar()->value() >= absscroll->verticalScrollBar()->maximum()) {
                    d->m_deltaSum += wheelEvent->delta();
                    bounceBack(wheelEvent->angleDelta().x() == 0 ? Qt::Vertical : Qt::Horizontal);
                }
            }
        }
    }

    return false;
}

void DBounceAnimation::bounceBack(Qt::Orientation orientation)
{
    D_D(DBounceAnimation);
    if (d->m_animation)
        return;

    if (orientation & Qt::Vertical && d->m_animationTarget->verticalScrollBar()->maximum() == d->m_animationTarget->verticalScrollBar()->minimum())
        return;

    if (orientation & Qt::Horizontal && d->m_animationTarget->horizontalScrollBar()->maximum() == d->m_animationTarget->horizontalScrollBar()->minimum())
        return;

    d->m_animation = new QPropertyAnimation(this);
    d->m_animation->setTargetObject(d->m_animationTarget->viewport());
    d->m_animation->setPropertyName("pos");
    d->m_animation->setDuration(100);
    d->m_animation->setEasingCurve(QEasingCurve::InQuart);
    d->m_animation->setStartValue(QPoint(d->m_animationTarget->viewport()->x(), d->m_animationTarget->viewport()->y()));

    QTimer::singleShot(100, this, [this, d, orientation]() {

        if (orientation & Qt::Vertical) {
            d->m_animation->setEndValue(
                QPoint(d->m_animationTarget->viewport()->x(), d->m_animationTarget->viewport()->y() + d->m_deltaSum / 16));
        } else {
            d->m_animation->setEndValue(
                QPoint(d->m_animationTarget->viewport()->x() + d->m_deltaSum / 16, d->m_animationTarget->viewport()->y()));
        }

        d->m_animation->start();

        connect(d->m_animation, &QPropertyAnimation::finished, this, [d]() {
            if (d->m_animation->direction() == QPropertyAnimation::Backward) {
                delete d->m_animation;
                d->m_animation = nullptr;
                return;
            }

            d->m_animation->setDirection(QPropertyAnimation::Direction::Backward);
            d->m_animation->setDuration(1000);
            d->m_animation->start(QPropertyAnimation::DeleteWhenStopped);
            d->m_deltaSum = 0;
        });
    });
}
