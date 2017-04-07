/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
#ifndef DFILESYSTEMWATCHER_WIN_P_H
#define DFILESYSTEMWATCHER_WIN_P_H

#include "dfilesystemwatcher.h"
#include "private/dobject_p.h"

DUTIL_BEGIN_NAMESPACE

class DFileSystemWatcherPrivate : public DObjectPrivate
{
    Q_DECLARE_PUBLIC(DFileSystemWatcher)

public:
    DFileSystemWatcherPrivate(int fd, DFileSystemWatcher *qq);
    ~DFileSystemWatcherPrivate();
};

DUTIL_END_NAMESPACE

#endif // DFILESYSTEMWATCHER_WIN_P_H
