/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DARROWRECTANGLE_H
#define DARROWRECTANGLE_H

#include <QDesktopWidget>
#include <QWidget>
#include <QLabel>
#include <QTextLine>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QTimer>
#include <QDebug>

#include "dobject.h"
#include "dwidget_global.h"
#include "dthememanager.h"
#include "dgraphicsgloweffect.h"
#include "dblureffectwidget.h"

DWIDGET_BEGIN_NAMESPACE

class DPlatformWindowHandle;
class DArrowRectanglePrivate;
class LIBDTKWIDGETSHARED_EXPORT DArrowRectangle : public QWidget, public DObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DArrowRectangle)
    D_DECLARE_PRIVATE(DArrowRectangle)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
    Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth)
    Q_PROPERTY(int radius READ radius WRITE setRadius)

public:
    enum ArrowDirection {
        ArrowLeft,
        ArrowRight,
        ArrowTop,
        ArrowBottom
    };

    explicit DArrowRectangle(ArrowDirection direction, QWidget * parent = 0);
    ~DArrowRectangle();

    int radius() const;
    int arrowHeight() const;
    int arrowWidth() const;
    int arrowX() const;
    int arrowY() const;
    int margin() const;
    int borderWidth() const;
    QColor borderColor() const;
    QColor backgroundColor() const;
    ArrowDirection arrowDirection() const;

    void setRadius(int value);
    void setArrowHeight(int value);
    void setArrowWidth(int value);
    void setArrowX(int value);
    void setArrowY(int value);
    void setMargin(int value);
    void setBorderWidth(int borderWidth);
    void setBorderColor(const QColor &borderColor);
    void setBackgroundColor(const QColor &backgroundColor);
    void setBackgroundColor(DBlurEffectWidget::MaskColorType type);
    void setArrowDirection(ArrowDirection value);
    void setWidth(int value);
    void setHeight(int value);

    virtual void show(int x, int y);

    void setContent(QWidget *content);
    QWidget *getContent() const;
    void resizeWithContent();
    void move(int x,int y);
    QSize getFixedSize();

    qreal shadowXOffset() const;
    qreal shadowYOffset() const;
    qreal shadowDistance() const;
    qreal shadowBlurRadius() const;

    void setShadowBlurRadius(const qreal &shadowBlurRadius);
    void setShadowDistance(const qreal &shadowDistance);
    void setShadowXOffset(const qreal &shadowXOffset);
    void setShadowYOffset(const qreal &shadowYOffset);

signals:
    void windowDeactivate() const;

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;
    bool event(QEvent *e);
};

DWIDGET_END_NAMESPACE

#endif // DARROWRECTANGLE_H
