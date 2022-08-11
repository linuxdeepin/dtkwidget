// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DMPRISMONITOR_H
#define DMPRISMONITOR_H

#include "dbusinterface.h"

#include <QObject>

class DMPRISMonitor : public QObject
{
    Q_OBJECT

public:
    explicit DMPRISMonitor(QObject *parent = 0);

    void init();

Q_SIGNALS:
    void mprisAcquired(const QString &path) const;
    void mprisLost(const QString &path) const;

private Q_SLOTS:
    void onNameOwnerChanged(const QString &name, const QString &oldOwner, const QString &newOwner);

private:
    DBusInterface *m_dbusInter;
};

#endif // DMPRISMONITOR_H
