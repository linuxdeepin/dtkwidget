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
#include "private/dframe_p.h"

#include <DIconButton>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QWheelEvent>
#include <QPicture>
#include <qmath.h>

DWIDGET_BEGIN_NAMESPACE

#define PREVIEW_WIDGET_MARGIN_RATIO   50
#define PREVIEW_ENLARGE_RATIO 1.25
#define PREVIEW_NARROW_RATIO 0.8
#define PREVIEW_SCALEBUTTON_MARGIN 10

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(QWidget *parent = nullptr);

public Q_SLOTS:
    void resetScale(bool autoReset = true);

Q_SIGNALS:
    void resized();

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;
    void showEvent(QShowEvent *e) override;
    void changeEvent(QEvent *e) override;

private Q_SLOTS:
    void onThemeTypeChanged(DGuiApplicationHelper::ColorType themeType);

private:
    DIconButton *scaleResetButton;
    double scaleRatio;
};


class ContentItem : public QGraphicsItem
{
public:
    ContentItem(const QPicture *_pagePicture, QRect _pageRect, QGraphicsItem *parent = nullptr)
        : QGraphicsItem(parent)
        , pagePicture(_pagePicture)
        , pageRect(_pageRect)
    {
        brect = QRectF(QPointF(0, 0), QSizeF(pageRect.size()));
        setCacheMode(DeviceCoordinateCache);
        setPos(pageRect.topLeft());
    }

    QRectF boundingRect() const override
    {
        return brect;
    }

    void setRect(const QRectF &rect)
    {
        setPos(rect.topLeft());
        brect = QRectF(QPointF(0, 0), QSizeF(rect.size()));
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
    void updateGrayContent();

protected:
    QPicture grayscalePaint(const QPicture &picture);
    QImage imageGrayscale(const QImage *origin);
private:
    const QPicture *pagePicture;
    QRect pageRect;
    QRectF brect;
    QPicture grayPicture;
};

class WaterMark : public QGraphicsItem
{
public:
    enum Type {
        None,
        Text,
        Image
    };
    enum Layout {
        Center,
        Tiled
    };
    WaterMark(QGraphicsItem *parent = nullptr)
        : QGraphicsItem(parent)
        , type(None)
        , layout(Center)
    {
    }

    inline void setType(Type t)
    {
        type = t;
    }
    inline Type getType() const
    {
        return type;
    }
    inline void setLayoutType(Layout l)
    {
        layout = l;
    }
    inline void setScaleFactor(qreal scale)
    {
        mScaleFactor = scale;
    }
    void setImage(const QImage &img);
    inline void setText(const QString str)
    {
        type = Text;
        text = str;
    }
    inline void setFont(const QFont &f)
    {
        font = f;
    }
    inline QFont getFont() const
    {
        return font;
    }
    inline void setColor(const QColor &c)
    {
        color = c;
    }
    inline void setBoundingRect(const QRectF &rect)
    {
        qreal rotate = rotation();
        setRotation(0);
        brect = rect;
        brectPolygon = mapToScene(brect);
        qreal width = brect.width();
        qreal height = brect.height();
        // 取斜边为宽度的矩形 使旋转时始终保持页面在水印内部
        qreal maxDis = qSqrt(qPow(width, 2) + qPow(height, 2));
        twoPolygon = mapToScene(QRectF(QPointF(brect.center().x() - maxDis / 2, brect.center().y() - maxDis / 2), QSizeF(maxDis, maxDis)));
        setTransformOriginPoint(brect.center());
        setRotation(rotate);
    }
    QRectF boundingRect() const override
    {
        return mapToScene(brect.toRect()).boundingRect();
    }
    inline QPolygonF itemMaxPolygon() const
    {
        return twoPolygon;
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
    void updatePicture(QPainter *painter);

protected:
    QPainterPath itemClipPath() const;

private:
    Type type;
    Layout layout;
    QImage sourceImage;
    QImage graySourceImage;
    QImage targetImage;
    QRectF brect;
    qreal mScaleFactor = 1.0;
    QGraphicsTextItem textItem;
    QString text;
    QFont font;
    QColor color;

    QPolygonF brectPolygon;
    QPolygonF twoPolygon;
};

class PageItem : public QGraphicsItem
{
public:
    PageItem(int _pageNum, const QPicture *_pagePicture, QSize _paperSize, QRect _pageRect)
        : pageNum(_pageNum)
        , pagePicture(_pagePicture)
        , paperSize(_paperSize)
        , pageRect(_pageRect)
        , content(new ContentItem(_pagePicture, _pageRect, this))
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

    void setVisible(bool isVisible);

private:
    int pageNum;
    const QPicture *pagePicture;
    QSize paperSize;
    QRect pageRect;
    QRectF brect;
    ContentItem *content;
};

typedef QList<QPair<QByteArray, QByteArray>> PrintOptions;
class DPrintPreviewWidgetPrivate : public DFramePrivate
{
public:
    // 水印刷新机制，包括立刻刷新和延时刷新
    enum RefreshMode { RefreshImmediately,
                       RefreshDelay };
    explicit DPrintPreviewWidgetPrivate(DPrintPreviewWidget *qq);

    void init();
    void populateScene();
    void generatePreview();
    void fitView();
    void print(bool printAsPicture = false);

    void setPageRangeAll();
    void setCurrentPage(int page);
    int pagesCount();
    int targetPage(int page);
    int index2page(int index);
    int page2index(int page);
    void impositionPages();
    QImage generateWaterMarkImage() const;
    PrintOptions printerOptions();
    void printByCups();
    void generatePreviewPicture();

    GraphicsView *graphicsView;
    QGraphicsScene *scene;

    QList<QPicture> targetPictures;
    QList<const QPicture *> pictures;
    QList<QGraphicsItem *> pages;
    QGraphicsRectItem *background;
    WaterMark *waterMark;
    QVector<int> pageRange;
    int currentPageNumber = 0;
    DPrinter::ColorMode colorMode;
    DPrintPreviewWidget::Imposition imposition;
    DPrintPreviewWidget::Order order;
    qreal scale = 1.0;
    DPrintPreviewWidget::PageRange pageRangeMode = DPrintPreviewWidget::AllPage;
    bool reviewChanged = true;

    DPrinter *previewPrinter;
    RefreshMode refreshMode;

    QString printFromPath;
    DPrintPreviewWidget::PrintMode printMode;
    bool isAsynPreview;
    QVector<int> previewPages;
    bool asynPreviewNeedUpdate;
    int asynPreviewTotalPage;

    Q_DECLARE_PUBLIC(DPrintPreviewWidget)
};

DWIDGET_END_NAMESPACE

#endif // DPRINTPREVIEWWIDGET_P_H
