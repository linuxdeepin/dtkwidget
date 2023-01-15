// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DPRINTPREVIEWSETTINGS_H
#define DPRINTPREVIEWSETTINGS_H

#include <dtkwidget_global.h>

#include "dprintpreviewwidget.h"

DWIDGET_BEGIN_NAMESPACE

class DPrintPreviewSettingInfo
{
public:
    enum SettingType {
        PS_Printer,
        PS_Copies,
        PS_PageRange,
        PS_Orientation,
        PS_PaperSize,
        PS_PrintDuplex,
        PS_NUpPrinting,
        PS_PageOrder,
        PS_ColorMode,
        PS_PaperMargins,
        PS_Scaling,
        PS_Watermark,
        PS_SettingsCount = 65535
    };

    explicit DPrintPreviewSettingInfo(SettingType type);
    virtual ~DPrintPreviewSettingInfo();

    inline SettingType type() const {
        return static_cast<SettingType>(t);
    }

private:
    ushort t;
};

class DPrintPreviewPrinterInfo : public DPrintPreviewSettingInfo
{
public:
    DPrintPreviewPrinterInfo()
        : DPrintPreviewSettingInfo(PS_Printer)
    {

    }

    QStringList printers;
};

class DPrintPreviewCopiesInfo : public DPrintPreviewSettingInfo
{
public:
    DPrintPreviewCopiesInfo()
        : DPrintPreviewSettingInfo(PS_Copies)
    {

    }

    int copies;
};

class DPrintPreviewPageRangeInfo : public DPrintPreviewSettingInfo
{
public:
    DPrintPreviewPageRangeInfo()
        : DPrintPreviewSettingInfo(PS_PageRange)
    {

    }

    DPrintPreviewWidget::PageRange rangeType;
    QString selectPages;
};

class DPrintPreviewOrientationInfo : public DPrintPreviewSettingInfo
{
public:
    DPrintPreviewOrientationInfo()
        : DPrintPreviewSettingInfo(PS_Orientation)
    {

    }

    DPrinter::Orientation orientationMode;
};

class DPrintPreviewPaperSizeInfo : public DPrintPreviewSettingInfo
{
public:
    DPrintPreviewPaperSizeInfo()
        : DPrintPreviewSettingInfo(PS_PaperSize)
    {

    }

    QStringList pageSize;
};

class DPrintPreviewPrintDuplexInfo : public DPrintPreviewSettingInfo
{
public:
    DPrintPreviewPrintDuplexInfo()
        : DPrintPreviewSettingInfo(PS_PrintDuplex)
    {

    }

    bool enable;
    DPrinter::DuplexMode duplex;
};

class DPrintPreviewNUpPrintInfo : public DPrintPreviewSettingInfo
{
public:
    DPrintPreviewNUpPrintInfo()
        : DPrintPreviewSettingInfo(PS_NUpPrinting)
    {

    }

    bool enable;
    DPrintPreviewWidget::Imposition imposition;
    DPrintPreviewWidget::Order order;
};

class DPrintPreviewPageOrderInfo : public DPrintPreviewSettingInfo
{
public:
    enum PageOrder {
        CollatePage,
        InOrderPage
    };
    enum OrderType {
        FrontToBack,
        BackToFront
    };
    DPrintPreviewPageOrderInfo()
        : DPrintPreviewSettingInfo(PS_PageOrder)
    {

    }

    PageOrder pageOrder;
    OrderType inOrdertype;
};

class DPrintPreviewColorModeInfo : public DPrintPreviewSettingInfo
{
public:
    DPrintPreviewColorModeInfo()
        : DPrintPreviewSettingInfo(PS_ColorMode)
    {

    }

    QStringList colorMode;
};

class DPrintPreviewPaperMarginsInfo : public DPrintPreviewSettingInfo
{
public:
    enum MarginType {
        Narrow,
        Normal,
        Moderate,
        Customize
    };
    DPrintPreviewPaperMarginsInfo()
        : DPrintPreviewSettingInfo(PS_PaperMargins)
    {

    }

    MarginType marginType;
    qreal topMargin;
    qreal bottomMargin;
    qreal leftMargin;
    qreal rightMargin;
};

class DPrintPreviewScalingInfo : public DPrintPreviewSettingInfo
{
public:
    enum ScalingType {
        ActualSize,
        ScaleSize
    };
    DPrintPreviewScalingInfo()
        : DPrintPreviewSettingInfo(PS_Scaling)
    {

    }

    ScalingType scalingType;
    int scaleRatio;
};

class DPrintPreviewWatermarkInfo : public DPrintPreviewSettingInfo
{
public:
    enum WatermarkType {
        TextWatermark,
        ImageWatermark
    };
    enum Layout {
        Tiled,
        Center
    };
    enum TextType {
        Confidential,
        Draft,
        Sample,
        Custom
    };

    DPrintPreviewWatermarkInfo()
        : DPrintPreviewSettingInfo(PS_Watermark)
    {

    }

    bool opened;
    int angle;
    int size;
    int transparency;
    qreal rowSpacing;
    qreal columnSpacing;
    Layout layout;
    WatermarkType currentWatermarkType;
    TextType textType;
    QString customText;
    QStringList fontList;
    QColor textColor;
    QString imagePath;
};

DWIDGET_END_NAMESPACE
#endif // DPRINTPREVIEWSETTINGS_H
