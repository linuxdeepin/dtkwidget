/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
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
#include "dwindowgroupleader.h"

#include <QWidget>
#include <QWindow>
#include <QGuiApplication>
#include <QPointer>

DWIDGET_BEGIN_NAMESPACE

#define DEFINE_CONST_CHAR(Name) const char _##Name[] = "_d_" #Name

DEFINE_CONST_CHAR(groupLeader);
DEFINE_CONST_CHAR(createGroupWindow);
DEFINE_CONST_CHAR(destoryGroupWindow);
DEFINE_CONST_CHAR(setWindowGroup);
DEFINE_CONST_CHAR(clientLeader);

class DWindowGroupLeaderPrivate
{
public:
    DWindowGroupLeaderPrivate(quint32 groupLeader)
        : groupLeader(groupLeader) {
        QFunctionPointer clientLeaderFun = qApp->platformFunction(_clientLeader);

        if (clientLeaderFun) {
            clientLeader = reinterpret_cast<quint32(*)()>(clientLeaderFun)();
        }
    }

    quint32 groupLeader;
    quint32 clientLeader = 0;
    bool groupLeaderFromUser = false;

    QList<QPointer<QWindow>> windowList;

    void ensureGroupLeader();
    bool setWindowGroupLeader(quint32 window, quint32 groupLeader);
};

void DWindowGroupLeaderPrivate::ensureGroupLeader()
{
    if (groupLeader != 0)
        return;

    QFunctionPointer createGroupWindow = qApp->platformFunction(_createGroupWindow);

    if (!createGroupWindow)
        return;

    groupLeader = reinterpret_cast<quint32(*)()>(createGroupWindow)();
}

bool DWindowGroupLeaderPrivate::setWindowGroupLeader(quint32 window, quint32 groupLeader)
{
    QFunctionPointer setWindowGroup = qApp->platformFunction(_setWindowGroup);

    if (!setWindowGroup) {
        return false;
    }

    reinterpret_cast<void(*)(quint32, quint32)>(setWindowGroup)(window, groupLeader);

    return true;
}

DWindowGroupLeader::DWindowGroupLeader(quint32 groupId)
    : d_ptr(new DWindowGroupLeaderPrivate(groupId))
{
    if (groupId != 0)
        d_ptr->groupLeaderFromUser = true;
}

DWindowGroupLeader::~DWindowGroupLeader()
{
    Q_D(DWindowGroupLeader);

    for (auto window : d->windowList)
        removeWindow(window);

    if (!d->groupLeaderFromUser) {
        QFunctionPointer destoryGroupWindow = qApp->platformFunction(_destoryGroupWindow);

        if (!destoryGroupWindow)
            return;

        reinterpret_cast<void(*)(quint32)>(destoryGroupWindow)(d->groupLeader);
    }
}

void DWindowGroupLeader::addWindow(QWindow *window)
{
    Q_ASSERT(window);
    Q_D(DWindowGroupLeader);

    d->ensureGroupLeader();

    window->setProperty(_groupLeader, d->groupLeader);

    if (window->handle()) {
        d->setWindowGroupLeader(window->winId(), d->groupLeader);
    }

    d->windowList << window;
}

void DWindowGroupLeader::removeWindow(QWindow *window)
{
    if (!window)
        return;

    window->setProperty(_groupLeader, QVariant());

    Q_D(DWindowGroupLeader);

    if (window->handle()) {
        d->setWindowGroupLeader(window->winId(), d->clientLeader);
    }
}

void DWindowGroupLeader::addWindow(QWidget *window)
{
    Q_ASSERT(window);
    Q_ASSERT(window->isWindow());

    if (!window->windowHandle()) {
        window->setAttribute(Qt::WA_NativeWindow);
    }

    return addWindow(window->windowHandle());
}

void DWindowGroupLeader::removeWindow(QWidget *window)
{
    return removeWindow(window->windowHandle());
}

DWIDGET_END_NAMESPACE
