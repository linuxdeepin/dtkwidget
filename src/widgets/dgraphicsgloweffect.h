// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DGRAPHICSGLOWEFFECT_H
#define DGRAPHICSGLOWEFFECT_H

#include <QGraphicsDropShadowEffect>
#include <QGraphicsEffect>
#include <QPainter>

#include <dtkwidget_global.h>

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DGraphicsGlowEffect : public QGraphicsEffect
{
    Q_OBJECT
public:
    explicit DGraphicsGlowEffect(QObject *parent = nullptr);

    void draw(QPainter *painter);
    QRectF boundingRectFor(const QRectF &rect) const;

    inline void setOffset(qreal dx, qreal dy) {m_xOffset = dx; m_yOffset = dy;}

    inline void setXOffset(qreal dx) {m_xOffset = dx;}
    inline qreal xOffset() const {return m_xOffset;}

    inline void setYOffset(qreal dy) {m_yOffset = dy;}
    inline qreal yOffset() const {return m_yOffset;}

    inline void setDistance(qreal distance) { m_distance = distance; updateBoundingRect(); }
    inline qreal distance() const { return m_distance; }

    inline void setBlurRadius(qreal blurRadius) { m_blurRadius = blurRadius; updateBoundingRect(); }
    inline qreal blurRadius() const { return m_blurRadius; }

    inline void setColor(const QColor &color) { m_color = color; }
    inline QColor color() const { return m_color; }

    // TODO: refactor with d-pointer;
    inline qreal opacity() const { return m_opacity; }
    inline void setOpacity(qreal opacity) { m_opacity = opacity; }

private:
    qreal m_opacity = 1.0;
    qreal m_xOffset;
    qreal m_yOffset;
    qreal m_distance;
    qreal m_blurRadius;
    QColor m_color;
};

DWIDGET_END_NAMESPACE

#endif // DGRAPHICSGLOWEFFECT_H
