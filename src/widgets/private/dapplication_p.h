/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef DAPPLICATION_P_H
#define DAPPLICATION_P_H

#include <DObjectPrivate>

#include "dapplication.h"
#include "dpathbuf.h"

#include <QIcon>

class QLocalServer;
class QTranslator;

DCORE_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

class DAboutDialog;

class DApplicationPrivate : public DObjectPrivate
{
    D_DECLARE_PUBLIC(DApplication)

public:
    explicit DApplicationPrivate(DApplication *q);
    ~DApplicationPrivate();

    QString theme() const;
    void setTheme(const QString &theme);

    bool setSingleInstanceBySemaphore(const QString &key);
#ifdef DTK_DBUS_SINGLEINSTANCE
    bool setSingleInstanceByDbus(const QString &key);
#endif

    bool loadDtkTranslator(QList<QLocale> localeFallback);
    bool loadTranslator(QList<DPathBuf> translateDirs, const QString &name, QList<QLocale> localeFallback);

    static bool isUserManualExists();
public:
// int m_pidLockFD = 0;
    QLocalServer *m_localServer = nullptr;
#ifdef Q_OS_UNIX
    QStringList m_monitoredStartupApps;
#endif

    QString productName;
    QIcon   productIcon;
    QString appLicense;
    QString appDescription;
    QString homePage;
    QString acknowledgementPage;

    bool visibleMenuShortcutText   = false;
    bool visibleMenuCheckboxWidget = false;
    bool visibleMenuIcon           = false;

    DAboutDialog *aboutDialog = Q_NULLPTR;
};

DWIDGET_END_NAMESPACE

#endif // DAPPLICATION_P_H
