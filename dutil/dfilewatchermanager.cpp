/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dfilewatchermanager.h"
#include "dfilewatcher.h"
#include "private/dobject_p.h"

#include <QMap>
#include <QUrl>

DUTIL_BEGIN_NAMESPACE

class DFileWatcherManagerPrivate : public DObjectPrivate
{
public:
    DFileWatcherManagerPrivate(DFileWatcherManager *qq);

    QMap<QString, DFileWatcher*> watchersMap;

    D_DECLARE_PUBLIC(DFileWatcherManager)
};

DFileWatcherManagerPrivate::DFileWatcherManagerPrivate(DFileWatcherManager *qq)
    : DObjectPrivate(qq)
{

}

DFileWatcherManager::DFileWatcherManager(QObject *parent)
    : QObject(parent)
    , DObject(*new DFileWatcherManagerPrivate(this))
{

}

DFileWatcherManager::~DFileWatcherManager()
{

}

DFileWatcher *DFileWatcherManager::add(const QString &filePath)
{
    Q_D(DFileWatcherManager);

    DFileWatcher *watcher = d->watchersMap.value(filePath);

    if (watcher)
        return watcher;

    watcher = new DFileWatcher(filePath, this);

    connect(watcher, &DFileWatcher::fileAttributeChanged, this, [this](const QUrl &url) {
        emit fileAttributeChanged(url.toLocalFile());
    });
    connect(watcher, &DFileWatcher::fileClosed, this, [this](const QUrl &url) {
        emit fileClosed(url.toLocalFile());
    });
    connect(watcher, &DFileWatcher::fileDeleted, this, [this](const QUrl &url) {
        emit fileDeleted(url.toLocalFile());
    });
    connect(watcher, &DFileWatcher::fileModified, this, [this](const QUrl &url) {
        emit fileModified(url.toLocalFile());
    });
    connect(watcher, &DFileWatcher::fileMoved, this, [this](const QUrl &fromUrl, const QUrl &toUrl) {
        emit fileMoved(fromUrl.toLocalFile(), toUrl.toLocalFile());
    });
    connect(watcher, &DFileWatcher::subfileCreated, this, [this](const QUrl &url) {
        emit subfileCreated(url.toLocalFile());
    });

    d->watchersMap[filePath] = watcher;
    watcher->startWatcher();

    return watcher;
}

void DFileWatcherManager::remove(const QString &filePath)
{
    Q_D(DFileWatcherManager);

    DFileWatcher *watcher = d->watchersMap.take(filePath);

    if (watcher)
        watcher->deleteLater();
}

DUTIL_END_NAMESPACE
