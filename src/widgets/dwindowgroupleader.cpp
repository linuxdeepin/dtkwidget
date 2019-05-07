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

/*!
 * \~chinese \class DWindowGroupLeader
 *
 * \~chinese \brief DWindowGroupLeader 用于设置窗口所在的组，可以把多个窗口加到同一个组。在dxcb平台上，所有未指定组的
 * \~chinese 窗口都会被分配到 DWindowGroupLeader::clientLeaderId 这个组，且在窗口对应的本地窗口被创建时，会对
 * \~chinese Qt::Dialog、Qt::Sheet、Qt::Tool、Qt::SplashScreen、Qt::ToolTip、Qt::Drawer、Qt::Popup
 * \~chinese 类型的窗口设置 WM_TRANSIENT_FOR 属性，对于此类型的窗口，如果没有手动调用 QWindow::setTransientParent ，
 * \~chinese 则会将其 WM_TRANSIENT_FOR 属性设置为所在组id，得到的效果就是：会保证此窗口显示到这个组所有其它窗口
 * \~chinese 之上。例子：
 * \~chinese \code
 * DWindowGroupLeader leader;
 * QWindow wa, wb;
 * QWindow topWindow;
 *
 * leader.addWindow(&wa);
 * leader.addWindow(&wb);
 * leader.addWindow(&topWindow);
 *
 * wa.setTitle("窗口wa");
 * wa.resize(300, 100);
 * wa.show();
 * wb.setTitle("窗口wb");
 * wb.resize(300, 100);
 * wb.show();
 * topWindow.setTitle("窗口topWindow");
 * topWindow.setFlag(Qt::Dialog);
 * topWindow.resize(300, 300);
 * topWindow.show();
 * \endcode
 * \~chinese 窗口topWindow一直处于窗口wa和wb上层
 * \~chinese \image html wa_wb_topWindow.gif
 * \~chinese 如果一个组内同时存在多个未设置过 QWindow::setTransientParent 的 Qt::Dialog（其它会自动设置 WM_TRANSIENT_FOR
 * \~chinese 属性的窗口也成立）类型的窗口，这些窗口之间不会互相影响显示顺序，具体的规则和 X11 ICCCM 标准一致。
 * \~chinese \sa https://tronche.com/gui/x/icccm/sec-4.html#WM_TRANSIENT_FOR
 * \~chinese \sa DWindowGroupLeader::clientLeaderId
 * \~chinese \sa DApplication::loadDXcbPlugin
 */

/*!
 * \~chinese \brief DWindowGroupLeader::DWindowGroupLeader
 * \~chinese \param groupId 为0时会在需要时自动创建一个有效的 groupLeaderId
 * \~chinese \sa DWindowGroupLeader::groupLeaderId
 */
DWindowGroupLeader::DWindowGroupLeader(quint32 groupId)
    : d_ptr(new DWindowGroupLeaderPrivate(groupId))
{
    if (groupId != 0)
        d_ptr->groupLeaderFromUser = true;
}

/*!
 * \~chinese \brief DWindowGroupLeader::~DWindowGroupLeader
 * \~chinese 对象销毁时会释放由自己自动创建的 groupLeaderId
 * \~chinese \sa DWindowGroupLeader::groupLeaderId
 */
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

/*!
 * \~chinese \brief DWindowGroupLeader::groupLeaderId
 * \~chinese \return 返回组的id，类似于 QWindow::winId
 * \~chinese \warning 如果没有一个有效的id，则会先创建一个新的组id
 */
quint32 DWindowGroupLeader::groupLeaderId() const
{
    Q_D(const DWindowGroupLeader);

    const_cast<DWindowGroupLeaderPrivate*>(d)->ensureGroupLeader();

    return d->groupLeader;
}

/*!
 * \~chinese \brief DWindowGroupLeader::clientLeaderId
 * \~chinese \return 返回应用程序默认的组id
 */
quint32 DWindowGroupLeader::clientLeaderId() const
{
    Q_D(const DWindowGroupLeader);

    return d->clientLeader;
}

/*!
 * \~chinese \brief DWindowGroupLeader::addWindow
 * \~chinese 将窗口添加到这个组
 * \~chinese \param window
 * \~chinese \warning 每个窗口只能有一个组，添加到新的组时，将不再受旧的组所带来的任何影响
 */
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

/*!
 * \~chinese \brief DWindowGroupLeader::removeWindow
 * \~chinese 将窗口从这个组中移除
 * \~chinese \param window
 * \~chinese \warning 窗口被移除后将不再受此组带来的任何影响
 */
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

/*!
 * \~chinese \brief DWindowGroupLeader::addWindow
 * \~chinese 同 DWindowGroupLeader::addWindow(QWindow *)，这只是针对不同类型窗口对象的重载函数，
 * \~chinese 将 window->windowHandle() 作为参数传递给 DWindowGroupLeader::addWindow(QWindow *)
 * \~chinese \param window
 * \~chinese \warning 不要对非顶层窗口设置组，否则它会被设置 Qt::WA_NativeWindow 标志
 */
void DWindowGroupLeader::addWindow(QWidget *window)
{
    Q_ASSERT(window);
    Q_ASSERT(window->isWindow());

    if (!window->windowHandle()) {
        window->setAttribute(Qt::WA_NativeWindow);
    }

    return addWindow(window->windowHandle());
}

/*!
 * \~chinese \brief DWindowGroupLeader::removeWindow
 * \~chinese 同 DWindowGroupLeader::removeWindow(QWindow *)，这只是针对不同类型窗口对象的重载函数，
 * \~chinese 等价于 removeWindow(window->windowHandle())
 * \~chinese \param window
 */
void DWindowGroupLeader::removeWindow(QWidget *window)
{
    return removeWindow(window->windowHandle());
}

DWIDGET_END_NAMESPACE
