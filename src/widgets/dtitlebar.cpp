// SPDX-FileCopyrightText: 2017 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dtitlebar.h"

#include <mutex>
#include <QDebug>
#include <QMenu>
#include <QHBoxLayout>
#include <QApplication>
#include <QMouseEvent>
#include <QProcess>
#include <QStandardPaths>
#include <QWidgetAction>

#include <DWindowManagerHelper>
#include <DObjectPrivate>
#include <DPlatformTheme>
#include <DConfig>
#include <QScreen>
#include <QWindow>
#include <QActionGroup>
#include <qpa/qplatformwindow.h>

#include "dpalettehelper.h"
#include "dstyleoption.h"
#include "dwindowclosebutton.h"
#include "dwindowmaxbutton.h"
#include "dwindowminbutton.h"
#include "dwindowoptionbutton.h"
#include "dtabletwindowoptionbutton.h"
#include "dwindowquitfullbutton.h"
#include "dplatformwindowhandle.h"
#include "daboutdialog.h"
#include "dapplication.h"
#include "private/dapplication_p.h"
#include "private/dsplitscreen_p.h"
#include "private/dmainwindow_p.h"
#include "dmainwindow.h"
#include "DHorizontalLine"
#include "dimagebutton.h"
#include "dblureffectwidget.h"
#include "dwidgetstype.h"
#include "dlabel.h"
#include "dsizemode.h"
#include "private/dtitlebarsettingsimpl.h"
#include "dtitlebarsettings.h"

DWIDGET_BEGIN_NAMESPACE

#define CHANGESPLITWINDOW_VAR "_d_splitWindowOnScreen"
#define GETSUPPORTSPLITWINDOW_VAR "_d_supportForSplittingWindow"

static inline int DefaultIconHeight() { return DSizeModeHelper::element(24, 32); }
static inline int DefaultExpandButtonHeight() { return DSizeModeHelper::element(48, 48); }

class DTitlebarPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
protected:
    DTitlebarPrivate(DTitlebar *qq);

private:
    void init();
    QWidget *targetWindow();
    // FIXME: get a batter salution
    // hide title will make eventFilter not work, instead set Height to zero
    bool isVisableOnFullscreen();
    void hideOnFullscreen();
    void showOnFullscreen();

    void updateFullscreen();
    void updateButtonsState(Qt::WindowFlags type);
    void updateButtonsFunc();
    void updateCenterArea();

    void handleParentWindowStateChange();
    void handleParentWindowIdChange();
    void _q_toggleWindowState();
    void _q_showMinimized();
    void _q_onTopWindowMotifHintsChanged(quint32 winId);
    void _q_closeWindow();

#ifndef QT_NO_MENU
    void _q_addDefaultMenuItems();
    void _q_helpActionTriggered();
    void _q_feedbackActionTriggered();
    void _q_toolBarActionTriggered();
    void _q_aboutActionTriggered();
    void _q_quitActionTriggered();
    void _q_switchThemeActionTriggered(QAction*action);
#endif

    void setIconVisible(bool visible);
    void updateTabOrder();
    void showSplitScreenWidget();
    void hideSplitScreenWidget();
    void updateTitleBarSize()
    {
        if (optionButton)
            optionButton->setIconSize(QSize(titlebarHeight, titlebarHeight));
        if (minButton)
            minButton->setIconSize(QSize(titlebarHeight, titlebarHeight));
        if (maxButton)
            maxButton->setIconSize(QSize(titlebarHeight, titlebarHeight));
        if (closeButton)
            closeButton->setIconSize(QSize(titlebarHeight, titlebarHeight));
        if (quitFullButton)
            quitFullButton->setIconSize(QSize(titlebarHeight, titlebarHeight));
        if (expandButton)
            expandButton->setIconSize(QSize(DefaultExpandButtonHeight(), DefaultExpandButtonHeight()));
        if (iconLabel)
            iconLabel->setIconSize(QSize(DefaultIconHeight(), DefaultIconHeight()));

        D_Q(DTitlebar);
        q->setFixedHeight(titlebarHeight);
        q->setMinimumHeight(titlebarHeight);
    }

    void setFixedButtonsEnabled(bool isEnabled);

    void updateTitlebarHeight();

    QHBoxLayout         *mainLayout;
    QWidget             *leftArea;
    QHBoxLayout         *leftLayout;
    QWidget             *rightArea;
    QHBoxLayout         *rightLayout;
    DLabel              *centerArea;
    QHBoxLayout         *centerLayout;
    DIconButton         *iconLabel;
    QWidget             *buttonArea;
    DWindowMinButton    *minButton;
    DWindowMaxButton    *maxButton;
    DWindowCloseButton  *closeButton;
    DIconButton         *optionButton;
    DWindowQuitFullButton *quitFullButton;
    DLabel              *titleLabel;
    QWidget             *customWidget = nullptr;

    DHorizontalLine     *separatorTop;
    DHorizontalLine     *separator;

    DBlurEffectWidget   *blurWidget = nullptr;
    QPointer<DSplitScreenWidget> splitWidget = nullptr;
    DSidebarHelper      *sidebarHelper = nullptr;
    DIconButton         *expandButton = nullptr;

    int                 titlebarHeight = 50;
    DConfig             *uiPreferDonfig = nullptr;

#ifndef QT_NO_MENU
    QMenu               *menu             = Q_NULLPTR;
    QAction             *helpAction       = Q_NULLPTR;
    QAction             *feedbackAction   = Q_NULLPTR;
    QAction             *toolbarAction    = Q_NULLPTR;
    QAction             *aboutAction      = Q_NULLPTR;
    QAction             *quitAction       = Q_NULLPTR;
    bool                canSwitchTheme    = true;
    QAction             *themeSeparator   = nullptr;
    QMenu               *switchThemeMenu  = nullptr;
    QAction             *autoThemeAction  = nullptr;
    QAction             *lightThemeAction = nullptr;
    QAction             *darkThemeAction  = nullptr;
#endif

    QWindow            *targetWindowHandle = Q_NULLPTR;

    Qt::WindowFlags     disableFlags;
    bool                mousePressed    = false;
    bool                embedMode       = false;
    bool                autoHideOnFullscreen = false;
    bool                fullScreenButtonVisible = true;
    bool                splitScreenWidgetEnable = true;
    QTimer              *maxButtonPressAndHoldTimer = nullptr;
    QWidget             *sidebarBackgroundWidget = nullptr;
    DTitlebarSettingsImpl *titlebarSettingsImpl = nullptr;
    DTitlebarSettings *titlebarSettings = nullptr;
    Q_DECLARE_PUBLIC(DTitlebar)
};

DTitlebarPrivate::DTitlebarPrivate(DTitlebar *qq)
    : DObjectPrivate(qq)
    , quitFullButton(nullptr)
{
}

void DTitlebarPrivate::init()
{
    D_Q(DTitlebar);

    mainLayout      = new QHBoxLayout;
    leftArea        = new QWidget;
    leftLayout      = new QHBoxLayout(leftArea);
    rightArea       = new QWidget;
    rightLayout     = new QHBoxLayout;
    centerArea      = new DLabel(q);
    centerLayout    = new QHBoxLayout(centerArea);
    iconLabel       = new DIconButton(q);
    buttonArea      = new QWidget;
    minButton       = new DWindowMinButton;
    maxButton       = new DWindowMaxButton;
    closeButton     = new DWindowCloseButton;
    maxButtonPressAndHoldTimer = new QTimer(q);
    maxButtonPressAndHoldTimer->setSingleShot(true);

    if (DGuiApplicationHelper::isTabletEnvironment()) {
        optionButton = new DTabletWindowOptionButton;
    } else {
        optionButton = new DWindowOptionButton;
    }

    auto config = new DConfig("org.deepin.dtk.preference", "", q);
    bool isUpdated = config->value("featureUpdated", false).toBool();
    DStyle::setRedPointVisible(optionButton, isUpdated);

    uiPreferDonfig = new DConfig("org.deepin.dtk.preference", "", q);
    updateTitlebarHeight();

    separatorTop    = new DHorizontalLine(q);
    separator       = new DHorizontalLine(q);
    titleLabel      = centerArea;
    titleLabel->setElideMode(Qt::ElideMiddle);

    minButton->installEventFilter(q);
    maxButton->installEventFilter(q);
    closeButton->installEventFilter(q);
    optionButton->installEventFilter(q);

    optionButton->setObjectName("DTitlebarDWindowOptionButton");
    optionButton->setAccessibleName("DTitlebarDWindowOptionButton");
    minButton->setObjectName("DTitlebarDWindowMinButton");
    minButton->setAccessibleName("DTitlebarDWindowMinButton");
    maxButton->setObjectName("DTitlebarDWindowMaxButton");
    maxButton->setAccessibleName("DTitlebarDWindowMaxButton");
    maxButton->setAttribute(Qt::WA_AlwaysShowToolTips);
    closeButton->setObjectName("DTitlebarDWindowCloseButton");
    closeButton->setAccessibleName("DTitlebarDWindowCloseButton");


    iconLabel->setWindowFlags(Qt::WindowTransparentForInput);
    iconLabel->setAttribute( Qt::WA_TransparentForMouseEvents, true);
    iconLabel->setFocusPolicy(Qt::NoFocus);
    iconLabel->setAccessibleName("DTitlebarIconLabel");
    iconLabel->setFlat(true);
    // 默认无图标，所以隐藏
    iconLabel->hide();

    leftArea->setWindowFlag(Qt::WindowTransparentForInput);
    leftArea->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    leftArea->setAccessibleName("DTitlebarLeftArea");
    leftLayout->setContentsMargins(0, 0, 0, 0);

    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerArea->setText(qApp->applicationName());
    centerArea->setWindowFlags(Qt::WindowTransparentForInput);
    centerArea->setFrameShape(QFrame::NoFrame);
    centerArea->setAutoFillBackground(false);
    centerArea->setBackgroundRole(QPalette::NoRole);
    centerArea->setAlignment(Qt::AlignCenter);
    centerArea->setAccessibleName("DTitlebarCenterArea");

    buttonArea->setWindowFlag(Qt::WindowTransparentForInput);
    buttonArea->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    buttonArea->setAccessibleName("DTitlebarButtonArea");
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonArea);
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setSpacing(0);
    buttonLayout->addWidget(optionButton);
    buttonLayout->addWidget(minButton);
    buttonLayout->addWidget(maxButton);
    if (!DGuiApplicationHelper::isTabletEnvironment()) {
        quitFullButton  = new DWindowQuitFullButton;
        quitFullButton->installEventFilter(q);
        quitFullButton->setObjectName("DTitlebarDWindowQuitFullscreenButton");
        quitFullButton->setAccessibleName("DTitlebarDWindowQuitFullscreenButton");
        quitFullButton->hide();
        buttonLayout->addWidget(quitFullButton);
    }
    buttonLayout->addWidget(closeButton);

    rightArea->setWindowFlag(Qt::WindowTransparentForInput);
    rightArea->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    rightArea->setAccessibleName("DTitlebarRightArea");
    rightLayout->setContentsMargins(0, 0, 0, 0);
    auto rightAreaLayout = new QHBoxLayout(rightArea);
    rightAreaLayout->setContentsMargins(0, 0, 0, 0);
    rightAreaLayout->setContentsMargins(0, 0, 0, 0);
    rightAreaLayout->setSpacing(0);
    rightAreaLayout->addLayout(rightLayout);
    rightAreaLayout->addWidget(buttonArea);

    separatorTop->setFixedHeight(1);
    separatorTop->setAccessibleName("DTitlebarTopHorizontalLine");
    separatorTop->hide();
    separatorTop->setWindowFlags(Qt::WindowTransparentForInput);

    separator->setFixedHeight(1);
    separator->setAccessibleName("DTitlebarHorizontalLine");
    separator->hide();
    separator->setWindowFlags(Qt::WindowTransparentForInput);

    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(leftArea, 0, Qt::AlignLeft);
    mainLayout->addWidget(rightArea, 0, Qt::AlignRight);

    q->setLayout(mainLayout);

    if (!DGuiApplicationHelper::isTabletEnvironment()) {
        q->connect(quitFullButton, &DWindowQuitFullButton::clicked, q, [ = ]() {
            bool isFullscreen = targetWindow()->windowState().testFlag(Qt::WindowFullScreen);
            if (isFullscreen) {
                targetWindow()->showNormal();
            } else {
                targetWindow()->showFullScreen();
            }
        });
    }
    q->connect(optionButton, &DIconButton::clicked, q, &DTitlebar::optionClicked);
    q->connect(DWindowManagerHelper::instance(), SIGNAL(windowMotifWMHintsChanged(quint32)),
               q, SLOT(_q_onTopWindowMotifHintsChanged(quint32)));
    q->connect(DGuiApplicationHelper::instance()->systemTheme(), &DPlatformTheme::iconThemeNameChanged, q, [ = ]() {
        iconLabel->update();
    });
    q->connect(maxButtonPressAndHoldTimer, &QTimer::timeout, q, [this]() {
        showSplitScreenWidget();
        if (splitWidget && splitWidget->isVisible())
            splitWidget->isMaxButtonPressAndHold = true;
    });
    if (isUpdated) {
        q->connect(config, &DConfig::valueChanged, q, [config, this](const QString &key){
            if (key == "featureUpdated") {
                auto result = config->value("featureUpdated", false);
                DStyle::setRedPointVisible(optionButton, result.toBool());
                optionButton->update();
                config->deleteLater();
            }
        });
    }
    q->connect(uiPreferDonfig, &DConfig::valueChanged, q, [this](const QString &key){
        if (key == "titlebarHeight") {
            updateTitlebarHeight();
            updateTitleBarSize();
        }
    });

    // 默认需要构造一个空的选项菜单
    q->setMenu(new QMenu(q));
    q->setFrameShape(QFrame::NoFrame);
    q->setBackgroundRole(QPalette::Base);
    q->setAutoFillBackground(true);
    q->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    // 确保点击标题栏后输入框能失去焦点
    // 另外，让标题栏接收焦点，还是为了避免一个focus控件隐藏时，会把焦点转移给标题栏上的按钮控件
    q->setFocusPolicy(Qt::StrongFocus);

    auto noTitlebarEnabled = []{
        if (DGuiApplicationHelper::testAttribute(DGuiApplicationHelper::IsWaylandPlatform)) {
            return true;
        }

        QFunctionPointer enableNoTitlebar = qApp->platformFunction("_d_isEnableNoTitlebar");
        bool enabled = qApp->platformName() == "dwayland" || qApp->property("_d_isDwayland").toBool();
        return enabled && enableNoTitlebar != nullptr;
    };
    // fix wayland 下显示了两个应用图标，系统标题栏 和 dtk标题栏 均显示应用图标
    q->setEmbedMode(!(DApplication::isDXcbPlatform()|| noTitlebarEnabled()));

    updateTitleBarSize();
}

QWidget *DTitlebarPrivate::targetWindow()
{
    D_Q(DTitlebar);
    return q->topLevelWidget()->window();
}

bool DTitlebarPrivate::isVisableOnFullscreen()
{
    D_Q(DTitlebar);
    return !q->property("_restore_height").isValid();
}

void DTitlebarPrivate::hideOnFullscreen()
{
    D_Q(DTitlebar);
    if (q->height() > 0) {
        q->setProperty("_restore_height", q->height());
    }
    q->setFixedHeight(0);
}

void DTitlebarPrivate::showOnFullscreen()
{
    D_Q(DTitlebar);
    if (q->property("_restore_height").isValid()) {
        q->setFixedHeight(q->property("_restore_height").toInt());
        q->setProperty("_restore_height", QVariant());
    }
}

void DTitlebarPrivate::updateFullscreen()
{
    D_Q(DTitlebar);

    if (!autoHideOnFullscreen) {
        return;
    }

    bool isFullscreen = targetWindow()->windowState().testFlag(Qt::WindowFullScreen);
    auto mainWindow = qobject_cast<DMainWindow *>(targetWindow());
    if (!isFullscreen) {
        if (!DGuiApplicationHelper::isTabletEnvironment())
            quitFullButton->hide();
        mainWindow->setMenuWidget(q);
        showOnFullscreen();
    } else {
        // must set to empty
        if (!DGuiApplicationHelper::isTabletEnvironment())
            quitFullButton->show();
        if (mainWindow->menuWidget()) {
            mainWindow->menuWidget()->setParent(nullptr);
            mainWindow->setMenuWidget(Q_NULLPTR);
        }

        q->setParent(mainWindow);
        q->show();
        hideOnFullscreen();
    }
}

void DTitlebarPrivate::updateButtonsState(Qt::WindowFlags type)
{
    D_Q(DTitlebar);
    bool useDXcb = DPlatformWindowHandle::isEnabledDXcb(targetWindow()) ||
                   DGuiApplicationHelper::testAttribute(DGuiApplicationHelper::IsWaylandPlatform);
    bool isFullscreen = targetWindow()->windowState().testFlag(Qt::WindowFullScreen);

//    bool forceShow = !useDXcb;
    bool forceShow = false;
#ifndef Q_OS_LINUX
    forceShow = false;
#endif

    bool showTitle = (type.testFlag(Qt::WindowTitleHint) || forceShow) && !embedMode;
    if (titleLabel) {
        if (showTitle) {
            titleLabel->setText(q->property("_dtk_title").toString());
        } else {
            q->setProperty("_dtk_title", titleLabel->text());
            titleLabel->clear();
        }
    }

    // Never show in embed/fullscreen
    bool forceHide = (!useDXcb) || embedMode || isFullscreen;

    bool showMin = (type.testFlag(Qt::WindowMinimizeButtonHint) || forceShow) && !forceHide;
    minButton->setVisible(showMin);

    bool allowResize = true;

    if (QWidget * window = q->window()) {
        if (window->windowHandle()) {
            auto functions_hints = DWindowManagerHelper::getMotifFunctions(window->windowHandle());
            allowResize = functions_hints.testFlag(DWindowManagerHelper::FUNC_RESIZE);
        }

        if (allowResize && Q_LIKELY(q->testAttribute(Qt::WA_WState_Created)) && Q_LIKELY(q->testAttribute(Qt::WA_Resized)))
            allowResize = (window->maximumSize() != window->minimumSize());
    }

    bool showMax = (type.testFlag(Qt::WindowMaximizeButtonHint) || forceShow) && !forceHide && allowResize;
    bool showClose = type.testFlag(Qt::WindowCloseButtonHint) && useDXcb;
    bool showQuit = isFullscreen && useDXcb && fullScreenButtonVisible;
    maxButton->setVisible(showMax);
    closeButton->setVisible(showClose);

    if (!DGuiApplicationHelper::isTabletEnvironment())
        quitFullButton->setVisible(showQuit);
    //    qDebug() << "max:"
    //             << "allowResize" << allowResize
    //             << "useDXcb" << useDXcb
    //             << "forceHide" << forceHide
    //             << "type.testFlag(Qt::WindowMaximizeButtonHint)" << type.testFlag(Qt::WindowMaximizeButtonHint);
}

void DTitlebarPrivate::updateButtonsFunc()
{
    // TASK-18145 (bug-17474) do not setMotifFunctions on wayland
    if (!targetWindowHandle) {
        return;
    }
    if (!qgetenv("WAYLAND_DISPLAY").isEmpty()) {
        closeButton->setEnabled(!disableFlags.testFlag(Qt::WindowCloseButtonHint));
        return;
    }

    // 根据 disableFlags 更新窗口标志，而标题栏上具体按钮的开启/禁用状态只根据
    // 窗口标志改变后做更新，且窗口标志改变后也会同步更新 disableFlags 的值，
    // 也就是说，实际上窗口按钮的状态只受窗口标志影响，而 setDisableFlags
    // 只是单纯的记录新的 disableFlags 的值，由于调用 setDisableFlags
    // 是应用中的主动行为，所以同时也会根据这个值更新窗口标志。
    DWindowManagerHelper::setMotifFunctions(
        targetWindowHandle,
        DWindowManagerHelper::FUNC_MAXIMIZE,
        !disableFlags.testFlag(Qt::WindowMaximizeButtonHint));
    DWindowManagerHelper::setMotifFunctions(
        targetWindowHandle,
        DWindowManagerHelper::FUNC_MINIMIZE,
        !disableFlags.testFlag(Qt::WindowMinimizeButtonHint));
    DWindowManagerHelper::setMotifFunctions(
        targetWindowHandle,
        DWindowManagerHelper::FUNC_CLOSE,
        !disableFlags.testFlag(Qt::WindowCloseButtonHint));
}

void DTitlebarPrivate::updateCenterArea()
{
    D_QC(DTitlebar);

    if (centerArea->isHidden()) {
        return;
    }

    int padding = qMax(leftArea->width(), rightArea->width());
    QRect rect(0, 0, q->width() - 2 * padding, q->height());
    rect.moveCenter(q->rect().center());
    centerArea->setGeometry(rect);
}

void DTitlebarPrivate::handleParentWindowStateChange()
{
    maxButton->setMaximized(targetWindow()->windowState().testFlag(Qt::WindowMaximized));
    updateFullscreen();
    updateButtonsState(targetWindow()->windowFlags());
}

//!
//! \brief DTitlebarPrivate::handleParentWindowIdChange
//! Them WindowStateChnage Event will miss some state changed message,
//! So use windowHandle::windowStateChanged instead
void DTitlebarPrivate::handleParentWindowIdChange()
{
    if (!targetWindowHandle) {
        targetWindowHandle = targetWindow()->windowHandle();

        updateButtonsFunc();
    } else if (targetWindow()->windowHandle() != targetWindowHandle) {
        // Parent change???, show never here
        qWarning() << "targetWindowHandle change" << targetWindowHandle << targetWindow()->windowHandle();
    }
}

void DTitlebarPrivate::_q_toggleWindowState()
{
    if (splitWidget && splitWidget->isMaxButtonPressAndHold) {
        splitWidget->isMaxButtonPressAndHold = false;
        return;
    }

    QWidget *parentWindow = targetWindow();

    if (!parentWindow || disableFlags.testFlag(Qt::WindowMaximizeButtonHint)) {
        return;
    }

    if (parentWindow->isMaximized()) {
        parentWindow->showNormal();
    } else if (!parentWindow->isFullScreen()
               && (maxButton->isVisible())) {
        parentWindow->showMaximized();
    }
}

void DTitlebarPrivate::_q_closeWindow()
{
    if (targetWindow()->windowHandle()) {
        targetWindow()->windowHandle()->close();
    }
}

void DTitlebarPrivate::_q_showMinimized()
{
    targetWindow()->showMinimized();
}

#if QT_VERSION < QT_VERSION_CHECK(5, 7, 0)
static Qt::WindowFlags &setWindowFlag(Qt::WindowFlags &flags, Qt::WindowType type, bool on)
{
    return on ? (flags |= type) : (flags &= ~int(type));
}
#endif

void DTitlebarPrivate::_q_onTopWindowMotifHintsChanged(quint32 winId)
{
    D_QC(DTitlebar);

    if (!DPlatformWindowHandle::isEnabledDXcb(targetWindow())) {
        q->disconnect(DWindowManagerHelper::instance(), SIGNAL(windowMotifWMHintsChanged(quint32)),
                      q, SLOT(_q_onTopWindowMotifHintsChanged(quint32)));
        return;
    }

    if (winId != q->window()->internalWinId()) {
        return;
    }

    DWindowManagerHelper::MotifDecorations decorations_hints = DWindowManagerHelper::getMotifDecorations(q->window()->windowHandle());
    DWindowManagerHelper::MotifFunctions functions_hints = DWindowManagerHelper::getMotifFunctions(q->window()->windowHandle());

    if (titleLabel) {
        titleLabel->setVisible(decorations_hints.testFlag(DWindowManagerHelper::DECOR_TITLE));
    }

    updateButtonsState(targetWindow()->windowFlags());

    bool enableMin = functions_hints.testFlag(DWindowManagerHelper::FUNC_MINIMIZE);
    bool enableMax = functions_hints.testFlag(DWindowManagerHelper::FUNC_MAXIMIZE) && functions_hints.testFlag(DWindowManagerHelper::FUNC_RESIZE);
    bool enableClose;
    if (!qEnvironmentVariableIsEmpty("WAYLAND_DISPLAY")) {
        enableClose = !disableFlags.testFlag(Qt::WindowCloseButtonHint);
    } else {
        enableClose = functions_hints.testFlag(DWindowManagerHelper::FUNC_CLOSE);
    }
    if(q->window()->isEnabled()) {
        minButton->setEnabled(enableMin);
        maxButton->setEnabled(enableMax);
        closeButton->setEnabled(enableClose);
    }
    // sync button state
#if QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
    disableFlags.setFlag(Qt::WindowMinimizeButtonHint, !enableMin);
    disableFlags.setFlag(Qt::WindowMaximizeButtonHint, !enableMax);
    disableFlags.setFlag(Qt::WindowCloseButtonHint, !enableClose);
#else
    setWindowFlag(disableFlags, Qt::WindowMinimizeButtonHint, !enableMin);
    setWindowFlag(disableFlags, Qt::WindowMaximizeButtonHint, !enableMax);
    setWindowFlag(disableFlags, Qt::WindowCloseButtonHint, !enableClose);
#endif
}

#ifndef QT_NO_MENU

void DTitlebarPrivate::_q_addDefaultMenuItems()
{
    D_Q(DTitlebar);

    // add switch theme sub menu
    if (!switchThemeMenu && DGuiApplicationHelper::testAttribute(DGuiApplicationHelper::Attribute::IsDeepinPlatformTheme)) {
        bool disableDtkSwitchThemeMenu = qEnvironmentVariableIsSet("KLU_DISABLE_MENU_THEME");
        if (!disableDtkSwitchThemeMenu) {
            switchThemeMenu = new QMenu(qApp->translate("TitleBarMenu", "Theme"), menu);
            switchThemeMenu->setAccessibleName("DTitlebarThemeMenu");
            lightThemeAction = switchThemeMenu->addAction(qApp->translate("TitleBarMenu", "Light Theme"));
            darkThemeAction = switchThemeMenu->addAction(qApp->translate("TitleBarMenu", "Dark Theme"));
            autoThemeAction = switchThemeMenu->addAction(qApp->translate("TitleBarMenu", "System Theme"));

            autoThemeAction->setCheckable(true);
            lightThemeAction->setCheckable(true);
            darkThemeAction->setCheckable(true);

            QActionGroup *group = new QActionGroup(switchThemeMenu);
            group->addAction(autoThemeAction);
            group->addAction(lightThemeAction);
            group->addAction(darkThemeAction);


            QObject::connect(group, SIGNAL(triggered(QAction*)),
                             q, SLOT(_q_switchThemeActionTriggered(QAction*)));

            menu->addMenu(switchThemeMenu);
            themeSeparator = menu->addSeparator();

            switchThemeMenu->menuAction()->setVisible(canSwitchTheme);

            themeSeparator->setVisible(DGuiApplicationHelper::isTabletEnvironment() ? false : canSwitchTheme);
        }
    }

    // add help menu item.
    if (!helpAction) {
        // init DGuiApplicationHelperPrivate::hasManual
        static std::once_flag onceFlag;
        std::call_once(onceFlag, DApplicationPrivate::isUserManualExists);

        helpAction = new QAction(qApp->translate("TitleBarMenu", "Help"), menu);
        QObject::connect(helpAction, SIGNAL(triggered(bool)), q, SLOT(_q_helpActionTriggered()));
        menu->addAction(helpAction);
        helpAction->setVisible(false);
    }

    // add feedback menu item for deepin or uos application
    if (!feedbackAction && qApp->organizationName() == "deepin" && !QStandardPaths::findExecutable("deepin-feedback").isEmpty()) {
        feedbackAction = new QAction(qApp->translate("TitleBarMenu", "Feedback"), menu);
        QObject::connect(feedbackAction, SIGNAL(triggered(bool)), q, SLOT(_q_feedbackActionTriggered()));
        menu->addAction(feedbackAction);
    }

    // add toolbarAction menu item for deepin or uos application
    if (titlebarSettingsImpl && titlebarSettingsImpl->isValid() && !toolbarAction) {
        toolbarAction = new QAction(qApp->translate("TitleBarMenu", "Custom toolbar"), menu);
        toolbarAction->setObjectName("TitlebarSettings");
        QObject::connect(toolbarAction, SIGNAL(triggered(bool)), q, SLOT(_q_toolBarActionTriggered()));
        menu->addAction(toolbarAction);
    }

    // add about menu item.
    if (!aboutAction) {
        aboutAction = new QAction(qApp->translate("TitleBarMenu", "About"), menu);
        QObject::connect(aboutAction, SIGNAL(triggered(bool)), q, SLOT(_q_aboutActionTriggered()));
        menu->addAction(aboutAction);
    }

    // add quit menu item.
    if (!quitAction) {
        quitAction = new QAction(qApp->translate("TitleBarMenu", "Exit"), menu);
        QObject::connect(quitAction, SIGNAL(triggered(bool)), q, SLOT(_q_quitActionTriggered()));
        if (!DGuiApplicationHelper::isTabletEnvironment()) {
            menu->addAction(quitAction);
        }
    }
}

void DTitlebarPrivate::_q_helpActionTriggered()
{
    DApplication *dapp = qobject_cast<DApplication *>(qApp);
    if (dapp) {
        dapp->handleHelpAction();
    }
}

void DTitlebarPrivate::_q_feedbackActionTriggered() {
    QProcess::startDetached("deepin-feedback", { qApp->applicationName() });
}

void DTitlebarPrivate::_q_toolBarActionTriggered()
{
    D_Q(DTitlebar);

    auto toolBarEditPanel = titlebarSettingsImpl->toolsEditPanel();
    if (toolBarEditPanel->minimumWidth() >= q->width()) {
        toolBarEditPanel->setParent(nullptr);
        int x = q->mapToGlobal(q->pos()).x() - (toolBarEditPanel->width()- q->width()) / 2 ;
        toolBarEditPanel->move(x, q->mapToGlobal(q->pos()).y() + q->height());
    } else {
        toolBarEditPanel->setParent(q->parentWidget());
        toolBarEditPanel->move(0, q->height());
        toolBarEditPanel->resize(q->width(), q->parentWidget()->height() * 70 / 100);
    }
    toolBarEditPanel->installEventFilter(q);

    titlebarSettingsImpl->showEditPanel();
}

void DTitlebarPrivate::_q_aboutActionTriggered()
{
    DApplication *dapp = qobject_cast<DApplication *>(qApp);
    if (dapp) {
        dapp->handleAboutAction();
    }
}

void DTitlebarPrivate::_q_quitActionTriggered()
{
    DApplication *dapp = qobject_cast<DApplication *>(qApp);
    if (dapp) {
        dapp->handleQuitAction();
    }
}

void DTitlebarPrivate::_q_switchThemeActionTriggered(QAction *action)
{
    DGuiApplicationHelper::ColorType type = DGuiApplicationHelper::UnknownType;

    if (action == lightThemeAction) {
        type = DGuiApplicationHelper::LightType;
    } else if (action == darkThemeAction) {
        type = DGuiApplicationHelper::DarkType;
    }

    DGuiApplicationHelper::instance()->setPaletteType(type);
}

void DTitlebarPrivate::setIconVisible(bool visible)
{
    if (iconLabel->isVisible() == visible)
        return;

    if (visible) {
        if (dynamic_cast<QSpacerItem *>(leftLayout->itemAt(0)))
            delete leftLayout->takeAt(0);
            
        leftLayout->insertSpacing(0, 10);
        leftLayout->insertWidget(1, iconLabel, 0, Qt::AlignLeading | Qt::AlignVCenter);
        iconLabel->show();
    } else {
        iconLabel->hide();
        // 从布局中移除图标相关的东西
        delete leftLayout->takeAt(0);
        delete leftLayout->takeAt(1);
    }
}

void DTitlebarPrivate::updateTabOrder()
{
    D_Q(DTitlebar);

    QList<QWidget *> orderWidget;
    QList<QHBoxLayout *> orderLayout;
    orderLayout << leftLayout << centerLayout << rightLayout;

    //查找 leftLayout、centerLayout、rightLayout 三个区域中有 TabFocus 属性的 widget
    for (QHBoxLayout * lyt : orderLayout) {
        if (!lyt) {
            continue;
        }

        for (int i = 0; i < lyt->count(); ++i) {
            QWidget *wdg = lyt->itemAt(i)->widget();
            if (wdg && (wdg->focusPolicy() & Qt::FocusPolicy::TabFocus)) {
                orderWidget.append(wdg);
            }
        }
    }

    if (orderWidget.isEmpty()) {
        return;
    }

    //对筛选出来的 widget 重新设置 taborder
    QWidget::setTabOrder(q, orderWidget.first());
    for (int i = 0; i < orderWidget.count() - 1; ++i) {
        QWidget::setTabOrder(orderWidget.at(i), orderWidget.at(i + 1));
    }
}

void DTitlebarPrivate::showSplitScreenWidget()
{
    D_Q(DTitlebar);

    if (!splitScreenWidgetEnable)
        return;

    if (disableFlags.testFlag(Qt::WindowMaximizeButtonHint))
        return;

    // 应产品要求 2D 模式下不对窗口分屏做任何显示
    if (auto wmHelper = DWindowManagerHelper::instance()) {
        if (!wmHelper->hasComposite())
            return;
    }

    // 窗管不支持分屏时，不显示分屏菜单
    if (!Q_LIKELY(DSplitScreenWidget::supportSplitScreenByWM(q->window())))
        return;

    if (!splitWidget) {
        splitWidget = new DSplitScreenWidget(q->window());
    }

    if (splitWidget->isVisible())
        return;

    QRect maxBtnRect = QRect(maxButton->mapToGlobal(maxButton->rect().topLeft()), maxButton->rect().size());

    QRect rect;
    if (QScreen *screen = QGuiApplication::screenAt(QCursor::pos())) {
        rect = screen->geometry();
    } else {
        rect = QGuiApplication::primaryScreen()->geometry();
    }

    int targetX = maxBtnRect.center().x() - splitWidget->width() / 2;
    int targetY = maxBtnRect.bottom();

    if (int outRightLen = maxBtnRect.center().x() - rect.x() + splitWidget->width() / 2 - rect.width(); outRightLen > 0) {    // 超出右边缘
        targetX -= outRightLen;
    } else if (int outLeftLen = splitWidget->width() / 2 - maxBtnRect.center().x() + rect.x(); outLeftLen > 0) {    // 超出左边缘
        targetX += outLeftLen;
    }

    if (maxBtnRect.bottom() + splitWidget->height() - rect.y() > rect.height()) {   // 超出下边缘
        targetY  -= maxButton->rect().height() + splitWidget->height();
    } 

    splitWidget->show(QPoint(targetX, targetY));
}

void DTitlebarPrivate::hideSplitScreenWidget()
{
    if (!splitWidget)
        return;

    if (splitWidget->isHidden())
        return;

    splitWidget->hide();
}

#endif

void DTitlebarPrivate::setFixedButtonsEnabled(bool isEnabled)
{
    maxButton->setEnabled(isEnabled);
    minButton->setEnabled(isEnabled);
    closeButton->setEnabled(isEnabled);
    optionButton->setEnabled(isEnabled);
}

void DTitlebarPrivate::updateTitlebarHeight()
{
    titlebarHeight = uiPreferDonfig->value("titlebarHeight").toInt();
    // 配置项默认值是-1，从配置读取进来的值超出0-100的范围，通过模式获取值，否则使用获取的配置值
    if (titlebarHeight <= 0 || titlebarHeight > 100)
        titlebarHeight = DSizeModeHelper::element(40, 50);
}

/*!
  @~english
  \class Dtk::Widget::DTitlebar
  \inmodule dtkwidget
  \brief The DTitlebar class is an universal title bar on the top of windows.Usually you don't need to construct a DTitlebar instance by your self, you
  can get an DTitlebar instance by DMainWindow::titlebar .
  \param[in] parent is the parent widget to be attached on.
 */

/*!
  @~english
  \brief This function provides to create an default widget with icon/title/ and buttons
  \param[in] parent 父控件指针
 */
DTitlebar::DTitlebar(QWidget *parent) :
    QFrame(parent),
    DObject(*new DTitlebarPrivate(this))
{
    if (DApplication::buildDtkVersion() < DTK_VERSION_CHECK(2, 0, 6, 1)) {
        setBackgroundTransparent(true);
    }

    D_D(DTitlebar);
    d->init();

    // 默认只在普通窗口中显示窗口菜单按钮
    if (parent && parent->window()->windowType() != Qt::Window) {
        d->optionButton->hide();
    }
}

#ifndef QT_NO_MENU
/*!
  @~english
  \brief DTitlebar::menu holds the QMenu object attached to this title bar.
  \return the QMenu object it holds, returns null if there's no one set.
 */
QMenu *DTitlebar::menu() const
{
    D_DC(DTitlebar);

    return d->menu;
}

/*!
  @~english
  \brief DTitlebar::setMenu attaches a QMenu object to the title bar.
  \param[in] menu is the target menu.
 */
void DTitlebar::setMenu(QMenu *menu)
{
    D_D(DTitlebar);

    d->menu = menu;
    if (d->menu) {
        d->menu->setAccessibleName("DTitlebarMainMenu");
        disconnect(this, &DTitlebar::optionClicked, 0, 0);
        connect(this, &DTitlebar::optionClicked, this, &DTitlebar::showMenu);

        if (DGuiApplicationHelper::isTabletEnvironment()) {
            if (QScreen *screen = QGuiApplication::primaryScreen()) {
                disconnect(screen, &QScreen::primaryOrientationChanged, this, 0);
                connect(screen, &QScreen::primaryOrientationChanged, this, [ = ] {
                    d->menu->move(d->optionButton->mapToGlobal(d->optionButton->rect().bottomLeft()));
                });
            }
        }
    }
}

#endif

/*!
  @~english
  \brief DTitlebar::customWidget, One can set customized widget to show some extra widgets on the title bar.
  \return the customized widget used in this title bar.
  \param[in] Dtk::Widget::DTitlebar::setCustomWidget()
 */
QWidget *DTitlebar::customWidget() const
{
    D_DC(DTitlebar);

    return d->customWidget;
}

#ifndef QT_NO_MENU
/*!
  @~english
  \brief DTitlebar::showMenu pop the menu of application on titlebar.
 */
void DTitlebar::showMenu()
{
    D_D(DTitlebar);

    if (d->helpAction)
        d->helpAction->setVisible(DApplicationPrivate::isUserManualExists());

    if (d->menu) {
        // 更新主题选中的项
        if (d->switchThemeMenu) {
            QAction *action;

            switch (DGuiApplicationHelper::instance()->paletteType()) {
            case DGuiApplicationHelper::LightType:
                action = d->lightThemeAction;
                break;
            case DGuiApplicationHelper::DarkType:
                action = d->darkThemeAction;
                break;
            default:
                action = d->autoThemeAction;
                break;
            }

            action->setChecked(true);
        }

        DConfig config("org.deepin.dtk.preference");
        bool isUpdated = config.value("featureUpdated", false).toBool();
        DStyle::setRedPointVisible(d->aboutAction, isUpdated);

        d->menu->exec(d->optionButton->mapToGlobal(d->optionButton->rect().bottomLeft()));
        d->optionButton->update(); // FIX: bug-25253 sometimes optionButton not udpate after menu exec(but why?)
    }
}
#endif

void DTitlebar::showEvent(QShowEvent *event)
{
    if (qobject_cast<QDialog *>(topLevelWidget())) {
        // QDialog::setvisible will send QFocusEvent (QEvent::FocusIn, Qt::TabFocusReason);
        // that will cause DTitlebar focus to next widget(see DTitlebar::event)
        if (topLevelWidget()->focusWidget() == this)
            clearFocus();
    }

    //fix the width issue and process menu
    D_D(DTitlebar);
    d->separatorTop->setFixedWidth(width());
    d->separatorTop->move(0, 0);
    d->separator->setFixedWidth(width());
    int x = (d->sidebarHelper && d->sidebarHelper->expanded()) ? d->sidebarHelper->width() : 0;
    d->separator->move(x, height() - d->separator->height());

#ifndef QT_NO_MENU
    // 默认菜单需要在showevent添加，否则`menu`接口返回空actions，导致接口逻辑不兼容
    d->_q_addDefaultMenuItems();
#endif

    QWidget::showEvent(event);

    if (DPlatformWindowHandle::isEnabledDXcb(window())) {
        d->_q_onTopWindowMotifHintsChanged(
            static_cast<quint32>(window()->internalWinId()));
    }

    d->updateCenterArea();
}

void DTitlebar::mousePressEvent(QMouseEvent *event)
{
    D_D(DTitlebar);
    d->mousePressed = (event->button() == Qt::LeftButton);

    if (event->button() == Qt::RightButton) {
        DWindowManagerHelper::popupSystemWindowMenu(window()->windowHandle());
        return;
    }

#ifdef DTK_TITLE_DRAG_WINDOW
    Q_EMIT mousePosPressed(event->buttons(), event->globalPos());
#endif
    Q_EMIT mousePressed(event->buttons());
}

void DTitlebar::mouseReleaseEvent(QMouseEvent *event)
{
    D_D(DTitlebar);
    if (event->button() == Qt::LeftButton) {
        d->mousePressed = false;
    }
}

bool DTitlebar::eventFilter(QObject *obj, QEvent *event)
{
    D_D(DTitlebar);

    if (event->type() == QEvent::MouseButtonPress &&
            static_cast<QMouseEvent *>(event)->button() == Qt::RightButton &&
            (obj ==d->minButton || obj == d->maxButton ||
            obj == d->closeButton || obj == d->optionButton ||
            obj == d->quitFullButton))
    {
        event->accept(); // button on titlebar should not show kwin menu
        return true;
    }

    if (obj == d->targetWindow()) {
        switch (event->type()) {
        case QEvent::ShowToParent:
            d->handleParentWindowIdChange();
            d->handleParentWindowStateChange();
            break;
        case QEvent::Resize:
            if (d->autoHideOnFullscreen) {
                setFixedWidth(d->targetWindow()->width());
            }
            break;
        case QEvent::HoverMove: {
            auto mouseEvent = reinterpret_cast<QMouseEvent *>(event);
            bool isFullscreen = d->targetWindow()->windowState().testFlag(Qt::WindowFullScreen);
            if (isFullscreen && d->autoHideOnFullscreen) {
                if (mouseEvent->pos().y() > height() && d->isVisableOnFullscreen()) {
                    d->hideOnFullscreen();
                }
                if (mouseEvent->pos().y() < 2) {
                    d->showOnFullscreen();
                }
            }
            break;
        }
        case QEvent::WindowStateChange: {
            d->handleParentWindowStateChange();
            break;
        }
        default:
            break;
        }
    } else if (obj == d->maxButton) {
        switch (event->type()) {
        case QEvent::ToolTip: {
            d->showSplitScreenWidget();
            break;
        }
        case QEvent::Leave: {
            d->hideSplitScreenWidget();
            break;
        }
        case QEvent::MouseButtonPress: {
            d->maxButtonPressAndHoldTimer->start(300);
            break;
        }
        case QEvent::MouseButtonRelease: {
            d->maxButtonPressAndHoldTimer->stop();
            break;
        }
        default:
            break;

        }
    }
    if (d->titlebarSettings && d->titlebarSettingsImpl->hasEditPanel() && obj == d->titlebarSettingsImpl->toolsEditPanel()) {
        if (event->type() == QEvent::Show) {
            d->setFixedButtonsEnabled(false);
        } else if ((event->type() == QEvent::Close)) {
            d->setFixedButtonsEnabled(true);
        }
    }
    return QWidget::eventFilter(obj, event);
}

bool DTitlebar::event(QEvent *e)
{
    if (e->type() == QEvent::LayoutRequest) {
        D_D(DTitlebar);

        d->updateCenterArea();
    }

    if (e->type() == QEvent::FocusIn) {
        QFocusEvent *fe = static_cast<QFocusEvent*>(e);
        if (fe->reason() == Qt::TabFocusReason || fe->reason() == Qt::BacktabFocusReason) {
            e->accept();
            // 还需要 Tab 切换焦点时不不会出现再自己身上减少一次按 Tab 键 fix bug-65703
            fe->reason() == Qt::TabFocusReason ? focusNextChild() : focusPreviousChild();
        }
    } else if (e->type() == QEvent::StyleChange) {
        D_D(DTitlebar);
        d->updateTitlebarHeight();
        d->updateTitleBarSize();
    }

    return QFrame::event(e);
}

void DTitlebar::resizeEvent(QResizeEvent *event)
{
    //override QWidget::resizeEvent to fix button and separator pos.
    D_D(DTitlebar);

    d->separatorTop->setFixedWidth(event->size().width());
    d->separator->setFixedWidth(event->size().width());
    int x = (d->sidebarHelper && d->sidebarHelper->expanded()) ? d->sidebarHelper->width() : 0;
    d->separator->move(x, height() - d->separator->height());
    d->updateCenterArea();

    if (d->blurWidget) {
        d->blurWidget->resize(event->size());
    }

    if (d->sidebarBackgroundWidget)
        d->sidebarBackgroundWidget->setFixedHeight(event->size().height());

    if (d->titlebarSettingsImpl && d->titlebarSettingsImpl->hasEditPanel() && d->titlebarSettingsImpl->toolsEditPanel()->isVisible()) {
        if (d->titlebarSettingsImpl->toolsEditPanel()->minimumWidth() >= this->width()) {
            d->titlebarSettingsImpl->toolsEditPanel()->setWindowFlag(Qt::Dialog);
            d->titlebarSettingsImpl->toolsEditPanel()->show();
            int x = this->mapToGlobal(this->pos()).x() - (d->titlebarSettingsImpl->toolsEditPanel()->width()- this->width()) / 2 ;
            d->titlebarSettingsImpl->toolsEditPanel()->move(x, this->mapToGlobal(this->pos()).y() + this->height());
        } else {
            d->titlebarSettingsImpl->toolsEditPanel()->setWindowFlag(Qt::Dialog, false);
            d->titlebarSettingsImpl->toolsEditPanel()->show();
            d->titlebarSettingsImpl->toolsEditPanel()->move(0, this->height());
            d->titlebarSettingsImpl->toolsEditPanel()->resize(width(), parentWidget()->height() * 70 / 100);
        }
    }

    return QWidget::resizeEvent(event);
}

/*!
  @~english
  \brief DTitlebar::setCustomWidget is an overloaded function.
  \param[in] w is the widget to be used as the customize widget shown in the title
  bar.
  \param[in] fixCenterPos indicates whether it should automatically move the
  customize widget to the horizontal center of the title bar or not.
 */
void DTitlebar::setCustomWidget(QWidget *w, bool fixCenterPos)
{
    D_D(DTitlebar);

    if (w == d->customWidget) {
        return;
    }

    if (d->customWidget) {
        d->mainLayout->removeWidget(d->customWidget);
        d->customWidget->hide();
        d->customWidget->deleteLater();
    }

    d->customWidget = w;

    if (w) {
        w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    } else {
        d->centerArea->show();
        d->titleLabel = d->centerArea;

        return;
    }

    if (fixCenterPos) {
        for (int i = 0; i < d->centerLayout->count(); ++i) {
            delete d->centerLayout->itemAt(i);
        }

        addWidget(w, Qt::Alignment());
        d->centerArea->show();
        d->titleLabel = d->centerArea;
    } else {
        d->mainLayout->insertWidget(1, w);
        d->titleLabel = nullptr;
        d->centerArea->hide();
    }
}

void DTitlebar::setSidebarHelper(DSidebarHelper *helper)
{
    D_D(DTitlebar);
    if (d->sidebarHelper == helper)
        return;

    d->sidebarHelper = helper;

    if (!d->expandButton) {
        d->expandButton = new DIconButton(this);
        d->expandButton->setIcon(DDciIcon::fromTheme("window_sidebar"));
        d->expandButton->setIconSize(QSize(DefaultExpandButtonHeight(), DefaultExpandButtonHeight()));
        d->expandButton->setFlat(true);

        d->sidebarBackgroundWidget = new QWidget(this);
        QHBoxLayout *hlay = new QHBoxLayout(d->sidebarBackgroundWidget);
        hlay->setContentsMargins(QMargins(0, 0, 0, 0));
        auto bgBlurWidget = new DBlurEffectWidget(d->sidebarBackgroundWidget);
        bgBlurWidget->setObjectName("titlebarBlurWidget");
        bgBlurWidget->setBlendMode(DBlurEffectWidget::BehindWindowBlend);
        bgBlurWidget->setMaskColor(DBlurEffectWidget::AutoColor);
        bgBlurWidget->setMaskAlpha(229); // 90%
        hlay->addWidget(bgBlurWidget);

        d->sidebarBackgroundWidget->setAccessibleName("SidebarBackgroundWidget");
        d->sidebarBackgroundWidget->setAutoFillBackground(true);
        d->sidebarBackgroundWidget->setBackgroundRole(DPalette::Button);
        d->sidebarBackgroundWidget->move(pos());
        d->sidebarBackgroundWidget->lower();
        d->leftLayout->addWidget(d->expandButton, 0, Qt::AlignLeft);
        connect(d->expandButton, &DIconButton::clicked, [this, d] (bool) {
            bool isExpanded = d->sidebarHelper->expanded();
            d->sidebarHelper->setExpanded(!isExpanded);
            int x = isExpanded ? d->sidebarHelper->width() : 0;
            d->separator->move(x, height() - d->separator->height());
        });
    }

    connect(helper, &DSidebarHelper::visibleChanged, this, [this](bool visible){
        d_func()->expandButton->setVisible(visible);
        d_func()->sidebarBackgroundWidget->setVisible(d_func()->sidebarHelper->sectionVisible());
    });
    connect(helper, &DSidebarHelper::expandChanged, this, [this](bool isExpanded){
        d_func()->sidebarBackgroundWidget->setVisible(isExpanded);
        d_func()->expandButton->setChecked(isExpanded);
    });
    connect(helper, &DSidebarHelper::widthChanged, this, [this](int width){
        d_func()->sidebarBackgroundWidget->setFixedWidth(width);
    });

}

void DTitlebar::addWidget(QWidget *w, Qt::Alignment alignment)
{
    D_D(DTitlebar);

    if (alignment & Qt::AlignLeft) {
        d->leftLayout->addWidget(w, 0, alignment & ~Qt::AlignLeft);
    } else if (alignment & Qt::AlignRight) {
        d->rightLayout->addWidget(w, 0, alignment & ~Qt::AlignRight);
    } else {
        d->centerLayout->addWidget(w, 0, alignment);
        d->centerArea->clear();
        d->titleLabel = nullptr;
    }

    updateGeometry();
    d->updateTabOrder();
}

void DTitlebar::removeWidget(QWidget *w)
{
    D_D(DTitlebar);

    d->leftLayout->removeWidget(w);
    d->centerLayout->removeWidget(w);
    d->rightLayout->removeWidget(w);

    if (d->centerLayout->isEmpty()) {
        d->titleLabel = d->centerArea;
        d->titleLabel->setText(d->targetWindowHandle->title());
        setProperty("_dtk_title", d->titleLabel->text());
    }

    updateGeometry();
    d->updateTabOrder();
}

/*!
  @~english
  \brief DTitlebar::setFixedHeight change the height of the title bar to
  another value.
  \param[in] h is the target height.
 */
void DTitlebar::setFixedHeight(int h)
{
    QWidget::setFixedHeight(h);
}

/*!
  @~english
  \brief DTitlebar::setBackgroundTransparent set the title background transparent
  \param[in] transparent is the targeting value.
 */
void DTitlebar::setBackgroundTransparent(bool transparent)
{
    setAutoFillBackground(!transparent);

    if (transparent)
        setBackgroundRole(QPalette::NoRole);
    else
        setBackgroundRole(QPalette::Base);
}

/*!
  @~english
  \brief DTitlebar::setSeparatorVisible sets the bottom separator of the title
  bar and the window contents to be visible or not.
  \param[in] visible is the targeting value.
 */
void DTitlebar::setSeparatorVisible(bool visible)
{
    D_D(DTitlebar);
    if (visible) {
        d->separator->show();
        d->separator->raise();
    } else {
        d->separator->hide();
    }
}

/*!
  @~english
  \brief DTitlebar::setTitle sets the title to be shown on the title bar.
  \param[in] title is the text to be used as the window title.
 */
void DTitlebar::setTitle(const QString &title)
{
    D_D(DTitlebar);
    if (d->titleLabel && !d->embedMode) {
        d->titleLabel->setText(title);
    } else if (parentWidget()) {
        parentWidget()->setWindowTitle(title);
    }
    setProperty("_dtk_title", title);
}

/*!
  @~english
  \brief DTitlebar::setIcon sets the icon to be shown on the title bar.
  \a icon is to be used as the window icon.
 */
void DTitlebar::setIcon(const QIcon &icon)
{
    D_D(DTitlebar);
    if (!d->embedMode) {
        d->iconLabel->setIcon(icon);
        d->setIconVisible(!icon.isNull());
    } else if (parentWidget()) {
        d->setIconVisible(false);
        parentWidget()->setWindowIcon(icon);
    }
}

void DTitlebar::toggleWindowState()
{
    D_D(DTitlebar);

    d->_q_toggleWindowState();
}

void DTitlebar::setBlurBackground(bool blurBackground)
{
    D_D(DTitlebar);

    if (static_cast<bool>(d->blurWidget) == blurBackground)
        return;

    if (d->blurWidget) {
        d->blurWidget->hide();
        d->blurWidget->deleteLater();
        d->blurWidget = nullptr;
    } else {
        d->blurWidget = new DBlurEffectWidget(this);
        d->blurWidget->lower();
        d->blurWidget->resize(size());
        d->blurWidget->setMaskColor(DBlurEffectWidget::AutoColor);
        d->blurWidget->setRadius(30);
        d->blurWidget->show();
    }

    setAutoFillBackground(!blurBackground);
}

/*!
  @~english
  \brief DTitlebar::buttonAreaWidth returns the width of the area that all the
  window buttons occupies.
 */
int DTitlebar::buttonAreaWidth() const
{
    D_DC(DTitlebar);
    return d->buttonArea->width();
}

/*!
  @~english
  \brief DTitlebar::separatorVisible returns the visibility of the bottom
  separator of the titlebar.
 */
bool DTitlebar::separatorVisible() const
{
    D_DC(DTitlebar);
    return d->separator->isVisible();
}

/*!
  @~english
  \brief DTitlebar::autoHideOnFullscreen returns if titlebar show on fullscreen mode.
  separator of the titlebar.
 */
bool DTitlebar::autoHideOnFullscreen() const
{
    D_DC(DTitlebar);
    return d->autoHideOnFullscreen;
}

/*!
  @~english
  \brief DTitlebar::setAutoHideOnFullscreen set if titlebar show when window is fullscreen state.
  \param[in] autohide Whether to hide automatically
 */
void DTitlebar::setAutoHideOnFullscreen(bool autohide)
{
    D_D(DTitlebar);
    d->autoHideOnFullscreen = autohide;
}

void DTitlebar::setVisible(bool visible)
{
    D_D(DTitlebar);

    if (visible == isVisible()) {
        return;
    }

    QWidget::setVisible(visible);

    if (visible) {
        if (!d->targetWindow()) {
            return;
        }
        d->targetWindow()->installEventFilter(this);

        connect(d->maxButton, SIGNAL(clicked()), this, SLOT(_q_toggleWindowState()), Qt::UniqueConnection);
        connect(this, SIGNAL(doubleClicked()), this, SLOT(_q_toggleWindowState()), Qt::UniqueConnection);
        connect(d->minButton, SIGNAL(clicked()), this, SLOT(_q_showMinimized()), Qt::UniqueConnection);
        connect(d->closeButton, SIGNAL(clicked()), this, SLOT(_q_closeWindow()), Qt::UniqueConnection);

        d->updateButtonsState(d->targetWindow()->windowFlags());
    } else {
        if (!d->targetWindow()) {
            return;
        }
        d->targetWindow()->removeEventFilter(this);
    }
}

/*!
  @~english
  \brief This function provides to set a titlebar is in parent.
  \param[in] while visible is true ，Replace the system title bar, otherwise the system title bar is hidden.
 */
void DTitlebar::setEmbedMode(bool visible)
{
    D_D(DTitlebar);
    d->embedMode = visible;
    d->separatorTop->setVisible(visible);
    d->updateButtonsState(windowFlags());
}

/*!
  @~english
  \brief Visualization of the menu button.
  \return true The menu is visible, false Menu is not visible.
 */
bool DTitlebar::menuIsVisible() const
{
    D_DC(DTitlebar);
    return !d->optionButton->isVisible();
}

/*!
  @~english
  \brief set the menu whether it is visible.
  \param[in] visible true The menu is visible, falseThe menu is not visible.
 */
void DTitlebar::setMenuVisible(bool visible)
{
    D_D(DTitlebar);
    d->optionButton->setVisible(visible);
}

/*!
  @~english
  \brief Whether the menu is disabled.
  \return true: Menu is disabled, false: The menu is not disabled
 */
bool DTitlebar::menuIsDisabled() const
{
    D_DC(DTitlebar);
    return !d->optionButton->isEnabled();
}

/*!
  @~english
  \brief set the menu whether it is disabled.
  \param[in] disabled true： Menu is disabled, false： The menu is not disabled
 */
void DTitlebar::setMenuDisabled(bool disabled)
{
    D_D(DTitlebar);
    d->optionButton->setDisabled(disabled);
}

/*!
  @~english
  \brief Whether the withdrawal menu is disabled.
  \return true Exit menu is disabled false The exit menu is not disabled
 */
bool DTitlebar::quitMenuIsDisabled() const
{
    D_DC(DTitlebar);

    return d->quitAction && !d->quitAction->isEnabled();
}

/*!
  @~english
  \brief Set the exit menu whether it is disabled.
  \param[in] disabled true Exit menu is disabled, false The exit menu is not disabled
 */
void DTitlebar::setQuitMenuDisabled(bool disabled)
{
    D_D(DTitlebar);

    if (!d->quitAction) {
        d->_q_addDefaultMenuItems();
    }

    d->quitAction->setEnabled(!disabled);
}

/*!
  @~english
  \brief Set the exit menu whether it is visible.
  \a visible true exit the menu visible, false exit the menu is not visible
 */
void DTitlebar::setQuitMenuVisible(bool visible)
{
    D_D(DTitlebar);

    if (!d->quitAction) {
        d->_q_addDefaultMenuItems();
    }

    d->quitAction->setVisible(visible);
}

/*!
  @~english
  \brief Set the visualization of the theme switch menu.
  \return true Switch theme menu can be seen, false switch theme menu is not visible
 */
bool DTitlebar::switchThemeMenuIsVisible() const
{
    D_DC(DTitlebar);

    return d->switchThemeMenu;
}

/*!
  @~english
  \brief Set the theme menu whether it is visible.
  \param[in] visible true Switch theme menu can be seen, false Switch theme menu is not visible
 */
void DTitlebar::setSwitchThemeMenuVisible(bool visible)
{
    D_D(DTitlebar);

    if (visible == d->canSwitchTheme) {
        return;
    }

    d->canSwitchTheme = visible;

    if (d->switchThemeMenu) {
        d->switchThemeMenu->menuAction()->setVisible(visible);
        d->themeSeparator->setVisible(visible);
    }
}

/*!
  @~english
  \brief This function provides to disable the button match flags.
  \param[in] flags the banned buttons that need to be disabled
 */
void DTitlebar::setDisableFlags(Qt::WindowFlags flags)
{
    D_D(DTitlebar);
    d->disableFlags = flags;
    d->updateButtonsFunc();
}

/*!
  @~english
  \brief Return which button is disabled.
  \return The disabled window logo
 */
Qt::WindowFlags DTitlebar::disableFlags() const
{
    D_DC(DTitlebar);
    return d->disableFlags;
}

void DTitlebar::setSplitScreenEnabled(bool enabled)
{
    D_D(DTitlebar);
    d->splitScreenWidgetEnable = enabled;
}

bool DTitlebar::splitScreenIsEnabled() const
{
    D_DC(DTitlebar);
    return d->splitScreenWidgetEnable;
}

QSize DTitlebar::sizeHint() const
{
    D_DC(DTitlebar);

    if (d->centerArea->isHidden()) {
        return QFrame::sizeHint();
    }

    int padding = qMax(d->leftArea->sizeHint().width(), d->rightArea->sizeHint().width());
    int width = d->centerArea->sizeHint().width() + 2 * d->mainLayout->spacing() + 2 * padding;

    return QSize(width, d->titlebarHeight);
}

QSize DTitlebar::minimumSizeHint() const
{
    return sizeHint();
}

bool DTitlebar::blurBackground() const
{
    D_DC(DTitlebar);
    return d->blurWidget;
}

void DTitlebar::setFullScreenButtonVisible(bool visible)
{
    D_D(DTitlebar);
    d->fullScreenButtonVisible = visible;
}

DTitlebarSettings *DTitlebar::settings()
{
    D_D(DTitlebar);

    if (!d->titlebarSettings) {
        auto settings = new DTitlebarSettings(this);
        d->titlebarSettingsImpl = settings->impl();
        d->titlebarSettings = settings;
    }
    return d->titlebarSettings;
}

void DTitlebar::mouseMoveEvent(QMouseEvent *event)
{
    D_D(DTitlebar);

    Qt::MouseButton button = event->buttons() & Qt::LeftButton ? Qt::LeftButton : Qt::NoButton;
    if (event->buttons() == Qt::LeftButton /*&& d->mousePressed*/) {
        if (!d->mousePressed) {
            return;
        }
        Q_EMIT mouseMoving(button);
    }

#ifdef DTK_TITLE_DRAG_WINDOW
    D_D(DTitlebar);
    if (d->mousePressed) {
        Q_EMIT mousePosMoving(button, event->globalPos());
    }
#endif
    QWidget::mouseMoveEvent(event);
}

void DTitlebar::mouseDoubleClickEvent(QMouseEvent *event)
{
    D_D(DTitlebar);

    if (event->buttons() == Qt::LeftButton) {
        d->mousePressed = false;
        Q_EMIT doubleClicked();
    }
}

DWIDGET_END_NAMESPACE

#include "moc_dtitlebar.cpp"
