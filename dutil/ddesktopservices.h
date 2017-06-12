/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DDESKTOPSERVICES_H
#define DDESKTOPSERVICES_H

#include "dutil_global.h"

#include <QUrl>

DUTIL_BEGIN_NAMESPACE

class DDesktopServices
{
public:

#ifdef Q_OS_LINUX
    enum SystemSoundEffect {
        SSE_Notifications,
        SSE_Screenshot,
        SSE_EmptyTrash,
        SSE_SendFileComplete
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

#ifdef Q_OS_UNIX
    static bool playSystemSoundEffect(const SystemSoundEffect &effect);
    static bool playSystemSoundEffect(const QString &name);
#endif

    static QString errorMessage();
};

DUTIL_END_NAMESPACE

#endif // DDESKTOPSERVICES_H
