/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DAPPLICATION_P_H
#define DAPPLICATION_P_H

#include <DObjectPrivate>

#include "dapplication.h"
#include "dpathbuf.h"

#include <QPixmap>

class QLocalServer;
class QTranslator;

DWIDGET_BEGIN_NAMESPACE

class DAboutDialog;

class DApplicationPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    D_DECLARE_PUBLIC(DApplication)

public:
    explicit DApplicationPrivate(DApplication *q);
    ~DApplicationPrivate();

    QString theme() const;
    void setTheme(const QString &theme);

    bool setSingleInstance(const QString &key);

    bool loadDtkTranslator(QList<QLocale> localeFallback);
    bool loadTranslator(QList<Dtk::Core::DPathBuf> translateDirs,
                        const QString &name,
                        QList<QLocale> localeFallback);

public:
// int m_pidLockFD = 0;
    QLocalServer *m_localServer = nullptr;
#ifdef Q_OS_UNIX
    QStringList m_monitoredStartupApps;
#endif

    QString productName;
    QPixmap productIcon;
    QString appLicense;
    QString appDescription;
    QString homePage;
    QString acknowledgementPage;

    DAboutDialog *aboutDialog = Q_NULLPTR;
};

DWIDGET_END_NAMESPACE

#endif // DAPPLICATION_P_H
