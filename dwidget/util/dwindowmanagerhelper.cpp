/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
#include "dwindowmanagerhelper.h"
#include "private/dobject_p.h"

#include <QGuiApplication>

#include <functional>

DWIDGET_BEGIN_NAMESPACE

#define DEFINE_CONST_CHAR(Name) const char _##Name[] = "_d_" #Name

// functions
DEFINE_CONST_CHAR(hasBlurWindow);
DEFINE_CONST_CHAR(hasComposite);
DEFINE_CONST_CHAR(connectWindowManagerChangedSignal);
DEFINE_CONST_CHAR(connectHasBlurWindowChanged);
DEFINE_CONST_CHAR(connectHasCompositeChanged);
DEFINE_CONST_CHAR(getCurrentWorkspaceWindows);
DEFINE_CONST_CHAR(connectWindowListChanged);

static bool connectWindowManagerChangedSignal(QObject *object, std::function<void ()> slot)
{
    QFunctionPointer connectWindowManagerChangedSignal = Q_NULLPTR;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    connectWindowManagerChangedSignal = qApp->platformFunction(_connectWindowManagerChangedSignal);
#endif

    return connectWindowManagerChangedSignal && reinterpret_cast<bool(*)(QObject *object, std::function<void ()>)>(connectWindowManagerChangedSignal)(object, slot);
}

static bool connectHasBlurWindowChanged(QObject *object, std::function<void ()> slot)
{
    QFunctionPointer connectHasBlurWindowChanged = Q_NULLPTR;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    connectHasBlurWindowChanged = qApp->platformFunction(_connectHasBlurWindowChanged);
#endif

    return connectHasBlurWindowChanged && reinterpret_cast<bool(*)(QObject *object, std::function<void ()>)>(connectHasBlurWindowChanged)(object, slot);
}

static bool connectHasCompositeChanged(QObject *object, std::function<void ()> slot)
{
    QFunctionPointer connectHasCompositeChanged = Q_NULLPTR;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    connectHasCompositeChanged = qApp->platformFunction(_connectHasCompositeChanged);
#endif

    return connectHasCompositeChanged && reinterpret_cast<bool(*)(QObject *object, std::function<void ()>)>(connectHasCompositeChanged)(object, slot);
}

static bool connectWindowListChanged(QObject *object, std::function<void ()> slot)
{
    QFunctionPointer connectWindowListChanged = Q_NULLPTR;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    connectWindowListChanged = qApp->platformFunction(_connectWindowListChanged);
#endif

    return connectWindowListChanged && reinterpret_cast<bool(*)(QObject *object, std::function<void ()>)>(connectWindowListChanged)(object, slot);
}

class DWindowManagerHelperPrivate : public DObjectPrivate
{
public:
    DWindowManagerHelperPrivate(DWindowManagerHelper *qq)
        : DObjectPrivate(qq) {}

    mutable QWindowList windowList;
};

class DWindowManagerHelper_ : public DWindowManagerHelper {};
Q_GLOBAL_STATIC(DWindowManagerHelper_, wmhGlobal)

DWindowManagerHelper::~DWindowManagerHelper()
{
    D_DC(DWindowManagerHelper);

    for (QWindow *w : d->windowList)
        w->deleteLater();
}

DWindowManagerHelper *DWindowManagerHelper::instance()
{
    return wmhGlobal;
}

bool DWindowManagerHelper::hasBlurWindow() const
{
    QFunctionPointer wmHasBlurWindow = Q_NULLPTR;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    wmHasBlurWindow = qApp->platformFunction(_hasBlurWindow);
#endif

    return wmHasBlurWindow && reinterpret_cast<bool(*)()>(wmHasBlurWindow)();
}

bool DWindowManagerHelper::hasComposite() const
{
    QFunctionPointer hasComposite = Q_NULLPTR;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    hasComposite = qApp->platformFunction(_hasComposite);
#endif

    return hasComposite && reinterpret_cast<bool(*)()>(hasComposite)();
}

QWindowList DWindowManagerHelper::currentWorkspaceWindows() const
{
    D_DC(DWindowManagerHelper);

    for (QWindow *w : d->windowList)
        w->deleteLater();

    d->windowList.clear();

    QFunctionPointer wmClientList = Q_NULLPTR;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    wmClientList = qApp->platformFunction(_getCurrentWorkspaceWindows);
#endif

    if (wmClientList) {
        for (WId wid : reinterpret_cast<QVector<quint32>(*)()>(wmClientList)()) {
            if (QWindow *w = QWindow::fromWinId(wid)) {
                d->windowList << w;
            }
        }
    }

    return d->windowList;
}

DWindowManagerHelper::DWindowManagerHelper(QObject *parent)
    : QObject(parent)
    , DObject(*new DWindowManagerHelperPrivate(this))
{
    connectWindowManagerChangedSignal(this, [this] {
        emit windowManagerChanged();
    });
    connectHasBlurWindowChanged(this, [this] {
        emit hasBlurWindowChanged();
    });
    connectHasCompositeChanged(this, [this] {
        emit hasCompositeChanged();
    });
    connectWindowListChanged(this, [this] {
        emit windowListChanged();
    });
}


DWIDGET_END_NAMESPACE
