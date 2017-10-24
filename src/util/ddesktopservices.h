/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef DDESKTOPSERVICES_H
#define DDESKTOPSERVICES_H

#include "dtkwidget_global.h"

#include <QUrl>

DWIDGET_BEGIN_NAMESPACE

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

    static bool trash(QString localFilePath);
    static bool trash(const QList<QString> localFilePaths);
    static bool trash(QUrl urlstartupId);
    static bool trash(const QList<QUrl> urls);

#ifdef Q_OS_UNIX
    static bool playSystemSoundEffect(const SystemSoundEffect &effect);
    static bool playSystemSoundEffect(const QString &name);
#endif

    static QString errorMessage();
};

DWIDGET_END_NAMESPACE

#endif // DDESKTOPSERVICES_H
