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

#include "dmainwindow.h"
#include "dplatformwindowhandle.h"
#include "dapplication.h"
#include "dtitlebar.h"
#include "dmessagemanager.h"

#include "private/dmainwindow_p.h"
#include "private/dapplication_p.h"

#include <QKeySequence>
#include <QShortcut>
#include <QWindow>
#include <QMouseEvent>
#include <QComboBox>

#ifdef Q_OS_MAC
#include "osxwindow.h"
#endif

/// shadow
#define SHADOW_COLOR_NORMAL QColor(0, 0, 0, 255 * 35/100)
#define SHADOW_COLOR_ACTIVE QColor(0, 0, 0, 255 * 60/100)

DWIDGET_BEGIN_NAMESPACE

DMainWindowPrivate::DMainWindowPrivate(DMainWindow *qq)
    : DObjectPrivate(qq)
{
    titlebar = new DTitlebar(qq);
    titlebar->setAccessibleName("DMainWindowTitlebar");
    auto noTitlebarEnabled = []{
        QFunctionPointer enableNoTitlebar = qApp->platformFunction("_d_isEnableNoTitlebar");
        bool enabled = qApp->platformName() == "dwayland" || qApp->property("_d_isDwayland").toBool();
        return enabled && enableNoTitlebar != nullptr;
    };
    if (DApplication::isDXcbPlatform() || noTitlebarEnabled()) {
        handle = new DPlatformWindowHandle(qq, qq);
        qq->setMenuWidget(titlebar);
    } else {
        qq->setMenuWidget(titlebar);
#ifdef Q_OS_MAC
        OSX::HideWindowTitlebar(qq->winId());
#else
        titlebar->setEmbedMode(true);
#endif
    }

    qq->setMinimumWidth(titlebar->minimumSizeHint().width());
    qq->setAccessibleName("DMainWindow");
}

void DMainWindowPrivate::init()
{
    D_Q(DMainWindow);

    // 默认开启标题栏阴影
    q->setTitlebarShadowEnabled(true);

    DApplication *dapp = qobject_cast<DApplication *>(qApp);
    if (dapp) {
        q->setWindowTitle(dapp->productName());
        q->setAttribute(Qt::WA_LayoutOnEntireRect, false);
        q->setAttribute(Qt::WA_ContentsMarginsRespectsSafeArea, false);
        dapp->acclimatizeVirtualKeyboard(q);
    } else {
        q->setWindowTitle(qApp->applicationDisplayName());
    }

    if (handle) {
        q->connect(handle, &DPlatformWindowHandle::borderColorChanged, q, &DMainWindow::borderColorChanged);
        q->connect(handle, &DPlatformWindowHandle::borderWidthChanged, q, &DMainWindow::borderWidthChanged);
        q->connect(handle, &DPlatformWindowHandle::clipPathChanged, q, &DMainWindow::clipPathChanged);
        q->connect(handle, &DPlatformWindowHandle::frameMarginsChanged, q, &DMainWindow::frameMarginsChanged);
        q->connect(handle, &DPlatformWindowHandle::frameMaskChanged, q, &DMainWindow::frameMaskChanged);
        q->connect(handle, &DPlatformWindowHandle::shadowColorChanged, q, &DMainWindow::shadowColorChanged);
        q->connect(handle, &DPlatformWindowHandle::shadowOffsetChanged, q, &DMainWindow::shadowOffsetChanged);
        q->connect(handle, &DPlatformWindowHandle::shadowRadiusChanged, q, &DMainWindow::shadowRadiusChanged);
        q->connect(handle, &DPlatformWindowHandle::windowRadiusChanged, q, &DMainWindow::windowRadiusChanged);
        q->connect(handle, &DPlatformWindowHandle::translucentBackgroundChanged, q, &DMainWindow::translucentBackgroundChanged);
        q->connect(handle, &DPlatformWindowHandle::enableSystemMoveChanged, q, &DMainWindow::enableSystemMoveChanged);
        q->connect(handle, &DPlatformWindowHandle::enableSystemResizeChanged, q, &DMainWindow::enableSystemResizeChanged);
        q->connect(handle, &DPlatformWindowHandle::enableBlurWindowChanged, q, &DMainWindow::enableBlurWindowChanged);
        q->connect(handle, &DPlatformWindowHandle::autoInputMaskByClipPathChanged, q, &DMainWindow::autoInputMaskByClipPathChanged);

        if (!handle->isEnabledNoTitlebar(q->windowHandle())) {
            q->connect(qApp, &QGuiApplication::focusWindowChanged, q, [q] {
                if (q->isActiveWindow())
                {
                    q->setShadowColor(SHADOW_COLOR_ACTIVE);
                } else
                {
                    q->setShadowColor(SHADOW_COLOR_NORMAL);
                }
            });
        }
    }

    if (!help) {
        help = new QShortcut(QKeySequence(Qt::Key_F1), q);
        QObject::connect(help, &QShortcut::activated,
        q, [ = ]() {
            DApplication *dapp = qobject_cast<DApplication *>(qApp);
            if (dapp) {
                dapp->handleHelpAction();
            }
        });
    }
}

void DMainWindowPrivate::updateTitleShadowGeometry()
{
    D_QC(DMainWindow);

    if (!titleShadow)
        return;

    QRect rect(0, titlebar->geometry().bottom() + 1, q->width(), titleShadow->sizeHint().height());
    titleShadow->setGeometry(rect);
    // 全凭时会隐藏窗口标题栏，因此不应该显示标题栏的阴影
    titleShadow->setVisible(!q->isFullScreen());
    titleShadow->raise();
}

/*!
  \class Dtk::Widget::DMainWindow
  \inmodule dtkwidget
  \brief The DMainWindow class provides a main application window.
  
  A main window provides a framework for building an application's user
  interface. DMainWindow has its own layout compared to QMainWindow,
  it has only title bar and content area, simpler and cleaner.
  
  Developers can provide customized title bar and content to make the
  application rich functional.
 */

/*!
  \brief DMainWindow::DMainWindow constructs an instance of DMainWindow
  \a parent is passed to QMainWindow construtor.
 */
DMainWindow::DMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , DObject(*new DMainWindowPrivate(this))
{
    d_func()->init();

    //平板模式下DMainWindow屏蔽掉最大，最小,以及关闭按钮
    if (DGuiApplicationHelper::isTabletEnvironment()) {
        setWindowFlags(windowFlags() & ~(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint));
    }
}

/*!
  \brief DMainWindow::titlebar
  \return a DTitlebar instance used by the main window.
 */
DTitlebar *DMainWindow::titlebar() const
{
    D_DC(DMainWindow);

    return d->titlebar;
}

/*!
  \brief DMainWindow::isDXcbWindow
  \return Whether this window is dxcb backended.
  
  Many features like blurred background and window clipping are supported
  only if the window is using the dxcb Qt platform plugin.
 */
bool DMainWindow::isDXcbWindow() const
{
    D_DC(DMainWindow);

    return d->handle;
}

/*!
  \property DMainWindow::windowRadius
  \brief This property holds the radius of the main window.
 */
int DMainWindow::windowRadius() const
{
    D_DC(DMainWindow);

    if (!d->handle) {
        return 0;
    }

    return d->handle->windowRadius();
}

/*!
  \property DMainWindow::borderWidth
  \brief This property holds the width of the main window's border.
 */
int DMainWindow::borderWidth() const
{
    D_DC(DMainWindow);

    if (!d->handle) {
        return 0;
    }

    return d->handle->borderWidth();
}

/*!
  \property DMainWindow::borderColor
  \brief This property holds the color of the main window's border.
 */
QColor DMainWindow::borderColor() const
{
    D_DC(DMainWindow);

    if (!d->handle) {
        return QColor();
    }

    return d->handle->borderColor();
}

/*!
  \property DMainWindow::shadowRadius
  \brief This property holds the shadow radius of the main widnow.
 */
int DMainWindow::shadowRadius() const
{
    D_DC(DMainWindow);

    if (!d->handle) {
        return 0;
    }

    return d->handle->shadowRadius();
}

/*!
  \property DMainWindow::shadowOffset
  \brief This property holds the offset applied on the window shadow.
 */
QPoint DMainWindow::shadowOffset() const
{
    D_DC(DMainWindow);

    if (!d->handle) {
        return QPoint();
    }

    return d->handle->shadowOffset();
}

/*!
  \property DMainWindow::shadowColor
  \brief This property holds the color of the window shadow.
 */
QColor DMainWindow::shadowColor() const
{
    D_DC(DMainWindow);

    if (!d->handle) {
        return QColor();
    }

    return d->handle->shadowColor();
}

/*!
  \property DMainWindow::clipPath
  \brief This property holds the custom QPainterPath to be used to clip the window.
  
  By default DMainWindow is clipped as a corner-rounded rectangle, but you can
  supply a custom QPainterPath to do custom shaped window.
  
  \sa DMainWindow::frameMask
 */
QPainterPath DMainWindow::clipPath() const
{
    D_DC(DMainWindow);

    if (!d->handle) {
        return QPainterPath();
    }

    return d->handle->clipPath();
}

/*!
  \property DMainWindow::frameMask
  \brief This property holds the mask to be applied on the window.
  
  For better clip quality, for example antialiasing, use property
  DMainWindow::clipPath instead.
 */
QRegion DMainWindow::frameMask() const
{
    D_DC(DMainWindow);

    if (!d->handle) {
        return QRegion();
    }

    return d->handle->frameMask();
}

QMargins DMainWindow::frameMargins() const
{
    D_DC(DMainWindow);

    if (!d->handle) {
        return QMargins();
    }

    return d->handle->frameMargins();
}

/*!
  \property DMainWindow::translucentBackground
  \brief This property holds whether the window has translucent background.
 */
bool DMainWindow::translucentBackground() const
{
    D_DC(DMainWindow);

    if (!d->handle) {
        return false;
    }

    return d->handle->translucentBackground();
}

/*!
  \brief DMainWindow::enableSystemResize
  \return This property holds whether the window can be resized by the user.
  
  The default value of this property is true.
  
  You can set this property to false and implement the resize polizy of this
  window by you self.
 */
bool DMainWindow::enableSystemResize() const
{
    D_DC(DMainWindow);

    if (!d->handle) {
        return false;
    }

    return d->handle->enableSystemResize();
}

/*!
  \property DMainWindow::enableSystemMove
  \brief This property holds whether the window can be moved by the user.
  
  The default value of this property is true.
  
  You can set this property to false and choose the effective area to drag and move.
 */
bool DMainWindow::enableSystemMove() const
{
    D_DC(DMainWindow);

    if (!d->handle) {
        return false;
    }

    return d->handle->enableSystemMove();
}

/*!
  \property DMainWindow::enableBlurWindow
  \brief This property holds whether the window background is blurred.
 */
bool DMainWindow::enableBlurWindow() const
{
    D_DC(DMainWindow);

    if (!d->handle) {
        return false;
    }

    return d->handle->enableBlurWindow();
}

/*!
  \property DMainWindow::autoInputMaskByClipPath
  \brief This property holds whether the user input is masked by the clip path.
  
  Sometimes you may want to handle events happening in the areas that are
  visually clipped by the setting DMainWindow::clipPath.
  
  The default value of this property is true.
 */
bool DMainWindow::autoInputMaskByClipPath() const
{
    D_DC(DMainWindow);

    if (!d->handle) {
        return false;
    }

    return d->handle->autoInputMaskByClipPath();
}


/*!
    \property DMainWindow::titlebarShadowEnabled
    \brief titleBar阴影属性.

    用于设置或者判断是否设置titleBar阴影属性.

 */
bool DMainWindow::titlebarShadowIsEnabled() const
{
    D_DC(DMainWindow);

    return d->titleShadow;
}

/*!
  \brief DMainWindow::setWindowRadius　设定窗口的圆角
  \a windowRadius　窗口的圆角值
 */
void DMainWindow::setWindowRadius(int windowRadius)
{
    D_D(DMainWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setWindowRadius(windowRadius);
}

/*!
  \brief DMainWindow::setBorderWidth 设定边框的宽度
  \a borderWidth　边框的宽度
 */
void DMainWindow::setBorderWidth(int borderWidth)
{
    D_D(DMainWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setBorderWidth(borderWidth);
}

/*!
  \brief DMainWindow::setBorderColor 设定边框的颜色
  \a borderColor　边框的颜色
 */
void DMainWindow::setBorderColor(const QColor &borderColor)
{
    D_D(DMainWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setBorderColor(borderColor);
}

/*!
  \brief DMainWindow::setShadowRadius 设定阴影区域的圆角
  \a shadowRadius　阴影区域圆角大小
 */
void DMainWindow::setShadowRadius(int shadowRadius)
{
    D_D(DMainWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setShadowRadius(shadowRadius);
}

/*!
  \brief DMainWindow::setShadowOffset 设定阴影区域的偏移距离
  \a shadowOffset　阴影区域的偏移距离
 */
void DMainWindow::setShadowOffset(const QPoint &shadowOffset)
{
    D_D(DMainWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setShadowOffset(shadowOffset);
}

/*!
  \brief DMainWindow::setShadowColor 设定阴影的颜色
  \a shadowColor　阴影的颜色
 */
void DMainWindow::setShadowColor(const QColor &shadowColor)
{
    D_D(DMainWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setShadowColor(shadowColor);
}

/*!
  \brief DMainWindow::setClipPath 设定裁剪路径
  \a clipPath　裁剪的路径
 */
void DMainWindow::setClipPath(const QPainterPath &clipPath)
{
    D_D(DMainWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setClipPath(clipPath);
}

/*!
  \brief DMainWindow::setFrameMask 设定边框的mask区域
  \a frameMask mask区域
 */
void DMainWindow::setFrameMask(const QRegion &frameMask)
{
    D_D(DMainWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setFrameMask(frameMask);
}

/*!
  \brief DMainWindow::setTranslucentBackground 设定时候擦除背景
  \a translucentBackground true擦除背景　false不擦除背景
 */
void DMainWindow::setTranslucentBackground(bool translucentBackground)
{
    D_D(DMainWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setTranslucentBackground(translucentBackground);
}

/*!
  \brief DMainWindow::setEnableSystemResize　设定是否允许系统调整窗口大小
  \a enableSystemResize　true允许系统调整　false不允许系统调整
 */
void DMainWindow::setEnableSystemResize(bool enableSystemResize)
{
    D_D(DMainWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setEnableSystemResize(enableSystemResize);
}

/*!
  \brief DMainWindow::setEnableSystemMove 设定时候允许系统移动窗口
  \a enableSystemMove　true允许移动　false不允许移动
 */
void DMainWindow::setEnableSystemMove(bool enableSystemMove)
{
    D_D(DMainWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setEnableSystemMove(enableSystemMove);
}

/*!
  \brief DMainWindow::setEnableBlurWindow　设置窗口模糊效果
  \~shinese 开启此功能请设置setAttribute(Qt::WA_TranslucentBackground)
  \a enableBlurWindow true开启模糊效果　false关闭模糊效果
 */
void DMainWindow::setEnableBlurWindow(bool enableBlurWindow)
{
    D_D(DMainWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setEnableBlurWindow(enableBlurWindow);
}

/*!
  \brief 通过裁剪区域自动设定mask.

  \a autoInputMaskByClipPath true自动设定　false不自动设定
 */
void DMainWindow::setAutoInputMaskByClipPath(bool autoInputMaskByClipPath)
{
    D_D(DMainWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setAutoInputMaskByClipPath(autoInputMaskByClipPath);
}

/*!
  \brief 发送浮动消息.

  \a icon 消息展示图标
  \a message　消息内容
 */
void DMainWindow::sendMessage(const QIcon &icon, const QString &message)
{
    DMessageManager::instance()->sendMessage(this, icon, message);
}

/*!
  \brief 发送浮动消息.

  \a message　DFloatingMessage消息
 */
void DMainWindow::sendMessage(DFloatingMessage *message)
{
    DMessageManager::instance()->sendMessage(this, message);
}

void DMainWindow::setTitlebarShadowEnabled(bool titlebarShadowEnabled)
{
    D_D(DMainWindow);

    if (static_cast<bool>(d->titleShadow) == titlebarShadowEnabled)
        return;

    if (titlebarShadowEnabled) {
        d->titleShadow = new DShadowLine(this);
        d->titleShadow->setAccessibleName("DMainWindowTitleShadowLine");
        d->titleShadow->setAttribute(Qt::WA_AlwaysStackOnTop);
        d->updateTitleShadowGeometry();
    } else {
        d->titleShadow->deleteLater();
        d->titleShadow = nullptr;
    }
}

#ifdef Q_OS_MAC
void DMainWindow::setWindowFlags(Qt::WindowFlags type)
{
    QMainWindow::setWindowFlags(type);
    OSX::HideWindowTitlebar(winId());
}
#endif

DMainWindow::DMainWindow(DMainWindowPrivate &dd, QWidget *parent)
    : QMainWindow(parent)
    , DObject(dd)
{
    d_func()->init();
}

void DMainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QWidget *compoment = childAt(event->pos());
    if (qobject_cast<QComboBox *>(compoment)) {
        return;
    }
    return QMainWindow::mouseMoveEvent(event);
}

void DMainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    D_D(DMainWindow);

    d->updateTitleShadowGeometry();

    return QMainWindow::resizeEvent(event);
}

void DMainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::WindowStateChange) {
        D_D(DMainWindow);
        d->updateTitleShadowGeometry();
    }

    return QMainWindow::changeEvent(event);
}

DWIDGET_END_NAMESPACE
