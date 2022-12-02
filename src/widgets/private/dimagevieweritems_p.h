// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DIMAGEVIEWERITEMS_P_H
#define DIMAGEVIEWERITEMS_P_H

#include <dtkwidget_global.h>
#include <DSvgRenderer>

#include <QGraphicsItem>

class QMovie;

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

DWIDGET_END_NAMESPACE

#endif // DIMAGEVIEWERITEMS_P_H
