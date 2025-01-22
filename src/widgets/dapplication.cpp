// SPDX-FileCopyrightText: 2015 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dapplication.h"
#include "private/dapplication_p.h"

#include <QtGlobal>
#ifdef Q_OS_LINUX
#ifdef private
#undef private
#endif
#define private public
#include <QWidget>
#undef private
#endif
#include <QDebug>
#include <QDir>
#include <QLocalSocket>
#include <QLibraryInfo>
#include <QTranslator>
#include <QLocalServer>
#include <QPixmapCache>
#include <QProcess>
#include <QMenu>
#include <QStyleFactory>
#include <QSystemSemaphore>
#include <QtConcurrent/QtConcurrent>

#include <qpa/qplatformintegrationfactory_p.h>
#include <qpa/qplatforminputcontext.h>
#include <private/qapplication_p.h>
#include <private/qcoreapplication_p.h>
#include <private/qwidget_p.h>

#include <DStandardPaths>

#ifdef Q_OS_UNIX
#include <unistd.h>
#endif

#include "dthememanager.h"
#include "daboutdialog.h"
#include "dfeaturedisplaydialog.h"
#include "dmainwindow.h"
#include "dsizemode.h"

#include <DPlatformHandle>
#include <DGuiApplicationHelper>
#include <DFontSizeManager>
#include <DTitlebar>
#include <DLicenseDialog>

#ifdef Q_OS_LINUX
#include "private/startupnotifications/startupnotificationmonitor.h"

#include <DDBusSender>
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <QGSettings>
#endif
#endif



DWIDGET_BEGIN_NAMESPACE

DCORE_USE_NAMESPACE
#define DXCB_PLUGIN_KEY "dxcb"
#define DXCB_PLUGIN_SYMBOLIC_PROPERTY "_d_isDxcb"
#define QT_THEME_CONFIG_PATH "D_QT_THEME_CONFIG_PATH"

DApplicationPrivate::DApplicationPrivate(DApplication *q) :
    DObjectPrivate(q)
{
#ifdef Q_OS_LINUX
    StartupNotificationMonitor *monitor = StartupNotificationMonitor::instance();
    auto cancelNotification = [this, q](const QString id) {
        m_monitoredStartupApps.removeAll(id);
        if (m_monitoredStartupApps.isEmpty()) {
            q->restoreOverrideCursor();
        }
    };
    QObject::connect(monitor, &StartupNotificationMonitor::appStartup,
                     q, [this, q, cancelNotification](const QString id) {
        // FIX bug start app quikly cursor will not restore...
        // Every setOverrideCursor() must eventually be followed by a corresponding restoreOverrideCursor(),
        // otherwise the stack will never be emptied.
        if (m_monitoredStartupApps.isEmpty()) {
            q->setOverrideCursor(Qt::WaitCursor);
        }
        m_monitoredStartupApps.append(id);
        // Set a timeout of 5s in case that some apps like pamac-tray started
        // with StartupNotify but don't show a window after startup finished.
        QTimer::singleShot(5 * 1000, q, [id, cancelNotification](){
            cancelNotification(id);
        });
    });
    QObject::connect(monitor, &StartupNotificationMonitor::appStartupCompleted,
                     q, cancelNotification);
#endif

    // If not in dde and not use deepin platform theme, force set style to chameleon.
    if (!DGuiApplicationHelper::testAttribute(DGuiApplicationHelper::Attribute::IsDeepinPlatformTheme) &&
        !DGuiApplicationHelper::testAttribute(DGuiApplicationHelper::Attribute::IsDeepinEnvironment)) {
        DApplication::setStyle("chameleon");
        q->setPalette(DGuiApplicationHelper::instance()->applicationPalette());
    }
}

DApplicationPrivate::~DApplicationPrivate()
{
    if (m_localServer) {
        m_localServer->close();
    }

    while (q_func()->overrideCursor()) {
        q_func()->restoreOverrideCursor();
    }

}

#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
QString DApplicationPrivate::theme() const
{
    return DThemeManager::instance()->theme();
}

void DApplicationPrivate::setTheme(const QString &theme)
{
    DThemeManager *themeManager = DThemeManager::instance();
    themeManager->setTheme(theme);
}
#endif

static bool tryAcquireSystemSemaphore(QSystemSemaphore *ss, qint64 timeout = 10)
{
    if (ss->error() != QSystemSemaphore::NoError) {
        return false;
    }

    QSystemSemaphore _tmp_ss(QString("%1-%2").arg("DTK::tryAcquireSystemSemaphore").arg(ss->key()), 1, QSystemSemaphore::Open);

    _tmp_ss.acquire();

    QElapsedTimer t;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QFuture<bool> request = QtConcurrent::run(ss, &QSystemSemaphore::acquire);
#else
    QFuture<bool> request = QtConcurrent::run(&QSystemSemaphore::acquire,ss);
#endif
    t.start();

    while (Q_LIKELY(t.elapsed() < timeout && !request.isFinished()));

    if (request.isFinished()) {
        return true;
    }

    if (Q_LIKELY(request.isRunning())) {
        if (Q_LIKELY(ss->release(1))) {
            request.waitForFinished();
        }
    }

    return false;
}

bool DApplicationPrivate::setSingleInstanceBySemaphore(const QString &key)
{
    static QSystemSemaphore ss(key, 1, QSystemSemaphore::Open);
    static bool singleInstance = false;

    if (singleInstance) {
        return true;
    }

    Q_ASSERT_X(ss.error() == QSystemSemaphore::NoError, "DApplicationPrivate::setSingleInstanceBySemaphore:", ss.errorString().toLocal8Bit().constData());

    singleInstance = tryAcquireSystemSemaphore(&ss);

    if (singleInstance) {
        QtConcurrent::run([this] {
            QPointer<DApplication> that = q_func();

            while (ss.acquire() && singleInstance)
            {
                if (!that) {
                    return;
                }

                if (that->startingUp() || that->closingDown()) {
                    break;
                }

                ss.release(1);

                if (that) {
                    Q_EMIT that->newInstanceStarted();
                }
            }
        });

        auto clean_fun = [] {
            ss.release(1);
            singleInstance = false;
        };

        qAddPostRoutine(clean_fun);
        atexit(clean_fun);
    }

    return singleInstance;
}

#ifdef Q_OS_UNIX
/*!
  \internal
  \brief DApplicationPrivate::setSingleInstanceByDbus will check singleinstance by
  register dbus service
  \a key is the last of dbus service name, like "com.deepin.SingleInstance.key"
  \return
*/
bool DApplicationPrivate::setSingleInstanceByDbus(const QString &key)
{
    auto basename = "com.deepin.SingleInstance.";
    QString name = basename + key;
    auto sessionBus = QDBusConnection::sessionBus();
    if (!sessionBus.registerService(name)) {
        qDebug() << "register service failed:" << sessionBus.lastError();
        return  false;
    }
    return true;
}
#endif

bool DApplicationPrivate::loadDtkTranslator(QList<QLocale> localeFallback)
{
    QList<QString> translateDirs;
    auto dtkwidgetDir = DWIDGET_TRANSLATIONS_DIR;
    auto dtkwidgetName = "dtkwidget";

    //("/home/user/.local/share", "/usr/local/share", "/usr/share")
    auto dataDirs = DStandardPaths::standardLocations(QStandardPaths::GenericDataLocation);
    for (const auto &path : dataDirs) {
        DPathBuf DPathBuf(path);
        translateDirs << (DPathBuf / dtkwidgetDir).toString();
    }

#ifdef DTK_STATIC_TRANSLATION
    translateDirs << QString(":/dtk/translations");
#endif

    return DGuiApplicationHelper::loadTranslator(dtkwidgetName, translateDirs, localeFallback);
}

// 自动激活DMainWindow类型的窗口
void DApplicationPrivate::_q_onNewInstanceStarted()
{
    if (!autoActivateWindows)
        return;

    for (QWidget *window : qApp->topLevelWidgets()) {
        if (qobject_cast<DMainWindow*>(window)) {
            // 如果窗口最小化或者隐藏了，應當先將其show出來
            if (window->isMinimized() || window->isHidden())
                window->showNormal();

            window->activateWindow();
            break; // 只激活找到的第一个窗口
        }
    }
}

void DApplicationPrivate::doAcclimatizeVirtualKeyboard(QWidget *window, QWidget *widget, bool allowResizeContentsMargins)
{
    // 如果新激活的输入窗口跟已经在处理中的窗口不一致，则恢复旧窗口的状态
    if (activeInputWindow && activeInputWindow != window) {
        activeInputWindow->setContentsMargins(activeInputWindowContentsMargins);
        activeInputWindow = nullptr;
    }

    auto platform_context = QGuiApplicationPrivate::platformIntegration()->inputContext();
    auto input_method = QGuiApplication::inputMethod();
    // 先检查输入面板的状态
    if (!platform_context->inputMethodAccepted() || !input_method->isVisible()) {
        if (activeInputWindow) {
            activeInputWindow->setContentsMargins(activeInputWindowContentsMargins);
            activeInputWindow = nullptr;
        }
        widget->setProperty("_dtk_selectHandleMargins", 0);
        return;
    }

    // 如果输入控件的主窗口处于未激活状态则忽略
    if (!window->isActiveWindow())
        return;

    // 虚拟键盘相对于当前窗口的geometry
    const QRectF &kRect = input_method->keyboardRectangle().translated(-window->mapToGlobal(QPoint(0, 0)));
    if (kRect.isEmpty() || !kRect.isValid())
        return;

    // 记录待处理窗口的数据
    if (!activeInputWindow) {
        activeInputWindow = window;
        activeInputWindowContentsMargins = window->contentsMargins();
        lastContentsMargins = qMakePair(0, 0);
    }

    // 可以被压缩的高度
    int resizeableHeight = lastContentsMargins.first;
    // 将要平移的距离
    int panValue = 0;

    const QRectF &cRect = input_method->cursorRectangle();
    const QRectF &iRect = input_method->inputItemClipRectangle();
    const QRectF &wRect = window->rect().marginsRemoved(activeInputWindowContentsMargins);

    if (allowResizeContentsMargins) {
        // 判断输入控件是否处于一个可滚动区域中
        QWidget *scrollWidget = widget;
        while (scrollWidget && !qobject_cast<QAbstractScrollArea*>(scrollWidget)) {
            scrollWidget = scrollWidget->parentWidget();
        }

        if (QAbstractScrollArea *scrollArea = qobject_cast<QAbstractScrollArea*>(scrollWidget)) {
            resizeableHeight = scrollArea->maximumViewportSize().height()
                    // 至少要保证可滚动区域的最小高度，以及光标的可显示区域
                    - qMax(scrollArea->minimumHeight(), qRound(cRect.height()));
            resizeableHeight = qMax(0, resizeableHeight);
        }
    }

    // 优先保证输入框下面的内容都能正常显示
    int shadowHeight = wRect.bottom() - kRect.top();
    if (shadowHeight > cRect.y()) {
        // 如果窗口底部的内容必须要被遮挡，则优先保证能完整显示整个输入区域，且最低限度是要显示输入光标
        shadowHeight = qMin(cRect.y(), iRect.bottom() - kRect.top());
    }
    // 如果输入区域没有被盖住则忽略
    if (shadowHeight <= 0)
        return;

    // 更新需要平移的区域
    int resizeHeight = qMin(resizeableHeight, shadowHeight);
    panValue = shadowHeight - resizeHeight;

    // 记录本次的计算结果
    lastContentsMargins.first = resizeableHeight;
    lastContentsMargins.second = panValue;

    QRectF cursor = input_method->anchorRectangle();
    QPointF cursorPoint = cursor.topLeft();

    if (cursorPoint.y() < kRect.y()) {
        if (window->contentsMargins() != QMargins())
            window->setContentsMargins(QMargins());
        return;
    }

    // 更新窗口内容显示区域以确保虚拟键盘能正常显示
    window->setContentsMargins(0, -panValue, 0, resizeHeight + panValue);
    widget->setProperty("_dtk_selectHandleMargins", panValue);
}

void DApplicationPrivate::acclimatizeVirtualKeyboardForFocusWidget(bool allowResizeContentsMargins)
{
    auto focus = QApplication::focusWidget();
    if (!focus)
        return;

    for (auto window : acclimatizeVirtualKeyboardWindows) {
        if (window->isAncestorOf(focus)) {
            return doAcclimatizeVirtualKeyboard(window ,focus, allowResizeContentsMargins);
        }
    }
}

void DApplicationPrivate::_q_panWindowContentsForVirtualKeyboard()
{
    acclimatizeVirtualKeyboardForFocusWidget(false);
}

void DApplicationPrivate::_q_resizeWindowContentsForVirtualKeyboard()
{
    // TODO(zccrs): 暂时不支持压缩窗口高度适应虚拟键盘的模式
    // 需要做到ScrollArea中的输入控件能在改变窗口高度之后还处于可见状态
    acclimatizeVirtualKeyboardForFocusWidget(false);
}

void DApplicationPrivate::_q_sizeModeChanged()
{
    QEvent ev(QEvent::StyleChange);
    for (auto item : qApp->topLevelWidgets()) {
        handleSizeModeChangeEvent(item, &ev);
    }
}

void DApplicationPrivate::handleSizeModeChangeEvent(QWidget *widget, QEvent *event)
{
    // 深度优先遍历，事件接受顺序：子 -> 父， 若parentWidget先处理event，可能存在布局没更新问题
    for (auto w : widget->findChildren<QWidget *>(QString(), Qt::FindDirectChildrenOnly)) {
        handleSizeModeChangeEvent(w, event);
    }
    if (widget->isWindow()) {
        // TODO 顶层窗口需要延迟，否则内部控件布局出现异常，例如DDialog, 若send事件，导致
        // 从compact -> normal -> campact时，DDialog内部控件布局的两次campact大小不一致.
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        qApp->postEvent(widget, event->clone());
#else
        qApp->postEvent(widget, new QEvent(*event));
#endif
    } else {
        QCoreApplication::sendEvent(widget, event);
    }
}

bool DApplicationPrivate::isUserManualExists()
{
    return DGuiApplicationHelper::instance()->hasUserManual();
}

/*!
  \class Dtk::Widget::DApplication
  \inmodule dtkwidget
  
  \brief DApplication 是 DTK 中用于替换 QCoreApplication 相关功能实现的类.

  继承自 QApplication ，并在此之上添加了一些特殊的设定，如：
  - 在 FORCE_RASTER_WIDGETS 宏生效的情况下，默认设置 Qt::AA_ForceRasterWidgets 以减少 glx 相关库的加载，减少程序启动时间；
  - 自动根据 applicationName 和 系统 locale 加载对应的翻译文件；
  - 会根据系统gsettings中 com.deepin.dde.dapplication 的 qpixmapCacheLimit 值来设置 QPixmapCache::cacheLimit ；
  - 会根据系统gsettings中 com.deepin.dde.touchscreen longpress-duration 的值来设置 QTapAndHoldGesture::timeout ；
  - 方便地通过 setSingleInstance 来实现程序的单实例。
  
  \note DApplication 设置的 QTapAndHoldGesture::timeout 会比 gsettings
  中的值小 100，用来绕过 Dock 长按松开容易导致应用启动的问题，详细解释见
  见代码注释或者 \l {https://github.com/linuxdeepin/internal-discussion/issues/430}
  
  \sa loadTranslator() setSingleInstance()
 */

/*!
  \fn DApplication::newInstanceStarted()
  \brief 该信号会在程序的一个新实例启动的时候被触发.
 */

/*!
  \fn DApplication::iconThemeChanged()
  \brief 该信号在系统图标主题发生改变的时候被触发.
 */

/*!
  \fn DApplication::screenDevicePixelRatioChanged(QScreen *screen)
  \brief 该信号在对应屏幕 \a screen 的缩放比可能发现变化时触发.

  依赖于 deepin 平台主题插件（dde-qt5integration 包中提供），实时更改
  屏幕缩放比是通过更改配置文件 ~/.config/deepin/qt-theme.ini 实现，与此相关的
  配置项有三个：
  \list
  \li ScreenScaleFactors：多屏幕设置不同缩放比，值格式和环境变量QT_SCREEN_SCALE_FACTORS一致
  \li ScaleFactor: 设置所有屏幕缩放比，值格式和环境变量QT_SCALE_FACTOR一致
  \li ScaleLogcailDpi：指定屏幕逻辑dpi，可影响仅设置了 point size 的 QFont 的绘制大小。
  \endlist

  未设置此值时，默认会在 ScreenScaleFactors 值改变后将屏幕逻辑dpi更改为主屏默认值，一般情况下，不需要设置此值。
  ScreenScaleFactors 和 ScaleFactor 的值改变后，会触发所有屏幕的 QScreen::geometryChanged, 且会根据当前缩放
  更新所有QWindow的geometry（更新时保持窗口的真实大小不变，新窗口大小=窗口真实大小/新的缩放比）。另外，可在构造
  DApplication 对象之前设置 Qt::AA_DisableHighDpiScaling 为 true，或添加环境变量 D_DISABLE_RT_SCREEN_SCALE
  禁用实时缩放的支持。
  
  \sa QScreen::devicePixelRatio()
 */

/*!
  \brief 返回一个DApplicatioin实例.

  如果在执行此函数之前DApplication已经被创建则返回已存在的实例，否则直接创建一个
  新的DApplication实例并返回。主要用于与deepin-trubo服务相配合，用于共享
  deepin-turbo dtkwidget booster中已经创建的DApplication对象，以此节省初始化时间。

  \a argc 传递给DApplication的构造函数
  \a argv 传递给DApplication的构造函数

  \return 返回一个DApplication对象
  \warning 不保证获取的DApplication对象一定有效，如果实例已存在，则直接使
  用static_case将其转换为DApplication对象
 */
DApplication *DApplication::globalApplication(int &argc, char **argv)
{
    if (instance()) {
        auto dp = static_cast<QCoreApplicationPrivate*>(qApp->QCoreApplication::d_ptr.data());
        Q_ASSERT(dp);

        // 清理QGuiApplication库的命令行参数
        int j = argc ? 1 : 0;
        const QByteArrayList qt_command = {
            "-platformpluginpath",
            "-platform",
            "-platformtheme",
            "-qwindowgeometry",
            "-geometry",
            "-qwindowtitle",
            "-title",
            "-qwindowicon",
            "-icon",
            "-plugin",
            "-reverse",
            "-session",
            "-style"
        };
        for (int i = 1; i < argc; i++) {
            if (!argv[i])
                continue;
            if (*argv[i] != '-') {
                argv[j++] = argv[i];
                continue;
            }
            const char *arg = argv[i];
            if (arg[1] == '-') // startsWith("--")
                ++arg;
            if (qt_command.indexOf(arg) >= 0) {
                // 跳过这个参数的值
                ++i;
            } else if (strcmp(arg, "-testability") != 0
                       && strncmp(arg, "-style=", 7) != 0) {
                argv[j++] = argv[i];
            }
        }

        if (j < argc) {
            argv[j] = nullptr;
            argc = j;
        }

        dp->argc = argc;
        dp->argv = argv;
        dp->processCommandLineArguments();
        static_cast<QApplicationPrivate*>(dp)->process_cmdline();
        return qApp;
    }

    return new DApplication(argc, argv);
}

/*!
  \brief constructs an instance of DApplication.
  \brief 用于构建 DApplication 实例的构造函数

  \a argc is the same as in the main function.
  \a argc 作用同 QApplication::QApplication 参数 argc。
  \a argv is the same as in the main function.
  \a argv 作用同 QApplication::QApplication 参数 argv。

  对象构造时会判断环境变量 DTK_FORCE_RASTER_WIDGETS 的值，如果为 TRUE 则开启
  Qt::AA_ForceRasterWidgets，为 FALSE 则不开启，当没有设置此环境变量时，如果
  编译时使用了宏 FORCE_RASTER_WIDGETS（龙芯和申威平台默认使用），则开启
  Qt::AA_ForceRasterWidgets，否则不开启。
 */
DApplication::DApplication(int &argc, char **argv) :
    QApplication(argc, argv),
    DObject(*new DApplicationPrivate(this))
{
    // FIXME: fix bug in nvidia prime workaround, do not know effoct, must test more!!!
    // 在龙芯和申威上，xcb插件中加载glx相关库（r600_dri.so等）会额外耗时1.xs（申威应该更长）
    if (
#ifdef FORCE_RASTER_WIDGETS
            QLatin1String("FALSE") !=
#else
            QLatin1String("TRUE") ==
#endif
    qgetenv("DTK_FORCE_RASTER_WIDGETS")) {
        setAttribute(Qt::AA_ForceRasterWidgets);
    }

#ifdef Q_OS_LINUX
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    // set qpixmap cache limit
    if (QGSettings::isSchemaInstalled("com.deepin.dde.dapplication"))
    {
        QGSettings gsettings("com.deepin.dde.dapplication", "/com/deepin/dde/dapplication/");
        if (gsettings.keys().contains("qpixmapCacheLimit"))
            QPixmapCache::setCacheLimit(gsettings.get("qpixmap-cache-limit").toInt());
    }

    // set QTapAndHoldGesture::timeout
    if (QGSettings::isSchemaInstalled("com.deepin.dde.touchscreen")) {
        QGSettings gsettings("com.deepin.dde.touchscreen");
        if (gsettings.keys().contains("longpressDuration"))
            // NOTE(hualet): -100 is a workaround against the situation that that sometimes longpress
            // and release on Dock cause App launches which should be avoided.
            //
            // I guess it happens like this: longpress happens on Dock,
            // Dock menu shows(doesn't grab the mouse which is a bug can't be fixed easily),
            // user ends the longpress, DDE Dock recevies mouseReleaseEvent and checks for
            // QTapAndHoldGesture  which is still not happening (maybe because the timer used is
            // a CoarseTimer?), so Dock treats the event as a normal mouseReleaseEvent, launches the
            // App or triggers the action.
            //
            // see: https://github.com/linuxdeepin/internal-discussion/issues/430
            //
            // This workaround hopefully can fix most of this situations.
            QTapAndHoldGesture::setTimeout(gsettings.get("longpress-duration").toInt() - 100);
    }
#endif
#endif

    connect(DGuiApplicationHelper::instance(), SIGNAL(sizeModeChanged(DGuiApplicationHelper::SizeMode)),
            this, SLOT(_q_sizeModeChanged()));
}

DApplication::~DApplication() {
    
}

/*!
  \enum Dtk::Widget::DApplication::SingleScope
  DApplication::SingleScope 定义了 DApplication 单实例的效应范围。
  
  \value UserScope
  代表单实例的范围为用户范围，即同一个用户会话中不允许其他实例出现。
  
  \value SystemScope
  代表单实例的范围为系统范围，当前系统内只允许一个程序实例运行。
 */

/*!
  \brief returns name of the theme that the application is currently using.
  
  theme name can be one of light, dark, semidark or semilight.
  theme 属性表示当前程序使用的主题名称，目前可选的主题名称有 light、dark、semidark 和 semilight。

  \return the theme name.
 */
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
QString DApplication::theme() const
{
    D_DC(DApplication);

    return d->theme();
}
#endif

/*!
  \brief set theme for the application to use the theme we provide.

  \a theme is the name of the theme we want to set.
 */
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
void DApplication::setTheme(const QString &theme)
{
    D_D(DApplication);

    d->setTheme(theme);
}
#endif

#ifdef Q_OS_UNIX
/*!
  \brief set Out-Of-Memory score.
  \brief 用于调整当前进程的 Out-Of-Memory 分数（linux环境下）

  \a score vaild range is [-1000, 1000]
  \a score 指定 oom-score，范围为 [-1000, 1000]。
  
  这个分数影响了内核在系统资源（内存）不足的情况下
  为了释放内存资源而挑选进程杀掉的算法，分数越高越容易被杀。
 */
void DApplication::setOOMScoreAdj(const int score)
{
    if (score > 1000 || score < -1000)
        qWarning() << "OOM score adjustment value out of range: " << score;

    QFile f("/proc/self/oom_score_adj");
    if (!f.open(QIODevice::WriteOnly))
    {
        qWarning() << "OOM score adjust failed, open file error: " << f.errorString();
        return;
    }

    f.write(std::to_string(score).c_str());
}
#endif

/*!
  \brief 用于将程序设置成单实例.

  \a key 是确定程序唯一性的ID，一般使用程序的二进制名称即可。
  
  \note 一般情况下单实例的实现使用 QSystemSemaphore，如果你的程序需要在沙箱
  环境如 flatpak 中运行，可选的一套方案是通过 DTK_DBUS_SINGLEINSTANCE 这个
  编译宏来控制单实例使用 DBus 方案。
  
  \return 设置成功返回 true，否则返回 false。
 */
bool DApplication::setSingleInstance(const QString &key)
{
    return setSingleInstance(key, UserScope);
}

/*!
  \brief 一个重写函数，增加了控制单实例范围的 \a singleScope 参数。

  在Linux环境下默认使用DBus的方式实现单例判断，在其它环境或者设置了环境变量 DTK_USE_SEMAPHORE_SINGLEINSTANCE
  时使用系统信号量的方式实现单例判断。

  \a key 是确定程序唯一性的ID，一般使用程序的二进制名称即可。
  \a singleScope 用于指定单实例的影响范围，具体见 DApplication::SingleScope 。
  \return 设置成功返回 true，否则返回 false。
 */
bool DApplication::setSingleInstance(const QString &key, SingleScope singleScope)
{
    D_D(DApplication);

    auto scope = singleScope == SystemScope ? DGuiApplicationHelper::WorldScope : DGuiApplicationHelper::UserScope;
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::newProcessInstance,
            this, &DApplication::newInstanceStarted, Qt::UniqueConnection);

    return DGuiApplicationHelper::setSingleInstance(key, scope);
}

/*!
  \brief loads translate file from system or application data path.
  \brief 加载程序的翻译文件.

  You must name the file correctly; if the program is dde-dock，
  then the qm file for English locale would be dde-dock_en.qm.
  Translation files must be placed in correct directories as well.
  The lookup order is as follows:

  \list
  \li ~/.local/share/APPNAME/translations;
  \li /usr/local/share/APPNAME/translations;
  \li /usr/share/APPNAME/translations;
  \endlist

  APPNAME is the name of program executable.
  \a localeFallback, a list of fallback locale you want load.
  \return load success return true, otherwise, return false.
  
  使用这个函数需要保证翻译文件必须正确命名: 例如程序名叫 dde-dock，
  那么翻译文件在中文locale下的名称必须是 dde-dock_zh_CN.qm；翻译文件还需要放置
  在特定的位置，此函数会按照优先级顺序在以下目录中查找翻译文件：

  \list
  \li ~/.local/share/APPNAME/translations;
  \li /usr/local/share/APPNAME/translations;
  \li /usr/share/APPNAME/translations;
  \endlist

  APPNAME 即可执行文件的名称。
  
  \a localeFallback 指定了回退的locale列表，默认只有系统locale。
  \return 加载成功返回 true，否则返回 false。
 */
bool DApplication::loadTranslator(QList<QLocale> localeFallback)
{
    D_D(DApplication);

    bool loadDtkTranslator =  d->loadDtkTranslator(localeFallback);
    // qt && qtbase && appName
    bool loadQtAppTranslator = DGuiApplicationHelper::loadTranslator(localeFallback);

    return loadDtkTranslator && loadQtAppTranslator;
}

/*!
  \brief 强制程序使用的平台插件到dxcb.

  这个函数的工作原理是通过设置 QT_QPA_PLATFORM 来影响平台插件的加载，所以此函数
  必须在 DApplication 实例创建前进行调用。

  \return 设置成功返回 true，否则返回 false。
 */
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
bool DApplication::loadDXcbPlugin()
{
    Q_ASSERT_X(!qApp, "DApplication::loadDxcbPlugin", "Must call before QGuiApplication defined object");

    if (!QPlatformIntegrationFactory::keys().contains(DXCB_PLUGIN_KEY)) {
        return false;
    }

    // fix QGuiApplication::platformName() to xcb
    qputenv("DXCB_FAKE_PLATFORM_NAME_XCB", "true");

    return qputenv("QT_QPA_PLATFORM", DXCB_PLUGIN_KEY);
}
#endif

/*!
  \brief 检查当前程序是否使用了dxcb平台插件.

  \return 正在使用返回 true，否则返回 false。
 */
bool DApplication::isDXcbPlatform()
{
    return DGUI_NAMESPACE::DPlatformHandle::isDXcbPlatform();
}

/*!
  \brief 返回编译时的dtk版本.
 */
int DApplication::buildDtkVersion()
{
    return DtkBuildVersion::value;
}

/*!
  \brief 返回运行时的dtk版本.
 */
int DApplication::runtimeDtkVersion()
{
    return DTK_VERSION;
}

/*!
  \brief 用于跟 startdde 进行通信，告知 startdde 进程已经启动成功.

  \note 只有DDE系统组件需要使用此函数，普通应用无需使用。
 */
void DApplication::registerDDESession()
{
#ifdef Q_OS_LINUX
    QString envName("DDE_SESSION_PROCESS_COOKIE_ID");

    QByteArray cookie = qgetenv(envName.toUtf8().data());
    qunsetenv(envName.toUtf8().data());

    if (!cookie.isEmpty()) {
        DDBusSender()
                .service("com.deepin.SessionManager")
                .path("/com/deepin/SessionManager")
                .interface("com.deepin.SessionManager")
                .method("Register")
                .arg(QString(cookie))
                .call();
    }
#endif
}

/*!
  \brief 根据用户家目录设置Qt主题配置文件的目录.

  \a home 家目录，不要以 "/" 结尾
  \warning 必须在构造 DApplication 对象之前调用
  \sa DApplication::customQtThemeConfigPath
 */
void DApplication::customQtThemeConfigPathByUserHome(const QString &home)
{
    customQtThemeConfigPath(home + "/.config");
}

/*!
  \brief 自定义 Qt 主题配置文件的路径。
  
  默认文件通常为 "~/.config/deepin/qt-theme.ini"
  其中包含应用的图标主题、字体、、屏幕缩放等相关的配置项。可应用于以root用户启动的
  应用，需要跟随某个一般用户的主题设置项。
  \a path 中不包含 "/deepin/qt-theme.ini" 部分，如：path = "/tmp"，
  则配置文件路径为："/tmp/deepin/qt-theme.ini"。
  \a path 不要以 "/" 结尾
  \warning 必须在构造 DApplication 对象之前调用

  \sa DApplication::customizedQtThemeConfigPath
 */
void DApplication::customQtThemeConfigPath(const QString &path)
{
    Q_ASSERT_X(!qApp, "DApplication::customQtThemeConfigPath", "Must call before QGuiApplication defined object");
    qputenv(QT_THEME_CONFIG_PATH, path.toLocal8Bit());
}

/*!
  \brief 自定义 Qt 主题配置文件路径.

  \return 返回自定义的 Qt 主题配置文件路径，未设置过此路径时返回为空。
  \sa DApplication::customQtThemeConfigPath
 */
QString DApplication::customizedQtThemeConfigPath()
{
    return QString::fromLocal8Bit(qgetenv(QT_THEME_CONFIG_PATH));
}

/*!
  \brief returns the product name of this application.
  \brief 程序的产品名称.

  It's mainly used to construct an about dialog of the application.
  
  产品名称不同与 applicationName ，应该是类似如“深度终端”，而不是 deepin-terminal，
  这个名称主要用于在程序的关于对话框中进行展示。
  如果没有手动通过 setProductName 来设置，会尝试使用 QApplication::applicationDisplayName 来充当产品名称。
  
  \return the product name of this application if set, otherwise the applicationDisplayName.
  \sa productIcon, aboutDialog
 */
QString DApplication::productName() const
{
    D_DC(DApplication);

    return d->productName.isEmpty() ? applicationDisplayName() : d->productName;
}

/*!
  \brief sets the product name of this application.

  \a productName is the product name to be set.
 */
void DApplication::setProductName(const QString &productName)
{
    D_D(DApplication);

    d->productName = productName;
}

/*!
  \brief returns the product icon of this application.
  
  It's mainly used to construct an about dialog of the application.
    
  \brief productIcon 属性是程序的产品图标，
  主要用于在关于对话框中进行展示。
  
  \return the product icon of this application if set, otherwise empty.
  \sa productName, aboutDialog
 */
const QIcon &DApplication::productIcon() const
{
    D_DC(DApplication);

    return d->productIcon;
}

/*!
  \brief sets the product icon of this application.

  \a productIcon is the product icon to be set.
 */
void DApplication::setProductIcon(const QIcon &productIcon)
{
    D_D(DApplication);

    d->productIcon = productIcon;
}

/*!
  \brief returns the license used by this application.
  
  It's mainly used to construct an about dialog of the application.
  
  applicationLicense 属性是程序所使用的授权协议；
  主要用于在关于对话框中进行展示，默认值为 GPLv3。

  \return the license used by this application.
 */
QString DApplication::applicationLicense() const
{
    D_DC(DApplication);

    return d->appLicense;
}

/*!
  \brief sets the license of this application.

  \a license is the license to be set.
 */
void DApplication::setApplicationLicense(const QString &license)
{
    D_D(DApplication);

    d->appLicense = license;
}

/*!
  \brief returns the long description of the application.
  \brief 属性记录了程序的描述信息，主要用于关于对话框中的信息展示.
  
  It's mainly used to construct an about dialog of the application.
  

  \return the description of the application if set, otherwise empty.
 */
QString DApplication::applicationDescription() const
{
    D_DC(DApplication);

    return d->appDescription;
}

/*!
  \brief sets the description of the application.

  \a description is description to be set.
 */
void DApplication::setApplicationDescription(const QString &description)
{
    D_D(DApplication);

    d->appDescription = description;
}

/*!
  \brief 程序的主页地址.

  该属性记录程序的主页网址，主要用于在关于对话框中进行展示。
  默认情况下，该地址为 Uos 或者 Deepin 官方网站地址。
 */
QString DApplication::applicationHomePage() const
{
    D_DC(DApplication);

    return d->homePage;
}

void DApplication::setApplicationHomePage(const QString &link)
{
    D_D(DApplication);

    d->homePage = link;
}

/*!
  \brief returns the acknowlegement page of the application.
  \brief 记录程序的鸣谢信息网址，主要用于在关于对话框中进行展示.
  
  It's mainly used to construct an about dialog of the application.  

  \return the acknowlegement page of the application if set, otherwise empty.
 */
QString DApplication::applicationAcknowledgementPage() const
{
    D_DC(DApplication);

    return d->acknowledgementPage;
}

/*!
  \brief sets the acknowlegement page of the application.

  \a link is the acknowlegement page link to be shown in the about dialog.
 */
void DApplication::setApplicationAcknowledgementPage(const QString &link)
{
    D_D(DApplication);

    d->acknowledgementPage = link;
}

/*!
  \brief 是否显示关于对话框中的鸣谢地址显示.

  \return 显示鸣谢地址返回 true，否则返回 false。
  \sa DApplication::setApplicationAcknowledgementVisible
 */
bool DApplication::applicationAcknowledgementVisible() const
{
    D_DC(DApplication);
    return d->acknowledgementPageVisible;
}

/*!
  \brief 设置是否显示关于对话框中的鸣谢地址显示.

  \a visible 为 true 显示鸣谢地址，为 false 隐藏鸣谢地址。
  \sa DApplication::applicationAcknowledgementVisible
 */
void DApplication::setApplicationAcknowledgementVisible(bool visible)
{
    D_D(DApplication);
    d->acknowledgementPageVisible = visible;
}

/*!
  \brief returns the about dialog of this application.
  \brief 返回一个基于当前程序信息的关于对话框.
  
  If the about dialog is not set, it will automatically construct one.
  此对话框可以通过 DApplication::setAboutDialog 进行设置，如果没有设置就使用此函数进行获取，
  系统会创建一个新的关于对话框。
  
  \return the about dialog instance.
  \sa DApplication::setAboutDialog
 */
DAboutDialog *DApplication::aboutDialog()
{
    D_D(DApplication);

    return d->aboutDialog;
}

/*!
  \brief sets the \a aboutDialog of this application.
  \brief 为当前程序设置一个关于对话框.
  
  It's mainly used to override the auto-constructed about dialog which is not
  a common case, so please do double check before using this method.
  
  \sa DApplication::aboutDialog
 */
void DApplication::setAboutDialog(DAboutDialog *aboutDialog)
{
    D_D(DApplication);

    if (d->aboutDialog && d->aboutDialog != aboutDialog) {
        d->aboutDialog->deleteLater();
    }

    d->aboutDialog = aboutDialog;
}

DFeatureDisplayDialog *DApplication::featureDisplayDialog()
{
    D_D(DApplication);
    if (d->featureDisplayDialog == nullptr) {
        d->featureDisplayDialog = new DFeatureDisplayDialog();
        connect(this, &DApplication::aboutToQuit, this, [this]{
            D_D(DApplication);
            d->featureDisplayDialog->deleteLater();
            d->featureDisplayDialog = nullptr;
        });
    }
    return d->featureDisplayDialog;
}

void DApplication::setFeatureDisplayDialog(DFeatureDisplayDialog *featureDisplayDialog)
{
    D_D(DApplication);

    if (d->featureDisplayDialog && d->featureDisplayDialog != featureDisplayDialog) {
        d->featureDisplayDialog->deleteLater();
    }

    d->featureDisplayDialog = featureDisplayDialog;
}

/*!
  \property DApplication::visibleMenuShortcutText

  \brief 菜单项快捷键文本的可视化属性

  该属性代表了程序中菜单项是否显示对应的快捷键。
  默认地，该属性为 false.
 */
bool DApplication::visibleMenuShortcutText() const
{
    D_DC(DApplication);

    return d->visibleMenuShortcutText;
}

void DApplication::setVisibleMenuShortcutText(bool value)
{
    D_D(DApplication);

    d->visibleMenuShortcutText = value;
}

/*!
  \property DApplication::visibleMenuCheckboxWidget

  \brief 菜单项内 CheckBox 控件的可视化属性

  该属性代表了程序中菜单项是否显示Checkbox控件。
  默认地，该属性为 false.
 */
bool DApplication::visibleMenuCheckboxWidget() const
{
    D_DC(DApplication);

    return d->visibleMenuCheckboxWidget;
}

void DApplication::setVisibleMenuCheckboxWidget(bool value)
{
    D_D(DApplication);

    d->visibleMenuCheckboxWidget = value;
}

/*!
  \property DApplication::visibleMenuIcon

  \brief 菜单按钮的可视化属性.

  visibleMenuIcon 属性代表了程序中菜单项是否显示图标。
  默认地，该属性为 false.
 */
bool DApplication::visibleMenuIcon() const
{
    D_DC(DApplication);

    return d->visibleMenuIcon;
}

void DApplication::setVisibleMenuIcon(bool value)
{
    D_D(DApplication);

    d->visibleMenuIcon = value;
}

bool DApplication::autoActivateWindows() const
{
    D_DC(DApplication);

    return d->autoActivateWindows;
}

void DApplication::setAutoActivateWindows(bool autoActivateWindows)
{
    D_D(DApplication);

    d->autoActivateWindows = autoActivateWindows;

    if (autoActivateWindows) {
        connect(DGuiApplicationHelper::instance(), SIGNAL(newProcessInstance(qint64, const QStringList &)),
                this, SLOT(_q_onNewInstanceStarted()));
    } else {
        disconnect(DGuiApplicationHelper::instance(), SIGNAL(newProcessInstance(qint64, const QStringList &)),
                this, SLOT(_q_onNewInstanceStarted()));
    }
}

/*!
  \brief 用于窗口中可输入控件自适应虚拟键盘.
  
  为窗口的可输入控件添加自动适应虚拟键盘输入法的功能。开启此功能后，当
  监听到 QInputMethod::keyboardRectangleChanged 后，会判断当
  前的可输入（不仅仅是处于焦点状态）控件是否为此 \a window 的子控件，
  如果是，则将通过 QWidget::setContentsMargins 更新 \a window
  的布局区域，以此确保可输入控件处于可见区域。如果可输入控件处于一个
   QAbstractScrollArea 中，将会压缩 \a window 的布局空间，促使
  可滚动区域缩小，再使用 QAbstraceScrollArea::scrollContentsBy
  将可输入控件滚动到合适的区域，否则将直接把 \a window 的内容向上移动为虚拟键盘
  腾出空间。

  \note 在使用之前要确保窗口的 Qt::WA_LayoutOnEntireRect
  和 Qt::WA_ContentsMarginsRespectsSafeArea 都为 false

  \a window 必须是一个顶层窗口
  \sa QWidget::isTopLevel QWidget::setContentsMargins
  \sa QInputMethod::cursorRectangle QInputMethod::inputItemClipRectangle QInputMethod::keyboardRectangle
  \sa QAbstractScrollArea
 */
void DApplication::acclimatizeVirtualKeyboard(QWidget *window)
{
    Q_ASSERT(!window->property("_dtk_NoTopLevelEnabled").toBool() ? window->isWindow() : true
             && !window->testAttribute(Qt::WA_LayoutOnEntireRect)
             && !window->testAttribute(Qt::WA_ContentsMarginsRespectsSafeArea));

    D_D(DApplication);
    if (d->acclimatizeVirtualKeyboardWindows.contains(window))
        return;

    if (d->acclimatizeVirtualKeyboardWindows.isEmpty()) {
        connect(this, SIGNAL(focusChanged(QWidget *, QWidget *)),
                this, SLOT(_q_resizeWindowContentsForVirtualKeyboard()),
                Qt::ConnectionType(Qt::QueuedConnection | Qt::UniqueConnection));
        connect(inputMethod(), SIGNAL(keyboardRectangleChanged()),
                this, SLOT(_q_resizeWindowContentsForVirtualKeyboard()),
                Qt::ConnectionType(Qt::QueuedConnection | Qt::UniqueConnection));
        connect(inputMethod(), SIGNAL(visibleChanged()),
                this, SLOT(_q_resizeWindowContentsForVirtualKeyboard()),
                Qt::ConnectionType(Qt::QueuedConnection | Qt::UniqueConnection));
        connect(inputMethod(), SIGNAL(cursorRectangleChanged()),
                this, SLOT(_q_panWindowContentsForVirtualKeyboard()),
                Qt::ConnectionType(Qt::QueuedConnection | Qt::UniqueConnection));
        connect(inputMethod(), SIGNAL(inputItemClipRectangleChanged()),
                this, SLOT(_q_panWindowContentsForVirtualKeyboard()),
                Qt::ConnectionType(Qt::QueuedConnection | Qt::UniqueConnection));
    }

    d->acclimatizeVirtualKeyboardWindows << window;

    connect(window, &QWidget::destroyed, this, [this, window] {
        this->ignoreVirtualKeyboard(window);
    });

    if (window->isAncestorOf(focusWidget())) {
        d->doAcclimatizeVirtualKeyboard(window, focusWidget(), true);
    }
}

/*!
  \brief 恢复虚拟键盘的自适应操作.

  该函数用于恢复虚拟键盘到默认状态，将不会为虚拟键盘的环境做任何自适应操作。

  \note 此操作不会恢复对 QWidget::contentsMargins 的修改
  \a window 需是一个调用过 acclimatizeVirtualKeyboard 的窗口
  \sa DApplication::isAcclimatizedVirtualKeyboard DApplication::acclimatizeVirtualKeyboard
 */
void DApplication::ignoreVirtualKeyboard(QWidget *window)
{
    D_D(DApplication);

    if (!d->acclimatizeVirtualKeyboardWindows.removeOne(window))
        return;

    if (d->acclimatizeVirtualKeyboardWindows.isEmpty()) {
        disconnect(this, SIGNAL(focusChanged(QWidget *, QWidget *)),
                   this, SLOT(_q_resizeWindowContentsForVirtualKeyboard()));
        disconnect(inputMethod(), SIGNAL(keyboardRectangleChanged()),
                   this, SLOT(_q_resizeWindowContentsForVirtualKeyboard()));
        disconnect(inputMethod(), SIGNAL(visibleChanged()),
                   this, SLOT(_q_resizeWindowContentsForVirtualKeyboard()));
        disconnect(inputMethod(), SIGNAL(cursorRectangleChanged()),
                   this, SLOT(_q_panWindowContentsForVirtualKeyboard()));
        disconnect(inputMethod(), SIGNAL(inputItemClipRectangleChanged()),
                   this, SLOT(_q_panWindowContentsForVirtualKeyboard()));
    }
}

/*!
  \brief 是否自适应虚拟键盘

  \return 如果 \a window 会自适应虚拟键盘返回 true,否则返回 false
 */
bool DApplication::isAcclimatizedVirtualKeyboard(QWidget *window) const
{
    D_DC(DApplication);
    return d->acclimatizeVirtualKeyboardWindows.contains(window);
}

QString DApplication::applicationCreditsFile() const
{
    D_DC(DApplication);
    return d->applicationCreditsFile;
}

void DApplication::setApplicationCreditsFile(const QString &file)
{
    D_D(DApplication);
    d->applicationCreditsFile = file;
}

QByteArray DApplication::applicationCreditsContent() const
{
    D_DC(DApplication);
    return d->applicationCreditsContent;
}

void DApplication::setApplicationCreditsContent(const QByteArray &content)
{
    D_D(DApplication);
    d->applicationCreditsContent = content;
}

QString DApplication::licensePath() const
{
    D_DC(DApplication);
    return d->licensePath;
}

void DApplication::setLicensePath(const QString &path)
{
    D_D(DApplication);
    d->licensePath = path;
}

/*!
   \brief 设置 app 的处理程序.

    如果要设置，必须对 help、about、quit 都进行处理。
    \a handler 用于特定 App 中的自定义处理逻辑，需要继承实现。

    \sa DApplication::customHandler
*/
void DApplication::setCustomHandler(DAppHandler *handler)
{
    D_D(DApplication);
    d->appHandler = handler;
}

/*!
  \brief 获取设置的 app 定制化处理程序
*/
DAppHandler *DApplication::customHandler()
{
    D_D(DApplication);
    return d->appHandler;
}

/*!
  \brief DApplication::handleHelpAction
  
  Triggered when user clicked the help menu item of this window's titlebar,
  default action is to open the user manual of this program, override this
  method if you want to change the default action.
  
  \brief 用于处理菜单中帮助按钮的点击.

  该函数在用户点击窗口标题栏的帮助按钮时触发。
  默认实现为打开当前程序的帮助手册，子类可以重现实现此函数以覆盖其默认行为。
 */
void DApplication::handleHelpAction()
{
    D_D(DApplication);
    if (customHandler()) {
        d->appHandler->handleHelpAction();
        return;
    }
    DGuiApplicationHelper::instance()->handleHelpAction();
}

/*!
  \brief 用于处理菜单中关于按钮的点击
  
  Triggered when user clicked the about menu item of this window's titlebar,
  default action is to show the about dialog of this window(if there is one),
  override this method if you want to change the default action.
  
  该函数在用户点击窗口标题栏的关于按钮时触发，默认实现为打开程序关于对话框，
  子类可以重现实现此函数以覆盖其默认行为。
 */
void DApplication::handleAboutAction()
{
    D_D(DApplication);
    if (customHandler()) {
        d->appHandler->handleAboutAction();
        return;
    }
    if (d->licenseDialog == nullptr) {
        d->licenseDialog = new DLicenseDialog();
        d->licenseDialog->setFile(d->applicationCreditsFile);
        d->licenseDialog->setContent(d->applicationCreditsContent);
        d->licenseDialog->setLicenseSearchPath(d->licensePath);
        d->licenseDialog->load();
        connect(this, &DApplication::aboutToQuit, this, [this]{
            D_D(DApplication);
            d->licenseDialog->deleteLater();
            d->licenseDialog = nullptr;
        });
    }
    if (d->aboutDialog) {
        d->aboutDialog->activateWindow();
        d->aboutDialog->raise();
        d->aboutDialog->setLicenseEnabled(d->licenseDialog->isValid());
        if (DGuiApplicationHelper::isTabletEnvironment()) {
            d->aboutDialog->exec();
        } else {
            d->aboutDialog->show();
        }
        return;
    }

    DAboutDialog *aboutDialog = new DAboutDialog(activeWindow());
    aboutDialog->setProductName(productName());
    aboutDialog->setProductIcon(productIcon());
    aboutDialog->setVersion(applicationVersion());
    aboutDialog->setDescription(applicationDescription());

    if (!applicationLicense().isEmpty()) {
        aboutDialog->setLicense(translate("DAboutDialog", "%1 is released under %2").arg(productName()).arg(applicationLicense()));
    }
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
    if (!applicationAcknowledgementPage().isEmpty()) {
        aboutDialog->setAcknowledgementLink(applicationAcknowledgementPage());
    }
#endif
    aboutDialog->setAcknowledgementVisible(d->acknowledgementPageVisible);
    aboutDialog->setAttribute(Qt::WA_DeleteOnClose);

    //目前的关于对话框是非模态的,这里的处理是防止关于对话框可以打开多个的情况
    // 不能使用aboutToClose信号 应用能够打开多个的情况下 打开关于后直接关闭程序
    // 此时aboutToColose信号不会触发 再次打开程序并打开关于会出现访问野指针 程序崩溃的情况
    d->aboutDialog = aboutDialog;
    d->aboutDialog->setLicenseEnabled(d->licenseDialog->isValid());
    connect(d->aboutDialog, &DAboutDialog::destroyed, this, [=] {
        d->aboutDialog = nullptr;
    });
    connect(d->aboutDialog, &DAboutDialog::featureActivated, this, [this] {
        featureDisplayDialog()->show();
    });
    connect(d->aboutDialog, &DAboutDialog::licenseActivated, this, [d] {
        d->licenseDialog->activateWindow();
        d->licenseDialog->show();
    });
    if (DGuiApplicationHelper::isTabletEnvironment()) {
        aboutDialog->exec();
    } else {
        aboutDialog->show();
    }
}

/*!
  \brief 用于处理菜单中退出按钮的点击
  
  Triggered when user clicked the exit menu item of this window's titlebar,
  default action is to quit this program, you can try to save your data before
  the program quitting by connecting to the aboutToQuit signal of this application.
  override this method if you want to change the default action.
  
  该函数在用户点击窗口标题栏的关闭按钮时触发，默认行为是退出整个程序，
  子类可以重写此函数以覆盖其行为。
 */
void DApplication::handleQuitAction()
{
    D_D(DApplication);
    if (customHandler()) {
        d->appHandler->handleQuitAction();
        return;
    }

    quit();
}

static inline bool basePrintPropertiesDialog(const QWidget *w)
{
    while (w) {
        if (w->inherits("QPrintPropertiesDialog")
                || w->inherits("QPageSetupDialog")) {
            return true;
        }

        w = w->parentWidget();
    }

    return false;
}

bool DApplication::notify(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::FocusIn) {
        QFocusEvent *fe = static_cast<QFocusEvent*>(event);
        QWidget *widget = qobject_cast<QWidget*>(obj);
        if (widget && fe->reason() == Qt::ActiveWindowFocusReason && !widget->isWindow()
                && ((widget->focusPolicy() & Qt::StrongFocus) != Qt::StrongFocus || qobject_cast<DTitlebar *>(widget)))  {
            // 针对激活窗口所获得的焦点，为了避免被默认给到窗口内部的控件上，此处将焦点还给主窗口并且只设置一次
#define NON_FIRST_ACTIVE "_d_dtk_non_first_active_focus"
            QWidget *top_window = widget->topLevelWidget();
            if (top_window->isWindow() && !top_window->property(NON_FIRST_ACTIVE).toBool()) {
                top_window->setFocus();
                top_window->setProperty(NON_FIRST_ACTIVE, true);
            }
        }
    }

// Qt 6.4 引入了 QPlatformTheme::ThemeHint::ButtonPressKeys
// 可以通过主题插件 themeHint 返回一个按键列表，按钮应该响应列表中的按键
// see https://github.com/linuxdeepin/qt5integration/pull/20
#if QT_VERSION < QT_VERSION_CHECK(6, 4, 0)
    if (event->type() == QEvent::KeyPress && obj == focusWidget()) {
        if (auto keyEvent = dynamic_cast<QKeyEvent *>(event)) {
            if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
                if (auto btn = qobject_cast<QAbstractButton *>(obj)) {
                    Q_EMIT btn->clicked();
                    return true;
                }
            }
        }
    }
#endif

    if (event->type() == QEvent::ApplicationFontChange) {
        // ApplicationFontChange 调用 font() 是 ok 的，如果在 fontChanged 中调用在某些版本中会出现 deadlock
        DFontSizeManager::instance()->setFontGenericPixelSize(static_cast<quint16>(DFontSizeManager::fontPixelSize(font())));
    }

    return QApplication::notify(obj, event);
}

int DtkBuildVersion::value = 0;

DWIDGET_END_NAMESPACE

#include "moc_dapplication.cpp"
