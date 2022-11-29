// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dimagevieweritems_p.h"

#include <QObject>
#include <QMovie>
#include <QPainter>
#include <QStyleOption>

DGUI_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

DGraphicsPixmapItem::DGraphicsPixmapItem(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent)
{
}

DGraphicsPixmapItem::DGraphicsPixmapItem(const QPixmap &pixmap, QGraphicsItem *parent)
    : QGraphicsPixmapItem(pixmap, parent)
{
}

DGraphicsPixmapItem::~DGraphicsPixmapItem()
{
    prepareGeometryChange();
}

void DGraphicsPixmapItem::setPixmap(const QPixmap &pixmap)
{
    cachePixmap = qMakePair(cachePixmap.first, pixmap);
    QGraphicsPixmapItem::setPixmap(pixmap);
}

void DGraphicsPixmapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    const QTransform ts = painter->transform();

    if (ts.type() == QTransform::TxScale && ts.m11() < 1) {
        QPixmap currentPixmap = pixmap();
        if (currentPixmap.width() < 10000 && currentPixmap.height() < 10000) {
            painter->setRenderHint(QPainter::SmoothPixmapTransform, (transformationMode() == Qt::SmoothTransformation));

            QPixmap pixmap;
            if (qIsNull(cachePixmap.first - ts.m11())) {
                pixmap = cachePixmap.second;
            } else {
                pixmap = currentPixmap.transformed(painter->transform(), transformationMode());
                cachePixmap = qMakePair(ts.m11(), pixmap);
            }

            pixmap.setDevicePixelRatio(painter->device()->devicePixelRatioF());
            painter->resetTransform();
            painter->drawPixmap(offset() + QPointF(ts.dx(), ts.dy()), pixmap);
            painter->setTransform(ts);
        } else {
            QGraphicsPixmapItem::paint(painter, option, widget);
        }
    } else {
        QGraphicsPixmapItem::paint(painter, option, widget);
    }
}

DGraphicsMovieItem::DGraphicsMovieItem(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent)
{
    movie = new QMovie;
    QObject::connect(movie, &QMovie::frameChanged, this, &DGraphicsMovieItem::onMovieFrameChanged);
}

DGraphicsMovieItem::DGraphicsMovieItem(const QString &fileName, QGraphicsItem *parent)
    : QGraphicsPixmapItem(fileName, parent)
{
    movie = new QMovie;
    QObject::connect(movie, &QMovie::frameChanged, this, &DGraphicsMovieItem::onMovieFrameChanged);
    setFileName(fileName);
}

DGraphicsMovieItem::~DGraphicsMovieItem()
{
    prepareGeometryChange();

    movie->stop();
    movie->deleteLater();
    movie = nullptr;
}

void DGraphicsMovieItem::onMovieFrameChanged()
{
    setPixmap(movie->currentPixmap());
}

void DGraphicsMovieItem::setFileName(const QString &fileName)
{
    movie->stop();
    movie->setFileName(fileName);
    movie->start();

    update();
}

DGraphicsSVGItem::DGraphicsSVGItem(QGraphicsItem *parent)
    : QGraphicsObject(parent)
{
    renderer = new DSvgRenderer(this);
}

DGraphicsSVGItem::DGraphicsSVGItem(const QString &fileName, QGraphicsItem *parent)
    : QGraphicsObject(parent)
{
    renderer = new DSvgRenderer(this);
    renderer->load(fileName);
    updateDefaultSize();
}
    
void DGraphicsSVGItem::setFileName(const QString &fileName)
{
    // Clear cached image.
    CacheMode mode = cacheMode();
    setCacheMode(QGraphicsItem::NoCache);
    renderer->load(fileName);
    updateDefaultSize();

    setCacheMode(mode);
    update();
}

QRectF DGraphicsSVGItem::boundingRect() const
{
    return imageRect;
}

void DGraphicsSVGItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (!renderer->isValid()) {
        return;
    }
    renderer->render(painter, imageRect);
}

int DGraphicsSVGItem::type() const
{
    return Type;
}

void DGraphicsSVGItem::updateDefaultSize()
{
    QRectF bounds = QRectF(QPointF(0, 0), renderer->defaultSize());

    if (bounds.size() != imageRect.size()) {
        prepareGeometryChange();
        imageRect.setSize(bounds.size());
    }
}

DWIDGET_END_NAMESPACE
