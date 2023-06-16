// SPDX-FileCopyrightText: 2011 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef KEYBOARDMONITOR_H
#define KEYBOARDMONITOR_H

#include <QThread>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QX11Info>
#else
#include <QtGui/private/qtx11extras_p.h>
#endif
#include <dtkwidget_global.h>

DWIDGET_BEGIN_NAMESPACE

class DKeyboardMonitor : public QThread
{
    Q_OBJECT

public:
    static DKeyboardMonitor *instance();

    bool isCapslockOn();
    bool isNumlockOn();
    bool setNumlockStatus(const bool &on);

Q_SIGNALS:
    void capslockStatusChanged(bool on);
    void numlockStatusChanged(bool on);

protected:
    void run() Q_DECL_OVERRIDE;

private:
    DKeyboardMonitor();

    int listen(Display *display);
};

DWIDGET_END_NAMESPACE

#endif // KEYBOARDMONITOR_H
