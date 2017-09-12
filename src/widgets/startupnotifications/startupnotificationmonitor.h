/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
