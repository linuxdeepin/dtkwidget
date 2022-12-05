// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DIMAGEVIEWERITEMS_P_H
#define DIMAGEVIEWERITEMS_P_H

#include <dtkwidget_global.h>
#include <DSvgRenderer>

#include <QGraphicsItem>

class QMovie;
class QGraphicsView;

DWIDGET_BEGIN_NAMESPACE

class DGraphicsPixmapItem : public QGraphicsPixmapItem
{
public:
    explicit DGraphicsPixmapItem(QGraphicsItem *parent = nullptr);
    explicit DGraphicsPixmapItem(const QPixmap &pixmap, QGraphicsItem *parent = nullptr);
    ~DGraphicsPixmapItem() Q_DECL_OVERRIDE;

    void setPixmap(const QPixmap &pixmap);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

private:
    QPair<qreal, QPixmap> cachePixmap;
};

class DGraphicsMovieItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit DGraphicsMovieItem(QGraphicsItem *parent = nullptr);
    explicit DGraphicsMovieItem(const QString &fileName, QGraphicsItem *parent = nullptr);
    ~DGraphicsMovieItem() Q_DECL_OVERRIDE;

    void setFileName(const QString &fileName);

private:
    Q_SLOT void onMovieFrameChanged();

private:
    QMovie *movie;
};

class DGraphicsSVGItem : public QGraphicsObject
{
public:
    explicit DGraphicsSVGItem(QGraphicsItem *parent = nullptr);
    explicit DGraphicsSVGItem(const QString &fileName, QGraphicsItem *parent = nullptr);

    void setFileName(const QString &fileName);

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) Q_DECL_OVERRIDE;

    enum { Type = QGraphicsItem::UserType + 1 };
    int type() const Q_DECL_OVERRIDE;

private:
    void updateDefaultSize();

private:
    DGUI_NAMESPACE::DSvgRenderer *renderer = nullptr;
    QRectF imageRect;
};

class DGraphicsCropItem : public QGraphicsItem
{
public:
    explicit DGraphicsCropItem(QGraphicsItem *parent = nullptr);
    void updateContentItem(QGraphicsItem *parentItem);

    enum CropMode {
        CropOriginal,
        CropFree,
        AspectRatio1x1,
        AspectRatio16x9,
        AspectRatio9x16,
        AspectRatio4x3,
        AspectRatio3x4,
        AspectRatio3x2,
        AspectRatio2x3
    };
    void setCropMode(CropMode mode);
    CropMode cropMode() const;
    void setAspectRatio(qreal w, qreal h);

    void setRect(const QRectF &rect);
    void setSize(qreal width, qreal height);
    void move(qreal x, qreal y);

    QRect cropRect() const;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

private:
    void drawTrisectorRect(QPainter *painter);
    void drawCornerHandle(QPainter *painter);

    enum MoveHandleType { TopLeft, TopRight, BottomRight, BottomLeft, Move };
    MoveHandleType detectHandleType(const QPointF &mousePoint) const;
    void updateRect(QRectF &rect, const QPointF &offset, MoveHandleType type);
    QRectF validRect(const QRectF &rect) const;
    QGraphicsView *contentView() const;

private:
    QRectF itemRect;
    QRectF originalParentRect;
    CropMode internalCropMode = CropFree;

    enum StaticProperty { HandleSize = 20, MinimalSize = 40 };
    MoveHandleType handleType = Move;
    bool mousePressed = false;
};

DWIDGET_END_NAMESPACE

#endif  // DIMAGEVIEWERITEMS_P_H
