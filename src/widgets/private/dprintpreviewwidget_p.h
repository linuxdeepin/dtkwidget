// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DPRINTPREVIEWWIDGET_P_H
#define DPRINTPREVIEWWIDGET_P_H

#include <dprintpreviewwidget.h>
#include "dframe_p.h"

#include <DIconButton>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QWheelEvent>
#include <QPicture>
#include <qmath.h>
#include <QBasicTimer>

DWIDGET_BEGIN_NAMESPACE

#define PREVIEW_WIDGET_MARGIN_RATIO   50
#define PREVIEW_ENLARGE_RATIO 1.25
#define PREVIEW_NARROW_RATIO 0.8
#define PREVIEW_SCALEBUTTON_MARGIN 10
#define PREVIEW_WATER_COUNT_WIDTH 28
#define PREVIEW_WATER_COUNT_HEIGHT 20
#define PREVIEW_WATER_COUNT_SPACE 10
#define NUMBERUP_SCALE_RATIO 1.05
#define NUMBERUP_SPACE_SCALE_RATIO 0.05

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
    void drawNumberUpPictures(QPainter *painter);

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
    inline QColor getColor() const
    {
        return color;
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
    void updatePicture(QPainter *painter, bool isPreview);

    void setNumberUpScale(const qreal &value);

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
    qreal numberUpScale = 1;

    QPolygonF brectPolygon;
    QPolygonF twoPolygon;
    friend class DPrintPreviewWidgetPrivate;
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
    void updatePreview();
    void generatePreview();
    void fitView();
    void print(bool printAsPicture = false);
    void updatePageByPagePrintVector(QVector<int> &pageVector, QList<const QPicture *> &pictures) const;
    void asynPrint(const QPointF &leftTop, const QRect &pageRect, const QVector<int> &pageVector);
    void syncPrint(const QPointF &leftTop, const QRect &pageRect, const QVector<int> &pageVector);
    void printAsImage(const QSize &paperSize, QVector<int> &pageVector);
    void printSinglePageDrawUtil(QPainter *painter, const QSize &translateSize, const QPointF &leftTop, const QImage &waterImage, const QPicture *picture);
    void printMultiPageDrawUtil(QPainter *painter, const QPointF &leftTop, const QImage &waterImage);

    void setPageRangeAll();
    void setCurrentPage(int page);
    int pagesCount();
    int targetPage(int page);
    int index2page(int index);
    int page2index(int page);
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
    D_DECL_DEPRECATED void impositionPages();  // 拼版
#endif
    int impositionPages(DPrintPreviewWidget::Imposition im); // 每页版数
    QImage generateWaterMarkImage() const;
    PrintOptions printerOptions();
    void printByCups();

    void generatePreviewPicture();// 发送requestPaint信号，重新获取原文档数据
    void calculateNumberUpPage();// 重绘页面，当拼版数改变、纸张大小等操作时必须调用，
    void calculateNumberPagePosition();// 计算每小页面的显示位置

    void updateNumberUpContent();
    QVector<int> requestPages(int page);// 取处理后page页的小页面
    void releaseImpositionData();// 并打切换单页时需要释放水印、页面拼版数据

    void displayWaterMarkItem();// 添加或更新水印效果，
    void calculateNumberPageScale();// 计算缩放比，拼版数发生改变需要调用
    void calculateCurrentNumberPage();// page是相对于原文档,添加page页需要显示的小页面到Vector
    QByteArray foundColorModelByCups() const;

    inline void setCurrentPageNumber(int page)
    {
        currentPageNumber = page;
    }

    GraphicsView *graphicsView;
    QGraphicsScene *scene;

    QList<QPicture> targetPictures;
    QList<const QPicture *> pictures;
    QList<QGraphicsItem *> pages;
    QGraphicsRectItem *background;
    WaterMark *waterMark;
    QVector<int> pageRange; // 选择的页码
    int currentPageNumber = 0; // 处理以后当前页，值一定是连续的，比如处理共10页，那么取值就是1到10
    DPrinter::ColorMode colorMode;
    DPrintPreviewWidget::Imposition imposition;
    DPrintPreviewWidget::Order order;
    qreal scale = 1.0;
    DPrintPreviewWidget::PageRange pageRangeMode = DPrintPreviewWidget::AllPage;
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
    D_DECL_DEPRECATED bool reviewChanged = true; // 预览页面是否发生改变
#endif
    DPrinter *previewPrinter;
    RefreshMode refreshMode;

    QString printFromPath;
    DPrintPreviewWidget::PrintMode printMode;
    bool isAsynPreview;
    QVector<int> previewPages;
    bool asynPreviewNeedUpdate;
    int asynPreviewTotalPage;
    int pageCopyCount = 0;
    bool isFirstPage;

    struct NumberUpData;
    NumberUpData *numberUpPrintData;
    QBasicTimer updateTimer;
    Q_DECLARE_PUBLIC(DPrintPreviewWidget)
};

struct DPrintPreviewWidgetPrivate::NumberUpData {
    class NumberItem : public QGraphicsItem
    {
    public:
        NumberItem(QVector<int> _pageNumberVector, QVector<QPointF> _numberPointVector, QRect _pageRect)
            : numberVector(_pageNumberVector)
            , numberPointVector(_numberPointVector)
        {
            brect = QRectF(QPointF(0, 0), QSizeF(_pageRect.size()));
            setCacheMode(DeviceCoordinateCache);
            setPos(_pageRect.topLeft());
        }

        inline void setPageNumbers(const QVector<int> &pageNumber)
        {
            numberVector = pageNumber;
        }

        inline void setNumberPositon(const QVector<QPointF> &numberPos)
        {
            numberPointVector = numberPos;
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

    private:
        QRectF brect;
        QVector<int> numberVector; // 页码
        QVector<QPointF> numberPointVector; // 坐标
    };

    struct WaterMarkProperty {
        WaterMark::Layout layout;
        WaterMark::Type type;
        qreal rotation;
        qreal scale;
        qreal opacity;
        QColor color;
        QString text;
        QImage sourceImage;
        QImage grayImage;
        QFont font;
    };

    QVector<QPair<int, const QPicture *>> previewPictures; // 并打当前页面属性列表（页码，图片）
    QVector<QPointF> paintPoints; // 并打单页位置属性列表
    qreal scaleRatio; // 并打页面缩放因子
    int rowCount; // 并打页面行数
    int columnCount; // 并打页面列数
    QPointF pageStartPoint; // 并打单页起始位置
    QList<WaterMark *> waterList; // 并打时水印
    WaterMarkProperty *waterProperty;
    NumberItem *numberItem;
    QGraphicsRectItem *waterParentItem;
    bool needRecreateWater; // 重绘时不管当前页面水印数量和内容数量一致 都要重新生成页面 因为页面大小发生了变化
    DPrintPreviewWidgetPrivate *parent;

    void resetData()
    {
        scaleRatio = 1;
        rowCount = 0;
        columnCount = 0;
        pageStartPoint = QPointF(0, 0);
    }

    QVector<QPointF> updatePositions(const qreal &scale)
    {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        QRectF pageRect = parent->previewPrinter->pageLayout().paintRectPixels(parent->previewPrinter->resolution());
#else
        QRectF pageRect = parent->previewPrinter->pageRect();
#endif
        QVector<QPointF> posList;
        QPointF startP(0, 0);

        QPointF topLeft;
        if (scale >= 1.0) {
            topLeft = QPointF(0, 0);
            if ((parent->imposition == DPrintPreviewWidget::OneRowTwoCol) || (parent->imposition == DPrintPreviewWidget::TwoRowThreeCol)) {
                topLeft.setY(pageStartPoint.y() * (scale - 1.0));
            }
        } else {
            topLeft.setX(pageRect.width() * (1.0 - scale) / 2.0);
            topLeft.setY((NUMBERUP_SCALE_RATIO * rowCount - NUMBERUP_SPACE_SCALE_RATIO) * pageRect.height() * scaleRatio * (1.0 - scale) / 2.0);
        }

        switch (parent->order) {
        case DPrintPreviewWidget::Copy:
        case DPrintPreviewWidget::L2R_T2B: {
            startP = pageStartPoint + topLeft;
            for (int i = 0; i < rowCount; ++i) {
                for (int j = 0; j < columnCount; ++j) {
                    posList.append(startP + QPointF(NUMBERUP_SCALE_RATIO * j * pageRect.width(), NUMBERUP_SCALE_RATIO * i * pageRect.height()) * scaleRatio * scale);
                }
            }
        } break;
        // R2L 从右往左扩大 应为从0,0开始扩大 需增加一个expanding使其从左往右扩张
        case DPrintPreviewWidget::R2L_T2B: {
            qreal expanding = scale > 1 ? pageRect.width() * (scale - 1) : 0;
            startP = pageStartPoint - QPointF(topLeft.x() - expanding - pageRect.width(), -topLeft.y());
            for (int i = 0; i < rowCount; ++i) {
                for (int j = 0; j < columnCount; ++j) {
                    posList.append(startP - QPointF((NUMBERUP_SCALE_RATIO * j + 1) * pageRect.width(), -(NUMBERUP_SCALE_RATIO * i) * pageRect.height()) * scaleRatio * scale);
                }
            }
        } break;
        case DPrintPreviewWidget::T2B_L2R: {
            startP = pageStartPoint + topLeft;
            for (int i = 0; i < columnCount; ++i) {
                for (int j = 0; j < rowCount; ++j) {
                    posList.append(startP + QPointF(NUMBERUP_SCALE_RATIO * i * pageRect.width(), NUMBERUP_SCALE_RATIO * j * pageRect.height()) * scaleRatio * scale);
                }
            }
        } break;
        case DPrintPreviewWidget::T2B_R2L: {
            qreal expanding = scale > 1 ? pageRect.width() * (scale - 1) : 0;
            startP = pageStartPoint - QPointF(topLeft.x() - expanding - pageRect.width(), -topLeft.y());
            for (int i = 0; i < columnCount; ++i) {
                for (int j = 0; j < rowCount; ++j) {
                    posList.append(startP - QPointF((NUMBERUP_SCALE_RATIO * i + 1) * pageRect.width(), -(NUMBERUP_SCALE_RATIO * j) * pageRect.height()) * scaleRatio * scale);
                }
            }
        } break;
        }

        return posList;
    }

    void setWaterMarksScale(qreal scale)
    {
        if (waterList.isEmpty())
            return;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        QRectF pageRect = parent->previewPrinter->pageLayout().paintRectPixels(parent->previewPrinter->resolution());
#else
        QRectF pageRect = parent->previewPrinter->pageRect();
#endif
        QMargins pageMargins = parent->previewPrinter->pageLayout().marginsPixels(parent->previewPrinter->resolution());
        const QVector<QPointF> &posList = updatePositions(scale);

        for (int c = 0; c < waterList.count(); ++c) {
            WaterMark *item = waterList.at(c);
            item->setBoundingRect(QRectF(QPointF(pageMargins.left(), pageMargins.top()) + posList.at(c), pageRect.size() * scaleRatio * scale));
            item->update();
        }

        if (numberItem)
            numberItem->update();
    }

    void setWaterMarkOriginProperties(WaterMark *wm)
    {
        if (!wm || !waterProperty)
            return;

        wm->type = waterProperty->type;
        wm->layout = waterProperty->layout;
        wm->mScaleFactor = waterProperty->scale;
        wm->color = waterProperty->color;
        wm->text = waterProperty->text;
        wm->sourceImage = waterProperty->sourceImage;
        wm->graySourceImage = waterProperty->grayImage;
        wm->font = waterProperty->font;
        wm->setRotation(waterProperty->rotation);
        wm->setOpacity(waterProperty->opacity);
    }

    void copyWaterMarkProperties()
    {
        // 当并打属性发生变化的时候 需要删除当前页面中的水印层
        // 因此需要保存当前层的水印效果以便创建时直接添加
        if (!parent->waterMark && waterList.isEmpty())
            return;

        WaterMark *wm;
        if (waterList.isEmpty()) {
            wm = parent->waterMark;
        } else {
            wm = waterList.first();
        }

        if (!waterProperty)
            waterProperty = new WaterMarkProperty;

        waterProperty->type = wm->type;
        waterProperty->layout = wm->layout;
        waterProperty->rotation = wm->rotation();
        waterProperty->scale = wm->mScaleFactor;
        waterProperty->opacity = wm->opacity();
        waterProperty->color = wm->color;
        waterProperty->text = wm->text;
        waterProperty->sourceImage = wm->sourceImage;
        waterProperty->grayImage = wm->graySourceImage;
        waterProperty->font = wm->font;
    }

    void updateWaterMarks()
    {
        if (waterList.isEmpty())
            return;

        for (auto *item : std::as_const(waterList))
            item->update();
    }

    template<typename T>
    void setWaterMarkProperty(T outFunction)
    {
        if (waterList.isEmpty())
            return;

        auto *firstWm = waterList.first();
        outFunction(firstWm);

        for (auto *item : std::as_const(waterList)) {
            if (item == firstWm)
                continue;

            // TODO: remove it in dtkwidget 5.6.
            item->type = firstWm->type;
            item->layout = firstWm->layout;
            item->mScaleFactor = firstWm->mScaleFactor;
            item->color = firstWm->color;
            item->text = firstWm->text;
            item->sourceImage = firstWm->sourceImage;
            item->graySourceImage = firstWm->graySourceImage;
            item->font = firstWm->font;
            item->setRotation(firstWm->rotation());
            item->setOpacity(firstWm->opacity());
        }
    }

    NumberUpData(DPrintPreviewWidgetPrivate *parent)
        : previewPictures(0)
        , paintPoints(0)
        , scaleRatio(1)
        , rowCount(0)
        , columnCount(0)
        , pageStartPoint(0, 0)
        , waterProperty(nullptr)
        , numberItem(nullptr)
        , waterParentItem(nullptr)
        , needRecreateWater(false)
        , parent(parent)
    {
    }

    ~NumberUpData()
    {
        delete waterProperty;
        delete numberItem;
        delete waterParentItem;
    }
};

DWIDGET_END_NAMESPACE
Q_DECLARE_TYPEINFO(DTK_WIDGET_NAMESPACE::DPrintPreviewWidgetPrivate::NumberUpData::WaterMarkProperty, Q_PRIMITIVE_TYPE);
#endif // DPRINTPREVIEWWIDGET_P_H
