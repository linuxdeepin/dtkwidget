/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DFILESYSTEMWATCHER_H
#define DFILESYSTEMWATCHER_H

#include "dutil_global.h"
#include "dobject.h"

#include <QObject>

DUTIL_BEGIN_NAMESPACE

class DFileSystemWatcherPrivate;
class DFileSystemWatcher : public QObject, public DObject
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DFileSystemWatcher)

public:
    DFileSystemWatcher(QObject *parent = Q_NULLPTR);
    DFileSystemWatcher(const QStringList &paths, QObject *parent = Q_NULLPTR);
    ~DFileSystemWatcher();

    bool addPath(const QString &file);
    QStringList addPaths(const QStringList &files);
    bool removePath(const QString &file);
    QStringList removePaths(const QStringList &files);

    QStringList files() const;
    QStringList directories() const;

Q_SIGNALS:
    void fileDeleted(const QString &path, const QString &name, QPrivateSignal);
    void fileAttributeChanged(const QString &path, const QString &name, QPrivateSignal);
    void fileClosed(const QString &path, const QString &name, QPrivateSignal);
    void fileMoved(const QString &fromPath, const QString &fromName,
                   const QString &toPath, const QString &toName, QPrivateSignal);
    void fileCreated(const QString &path, const QString &name, QPrivateSignal);
    void fileModified(const QString &path, const QString &name, QPrivateSignal);

private:
    Q_PRIVATE_SLOT(d_func(), void _q_readFromInotify())
};

DUTIL_END_NAMESPACE

#endif // DFILESYSTEMWATCHER_H
