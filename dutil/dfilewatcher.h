/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DFILEWATCHER_H
#define DFILEWATCHER_H

#include "dbasefilewatcher.h"

DUTIL_BEGIN_NAMESPACE

class DFileWatcherPrivate;
class DFileWatcher : public DBaseFileWatcher
{
    Q_OBJECT

public:
    explicit DFileWatcher(const QString &filePath, QObject *parent = 0);

private slots:
    void onFileDeleted(const QString &path, const QString &name);
    void onFileAttributeChanged(const QString &path, const QString &name);
    void onFileMoved(const QString &fromPath, const QString &fromName,
                   const QString &toPath, const QString &toName);
    void onFileCreated(const QString &path, const QString &name);
    void onFileModified(const QString &path, const QString &name);
    void onFileClosed(const QString &path, const QString &name);

private:
    D_DECLARE_PRIVATE(DFileWatcher)
};

DUTIL_END_NAMESPACE

#endif // DFILEWATCHER_H
