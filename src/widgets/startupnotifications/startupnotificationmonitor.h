/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef STARTUPNOTIFICATIONMONITOR_H
#define STARTUPNOTIFICATIONMONITOR_H

#include <QObject>
#include <QAbstractNativeEventFilter>

class StartupNotificationMonitor : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    static StartupNotificationMonitor* instance();

    ~StartupNotificationMonitor();
    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);

Q_SIGNALS:
    void appStartup(QString id);
    void appStartupCompleted(QString id);

protected:
    StartupNotificationMonitor();
};

#endif // STARTUPNOTIFICATIONMONITOR_H
