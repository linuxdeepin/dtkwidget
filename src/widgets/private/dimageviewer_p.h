// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DIMAGEVIEWER_P_H
#define DIMAGEVIEWER_P_H

#include "dimageviewer.h"
#include <DObjectPrivate>

class QGestureEvent;
class QPinchGesture;
class QImageReader;
class QGraphicsRectItem;

DWIDGET_BEGIN_NAMESPACE
class DGraphicsCropItem;

enum ImageType {
    ImageTypeBlank = 0,
    ImageTypeStatic,
    ImageTypeDynamic,
    ImageTypeSvg,
};

class DImageViewerPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    D_DECLARE_PUBLIC(DImageViewer)

public:
    explicit DImageViewerPrivate(DImageViewer *qq);
    ~DImageViewerPrivate() Q_DECL_OVERRIDE;

    void init();
    ImageType detectImageType(const QString &fileName) const;
    void resetItem(ImageType type);
    QImage loadImage(const QString &fileName, ImageType type) const;

    void updateItemAndSceneRect();
    bool rotatable() const;
    bool isRotateVertical() const;
    qreal validRotateAngle(qreal angle) const;

    qreal validScaleFactor(qreal scale) const;
    inline qreal imageRelativeScale() const { return q_func()->transform().m11(); }
    qreal widgetRelativeScale() const;

    void checkPinchData();
    void handleGestureEvent(QGestureEvent *gesture);
    void pinchTriggered(QPinchGesture *gesture);
    void playRotationAnimation();
    void _q_pinchAnimeFinished();

    void checkCropData();

    void handleMousePressEvent(QMouseEvent *event);
    void handleMouseReleaseEvent(QMouseEvent *event);
    void handleResizeEvent(QResizeEvent *event);

    QGraphicsRectItem *proxyItem = nullptr;
    QGraphicsItem *contentItem = nullptr;
    ImageType imageType = ImageType::ImageTypeBlank;
    QImage contentImage;
    QString fileName;

    enum FitFlag { Unfit, FitWidget, FitNotmalSize };
    FitFlag fitFlag = Unfit;
    qreal scaleFactor = 1.0;
    int clickStartPointX = 0;
    int maxTouchPoints = 0;

    struct PinchData
    {
        bool isFirstPinch = false;
        bool isAnimationRotating = false;
        int startTouchPointX = 0;
        qreal rotationTouchAngle = 0;
        int storeItemAngle = 0;
        int rotationEndValue = 0;
        QPointF centerPoint;
    };
    PinchData *pinchData = nullptr;

    struct CropData
    {
        DGraphicsCropItem *cropItem = nullptr;
        QRect cropRect;
    };
    CropData *cropData = nullptr;
};

DWIDGET_END_NAMESPACE

#endif  // DIMAGEVIEWER_P_H
