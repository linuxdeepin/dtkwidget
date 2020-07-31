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

class LIBDTKWIDGETSHARED_EXPORT DPrintPreviewWidget : public QWidget
    , public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
public:
    explicit DPrintPreviewWidget(DPrinter *printer, QWidget *parent = nullptr);

    void setVisible(bool visible) override;
    void setPageRange(const QVector<int> &rangePages);
    int pagesCount();
    bool turnPageAble();
    void showPage(int pageNumber);
    void setColorMode(const DPrinter::ColorMode &colorMode);
    void setOrientation(const DPrinter::Orientation &pageOrientation);
    DPrinter::ColorMode getColorMode();

public Q_SLOTS:
    void updatePreview();
    void turnFront();
    void turnBack();
    void turnBegin();
    void turnEnd();
    void setCurrentPage(int page);

Q_SIGNALS:
    void paintRequested(DPrinter *printer);
    void previewChanged();
    void currentPageChanged(int page);

private:
    D_DECLARE_PRIVATE(DPrintPreviewWidget)
};

DWIDGET_END_NAMESPACE

#endif // DPRINTPREVIEWWIDGET_H
