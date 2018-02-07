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

#include "dtitlebar.h"

#include <QDebug>
#include <QMenu>
#include <QHBoxLayout>
#include <QApplication>
#include <QMouseEvent>
#include <QProcess>

#include <DObjectPrivate>

#include "dwindowclosebutton.h"
#include "dwindowmaxbutton.h"
#include "dwindowminbutton.h"
#include "dwindowoptionbutton.h"
#include "dlabel.h"
#include "dplatformwindowhandle.h"
#ifdef Q_OS_LINUX
#include "../platforms/x11/xutil.h"
#endif
#include "daboutdialog.h"
#include "dapplication.h"
#include "private/dapplication_p.h"
#include "dthememanager.h"
#include "util/dwindowmanagerhelper.h"
#include "dmainwindow.h"

DWIDGET_BEGIN_NAMESPACE

const int DefaultTitlebarHeight = 40;
const int DefaultIconHeight = 24;
const int DefaultIconWidth = 24;

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
    void _q_toggleWindowState();
    void _q_showMinimized();
    void _q_onTopWindowMotifHintsChanged(quint32 winId);

#ifndef QT_NO_MENU
    void _q_addDefaultMenuItems();
    void _q_helpActionTriggered();
    void _q_aboutActionTriggered();
    void _q_quitActionTriggered();
#endif

    QHBoxLayout         *mainLayout;
    DLabel              *iconLabel;
    DLabel              *titleLabel;
    DWindowMinButton    *minButton;
    DWindowMaxButton    *maxButton;
    DWindowCloseButton  *closeButton;
    DWindowOptionButton *optionButton;
    DImageButton        *quitFullButton;

    QWidget             *customWidget = Q_NULLPTR;
    QWidget             *coustomAtea;
    QWidget             *buttonArea;
    QWidget             *titleArea;
    QWidget             *titlePadding;
    QLabel              *separatorTop;
    QLabel              *separator;

#ifndef QT_NO_MENU
    QMenu               *menu = Q_NULLPTR;
    QAction             *helpAction = Q_NULLPTR;
    QAction             *aboutAction = Q_NULLPTR;
    QAction             *quitAction = Q_NULLPTR;
#endif

    Qt::WindowFlags     disableFlags;
    bool                mousePressed    = false;
    bool                embedMode       = false;
    bool                autoHideOnFullscreen = false;

    Q_DECLARE_PUBLIC(DTitlebar)
};

DTitlebarPrivate::DTitlebarPrivate(DTitlebar *qq): DObjectPrivate(qq)
{
}

void DTitlebarPrivate::init()
{
    D_Q(DTitlebar);

    mainLayout      = new QHBoxLayout;
    iconLabel       = new DLabel;
    titleLabel      = new DLabel;
    minButton       = new DWindowMinButton;
    maxButton       = new DWindowMaxButton;
    closeButton     = new DWindowCloseButton;
    optionButton    = new DWindowOptionButton;
    quitFullButton  = new DImageButton;
    coustomAtea     = new QWidget;
    buttonArea      = new QWidget;
    titleArea       = new QWidget;
    titlePadding    = new QWidget;
    separatorTop    = new QLabel(q);
    separator       = new QLabel(q);

    optionButton->setObjectName("DTitlebarDWindowOptionButton");
    minButton->setObjectName("DTitlebarDWindowMinButton");
    maxButton->setObjectName("DTitlebarDWindowMaxButton");
    closeButton->setObjectName("DTitlebarDWindowCloseButton");
    quitFullButton->setObjectName("DTitlebarDWindowQuitFullscreenButton");
    quitFullButton->hide();

    mainLayout->setContentsMargins(6, 0, 0, 0);
    mainLayout->setSpacing(0);

    iconLabel->setFixedSize(DefaultIconWidth, DefaultIconHeight);
    iconLabel->setWindowFlags(Qt::WindowTransparentForInput);
    titleLabel->setText(qApp->applicationName());
    // TODO: use QSS
    titleLabel->setStyleSheet("font-size: 12px;");
    titleLabel->setContentsMargins(0, 0, DefaultIconWidth + 10, 0);
    titleLabel->setWindowFlags(Qt::WindowTransparentForInput);
//    q->setStyleSheet("background-color: green;");

    separatorTop->setFixedHeight(1);
    separatorTop->setStyleSheet("background: rgba(0, 0, 0, 20);");
    separatorTop->hide();
    separatorTop->setWindowFlags(Qt::WindowTransparentForInput);

    separator->setFixedHeight(1);
    separator->setStyleSheet("background: rgba(0, 0, 0, 20);");
    separator->hide();
    separator->setWindowFlags(Qt::WindowTransparentForInput);

    QHBoxLayout *buttonAreaLayout = new QHBoxLayout;
    buttonAreaLayout->setContentsMargins(0, 1, 0, 0);
    buttonAreaLayout->setMargin(0);
    buttonAreaLayout->setSpacing(0);
    buttonAreaLayout->addWidget(optionButton);
    buttonAreaLayout->addWidget(minButton);
    buttonAreaLayout->addWidget(maxButton);
    buttonAreaLayout->addWidget(closeButton);
    buttonAreaLayout->addWidget(quitFullButton);
    buttonArea->setLayout(buttonAreaLayout);

    QHBoxLayout *titleAreaLayout = new QHBoxLayout;
    titleAreaLayout->setContentsMargins(4, 0, 10 + iconLabel->width(), 0);
    titleAreaLayout->setSpacing(0);
    titleAreaLayout->addWidget(iconLabel);
    titleAreaLayout->setAlignment(iconLabel, Qt::AlignLeft);
    titlePadding->setFixedSize(buttonArea->size());
    titlePadding->setWindowFlags(Qt::WindowTransparentForInput);
    titleAreaLayout->addWidget(titlePadding);
    titleAreaLayout->addStretch();
    titleAreaLayout->addWidget(titleLabel);
    titleAreaLayout->setAlignment(titleLabel, Qt::AlignCenter);

    titleAreaLayout->addStretch();
    titleArea->setLayout(titleAreaLayout);
    titleArea->setWindowFlags(Qt::WindowTransparentForInput);

    QHBoxLayout *coustomAteaLayout = new QHBoxLayout;
    coustomAteaLayout->setMargin(0);
    coustomAteaLayout->setSpacing(0);
    coustomAteaLayout->addWidget(titleArea);
    coustomAtea->setLayout(coustomAteaLayout);

    mainLayout->addWidget(coustomAtea);
    mainLayout->addWidget(buttonArea);
    mainLayout->setAlignment(buttonArea, Qt::AlignRight |  Qt::AlignVCenter);

    q->setLayout(mainLayout);
    q->setFixedHeight(DefaultTitlebarHeight);
    q->setMinimumHeight(DefaultTitlebarHeight);
    coustomAtea->setFixedHeight(q->height());
    buttonArea->setFixedHeight(q->height());

    q->connect(quitFullButton, &DImageButton::clicked, q, [ = ]() {
        bool isFullscreen = targetWindow()->windowState().testFlag(Qt::WindowFullScreen);
        if (isFullscreen) {
            targetWindow()->showNormal();
        } else {
            targetWindow()->showFullScreen();
        }
    });
    q->connect(optionButton, &DWindowOptionButton::clicked, q, &DTitlebar::optionClicked);
    q->connect(DWindowManagerHelper::instance(), SIGNAL(windowMotifWMHintsChanged(quint32)),
               q, SLOT(_q_onTopWindowMotifHintsChanged(quint32)));
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
    q->setProperty("_restore_height", q->height());
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
        quitFullButton->hide();
        mainWindow->setMenuWidget(q);
        showOnFullscreen();
    } else {
        // must set to empty
        quitFullButton->show();
        mainWindow->menuWidget()->setParent(nullptr);
        mainWindow->setMenuWidget(Q_NULLPTR);
        q->setParent(mainWindow);
        q->show();
        hideOnFullscreen();
    }
}

void DTitlebarPrivate::updateButtonsState(Qt::WindowFlags type)
{
    bool useDXcb = DPlatformWindowHandle::isEnabledDXcb(targetWindow());
    bool isFullscreen = targetWindow()->windowState().testFlag(Qt::WindowFullScreen);

    bool showTitle = (type.testFlag(Qt::WindowTitleHint) || !useDXcb) && !embedMode;
    if (titleLabel) {
        titleLabel->setVisible(showTitle);
    }
    if (iconLabel) {
        iconLabel->setVisible(showTitle);
    }

    // Never show in embed/fullscreen
    bool forceHide = embedMode || isFullscreen;

    bool showMin = (type.testFlag(Qt::WindowMinimizeButtonHint) || !useDXcb) && !forceHide;
    minButton->setVisible(showMin);

    bool showMax = (type.testFlag(Qt::WindowMaximizeButtonHint) || !useDXcb) && !forceHide;
    maxButton->setVisible(showMax);
    ;
    bool showClose = (type.testFlag(Qt::WindowCloseButtonHint) || !useDXcb) && !forceHide;
    closeButton->setVisible(showClose);

    bool showOption = (type.testFlag(Qt::WindowSystemMenuHint) || !useDXcb) && !isFullscreen;
    optionButton->setVisible(showOption);

    buttonArea->adjustSize();
    buttonArea->resize(buttonArea->size());

    if (titlePadding) {
        titlePadding->setFixedSize(buttonArea->size());
    }
}

void DTitlebarPrivate::updateButtonsFunc()
{
    optionButton->setDisabled(disableFlags.testFlag(Qt::WindowSystemMenuHint));
    minButton->setDisabled(disableFlags.testFlag(Qt::WindowMinimizeButtonHint));
    maxButton->setDisabled(disableFlags.testFlag(Qt::WindowMaximizeButtonHint));
    closeButton->setDisabled(disableFlags.testFlag(Qt::WindowCloseButtonHint));

    DWindowManagerHelper::setMotifFunctions(
        targetWindow()->windowHandle(),
        DWindowManagerHelper::FUNC_MAXIMIZE,
        !disableFlags.testFlag(Qt::WindowMaximizeButtonHint));
    DWindowManagerHelper::setMotifFunctions(
        targetWindow()->windowHandle(),
        DWindowManagerHelper::FUNC_MINIMIZE,
        !disableFlags.testFlag(Qt::WindowMinimizeButtonHint));
    DWindowManagerHelper::setMotifFunctions(
        targetWindow()->windowHandle(),
        DWindowManagerHelper::FUNC_CLOSE,
        !disableFlags.testFlag(Qt::WindowCloseButtonHint));
}

void DTitlebarPrivate::_q_toggleWindowState()
{
    QWidget *parentWindow = targetWindow();

    if (!parentWindow || disableFlags.testFlag(Qt::WindowMaximizeButtonHint)) {
        return;
    }

    if (parentWindow->isMaximized()) {
        parentWindow->showNormal();
    } else if (!parentWindow->isFullScreen()
               && (maxButton->isVisible())) {
        maxButton->setState(DImageButton::Normal);
        parentWindow->showMaximized();
    }
}

void DTitlebarPrivate::_q_showMinimized()
{
    minButton->setState(DImageButton::Normal);
    targetWindow()->showMinimized();
}

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

    DWindowManagerHelper::MotifFunctions functions_hints = DWindowManagerHelper::getMotifFunctions(q->window()->windowHandle());
    DWindowManagerHelper::MotifDecorations decorations_hints = DWindowManagerHelper::getMotifDecorations(q->window()->windowHandle());

    if (titleLabel) {
        titleLabel->setVisible(decorations_hints.testFlag(DWindowManagerHelper::DECOR_TITLE));
    }

    if (iconLabel) {
        iconLabel->setVisible(decorations_hints.testFlag(DWindowManagerHelper::DECOR_TITLE));
    }

//    updateButtonsState(targetWindow()->windowFlags());

//    minButton->setEnabled(functions_hints.testFlag(DWindowManagerHelper::FUNC_MINIMIZE) && !embedMode);
//    maxButton->setEnabled(functions_hints.testFlag(DWindowManagerHelper::FUNC_MAXIMIZE) && !embedMode);
//    closeButton->setEnabled(functions_hints.testFlag(DWindowManagerHelper::FUNC_CLOSE) && !embedMode);
//    // sync button state
//    if (minButton->isEnabled()) {
//        disableFlags &= ~Qt::WindowMinimizeButtonHint;
//    } else {
//        disableFlags |= Qt::WindowMinimizeButtonHint;
//    }
//    if (maxButton->isEnabled()) {
//        disableFlags &= ~Qt::WindowMaximizeButtonHint;
//    } else {
//        disableFlags |= Qt::WindowMaximizeButtonHint;
//    }
//    if (closeButton->isEnabled()) {
//        disableFlags &= ~Qt::WindowCloseButtonHint;
//    } else {
//        disableFlags |= Qt::WindowCloseButtonHint;
//    }

    if (titlePadding) {
        titlePadding->setFixedSize(buttonArea->size());
    }
}

#ifndef QT_NO_MENU

void DTitlebarPrivate::_q_addDefaultMenuItems()
{
    D_Q(DTitlebar);

    if (!menu) {
        q->setMenu(new QMenu(q));
    }

    // add help menu item.
    if (!helpAction && DApplicationPrivate::isUserManualExists()) {
        helpAction = new QAction(qApp->translate("TitleBarMenu", "Help"), menu);
        QObject::connect(helpAction, SIGNAL(triggered(bool)), q, SLOT(_q_helpActionTriggered()));
        menu->addAction(helpAction);
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
        menu->addAction(quitAction);
    }
}

void DTitlebarPrivate::_q_helpActionTriggered()
{
    DApplication *dapp = qobject_cast<DApplication *>(qApp);
    if (dapp) {
        dapp->handleHelpAction();
    }
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

#endif

/**
 * \class DTitlebar
 * \brief The DTitlebar class is an universal title bar on the top of windows.
 * \param parent is the parent widget to be attached on.
 *
 * Usually you don't need to construct a DTitlebar instance by your self, you
 * can get an DTitlebar instance by DMainWindow::titlebar .
 */

/**
 * @brief DTitlebar::DTitlebar create an default widget with icon/title/and bottons
 * @param parent
 */
DTitlebar::DTitlebar(QWidget *parent) :
    QFrame(parent),
    DObject(*new DTitlebarPrivate(this))
{
    if (DApplication::buildDtkVersion() < DTK_VERSION_CHECK(2, 0, 6, 1)) {
        setBackgroundTransparent(true);
    }

    DThemeManager::registerWidget(this, QStringList({"transparent"}));

    D_D(DTitlebar);
    d->init();
    d->buttonArea->adjustSize();
    d->buttonArea->resize(d->buttonArea->size());
    d->titlePadding->setFixedSize(d->buttonArea->size());
}

#ifndef QT_NO_MENU
/**
 * @brief DTitlebar::menu holds the QMenu object attached to this title bar.
 * @return the QMenu object it holds, returns null if there's no one set.
 */
QMenu *DTitlebar::menu() const
{
    D_DC(DTitlebar);

    return d->menu;
}

/**
 * @brief DTitlebar::setMenu attaches a QMenu object to the title bar.
 * @param menu is the target menu.
 */
void DTitlebar::setMenu(QMenu *menu)
{
    D_D(DTitlebar);

    d->menu = menu;
    if (d->menu) {
        disconnect(this, &DTitlebar::optionClicked, 0, 0);
        connect(this, &DTitlebar::optionClicked, this, &DTitlebar::showMenu);
    }
}

#endif

/**
 * @brief DTitlebar::customWidget
 * @return the customized widget used in this title bar.
 *
 * One can set customized widget to show some extra widgets on the title bar.
 * \see DTitlebar::setCustomWidget()
 */
QWidget *DTitlebar::customWidget() const
{
    D_DC(DTitlebar);

    return d->customWidget;
}

///
/// \brief setWindowFlags
/// \param type
/// accpet  WindowTitleHint, WindowSystemMenuHint, WindowMinimizeButtonHint, WindowMaximizeButtonHint
/// and WindowMinMaxButtonsHint.
void DTitlebar::setWindowFlags(Qt::WindowFlags type)
{
    D_D(DTitlebar);
    d->targetWindow()->setWindowFlags(type);
}

#ifndef QT_NO_MENU
void DTitlebar::showMenu()
{
    D_D(DTitlebar);

    if (d->menu) {
        d->optionButton->setState(DImageButton::Normal);
        d->menu->exec(d->optionButton->mapToGlobal(d->optionButton->rect().bottomLeft()));
    }
}
#endif

void DTitlebar::showEvent(QShowEvent *event)
{
    D_D(DTitlebar);
    d->separatorTop->setFixedWidth(width());
    d->separatorTop->move(0, 0);
    d->separator->setFixedWidth(width());
    d->separator->move(0, height() - d->separator->height());

#ifndef QT_NO_MENU
    d->_q_addDefaultMenuItems();
#endif

    QWidget::showEvent(event);

    if (DPlatformWindowHandle::isEnabledDXcb(window())) {
        d->_q_onTopWindowMotifHintsChanged(
            static_cast<quint32>(window()->internalWinId()));
    }
}

void DTitlebar::mousePressEvent(QMouseEvent *event)
{
    D_D(DTitlebar);
    d->mousePressed = (event->buttons() == Qt::LeftButton);

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
    if (event->buttons() == Qt::LeftButton) {
        d->mousePressed = false;
    }
}

bool DTitlebar::eventFilter(QObject *obj, QEvent *event)
{
    D_D(DTitlebar);

    if (obj == d->targetWindow()) {
        switch (event->type()) {
        case QEvent::WindowStateChange: {
            d->maxButton->setMaximized(d->targetWindow()->windowState() == Qt::WindowMaximized);
            d->updateFullscreen();
            d->updateButtonsState(d->targetWindow()->windowFlags());
            break;
        }
        case QEvent::ShowToParent:
            d->updateButtonsFunc();
            d->updateButtonsState(d->targetWindow()->windowFlags());
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
        default:
            break;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void DTitlebar::resizeEvent(QResizeEvent *event)
{
    D_D(DTitlebar);

    d->optionButton->setFixedHeight(event->size().height());
    d->minButton->setFixedHeight(event->size().height());
    d->maxButton->setFixedHeight(event->size().height());
    d->closeButton->setFixedHeight(event->size().height());
    d->separatorTop->setFixedWidth(event->size().width());
    d->separator->setFixedWidth(event->size().width());
    return QWidget::resizeEvent(event);
}

/**
 * @brief DTitlebar::setCustomWidget is an overloaded function.
 * @param w is the widget to be used as the customize widget shown in the title
 * bar.
 * @param fixCenterPos indicates whether it should automatically move the
 * customize widget to the horizontal center of the title bar or not.
 */
void DTitlebar::setCustomWidget(QWidget *w, bool fixCenterPos)
{
    setCustomWidget(w, Qt::AlignCenter, fixCenterPos);
}

/**
 * @brief DTitlebar::setCustomWidget sets a customized widget to be used as the
 * central content of the title bar.
 * @param w is the widget to be used as the customize widget shown in the title
 * bar.
 * @param wflag is used to vertically align the widget.
 * @param fixCenterPos indicates whether it should automatically move the
 * customize widget to the horizontal center of the title bar or not.
 */
void DTitlebar::setCustomWidget(QWidget *w, Qt::AlignmentFlag wflag, bool fixCenterPos)
{
    D_D(DTitlebar);
    if (!w || w == d->titleArea) {
        return;
    }

    QSize old = d->buttonArea->size();

    QHBoxLayout *l = new QHBoxLayout;
    l->setSpacing(0);
    l->setMargin(0);

    if (fixCenterPos) {
        d->titlePadding = new QWidget;
        d->titlePadding->setFixedSize(old);
        l->addWidget(d->titlePadding);
    }

    l->addWidget(w, 0, wflag);
    qDeleteAll(d->coustomAtea->children());
    d->titleLabel = Q_NULLPTR;
    d->titleArea = Q_NULLPTR;
    d->iconLabel = Q_NULLPTR;
    d->titlePadding = Q_NULLPTR;
    d->coustomAtea->setLayout(l);
    d->buttonArea->resize(old);
    d->customWidget = w;

    w->resize(d->coustomAtea->size());
}

/**
 * @brief DTitlebar::setFixedHeight change the height of the title bar to
 * another value.
 * @param h is the target height.
 */
void DTitlebar::setFixedHeight(int h)
{
    D_D(DTitlebar);
    QWidget::setFixedHeight(h);
    d->coustomAtea->setFixedHeight(h);
    d->buttonArea->setFixedHeight(h);
}

/**
 * @brief DTitlebar::setBackgroundTransparent set the title background transparent
 * @param transparent is the targeting value.
 */
void DTitlebar::setBackgroundTransparent(bool transparent)
{
    setProperty("transparent", transparent);
}

/**
 * @brief DTitlebar::setSeparatorVisible sets the bottom separator of the title
 * bar and the window contents to be visible or not.
 * @param visible is the targeting value.
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

/**
 * @brief DTitlebar::setTitle sets the title to be shown on the title bar.
 * @param title is the text to be used as the window title.
 */
void DTitlebar::setTitle(const QString &title)
{
    D_D(DTitlebar);
    if (d->titleLabel && !d->embedMode) {
        d->titleLabel->setText(title);
    } else if (parentWidget()) {
        parentWidget()->setWindowTitle(title);
    }
}

/*!
 * \brief Use setIcon(const QIcon &icon) instead
 */
void DTitlebar::setIcon(const QPixmap &icon)
{
    D_D(DTitlebar);
    if (d->titleLabel && !d->embedMode) {
        d->titleLabel->setContentsMargins(0, 0, 0, 0);
        d->iconLabel->setPixmap(icon.scaled(DefaultIconWidth * icon.devicePixelRatio(),
                                            DefaultIconHeight * icon.devicePixelRatio(), Qt::KeepAspectRatio));
    }
}

/*!
 * @brief DTitlebar::setIcon sets the icon to be shown on the title bar.
 * @param icon is to be used as the window icon.
 */
void DTitlebar::setIcon(const QIcon &icon)
{
    D_D(DTitlebar);
    if (d->titleLabel && !d->embedMode) {
        d->titleLabel->setContentsMargins(0, 0, 0, 0);
        d->iconLabel->setPixmap(icon.pixmap(QSize(DefaultIconWidth, DefaultIconHeight)));
    } else if (parentWidget()) {
        parentWidget()->setWindowIcon(icon);
    }
}

void DTitlebar::setWindowState(Qt::WindowState windowState)
{
    D_D(DTitlebar);
    d->maxButton->setWindowState(windowState);
}

void DTitlebar::toggleWindowState()
{
    D_D(DTitlebar);

    d->_q_toggleWindowState();
}

/**
 * @brief DTitlebar::buttonAreaWidth returns the width of the area that all the
 * window buttons occupies.
 */
int DTitlebar::buttonAreaWidth() const
{
    D_DC(DTitlebar);
    return d->buttonArea->width();
}

/**
 * @brief DTitlebar::separatorVisible returns the visibility of the bottom
 * separator of the titlebar.
 */
bool DTitlebar::separatorVisible() const
{
    D_DC(DTitlebar);
    return d->separator->isVisible();
}

bool DTitlebar::autoHideOnFullscreen() const
{
    D_DC(DTitlebar);
    return d->autoHideOnFullscreen;
}

void DTitlebar::setAutoHideOnFullscreen(bool autohide)
{
    D_D(DTitlebar);
    d->autoHideOnFullscreen = autohide;
}

/**
 * @brief DTitlebar::setVisible overrides QWidget::setVisible(bool visible)
 */
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

        connect(d->maxButton, SIGNAL(clicked()), this, SLOT(_q_toggleWindowState()));
        connect(this, SIGNAL(doubleClicked()), this, SLOT(_q_toggleWindowState()));
        connect(d->minButton, SIGNAL(clicked()), this, SLOT(_q_showMinimized()));
        connect(d->closeButton, &DWindowCloseButton::clicked, d->targetWindow(), &QWidget::close);
    } else {
        if (!d->targetWindow()) {
            return;
        }

        d->targetWindow()->removeEventFilter(this);

        disconnect(d->maxButton, SIGNAL(clicked()), this, SLOT(_q_toggleWindowState()));
        disconnect(this, SIGNAL(doubleClicked()), this, SLOT(_q_toggleWindowState()));
        disconnect(d->minButton, SIGNAL(clicked()), this, SLOT(_q_showMinimized()));
        disconnect(d->closeButton, &DWindowCloseButton::clicked, d->targetWindow(), &QWidget::close);
    }
}


/**
 * @brief DTitlebar::setEmbedMode set a titar is in parent;
 */
void DTitlebar::setEmbedMode(bool visible)
{
    D_D(DTitlebar);
    d->embedMode = visible;
    d->separatorTop->setVisible(visible);
    d->updateButtonsState(windowFlags());
}

/**
 * @brief DTitlebar::resize resizes the title bar.
 * @param w is the target width.
 * @param h is the target height.
 */
void DTitlebar::resize(int w, int h)
{
    D_DC(DTitlebar);
    if (d->customWidget) {
        d->customWidget->resize(w - d->buttonArea->width(), h);
    }
}

/**
 * @brief DTitlebar::resize is an overloaded function.
 * @param sz the target size.
 *
 * @see DTitlebar::resize(int w, int h)
 */
void DTitlebar::resize(const QSize &sz)
{
    DTitlebar::resize(sz.width(), sz.height());
}

void DTitlebar::setDisableFlags(Qt::WindowFlags flags)
{
    D_D(DTitlebar);
    d->disableFlags = flags;
    d->updateButtonsFunc();
}

Qt::WindowFlags DTitlebar::disableFlags() const
{
    D_DC(DTitlebar);
    return d->disableFlags;
}

void DTitlebar::mouseMoveEvent(QMouseEvent *event)
{
    Qt::MouseButton button = event->buttons() & Qt::LeftButton ? Qt::LeftButton : Qt::NoButton;
    if (event->buttons() == Qt::LeftButton /*&& d->mousePressed*/) {
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
