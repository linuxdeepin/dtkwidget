// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "private/dindeterminateprogressbar_p.h"

#include <DStyle>

#include <QPainter>
#include <QTimer>
#include <QPropertyAnimation>
#include <QDebug>
#include <QPainterPath>

const int SPOT_WIDGET_WIDTH = 200;

DIndeterminateProgressbarPrivate::DIndeterminateProgressbarPrivate(DIndeterminateProgressbar *qq)
    : DObjectPrivate(qq)
    , m_sliderWidget(new QWidget(qq))
    , m_timer(new QTimer(qq))
    , m_leftToRight(true)
    , m_spotWidget(new QWidget(qq))
    , m_animation(new QPropertyAnimation(m_spotWidget, "pos", qq))
{
}

DIndeterminateProgressbar::DIndeterminateProgressbar(QWidget *parent)
    : QWidget(parent)
    , DObject(*new DIndeterminateProgressbarPrivate(this))
{
    D_D(DIndeterminateProgressbar);
    d->m_spotWidget->setFixedSize(SPOT_WIDGET_WIDTH, height());
    d->m_spotWidget->move(-SPOT_WIDGET_WIDTH, 0);

    d->m_sliderWidget->setFixedWidth(150);
    d->m_sliderWidget->move(0, 0);

    d->m_timer->setInterval(10);
    static int step = 0;
    connect(d->m_timer, &QTimer::timeout, this, [this, d]() {
        if (d->m_sliderWidget->geometry().right() >= rect().right()) {
            d->m_leftToRight = false;
        }

        if (d->m_sliderWidget->geometry().left() <= rect().left()) {
            d->m_leftToRight = true;
        }

        d->m_leftToRight ? step += 2 : step -= 2;
        d->m_sliderWidget->move(step, 0);
        update();
    });
    d->m_timer->start();
}

void DIndeterminateProgressbar::resizeEvent(QResizeEvent *e)
{
    D_D(DIndeterminateProgressbar);
    d->m_sliderWidget->setFixedHeight(height());
    d->m_spotWidget->setFixedSize(SPOT_WIDGET_WIDTH, height());

    d->m_animation->setStartValue(QPoint(-SPOT_WIDGET_WIDTH, 0));
    d->m_animation->setEndValue(QPoint(rect().right(), 0));
    d->m_animation->setDuration(3000);
    d->m_animation->setEasingCurve(QEasingCurve::InQuad);
    d->m_animation->start();
    connect(d->m_animation, &QPropertyAnimation::finished, this, [d]() {
        d->m_animation->start();
    });
    QWidget::resizeEvent(e);
}

void DIndeterminateProgressbar::paintEvent(QPaintEvent *e)
{
    D_D(DIndeterminateProgressbar);
    QWidget::paintEvent(e);
    QPainter p(this);

    p.setRenderHint(QPainter::Antialiasing);
    int radius;
    this->height() <= DTK_WIDGET_NAMESPACE::DStyle::pixelMetric(style(), DTK_WIDGET_NAMESPACE::DStyle::PM_FrameRadius) * 2
            ? radius = height() / 2
            : radius = DTK_WIDGET_NAMESPACE::DStyle::pixelMetric(style(), DTK_WIDGET_NAMESPACE::DStyle::PM_FrameRadius);

    p.setBrush(QColor(0, 0, 0, int(0.1 * 255)));
    p.setPen(Qt::NoPen);

    p.drawRoundedRect(rect(), radius, radius);

    QPen pen;
    pen.setWidth(1);
    pen.setColor(QColor(0, 0, 0, int(0.2 * 255)));
    p.setBrush(Qt::NoBrush);
    p.setPen(pen);
    p.drawRoundedRect(rect().marginsRemoved(QMargins(1, 1, 1, 1)), radius, radius);

    p.setPen(Qt::NoPen);
    p.setBrush(palette().highlight().color());
    p.drawRoundedRect(d->m_sliderWidget->geometry(), radius, radius);

    pen.setColor(QColor(0, 0, 0, int(0.3 * 255)));
    p.setBrush(Qt::NoBrush);
    p.setPen(pen);
    p.drawRoundedRect(d->m_sliderWidget->geometry().marginsRemoved(QMargins(1, 1, 1, 1)), radius, radius);

    if (d->m_sliderWidget->width() < d->m_spotWidget->width() / 2)
        return;

    QPointF pointStart(d->m_spotWidget->geometry().left(), d->m_spotWidget->geometry().center().y());
    QPointF pointEnd(d->m_spotWidget->geometry().right(), d->m_spotWidget->geometry().center().y());

    QColor shadowColor(0, 0, 0, int(0.15 * 255));
    QColor spotColor(255, 255, 255, int(0.5 * 255));
    QColor highLightColor(palette().highlight().color());

    QLinearGradient linear(pointStart, pointEnd);
    linear.setColorAt(0, highLightColor);
    linear.setColorAt(0.35, shadowColor);
    linear.setColorAt(0.5, spotColor);
    linear.setColorAt(0.65, shadowColor);
    linear.setColorAt(1, highLightColor);
    linear.setSpread(QGradient::PadSpread);
    linear.setInterpolationMode(QLinearGradient::InterpolationMode::ColorInterpolation);

    p.setBrush(linear);
    p.setPen(Qt::NoPen);

    QPainterPath clipPath;
    clipPath.addRoundedRect(d->m_sliderWidget->geometry(), radius, radius);
    p.setClipPath(clipPath);
    p.setClipping(true);
    p.drawRoundedRect(d->m_spotWidget->geometry().marginsRemoved(QMargins(2, 2, 2, 2)), radius, radius);
    p.setClipping(false);
}
