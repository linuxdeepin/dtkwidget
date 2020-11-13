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

#ifndef DPRINTPREVIEWWIDGET_H
#define DPRINTPREVIEWWIDGET_H

#include <dtkwidget_global.h>
#include <DApplicationHelper>
#include <DFrame>

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

    explicit DPrintPreviewWidget(DPrinter *printer, QWidget *parent = nullptr);

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
    void setWaterMarkColor(const QColor &color);
    void setWaterMarkLayout(int layout);
    void setImposition(Imposition im);
    void setOrder(Order order);

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
    void previewChanged();
    void currentPageChanged(int page);
    void totalPages(int);
    void pagesCountChanged(int pages);

private:
    void setCurrentTargetPage(int page);

    D_DECLARE_PRIVATE(DPrintPreviewWidget)
};

DWIDGET_END_NAMESPACE

#endif // DPRINTPREVIEWWIDGET_H
