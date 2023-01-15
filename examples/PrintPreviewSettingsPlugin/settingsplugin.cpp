// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "settingsplugin.h"

#include "dprintpreviewwidget.h"

PrintPreviewSettingsPlugin::PrintPreviewSettingsPlugin(QObject *parent) :
    QObject(parent)
{
}

QString PrintPreviewSettingsPlugin::name() const
{
    return QLatin1String("WaterMarkFilter");
}

bool PrintPreviewSettingsPlugin::settingFilter(const QVariant &mimeData, DPrintPreviewSettingInfo *info)
{
    const QString &path = mimeData.toString();

    // Filter the special file
    if (!path.endsWith("secrecy"))
        return false;

    switch (info->type()) {
    case DPrintPreviewSettingInfo::PS_Printer: {
        auto printerInfo = static_cast<DPrintPreviewPrinterInfo *>(info);
        auto &printers = printerInfo->printers;
        auto it = std::remove_if(printers.begin(), printers.end(), [](const QString &printer) {
            if (printer.contains("Pdf", Qt::CaseInsensitive))
                return true;
            return false;
        });

        if (it == printers.end())
            break;
        printers.erase(it);
        printerInfo->printers = printers;
    }
        return true;
    case DPrintPreviewSettingInfo::PS_Copies: {
        auto copiesInfo = static_cast<DPrintPreviewCopiesInfo *>(info);
        copiesInfo->copies = 2;
    }
        return true;
    case DPrintPreviewSettingInfo::PS_Scaling: {
        auto scalingInfo = static_cast<DPrintPreviewScalingInfo *>(info);
        scalingInfo->scalingType = DPrintPreviewScalingInfo::ScaleSize;
        scalingInfo->scaleRatio = 20;
    }
        return true;
    case DPrintPreviewSettingInfo::PS_ColorMode: {
        auto colorModeInfo = static_cast<DPrintPreviewColorModeInfo *>(info);
        auto &colorModes = colorModeInfo->colorMode;
        auto it = std::remove_if(colorModes.begin(), colorModes.end(), [](const QString &printer) {
            if (printer.contains("color", Qt::CaseInsensitive))
                return true;
            return false;
        });

        if (it == colorModes.end())
            break;
        colorModes.erase(it);
        colorModeInfo->colorMode = colorModes;
    }
        return true;
    case DPrintPreviewSettingInfo::PS_PageOrder: {
        auto pageOrderInfo = static_cast<DPrintPreviewPageOrderInfo *>(info);
        pageOrderInfo->pageOrder = DPrintPreviewPageOrderInfo::InOrderPage;
        pageOrderInfo->inOrdertype = DPrintPreviewPageOrderInfo::BackToFront;
    }
        return true;
    case DPrintPreviewSettingInfo::PS_PageRange: {
        auto pageRangeInfo = static_cast<DPrintPreviewPageRangeInfo *>(info);
        pageRangeInfo->rangeType = DPrintPreviewWidget::SelectPage;
        pageRangeInfo->selectPages = "1,2,3,4";
    }
        return true;
    case DPrintPreviewSettingInfo::PS_PaperSize: {
        auto paperSizeInfo = static_cast<DPrintPreviewPaperSizeInfo *>(info);
        if (paperSizeInfo->pageSize.contains("A4")) {
            paperSizeInfo->pageSize = (QStringList() << "A4");
            return true;
        }
        return false;
    }
    case DPrintPreviewSettingInfo::PS_Watermark: {
        auto waterMarkInfo = static_cast<DPrintPreviewWatermarkInfo *>(info);
        waterMarkInfo->currentWatermarkType = DPrintPreviewWatermarkInfo::TextWatermark;
        waterMarkInfo->textType = DPrintPreviewWatermarkInfo::Custom;
        waterMarkInfo->customText = "控制的自定义文本";
        waterMarkInfo->textColor = Qt::red;

        waterMarkInfo->opened = true;
        waterMarkInfo->angle = 0;
        waterMarkInfo->layout = DPrintPreviewWatermarkInfo::Tiled;
        waterMarkInfo->size = 20;
        waterMarkInfo->transparency = 60;
        waterMarkInfo->rowSpacing = 1.0;
        waterMarkInfo->columnSpacing = 0.5;
    }
        return true;
    case DPrintPreviewSettingInfo::PS_NUpPrinting: {
        auto nupprintInfo = static_cast<DPrintPreviewNUpPrintInfo *>(info);
        nupprintInfo->enable = true;
        nupprintInfo->imposition = DPrintPreviewWidget::OneRowTwoCol;
    }
        return true;
    case DPrintPreviewSettingInfo::PS_Orientation: {
        auto orientation = static_cast<DPrintPreviewOrientationInfo *>(info);
        orientation->orientationMode = DPrinter::Landscape;
    }
        return true;
    case DPrintPreviewSettingInfo::PS_PrintDuplex: {
        auto duplexInfo = static_cast<DPrintPreviewPrintDuplexInfo *>(info);
        duplexInfo->enable = true;
        duplexInfo->duplex = DPrinter::DuplexLongSide;
    }
        return true;
    case DPrintPreviewSettingInfo::PS_PaperMargins: {
        auto marginInfo = static_cast<DPrintPreviewPaperMarginsInfo *>(info);
        marginInfo->marginType = DPrintPreviewPaperMarginsInfo::Customize;
        marginInfo->leftMargin = 40;
    }
        return true;
    default:
        break;
    }

    return false;
}

DPrintPreviewSettingInterface::SettingStatus PrintPreviewSettingsPlugin::settingStatus(const QVariant &mimeData, DPrintPreviewSettingInterface::SettingSubControl control)
{
    const QString &path = mimeData.toString();

    // Filter the special file
    if (!path.endsWith("secrecy"))
        return DPrintPreviewSettingInterface::settingStatus(mimeData, control);

    DPrintPreviewSettingInterface::SettingStatus status = DPrintPreviewSettingInterface::Default;
    switch (control) {
    case DPrintPreviewSettingInterface::SC_WatermarkWidget:
        status = DPrintPreviewSettingInterface::Disabled;
        break;
    default:
        break;
    }
    return status;
}
