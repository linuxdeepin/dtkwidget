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

/*!
 * \~chinese \class DWindowManagerHelper
 * \~chinese \brief 提供与窗口管理器交互的接口，同 \a DPlatformWindowHandle 依赖 dxcb 插件。
 * \~chinese dxcb 插件抽象出所有需要和X11平台交互的接口以供上层调用，DTK 使用插件中提供的接口再
 * \~chinese 次封装提供给应用程序使用，从设计角度讲，DTK库中不应该直接使用任何跟平台相关的接口
 * \~chinese （如：X11、Wayland、Windows），在这样的结构支撑下，在一个新的平台上，只需要提供和
 * \~chinese dxcb 同样的接口，DTK应用即可无缝迁移。
 * \~chinese \sa \href{https://github.com/linuxdeepin/qt5dxcb-plugin/,dxcb插件}
 * \~chinese \sa DApplication::loadDXcbPlugin
 * \~chinese \sa DApplication::isDXcbPlatform
 * \~chinese \sa DPlatformWindowHandle
 */

/*!
 * \~chinese \property DWindowManagerHelper::hasBlurWindow
 * \~chinese \brief 窗口管理器是否支持窗口背景模糊特效
 * \~chinese \note 在 dxcb 插件中目前只支持 deepin-wm 和 kwin 这两种窗管的模糊特效
 * \~chinese \note 只读
 */

/*!
 * \~chinese \property DWindowManagerHelper::hasComposite
 * \~chinese \brief 窗口管理器是否支持混成效果。如果不支持混成，则表示所有窗口的背景都不能透明，
 * \~chinese 随之而来也不会有窗口阴影等效果，不规则窗口的边缘也会存在锯齿。
 * \~chinese \note 只读
 */

/*!
 * \~chinese \enum DWindowManagerHelper::MotifFunction
 * \~chinese MotifFunction::MotifFunction 窗口管理器对窗口所能控制的行为
 *
 * \~chinese \var DWindowManagerHelper::FUNC_RESIZE DWindowManagerHelper::FUNC_RESIZE
 * \~chinese 控制窗口大小。如果存在此标志，则窗口管理器可以改变窗口大小（如使用鼠标拖拽窗口边缘），
 * \~chinese 否则无法通过外部行为调整窗口大小。
 * \~chinese \code
 * DMainWindow w;
 *
 * w.resize(400, 200);
 * w.show();
 * DWindowManagerHelper::setMotifFunctions(w.windowHandle(), DWindowManagerHelper::FUNC_RESIZE, false);
 * \endcode
 * \~chinese \image html disable_resize_function.gif
 * \~chinese \note 普通窗口默认存在此标志，对于 Qt::Popup 和 Qt::BypassWindowManagerHint
 * \~chinese 类型的窗口，不受此标志位影响
 * \~chinese \note 设置此标志后也会影响窗口标题栏对应功能入口的状态
 * \~chinese \note 对于使用系统标题栏的窗口，此功能和具体窗口管理器实现相关，deepin-wm 中设置
 * \~chinese 此标志无效。
 * \~chinese \sa DPlatformWindowHandle::enableDXcbForWindow
 * \~chinese \sa DPlatformWindowHandle::isEnabledDXcb
 *
 * \~chinese \var DWindowManagerHelper::FUNC_MOVE DWindowManagerHelper::FUNC_MOVE
 * \~chinese 控制窗口位置。如果存在此标志，则窗口管理器可以移动窗口（如使用鼠标拖动标题栏），否则
 * \~chinese 无法通过外部行为移动窗口位置。
 * \~chinese \code
 * DWindowManagerHelper::setMotifFunctions(w.windowHandle(), DWindowManagerHelper::FUNC_MOVE, false);
 * \endcode
 * \~chinese \image html disable_move_function.gif
 * \~chinese \var DWindowManagerHelper::FUNC_MINIMIZE DWindowManagerHelper::FUNC_MINIMIZE
 * \~chinese 最小化窗口。如果存在此标志，则窗口可以被最小化（如点击标题栏的最小化按钮），否则无法
 * \~chinese 通过外部行为最小化窗口。
 * \~chinese \code
 * DWindowManagerHelper::setMotifFunctions(w.windowHandle(), DWindowManagerHelper::FUNC_MINIMIZE, false);
 * \endcode
 * \~chinese \image html disable_minimize_function.gif
 * \~chinese \note 设置此标志后也会影响窗口标题栏对应功能入口的状态
 *
 * \~chinese \var DWindowManagerHelper::FUNC_MAXIMIZE DWindowManagerHelper::FUNC_MAXIMIZE
 * \~chinese 最大化窗口。如果存在此标志，则窗口可以被最大化（如点击标题栏的最大化按钮），否则无法
 * \~chinese 通过外部行为最大化窗口。
 * \~chinese \code
 * DWindowManagerHelper::setMotifFunctions(w.windowHandle(), DWindowManagerHelper::FUNC_MAXIMIZE, false);
 * \endcode
 * \~chinese \image html disable_maximize_function.gif
 * \~chinese \note 设置此标志后也会影响窗口标题栏对应功能入口的状态
 *
 * \~chinese \var DWindowManagerHelper::FUNC_CLOSE DWindowManagerHelper::FUNC_CLOSE
 * \~chinese 关闭窗口。如果存在此标志，则窗口可以被关闭（如点击标题栏的关闭按钮或使用Alt+F4快捷键），
 * \~chinese 否则无法通过外部行为关闭窗口。
 * \~chinese \code
 * DWindowManagerHelper::setMotifFunctions(w.windowHandle(), DWindowManagerHelper::FUNC_CLOSE, false);
 * \endcode
 * \~chinese \image html disable_close_function.gif
 * \~chinese \note 设置此标志后也会影响窗口标题栏对应功能入口的状态
 *
 * \~chinese \var DWindowManagerHelper::FUNC_ALL DWindowManagerHelper::FUNC_ALL
 * \~chinese 所有功能性行为
 */

/*!
 * \~chinese \enum DWindowManagerHelper::MotifDecoration
 * \~chinese MotifFunction::MotifDecoration 窗口管理器对窗口添加的修饰。只影响窗口上对应功能
 * \~chinese 的入口，不影响实际的功能，比如：禁用掉 FUNC_MAXIMIZE 后，还可以使用快捷键最大化窗口
 *
 * \~chinese \var DWindowManagerHelper::DECOR_BORDER DWindowManagerHelper::DECOR_BORDER
 * \~chinese 窗口描边。如果存在此标志，则窗口管理器会为窗口绘制描边，否则窗口没有描边。
 * \~chinese 否则无法通过外部行为调整窗口大小。
 * \~chinese \note 只支持使用系统标题栏的窗口，此功能和具体窗口管理器实现相关，deepin-wm 中设置
 * \~chinese 此标志无效。
 *
 * \~chinese \var DWindowManagerHelper::DECOR_RESIZEH DWindowManagerHelper::DECOR_RESIZEH
 * \~chinese 改变窗口大小。如果存在此标志，则窗口管理器会在窗口的修饰上显示一个更改窗口大小的控件，
 * \~chinese 否则无此控件。
 * \~chinese \note 只支持使用系统标题栏的窗口，此功能和具体窗口管理器实现相关，deepin-wm 中设置
 * \~chinese 此标志无效。
 *
 * \~chinese \var DWindowManagerHelper::DECOR_TITLE DWindowManagerHelper::DECOR_TITLE
 * \~chinese 窗口标题。如果存在此标志，则窗口管理器会在窗口的修饰上显示窗口标题，否则不显示。
 * \~chinese \note 只支持使用系统标题栏的窗口，此功能和具体窗口管理器实现相关，deepin-wm 中设置
 * \~chinese 此标志无效。
 *
 * \~chinese \var DWindowManagerHelper::DECOR_MENU DWindowManagerHelper::DECOR_MENU
 * \~chinese 窗口菜单。如果存在此标志，则窗口管理器会在窗口的修饰上显示一个窗口菜单控件，否则不显示。
 * \~chinese \note 只支持使用系统标题栏的窗口，此功能和具体窗口管理器实现相关，deepin-wm 中设置
 * \~chinese 此标志无效。
 *
 * \~chinese \var DWindowManagerHelper::DECOR_MINIMIZE DWindowManagerHelper::DECOR_MINIMIZE
 * \~chinese 窗口最小化。如果存在此标志，则窗口管理器会在窗口的修饰上显示一个最小化窗口控件，否则不显示。
 * \~chinese \note 只支持使用系统标题栏的窗口，此功能和具体窗口管理器实现相关，deepin-wm 中设置
 * \~chinese 此标志无效。
 * \~chinese \sa Qt::WindowMinimizeButtonHint
 *
 * \~chinese \var DWindowManagerHelper::DECOR_MAXIMIZE DWindowManagerHelper::DECOR_MAXIMIZE
 * \~chinese 窗口最大化。如果存在此标志，则窗口管理器会在窗口的修饰上显示一个最大化窗口控件，否则不显示。
 * \~chinese \note 只支持使用系统标题栏的窗口，此功能和具体窗口管理器实现相关，deepin-wm 中设置
 * \~chinese 此标志无效。
 * \~chinese \sa Qt::WindowMaximizeButtonHint
 *
 * \~chinese \var DWindowManagerHelper::DECOR_ALL DWindowManagerHelper::DECOR_ALL
 * \~chinese 所有窗口修饰。
 */

/*!
 * \~chinese \enum DWindowManagerHelper::WMName
 * \~chinese DWindowManagerHelper::WMName 窗口管理器类型
 * \~chinese \var DWindowManagerHelper::DeepinWM DWindowManagerHelper::DeepinWM
 * \~chinese 深度系统桌面环境窗口管理器
 *
 * \~chinese \var DWindowManagerHelper::DeepinWM DWindowManagerHelper::KWinWM
 * \~chinese KDE系统桌面环境窗口管理器
 *
 * \~chinese \var DWindowManagerHelper::DeepinWM DWindowManagerHelper::OtherWM
 * \~chinese 其它窗口管理器
 */

/*!
 * \~chinese \fn DWindowManagerHelper::windowManagerChanged
 * \~chinese \brief 信号会在当前环境窗口管理器变化时被发送
 * \~chinese \fn DWindowManagerHelper::hasBlurWindowChanged
 * \~chinese \brief 信号会在 hasBlurWindow 属性的值改变时被发送
 * \~chinese \fn DWindowManagerHelper::hasCompositeChanged
 * \~chinese \brief 信号会在 hasComposite 属性的值改变时被发送
 * \~chinese \fn DWindowManagerHelper::windowListChanged
 * \~chinese \brief 信号会在当前环境本地窗口列表变化时被发送。包含打开新窗口、关闭窗口、改变窗口的
 * \~chinese 层叠顺序
 * \~chinese \fn DWindowManagerHelper::windowMotifWMHintsChanged
 * \~chinese \brief 信号会在窗口功能或修饰标志改变时被发送
 * \~chinese \param 窗口id
 * \~chinese \note 只对当前应用程序中的窗口有效
 */

DWindowManagerHelper::~DWindowManagerHelper()
{
    D_DC(DWindowManagerHelper);

    for (QWindow *w : d->windowList) {
        w->deleteLater();
    }
}

/*!
 * \~chinese \brief DWindowManagerHelper::instance
 * \~chinese DWindowManagerHelper 的单例对象，使用 Q_GLOBAL_STATIC 定义，在第一次调用时实例化。
 * \~chinese \return
 */
DWindowManagerHelper *DWindowManagerHelper::instance()
{
    return wmhGlobal;
}

/*!
 * \~chinese \brief DWindowManagerHelper::setMotifFunctions
 * \~chinese 设置窗口的功能性标志，会覆盖之前的设置
 * \~chinese \param window
 * \~chinese \param hints
 */
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

/*!
 * \~chinese \brief DWindowManagerHelper::setMotifFunctions
 * \~chinese 设置窗口某些标志位的开启状态，不影响其它标志位
 * \~chinese \param window
 * \~chinese \param hints 要设置的标志位
 * \~chinese \param on 如果值为 true 则开启标志，否则关闭
 * \~chinese \return 返回设置后的窗口标志
 */
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

/*!
 * \~chinese \brief DWindowManagerHelper::getMotifFunctions
 * \~chinese \param window
 * \~chinese \return 返回窗口当前的功能标志
 */
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

/*!
 * \~chinese \brief DWindowManagerHelper::setMotifDecorations
 * \~chinese 设置窗口的修饰性标志，会覆盖之前的设置
 * \~chinese \param window
 * \~chinese \param hints
 */
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

/*!
 * \~chinese \brief DWindowManagerHelper::setMotifFunctions
 * \~chinese 设置窗口某些标志位的开启状态，不影响其它标志位
 * \~chinese \param window
 * \~chinese \param hints 要设置的标志位
 * \~chinese \param on 如果值为 true 则开启标志，否则关闭
 * \~chinese \return 返回设置后的窗口标志
 */
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

/*!
 * \~chinese \brief DWindowManagerHelper::getMotifFunctions
 * \~chinese \param window
 * \~chinese \return 返回窗口当前的修饰标志
 */
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

/*!
 * \~chinese \brief DWindowManagerHelper::popupSystemWindowMenu
 * \~chinese 显示窗口管理器对窗口的菜单，和有边框的窗口在标题栏上点击鼠标右键弹出的菜单内容一致。
 * \~chinese 在 DMainWindow 的标题栏上点击鼠标右键会调用此函数打开系统菜单：
 * \~chinese \image html window_system_menu.gif
 * \~chinese \param window
 */
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

/*!
 * \~chinese \brief DWindowManagerHelper::hasBlurWindow
 * \~chinese \return 如果当前窗口管理器支持窗口背景模糊特效则返回 true，否则返回 false
 */
bool DWindowManagerHelper::hasBlurWindow() const
{
    QFunctionPointer wmHasBlurWindow = Q_NULLPTR;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    wmHasBlurWindow = qApp->platformFunction(_hasBlurWindow);
#endif

    return wmHasBlurWindow && reinterpret_cast<bool(*)()>(wmHasBlurWindow)();
}

/*!
 * \~chinese \brief DWindowManagerHelper::hasComposite
 * \~chinese \return 如果当前窗口管理器支持混成则返回 true，否则返回 false
 */
bool DWindowManagerHelper::hasComposite() const
{
    QFunctionPointer hasComposite = Q_NULLPTR;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    hasComposite = qApp->platformFunction(_hasComposite);
#endif

    return hasComposite && reinterpret_cast<bool(*)()>(hasComposite)();
}

/*!
 * \~chinese \brief DWindowManagerHelper::windowManagerNameString
 * \~chinese \return 返回窗口管理器名称。在X11平台上，此值为窗口管理器对应窗口的 _NET_WM_NAME
 * \~chinese 的值
 * \~chinese \sa \href{https://specifications.freedesktop.org/wm-spec/1.3/ar01s03.html,_NET_SUPPORTING_WM_CHECK}
 * \~chinese \sa \href{https://specifications.freedesktop.org/wm-spec/1.3/ar01s05.html,_NET_WM_NAME}
 */
QString DWindowManagerHelper::windowManagerNameString() const
{
    QFunctionPointer windowManagerName = Q_NULLPTR;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    windowManagerName = qApp->platformFunction(_windowManagerName);
#endif

    return windowManagerName ? reinterpret_cast<QString(*)()>(windowManagerName)() : QString();
}

/*!
 * \~chinese \brief DWindowManagerHelper::windowManagerName
 * \~chinese \return 返回当前窗口管理器类型
 * \~chinese \sa DWindowManagerHelper::windowManagerNameString
 */
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

/*!
 * \~chinese \brief DWindowManagerHelper::allWindowIdList
 * \~chinese \return 返回当前环境所有本地窗口的窗口id列表
 * \~chinese \note 顺序和窗口层叠顺序相关，显示越靠下层的窗口在列表中顺序越靠前
 * \~chinese \sa DWindowManagerHelper::currentWorkspaceWindowIdList
 */
QVector<quint32> DWindowManagerHelper::allWindowIdList() const
{
    QFunctionPointer wmClientList = Q_NULLPTR;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    wmClientList = qApp->platformFunction(_getWindows);
#endif

    if (!wmClientList) return QVector<quint32>();

    return reinterpret_cast<QVector<quint32>(*)()>(wmClientList)();
}

/*!
 * \~chinese \brief DWindowManagerHelper::currentWorkspaceWindowIdList
 * \~chinese \return 返回当前工作区所有本地窗口的窗口id列表
 * \~chinese \note 顺序和窗口层叠顺序相关，显示越靠下层的窗口在列表中顺序越靠前
 * \~chinese \sa DWindowManagerHelper::allWindowIdList
 */
QVector<quint32> DWindowManagerHelper::currentWorkspaceWindowIdList() const
{
    QFunctionPointer wmClientList = Q_NULLPTR;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    wmClientList = qApp->platformFunction(_getCurrentWorkspaceWindows);
#endif

    if (!wmClientList) return QVector<quint32>();

    return reinterpret_cast<QVector<quint32>(*)()>(wmClientList)();
}

/*!
 * \~chinese \brief DWindowManagerHelper::currentWorkspaceWindowIdList
 * \~chinese \return 返回当前工作区所有本地窗口对象列表。和 \a currentWorkspaceWindowIdList
 * \~chinese 类似，只不过自动通过窗口id创建了 DForeignWindow 对象
 * \~chinese \note 顺序和窗口层叠顺序相关，显示越靠下层的窗口在列表中顺序越靠前
 * \~chinese \note 列表中对象的生命周期由 DForeignWindow 负责
 * \~chinese \sa DWindowManagerHelper::currentWorkspaceWindowIdList
 * \~chinese \sa DForeignWindow::fromWinId
 */
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
        if (window->property("_q_foreignWinId").isValid()) continue;

        currentApplicationWindowList.append(window->winId());
    }

    QVector<quint32> wmClientList = currentWorkspaceWindowIdList();

    for (WId wid : wmClientList) {
        if (currentApplicationWindowList.contains(wid))
            continue;

        if (DForeignWindow *w = DForeignWindow::fromWinId(wid)) {
            d->windowList << w;
        }
    }

    return d->windowList;
}

/*!
 * \~chinese \brief DWindowManagerHelper::DWindowManagerHelper
 * \~chinese 不允许直接实例化此对象
 * \~chinese \param parent
 * \~chinese \sa DWindowManagerHelper::instance
 */
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
