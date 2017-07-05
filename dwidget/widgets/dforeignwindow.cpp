/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
#include "dforeignwindow.h"
#include "private/dobject_p.h"

#include <private/qguiapplication_p.h>
#include <qpa/qplatformintegration.h>

#include <QEvent>
#include <QDynamicPropertyChangeEvent>
#include <QDebug>

DWIDGET_BEGIN_NAMESPACE

#define DEFINE_CONST_CHAR(Name) const char _##Name[] = "_d_" #Name

// propertys
DEFINE_CONST_CHAR(WmClass);
DEFINE_CONST_CHAR(ProcessId);

class DForeignWindowPrivate : public DObjectPrivate
{
public:
    DForeignWindowPrivate(DForeignWindow *qq)
        : DObjectPrivate(qq) {}
};

DForeignWindow::DForeignWindow(QWindow *parent)
    : QWindow(parent)
    , DObject(*new DForeignWindowPrivate(this))
{

}

DForeignWindow *DForeignWindow::fromWinId(WId id)
{
    if (!QGuiApplicationPrivate::platformIntegration()->hasCapability(QPlatformIntegration::ForeignWindows)) {
        qWarning() << "DForeignWindow::fromWinId(): platform plugin does not support foreign windows.";
        return 0;
    }

    DForeignWindow *window = new DForeignWindow;
    window->setFlags(Qt::ForeignWindow);
    window->setProperty("_q_foreignWinId", QVariant::fromValue(id));
    window->create();
    return window;
}

QString DForeignWindow::wmClass() const
{
    return property(_WmClass).toString();
}

quint32 DForeignWindow::pid() const
{
    return qvariant_cast<quint32>(property(_ProcessId));
}

bool DForeignWindow::event(QEvent *e)
{
    if (e->type() == QEvent::DynamicPropertyChange) {
        QDynamicPropertyChangeEvent *event = static_cast<QDynamicPropertyChangeEvent*>(e);

        if (event->propertyName() == _WmClass) {
            Q_EMIT wmClassChanged();

            return true;
        } else if (event->propertyName() == _ProcessId) {
            Q_EMIT pidChanged();

            return true;
        }
    }

    return false;
}

DWIDGET_END_NAMESPACE
