/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DBASEFILEWATCHER_H
#define DBASEFILEWATCHER_H

#include "dutil_global.h"
#include "dobject.h"

#include <QObject>

DUTIL_BEGIN_NAMESPACE

class DBaseFileWatcherPrivate;
class DBaseFileWatcher : public QObject, public DObject
{
    Q_OBJECT

public:
    ~DBaseFileWatcher();

    QUrl fileUrl() const;

    bool startWatcher();
    bool stopWatcher();
    bool restartWatcher();

    virtual void setEnabledSubfileWatcher(const QUrl &subfileUrl, bool enabled = true);

    using SignalType1 = void(DBaseFileWatcher::*)(const QUrl&);
    using SignalType2 = void(DBaseFileWatcher::*)(const QUrl&, const QUrl&);
    static bool ghostSignal(const QUrl &targetUrl, SignalType1 signal, const QUrl &arg1);
    static bool ghostSignal(const QUrl &targetUrl, SignalType2 signal, const QUrl &arg1, const QUrl &arg2);

signals:
    void fileDeleted(const QUrl &url);
    void fileAttributeChanged(const QUrl &url);
    void fileMoved(const QUrl &fromUrl, const QUrl &toUrl);
    void subfileCreated(const QUrl &url);
    void fileModified(const QUrl &url);
    void fileClosed(const QUrl &url);

protected:
    explicit DBaseFileWatcher(DBaseFileWatcherPrivate &dd, const QUrl &url, QObject *parent = 0);

private:
    Q_DISABLE_COPY(DBaseFileWatcher)
    D_DECLARE_PRIVATE(DBaseFileWatcher)
};

DUTIL_END_NAMESPACE

#endif // DBASEFILEWATCHER_H
