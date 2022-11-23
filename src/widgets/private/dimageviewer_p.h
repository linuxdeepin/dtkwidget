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

DWIDGET_BEGIN_NAMESPACE

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
    DImageViewerPrivate(DImageViewer *qq);
    ~DImageViewerPrivate() Q_DECL_OVERRIDE;

    void init();

    ImageType detectImageType(const QString &fileName) const;
    void resetItem(ImageType type);

    QImage loadImage(const QString &fileName, ImageType type) const;

    void handleGestureEvent(QGestureEvent *gesture);
    void pinchTriggered(QPinchGesture *gesture);

    void _q_pinchAnimeFinished();

    QGraphicsItem *contentItem = nullptr;
    ImageType imageType = ImageType::ImageTypeBlank;
    QImage contentImage;
    QString fileName;

    qreal rotateAngle = 0;
};

DWIDGET_END_NAMESPACE

#endif  // DIMAGEVIEWER_P_H
