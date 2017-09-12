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

#include <QtWidgets>
#include <QX11Info>

#include <libsn/sn-monitor.h>
#include <xcb/xcb_aux.h>

#include "startupnotificationmonitor.h"

class StartupNotificationMonitorSub : public StartupNotificationMonitor
{

};

Q_GLOBAL_STATIC(StartupNotificationMonitorSub, StartupNotificationMonitorInstance)

static SnDisplay *display;
static SnMonitorContext *context;

static void monitor_event_func (SnMonitorEvent *event, void *user_data)
{
    SnMonitorContext *context;
    SnStartupSequence *sequence;

    context = sn_monitor_event_get_context (event);
    sequence = sn_monitor_event_get_startup_sequence (event);

    const char* app_id = sn_startup_sequence_get_id(sequence);

    StartupNotificationMonitor *monitor = static_cast<StartupNotificationMonitor*>(user_data);

    switch (sn_monitor_event_get_type (event))
    {
    case SN_MONITOR_EVENT_INITIATED:
        qDebug() << "startup notification initiated: " << app_id;
        Q_EMIT monitor->appStartup(app_id);
        break;

    case SN_MONITOR_EVENT_CHANGED:
        break;

    case SN_MONITOR_EVENT_COMPLETED:
        qDebug() << "startup notification completed: " << app_id;
        Q_EMIT monitor->appStartupCompleted(app_id);
        break;

    case SN_MONITOR_EVENT_CANCELED:
        qDebug() << "startup notification canceled: " << app_id;
        break;
    }
}

StartupNotificationMonitor::StartupNotificationMonitor() :
    QObject()
{
    int screen = 0;

    xcb_screen_t *s = xcb_aux_get_screen (QX11Info::connection(), screen);
    const uint32_t select_input_val[] = { XCB_EVENT_MASK_PROPERTY_CHANGE };
    xcb_change_window_attributes (QX11Info::connection(), s->root, XCB_CW_EVENT_MASK,
                                  select_input_val);

    display = sn_xcb_display_new (QX11Info::connection(), NULL, NULL);

    context = sn_monitor_context_new (display, screen,
                                      monitor_event_func,
                                      this, NULL);

    qApp->installNativeEventFilter(this);
}

StartupNotificationMonitor* StartupNotificationMonitor::StartupNotificationMonitor::instance()
{
    return StartupNotificationMonitorInstance;
}

StartupNotificationMonitor::~StartupNotificationMonitor()
{
    qApp->removeNativeEventFilter(this);
}

bool StartupNotificationMonitor::nativeEventFilter(const QByteArray &eventType, void *message, long *)
{
    if(Q_LIKELY(eventType == "xcb_generic_event_t")) {
        xcb_generic_event_t* event = static_cast<xcb_generic_event_t *>(message);
        sn_xcb_display_process_event (display, event);
    }

    return false;
}
