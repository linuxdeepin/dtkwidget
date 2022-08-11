// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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

Q_SIGNALS:
    /*!
     * \brief appStartup is emitted when an application is starting up.
     * \a id is the startup id associated with the startup.
     */
    void appStartup(QString id);

    /*!
     * \brief appStartupCompleted is emitted when the start up process of an applciation is finished.
     * \a id is the startup id associated with the startup.
     */
    void appStartupCompleted(QString id);

protected:
    StartupNotificationMonitor();

    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
};

#endif // STARTUPNOTIFICATIONMONITOR_H
