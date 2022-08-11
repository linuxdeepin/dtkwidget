// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dmprismonitor.h"

#include <QDebug>

DMPRISMonitor::DMPRISMonitor(QObject *parent)
    : QObject(parent),

      m_dbusInter(new DBusInterface("org.freedesktop.DBus", "/org/freedesktop/DBus", QDBusConnection::sessionBus(), this))
{
}

void DMPRISMonitor::init()
{
    connect(m_dbusInter, &DBusInterface::NameOwnerChanged, this, &DMPRISMonitor::onNameOwnerChanged);

    for (const auto &name : m_dbusInter->ListNames().value())
        onNameOwnerChanged(name, QString(), name);
}

void DMPRISMonitor::onNameOwnerChanged(const QString &name, const QString &oldOwner, const QString &newOwner)
{
    Q_UNUSED(oldOwner);

    if (!name.startsWith("org.mpris.MediaPlayer2."))
        return;

    if (newOwner.isEmpty())
        Q_EMIT mprisLost(name);
    else
        Q_EMIT mprisAcquired(name);
}
