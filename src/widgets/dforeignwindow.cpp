/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#include "dforeignwindow.h"
#include <DObjectPrivate>

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

class DForeignWindowPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
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
