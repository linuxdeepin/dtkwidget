// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dimagevieweritems_p.h"

#include <QObject>
#include <QMovie>
#include <QPainter>
#include <QStyleOption>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <DIconTheme>

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

DGraphicsCropItem::DGraphicsCropItem(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    updateContentItem(parent);
    setZValue(100);
}

void DGraphicsCropItem::updateContentItem(QGraphicsItem *parentItem)
{
    setParentItem(parentItem);

    if (parentItem) {
        QRectF parentRect = parentItem->boundingRect();
        originalParentRect = QRectF(0, 0, parentRect.width(), parentRect.height());
        itemRect = originalParentRect;
    } else {
        itemRect = QRectF();
        originalParentRect = QRectF();
    }

    update();
}

void DGraphicsCropItem::setCropMode(CropMode mode)
{
    internalCropMode = mode;

    // CropFree will do nothing, using current item rect.
    switch (internalCropMode) {
        case CropOriginal:
            itemRect = originalParentRect;
            break;
        case AspectRatio1x1:
            setAspectRatio(1.0, 1.0);
            break;
        case AspectRatio16x9:
            setAspectRatio(16, 9);
            break;
        case AspectRatio9x16:
            setAspectRatio(9.0, 16.0);
            break;
        case AspectRatio4x3:
            setAspectRatio(4.0, 3.0);
            break;
        case AspectRatio3x4:
            setAspectRatio(3.0, 4.0);
            break;
        case AspectRatio3x2:
            setAspectRatio(3.0, 2.0);
            break;
        case AspectRatio2x3:
            setAspectRatio(2.0, 3.0);
            break;
        default:
            break;
    }

    update();
}

DGraphicsCropItem::CropMode DGraphicsCropItem::cropMode() const
{
    return internalCropMode;
}

void DGraphicsCropItem::setAspectRatio(qreal w, qreal h)
{
    qreal calcWidth = originalParentRect.width();
    qreal calcHeight = calcWidth * h / w;
    // Check parent item rotate.
    if (parentItem()) {
        int rotate = qRound(parentItem()->rotation());
        if (rotate % 180) {
            calcHeight = calcWidth * w / h;
        }
    }

    // Make sure parent rectangle contain item rectangle.
    if (calcWidth > originalParentRect.width() || calcHeight > originalParentRect.height()) {
        qreal aspectRatio = originalParentRect.width() / originalParentRect.height();
        qreal calcRatio = calcWidth / calcHeight;
        if (calcRatio > aspectRatio) {
            calcWidth = originalParentRect.width();
            calcHeight = (calcWidth / calcRatio);
        } else {
            calcHeight = originalParentRect.height();
            calcWidth = calcHeight * calcRatio;
        }
    }

    // Move rectangle to center.
    QPointF center = originalParentRect.center();
    QPointF topLeft = center - QPointF(calcWidth / 2, calcHeight / 2);
    QPointF bottomRight = center + QPointF(calcWidth / 2, calcHeight / 2);
    itemRect = QRectF(topLeft, bottomRight);

    update();
}

void DGraphicsCropItem::setRect(const QRectF &rect)
{
    prepareGeometryChange();
    itemRect = validRect(rect);

    update();
}

void DGraphicsCropItem::setSize(qreal width, qreal height)
{
    prepareGeometryChange();
    itemRect = validRect(itemRect.adjusted(0, 0, width, height));
    update();
}

void DGraphicsCropItem::move(qreal x, qreal y)
{
    prepareGeometryChange();
    itemRect.moveTo(x, y);
    update();
}

QRect DGraphicsCropItem::cropRect() const
{
    return mapRectToParent(itemRect).toRect();
}

void DGraphicsCropItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setClipping(false);
    QColor activeColor = QColor(Qt::cyan);
    qreal penWidth = 0;
    if (parentItem() && !qFuzzyIsNull(parentItem()->rotation())) {
        auto transform = painter->worldTransform();
        transform.rotate(-parentItem()->rotation());
        penWidth = 1.0 / transform.m11();
    } else {
        penWidth = 1.0 / painter->worldTransform().m11();
    }

    QRectF rct = itemRect.adjusted(penWidth, penWidth, -penWidth, -penWidth);
    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setColor(QColor("#EDEDED"));
    pen.setWidthF(penWidth * 3);

    // Draw the lines that divide the rectangle.
    painter->save();
    painter->setPen(pen);
    drawTrisectorRect(painter);
    painter->restore();

    // Draw border shadow, shadow witdh is 5 times the width of pen.
    painter->save();
    QPen rectPen(pen);
    rectPen.setStyle(Qt::SolidLine);
    QColor rectColor(activeColor);
    rectColor.setAlpha(26);
    rectPen.setColor(rectColor);
    rectPen.setWidthF(penWidth * 5);
    painter->setPen(rectPen);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(rct);
    painter->restore();

    painter->save();
    rectPen.setStyle(Qt::DashLine);
    rectPen.setColor(activeColor);
    rectPen.setWidthF(penWidth);
    painter->setPen(rectPen);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(rct);
    painter->restore();

    painter->save();
    drawCornerHandle(painter);
    painter->restore();

    // Draw cross fill background.
    painter->save();
    QRectF sceneRct = scene()->sceneRect();
    QRectF itemSceneRect = sceneBoundingRect();
    QRegion r1(sceneRct.toRect());
    QRegion r2(itemSceneRect.toRect());
    QRegion r3 = r2.subtracted(r1);
    QPainterPath path;
    path.addRegion(r3);

    QColor background(activeColor);
    background.setAlpha(26);
    painter->setPen(Qt::NoPen);
    painter->setBrush(background);
    painter->drawPath(path);
    painter->restore();
}

void DGraphicsCropItem::drawTrisectorRect(QPainter *painter)
{
    qreal penWidth = 0;
    if (parentItem() && !qFuzzyIsNull(parentItem()->rotation())) {
        auto transform = painter->worldTransform();
        transform.rotate(-parentItem()->rotation());
        penWidth = 1.0 / transform.m11();
    } else {
        penWidth = 1.0 / painter->worldTransform().m11();
    }
    QPainterPath path;
    QRectF rct = itemRect.adjusted(penWidth, penWidth, -penWidth, -penWidth);
    path.moveTo(rct.x(), rct.y() + rct.height() / 3);
    path.lineTo(rct.x() + rct.width(), rct.y() + rct.height() / 3);

    path.moveTo(rct.x(), rct.y() + rct.height() / 3 * 2);
    path.lineTo(rct.x() + rct.width(), rct.y() + rct.height() / 3 * 2);

    path.moveTo(rct.x() + rct.width() / 3, rct.y());
    path.lineTo(rct.x() + rct.width() / 3, rct.y() + rct.height());

    path.moveTo(rct.x() + rct.width() / 3 * 2, rct.y());
    path.lineTo(rct.x() + rct.width() / 3 * 2, rct.y() + rct.height());

    painter->drawPath(path);
}

void DGraphicsCropItem::drawCornerHandle(QPainter *painter)
{
    QGraphicsView *view = contentView();
    if (!view) {
        return;
    }

    qreal penWidth = 0;
    if (parentItem() && !qFuzzyIsNull(parentItem()->rotation())) {
        auto transform = painter->worldTransform();
        transform.rotate(-parentItem()->rotation());
        penWidth = 1.0 / transform.m11();
    } else {
        penWidth = 1.0 / painter->worldTransform().m11();
    }
    QRectF rct = itemRect.adjusted(penWidth, penWidth, -penWidth, -penWidth);

    const qreal minlenth = 24;
    qreal scale = view->transform().m11();
    qreal showW = itemRect.width() * scale;
    qreal showH = itemRect.height() * scale;

    if (showW < minlenth || showH < minlenth) {
        // When the clipping area is smaller than the corner picture, the brush is drawn
        QPen pen(painter->pen());
        pen.setWidthF(1.0);
        pen.setColor(QColor("#EDEDED"));
        pen.setStyle(Qt::SolidLine);

        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(rct);

        QLineF topLine = QLineF(rct.topLeft(), rct.topRight());
        QLineF botLine = QLineF(rct.bottomLeft(), rct.bottomRight());
        QLineF leftLine = QLineF(rct.topLeft(), rct.bottomLeft());
        QLineF rightLine = QLineF(rct.topRight(), rct.bottomRight());

        painter->drawLine(topLine.p1(), topLine.center());
        painter->drawLine(rightLine.p1(), rightLine.center());
        painter->drawLine(botLine.center(), botLine.p2());
        painter->drawLine(leftLine.center(), leftLine.p2());

        painter->drawLine(topLine.center(), topLine.p2());
        painter->drawLine(rightLine.center(), rightLine.p2());
        painter->drawLine(botLine.center(), botLine.p1());
        painter->drawLine(leftLine.center(), leftLine.p1());
    } else {
        painter->save();

        int pixWidth = 20;
        int offset = 2;
        int offsetWidth = 18;

        auto painterTopLeft = view->mapFromScene(sceneBoundingRect().topLeft());
        auto painterBottomRight = view->mapFromScene(sceneBoundingRect().bottomRight());
        rct = QRectF(painterTopLeft, painterBottomRight);

        // Reset tranform, keep corner same size
        painter->resetTransform();

        // Draw four corner handles.
        QPixmap cornerPixmap = DIconTheme::findQIcon("selection_topleft").pixmap(QSize(pixWidth, pixWidth));
        QRectF cornerRect = QRectF(painterTopLeft + QPointF(-offset, -offset), painterTopLeft + QPointF(pixWidth, pixWidth));
        painter->drawPixmap(cornerRect, cornerPixmap, QRectF(0, 0, cornerPixmap.width(), cornerPixmap.height()));

        cornerPixmap = DIconTheme::findQIcon("selection_topright").pixmap(QSize(pixWidth, pixWidth));
        cornerRect =
            QRectF(QPointF(painterBottomRight.x() - offsetWidth, painterTopLeft.y() - offset), QSizeF(pixWidth, pixWidth));
        painter->drawPixmap(cornerRect, cornerPixmap, QRectF(0, 0, cornerPixmap.width(), cornerPixmap.height()));

        cornerPixmap = DIconTheme::findQIcon("selection_bottomright").pixmap(QSize(pixWidth, pixWidth));
        cornerRect = QRectF(rct.bottomRight() + QPointF(-pixWidth, -pixWidth), rct.bottomRight() + QPointF(offset, offset));
        painter->drawPixmap(cornerRect, cornerPixmap, QRectF(0, 0, cornerPixmap.width(), cornerPixmap.height()));

        cornerPixmap = DIconTheme::findQIcon("selection_bottomleft").pixmap(QSize(pixWidth, pixWidth));
        cornerRect = QRectF(QPointF(rct.x() - offset, rct.y() + rct.height() - offsetWidth), QSizeF(pixWidth, pixWidth));
        painter->drawPixmap(cornerRect, cornerPixmap, QRectF(0, 0, cornerPixmap.width(), cornerPixmap.height()));

        painter->restore();
    }
}

QRectF DGraphicsCropItem::boundingRect() const
{
    return itemRect;
}

void DGraphicsCropItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (!mousePressed) {
        handleType = detectHandleType(event->scenePos());
        mousePressed = true;
    }
}

void DGraphicsCropItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (mousePressed) {
        QPointF offset = event->pos() - event->lastPos();
        QRectF newRect = itemRect;
        updateRect(newRect, offset, handleType);

        if (newRect != itemRect) {
            itemRect = newRect;
            update();
        }
    }
}

void DGraphicsCropItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    if (mousePressed) {
        // Reset handle mode.
        handleType = Move;
        mousePressed = false;
    }
}

DGraphicsCropItem::MoveHandleType DGraphicsCropItem::detectHandleType(const QPointF &mousePoint) const
{
    MoveHandleType type = Move;
    QGraphicsView *view = contentView();
    if (view) {
        QRectF itemSceneRect = sceneBoundingRect();
        QPointF itemViewTopLeft = view->mapFromScene(itemSceneRect.topLeft());
        QPointF itemViewBottomRight = view->mapFromScene(itemSceneRect.bottomRight());
        QRectF itemViewRect(itemViewTopLeft, itemViewBottomRight);
        QPointF mouseViewPoint = view->mapFromScene(mousePoint);

        // Detect which corner handle.
        if (itemViewRect.left() <= mouseViewPoint.x() && mouseViewPoint.x() <= itemViewRect.left() + HandleSize) {
            if (itemViewRect.top() <= mouseViewPoint.y() && mouseViewPoint.y() <= itemViewRect.top() + HandleSize) {
                type = TopLeft;
            } else if (itemViewRect.bottom() - HandleSize <= mouseViewPoint.y() && mouseViewPoint.y() <= itemViewRect.bottom()) {
                type = BottomLeft;
            }
        } else if (itemViewRect.right() - HandleSize <= mouseViewPoint.x() && mouseViewPoint.x() <= itemViewRect.right()) {
            if (itemViewRect.top() <= mouseViewPoint.y() && mouseViewPoint.y() <= itemViewRect.top() + HandleSize) {
                type = TopRight;
            } else if (itemViewRect.bottom() - HandleSize <= mouseViewPoint.y() && mouseViewPoint.y() <= itemViewRect.bottom()) {
                type = BottomRight;
            }
        }
    }

    // Warning: Must refactor code while the enum MoveHandleType changed.
    if (Move != type && parentItem()) {
        // Adjust handle type while parent item is rotate.
        int rotation = qRound(parentItem()->rotation());
        int rotate90Count = rotation / 90;

        // 4 is corner enum count.
        type = MoveHandleType((int(type) + 4 - rotate90Count) % 4);
    }

    return type;
}

void DGraphicsCropItem::updateRect(QRectF &rect, const QPointF &offset, MoveHandleType type)
{
    switch (type) {
        case Move:  // Move top left point, not resize.
            rect.moveTop(qBound<qreal>(0, rect.top() + offset.y(), originalParentRect.height() - rect.height()));
            rect.moveLeft(qBound<qreal>(0, rect.left() + offset.x(), originalParentRect.width() - rect.width()));
            break;
        case TopLeft:  // Will change item rectangle.
            rect.setTop(qBound<qreal>(0, rect.top() + offset.y(), rect.bottom() - MinimalSize));
            rect.setLeft(qBound<qreal>(0, rect.left() + offset.x(), rect.right() - MinimalSize));
            break;
        case TopRight:
            rect.setTop(qBound<qreal>(0, rect.top() + offset.y(), rect.bottom() - MinimalSize));
            rect.setRight(qBound<qreal>(MinimalSize, rect.right() + offset.x(), originalParentRect.width()));
            break;
        case BottomRight:
            rect.setBottom(qBound<qreal>(MinimalSize, rect.bottom() + offset.y(), originalParentRect.height()));
            rect.setRight(qBound<qreal>(MinimalSize, rect.right() + offset.x(), originalParentRect.width()));
            break;
        case BottomLeft:
            rect.setBottom(qBound<qreal>(MinimalSize, rect.bottom() + offset.y(), originalParentRect.height()));
            rect.setLeft(qBound<qreal>(0, rect.left() + offset.x(), rect.right() - MinimalSize));
            break;
        default:
            break;
    }
}

QRectF DGraphicsCropItem::validRect(const QRectF &rect) const
{
    if (parentItem()) {
        return rect.intersected(parentItem()->boundingRect());
    }
    return rect;
}

QGraphicsView *DGraphicsCropItem::contentView() const
{
    if (scene()) {
        auto viewList = scene()->views();
        if (!viewList.isEmpty()) {
            return viewList.first();
        }
    }

    return nullptr;
}

DWIDGET_END_NAMESPACE
