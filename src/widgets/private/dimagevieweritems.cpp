// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dimagevieweritems_p.h"

#include <QMovie>

DGUI_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

DGraphicsPixmapItem::DGraphicsPixmapItem(const QPixmap &pixmap, QGraphicsItem *parent)
    : QGraphicsPixmapItem(pixmap, parent)
{
}

DGraphicsPixmapItem::~DGraphicsPixmapItem()
{
    // todo
}

void DGraphicsPixmapItem::setPixmap(const QPixmap &pixmap)
{
    // todo
}

void DGraphicsPixmapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // todo
}

DGraphicsMovieItem::DGraphicsMovieItem(const QString &fileName, QGraphicsItem *parent)
    : QGraphicsPixmapItem(fileName, parent)
{
}

DGraphicsMovieItem::~DGraphicsMovieItem()
{

}

DGraphicsSVGItem::DGraphicsSVGItem(QGraphicsItem *parent)
    : QGraphicsObject(parent)
{
    renderer = new DSvgRenderer;
}

DGraphicsSVGItem::DGraphicsSVGItem(const QString &fileName, QGraphicsItem *parent)
{
    // todo
}

QRectF DGraphicsSVGItem::boundingRect() const
{
    // todo
}

void DGraphicsSVGItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // todo
}

int DGraphicsSVGItem::type() const
{
    return Type;
}

void DGraphicsSVGItem::updateDefaultSize()
{
    // todo
}

DWIDGET_END_NAMESPACE
