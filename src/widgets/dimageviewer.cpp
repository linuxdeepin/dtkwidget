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
#include <QVariantAnimation>
#include <QGraphicsRectItem>
#include <qmath.h>

DGUI_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

const qreal MAX_SCALE_FACTOR = 20.0;
const qreal MIN_SCALE_FACTOR = 0.02;

/*!
  \class Dtk::Widget::DImageViewerPrivate
  \internal
 */

/*! \internal */
DImageViewerPrivate::DImageViewerPrivate(DImageViewer *qq)
    : DObjectPrivate(qq)
{
}

/*! \internal */
DImageViewerPrivate::~DImageViewerPrivate()
{
    if (pinchData) {
        delete pinchData;
    }

    if (cropData) {
        // Crop image item may be lose parent when setting a null image, need released manually.
        // Must release before content item.
        if (cropData->cropItem) {
            q_func()->scene()->removeItem(cropData->cropItem);
            delete cropData->cropItem;
        }

        delete cropData;
    }

    // Proxy item and content item will be autodelete in scene.
    q_func()->scene()->clear();
}

/*! \internal */
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

    // The proxy item store rotation info, and clip the content item when setting the crop rect.
    proxyItem = new QGraphicsRectItem;
    proxyItem->setFlags(proxyItem->flags() | QGraphicsItem::ItemClipsChildrenToShape);
    // Not draw proxy item rect.
    proxyItem->setPen(QPen(Qt::NoPen));
    proxyItem->setBrush(QBrush(Qt::NoBrush));
    q->scene()->addItem(proxyItem);
}

/*! \internal */
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

/*! \internal */
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
        resetCropData();

        contentItem->setPos(0, 0);
        contentItem->resetTransform();
        proxyItem->setRotation(0);
        proxyItem->resetTransform();
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
        contentItem->setParentItem(proxyItem);
        proxyItem->setRotation(0);
        proxyItem->resetTransform();
        proxyItem->setRect(contentItem->boundingRect());
    }
}

/*! \internal */
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

/*! \internal */
void DImageViewerPrivate::updateItemAndSceneRect()
{
    D_Q(DImageViewer);
    if (proxyItem) {
        QRectF itemRect = proxyItem->mapRectToScene(proxyItem->boundingRect());
        // The image rect top left point will be changed after rotation,
        // needs to adjust top left point to the accurate position.
        if (itemRect.left() != 0 || itemRect.top() != 0) {
            proxyItem->moveBy(-itemRect.left(), -itemRect.top());
            itemRect.moveTopLeft(QPointF(0, 0));
        }
        q->setSceneRect(itemRect);
    }
}

/*! \internal */
bool DImageViewerPrivate::rotatable() const
{
    return (ImageTypeBlank != imageType && ImageTypeDynamic != imageType);
}

/*! \internal */
bool DImageViewerPrivate::isRotateVertical() const
{
    if (proxyItem) {
        // Check item rotation angle around 90 and 270 degrees.
        qreal angle = abs(proxyItem->rotation());
        return (angle > 35 && angle < 135) || (angle > 225 && angle < 315);
    }
    return false;
}

/*! \internal */
qreal DImageViewerPrivate::validRotateAngle(qreal angle) const
{
    // From Qt help doc: A rotation transformation of 180 degrees
    // and/or 360 degrees is treated as a scaling transformation.
    if (qFuzzyCompare(abs(angle), 180)) {
        // 180 degrees needs adjustment to avoid transformation errors.
        return angle + (angle < 0 ? -0.00001 : 0.0001);
    } else {
        return angle;
    }
}

/*! \internal */
qreal DImageViewerPrivate::validScaleFactor(qreal scale) const
{
    return qBound(MIN_SCALE_FACTOR, scale, MAX_SCALE_FACTOR);
}

/*! \internal */
qreal DImageViewerPrivate::widgetRelativeScale() const
{
    D_QC(DImageViewer);

    QRectF sceneRect = q->sceneRect();
    if (((1.0 * q->width()) / q->height()) > (sceneRect.width() / sceneRect.height())) {
        return (1.0 * q->height()) / sceneRect.height();
    } else {
        return (1.0 * q->width()) / sceneRect.width();
    }
}

/*! \internal */
void DImageViewerPrivate::checkPinchData()
{
    if (!pinchData) {
        pinchData = new PinchData;
    }
}

/*! \internal */
void DImageViewerPrivate::handleGestureEvent(QGestureEvent *gesture)
{
    if (QGesture *pinch = gesture->gesture(Qt::PinchGesture)) {
        pinchTriggered(static_cast<QPinchGesture *>(pinch));
    }
}

/*! \internal */
void DImageViewerPrivate::pinchTriggered(QPinchGesture *gesture)
{
    D_Q(DImageViewer);
    // Must check pinch data before use it.
    checkPinchData();

    maxTouchPoints = 2;
    QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();
    if (changeFlags & QPinchGesture::ScaleFactorChanged) {
        QPoint pos = q->mapFromGlobal(gesture->centerPoint().toPoint());
        if (abs(gesture->scaleFactor() - 1) > 0.006) {
            q->scaleAtPoint(pos, gesture->scaleFactor());
        }
    }

    // Two finger rotation.
    if (changeFlags & QPinchGesture::RotationAngleChanged) {
        if (!rotatable() || maxTouchPoints > 2) {
            return;
        }

        // Can not use gesture while rotation animation running.
        if (pinchData->isAnimationRotating) {
            gesture->setRotationAngle(gesture->lastRotationAngle());
            return;
        }

        qreal rotationDelta = gesture->rotationAngle() - gesture->lastRotationAngle();
        if (abs(rotationDelta) > 0.2) {
            if (qFuzzyIsNull(pinchData->rotationTouchAngle)) {
                // First rotation step.
                pinchData->storeItemAngle = proxyItem->rotation();
            }
            pinchData->rotationTouchAngle = gesture->rotationAngle();
            proxyItem->setRotation(pinchData->storeItemAngle + pinchData->rotationTouchAngle);
        }
    }

    if (changeFlags & QPinchGesture::CenterPointChanged) {
        if (!pinchData->isFirstPinch) {
            pinchData->centerPoint = gesture->centerPoint();
            pinchData->isFirstPinch = true;
        }
    }

    if (gesture->state() == Qt::GestureFinished) {
        pinchData->isFirstPinch = false;
        gesture->setCenterPoint(pinchData->centerPoint);

        if (!rotatable()) {
            return;
        }

        playRotationAnimation();
    }
}

/*! \internal */
void DImageViewerPrivate::playRotationAnimation()
{
    D_Q(DImageViewer);
    checkPinchData();

    pinchData->isAnimationRotating = true;
    // Auto delete after animation finished.
    QVariantAnimation *animation = new QVariantAnimation(q);
    animation->setDuration(200);
    if (pinchData->rotationTouchAngle < 0) {
        pinchData->rotationTouchAngle += 360;
    }

    qreal endvalue;
    if (abs(0 - abs(pinchData->rotationTouchAngle)) <= 10) {
        endvalue = 0;
    } else if (abs(360 - abs(pinchData->rotationTouchAngle)) <= 10) {
        endvalue = 0;
    } else if (abs(90 - abs(pinchData->rotationTouchAngle)) <= 10) {
        endvalue = 90;
    } else if (abs(180 - abs(pinchData->rotationTouchAngle)) <= 10) {
        endvalue = 180;
    } else if (abs(270 - abs(pinchData->rotationTouchAngle)) <= 10) {
        endvalue = 270;
    } else {
        endvalue = 0;
    }

    pinchData->rotationEndValue = endvalue;
    qreal startvalue;
    if (abs(pinchData->rotationTouchAngle - endvalue) > 180) {
        startvalue = pinchData->rotationTouchAngle - 360;
    } else {
        startvalue = pinchData->rotationTouchAngle;
    }
    animation->setStartValue(startvalue);
    animation->setEndValue(endvalue);

    QObject::connect(animation, &QVariantAnimation::valueChanged, [=](const QVariant &value) {
        pinchData->rotationTouchAngle = value.toReal();
        if (static_cast<int>(value.toReal()) != static_cast<int>(endvalue)) {
            proxyItem->setRotation(pinchData->storeItemAngle + pinchData->rotationTouchAngle);
        }
    });

    QObject::connect(animation, SIGNAL(finished()), q, SLOT(_q_pinchAnimeFinished()));
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

/*! \internal */
void DImageViewerPrivate::_q_pinchAnimeFinished()
{
    D_Q(DImageViewer);
    // Must check pinch data before use it.
    checkPinchData();

    pinchData->isAnimationRotating = false;
    pinchData->rotationTouchAngle = 0;

    // Set image item rotate angle.
    int rotateAngle = (pinchData->storeItemAngle + pinchData->rotationEndValue) % 360;
    qreal validAngle = validRotateAngle(rotateAngle);
    proxyItem->setRotation(validAngle);
    updateItemAndSceneRect();

    pinchData->storeItemAngle = 0;
}

/*! \internal */
void DImageViewerPrivate::checkCropData()
{
    if (!cropData) {
        cropData = new CropData;
        cropData->cropItem = new DGraphicsCropItem;
        cropData->cropItem->setVisible(false);
    }
}

/*! \internal */
void DImageViewerPrivate::resetCropData()
{
    if (cropData) {
        cropData->cropItem->setParentItem(nullptr);
        cropData->cropItem->setVisible(false);
        cropData->cropRect = QRect();
        cropData->cropping = false;
    }
}

/*! \internal */
void DImageViewerPrivate::handleMousePressEvent(QMouseEvent *event)
{
    D_Q(DImageViewer);

    q->viewport()->unsetCursor();
    q->viewport()->setCursor(Qt::ArrowCursor);
    clickStartPointX = event->pos().x();
}

/*! \internal */
void DImageViewerPrivate::handleMouseReleaseEvent(QMouseEvent *event)
{
    D_Q(DImageViewer);

    q->viewport()->setCursor(Qt::ArrowCursor);
    if (Qt::MouseEventSynthesizedByQt == event->source() && 1 == maxTouchPoints) {
        const QRect &r = q->visibleImageRect();
        const QRectF &sr = q->sceneRect();
        int xPos = event->pos().x() - clickStartPointX;

        // Swipe image.
        if ((r.width() >= (sr.width() - 1) && r.height() >= (sr.height() - 1))) {
            if (abs(xPos) > 200 && clickStartPointX != 0) {
                if (xPos > 0) {
                    Q_EMIT q->requestPreviousImage();
                } else {
                    Q_EMIT q->requestNextImage();
                }
            }
        }
    }
    clickStartPointX = 0;
    maxTouchPoints = 0;
}

/*! \internal */
void DImageViewerPrivate::handleResizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    D_Q(DImageViewer);
    // Keep scale factor.
    if (DImageViewerPrivate::FitWidget != fitFlag) {
        q->scaleImage(1.0);
    }
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

    QImage result = d->contentImage;
    if (d->cropData && !d->cropData->cropRect.isEmpty()) {
        result = result.copy(d->cropData->cropRect);
    }

    int angle = rotateAngle();
    if (0 != angle) {
        QTransform rotateMatrix;
        rotateMatrix.rotate(angle);
        result = result.transformed(rotateMatrix, Qt::SmoothTransformation);
    }

    // Return cut out and rotate image.
    return result;
}

void DImageViewer::setImage(const QImage &image)
{
    D_D(DImageViewer);
    d->resetItem(ImageTypeStatic);
    Q_ASSERT(d->contentItem && d->proxyItem);

    auto staticItem = static_cast<DGraphicsPixmapItem *>(d->contentItem);
    staticItem->setPixmap(QPixmap::fromImage(image));
    d->contentImage = image;

    // Change item center, will affect rotation and scale.
    d->proxyItem->setRect(d->contentItem->boundingRect());
    auto itemSize = d->proxyItem->boundingRect().size();
    d->proxyItem->setTransformOriginPoint(itemSize.width() / 2, itemSize.height() / 2);
    d->updateItemAndSceneRect();
    autoFitImage();
    update();

    Q_EMIT imageChanged(d->contentImage);
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

    Q_ASSERT(d->contentItem && d->proxyItem);
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

    // Change item center, will affect rotation and scale.
    d->proxyItem->setRect(d->contentItem->boundingRect());
    d->proxyItem->setTransformOriginPoint(d->proxyItem->boundingRect().center());
    d->updateItemAndSceneRect();
    autoFitImage();
    update();

    Q_EMIT fileNameChanged(d->fileName);
    Q_EMIT imageChanged(d->contentImage);
}

qreal DImageViewer::scaleFactor() const
{
    D_DC(DImageViewer);
    return d->scaleFactor;
}

void DImageViewer::setScaleFactor(qreal factor)
{
    D_D(DImageViewer);
    factor = d->validScaleFactor(factor);

    qreal realFactor = factor / d->scaleFactor;
    d->scaleFactor = factor;
    scale(realFactor, realFactor);

    Q_EMIT scaleFactorChanged(d->scaleFactor);
}

void DImageViewer::scaleImage(qreal factor)
{
    D_D(DImageViewer);
    qreal tmpScaleFactor = d->scaleFactor * factor;
    qreal realFactor = d->validScaleFactor(tmpScaleFactor);
    if (!qFuzzyCompare(tmpScaleFactor, realFactor)) {
        factor = realFactor / d->scaleFactor;
    } else {
        d->fitFlag = DImageViewerPrivate::Unfit;
    }

    d->scaleFactor = realFactor;
    scale(factor, factor);

    Q_EMIT scaleFactorChanged(d->scaleFactor);
}

void DImageViewer::autoFitImage()
{
    D_D(DImageViewer);
    if (d->contentImage.isNull()) {
        return;
    }

    QSize imageSize = d->contentImage.size();
    if (d->isRotateVertical()) {
        int tmp = imageSize.rheight();
        imageSize.setHeight(imageSize.width());
        imageSize.setWidth(tmp);
    }

    if ((imageSize.width() >= width() || imageSize.height() > height()) && width() > 0 && height() > 0) {
        fitToWidget();
    } else {
        fitNormalSize();
    }
}

void DImageViewer::fitToWidget()
{
    D_D(DImageViewer);
    qreal factor = d->widgetRelativeScale();
    factor = d->validScaleFactor(factor);
    if (qFuzzyCompare(factor, d->scaleFactor)) {
        d->fitFlag = DImageViewerPrivate::FitWidget;
        return;
    }

    resetTransform();

    d->fitFlag = DImageViewerPrivate::FitWidget;
    d->scaleFactor = factor;
    scale(factor, factor);
    Q_EMIT scaleFactorChanged(d->scaleFactor);
}

void DImageViewer::fitNormalSize()
{
    D_D(DImageViewer);
    if (qFuzzyCompare(1.0, d->scaleFactor)) {
        d->fitFlag = DImageViewerPrivate::FitNotmalSize;
        return;
    }

    resetTransform();

    d->fitFlag = DImageViewerPrivate::FitNotmalSize;
    d->scaleFactor = 1.0;
    scale(d->scaleFactor, d->scaleFactor);

    Q_EMIT scaleFactorChanged(d->scaleFactor);
}

void DImageViewer::rotateClockwise()
{
    D_D(DImageViewer);
    if (d->proxyItem) {
        int rotation = (static_cast<int>(d->proxyItem->rotation()) + 90) % 360;
        d->proxyItem->setRotation(d->validRotateAngle(rotation));
        d->updateItemAndSceneRect();
        autoFitImage();

        Q_EMIT rotateAngleChanged(d->proxyItem->rotation());
    }
}

void DImageViewer::rotateCounterclockwise()
{
    D_D(DImageViewer);
    if (d->proxyItem) {
        int rotation = (static_cast<int>(d->proxyItem->rotation()) - 90) % 360;
        d->proxyItem->setRotation(d->validRotateAngle(rotation));
        d->updateItemAndSceneRect();
        autoFitImage();

        Q_EMIT rotateAngleChanged(d->proxyItem->rotation());
    }
}

int DImageViewer::rotateAngle() const
{
    D_DC(DImageViewer);
    return d->proxyItem ? d->proxyItem->rotation() : 0;
}

void DImageViewer::resetRotateAngle()
{
    D_D(DImageViewer);
    if (d->proxyItem && !qFuzzyIsNull(d->proxyItem->rotation())) {
        // Reset scene rect.
        if (d->isRotateVertical()) {
            d->updateItemAndSceneRect();
        }

        d->proxyItem->setRotation(0);
        autoFitImage();

        Q_EMIT rotateAngleChanged(0);
    }
}

void DImageViewer::clear()
{
    D_D(DImageViewer);
    // Crop data need reset before release contentItem.
    d->resetCropData();

    if (d->contentItem) {
        scene()->removeItem(d->contentItem);
        delete d->contentItem;
        d->contentItem = nullptr;
    }
    d->proxyItem->resetTransform();
    resetTransform();

    d->fileName.clear();
    d->contentImage = QImage();
    d->imageType = ImageTypeBlank;
    d->scaleFactor = 1.0;

    Q_EMIT fileNameChanged(d->fileName);
    Q_EMIT imageChanged(d->contentImage);
}

void DImageViewer::centerOn(qreal x, qreal y)
{
    DGraphicsView::centerOn(x, y);
    Q_EMIT transformChanged();
}

QRect DImageViewer::visibleImageRect() const
{
    D_DC(DImageViewer);
    if (d->contentItem) {
        QRect viewRect = viewportTransform().inverted().mapRect(rect());
        return viewRect & sceneRect().toRect();
    } else {
        return QRect();
    }
}

void DImageViewer::scaleAtPoint(QPoint pos, qreal factor)
{
    const QPointF targetScenePos = mapToScene(pos);

    scaleImage(factor);

    // Restore zoom anchor point.
    const QPointF curPos = mapFromScene(targetScenePos);
    const QPointF centerPos = QPointF(width() / 2.0, height() / 2.0) + (curPos - pos);
    const QPointF centerScenePos = mapToScene(centerPos.toPoint());
    centerOn(static_cast<int>(centerScenePos.x()), static_cast<int>(centerScenePos.y()));
}

void DImageViewer::beginCropImage()
{
    D_D(DImageViewer);
    if (d->proxyItem && d->contentItem) {
        d->checkCropData();
        if (d->cropData->cropping) {
            return;
        }

        d->cropData->cropping = true;
        d->cropData->cropItem->updateContentItem(d->proxyItem);
        d->cropData->cropItem->setVisible(true);
    }
}

void DImageViewer::endCropImage()
{
    D_D(DImageViewer);
    if (d->cropData && d->cropData->cropping) {
        // Crop item must remove parent after corped.
        d->cropData->cropItem->setParentItem(nullptr);
        d->cropData->cropItem->setVisible(false);

        QRect newRect = d->cropData->cropItem->cropRect();

        if (newRect != d->proxyItem->boundingRect()) {
            // Already has crop rect, add new crop rect.
            if (!d->cropData->cropRect.isEmpty()) {
                newRect.moveTopLeft(newRect.topLeft() + d->cropData->cropRect.topLeft());
            }
            d->cropData->cropRect = newRect;

            if (d->contentItem) {
                d->contentItem->setPos(-newRect.left(), -newRect.top());
            }

            d->proxyItem->setRect(0, 0, newRect.width(), newRect.height());
            d->proxyItem->setTransformOriginPoint(d->proxyItem->boundingRect().center());
            d->updateItemAndSceneRect();

            Q_EMIT cropImageChanged(d->cropData->cropRect);
        }

        d->cropData->cropping = false;
    }
}

void DImageViewer::resetCropImage()
{
    D_D(DImageViewer);
    if (d->cropData && d->contentItem) {
        d->resetCropData();

        d->contentItem->setPos(0, 0);
        d->contentItem->resetTransform();
        d->proxyItem->setRect(d->contentItem->boundingRect());
        d->proxyItem->setTransformOriginPoint(d->proxyItem->boundingRect().center());
        d->updateItemAndSceneRect();
        autoFitImage();
    }
}

void DImageViewer::setCropAspectRatio(qreal w, qreal h)
{
    D_D(DImageViewer);
    if (d->cropData) {
        d->cropData->cropItem->setAspectRatio(w, h);
    }
}

QRect DImageViewer::cropImageRect() const
{
    D_DC(DImageViewer);
    if (d->cropData) {
        return d->cropData->cropRect;
    }
    return QRect();
}

void DImageViewer::mouseMoveEvent(QMouseEvent *event)
{
    if (!(Qt::NoButton | event->buttons())) {
        viewport()->setCursor(Qt::ArrowCursor);
    } else {
        DGraphicsView::mouseMoveEvent(event);
        viewport()->setCursor(Qt::ClosedHandCursor);

        Q_EMIT transformChanged();
    }
}

void DImageViewer::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier) {
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
        qreal delta = event->delta();
#else
        qreal delta = event->angleDelta().y();
#endif
        if (delta > 0) {
            Q_EMIT requestPreviousImage();
        } else if (delta < 0) {
            Q_EMIT requestNextImage();
        }
    } else {
        qreal factor = qPow(1.2, event->angleDelta().y() / 240.0);
        // Qt deprecated pos() since 5.15
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
        scaleAtPoint(event->pos(), factor);
#else
        scaleAtPoint(event->position().toPoint(), factor);
#endif
        event->accept();
    }
}

bool DImageViewer::event(QEvent *event)
{
    D_D(DImageViewer);
    static int touchCount = 0;

    QEvent::Type type = event->type();
    switch (type) {
        case QEvent::TouchBegin: {
            touchCount = 0;
            d->maxTouchPoints = 1;
            break;
        }
        case QEvent::TouchUpdate: {
            QTouchEvent *touchEvent = dynamic_cast<QTouchEvent *>(event);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->points();
#else
            QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
#endif
            if (touchPoints.size() > touchCount) {
                touchCount = touchPoints.size();
            }
            break;
        }
        case QEvent::TouchEnd: {
            QTouchEvent *touchEvent = dynamic_cast<QTouchEvent *>(event);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->points();
#else
            QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
#endif
            if (touchPoints.size() == 1 && touchCount <= 1) {
                // Swipe gesture.
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                qreal offset = touchPoints.at(0).lastPosition().x() - touchPoints.at(0).pressPosition().x();
#else
                qreal offset = touchPoints.at(0).lastPos().x() - touchPoints.at(0).startPos().x();
#endif
                if (qAbs(offset) > 200) {
                    if (offset > 0) {
                        Q_EMIT requestPreviousImage();
                    } else {
                        Q_EMIT requestNextImage();
                    }
                }
            }
            break;
        }
        case QEvent::Gesture:
            d->handleGestureEvent(static_cast<QGestureEvent *>(event));
            break;
        case QEvent::Resize:
            d->handleResizeEvent(static_cast<QResizeEvent *>(event));
            break;
        default:
            break;
    }

    bool accept = DGraphicsView::event(event);

    switch (type) {
        case QEvent::MouseButtonPress:
            d->handleMousePressEvent(static_cast<QMouseEvent *>(event));
            break;
        case QEvent::MouseButtonRelease:
            d->handleMouseReleaseEvent(static_cast<QMouseEvent *>(event));
            break;
        default:
            break;
    }

    return accept;
}

DWIDGET_END_NAMESPACE

#include "moc_dimageviewer.cpp"
