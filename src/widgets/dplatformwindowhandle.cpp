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

#include "dplatformwindowhandle.h"
#include "dapplication.h"
#include "util/dwindowmanagerhelper.h"

#include <QWindow>
#include <QWidget>
#include <QGuiApplication>
#include <QDebug>

DWIDGET_BEGIN_NAMESPACE

#define DEFINE_CONST_CHAR(Name) const char _##Name[] = "_d_" #Name

DEFINE_CONST_CHAR(useDxcb);
DEFINE_CONST_CHAR(redirectContent);
DEFINE_CONST_CHAR(netWmStates);
DEFINE_CONST_CHAR(windowRadius);
DEFINE_CONST_CHAR(borderWidth);
DEFINE_CONST_CHAR(borderColor);
DEFINE_CONST_CHAR(shadowRadius);
DEFINE_CONST_CHAR(shadowOffset);
DEFINE_CONST_CHAR(shadowColor);
DEFINE_CONST_CHAR(clipPath);
DEFINE_CONST_CHAR(frameMask);
DEFINE_CONST_CHAR(frameMargins);
DEFINE_CONST_CHAR(translucentBackground);
DEFINE_CONST_CHAR(enableSystemResize);
DEFINE_CONST_CHAR(enableSystemMove);
DEFINE_CONST_CHAR(enableBlurWindow);
DEFINE_CONST_CHAR(windowBlurAreas);
DEFINE_CONST_CHAR(windowBlurPaths);
DEFINE_CONST_CHAR(autoInputMaskByClipPath);

// functions
DEFINE_CONST_CHAR(setWmBlurWindowBackgroundArea);
DEFINE_CONST_CHAR(setWmBlurWindowBackgroundPathList);
DEFINE_CONST_CHAR(setWmBlurWindowBackgroundMaskImage);
DEFINE_CONST_CHAR(setWindowProperty);
DEFINE_CONST_CHAR(pluginVersion);
DEFINE_CONST_CHAR(disableOverrideCursor);
DEFINE_CONST_CHAR(enableDxcb);
DEFINE_CONST_CHAR(isEnableDxcb);
DEFINE_CONST_CHAR(setEnableNoTitlebar);
DEFINE_CONST_CHAR(isEnableNoTitlebar);

static void setWindowProperty(QWindow *window, const char *name, const QVariant &value)
{
    if (!window)
        return;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    static QFunctionPointer setWindowProperty = qApp->platformFunction(_setWindowProperty);
#else
    constexpr QFunctionPointer setWindowProperty = nullptr;
#endif

    if (!setWindowProperty) {
        window->setProperty(name, value);

        return;
    }

    reinterpret_cast<void(*)(QWindow *, const char *, const QVariant &)>(setWindowProperty)(window, name, value);
}

/*!
 * \~chinese \class DPlatformWindowHandle
 * \~chinese \brief 一个和Qt dxcb平台插件交互的工具类，实质性的功能皆在dxcb插件中实现，此插件目前只
 * \~chinese 支持X11平台，在其它平台上使用这个类不会有任何效果。关于dxcb：它介于Qt应用和Qt xcb平台
 * \~chinese 插件之间，通过覆写xcb插件中某些对象的虚函数来改变它的一些行为，本质上来讲是Qt xcb插件的
 * \~chinese 扩展，在X11平台上为DTK应用提供了一些改变窗口效果的功能（比如自定义窗口的边框）、其它和平
 * \~chinese 台密切相关的实现（比如修复Qt应用在X11平台的一些bug），不能脱离Qt xcb插件独立运行。dxcb
 * \~chinese 通过重载 QPlatformNativeInterface 提供接口，DPlatformWindowHandle 中使用
 * \~chinese QGuiApplication::platformFunction 调用这些接口。Application、dxcb、qt xcb 之间
 * \~chinese 的关系：
 * \~chinese \htmlonly
 * <pre style="font-family: FreeMono, Consolas, Menlo, 'Noto Mono', 'Courier New', Courier, monospace;line-height: 100%;">
 *       ┏━━━━━━━━━━━━━━━━┓
 *       ┃   Application  ┃
 *       ┗━━━━━━━━━━━━━━━━┛
 *               ⇅
 *     ┏━━━━━━━━━━━━━━━━━━━━┓
 *     ┃     dxcb plugin    ┃
 *     ┗━━━━━━━━━━━━━━━━━━━━┛
 *               ⇅
 *  ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
 *  ┃      qt xcb platform      ┃
 *  ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
 * </pre>
 * \endhtmlonly
 * \~chinese \sa \href{http://doc.qt.io/qt-5/qpa.html,QPA}
 * \~chinese \sa \href{https://github.com/linuxdeepin/qt5dxcb-plugin/,dxcb插件}
 * \~chinese \sa DApplication::loadDXcbPlugin
 * \~chinese \sa DApplication::isDXcbPlatform
 * \~chinese \sa QGuiApplication::platformNativeInterface
 * \~chinese \sa DMainWindow
 * \~chinese \sa DWindowManagerHelper
 * \~chinese \warning 使用此工具前要确保应用加载了dxcb插件
 * \~chinese \warning 因为 QGuiApplication::platformFunction 是在 Qt 5.4.0 版本引入的新接口，
 * \~chinese 所以 DPlatformWindowHandle 不支持 Qt 5.4.0 以下版本。
 */

/*!
  * \~chinese \property DPlatformWindowHandle::windowRadius
  * \~chinese \brief 窗口的圆角半径。默认情况下，窗口管理器支持混成时，圆角半径为4，否则为0，并且
  * \~chinese 会随着窗口管理器开启/关闭混成效果而变化
  * \~chinese \note 可读可写
  * \~chinese \note 窗口为半屏、全屏或最大化状态时此值不生效
  * \~chinese \warning 手动设置值后将无法再随着窗口管理器是否支持混成而自动更新边框宽度
  * \~chinese \sa DWindowManagerHelper::hasComposite
  */

/*!
  * \~chinese \property DPlatformWindowHandle::borderWidth
  * \~chinese \brief 窗口的外边框宽度。默认情况下，窗口管理器支持混成时，边框宽度为1，否则对于可以
  * \~chinese 改变大小的窗口其值为2，否则为1，并且会随着窗口管理器开启/关闭混成效果而变化
  * \~chinese \note 可读可写
  * \~chinese \warning 手动设置值后将无法再随着窗口管理器是否支持混成而自动更新边框宽度
  * \~chinese \sa DWindowManagerHelper::hasComposite
  */

/*!
  * \~chinese \property DPlatformWindowHandle::borderColor
  * \~chinese \brief 窗口外边框的颜色。默认情况下，窗口管理器支持混成时，颜色为 QColor(0, 0, 0, 255 * 0.15)，
  * \~chinese 否则为边框颜色和 #e0e0e0 的混合，并且会随着窗口管理器开启/关闭混成效果而变化
  * \~chinese \note 可读可写
  * \~chinese \sa DWindowManagerHelper::hasComposite
  */

/*!
  * \~chinese \property DPlatformWindowHandle::shadowRadius
  * \~chinese \brief 窗口的阴影半径。默认为 60
  * \~chinese \note 可读可写
  * \~chinese \note 窗口管理器不支持混成时此值无效
  * \~chinese \sa DWindowManagerHelper::hasComposite
  */

/*!
  * \~chinese \property DPlatformWindowHandle::shadowOffset
  * \~chinese \brief 窗口阴影的偏移量。默认为 QPoint(0，16)
  * \~chinese \note 可读可写
  * \~chinese \note 窗口管理器不支持混成时此值无效
  * \~chinese \sa DWindowManagerHelper::hasComposite
  */

/*!
  * \~chinese \property DPlatformWindowHandle::shadowColor
  * \~chinese \brief 窗口阴影的颜色。默认为 QColor(0, 0, 0, 255 * 0.6)
  * \~chinese \note 可读可写
  * \~chinese \note 窗口管理器不支持混成时此值无效
  * \~chinese \sa DWindowManagerHelper::hasComposite
  */

/*!
  * \~chinese \property DPlatformWindowHandle::clipPath
  * \~chinese \brief 窗口的裁剪区域。处于路径内部的区域为窗口有效区域，非有效区域内的窗口内容
  * \~chinese 将无法显示，并且无法收到鼠标和触摸事件。示例：
  * \~chinese \code
  * QWidget w;
  * QPainterPath path;
  * QFont font;
  *
  * font.setPixelSize(100);
  * path.addText(0, 150, font, "deepin");
  *
  * DPlatformWindowHandle handle(&w);
  *
  * handle.setClipPath(path);
  * w.resize(400, 200);
  * w.show();
  * \endcode
  * \~chinese \image html clip_window_demo.gif
  * \~chinese \note 可读可写
  * \~chinese \note 窗口的阴影和外边框绘制和其有效区域密切相关
  * \~chinese \warning 设置此属性后将导致 DPlatformWindowHandle::windowRadius 失效
  */

/*!
  * \~chinese \property DPlatformWindowHandle::frameMask
  * \~chinese \brief 设置 Frame Window 的遮罩，和 \a clipPath 不同的是，它的裁剪包括阴影
  * \~chinese 部分。示例：
  * \~chinese \code
  * QWidget w;
  * DPlatformWindowHandle handle(&w);
  *
  * // 为何更好的观察效果，此处将阴影改为蓝色
  * handle.setShadowColor(Qt::blue);
  * w.resize(400, 200);
  * w.show();
  * QRect frame_rect = w.rect() + handle.frameMargins();
  * frame_rect.moveTopLeft(QPoint(0, 0));
  * handle.setFrameMask(QRegion(frame_rect, QRegion::Ellipse));
  * \endcode
  * \~chinese \image html frame_mask_demo.png
  * \~chinese \note 可读可写
  * \~chinese \note 由于实现机制限制，使用此属性裁剪 Frame Window 时，无法去除边缘产生的锯齿
  */

/*!
  * \~chinese \property DPlatformWindowHandle::frameMargins
  * \~chinese \brief Sub Window 相对于 Frame Window 的边距
  * \~chinese \image html frame_margins.png
  * \~chinese \note 只读
  * \~chinese \warning 在窗口隐藏时不保证此值的正确性
  */

/*!
  * \~chinese \property DPlatformWindowHandle::translucentBackground
  * \~chinese \brief 如果此属性值为 true，则在更新窗口绘制内容之前会先清空要更新区域内的图像，
  * \~chinese 否则不清空，默认为 false
  * \~chinese \note 可读可写
  */

/*!
  * \~chinese \property DPlatformWindowHandle::enableSystemResize
  * \~chinese \brief 如果此属性值为 true，则允许外界改变窗口的大小（如使用鼠标拖拽窗口边框），
  * \~chinese 否则不允许。默认为 true
  * \~chinese \note 无论属性值是多少，Qt::Popup 和 Qt::BypassWindowManagerHint 类型的
  * \~chinese 窗口都不允许改变大小
  * \~chinese \note 可读可写
  * \~chinese \note 此属性仅仅控制 dxcb 中的行为，不会影响窗口管理器的行为
  * \~chinese \sa QWidget::setFixedSize
  * \~chinese \sa QWindow::setMinimumSize
  * \~chinese \sa QWindow::setMaximumSize
  * \~chinese \sa DWindowManagerHelper::FUNC_RESIZE
  */

/*!
  * \~chinese \property DPlatformWindowHandle::enableSystemMove
  * \~chinese \brief 如果此属性值为 ture，则允许外界移动窗口的位置（如使用鼠标拖拽移动窗口），
  * \~chinese 否则不允许。默认为 true
  * \~chinese \note 无论属性值是多少，Qt::Popup 和 Qt::BypassWindowManagerHint 类型的
  * \~chinese 窗口都不允许改变大小
  * \~chinese \note 可读可写
  * \~chinese \note 此属性仅仅控制 dxcb 中的行为，不会影响窗口管理器的行为
  * \~chinese \sa DWindowManagerHelper::FUNC_MOVE
  */

/*!
  * \~chinese \property DPlatformWindowHandle::enableBlurWindow
  * \~chinese \brief 如果此属性为 true，则窗口有效区域内的背景将呈现出模糊效果，否则无特效。
  * \~chinese 默认为 false
  * \~chinese \note 可读可写
  * \~chinese \sa DPlatformWindowHandle::setWindowBlurAreaByWM
  */

/*!
  * \~chinese \property DPlatformWindowHandle::autoInputMaskByClipPath
  * \~chinese \brief 如果此属性值为 true，则窗口可输入区域跟随其 \a clipPath 属性，否则不
  * \~chinese 跟随。默认为 true
  * \~chinese \note 可输入区域指可接收鼠标或触摸事件的区域
  * \~chinese \note 可读可写
  */

/*!
  * \~chinese \property DPlatformWindowHandle::realWindowId
  * \~chinese \brief Sub Window 的窗口 id，直接使用 QWindow::winId 或 QWidget::winId
  * \~chinese 获取到的是 Frame Window 的窗口 id
  * \~chinese \note 只读
  */

/*!
 * \~chinese \fn DPlatformWindowHandle::frameMarginsChanged
 * \~chinese \brief 信号会在 frameMargins 属性的值改变时被发送
 * \~chinese \fn DPlatformWindowHandle::windowRadiusChanged
 * \~chinese \brief 信号会在 windowRadius 属性的值改变时被发送
 * \~chinese \fn DPlatformWindowHandle::borderWidthChanged
 * \~chinese \brief 信号会在 borderWidth 属性的值改变时被发送
 * \~chinese \fn DPlatformWindowHandle::borderColorChanged
 * \~chinese \brief 信号会在 borderColor 属性的值改变时被发送
 * \~chinese \fn DPlatformWindowHandle::shadowRadiusChanged
 * \~chinese \brief 信号会在 shadowRadius 属性的值改变时被发送
 * \~chinese \fn DPlatformWindowHandle::shadowOffsetChanged
 * \~chinese \brief 信号会在 shadowOffset 属性的值改变时被发送
 * \~chinese \fn DPlatformWindowHandle::shadowColorChanged
 * \~chinese \brief 信号会在 shadowColor 属性的值改变时被发送
 * \~chinese \fn DPlatformWindowHandle::clipPathChanged
 * \~chinese \brief 信号会在 clipPath 属性的值改变时被发送
 * \~chinese \fn DPlatformWindowHandle::frameMaskChanged
 * \~chinese \brief 信号会在 frameMask 属性的值改变时被发送
 * \~chinese \fn DPlatformWindowHandle::translucentBackgroundChanged
 * \~chinese \brief 信号会在 translucentBackground 属性的值改变时被发送
 * \~chinese \fn DPlatformWindowHandle::enableSystemResizeChanged
 * \~chinese \brief 信号会在 enableSystemResize 属性的值改变时被发送
 * \~chinese \fn DPlatformWindowHandle::enableSystemMoveChanged
 * \~chinese \brief 信号会在 enableSystemMove 属性的值改变时被发送
 * \~chinese \fn DPlatformWindowHandle::enableBlurWindowChanged
 * \~chinese \brief 信号会在 enableBlurWindow 属性的值改变时被发送
 * \~chinese \fn DPlatformWindowHandle::autoInputMaskByClipPathChanged
 * \~chinese \brief 信号会在 autoInputMaskByClipPath 属性的值改变时被发送
 */

/*!
 * \~chinese \struct DPlatformWindowHandle::WMBlurArea
 * \~chinese \brief 描述窗口背景模糊区域的数据结构，包含位置、大小、圆角半径等信息
 * \var DPlatformWindowHandle::WMBlurArea::x
 * 水平方向的坐标
 * \var DPlatformWindowHandle::WMBlurArea::y
 * 竖直方向的坐标
 * \var DPlatformWindowHandle::WMBlurArea::width
 * 区域的宽度
 * \var DPlatformWindowHandle::WMBlurArea::height
 * 区域的高度
 * \var DPlatformWindowHandle::WMBlurArea::xRadius
 * 水平方向的圆角半径
 * \var DPlatformWindowHandle::WMBlurArea::yRaduis
 * 竖直方向的圆角半径
*/

/*!
 * \~chinese \brief DPlatformWindowHandle::DPlatformWindowHandle
 * \~chinese 将 \a window 对象传递给 enableDXcbForWindow
 * \~chinese \param window 要开启DTK风格的主窗口
 * \~chinese \param parent DPlatformWindowHandle 对象的父对象
 * \~chinese \sa DPlatformWindowHandle::enableDXcbForWindow(QWindow *)
 */
DPlatformWindowHandle::DPlatformWindowHandle(QWindow *window, QObject *parent)
    : QObject(parent)
    , m_window(window)
{
    enableDXcbForWindow(window);

    window->installEventFilter(this);
}

/*!
 * \~chinese \brief DPlatformWindowHandle::DPlatformWindowHandle
 * \~chinese 同 DPlatformWindowHandle::DPlatformWindowHandle(QWindow *window, QObject *parent)，
 * \~chinese 这只是针对 QWidget 对象的重载，将 \a widget 对象传递给 enableDXcbForWindow
 * \~chinese \param widget 要开启DTK风格的主窗口
 * \~chinese \param parent DPlatformWindowHandle 对象的父对象
 * \~chinese \sa DPlatformWindowHandle::enableDXcbForWindow(QWidget *)
 */
DPlatformWindowHandle::DPlatformWindowHandle(QWidget *widget, QObject *parent)
    : QObject(parent)
    , m_window(Q_NULLPTR)
{
    enableDXcbForWindow(widget);

    m_window = widget->window()->windowHandle();

    if (m_window)
        m_window->installEventFilter(this);
}

/*!
 * \~chinese \brief DPlatformWindowHandle::pluginVersion
 * \~chinese \return 返回dxcb插件的版本
 * \~chinese \note 在旧版dxcb插件中未实现获取版本的接口，将会返回一个空的 QString 对象
 */
QString DPlatformWindowHandle::pluginVersion()
{
    QFunctionPointer pv = 0;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    pv = qApp->platformFunction(_pluginVersion);
#endif

    if (Q_UNLIKELY(!pv))
        return QString();

    return reinterpret_cast<QString(*)()>(pv)();
}

static QWindow *ensureWindowHandle(QWidget *widget)
{
    QWidget *window = widget->window();
    QWindow *handle = window->windowHandle();

    if (!handle) {
        bool save_flag = qApp->testAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
        qApp->setAttribute(Qt::AA_DontCreateNativeWidgetSiblings, true);
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
 * \~chinese \brief DPlatformWindowHandle::enableDXcbForWindow
 * \~chinese 这是一个针对 QWidget 类型主窗口的重载函数，此函数将设置 QWidget 对象的 Qt::WA_NativeWindow
 * \~chinese 标志，之后将其 QWidget::windowHandle 对象作为参数传递给 enableDXcbForWindow(QWindow *)
 * \~chinese \param widget
 * \~chinese \sa DPlatformWindowHandle::enableDXcbForWindow(QWindow *window)
 */
void DPlatformWindowHandle::enableDXcbForWindow(QWidget *widget)
{
    if (!DApplication::isDXcbPlatform())
        return;

    QWindow *handle = ensureWindowHandle(widget);

    enableDXcbForWindow(handle);
}

/*!
 * \~chinese \brief DPlatformWindowHandle::enableDXcbForWindow
 * \~chinese 将 QWindow 的窗口装饰设置为 DTK 风格，这将使用 Qt::FramelessWindowHint 去除本地窗口管理器
 * \~chinese 给窗口附加的边框修饰以及窗口阴影效果，并且，会创建一个对应的本地窗口（在X11平台就是指X Window）
 * \~chinese 作为此窗口的父窗口，父窗口（Frame Window）中将根据子窗口（Sub Window）的有效区域绘制阴影和边
 * \~chinese 框等效果，默认情况下，子窗口的有效区域为一个圆角矩形，结构如下：
 * \~chinese \htmlonly
 * <pre style="font-family: FreeMono, Consolas, Menlo, 'Noto Mono', 'Courier New', Courier, monospace;line-height: 100%;">
 * ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
 * ┃    Frame Window             ┃
 * ┃                             ┃
 * ┃                             ┃
 * ┃     ╭┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅╮     ┃
 * ┃     ┋    Sub Window   ┋     ┃
 * ┃     ┋                 ┋     ┃
 * ┃     ┋                 ┋     ┃
 * ┃     ┋                 ┋     ┃
 * ┃     ┋                 ┋     ┃
 * ┃     ┋                 ┋     ┃
 * ┃     ┋                 ┋     ┃
 * ┃     ┋                 ┋     ┃
 * ┃     ┋                 ┋     ┃
 * ┃     ╰┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅╯     ┃
 * ┃                             ┃
 * ┃                             ┃
 * ┃                             ┃
 * ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
 * </pre>
 * \endhtmlonly
 *
 * 但是，如果窗口管理器自身支持隐藏窗口标题栏，则此方法将优先调用 enableNoTitlebarForWindow 实现同样的效果。
 *
 * 例子：
 * \~chinese \code
 * QWidget w1;
 *
 * w1.setWindowTitle("使用系统边框的窗口");
 * w1.show();
 *
 * DMainWindow w2;
 * QWidget w3;
 *
 * w2.titlebar()->setTitle("使用DTK风格边框带标题栏的窗口");
 * w3.setWindowTitle("使用DTK风格边框没有标题栏的窗口");
 * w2.show();
 *
 * DPlatformWindowHandle::enableDXcbForWindow(&w3);
 * w3.show(); // 因为这个窗口没有标题栏，所以不会显示窗口标题
 *
 * \endcode
 * \~chinese \image html dtk_and_system_window.jpeg
 * \~chinese 开启了dxcb的窗口，在窗口外边缘10像素的范围按下鼠标左键可以触发改变窗口大小的行为，
 * \~chinese 而且会自动将鼠标吸附到对应的窗口边缘，增强了拖拽改变窗口大小的体验。效果：
 * \~chinese \image html dtk_window_cursor_effect.gif
 * \~chinese 另外，所有到达主窗口的鼠标移动事件如果没有调用 QEvent::accepted ，则会触发主窗
 * \~chinese 口的移动效果，默认情况下，一个没有子控件的DTK窗口，如果没有重写 QWidget::mouseMoveEvent ，
 * \~chinese 则使用鼠标左键在窗口的任意地方按住并移动都会触发移动窗口的动作。如：
 * \~chinese \code
 * class Window : public QWidget
 * {
 * public:
 *    explicit Window() {
 *
 *    }

 * protected:
 *    void mouseMoveEvent(QMouseEvent *event) override {
 *        event->accept();
 *    }
 * };
 * \endcode
 * \~chinese \code
 * Window w;
 * DPlatformWindowHandle::enableDXcbForWindow(&w);
 * w.show();
 * \endcode
 * \~chinese 将无法使用鼠标移动窗口w
 *
 * \~chinese 窗口管理器（如X11平台上的Window Manager）是否支持混成会影响dxcb插件对窗口添加的默认装饰。
 * \~chinese \note 在 Deepin 桌面环境中，打开窗口特效则支持混成，关闭窗口特效则不支持混成
 *
 * \~chinese 支持混成：
 * \~chinese \image html enable_composite.png
 * \~chinese 不支持混成：
 * \~chinese \image html disable_composite.png
 * \~chinese 并且，在不支持混成的窗口管理器中，上述“窗口边缘的鼠标吸附”效果也会被禁用。可以使用
 * \~chinese DWindowManagerHelper::hasComposite 或 QX11Info::isCompositingManagerRunning
 * \~chinese 判断当前运行的窗口管理器是否支持混成。
 * \~chinese \param window
 * \~chinese \sa DPlatformWindowHandle::setEnableNoTitlebarForWindow
 */
void DPlatformWindowHandle::enableDXcbForWindow(QWindow *window)
{
    // 优先使用窗口管理器中实现的no titlebar接口实现自定义窗口修饰器的效果
    if (setEnableNoTitlebarForWindow(window, true)) {
        return;
    }

    if (!DApplication::isDXcbPlatform())
        return;

    QFunctionPointer enable_dxcb = nullptr;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    enable_dxcb = qApp->platformFunction(_enableDxcb);
#endif

    if (enable_dxcb) {
        (*reinterpret_cast<bool(*)(QWindow*)>(enable_dxcb))(window);
    } else if (window->handle()) {
        Q_ASSERT_X(window->property(_useDxcb).toBool(), "DPlatformWindowHandler:",
                   "Must be called before window handle has been created. See also QWindow::handle()");
    } else {
        window->setProperty(_useDxcb, true);
    }
}

/*!
 * \~chinese \brief DPlatformWindowHandle::enableDXcbForWindow
 * \~chinese 这只是一个重载函数
 * \~chinese \param widget
 * \~chinese \param redirectContent
 * \~chinese \sa DPlatformWindowHandle::enableDXcbForWindow(QWidget*)
 * \~chinese \sa DPlatformWindowHandle::enableDXcbForWindow(QWindow *, bool)
 */
void DPlatformWindowHandle::enableDXcbForWindow(QWidget *widget, bool redirectContent)
{
    enableDXcbForWindow(ensureWindowHandle(widget), redirectContent);
}

/*!
 * \~chinese \brief DPlatformWindowHandle::enableDXcbForWindow
 * \~chinese 功能上和 DPlatformWindowHandle::enableDXcbForWindow(QWindow *) 一致
 * \~chinese \param window
 * \~chinese \param redirectContent 如果值为 true，Sub Window 将不可见，且它的绘制内容会
 * \~chinese 被合成到外层的 Frame Window（它的父窗口），否则 Sub Window 和 Frame Window
 * \~chinese 会分开绘制和显示。默认情况下只需要使用 DPlatformWindowHandle::enableDXcbForWindow(QWindow *)，
 * \~chinese dxcb插件中会自动根据窗口渲染类型选择使用更合适的实现方式，使用 OpenGL 渲染的窗口将开启
 * \~chinese redirectContent 模式。
 * \~chinese \note 如果窗口内嵌入了其它的本地窗口（如X11平台的X Window），默认情况下，这个窗口
 * \~chinese 绘制的内容不受dxcb插件的控制，它的绘制内容可能会超过 Sub Window 的有效区域，这种
 * \~chinese 情况下，应该使用此接口，并将 redirectContent 指定为 true。
 * \~chinese \sa X11 Damage: https://www.x.org/releases/X11R7.5/doc/damageproto/damageproto.txt
 */
void DPlatformWindowHandle::enableDXcbForWindow(QWindow *window, bool redirectContent)
{
    window->setProperty(_redirectContent, redirectContent);

    enableDXcbForWindow(window);
}

/*!
 * \~chinese \brief DPlatformWindowHandle::isEnabledDXcb
 * \~chinese 这只是一个重载函数，如果 widget 的 QWidget::windowHandle 为 nullptr 则返回 false，
 * \~chinese 否则返回 isEnabledDXcb(widget->windowHandle()) 的值
 * \~chinese \param widget
 * \~chinese \return
 */
bool DPlatformWindowHandle::isEnabledDXcb(const QWidget *widget)
{
    if (QWindow *handle = widget->windowHandle()) {
        return isEnabledDXcb(handle);
    }

    return false;
}

/*!
 * \~chinese \brief DPlatformWindowHandle::isEnabledDXcb
 * \~chinese \param widget
 * \~chinese \return 如果窗口开启了DTK风格的窗口修饰则返回 true，否则返回 false
 * \~chinese \sa DPlatformWindowHandle::isEnableNoTitlebar
 */
bool DPlatformWindowHandle::isEnabledDXcb(const QWindow *window)
{
    if (isEnableNoTitlebar(window))
        return true;

    QFunctionPointer is_enable_dxcb = nullptr;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    is_enable_dxcb = qApp->platformFunction(_isEnableDxcb);
#endif

    if (is_enable_dxcb) {
        return (*reinterpret_cast<bool(*)(const QWindow*)>(is_enable_dxcb))(window);
    }

    return window->property(_useDxcb).toBool();
}

/*!
 * \~chinese \brief DPlatformWindowHandle::setEnableNoTitlebarForWindow
 * \~chinese 使用窗口管理器提供的方式隐藏窗口的标题栏，目前已适配 DDE KWin 窗管，在窗口管理器支持的前提下，
 * \~chinese 此方法将通过设置窗口属性 _DEEPIN_SCISSOR_WINDOW 的值为 1 来开启无标题栏效果。
 * \~chinese \param window
 * \~chinese \param enable
 * \~chinese \sa DPlatformWindowHandle::enableDXcbForWindow(QWindow *)
 * \~chinese \sa DWindowManagerHelper::hasNoTitlebar
 */
bool DPlatformWindowHandle::setEnableNoTitlebarForWindow(QWindow *window, bool enable)
{
    if (!DApplication::isDXcbPlatform())
        return false;

    QFunctionPointer enable_no_titlear = nullptr;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    enable_no_titlear = qApp->platformFunction(_setEnableNoTitlebar);
#endif

    if (enable_no_titlear) {
        return (*reinterpret_cast<bool(*)(QWindow*, bool)>(enable_no_titlear))(window, enable);
    }

    return false;
}

/*!
 * \~chinese \brief DPlatformWindowHandle::isEnableNoTitlebar
 * \~chinese \param window
 * \~chinese \return 如果窗口使用窗管提供的方式隐藏了标题栏则返回 true，否则返回 false
 * \~chinese \sa DPlatformWindowHandle::isEnabledDXcb
 */
bool DPlatformWindowHandle::isEnableNoTitlebar(const QWindow *window)
{
    QFunctionPointer is_enable_no_titlebar = nullptr;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    is_enable_no_titlebar = qApp->platformFunction(_isEnableNoTitlebar);
#endif

    if (is_enable_no_titlebar) {
        return (*reinterpret_cast<bool(*)(const QWindow*)>(is_enable_no_titlebar))(window);
    }

    return false;
}

/*!
 * \~chinese \brief DPlatformWindowHandle::setWindowBlurAreaByWM
 * \~chinese 这只是一个重载函数，如果 widget 的 QWidget::windowHandle 为 nullptr 则返回 false，
 * \~chinese 否则返回 setWindowBlurAreaByWM(wiget->windowHandle(), area) 的值。示例：
 * \~chinese \code
 * DMainWindow w;
 * QVector<DPlatformWindowHandle::WMBlurArea> area_list;
 * DPlatformWindowHandle::WMBlurArea area;
 *
 * area.x = 50;
 * area.y = 50;
 * area.width = 200;
 * area.height = 200;
 * area.xRadius = 10;
 * area.yRaduis = 10;
 * area_list.append(area);
 *
 * DPlatformWindowHandle::setWindowBlurAreaByWM(&w, area_list);
 *
 * w.resize(300, 300);
 * w.setAttribute(Qt::WA_TranslucentBackground);
 * w.show();
 *
 * \endcode
 * \~chinese \image html blur_widget_demo1.png
 * \~chinese \param widget
 * \~chinese \param area
 * \~chinese \return
 * \~chinese \note 需要对 \a widget 添加 Qt::WA_TranslucentBackground 使其背景透明
 * \~chinese \sa QWidget::setAttribute
 */
bool DPlatformWindowHandle::setWindowBlurAreaByWM(QWidget *widget, const QVector<DPlatformWindowHandle::WMBlurArea> &area)
{
    Q_ASSERT(widget);

    return widget->windowHandle() && setWindowBlurAreaByWM(widget->windowHandle(), area);
}

/*!
 * \~chinese \brief DPlatformWindowHandle::setWindowBlurAreaByWM
 * \~chinese 这只是一个重载函数，如果 widget 的 QWidget::windowHandle 为 nullptr 则返回 false，
 * \~chinese 否则返回 setWindowBlurAreaByWM(wiget->windowHandle(), paths) 的值。示例：
 * \~chinese \code
 * DMainWindow w;
 * QList<QPainterPath> path_list;
 * QPainterPath path;
 * QFont font;
 *
 * font.setPixelSize(100);
 * font.setBold(true);
 * path.addText(0, 150, font, "deepin");
 * path_list.append(path);
 *
 * DPlatformWindowHandle::setWindowBlurAreaByWM(&w, path_list);
 *
 * QSurfaceFormat format = w.format();
 * format.setAlphaBufferSize(8);
 *
 * w.setFormat(format);
 * w.resize(400, 300);
 * w.setAttribute(Qt::WA_TranslucentBackground);
 * w.show();
 *
 * \endcode
 * \~chinese \image html blur_widget_demo2.png
 * \~chinese \param widget
 * \~chinese \param area
 * \~chinese \return
 * \~chinese \note 需要对 \a widget 添加 Qt::WA_TranslucentBackground 使其背景透明
 * \~chinese \sa QWidget::setAttribute
 */
bool DPlatformWindowHandle::setWindowBlurAreaByWM(QWidget *widget, const QList<QPainterPath> &paths)
{
    Q_ASSERT(widget);

    return widget->windowHandle() && setWindowBlurAreaByWM(widget->windowHandle(), paths);
}

inline DPlatformWindowHandle::WMBlurArea operator *(const DPlatformWindowHandle::WMBlurArea &area, qreal scale)
{
    if (qFuzzyCompare(scale, 1.0))
        return area;

    DPlatformWindowHandle::WMBlurArea new_area;

    new_area.x = qRound64(area.x * scale);
    new_area.y = qRound64(area.y * scale);
    new_area.width = qRound64(area.width * scale);
    new_area.height = qRound64(area.height * scale);
    new_area.xRadius = qRound64(area.xRadius * scale);
    new_area.yRaduis = qRound64(area.yRaduis * scale);

    return new_area;
}

/*!
 * \~chinese \brief DPlatformWindowHandle::setWindowBlurAreaByWM
 * \~chinese 设置窗口背景的模糊区域，示例：
 * \~chinese \code
 * QWindow w;
 * QVector<DPlatformWindowHandle::WMBlurArea> area_list;
 * DPlatformWindowHandle::WMBlurArea area;
 *
 * area.x = 50;
 * area.y = 50;
 * area.width = 200;
 * area.height = 200;
 * area.xRadius = 10;
 * area.yRaduis = 10;
 * area_list.append(area);
 *
 * DPlatformWindowHandle::setWindowBlurAreaByWM(&w, area_list);
 *
 * QSurfaceFormat format = w.format();
 * format.setAlphaBufferSize(8);
 *
 * w.setFormat(format);
 * w.resize(300, 300);
 * w.show();
 *
 * \endcode
 * \~chinese \image html blur_window_demo1.png
 * \~chinese \param window 目标窗口对象
 * \~chinese \param area 模糊区域，此区域范围内的窗口背景将填充为窗口后面内容模糊之后的图像
 * \~chinese \return 如果设置成功则返回 true，否则返回 false
 * \~chinese \note 对于需要显示模糊背景的窗口，需要将其 QSurfaceFormat 的 alpha 通道设置为8
 * \~chinese \note 调用此接口设置窗口背景模糊区域后将覆盖之前所设置的区域，包括调用
 * \~chinese setWindowBlurAreaByWM(QWindow *, const QList<QPainterPath> &)
 * \~chinese 所设置的区域
 * \~chinees \note 建议使用 DBlurEffectWidget 实现窗口背景模糊效果
 * \~chinese \note 此功能依赖于窗口管理器的实现，目前仅支持 deepin-wm 和 kwin 这两个窗口管理器
 * \~chinese \sa DBlurEffectWidget
 * \~chinese \sa QSurfaceFormat::setAlphaBufferSize
 * \~chinese \sa QWindow::setFormat
 * \~chinese \sa DWindowManagerHelper::hasBlurWindow
 * \~chinese \sa DPlatformWindowHandle::setWindowBlurAreaByWM(QWindow *, const QList<QPainterPath> &)
 */
bool DPlatformWindowHandle::setWindowBlurAreaByWM(QWindow *window, const QVector<DPlatformWindowHandle::WMBlurArea> &area)
{
    if (!window) {
        return false;
    }

    if (isEnabledDXcb(window)) {
        setWindowProperty(window, _windowBlurAreas, QVariant::fromValue(*(reinterpret_cast<const QVector<quint32>*>(&area))));

        return true;
    }

    QFunctionPointer setWmBlurWindowBackgroundArea = Q_NULLPTR;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    setWmBlurWindowBackgroundArea = qApp->platformFunction(_setWmBlurWindowBackgroundArea);
#endif

    if (!setWmBlurWindowBackgroundArea) {
        qWarning("setWindowBlurAreaByWM is not support");

        return false;
    }

    QSurfaceFormat format = window->format();

    format.setAlphaBufferSize(8);
    window->setFormat(format);

    const qreal device_ratio = window->devicePixelRatio();

    if (qFuzzyCompare(device_ratio, 1.0)) {
        return reinterpret_cast<bool(*)(const quint32, const QVector<WMBlurArea>&)>(setWmBlurWindowBackgroundArea)(window->winId(), area);
    }

    QVector<WMBlurArea> new_areas;

    new_areas.reserve(area.size());

    for (const WMBlurArea &a : area) {
        new_areas.append(a * device_ratio);
    }

    return reinterpret_cast<bool(*)(const quint32, const QVector<WMBlurArea>&)>(setWmBlurWindowBackgroundArea)(window->winId(), new_areas);
}

inline QPainterPath operator *(const QPainterPath &path, qreal scale)
{
    if (qFuzzyCompare(1.0, scale))
        return path;

    QPainterPath new_path = path;

    for (int i = 0; i < path.elementCount(); ++i) {
        const QPainterPath::Element &e = path.elementAt(i);

        new_path.setElementPositionAt(i, qRound(e.x * scale), qRound(e.y * scale));
    }

    return new_path;
}

/*!
 * \~chinese \brief DPlatformWindowHandle::setWindowBlurAreaByWM
 * \~chinese 设置窗口背景的模糊区域，使用 QPainterPath 描述模糊区域，使用起来更加的灵活，可以
 * \~chinese 实现任何形状，但是性能要低于使用 QVector<DPlatformWindowHandle::WMBlurArea>
 * \~chinese 描述模糊区域。示例：
 * \~chinese \code
 * QWindow w;
 * QList<QPainterPath> path_list;
 * QPainterPath path;
 * QFont font;
 *
 * font.setPixelSize(100);
 * font.setBold(true);
 * path.addText(0, 150, font, "deepin");
 * path_list.append(path);
 *
 * DPlatformWindowHandle::setWindowBlurAreaByWM(&w, path_list);
 *
 * QSurfaceFormat format = w.format();
 * format.setAlphaBufferSize(8);
 *
 * w.setFormat(format);
 * w.resize(300, 300);
 * w.show();
 *
 * \endcode
 * \~chinese \image html blur_window_demo2.png
 * \~chinese \param window 目标窗口对象
 * \~chinese \param paths 模糊区域，此区域范围内的窗口背景将填充为窗口后面内容模糊之后的图像
 * \~chinese \return 如果设置成功则返回 true，否则返回 false
 * \~chinese \note 调用此接口设置窗口背景模糊区域后将覆盖之前所设置的区域，包括调用
 * \~chinese setWindowBlurAreaByWM(QWindow *, QVector<DPlatformWindowHandle::WMBlurArea> &)
 * \~chinese 设置的窗口背景模糊路径
 * \~chinese \note 对于需要显示模糊背景的窗口，需要将其 QSurfaceFormat 的 alpha 通道设置为8
 * \~chinees \note 建议使用 DBlurEffectWidget 实现窗口背景模糊效果
 * \~chinese \note 此功能依赖于窗口管理器的实现，目前仅支持 deepin-wm 和 kwin 这两个窗口管理器
 * \~chinese \warning setWindowBlurAreaByWM(QWindow *, QVector<DPlatformWindowHandle::WMBlurArea> &)
 * \~chinese 能满足需求请不要使用此接口
 * \~chinese \sa DBlurEffectWidget
 * \~chinese \sa QSurfaceFormat::setAlphaBufferSize
 * \~chinese \sa QWindow::setFormat
 * \~chinese \sa DWindowManagerHelper::hasBlurWindow
 * \~chinese \sa DPlatformWindowHandle::setWindowBlurAreaByWM(QWindow *, QVector<DPlatformWindowHandle::WMBlurArea> &)
 */
bool DPlatformWindowHandle::setWindowBlurAreaByWM(QWindow *window, const QList<QPainterPath> &paths)
{
    if (!window) {
        return false;
    }

    if (isEnabledDXcb(window)) {
        setWindowProperty(window, _windowBlurPaths, QVariant::fromValue(paths));

        return true;
    }

    QFunctionPointer setWmBlurWindowBackgroundPathList = Q_NULLPTR;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    setWmBlurWindowBackgroundPathList = qApp->platformFunction(_setWmBlurWindowBackgroundPathList);
#endif

    if (!setWmBlurWindowBackgroundPathList) {
        qWarning("setWindowBlurAreaByWM is not support");

        return false;
    }

    QSurfaceFormat format = window->format();

    format.setAlphaBufferSize(8);
    window->setFormat(format);

    const qreal device_ratio = window->devicePixelRatio();

    if (qFuzzyCompare(device_ratio, 1.0)) {
        return reinterpret_cast<bool(*)(const quint32, const QList<QPainterPath>&)>(setWmBlurWindowBackgroundPathList)(window->winId(), paths);
    }

    QList<QPainterPath> new_paths;

    new_paths.reserve(paths.size());

    for (const QPainterPath &p : paths) {
        new_paths.append(p * device_ratio);
    }

    return reinterpret_cast<bool(*)(const quint32, const QList<QPainterPath>&)>(setWmBlurWindowBackgroundPathList)(window->winId(), new_paths);
}

/*!
 * \~chinese \brief DPlatformWindowHandle::connectWindowManagerChangedSignal
 * \~chinese 将窗口管理器变化的信号链接到 object 对象的 slot 槽，建议使用　DWindowManager::windowManagerChanged
 * \~chinese \param object
 * \~chinese \param slot
 * \~chinese \return 如果链接成功则返回 true，否则返回 false
 * \~chinese \sa DWindowManager::windowManagerChanged
 */
bool DPlatformWindowHandle::connectWindowManagerChangedSignal(QObject *object, std::function<void ()> slot)
{
    if (object) {
        return QObject::connect(DWindowManagerHelper::instance(), &DWindowManagerHelper::windowManagerChanged, object, slot);
    }

    return QObject::connect(DWindowManagerHelper::instance(), &DWindowManagerHelper::windowManagerChanged, slot);
}

/*!
 * \~chinese \brief DPlatformWindowHandle::connectHasBlurWindowChanged
 * \~chinese 将窗口管理器是否支持背景模糊的信号链接到 object 对象的 slot 槽，建议使用
 * \~chinese DWindowManager::hasBlurWindowChanged
 * \~chinese \param object
 * \~chinese \param slot
 * \~chinese \return 如果链接成功则返回 true，否则返回 false
 * \~chinese \sa DWindowManager::hasBlurWindowChanged
 */
bool DPlatformWindowHandle::connectHasBlurWindowChanged(QObject *object, std::function<void ()> slot)
{
    if (object) {
        return QObject::connect(DWindowManagerHelper::instance(), &DWindowManagerHelper::hasBlurWindowChanged, object, slot);
    }

    return QObject::connect(DWindowManagerHelper::instance(), &DWindowManagerHelper::hasBlurWindowChanged, slot);
}

/*!
 * \~chinese \brief DPlatformWindowHandle::setWindowBlurAreaByWM
 * \~chinese 这只是一个重载的函数，将调用 setWindowBlurAreaByWM(QWindow *, const QVector<DPlatformWindowHandle::WMBlurArea> &)
 * \~chinese 并将构造对象时传递的主窗口当做第一个参数
 * \~chinese \param area
 * \~chinese \return
 * \~chinese \sa DPlatformWindowHandle::setWindowBlurAreaByWM(QWindow *, const QVector<DPlatformWindowHandle::WMBlurArea> &)
 */
bool DPlatformWindowHandle::setWindowBlurAreaByWM(const QVector<DPlatformWindowHandle::WMBlurArea> &area)
{
    return setWindowBlurAreaByWM(m_window, area);
}

/*!
 * \~chinese \brief DPlatformWindowHandle::setWindowBlurAreaByWM
 * \~chinese 这只是一个重载的函数，将调用 setWindowBlurAreaByWM(QWindow *, const QList<QPainterPath> &)
 * \~chinese 并将构造对象时传递的主窗口当做第一个参数
 * \~chinese \param area
 * \~chinese \return
 * \~chinese \sa DPlatformWindowHandle::setWindowBlurAreaByWM(QWindow *, const QList<QPainterPath> &)
 */
bool DPlatformWindowHandle::setWindowBlurAreaByWM(const QList<QPainterPath> &paths)
{
    return setWindowBlurAreaByWM(m_window, paths);
}

/*!
 * \~chinese \brief DPlatformWindowHandle::setDisableWindowOverrideCursor
 * \~chinese 如果 \a disable 为 true，则禁止窗口 \a window 改变光标样式，否则允许改变光标样式。
 * \~chinese 窗口被禁止改变光标样式后，使用 QWindow::setCursor 将不会产生任何效果。
 * \~chinese \param window
 * \~chinese \param disable
 */
void DPlatformWindowHandle::setDisableWindowOverrideCursor(QWindow *window, bool disable)
{
    window->setProperty(_disableOverrideCursor, disable);
}

int DPlatformWindowHandle::windowRadius() const
{
    return m_window->property(_windowRadius).toInt();
}

int DPlatformWindowHandle::borderWidth() const
{
    return m_window->property(_borderWidth).toInt();
}

QColor DPlatformWindowHandle::borderColor() const
{
    return qvariant_cast<QColor>(m_window->property(_borderColor));
}

int DPlatformWindowHandle::shadowRadius() const
{
    return m_window->property(_shadowRadius).toInt();
}

QPoint DPlatformWindowHandle::shadowOffset() const
{
    return m_window->property(_shadowOffset).toPoint();
}

QColor DPlatformWindowHandle::shadowColor() const
{
    return qvariant_cast<QColor>(m_window->property(_shadowColor));
}

QPainterPath DPlatformWindowHandle::clipPath() const
{
    return qvariant_cast<QPainterPath>(m_window->property(_clipPath));
}

QRegion DPlatformWindowHandle::frameMask() const
{
    return qvariant_cast<QRegion>(m_window->property(_frameMask));
}

QMargins DPlatformWindowHandle::frameMargins() const
{
    return qvariant_cast<QMargins>(m_window->property(_frameMargins));
}

bool DPlatformWindowHandle::translucentBackground() const
{
    return m_window->property(_translucentBackground).toBool();
}

bool DPlatformWindowHandle::enableSystemResize() const
{
    return m_window->property(_enableSystemResize).toBool();
}

bool DPlatformWindowHandle::enableSystemMove() const
{
    return m_window->property(_enableSystemMove).toBool();
}

bool DPlatformWindowHandle::enableBlurWindow() const
{
    return m_window->property(_enableBlurWindow).toBool();
}

bool DPlatformWindowHandle::autoInputMaskByClipPath() const
{
    return m_window->property(_autoInputMaskByClipPath).toBool();
}

WId DPlatformWindowHandle::realWindowId() const
{
    return qvariant_cast<WId>(m_window->property("_d_real_content_window"));
}

void DPlatformWindowHandle::setWindowRadius(int windowRadius)
{
    setWindowProperty(m_window, _windowRadius, windowRadius);
}

void DPlatformWindowHandle::setBorderWidth(int borderWidth)
{
    setWindowProperty(m_window, _borderWidth, borderWidth);
}

void DPlatformWindowHandle::setBorderColor(const QColor &borderColor)
{
    setWindowProperty(m_window, _borderColor, QVariant::fromValue(borderColor));
}

void DPlatformWindowHandle::setShadowRadius(int shadowRadius)
{
    setWindowProperty(m_window, _shadowRadius, shadowRadius);
}

void DPlatformWindowHandle::setShadowOffset(const QPoint &shadowOffset)
{
    setWindowProperty(m_window, _shadowOffset, shadowOffset);
}

void DPlatformWindowHandle::setShadowColor(const QColor &shadowColor)
{
    setWindowProperty(m_window, _shadowColor, QVariant::fromValue(shadowColor));
}

void DPlatformWindowHandle::setClipPath(const QPainterPath &clipPath)
{
    setWindowProperty(m_window, _clipPath, QVariant::fromValue(clipPath));
}

void DPlatformWindowHandle::setFrameMask(const QRegion &frameMask)
{
    setWindowProperty(m_window, _frameMask, QVariant::fromValue(frameMask));
}

void DPlatformWindowHandle::setTranslucentBackground(bool translucentBackground)
{
    setWindowProperty(m_window, _translucentBackground, translucentBackground);
}

void DPlatformWindowHandle::setEnableSystemResize(bool enableSystemResize)
{
    setWindowProperty(m_window, _enableSystemResize, enableSystemResize);
}

void DPlatformWindowHandle::setEnableSystemMove(bool enableSystemMove)
{
    setWindowProperty(m_window, _enableSystemMove, enableSystemMove);
}

void DPlatformWindowHandle::setEnableBlurWindow(bool enableBlurWindow)
{
    setWindowProperty(m_window, _enableBlurWindow, enableBlurWindow);
}

void DPlatformWindowHandle::setAutoInputMaskByClipPath(bool autoInputMaskByClipPath)
{
    setWindowProperty(m_window, _autoInputMaskByClipPath, autoInputMaskByClipPath);
}

bool DPlatformWindowHandle::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_window) {
        if (event->type() == QEvent::DynamicPropertyChange) {
            QDynamicPropertyChangeEvent *e = static_cast<QDynamicPropertyChangeEvent *>(event);

            if (e->propertyName() == _windowRadius) {
                Q_EMIT windowRadiusChanged();
            } else if (e->propertyName() == _borderWidth) {
                Q_EMIT borderWidthChanged();
            } else if (e->propertyName() == _borderColor) {
                Q_EMIT borderColorChanged();
            } else if (e->propertyName() == _shadowRadius) {
                Q_EMIT shadowRadiusChanged();
            } else if (e->propertyName() == _shadowOffset) {
                Q_EMIT shadowOffsetChanged();
            } else if (e->propertyName() == _shadowColor) {
                Q_EMIT shadowColorChanged();
            } else if (e->propertyName() == _clipPath) {
                Q_EMIT clipPathChanged();
            } else if (e->propertyName() == _frameMask) {
                Q_EMIT frameMaskChanged();
            } else if (e->propertyName() == _frameMargins) {
                Q_EMIT frameMarginsChanged();
            } else if (e->propertyName() == _translucentBackground) {
                Q_EMIT translucentBackgroundChanged();
            } else if (e->propertyName() == _enableSystemResize) {
                Q_EMIT enableSystemResizeChanged();
            } else if (e->propertyName() == _enableSystemMove) {
                Q_EMIT enableSystemMoveChanged();
            } else if (e->propertyName() == _enableBlurWindow) {
                Q_EMIT enableBlurWindowChanged();
            } else if (e->propertyName() == _autoInputMaskByClipPath) {
                Q_EMIT autoInputMaskByClipPathChanged();
            }
        }
    }

    return false;
}

DWIDGET_END_NAMESPACE

QT_BEGIN_NAMESPACE
QDebug operator<<(QDebug deg, const DPlatformWindowHandle::WMBlurArea &area)
{
    QDebugStateSaver saver(deg);
    Q_UNUSED(saver)

    deg.setAutoInsertSpaces(true);
    deg << "x:" << area.x
        << "y:" << area.y
        << "width:" << area.width
        << "height:" << area.height
        << "xRadius:" << area.xRadius
        << "yRadius:" << area.yRaduis;

    return deg;
}
QT_END_NAMESPACE
