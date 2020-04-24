/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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
