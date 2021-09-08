/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "dplatformwindowhandle.h"

#include <QWidget>
#include <QApplication>

DWIDGET_BEGIN_NAMESPACE

static QWindow *ensureWindowHandle(QWidget *widget)
{
    QWidget *window = widget->window();
    QWindow *handle = window->windowHandle();

    if (!handle) {
        bool save_flag = qApp->testAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
        qApp->setAttribute(Qt::AA_DontCreateNativeWidgetSiblings, true);

        if (window->testAttribute(Qt::WA_NativeWindow)) {
            // FIX null handle return on app kdevelop
            window->setAttribute(Qt::WA_NativeWindow, false);
        }

        window->setAttribute(Qt::WA_NativeWindow);
        handle = window->windowHandle();
        window->setAttribute(Qt::WA_NativeWindow, false);

        // dxcb version >= 1.1.6
        if (!DPlatformWindowHandle::pluginVersion().isEmpty()) {
            /// TODO: Avoid call parentWidget()->enforceNativeChildren().
            qApp->setAttribute(Qt::AA_DontCreateNativeWidgetSiblings, save_flag);
        }
    }

    return handle;
}

/*!
    \class Dtk::Widget::DPlatformWindowHandle
    \inmodule dtkwidget
 */

/*!
  \brief DPlatformWindowHandle::DPlatformWindowHandle.

  同 DPlatformHandle::DPlatformHandle(QWindow *window, QObject *parent)，
  这只是针对 QWidget 对象的重载，将 \a widget 对象传递给 enableDXcbForWindow。

  \a widget 要开启DTK风格的主窗口
  \a parent DPlatformWindowHandle 对象的父对象
  \sa Dtk::Gui::DPlatformHandle::enableDXcbForWindow()
 */
DPlatformWindowHandle::DPlatformWindowHandle(QWidget *widget, QObject *parent)
    : DPlatformHandle(ensureWindowHandle(widget), parent)
{

}

/*!
  \fn void DPlatformWindowHandle::enableDXcbForWindow(QWidget *widget)
  \brief DPlatformWindowHandle::enableDXcbForWindow.
  \overload

  这是一个针对 QWidget 类型主窗口的重载函数，此函数将设置 QWidget 对象的 Qt::WA_NativeWindow
  标志，之后将其 QWidget::windowHandle 对象作为参数传递给 enableDXcbForWindow(QWindow *)。

  \a widget
  \sa DPlatformWindowHandle::enableDXcbForWindow(QWindow *window)
 */
void DPlatformWindowHandle::enableDXcbForWindow(QWidget *widget)
{
    if (!isDXcbPlatform())
        return;

    QWindow *handle = ensureWindowHandle(widget);

    DPlatformHandle::enableDXcbForWindow(handle);
}

/*!
  \fn void DPlatformWindowHandle::enableDXcbForWindow(QWidget *widget, bool redirectContent)
  \brief DPlatformWindowHandle::enableDXcbForWindow
  \overload

  这只是一个重载函数
  \a widget
  \a redirectContent
  \sa DPlatformWindowHandle::enableDXcbForWindow(QWidget*)
  \sa DPlatformWindowHandle::enableDXcbForWindow(QWindow *, bool)
 */
void DPlatformWindowHandle::enableDXcbForWindow(QWidget *widget, bool redirectContent)
{
    DPlatformHandle::enableDXcbForWindow(ensureWindowHandle(widget), redirectContent);
}

/*!
  \brief DPlatformWindowHandle::isEnabledDXcb.

  这只是一个重载函数，如果 widget 的 QWidget::windowHandle 为 nullptr 则返回 false，
  否则返回 isEnabledDXcb(widget->windowHandle()) 的值。

  \a widget
  \return
 */
bool DPlatformWindowHandle::isEnabledDXcb(const QWidget *widget)
{
    if (QWindow *handle = widget->windowHandle()) {
        return DPlatformHandle::isEnabledDXcb(handle);
    }

    return false;
}

/*!
  \fn bool DPlatformWindowHandle::setWindowBlurAreaByWM(QWidget *widget, const QVector<DPlatformHandle::WMBlurArea> &area)
  \brief DPlatformWindowHandle::setWindowBlurAreaByWM.
  \overload

  这只是一个重载函数，如果 widget 的 QWidget::windowHandle 为 nullptr 则返回 false，
  否则返回 setWindowBlurAreaByWM(wiget->windowHandle(), area) 的值。示例：
  \code
  DMainWindow w;
  QVector<DPlatformWindowHandle::WMBlurArea> area_list;
  DPlatformWindowHandle::WMBlurArea area;
  
  area.x = 50;
  area.y = 50;
  area.width = 200;
  area.height = 200;
  area.xRadius = 10;
  area.yRaduis = 10;
  area_list.append(area);
  
  DPlatformWindowHandle::setWindowBlurAreaByWM(&w, area_list);
  
  w.resize(300, 300);
  w.setAttribute(Qt::WA_TranslucentBackground);
  w.show();
  
  \endcode
  \a widget
  \a area
  \return
  \note 需要对 \a widget 添加 Qt::WA_TranslucentBackground 使其背景透明
  \sa QWidget::setAttribute
 */
bool DPlatformWindowHandle::setWindowBlurAreaByWM(QWidget *widget, const QVector<DPlatformHandle::WMBlurArea> &area)
{
    Q_ASSERT(widget);

    return widget->windowHandle() && DPlatformHandle::setWindowBlurAreaByWM(widget->windowHandle(), area);
}

/*!
  \fn bool DPlatformWindowHandle::setWindowBlurAreaByWM(QWidget *widget, const QList<QPainterPath> &paths)
  \brief DPlatformWindowHandle::setWindowBlurAreaByWM.
  \overload

  这只是一个重载函数，如果 \a widget 的 QWidget::windowHandle 为 nullptr 则返回 false，
  否则返回 setWindowBlurAreaByWM(wiget->windowHandle(), paths) 的值。示例：
  \code
  DMainWindow w;
  QList<QPainterPath> path_list;
  QPainterPath path;
  QFont font;
  
  font.setPixelSize(100);
  font.setBold(true);
  path.addText(0, 150, font, "deepin");
  path_list.append(path);
  
  DPlatformWindowHandle::setWindowBlurAreaByWM(&w, path_list);
  
  QSurfaceFormat format = w.format();
  format.setAlphaBufferSize(8);
  
  w.setFormat(format);
  w.resize(400, 300);
  w.setAttribute(Qt::WA_TranslucentBackground);
  w.show();
  
  \endcode
  \a paths
  \return
  \note 需要对 \a widget 添加 Qt::WA_TranslucentBackground 使其背景透明
  \sa QWidget::setAttribute
 */
bool DPlatformWindowHandle::setWindowBlurAreaByWM(QWidget *widget, const QList<QPainterPath> &paths)
{
    Q_ASSERT(widget);

    return widget->windowHandle() && DPlatformHandle::setWindowBlurAreaByWM(widget->windowHandle(), paths);
}

/*!
  \brief DPlatformWindowHandle::setWindowWallpaperParaByWM.

  这只是一个重载函数，如果 widget 的 QWidget::windowHandle 为 nullptr 则返回 false，
  否则返回 setWindowWallpaperParaByWM(wiget->windowHandle(), area) 的值。示例：
  \code
  DMainWindow w;.000000
  QRect area;
  DPlatformWindowHandle::WallpaperScaleMode sMode
  DPlatformWindowHandle::WallpaperFillMode fMode
  
  area.x = 50;
  area.y = 50;
  area.width = 200;
  area.height = 200;
  area.xRadius = 10;
  area.yRaduis = 10;
  
  sMode = WallpaperScaleFlag::FollowWindow;
  fMode = WallpaperFillFlag::PreserveAspectCrop;
  
  DPlatformWindowHandle::setWindowWallpaperParaByWM(&w, area, sMode, fMode);
  
  w.resize(300, 300);
  w.setAttribute(Qt::WA_TranslucentBackground);
  w.show();
  
  \endcode
  \a widget
  \a area
  \a sMode
  \a fMode
  \return
  \note 需要对 \a widget 添加 Qt::WA_TranslucentBackground 使其背景透明
  \sa QWidget::setAttribute
 */
bool DPlatformWindowHandle::setWindowWallpaperParaByWM(QWidget *widget, const QRect &area, WallpaperScaleMode sMode, WallpaperFillMode fMode)
{
    Q_ASSERT(widget);

    return DPlatformHandle::setWindowWallpaperParaByWM(ensureWindowHandle(widget), area, sMode, fMode);
}

DWIDGET_END_NAMESPACE
