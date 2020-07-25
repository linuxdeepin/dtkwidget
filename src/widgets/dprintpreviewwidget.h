#ifndef DPRINTPREVIEWWIDGET_H
#define DPRINTPREVIEWWIDGET_H

#include <dtkwidget_global.h>
#include <DObject>
#include <QWidget>
#include <QPainter>
#include <QStyleOption>
#include <QGraphicsItem>

#define private protected
#include <QPrinter>
#undef private

DWIDGET_BEGIN_NAMESPACE

class DPrintPreviewWidgetPrivate;

class DPrinter : public QPrinter
{
public:
    explicit DPrinter(PrinterMode mode = ScreenResolution);
    ~DPrinter() {}

    void setPreviewMode(bool isPreview);

    QList<const QPicture *> getPrinterPages();

private:
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
        qreal border = qMax(paperSize.height(), paperSize.width()) / 50;
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

private:
    int pageNum;
    const QPicture *pagePicture;
    QSize paperSize;
    QRect pageRect;
    QRectF brect;
};

class LIBDTKWIDGETSHARED_EXPORT DPrintPreviewWidget : public QWidget
    , public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
public:
    explicit DPrintPreviewWidget(DPrinter *printer, QWidget *parent = nullptr);

    void setVisible(bool visible) override;

public Q_SLOTS:
    void updatePreview();

Q_SIGNALS:
    void paintRequested(DPrinter *printer);
    void previewChanged();

private:
    D_DECLARE_PRIVATE(DPrintPreviewWidget)
};

DWIDGET_END_NAMESPACE

#endif // DPRINTPREVIEWWIDGET_H
