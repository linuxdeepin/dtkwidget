/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dbasefilewatcher.h"
#include "private/dbasefilewatcher_p.h"

#include <QEvent>
#include <QDebug>

DUTIL_BEGIN_NAMESPACE

QList<DBaseFileWatcher*> DBaseFileWatcherPrivate::watcherList;
DBaseFileWatcherPrivate::DBaseFileWatcherPrivate(DBaseFileWatcher *qq)
    : DObjectPrivate(qq)
{

}

DBaseFileWatcher::~DBaseFileWatcher()
{
    stopWatcher();
    DBaseFileWatcherPrivate::watcherList.removeOne(this);
}

QUrl DBaseFileWatcher::fileUrl() const
{
    Q_D(const DBaseFileWatcher);

    return d->url;
}

bool DBaseFileWatcher::startWatcher()
{
    Q_D(DBaseFileWatcher);

    if (d->started)
        return true;

    if (d->start()) {
        d->started = true;

        return true;
    }

    return false;
}

bool DBaseFileWatcher::stopWatcher()
{
    Q_D(DBaseFileWatcher);

    if (!d->started)
        return false;

    if (d->stop()) {
        d->started = false;

        return true;
    }

    return false;
}

bool DBaseFileWatcher::restartWatcher()
{
    bool ok = stopWatcher();
    return ok && startWatcher();
}

void DBaseFileWatcher::setEnabledSubfileWatcher(const QUrl &subfileUrl, bool enabled)
{
    Q_UNUSED(subfileUrl)
    Q_UNUSED(enabled)
}

bool DBaseFileWatcher::ghostSignal(const QUrl &targetUrl, DBaseFileWatcher::SignalType1 signal, const QUrl &arg1)
{
    if (!signal)
        return false;

    bool ok = false;

    for (DBaseFileWatcher *watcher : DBaseFileWatcherPrivate::watcherList) {
        if (watcher->fileUrl() == targetUrl) {
            ok = true;
            (watcher->*signal)(arg1);
        }
    }

    return ok;
}

bool DBaseFileWatcher::ghostSignal(const QUrl &targetUrl, DBaseFileWatcher::SignalType2 signal, const QUrl &arg1, const QUrl &arg2)
{
    if (!signal)
        return false;

    bool ok = false;

    for (DBaseFileWatcher *watcher : DBaseFileWatcherPrivate::watcherList) {
        if (watcher->fileUrl() == targetUrl) {
            ok = true;
            (watcher->*signal)(arg1, arg2);
        }
    }

    return ok;
}

DBaseFileWatcher::DBaseFileWatcher(DBaseFileWatcherPrivate &dd,
                                           const QUrl &url, QObject *parent)
    : QObject(parent)
    , DObject(dd)
{
    Q_ASSERT(url.isValid());

    d_func()->url = url;
    DBaseFileWatcherPrivate::watcherList << this;
}

DUTIL_END_NAMESPACE

#include "moc_dbasefilewatcher.cpp"
