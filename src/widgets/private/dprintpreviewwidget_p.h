/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     chengyulong <chengyulong@uniontech.com>
*
* Maintainer: chengyulong <chengyulong@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DPRINTPREVIEWWIDGET_P_H
#define DPRINTPREVIEWWIDGET_P_H

#include "dprintpreviewwidget.h"
#include <DObjectPrivate>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QWheelEvent>

DWIDGET_BEGIN_NAMESPACE

#define PREVIEW_WIDGET_MARGIN_RATIO   50
#define PREVIEW_ENLARGE_RATIO 1.25
#define PREVIEW_NARROW_RATIO 0.8

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(QWidget *parent = nullptr)
        : QGraphicsView(parent)
    {
    }
Q_SIGNALS:
    void resized();

protected:
    void wheelEvent(QWheelEvent *e) override
    {
        if (0 > e->angleDelta().y()) {
            scale(PREVIEW_NARROW_RATIO, PREVIEW_NARROW_RATIO);
        } else {
            scale(PREVIEW_ENLARGE_RATIO, PREVIEW_ENLARGE_RATIO);
        }
    }

    void resizeEvent(QResizeEvent *e) override
    {
        QGraphicsView::resizeEvent(e);
        Q_EMIT resized();
    }

    void showEvent(QShowEvent *e) override
    {
        QGraphicsView::showEvent(e);
        Q_EMIT resized();
    }
};

class PageItem : public QGraphicsItem
{
public:
    PageItem(int _pageNum, const QPicture *_pagePicture, QSize _paperSize, QRect _pageRect)
        : pageNum(_pageNum)
        , pagePicture(_pagePicture)
        , paperSize(_paperSize)
        , pageRect(_pageRect)
    {
        qreal border = qMax(paperSize.height(), paperSize.width()) / PREVIEW_WIDGET_MARGIN_RATIO;
        brect = QRectF(QPointF(-border, -border),
                       QSizeF(paperSize) + QSizeF(2 * border, 2 * border));
        setCacheMode(DeviceCoordinateCache);
    }

    QRectF boundingRect() const override
    {
        return brect;
    }

    inline int pageNumber() const
    {
        return pageNum;
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

protected:
    QPicture grayscalePaint(const QPicture &picture);
    QImage imageGrayscale(const QImage *origin);

private:
    int pageNum;
    const QPicture *pagePicture;
    QSize paperSize;
    QRect pageRect;
    QRectF brect;
};

class DPrintPreviewWidgetPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    explicit DPrintPreviewWidgetPrivate(DPrintPreviewWidget *qq);

    void init();
    void populateScene();
    void generatePreview();
    void fitView();
    void print();

    void setPageRangeAll();
    void setCurrentPage(int page);
    int pagesCount();
    int targetPage(int page);
    int index2page(int index);
    int page2index(int page);

    GraphicsView *graphicsView;
    QGraphicsScene *scene;

    QList<QPicture> targetPictures;
    QList<const QPicture *> pictures;
    QList<QGraphicsItem *> pages;
    QVector<int> pageRange;
    int currentPageNumber = 0;
    DPrinter::ColorMode colorMode;
    DPrintPreviewWidget::Imposition imposition;
    bool isGenerate = false; // 是否重新生成页面后没有显示过第一页
    qreal scale = 1.0;

    DPrinter *previewPrinter;

    Q_DECLARE_PUBLIC(DPrintPreviewWidget)
};

DWIDGET_END_NAMESPACE

#endif // DPRINTPREVIEWWIDGET_P_H
