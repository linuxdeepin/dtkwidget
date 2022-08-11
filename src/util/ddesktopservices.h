// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DDESKTOPSERVICES_H
#define DDESKTOPSERVICES_H

#include <dtkwidget_global.h>

#include <QUrl>

DWIDGET_BEGIN_NAMESPACE

class DDesktopServices
{
public:

#ifdef Q_OS_LINUX
    enum SystemSoundEffect {
        SSE_Notifications,
        SEE_Screenshot,
        SSE_EmptyTrash,
        SSE_SendFileComplete,
        SSE_BootUp,
        SSE_Shutdown,
        SSE_Logout,
        SSE_WakeUp,
        SSE_VolumeChange,
        SSE_LowBattery,
        SSE_PlugIn,
        SSE_PlugOut,
        SSE_DeviceAdded,
        SSE_DeviceRemoved,
        SSE_Error,
    };
#endif

    static bool showFolder(QString localFilePath, const QString &startupId = QString());
    static bool showFolders(const QList<QString> localFilePaths, const QString &startupId = QString());
    static bool showFolder(QUrl url, const QString &startupId = QString());
    static bool showFolders(const QList<QUrl> urls, const QString &startupId = QString());

    static bool showFileItemPropertie(QString localFilePath, const QString &startupId = QString());
    static bool showFileItemProperties(const QList<QString> localFilePaths, const QString &startupId = QString());
    static bool showFileItemPropertie(QUrl url, const QString &startupId = QString());
    static bool showFileItemProperties(const QList<QUrl> urls, const QString &startupId = QString());

    static bool showFileItem(QString localFilePath, const QString &startupId = QString());
    static bool showFileItems(const QList<QString> localFilePaths, const QString &startupId = QString());
    static bool showFileItem(QUrl url, const QString &startupId = QString());
    static bool showFileItems(const QList<QUrl> urls, const QString &startupId = QString());

    static bool trash(QString localFilePath);
    static bool trash(const QList<QString> localFilePaths);
    static bool trash(QUrl urlstartupId);
    static bool trash(const QList<QUrl> urls);

#ifdef Q_OS_LINUX
    static bool playSystemSoundEffect(const SystemSoundEffect &effect);
    static bool playSystemSoundEffect(const QString &name);
    static bool previewSystemSoundEffect(const SystemSoundEffect &effect);
    static bool previewSystemSoundEffect(const QString &name);
    static QString getNameByEffectType(const SystemSoundEffect &effect);
#endif

    static QString errorMessage();
};

DWIDGET_END_NAMESPACE

#ifdef Q_OS_LINUX
Q_DECLARE_METATYPE(DTK_WIDGET_NAMESPACE::DDesktopServices::SystemSoundEffect)
#endif

#endif // DDESKTOPSERVICES_H
