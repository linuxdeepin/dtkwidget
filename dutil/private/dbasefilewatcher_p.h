/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DBASEFILEWATCHER_P_H
#define DBASEFILEWATCHER_P_H

#include "dbasefilewatcher.h"
#include "private/dobject_p.h"

#include <QUrl>

DUTIL_BEGIN_NAMESPACE

class DBaseFileWatcherPrivate : public DObjectPrivate
{
public:
    DBaseFileWatcherPrivate(DBaseFileWatcher *qq);

    virtual bool start() = 0;
    virtual bool stop() = 0;

    QUrl url;
    bool started = false;
    static QList<DBaseFileWatcher*> watcherList;

    D_DECLARE_PUBLIC(DBaseFileWatcher)
};

DUTIL_END_NAMESPACE

#endif // DBASEFILEWATCHER_P_H
