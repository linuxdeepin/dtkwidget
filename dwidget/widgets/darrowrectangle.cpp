/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "darrowrectangle.h"
#include "dplatformwindowhandle.h"
#include "dapplication.h"
#include "private/darrowrectangle_p.h"

#ifdef Q_OS_LINUX
#include <X11/extensions/shape.h>
#include <QX11Info>
#endif
#include <QApplication>
#include <QScreen>
#include <QEvent>

DWIDGET_USE_NAMESPACE

DArrowRectangle::DArrowRectangle(ArrowDirection direction, QWidget * parent) :
    QWidget(parent),
    DObject(*new DArrowRectanglePrivate(direction, this))
{
    D_D(DArrowRectangle);

    d->init();
}

void DArrowRectangle::show(int x, int y)
{
    D_D(DArrowRectangle);

    d->show(x, y);
}

void DArrowRectangle::setContent(QWidget *content)
{
    D_D(DArrowRectangle);

    d->setContent(content);
}

QWidget *DArrowRectangle::getContent() const
{
    D_DC(DArrowRectangle);

    return d->m_content;
}

void DArrowRectangle::resizeWithContent()
{
    D_D(DArrowRectangle);

    d->resizeWithContent();
}

QSize DArrowRectangle::getFixedSize()
{
    D_D(DArrowRectangle);

    if (d->m_content)
    {
        qreal delta = (d->m_handle ? 0 : shadowBlurRadius() + shadowDistance()) + margin();

        switch(d->m_arrowDirection)
        {
        case ArrowLeft:
        case ArrowRight:
            return QSize(d->m_content->width() + delta * 2 + d->m_arrowHeight, d->m_content->height() + delta * 2);
        case ArrowTop:
        case ArrowBottom:
            return QSize(d->m_content->width() + delta * 2, d->m_content->height() + delta * 2 + d->m_arrowHeight);
        }
    }

    return QSize(0, 0);
}

void DArrowRectangle::move(int x, int y)
{
    D_D(DArrowRectangle);

    switch (d->m_arrowDirection)
    {
    case ArrowLeft:
    case ArrowRight:
        d->verticalMove(x, y);
        break;
    case ArrowTop:
    case ArrowBottom:
        d->horizontalMove(x, y);
        break;
    default:
        QWidget::move(x, y);
        break;
    }
}

// override methods
void DArrowRectangle::paintEvent(QPaintEvent *e)
{
    D_D(DArrowRectangle);

    d->paintEvent(e);
}

void DArrowRectangle::resizeEvent(QResizeEvent *e)
{
    D_D(DArrowRectangle);

    d->resizeEvent(e);
}

bool DArrowRectangle::event(QEvent *e)
{
    switch (e->type())
    {
    case QEvent::WindowDeactivate:  emit windowDeactivate();    break;
    default:;
    }

    return QWidget::event(e);
}

const QRect DArrowRectanglePrivate::currentScreenRect(const int x, const int y)
{
    for (QScreen *screen : qApp->screens())
        if (screen->geometry().contains(x, y))
            return screen->geometry();

    return QRect();
}

qreal DArrowRectangle::shadowYOffset() const
{
    D_DC(DArrowRectangle);

    return d->m_shadowYOffset;
}

void DArrowRectangle::setShadowYOffset(const qreal &shadowYOffset)
{
    D_D(DArrowRectangle);

    d->m_shadowYOffset = shadowYOffset;

    if (d->m_handle)
        d->m_handle->setShadowOffset(QPoint(d->m_shadowXOffset, shadowYOffset));
}

qreal DArrowRectangle::shadowXOffset() const
{
    D_DC(DArrowRectangle);

    return d->m_shadowXOffset;
}

void DArrowRectangle::setShadowXOffset(const qreal &shadowXOffset)
{
    D_D(DArrowRectangle);

    d->m_shadowXOffset = shadowXOffset;

    if (d->m_handle)
        d->m_handle->setShadowOffset(QPoint(shadowXOffset, d->m_shadowYOffset));
}

qreal DArrowRectangle::shadowDistance() const
{
    D_DC(DArrowRectangle);

    return d->m_shadowDistance;
}

void DArrowRectangle::setShadowDistance(const qreal &shadowDistance)
{
    D_D(DArrowRectangle);

    d->m_shadowDistance = shadowDistance;
}

qreal DArrowRectangle::shadowBlurRadius() const
{
    D_DC(DArrowRectangle);

    return d->m_shadowBlurRadius;
}

void DArrowRectangle::setShadowBlurRadius(const qreal &shadowBlurRadius)
{
    D_D(DArrowRectangle);

    d->m_shadowBlurRadius = shadowBlurRadius;

    if (d->m_handle)
        d->m_handle->setShadowRadius(shadowBlurRadius);
}

QColor DArrowRectangle::borderColor() const
{
    D_DC(DArrowRectangle);

    return d->m_borderColor;
}

void DArrowRectangle::setBorderColor(const QColor &borderColor)
{
    D_D(DArrowRectangle);

    d->m_borderColor = borderColor;

    if (d->m_handle)
        d->m_handle->setBorderColor(borderColor);
}

int DArrowRectangle::borderWidth() const
{
    D_DC(DArrowRectangle);

    return d->m_borderWidth;
}

void DArrowRectangle::setBorderWidth(int borderWidth)
{
    D_D(DArrowRectangle);

    d->m_borderWidth = borderWidth;

    if (d->m_handle)
        d->m_handle->setBorderWidth(borderWidth);
}

QColor DArrowRectangle::backgroundColor() const
{
    D_DC(DArrowRectangle);

    return d->m_backgroundColor;
}

DArrowRectangle::ArrowDirection DArrowRectangle::arrowDirection() const
{
    D_DC(DArrowRectangle);

    return d->m_arrowDirection;
}

void DArrowRectangle::setBackgroundColor(const QColor &backgroundColor)
{
    D_D(DArrowRectangle);

    d->m_backgroundColor = backgroundColor;

    if (d->m_handle && d->m_backgroundColor.toRgb().alpha() < 255) {
        if (!d->m_blurBackground) {
            d->m_blurBackground = new DBlurEffectWidget(this);
            d->m_blurBackground->setBlendMode(DBlurEffectWidget::BehindWindowBlend);
            d->m_blurBackground->resize(size());
            d->m_blurBackground->lower();
            d->m_blurBackground->show();
        }

        d->m_blurBackground->setMaskColor(d->m_backgroundColor);
    } else {
        if (d->m_blurBackground) {
            d->m_blurBackground->hide();
            d->m_blurBackground->setParent(0);
            delete d->m_blurBackground;
            d->m_blurBackground = Q_NULLPTR;
        }
    }
}

void DArrowRectangle::setBackgroundColor(DBlurEffectWidget::MaskColorType type)
{
    D_D(DArrowRectangle);

    if (d->m_blurBackground)
        d->m_blurBackground->setMaskColor(type);
}

int DArrowRectangle::radius() const
{
    D_DC(DArrowRectangle);

    return d->m_radius;
}

int DArrowRectangle::arrowHeight() const
{
    D_DC(DArrowRectangle);

    return d->m_arrowHeight;
}

int DArrowRectangle::arrowWidth() const
{
    D_DC(DArrowRectangle);

    return d->m_arrowWidth;
}

int DArrowRectangle::arrowX() const
{
    D_DC(DArrowRectangle);

    return d->m_arrowX;
}

int DArrowRectangle::arrowY() const
{
    D_DC(DArrowRectangle);

    return d->m_arrowY;
}

int DArrowRectangle::margin() const
{
    D_DC(DArrowRectangle);

    return d->m_margin;
}

void DArrowRectangle::setArrowDirection(ArrowDirection value)
{
    D_D(DArrowRectangle);

    d->m_arrowDirection = value;
}

void DArrowRectangle::setWidth(int value)
{
    setFixedWidth(value);
}

void DArrowRectangle::setHeight(int value)
{
    setFixedHeight(value);
}

void DArrowRectangle::setRadius(int value)
{
    D_D(DArrowRectangle);

    d->m_radius = value;
}

void DArrowRectangle::setArrowHeight(int value)
{
    D_D(DArrowRectangle);

    d->m_arrowHeight = value;
}

void DArrowRectangle::setArrowWidth(int value)
{
    D_D(DArrowRectangle);

    d->m_arrowWidth = value;
}

void DArrowRectangle::setArrowX(int value)
{
    D_D(DArrowRectangle);

    d->m_arrowX = value;
}

void DArrowRectangle::setArrowY(int value)
{
    D_D(DArrowRectangle);

    d->m_arrowY = value;
}

void DArrowRectangle::setMargin(int value)
{
    D_D(DArrowRectangle);

    d->m_margin = value;
}

QPainterPath DArrowRectanglePrivate::getLeftCornerPath()
{
    D_Q(DArrowRectangle);

    QRect rect = q->rect();

    if (!m_handle) {
        qreal delta = q->shadowBlurRadius() + q->shadowDistance();

        rect = rect.marginsRemoved(QMargins(delta, delta, delta, delta));
    }

    QPoint cornerPoint(rect.x(), rect.y() + (m_arrowY > 0 ? m_arrowY : (rect.height() / 2)));
    QPoint topLeft(rect.x() + m_arrowHeight, rect.y());
    QPoint topRight(rect.x() + rect.width(), rect.y());
    QPoint bottomRight(rect.x() + rect.width(), rect.y() + rect.height());
    QPoint bottomLeft(rect.x() + m_arrowHeight, rect.y() + rect.height());
    int radius = this->m_radius > (rect.height() / 2) ? (rect.height() / 2) : this->m_radius;

    QPainterPath border;
    border.moveTo(topLeft.x() + radius,topLeft.y());
    border.lineTo(topRight.x() - radius, topRight.y());
    border.arcTo(topRight.x() - 2 * radius, topRight.y(), 2 * radius, 2 * radius, 90, -90);
    border.lineTo(bottomRight.x(), bottomRight.y() - radius);
    border.arcTo(bottomRight.x() - 2 * radius, bottomRight.y() - 2 * radius, 2 * radius, 2 * radius, 0, -90);
    border.lineTo(bottomLeft.x() + radius,bottomLeft.y());
    border.arcTo(bottomLeft.x(),bottomLeft.y() - 2 * radius,2 * radius,2 * radius,-90,-90);
    border.lineTo(cornerPoint.x() + m_arrowHeight,cornerPoint.y() + m_arrowWidth / 2);
    border.lineTo(cornerPoint);
    border.lineTo(cornerPoint.x() + m_arrowHeight,cornerPoint.y() - m_arrowWidth / 2);
    border.lineTo(topLeft.x(),topLeft.y() + radius);
    border.arcTo(topLeft.x(),topLeft.y(),2 * radius,2 * radius,-180,-90);

    return border;
}

QPainterPath DArrowRectanglePrivate::getRightCornerPath()
{
    D_Q(DArrowRectangle);

    QRect rect = q->rect();

    if (!m_handle) {
        qreal delta = q->shadowBlurRadius() + q->shadowDistance();

        rect = rect.marginsRemoved(QMargins(delta, delta, delta, delta));
    }

    QPoint cornerPoint(rect.x() + rect.width(), rect.y() + (m_arrowY > 0 ? m_arrowY : rect.height() / 2));
    QPoint topLeft(rect.x(), rect.y());
    QPoint topRight(rect.x() + rect.width() - m_arrowHeight, rect.y());
    QPoint bottomRight(rect.x() + rect.width() - m_arrowHeight, rect.y() + rect.height());
    QPoint bottomLeft(rect.x(), rect.y() + rect.height());
    int radius = this->m_radius > (rect.height() / 2) ? rect.height() / 2 : this->m_radius;

    QPainterPath border;
    border.moveTo(topLeft.x() + radius, topLeft.y());
    border.lineTo(topRight.x() - radius,topRight.y());
    border.arcTo(topRight.x() - 2 * radius,topRight.y(),2 * radius,2 * radius,90,-90);
    border.lineTo(cornerPoint.x() - m_arrowHeight,cornerPoint.y() - m_arrowWidth / 2);
    border.lineTo(cornerPoint);
    border.lineTo(cornerPoint.x() - m_arrowHeight,cornerPoint.y() + m_arrowWidth / 2);
    border.lineTo(bottomRight.x(),bottomRight.y() - radius);
    border.arcTo(bottomRight.x() - 2 * radius,bottomRight.y() - 2 * radius,2 * radius,2 * radius,0,-90);
    border.lineTo(bottomLeft.x() + radius, bottomLeft.y());
    border.arcTo(bottomLeft.x(), bottomLeft.y() - 2 * radius, 2 * radius, 2 * radius, -90, -90);
    border.lineTo(topLeft.x(), topLeft.y() + radius);
    border.arcTo(topLeft.x(), topLeft.y(), 2 * radius, 2 * radius, 180, -90);

    return border;
}

QPainterPath DArrowRectanglePrivate::getTopCornerPath()
{
    D_Q(DArrowRectangle);

    QRect rect = q->rect();

    if (!m_handle) {
        qreal delta = q->shadowBlurRadius() + q->shadowDistance();

        rect = rect.marginsRemoved(QMargins(delta, delta, delta, delta));
    }

    QPoint cornerPoint(rect.x() + (m_arrowX > 0 ? m_arrowX : rect.width() / 2), rect.y());
    QPoint topLeft(rect.x(), rect.y() + m_arrowHeight);
    QPoint topRight(rect.x() + rect.width(), rect.y() + m_arrowHeight);
    QPoint bottomRight(rect.x() + rect.width(), rect.y() + rect.height());
    QPoint bottomLeft(rect.x(), rect.y() + rect.height());
    int radius = this->m_radius > (rect.height() / 2 - m_arrowHeight) ? rect.height() / 2 -m_arrowHeight : this->m_radius;

    QPainterPath border;
    border.moveTo(topLeft.x() + radius, topLeft.y());
    border.lineTo(cornerPoint.x() - m_arrowWidth / 2, cornerPoint.y() + m_arrowHeight);
    border.lineTo(cornerPoint);
    border.lineTo(cornerPoint.x() + m_arrowWidth / 2, cornerPoint.y() + m_arrowHeight);
    border.lineTo(topRight.x() - radius, topRight.y());
    border.arcTo(topRight.x() - 2 * radius, topRight.y(), 2 * radius, 2 * radius, 90, -90);
    border.lineTo(bottomRight.x(), bottomRight.y() - radius);
    border.arcTo(bottomRight.x() - 2 * radius, bottomRight.y() - 2 * radius, 2 * radius, 2 * radius, 0, -90);
    border.lineTo(bottomLeft.x() + radius, bottomLeft.y());
    border.arcTo(bottomLeft.x(), bottomLeft.y() - 2 * radius, 2 * radius, 2 * radius, - 90, -90);
    border.lineTo(topLeft.x(), topLeft.y() + radius);
    border.arcTo(topLeft.x(), topLeft.y(), 2 * radius, 2 * radius, 180, -90);

    return border;
}

QPainterPath DArrowRectanglePrivate::getBottomCornerPath()
{
    D_QC(DArrowRectangle);

    QRect rect = q->rect();

    if (!m_handle) {
        qreal delta = q->shadowBlurRadius() + q->shadowDistance();

        rect = rect.marginsRemoved(QMargins(delta, delta, delta, delta));
    }

    QPoint cornerPoint(rect.x() + (m_arrowX > 0 ? m_arrowX : qRound(double(rect.width()) / 2)), rect.y()  + rect.height());
    QPoint topLeft(rect.x(), rect.y());
    QPoint topRight(rect.x() + rect.width(), rect.y());
    QPoint bottomRight(rect.x() + rect.width(), rect.y() + rect.height() - m_arrowHeight);
    QPoint bottomLeft(rect.x(), rect.y() + rect.height() - m_arrowHeight);
    int radius = this->m_radius > (rect.height() / 2 - m_arrowHeight) ? rect.height() / 2 -m_arrowHeight : this->m_radius;

    QPainterPath border;
    border.moveTo(topLeft.x() + radius, topLeft.y());
    border.lineTo(topRight.x() - radius, topRight.y());
    border.arcTo(topRight.x() - 2 * radius, topRight.y(), 2 * radius, 2 * radius, 90, -90);
    border.lineTo(bottomRight.x(), bottomRight.y() - radius);
    border.arcTo(bottomRight.x() - 2 * radius, bottomRight.y() - 2 * radius, 2 * radius, 2 * radius, 0, -90);
    border.lineTo(cornerPoint.x() + m_arrowWidth / 2, cornerPoint.y() - m_arrowHeight);
    border.lineTo(cornerPoint);
    border.lineTo(cornerPoint.x() - m_arrowWidth / 2, cornerPoint.y() - m_arrowHeight);
    border.lineTo(bottomLeft.x() + radius, bottomLeft.y());
    border.arcTo(bottomLeft.x(), bottomLeft.y() - 2 * radius, 2 * radius, 2 * radius, -90, -90);
    border.lineTo(topLeft.x(), topLeft.y() + radius);
    border.arcTo(topLeft.x(), topLeft.y(), 2 * radius, 2 * radius, 180, -90);

    return border;
}

void DArrowRectanglePrivate::verticalMove(int x, int y)
{
    D_Q(DArrowRectangle);

    const QRect dRect = currentScreenRect(x, y);
    qreal delta = m_handle ? 0 : q->shadowBlurRadius() - q->shadowDistance();

    int lRelativeY = y - dRect.y() - (q->height() - delta) / 2;
    int rRelativeY = y - dRect.y() + (q->height() - delta) / 2 - dRect.height();
    int absoluteY = 0;

    if (lRelativeY < 0)//out of screen in top side
    {
        //arrowY use relative coordinates
        q->setArrowY(q->height() / 2 - delta + lRelativeY);
        absoluteY = dRect.y() - delta;
    }
    else if(rRelativeY > 0)//out of screen in bottom side
    {
        q->setArrowY(q->height() / 2 - delta / 2 + rRelativeY);
        absoluteY = dRect.y() + dRect.height() - q->height() + delta;
    }
    else
    {
        q->setArrowY(0);
        absoluteY = y - q->height() / 2;
    }

    switch (m_arrowDirection)
    {
    case DArrowRectangle::ArrowLeft:
        q->QWidget::move(x - delta, absoluteY);
        break;
    case DArrowRectangle::ArrowRight:
        q->QWidget::move(x - q->width() + delta, absoluteY);
        break;
    default:
        break;
    }
}

void DArrowRectanglePrivate::horizontalMove(int x, int y)
{
    D_Q(DArrowRectangle);

    const QRect dRect = currentScreenRect(x, y);
    qreal delta = m_handle ? 0 : q->shadowBlurRadius() - q->shadowDistance();

    int lRelativeX = x - dRect.x() - (q->width() - delta) / 2;
    int rRelativeX = x - dRect.x() + (q->width() - delta) / 2 - dRect.width();
    int absoluteX = 0;

    if (lRelativeX < 0)//out of screen in left side
    {
        //arrowX use relative coordinates
        q->setArrowX((q->width() - delta) / 2 + lRelativeX);
        absoluteX = dRect.x() - delta;
    }
    else if(rRelativeX > 0)//out of screen in right side
    {
        q->setArrowX(q->width() / 2 - delta * 2 + rRelativeX);
        absoluteX = dRect.x() + dRect.width() - q->width() + delta;
    }
    else
    {
        q->setArrowX(0);
        absoluteX = x - q->width() / 2;
    }

    switch (m_arrowDirection)
    {
    case DArrowRectangle::ArrowTop:
        q->QWidget::move(absoluteX, y - delta);
        break;
    case DArrowRectangle::ArrowBottom:
        q->QWidget::move(absoluteX, y - q->height() + delta);
        break;
    default:
        break;
    }
}

void DArrowRectanglePrivate::updateClipPath()
{
    D_Q(DArrowRectangle);

    if (!m_handle)
        return;

    QPainterPath path;

    switch (m_arrowDirection)
    {
    case DArrowRectangle::ArrowLeft:
        path = getLeftCornerPath();
        break;
    case DArrowRectangle::ArrowRight:
        path = getRightCornerPath();
        break;
    case DArrowRectangle::ArrowTop:
        path = getTopCornerPath();
        break;
    case DArrowRectangle::ArrowBottom:
        path = getBottomCornerPath();
        break;
    default:
        path = getRightCornerPath();
    }

    m_handle->setClipPath(path);
}

DArrowRectangle::~DArrowRectangle()
{

}



Dtk::Widget::DArrowRectanglePrivate::DArrowRectanglePrivate(DArrowRectangle::ArrowDirection direction, DArrowRectangle *q)
    : DObjectPrivate(q),

      m_arrowDirection(direction)
{

}

void DArrowRectanglePrivate::init()
{
    D_Q(DArrowRectangle);

    q->setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
    q->setAttribute(Qt::WA_TranslucentBackground);

    if (DApplication::isDXcbPlatform()) {
        m_handle = new DPlatformWindowHandle(q);
        m_handle->setTranslucentBackground(true);

        m_blurBackground = new DBlurEffectWidget(q);
        m_blurBackground->setMaskColor(DBlurEffectWidget::DarkColor);
        m_blurBackground->setBlendMode(DBlurEffectWidget::BehindWindowBlend);
    } else {
        DGraphicsGlowEffect *glowEffect = new DGraphicsGlowEffect;
        glowEffect->setBlurRadius(q->shadowBlurRadius());
        glowEffect->setDistance(q->shadowDistance());
        glowEffect->setXOffset(q->shadowXOffset());
        glowEffect->setYOffset(q->shadowYOffset());
        q->setGraphicsEffect(glowEffect);
    }
}

void DArrowRectanglePrivate::show(int x, int y)
{
    D_Q(DArrowRectangle);

    q->resizeWithContent();

    m_lastPos = QPoint(x, y);
    q->move(x, y);//Overload function
    if (!q->isVisible())
    {
        q->QWidget::show();
        q->QWidget::activateWindow();
    }

    q->update();
    updateClipPath();
}

void DArrowRectanglePrivate::setContent(QWidget *content)
{
    D_Q(DArrowRectangle);

    if (!content)
        return;

    m_content = content;
    m_content->setParent(q);
    m_content->show();

    qreal delta = (m_handle ? 0 : q->shadowBlurRadius() + q->shadowDistance()) + q->margin();

    q->resizeWithContent();

    switch(m_arrowDirection)
    {
    case DArrowRectangle::ArrowLeft:
        m_content->move(m_arrowHeight + delta, delta);
        break;
    case DArrowRectangle::ArrowRight:
        m_content->move(delta, delta);
        break;
    case DArrowRectangle::ArrowTop:
        m_content->move(delta, delta + m_arrowHeight);
        break;
    case DArrowRectangle::ArrowBottom:
        m_content->move(delta, delta);
        break;
    }

    q->update();
}

void DArrowRectanglePrivate::resizeWithContent()
{
    D_Q(DArrowRectangle);

    if (m_content.isNull())
        return;

    q->setFixedSize(q->getFixedSize());

#ifdef Q_OS_LINUX
    if (!m_handle) {
        XRectangle m_contentXRect;
        m_contentXRect.x = m_content->pos().x();
        m_contentXRect.y = m_content->pos().y();
        m_contentXRect.width = m_content->width();
        m_contentXRect.height = m_content->height();
        XShapeCombineRectangles(QX11Info::display(), q->winId(), ShapeInput,
                                0,
                                0,
                                &m_contentXRect, 1, ShapeSet, YXBanded);
    }
#endif
}

void DArrowRectanglePrivate::paintEvent(QPaintEvent *e)
{
    D_Q(DArrowRectangle);

    if (m_blurBackground)
        return;

    QPainter painter(q);

    if (m_handle) {
        painter.fillRect(e->rect(), m_backgroundColor);
    } else {
        painter.setRenderHint(QPainter::Antialiasing);

        QPainterPath border;

        switch (m_arrowDirection)
        {
        case DArrowRectangle::ArrowLeft:
            border = getLeftCornerPath();
            break;
        case DArrowRectangle::ArrowRight:
            border = getRightCornerPath();
            break;
        case DArrowRectangle::ArrowTop:
            border = getTopCornerPath();
            break;
        case DArrowRectangle::ArrowBottom:
            border = getBottomCornerPath();
            break;
        default:
            border = getRightCornerPath();
        }

        painter.setClipPath(border);
        painter.fillPath(border, QBrush(m_backgroundColor));

        QPen strokePen;
        strokePen.setColor(m_borderColor);
        strokePen.setWidth(m_borderWidth);
        painter.strokePath(border, strokePen);
    }
}

void DArrowRectanglePrivate::resizeEvent(QResizeEvent *e)
{
    D_Q(DArrowRectangle);

    q->QWidget::resizeEvent(e);

    if (m_blurBackground) {
        m_blurBackground->resize(e->size());
    }

    updateClipPath();
}
