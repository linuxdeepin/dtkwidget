// SPDX-FileCopyrightText: 2017 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "ddesktopservices.h"

DWIDGET_BEGIN_NAMESPACE

static QStringList urls2uris(const QList<QUrl> &urls)
{
    QStringList list;

    list.reserve(urls.size());

    for (const QUrl url : urls) {
        list << url.toString();
    }

    return list;
}

static QList<QUrl> path2urls(const QList<QString> &paths)
{
    QList<QUrl> list;

    list.reserve(paths.size());

    for (const QString &path : paths) {
        list << QUrl::fromLocalFile(path);
    }

    return list;
}

bool DDesktopServices::showFolder(QString localFilePath, const QString &startupId)
{
    Q_UNUSED(localFilePath);
    Q_UNUSED(startupId);
    return false;
}

bool DDesktopServices::showFolders(const QList<QString> localFilePaths, const QString &startupId)
{
    Q_UNUSED(localFilePaths);
    Q_UNUSED(startupId);
    return false;
}

bool DDesktopServices::showFolder(QUrl url, const QString &startupId)
{
    Q_UNUSED(url);
    Q_UNUSED(startupId);
    return false;
}

bool DDesktopServices::showFolders(const QList<QUrl> urls, const QString &startupId)
{
    Q_UNUSED(urls);
    Q_UNUSED(startupId);
    return false;
}

bool DDesktopServices::showFileItemPropertie(QString localFilePath, const QString &startupId)
{
    Q_UNUSED(localFilePath);
    Q_UNUSED(startupId);
    return false;
}

bool DDesktopServices::showFileItemProperties(const QList<QString> localFilePaths, const QString &startupId)
{
    Q_UNUSED(localFilePaths);
    Q_UNUSED(startupId);
    return false;
}

bool DDesktopServices::showFileItemPropertie(QUrl url, const QString &startupId)
{
    Q_UNUSED(url);
    Q_UNUSED(startupId);
    return false;
}

bool DDesktopServices::showFileItemProperties(const QList<QUrl> urls, const QString &startupId)
{
    Q_UNUSED(urls);
    Q_UNUSED(startupId);
    return false;
}

bool DDesktopServices::showFileItem(QString localFilePath, const QString &startupId)
{
    Q_UNUSED(localFilePath);
    Q_UNUSED(startupId);
    return false;
}

bool DDesktopServices::showFileItems(const QList<QString> localFilePaths, const QString &startupId)
{
    Q_UNUSED(localFilePaths);
    Q_UNUSED(startupId);
    return false;
}

bool DDesktopServices::showFileItem(QUrl url, const QString &startupId)
{
    Q_UNUSED(url);
    Q_UNUSED(startupId);
    return false;
}

bool DDesktopServices::showFileItems(const QList<QUrl> urls, const QString &startupId)
{
    Q_UNUSED(urls);
    Q_UNUSED(startupId);
    return false;
}

bool DDesktopServices::trash(QString localFilePath)
{
    Q_UNUSED(localFilePath);
    return false;
}

bool DDesktopServices::trash(const QList<QString> localFilePaths)
{
    Q_UNUSED(localFilePaths);
    return false;
}

bool DDesktopServices::trash(QUrl url)
{
    Q_UNUSED(url);
    return false;
}

bool DDesktopServices::trash(const QList<QUrl> urls)
{
    Q_UNUSED(urls);
    return false;
}


QString DDesktopServices::errorMessage()
{
    return QString();
}

DWIDGET_END_NAMESPACE
