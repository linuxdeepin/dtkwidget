/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

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
    return false;
}

bool DDesktopServices::showFolders(const QList<QString> localFilePaths, const QString &startupId)
{
    return false;
}

bool DDesktopServices::showFolder(QUrl url, const QString &startupId)
{
    return false;
}

bool DDesktopServices::showFolders(const QList<QUrl> urls, const QString &startupId)
{
    return false;
}

bool DDesktopServices::showFileItemPropertie(QString localFilePath, const QString &startupId)
{
    return false;
}

bool DDesktopServices::showFileItemProperties(const QList<QString> localFilePaths, const QString &startupId)
{
    return false;
}

bool DDesktopServices::showFileItemPropertie(QUrl url, const QString &startupId)
{
    return false;
}

bool DDesktopServices::showFileItemProperties(const QList<QUrl> urls, const QString &startupId)
{
    return false;
}

bool DDesktopServices::showFileItem(QString localFilePath, const QString &startupId)
{
    return false;
}

bool DDesktopServices::showFileItems(const QList<QString> localFilePaths, const QString &startupId)
{
    return false;
}

bool DDesktopServices::showFileItem(QUrl url, const QString &startupId)
{
    return false;
}

bool DDesktopServices::showFileItems(const QList<QUrl> urls, const QString &startupId)
{
    return false;
}

QString DDesktopServices::errorMessage()
{
    return QString();
}

DWIDGET_END_NAMESPACE
