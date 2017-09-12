/*
 * Copyright (C) 2016 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef DBUSINTERFACE_H
#define DBUSINTERFACE_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface org.freedesktop.DBus
 */
class DBusInterface: public QDBusAbstractInterface
{
    Q_OBJECT

    Q_SLOT void __propertyChanged__(const QDBusMessage& msg)
    {
        QList<QVariant> arguments = msg.arguments();
        if (3 != arguments.count())
            return;
        QString interfaceName = msg.arguments().at(0).toString();
        if (interfaceName != "org.freedesktop.DBus")
            return;
        QVariantMap changedProps = qdbus_cast<QVariantMap>(arguments.at(1).value<QDBusArgument>());
        Q_FOREACH(const QString &prop, changedProps.keys()) {
            const QMetaObject* self = metaObject();
            for (int i=self->propertyOffset(); i < self->propertyCount(); ++i) {
                QMetaProperty p = self->property(i);
                QGenericArgument value(QMetaType::typeName(p.type()), const_cast<void*>(changedProps[prop].constData()));
                if (p.name() == prop) {
                    Q_EMIT p.notifySignal().invoke(this, value);
                }
            }
        }
   }
public:
    static inline const char *staticInterfaceName()
    { return "org.freedesktop.DBus"; }

public:
    DBusInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~DBusInterface();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> AddMatch(const QString &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QStringLiteral("AddMatch"), argumentList);
    }

    inline QDBusPendingReply<QByteArray> GetAdtAuditSessionData(const QString &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QStringLiteral("GetAdtAuditSessionData"), argumentList);
    }

    inline QDBusPendingReply<QByteArray> GetConnectionSELinuxSecurityContext(const QString &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QStringLiteral("GetConnectionSELinuxSecurityContext"), argumentList);
    }

    inline QDBusPendingReply<uint> GetConnectionUnixProcessID(const QString &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QStringLiteral("GetConnectionUnixProcessID"), argumentList);
    }

    inline QDBusPendingReply<uint> GetConnectionUnixUser(const QString &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QStringLiteral("GetConnectionUnixUser"), argumentList);
    }

    inline QDBusPendingReply<QString> GetId()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("GetId"), argumentList);
    }

    inline QDBusPendingReply<QString> GetNameOwner(const QString &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QStringLiteral("GetNameOwner"), argumentList);
    }

    inline QDBusPendingReply<QString> Hello()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("Hello"), argumentList);
    }

    inline QDBusPendingReply<QStringList> ListActivatableNames()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("ListActivatableNames"), argumentList);
    }

    inline QDBusPendingReply<QStringList> ListNames()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("ListNames"), argumentList);
    }

    inline QDBusPendingReply<QStringList> ListQueuedOwners(const QString &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QStringLiteral("ListQueuedOwners"), argumentList);
    }

    inline QDBusPendingReply<bool> NameHasOwner(const QString &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QStringLiteral("NameHasOwner"), argumentList);
    }

    inline QDBusPendingReply<uint> ReleaseName(const QString &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QStringLiteral("ReleaseName"), argumentList);
    }

    inline QDBusPendingReply<> ReloadConfig()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("ReloadConfig"), argumentList);
    }

    inline QDBusPendingReply<> RemoveMatch(const QString &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QStringLiteral("RemoveMatch"), argumentList);
    }

    inline QDBusPendingReply<uint> RequestName(const QString &in0, uint in1)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0) << QVariant::fromValue(in1);
        return asyncCallWithArgumentList(QStringLiteral("RequestName"), argumentList);
    }

    inline QDBusPendingReply<uint> StartServiceByName(const QString &in0, uint in1)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0) << QVariant::fromValue(in1);
        return asyncCallWithArgumentList(QStringLiteral("StartServiceByName"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void NameAcquired(const QString &in0);
    void NameLost(const QString &in0);
    void NameOwnerChanged(const QString &in0, const QString &in1, const QString &in2);
// begin property changed signals
};

namespace org {
  namespace freedesktop {
    typedef ::DBusInterface DBus;
  }
}
#endif
