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

signals:
    void mprisAcquired(const QString &path) const;
    void mprisLost(const QString &path) const;

private slots:
    void onNameOwnerChanged(const QString &name, const QString &oldOwner, const QString &newOwner);

private:
    DBusInterface *m_dbusInter;
};

#endif // DMPRISMONITOR_H
