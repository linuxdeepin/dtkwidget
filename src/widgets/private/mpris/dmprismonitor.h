/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

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
