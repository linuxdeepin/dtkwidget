// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DPRINTPREVIEWWIDGET_H
#define DPRINTPREVIEWWIDGET_H

#include <dtkwidget_global.h>
#include <DGuiApplicationHelper>
#include <DFrame>

#include <QWidget>
#include <QPainter>
#include <QStyleOption>
#include <QGraphicsItem>

#define private protected
#include <QPrinter>
#undef private

DGUI_USE_NAMESPACE

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

class LIBDTKWIDGETSHARED_EXPORT DPrintPreviewWidget : public DFrame
{
    Q_OBJECT
public:
    enum Imposition { // 并打
        One, // 单页
        OneRowTwoCol, // 一行两列
        TwoRowTwoCol, // 两行两列
        TwoRowThreeCol, // 两行三列
        ThreeRowThreeCol, // 三行三列
        FourRowFourCol // 四行四列
    };
    enum PageRange {
        AllPage,
        CurrentPage,
        SelectPage
    };
    enum Order { // 并打顺序
        L2R_T2B, // 从左到右，从上到下
        R2L_T2B, // 从右到左，从上到下
        T2B_L2R, // 从上到下，从左到右
        T2B_R2L, // 从上到下，从右到左
        Copy // 重复
    };

    enum PrintMode { // 打印模式
        PrintToPrinter, // 打印到打印机
        PrintToPdf, // 打印到pdf
        PrintToImage // 另存为图片
    };

    explicit DPrintPreviewWidget(DPrinter *printer, QWidget *parent = nullptr);
    ~DPrintPreviewWidget() override;

    void setVisible(bool visible) override;
    void setPageRange(const QVector<int> &rangePages);
    void setPageRange(int from, int to);
    void setPageRangeALL();
    D_DECL_DEPRECATED void setReGenerate(bool generate);
    void setPageRangeMode(PageRange mode);
    PageRange pageRangeMode();
    void reviewChange(bool generate);
    int pagesCount();
    int currentPage();
    bool turnPageAble();
    void setColorMode(const DPrinter::ColorMode &colorMode);
    void setOrientation(const DPrinter::Orientation &pageOrientation);
    DPrinter::ColorMode getColorMode();
    void setScale(qreal scale);
    qreal getScale() const;
    void updateView();
    void updateWaterMark();
    void refreshBegin();
    void refreshEnd();
    void setWaterMarkType(int type);
    void setWaterMargImage(const QImage &image);
    void setWaterMarkRotate(qreal rotate);
    void setWaterMarkScale(qreal scale);
    void setWaterMarkOpacity(qreal opacity);
    void setConfidentialWaterMark();
    void setDraftWaterMark();
    void setSampleWaterMark();
    void setCustomWaterMark(const QString &text);
    void setTextWaterMark(const QString &text);
    void setWaterMarkFont(const QFont &font);
    QColor waterMarkColor() const;
    void setWaterMarkColor(const QColor &color);
    void setWaterMarkLayout(int layout);
    void setImposition(Imposition im);
    Imposition imposition() const;
    void setOrder(Order order);
    DPrintPreviewWidget::Order order() const;
    void setPrintFromPath(const QString &path);
    QString printFromPath() const;
    void setPrintMode(PrintMode pt);
    void setAsynPreview(int totalPage);
    bool isAsynPreview() const;
    void isPageByPage(int pageCopy,bool isFirst);
    int targetPageCount(int pageCount);
    int originPageCount();
    QByteArray printerColorModel() const;

public Q_SLOTS:
    void updatePreview();
    void turnFront();
    void turnBack();
    void turnBegin();
    void turnEnd();
    void setCurrentPage(int page);
    void print(bool isSavedPicture = false);
    void themeTypeChanged(DGuiApplicationHelper::ColorType themeType);

Q_SIGNALS:
    void paintRequested(DPrinter *printer);
    void paintRequested(DPrinter *printer, const QVector<int> &pageRange);
    void previewChanged();
    void currentPageChanged(int page);
    void totalPages(int);
    void pagesCountChanged(int pages);

private:
    void timerEvent(QTimerEvent *event) override;
    void setCurrentTargetPage(int page);

    D_DECLARE_PRIVATE(DPrintPreviewWidget)
    friend class ContentItem;
};

DWIDGET_END_NAMESPACE

#endif // DPRINTPREVIEWWIDGET_H
