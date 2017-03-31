/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "ddesktopservices.h"

#include <QDBusInterface>
#include <QDBusPendingCall>
#include <QDebug>

DUTIL_BEGIN_NAMESPACE

#define EASY_CALL_DBUS(name)\
    QDBusInterface *interface = fileManager1DBusInterface();\
    return interface && interface->call(#name, urls2uris(urls), startupId).type() != QDBusMessage::ErrorMessage;

static QDBusInterface *fileManager1DBusInterface()
{
    static QDBusInterface interface(QStringLiteral("org.freedesktop.FileManager1"),
                                    QStringLiteral("/org/freedesktop/FileManager1"));

    return &interface;
}

static QStringList urls2uris(const QList<QUrl> &urls)
{
    QStringList list;

    list.reserve(urls.size());

    for (const QUrl url : urls)
        list << url.toString();

    return list;
}

static QList<QUrl> path2urls(const QList<QString> &paths)
{
    QList<QUrl> list;

    list.reserve(paths.size());

    for (const QString &path : paths)
        list << QUrl::fromLocalFile(path);

    return list;
}

bool DDesktopServices::showFolder(QString localFilePath, const QString &startupId)
{
    return showFolder(QUrl::fromLocalFile(localFilePath), startupId);
}

bool DDesktopServices::showFolders(const QList<QString> localFilePaths, const QString &startupId)
{
    return showFolders(path2urls(localFilePaths), startupId);
}

bool DDesktopServices::showFolder(QUrl url, const QString &startupId)
{
    return showFolders(QList<QUrl>() << url, startupId);
}

bool DDesktopServices::showFolders(const QList<QUrl> urls, const QString &startupId)
{
    EASY_CALL_DBUS(ShowFolders)
}

bool DDesktopServices::showFileItemPropertie(QString localFilePath, const QString &startupId)
{
    return showFileItemPropertie(QUrl::fromLocalFile(localFilePath), startupId);
}

bool DDesktopServices::showFileItemProperties(const QList<QString> localFilePaths, const QString &startupId)
{
    return showFileItemProperties(path2urls(localFilePaths), startupId);
}

bool DDesktopServices::showFileItemPropertie(QUrl url, const QString &startupId)
{
    return showFileItemProperties(QList<QUrl>() << url, startupId);
}

bool DDesktopServices::showFileItemProperties(const QList<QUrl> urls, const QString &startupId)
{
    EASY_CALL_DBUS(ShowItemProperties)
}

bool DDesktopServices::showFileItem(QString localFilePath, const QString &startupId)
{
    return showFileItem(QUrl::fromLocalFile(localFilePath), startupId);
}

bool DDesktopServices::showFileItems(const QList<QString> localFilePaths, const QString &startupId)
{
    return showFileItems(path2urls(localFilePaths), startupId);
}

bool DDesktopServices::showFileItem(QUrl url, const QString &startupId)
{
    return showFileItems(QList<QUrl>() << url, startupId);
}

bool DDesktopServices::showFileItems(const QList<QUrl> urls, const QString &startupId)
{
    EASY_CALL_DBUS(ShowItems)
}

QString DDesktopServices::errorMessage()
{
    return fileManager1DBusInterface()->lastError().message();
}

DUTIL_END_NAMESPACE
