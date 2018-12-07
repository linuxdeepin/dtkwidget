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

#include "dwindowmanagerhelper.h"
#include "dforeignwindow.h"

#include <DObjectPrivate>
#include <QGuiApplication>

#include <qpa/qplatformwindow.h>

#include <functional>

DWIDGET_BEGIN_NAMESPACE

#define DEFINE_CONST_CHAR(Name) const char _##Name[] = "_d_" #Name
#define MWM_FUNC_ALL (1L << 0)
#define MWM_DECOR_ALL (1L << 0)

// functions
DEFINE_CONST_CHAR(hasBlurWindow);
DEFINE_CONST_CHAR(hasComposite);
DEFINE_CONST_CHAR(windowManagerName);
DEFINE_CONST_CHAR(connectWindowManagerChangedSignal);
DEFINE_CONST_CHAR(connectHasBlurWindowChanged);
DEFINE_CONST_CHAR(connectHasCompositeChanged);
DEFINE_CONST_CHAR(getCurrentWorkspaceWindows);
DEFINE_CONST_CHAR(getWindows);
DEFINE_CONST_CHAR(connectWindowListChanged);
DEFINE_CONST_CHAR(setMWMFunctions);
DEFINE_CONST_CHAR(getMWMFunctions);
DEFINE_CONST_CHAR(setMWMDecorations);
DEFINE_CONST_CHAR(getMWMDecorations);
DEFINE_CONST_CHAR(connectWindowMotifWMHintsChanged);
DEFINE_CONST_CHAR(popupSystemWindowMenu);

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

static bool connectWindowMotifWMHintsChanged(QObject *object, std::function<void (quint32)> slot)
{
    QFunctionPointer connectWindowMotifWMHintsChanged = Q_NULLPTR;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    connectWindowMotifWMHintsChanged = qApp->platformFunction(_connectWindowMotifWMHintsChanged);
#endif

    return connectWindowMotifWMHintsChanged && reinterpret_cast<bool(*)(QObject *object, std::function<void (quint32)>)>(connectWindowMotifWMHintsChanged)(object, slot);
}

class DWindowManagerHelperPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    DWindowManagerHelperPrivate(DWindowManagerHelper *qq)
        : DObjectPrivate(qq) {}

    mutable QList<DForeignWindow *> windowList;
};

class DWindowManagerHelper_ : public DWindowManagerHelper {};
Q_GLOBAL_STATIC(DWindowManagerHelper_, wmhGlobal)

DWindowManagerHelper::~DWindowManagerHelper()
{
    D_DC(DWindowManagerHelper);

    for (QWindow *w : d->windowList) {
        w->deleteLater();
    }
}

DWindowManagerHelper *DWindowManagerHelper::instance()
{
    return wmhGlobal;
}

void DWindowManagerHelper::setMotifFunctions(const QWindow *window, MotifFunctions hints)
{
    QFunctionPointer setMWMFunctions = Q_NULLPTR;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    setMWMFunctions = qApp->platformFunction(_setMWMFunctions);
#endif

    if (setMWMFunctions && window->handle()) {
        if (hints == FUNC_ALL)
            hints = (MotifFunction)MWM_FUNC_ALL;

        reinterpret_cast<void(*)(quint32, quint32)>(setMWMFunctions)(window->handle()->winId(), (quint32)hints);
    }
}

DWindowManagerHelper::MotifFunctions DWindowManagerHelper::setMotifFunctions(const QWindow *window, MotifFunctions hints, bool on)
{
    MotifFunctions old_hints = getMotifFunctions(window);

    if (on)
        hints |= old_hints;
    else
        hints = old_hints & ~hints;

    setMotifFunctions(window, hints);

    return hints;
}

DWindowManagerHelper::MotifFunctions DWindowManagerHelper::getMotifFunctions(const QWindow *window)
{
    QFunctionPointer getMWMFunctions = Q_NULLPTR;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    getMWMFunctions = qApp->platformFunction(_getMWMFunctions);
#endif

    if (getMWMFunctions && window->handle()) {
        quint32 hints = reinterpret_cast<quint32(*)(quint32)>(getMWMFunctions)(window->handle()->winId());

        if (!(hints & MWM_FUNC_ALL))
            return (MotifFunctions)hints;
    }

    return FUNC_ALL;
}

void DWindowManagerHelper::setMotifDecorations(const QWindow *window, MotifDecorations hints)
{
    QFunctionPointer setMWMDecorations = Q_NULLPTR;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    setMWMDecorations = qApp->platformFunction(_setMWMDecorations);
#endif

    if (setMWMDecorations && window->handle()) {
        if (hints == DECOR_ALL)
            hints = (MotifDecoration)MWM_DECOR_ALL;

        reinterpret_cast<void(*)(quint32, quint32)>(setMWMDecorations)(window->handle()->winId(), (quint32)hints);
    }
}

DWindowManagerHelper::MotifDecorations DWindowManagerHelper::setMotifDecorations(const QWindow *window, MotifDecorations hints, bool on)
{
    MotifDecorations old_hints = getMotifDecorations(window);

    if (on)
        hints |= old_hints;
    else
        hints = old_hints & ~hints;

    setMotifDecorations(window, hints);

    return hints;
}

DWindowManagerHelper::MotifDecorations DWindowManagerHelper::getMotifDecorations(const QWindow *window)
{
    QFunctionPointer getMWMDecorations = Q_NULLPTR;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    getMWMDecorations = qApp->platformFunction(_getMWMDecorations);
#endif

    if (getMWMDecorations && window->handle()) {
        quint32 hints = reinterpret_cast<quint32(*)(quint32)>(getMWMDecorations)(window->handle()->winId());

        if (!(hints & MWM_DECOR_ALL))
            return (MotifDecorations)hints;
    }

    return DECOR_ALL;
}

void DWindowManagerHelper::popupSystemWindowMenu(const QWindow *window)
{
    QFunctionPointer popupSystemWindowMenu = Q_NULLPTR;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    popupSystemWindowMenu = qApp->platformFunction(_popupSystemWindowMenu);
#endif

    if (popupSystemWindowMenu && window->handle()) {
        reinterpret_cast<void(*)(quint32)>(popupSystemWindowMenu)(window->handle()->winId());
    }
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

QString DWindowManagerHelper::windowManagerNameString() const
{
    QFunctionPointer windowManagerName = Q_NULLPTR;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    windowManagerName = qApp->platformFunction(_windowManagerName);
#endif

    return windowManagerName ? reinterpret_cast<QString(*)()>(windowManagerName)() : QString();
}

DWindowManagerHelper::WMName DWindowManagerHelper::windowManagerName() const
{
    const QString &wmName = windowManagerNameString();

    if (wmName == QStringLiteral("Mutter(DeepinGala)")) {
        return DeepinWM;
    }

    if (wmName == QStringLiteral("KWin")) {
        return KWinWM;
    }

    return OtherWM;
}

QVector<WId> DWindowManagerHelper::allWindowIdList() const
{
    QFunctionPointer wmClientList = Q_NULLPTR;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    wmClientList = qApp->platformFunction(_getWindows);
#endif

    if (!wmClientList) return QVector<WId>();

    return reinterpret_cast<QVector<WId>(*)()>(wmClientList)();
}

QVector<WId> DWindowManagerHelper::currentWorkspaceWindowIdList() const
{
    QFunctionPointer wmClientList = Q_NULLPTR;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    wmClientList = qApp->platformFunction(_getCurrentWorkspaceWindows);
#endif

    if (!wmClientList) return QVector<WId>();

    return reinterpret_cast<QVector<WId>(*)()>(wmClientList)();
}

QList<DForeignWindow *> DWindowManagerHelper::currentWorkspaceWindows() const
{
    D_DC(DWindowManagerHelper);

    for (QWindow *w : d->windowList) {
        w->deleteLater();
    }

    d->windowList.clear();

    QList<WId> currentApplicationWindowList;
    const QWindowList &list = qApp->allWindows();

    currentApplicationWindowList.reserve(list.size());

    for (auto window : list) {
        currentApplicationWindowList.append(window->winId());
    }

    QVector<WId> wmClientList = currentWorkspaceWindowIdList();

    for (WId wid : wmClientList) {
        if (currentApplicationWindowList.contains(wid))
            continue;

        if (DForeignWindow *w = DForeignWindow::fromWinId(wid)) {
            d->windowList << w;
        }
    }

    return d->windowList;
}

DWindowManagerHelper::DWindowManagerHelper(QObject *parent)
    : QObject(parent)
    , DObject(*new DWindowManagerHelperPrivate(this))
{
    connectWindowManagerChangedSignal(this, [this] {
        Q_EMIT windowManagerChanged();
    });
    connectHasBlurWindowChanged(this, [this] {
        Q_EMIT hasBlurWindowChanged();
    });
    connectHasCompositeChanged(this, [this] {
        Q_EMIT hasCompositeChanged();
    });
    connectWindowListChanged(this, [this] {
        Q_EMIT windowListChanged();
    });
    connectWindowMotifWMHintsChanged(this, [this] (quint32 winId) {
        Q_EMIT windowMotifWMHintsChanged(winId);
    });
}


DWIDGET_END_NAMESPACE
