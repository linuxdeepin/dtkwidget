// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef PRINTPREVIEWSETTINGSPLUGIN_H
#define PRINTPREVIEWSETTINGSPLUGIN_H

#include <QObject>
#include "dprintpreviewsettinginterface.h"

DWIDGET_USE_NAMESPACE

class PrintPreviewSettingsPlugin : public QObject, public DPrintPreviewSettingInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.deepin.dtk.plugin.PrintPreview.SettingsExample" FILE "PrintPreviewSettingsPlugin.json")

public:
    PrintPreviewSettingsPlugin(QObject *parent = nullptr);
    QString name() const;
    bool settingFilter(const QVariant &mimeData, DPrintPreviewSettingInfo *info);
    SettingStatus settingStatus(const QVariant &mimeData, SettingSubControl control);
};

#endif // PRINTPREVIEWSETTINGSPLUGIN_H
