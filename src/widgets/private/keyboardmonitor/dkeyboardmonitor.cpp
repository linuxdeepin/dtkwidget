/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "dkeyboardmonitor.h"

#include <QX11Info>
#include <QDebug>

#include <X11/X.h>
#include <X11/XKBlib.h>
#include <X11/extensions/XInput.h>
#include <X11/extensions/XInput2.h>

#include <stdio.h>
#include <stdlib.h>

DWIDGET_BEGIN_NAMESPACE

static int xi2_opcode;

int xinput_version(Display *display)
{
    XExtensionVersion *version;
    static int vers = -1;

    if (vers != -1)
        return vers;

    version = XGetExtensionVersion(display, INAME);

    if (version && (version != (XExtensionVersion*) NoSuchExtension)) {
        vers = version->major_version;
        XFree(version);
    }

    /* Announce our supported version so the server treats us correctly. */
    if (vers >= XI_2_Major)
    {
        const char *forced_version;
        int maj = 2, min = 2;

        forced_version = getenv("XINPUT_XI2_VERSION");
        if (forced_version) {
            if (sscanf(forced_version, "%d.%d", &maj, &min) != 2) {
                fprintf(stderr, "Invalid format of XINPUT_XI2_VERSION "
                                "environment variable. Need major.minor\n");
                exit(1);
            }
            printf("Overriding XI2 version to: %d.%d\n", maj, min);
        }

        XIQueryVersion(display, &maj, &min);
    }

    return vers;
}
void select_events(Display* display)
{
    XIEventMask m;
    m.deviceid = XIAllMasterDevices;
    m.mask_len = XIMaskLen(XI_LASTEVENT);
    m.mask = (unsigned char*)calloc(m.mask_len, sizeof(char));

    //    XISetMask(m.mask, XI_RawKeyPress);
    XISetMask(m.mask, XI_RawKeyRelease);
    //    XISetMask(m.mask, XI_RawButtonPress);
    //    XISetMask(m.mask, XI_RawButtonRelease);
    //    XISetMask(m.mask, XI_RawMotion);
    //    XISetMask(m.mask, XI_RawTouchBegin);
    //    XISetMask(m.mask, XI_RawTouchUpdate);
    //    XISetMask(m.mask, XI_RawTouchEnd);

    XISelectEvents(display, DefaultRootWindow(display), &m, 1);

    free(m.mask);
    XSync(display, False);
}

int DKeyboardMonitor::listen(Display *display)
{
    Window root = DefaultRootWindow(display);
    int root_x, root_y, nouse;
    Window noused_window;
    unsigned int mask;

    while(1)
    {
        XEvent ev;
        XGenericEventCookie *cookie = (XGenericEventCookie*)&ev.xcookie;
        XNextEvent(display, (XEvent*)&ev);

        if (XGetEventData(display, cookie) &&
                cookie->type == GenericEvent &&
                cookie->extension == xi2_opcode)
        {
            XQueryPointer(display, root, &noused_window, &noused_window, &root_x, &root_y, &nouse, &nouse, &mask);
            XIRawEvent* event = static_cast<XIRawEvent*>(cookie->data);
            switch (cookie->evtype)
            {
            case XI_RawButtonPress:
                /*printf("Button press: Detail(%d), X(%d), Y(%d), Mask(%u)\n", event->detail, root_x, root_y, mask);*/
                break;
            case XI_RawButtonRelease:
                /*printf("Button release: Detail(%d), X(%d), Y(%d), Mask(%u)\n", event->detail, root_x, root_y, mask);*/
                break;
            case XI_RawKeyPress:
                break;
            case XI_RawKeyRelease:
                if (event->detail == 66) { // check if the key pressed is capslock first.
                    Q_EMIT capslockStatusChanged(isCapslockOn());
                } else if (event->detail == 77) {
                    Q_EMIT numlockStatusChanged(isNumlockOn());
                }
                /*printf("Key release: Detail(%d), X(%d), Y(%d), Mask(%u)\n", event->detail, root_x, root_y, mask);*/
                break;
            case XI_RawTouchBegin:
                //                printf("Touch begin: Detail(%d), X(%d), Y(%d), Mask(%u)\n", event->detail, root_x, root_y, mask);
                break;
            case XI_RawTouchUpdate:
                //                printf("Touch update: Detail(%d), X(%d), Y(%d), Mask(%u)\n", event->detail, root_x, root_y, mask);
                break;
            case XI_RawTouchEnd:
                //                printf("Touch end: Detail(%d), X(%d), Y(%d), Mask(%u)\n", event->detail, root_x, root_y, mask);
                break;
            }
        }

        XFreeEventData(display, cookie);
    }
    return EXIT_SUCCESS;
}

DKeyboardMonitor::DKeyboardMonitor() :
    QThread()
{

}

DKeyboardMonitor *DKeyboardMonitor::instance()
{
    static DKeyboardMonitor *KeyboardMonitorInstance = nullptr;

    if (!KeyboardMonitorInstance) {
        KeyboardMonitorInstance = new DKeyboardMonitor;
    }

    return KeyboardMonitorInstance;
}

bool DKeyboardMonitor::isCapslockOn()
{
    bool result;
    unsigned int n = 0;
    static Display* d = QX11Info::display();

    XkbGetIndicatorState(d, XkbUseCoreKbd, &n);
    result = (n & 0x01) != 0;

    return result;
}

bool DKeyboardMonitor::isNumlockOn()
{
    bool result;
    unsigned int n = 0;
    static Display* d = QX11Info::display();

    XkbGetIndicatorState(d, XkbUseCoreKbd, &n);
    result = (n & 0x02) != 0;

    return result;
}

bool DKeyboardMonitor::setNumlockStatus(const bool &on)
{
    Display* d = QX11Info::display();

    bool result = XkbLockModifiers(d, XkbUseCoreKbd, Mod2Mask, on ? Mod2Mask : 0);
    XFlush(d);

    return result;
}

void DKeyboardMonitor::run()
{
    Display* display = XOpenDisplay(NULL);
    int event, error;

    if (!XQueryExtension(display, "XInputExtension", &xi2_opcode, &event, &error)) {
        fprintf(stderr, "XInput2 not available.\n");
        return;
    }

    if (!xinput_version(display)) {
        fprintf(stderr, "XInput2 extension not available\n");
        return;
    }

    select_events(display);
    listen(display);
}

DWIDGET_END_NAMESPACE
