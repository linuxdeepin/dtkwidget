// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dimageviewer.h"
#include "private/dimageviewer_p.h"
#include "private/dimagevieweritems_p.h"

#include <QGestureEvent>
#include <QImageReader>
#include <QPinchGesture>

DWIDGET_BEGIN_NAMESPACE

DImageViewerPrivate::DImageViewerPrivate(DImageViewer *qq)
    : DObjectPrivate(qq)
{
}

DImageViewerPrivate::~DImageViewerPrivate()
{
    // todo
}

void DImageViewerPrivate::init()
{
    // todo
}

void DImageViewerPrivate::handleGestureEvent(QGestureEvent *gesture)
{
    // todo
}

void DImageViewerPrivate::pinchTriggered(QPinchGesture *gestrue)
{
    // todo
}

void DImageViewerPrivate::_q_imageLoadFinished()
{
    // todo
}

void DImageViewerPrivate::_q_pinchAnimeFinished()
{
    // todo
}

DImageViewer::DImageViewer(QWidget *parent)
    : DGraphicsView(parent)
    , DObject(*new DImageViewerPrivate(this))
{
    D_D(DImageViewer);
    d->init();
}

DImageViewer::DImageViewer(const QImage &image, QWidget *parent)
    : DGraphicsView(parent)
    , DObject(*new DImageViewerPrivate(this))
{
    D_D(DImageViewer);
    d->contentImage = image;
    d->init();
}

DImageViewer::DImageViewer(const QString &fileName, QWidget *parent)
    : DGraphicsView(parent)
    , DObject(*new DImageViewerPrivate(this))
{
    D_D(DImageViewer);
    d->fileName = fileName;
    d->init();
}

DImageViewer::~DImageViewer()
{
    // todo
}

QImage DImageViewer::image() const
{
    // todo
}

void DImageViewer::setImage(const QImage &image)
{
    // todo
}

QString DImageViewer::fileName() const
{
    // todo
}

void DImageViewer::setFileName(const QString &fileName)
{
    // todo
}

qreal DImageViewer::scaleFactor() const
{
    // todo
}

void DImageViewer::setScaleFactor(qreal factor)
{
    // todo
}

void DImageViewer::autoFitImage()
{
    // todo
}

void DImageViewer::fitToWidget()
{
    // todo
}

void DImageViewer::fitNormalSize()
{
    // todo
}

void DImageViewer::rotateClockwise()
{
    // todo
}

void DImageViewer::rotateCounterclockwise()
{
    // todo
}

qreal DImageViewer::rotateAngle() const
{
    // todo
}

void DImageViewer::resetRotateAngle()
{
    // todo
}

void DImageViewer::clear()
{
    // todo
}

void DImageViewer::centerOn(qreal x, qreal y)
{
    // todo
}

void DImageViewer::mouseDoubleClickEvent(QMouseEvent *event)
{
    DGraphicsView::mouseDoubleClickEvent(event);
}

void DImageViewer::mouseReleaseEvent(QMouseEvent *event)
{
    DGraphicsView::mouseDoubleClickEvent(event);
}

void DImageViewer::mousePressEvent(QMouseEvent *event)
{
    DGraphicsView::mousePressEvent(event);
}

void DImageViewer::mouseMoveEvent(QMouseEvent *event)
{
    DGraphicsView::mouseMoveEvent(event);
}

void DImageViewer::wheelEvent(QWheelEvent *event)
{
    DGraphicsView::wheelEvent(event);
}

void DImageViewer::resizeEvent(QResizeEvent *event)
{
    DGraphicsView::resizeEvent(event);
}

void DImageViewer::timerEvent(QTimerEvent *event)
{
    DGraphicsView::timerEvent(event);
}

void DImageViewer::drawBackground(QPainter *painter, const QRectF &rect)
{
    DGraphicsView::drawBackground(painter, rect);
}

bool DImageViewer::event(QEvent *event)
{
    return DGraphicsView::event(event);
}

DWIDGET_END_NAMESPACE

#include "moc_dimageviewer.cpp"
