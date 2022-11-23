// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dimageviewer.h"
#include "private/dimageviewer_p.h"
#include "private/dimagevieweritems_p.h"

#include <DSvgRenderer>

#include <QFileInfo>
#include <QGestureEvent>
#include <QImageReader>
#include <QMimeDatabase>
#include <QPinchGesture>

DGUI_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

DImageViewerPrivate::DImageViewerPrivate(DImageViewer *qq)
    : DObjectPrivate(qq)
{
}

DImageViewerPrivate::~DImageViewerPrivate()
{
    if (contentItem) {
        q_func()->scene()->removeItem(contentItem);
        delete contentItem;
    }
}

void DImageViewerPrivate::init()
{
    D_Q(DImageViewer);

    q->setScene(new QGraphicsScene(q));
    q->setContentsMargins(0, 0, 0, 0);
    q->setMouseTracking(true);
    q->setAcceptDrops(false);
    q->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    q->setDragMode(QGraphicsView::ScrollHandDrag);
    q->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    q->setResizeAnchor(QGraphicsView::AnchorViewCenter);
    q->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    q->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    q->setFrameShape(QFrame::Shape::NoFrame);

    q->setAttribute(Qt::WA_AcceptTouchEvents);
    q->grabGesture(Qt::PinchGesture);
    q->grabGesture(Qt::SwipeGesture);
    q->grabGesture(Qt::PanGesture);
    q->viewport()->setCursor(Qt::ArrowCursor);
}

ImageType DImageViewerPrivate::detectImageType(const QString &fileName) const
{
    ImageType type = ImageType::ImageTypeBlank;
    if (!fileName.isEmpty()) {
        QFileInfo info(fileName);
        QString typeStr = info.suffix().toLower();

        QMimeDatabase db;
        QMimeType contentType = db.mimeTypeForFile(fileName, QMimeDatabase::MatchContent);
        QMimeType exntensionType = db.mimeTypeForFile(fileName, QMimeDatabase::MatchExtension);

        QImageReader reader(fileName);
        int nSize = reader.imageCount();

        if (typeStr == "svg" && DSvgRenderer(fileName).isValid()) {
            type = ImageType::ImageTypeSvg;
        } else if ((typeStr == "mng") || ((typeStr == "gif") && nSize > 1) || (typeStr == "webp" && nSize > 1) ||
                   ((contentType.name().startsWith("image/gif")) && nSize > 1) ||
                   ((exntensionType.name().startsWith("image/gif")) && nSize > 1) ||
                   (contentType.name().startsWith("video/x-mng")) || (exntensionType.name().startsWith("video/x-mng"))) {
            type = ImageType::ImageTypeDynamic;
        } else {
            type = ImageType::ImageTypeStatic;
        }
    }

    return type;
}

void DImageViewerPrivate::resetItem(ImageType type)
{
    D_Q(DImageViewer);

    if (type != imageType) {
        if (contentItem) {
            q->scene()->removeItem(contentItem);
            delete contentItem;
            contentItem = nullptr;
        }

        imageType = type;
    } else if (contentItem) {
        return;
    }

    switch (imageType) {
        case ImageTypeStatic:
            contentItem = new DGraphicsPixmapItem;
            break;
        case ImageTypeDynamic:
            contentItem = new DGraphicsMovieItem;
            break;
        case ImageTypeSvg:
            contentItem = new DGraphicsSVGItem;
            break;
        default:
            break;
    }

    if (contentItem) {
        q->scene()->addItem(contentItem);
    }
}

QImage DImageViewerPrivate::loadImage(const QString &fileName, ImageType type) const
{
    QImage image;
    switch (type) {
        case ImageTypeStatic:
        case ImageTypeDynamic:
            image = QImageReader(fileName).read();
            break;
        case ImageTypeSvg: {
            DSvgRenderer render(fileName);
            if (render.isValid()) {
                image = render.toImage(render.defaultSize());
            }
            break;
        }
        default:
            break;
    }

    return image;
}

void DImageViewerPrivate::handleGestureEvent(QGestureEvent *gesture)
{
    // todo
}

void DImageViewerPrivate::pinchTriggered(QPinchGesture *gestrue)
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
    d->init();
    setImage(image);
}

DImageViewer::DImageViewer(const QString &fileName, QWidget *parent)
    : DGraphicsView(parent)
    , DObject(*new DImageViewerPrivate(this))
{
    D_D(DImageViewer);
    d->init();
    setFileName(fileName);
}

DImageViewer::~DImageViewer()
{
    clear();
}

QImage DImageViewer::image() const
{
    D_DC(DImageViewer);
    return d->contentImage;
}

void DImageViewer::setImage(const QImage &image)
{
    D_D(DImageViewer);
    d->resetItem(ImageTypeStatic);
    Q_ASSERT(d->contentItem);

    auto staticItem = static_cast<DGraphicsPixmapItem *>(d->contentItem);
    staticItem->setPixmap(QPixmap::fromImage(image));
    d->contentImage = image;
    emit imageChanged(d->contentImage);
}

QString DImageViewer::fileName() const
{
    D_DC(DImageViewer);
    return d->fileName;
}

void DImageViewer::setFileName(const QString &fileName)
{
    D_D(DImageViewer);

    ImageType type = d->detectImageType(fileName);
    d->resetItem(type);

    if (ImageTypeBlank == d->imageType) {
        clear();
        return;
    }

    Q_ASSERT(d->contentItem);
    d->fileName = fileName;
    d->contentImage = d->loadImage(d->fileName, d->imageType);

    switch (d->imageType) {
        case ImageTypeStatic: {
            auto staticItem = static_cast<DGraphicsPixmapItem *>(d->contentItem);
            staticItem->setPixmap(QPixmap::fromImage(d->contentImage));
            break;
        }
        case ImageTypeDynamic: {
            auto movieItem = static_cast<DGraphicsMovieItem *>(d->contentItem);
            movieItem->setFileName(d->fileName);
            break;
        }
        case ImageTypeSvg: {
            auto svgItem = static_cast<DGraphicsSVGItem *>(d->contentItem);
            svgItem->setFileName(d->fileName);
            break;
        }
        default:
            break;
    }

    update();
    emit fileNameChanged(d->fileName);
    emit imageChanged(d->contentImage);
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
    D_DC(DImageViewer);
    return d->rotateAngle;
}

void DImageViewer::resetRotateAngle()
{
    D_D(DImageViewer);
    d->rotateAngle = 0;

    // todo

    emit rotateAngleChanged(d->rotateAngle);
}

void DImageViewer::clear()
{
    D_D(DImageViewer);

    if (d->contentItem) {
        scene()->removeItem(d->contentItem);
        delete d->contentItem;
        d->contentItem = nullptr;
    }
    scene()->clear();
    resetRotateAngle();

    d->fileName.clear();
    d->contentImage = QImage();
    d->imageType = ImageTypeBlank;

    emit fileNameChanged(d->fileName);
    emit imageChanged(d->contentImage);
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
