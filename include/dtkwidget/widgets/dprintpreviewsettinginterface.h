// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DPRINTPREVIEWSETTINGINTERFACE_H
#define DPRINTPREVIEWSETTINGINTERFACE_H

#include <dtkwidget_global.h>
#include <QtPlugin>

#include "dprintpreviewsettinginfo.h"

DWIDGET_BEGIN_NAMESPACE

class DPrintPreviewSettingInterface
{
public:
    enum SettingStatus {
        Default,
        Disabled,
        Hidden
    };

    enum SettingSubControl {
        SC_PrinterWidget,
        SC_CopiesWidget,
        SC_PageRangeWidget,
        SC_PageRange_TypeControl,
        SC_PageRange_SelectEdit,
        SC_OrientationWidget,
        SC_PaperSizeWidget,
        SC_DuplexWidget,
        SC_Duplex_TypeControl,
        SC_NPrintWidget,
        SC_NPrint_Numbers,
        SC_NPrint_Layout,
        SC_PageOrderWidget,
        SC_PageOrder_SequentialPrint,
        SC_PageOrder_TypeControl,
        SC_ColorModeWidget,
        SC_MarginWidget,
        SC_Margin_TypeControl,
        SC_Margin_AdjustContol,
        SC_ScalingWidget,
        SC_WatermarkWidget,
        SC_WatermarkContentWidget,
        SC_Watermark_TypeGroup,
        SC_Watermark_TextType,
        SC_Watermark_CustomText,
        SC_Watermark_TextFont,
        SC_Watermark_TextColor,
        SC_Watermark_ImageEdit,
        SC_Watermark_Layout,
        SC_Watermark_Angle,
        SC_Watermark_Size,
        SC_Watermark_Transparency,

        SC_ControlCount
    };

    virtual ~DPrintPreviewSettingInterface() {}

    virtual QString name() const = 0;
    inline virtual bool settingFilter(const QVariant &mimeData, DPrintPreviewSettingInfo *info)
    {
        Q_UNUSED(mimeData);
        Q_UNUSED(info);
        return false;
    }
    inline virtual SettingStatus settingStatus(const QVariant &mimeData, SettingSubControl control)
    {
        Q_UNUSED(mimeData);
        Q_UNUSED(control);
        return SettingStatus::Default;
    }
};


DWIDGET_END_NAMESPACE

QT_BEGIN_NAMESPACE
#define SettingInterface_iid "org.deepin.dtk.printpreview.SettingInterface/1.0"
Q_DECLARE_INTERFACE(DTK_WIDGET_NAMESPACE::DPrintPreviewSettingInterface, SettingInterface_iid)
QT_END_NAMESPACE
#endif // DPRINTPREVIEWSETTINGINTERFACE_H
