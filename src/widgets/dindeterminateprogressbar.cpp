// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "private/dindeterminateprogressbar_p.h"

#include <DStyle>
#include <DGuiApplicationHelper>

#include <QPainter>
#include <QTimer>
#include <QPropertyAnimation>
#include <QDebug>
#include <QPainterPath>

DGUI_USE_NAMESPACE

const int SPOT_WIDGET_WIDTH = 200;

DIndeterminateProgressbarPrivate::DIndeterminateProgressbarPrivate(DIndeterminateProgressbar *qq)
    : DObjectPrivate(qq)
    , m_sliderWidget(new QWidget(qq))
    , m_timer(new QTimer(qq))
    , m_leftToRight(true)
    , m_spotWidget(nullptr)
    , m_animation(nullptr)
{
    if (!ENABLE_ANIMATIONS && !ENABLE_ANIMATION_PROGRESSBAR)
        return;

    m_spotWidget = new QWidget(qq);
    m_animation = new QPropertyAnimation(m_spotWidget, "pos", qq);
}

DIndeterminateProgressbar::DIndeterminateProgressbar(QWidget *parent)
    : QWidget(parent)
    , DObject(*new DIndeterminateProgressbarPrivate(this))
{
    D_D(DIndeterminateProgressbar);

    if (ENABLE_ANIMATIONS && ENABLE_ANIMATION_PROGRESSBAR) {
        d->m_spotWidget->setFixedSize(SPOT_WIDGET_WIDTH, height());
        d->m_spotWidget->move(-SPOT_WIDGET_WIDTH, 0);
    }

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

    if (!ENABLE_ANIMATIONS || !ENABLE_ANIMATION_PROGRESSBAR)
        QWidget::resizeEvent(e);

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

    bool isDarkType = DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::DarkType;
    QColor color = isDarkType ? QColor(255, 255, 255, int(0.1 * 255)) : QColor(0, 0, 0, int(0.1 * 255));
    p.setBrush(color);
    p.setPen(Qt::NoPen);

    p.drawRoundedRect(rect(), radius, radius);

    QPen pen;
    pen.setWidth(1);
    pen.setColor(color);
    p.setBrush(Qt::NoBrush);
    p.setPen(pen);
    p.drawRoundedRect(rect(), radius, radius);

    p.setPen(Qt::NoPen);
    p.setBrush(palette().highlight().color());
    p.drawRoundedRect(d->m_sliderWidget->geometry(), radius, radius);

    QColor highLightColor(palette().highlight().color());
    auto borderColor = isDarkType ? DGuiApplicationHelper::adjustColor(highLightColor, 0, 0, +10, 0, 0, 0, 0)
                                  : DGuiApplicationHelper::adjustColor(highLightColor, 0, 0, -20, 0, 0, 0, -20);
    pen.setColor(borderColor);
    p.setBrush(Qt::NoBrush);
    p.setPen(pen);
    p.drawRoundedRect(d->m_sliderWidget->geometry(), radius, radius);

    if (!ENABLE_ANIMATIONS || !ENABLE_ANIMATION_PROGRESSBAR)
        return;

    if (d->m_sliderWidget->width() < d->m_spotWidget->width() / 2)
        return;

    QPointF pointStart(d->m_spotWidget->geometry().left(), d->m_spotWidget->geometry().center().y());
    QPointF pointEnd(d->m_spotWidget->geometry().right(), d->m_spotWidget->geometry().center().y());

    QColor spotColor = DGuiApplicationHelper::adjustColor(highLightColor, 0, +30, +30, 0, 0, 0, 0);

    QLinearGradient linear(pointStart, pointEnd);
    linear.setColorAt(0, highLightColor);
    linear.setColorAt(0.5, spotColor);
    linear.setColorAt(1, highLightColor);
    linear.setSpread(QGradient::PadSpread);
    linear.setInterpolationMode(QLinearGradient::InterpolationMode::ColorInterpolation);

    p.setBrush(linear);
    p.setPen(Qt::NoPen);

    QPainterPath clipPath;
    clipPath.addRoundedRect(d->m_sliderWidget->geometry().marginsRemoved(QMargins(1, 1, 1, 1)), radius - 1, radius - 1);
    p.setClipPath(clipPath);
    p.setClipping(true);
    p.drawRoundedRect(d->m_spotWidget->geometry(), radius, radius);
    p.setClipping(false);
}
