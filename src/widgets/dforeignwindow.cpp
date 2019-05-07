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

/*!
 * \~chinese \class DForeignWindow
 * \~chinese \brief 一个用于获取本地窗口信息的类，继承于 QWindow，支持 QWindow::geometry
 * \~chinese QWindow::x QWindow::y QWindow::width QWindow::height
 * \~chinese QWindow::title QWindow::flags QWindow::visibility QWindow::type
 * \~chinese QWindow::windowStates QWindow::windowState 等接口的使用，另外扩展
 * \~chinese 增加了一部分接口，方面更加详细的获取窗口信息。依赖于 dxcb 插件，在未加载
 * \~chinese dxcb 插件的应用中使用时结果未知
 * \~chinese \sa DWindowManagerHelper::currentWorkspaceWindows
 * \~chinese \sa DApplication::loadDXcbPlugin
 * \~chinese \sa DApplication::isDXcbPlatform
 */

/*!
 * \~chinese \property DForeignWindow::wmClass
 * \~chinese \brief 窗口 WM_CLASS 的值
 * \~chinese \note 只读
 * \~chinese \sa \href{https://tronche.com/gui/x/icccm/sec-4.html#WM_CLASS,WM_CLASS}
 */

/*!
 * \~chinese \property DForeignWindow::pid
 * \~chinese \brief 窗口所属进程的 pid
 * \~chinese \note 只读
 * \~chinese \sa \href{https://specifications.freedesktop.org/wm-spec/1.3/ar01s05.html,_NET_WM_PID}
 */

/*!
 * \~chinese \fn DForeignWindow::wmClassChanged
 * \~chinese \brief 信号会在 \a wmClass 属性改变时被发送
 * \~chinese \fn DForeignWindow::pidChanged
 * \~chinese \brief 信号会在 \a pid 属性的值改变时被发送
 */

/*!
 * \~chinese \brief DForeignWindow::DForeignWindow
 * \~chinese 直接构造一个 DForeignWindow 对象和使用 QWindow 对象没有区别
 * \~chinese \param parent
 * \~chinese \sa DForeignWindow::fromWinId
 */
DForeignWindow::DForeignWindow(QWindow *parent)
    : QWindow(parent)
    , DObject(*new DForeignWindowPrivate(this))
{

}

/*!
 * \~chinese \brief DForeignWindow::fromWinId
 * \~chinese 使用这个窗口id创建一个 DForeignWindow 对象，此对象不会被加到 QGuiApplication::allWindows
 * \~chinese 中。一般应用在需要获取一个本地窗口信息的场景。示例：
 * \~chinese \code
 * // a.cpp
 * int main(int argc, char *argv[])
 * {
 *     DApplication a(argc, argv);
 *
 *     QWidget w;
 *
 *     w.setWindowTitle("deepin");
 *     w.show();
 *
 *     QFile app_win_id("/tmp/window_id.txt");
 *     if (app_win_id.open(QFile::WriteOnly)) {
 *         app_win_id.write(QByteArray::number(w.winId()));
 *         app_win_id.close();
 *     }
 *
 *     return a.exec();
 * }
 * \endcode
 *
 * \~chinese \code
 * // b.cpp
 * int main(int argc, char *argv[])
 * {
 *     DApplication::loadDXcbPlugin();
 *     DApplication a(argc, argv);
 *
 *     DForeignWindow *fw = nullptr;
 *     QFile app_win_id("/tmp/window_id.txt");
 *     if (app_win_id.open(QFile::ReadOnly)) {
 *         fw = DForeignWindow::fromWinId(app_win_id.readAll().toInt());
 *     }
 *
 *     if (fw) {
 *         qDebug() << fw->title();
 *
 *         fw->connect(fw, &DForeignWindow::widthChanged, [&] {
 *             qDebug() << fw->width();
 *         });
 *     }
 *
 *     return a.exec();
 * }
 * \endcode
 *
 * \arg \c 先启动应用 a
 * \arg \c 再启动应用 b
 *
 * \~chinese 在应用 b 启动后将看到如下输出：
 * \~chinese \code
 * "deepin"
 * \endcode
 * 当改变应用 a 中的窗口宽度时，在应用 b 中会看到宽度的输出
 * \~chinese \param id
 * \~chinese \return
 * \~chinese \warning 不要尝试对由本应用创建的窗口调用此接口，可能会导致窗口行为发生不可逆转的变化
 */
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
