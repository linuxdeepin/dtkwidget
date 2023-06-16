// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dsplitscreen_p.h"
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QDesktopWidget>
#else
#include <QWindow>
#include <QScreen>
#endif
#include <QLoggingCategory>
#include <qpa/qplatformwindow.h>

#include "dpalettehelper.h"
#include "dstyleoption.h"
#include "dapplication.h"
#include "dstyle.h"
#include "dplatformwindowhandle.h"

DGUI_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

#define CHANGESPLITWINDOW_VAR "_d_splitWindowOnScreenByType"
#define GETSUPPORTSPLITWINDOW_VAR "_d_supportForSplittingWindowByType"

#ifdef QT_DEBUG
Q_LOGGING_CATEGORY(dSplitScreen, "dtk.splitscreen")
#else
Q_LOGGING_CATEGORY(dSplitScreen, "dtk.splitscreen", QtInfoMsg)
#endif

static const QSize SplitScreenCellSize(96, 64);
static const QSize TwoInOneSplitScreenPlaceholderSize(44, 58);
static const QSize OneSplitScreenPlaceholderSize(44, 28);
static const int SplitScreenPlaceholderRadius(4);
static const QSize TowSplitScreenSize(117, 85);
static const QSize FourSplitScreenSize(222, 158);

// Position的mask个数
static int numberMaskOfFlag(DSplitScreenPlaceholder::Position flag)
{
    int count = 0;
    const QVector<DSplitScreenPlaceholder::PositionFlag> flags {
        DSplitScreenPlaceholder::Left,
        DSplitScreenPlaceholder::Right,
        DSplitScreenPlaceholder::Top,
        DSplitScreenPlaceholder::Bottom
    };
    for (auto item : flags) {
        if (flag.testFlag(item))
            count++;
    }
    return count;
}
static quint32 getWinId(const QWidget *window)
{
    if (window) {
        const QWindow *windowHandle = window->windowHandle();
        if (windowHandle && windowHandle->handle())
            return windowHandle->handle()->winId();
    }
    return 0;
}

//### 目前接口尚不公开在 dtkgui 中，等待获取后续接口稳定再做移植
static bool supportForSplittingWindowByType(quint32 wid, const int screenSplittingType)
{
    bool supported = false;

    QFunctionPointer getSupportSplitWindow = qApp->platformFunction(GETSUPPORTSPLITWINDOW_VAR);
    if (!getSupportSplitWindow) {
        qCWarning(dSplitScreen) << "Can't get handler for `supportForSplittingWindowByType` of platform function, "
                                   "need to update `qt5platform-plugins` related package.";
    }

    if (getSupportSplitWindow)
        supported = reinterpret_cast<bool(*)(quint32, quint32)>(getSupportSplitWindow)(wid, static_cast<quint32>(screenSplittingType));

    if (!getSupportSplitWindow && !supported) {
        qCDebug(dSplitScreen) << "Can't support splitting Window Type:[" << screenSplittingType
                              << "] from `supportForSplittingWindowByType` of platform function.";
    }
    return supported;
}
static void splitWindowOnScreenByType(quint32 wid, quint32 position, quint32 type)
{
    QFunctionPointer splitWindowOnScreen = qApp->platformFunction(CHANGESPLITWINDOW_VAR);

    if (!splitWindowOnScreen) {
        qCWarning(dSplitScreen) << "Can't get handler for `splitWindowOnScreenByType` of platform function, "
                                   "need to update `qt5platform-plugins` related package.";
    }
    if (splitWindowOnScreen) {
        qCDebug(dSplitScreen) << "Call `splitWindowOnScreenByType` of platform function, "
                 << "arguments of position is [" << position << "] and type is [" << type << "].";
        reinterpret_cast<void(*)(quint32, quint32, quint32)>(splitWindowOnScreen)(wid, position, type);
    }
}

static void splitWindowOnScreenByType(const QWidget *widget, quint32 position, quint32 type)
{
    if (auto wid = getWinId(widget))
        splitWindowOnScreenByType(wid, position, type);
}

DSplitScreenPlaceholder::DSplitScreenPlaceholder(Position position, QWidget *parent)
    : DPushButton(parent)
    , m_position(position)
{
    // 若position只含有1个mask，则是二合一类型的cell，即为大的cell.
    const bool isTwoInOne = numberMaskOfFlag(position) <= 1;
    if (isTwoInOne) {
        setFixedSize(TwoInOneSplitScreenPlaceholderSize);
    } else {
        setFixedSize(OneSplitScreenPlaceholderSize);
    }
    DStyle::setFrameRadius(this, SplitScreenPlaceholderRadius);
    // remove influence of parent's FrameBorder
    auto dpal = DPaletteHelper::instance()->palette(this);
    DPaletteHelper::instance()->setPalette(this, dpal);
}

DSplitScreenPlaceholder::Position DSplitScreenPlaceholder::position() const
{
    return m_position;
}

void DSplitScreenPlaceholder::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    DStylePainter p(this);
    QStyleOptionButton option;
    initStyleOption(&option);
    if (m_paintFocus)
        option.state |= QStyle::State_HasFocus;
    p.drawControl(QStyle::CE_PushButton, option);
}

bool DSplitScreenPlaceholder::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::HoverEnter:
    case QEvent::HoverLeave: {
        m_paintFocus = event->type() == QEvent::HoverEnter;
    } break;
    default:
        break;
    }
    return DPushButton::event(event);
}

DSplitScreenCell::DSplitScreenCell(const DSplitScreenCell::Mode mode, QWidget *parent)
    : DFrame(parent)
{
    layout = new DFlowLayout(this);
    layout->setSpacing(2);
    layout->setContentsMargins(2, 2, 2, 2);

    setFixedSize(SplitScreenCellSize);
    setLineWidth(1);
    setMidLineWidth(1);
    auto pa = palette();
    pa.setBrush(backgroundRole(), Qt::transparent);
    setPalette(pa);

    for (auto item :positionsByScreenMode(mode)) {
        auto view  = new DSplitScreenPlaceholder(item);
        connect(view, &DSplitScreenPlaceholder::clicked, this, &DSplitScreenCell::onScreenPlaceholderSelected);
        layout->addWidget(view);
    }
}

void DSplitScreenCell::onScreenPlaceholderSelected()
{
    if (auto holder = qobject_cast<DSplitScreenPlaceholder *>(sender())) {
         screenSelected(m_type, holder->position());
    }
}

QVector<DSplitScreenPlaceholder::Position> DSplitScreenCell::positionsByScreenMode(const DSplitScreenCell::Mode mode)
{
    QVector<DSplitScreenPlaceholder::Position> positions;
    if (mode & TwoSplit) {
        layout->setFlow(QListView::LeftToRight);
        m_type = TwoSplit;
        positions.append(DSplitScreenPlaceholder::Left);
        positions.append(DSplitScreenPlaceholder::Right);
    } else if (mode & ThreeSplit) {
        layout->setFlow(QListView::TopToBottom);
        m_type = ThreeSplit;
        const bool left = mode & DSplitScreenCell::Left;
        if  (left) {
            positions.append(DSplitScreenPlaceholder::Left);
            positions.append(DSplitScreenPlaceholder::Right | DSplitScreenPlaceholder::Top);
            positions.append(DSplitScreenPlaceholder::Right | DSplitScreenPlaceholder::Bottom);
        } else {
            positions.append(DSplitScreenPlaceholder::Left | DSplitScreenPlaceholder::Top);
            positions.append(DSplitScreenPlaceholder::Left | DSplitScreenPlaceholder::Bottom);
            positions.append(DSplitScreenPlaceholder::Right);
        }
    } else if (mode & FourSplit) {
        layout->setFlow(QListView::LeftToRight);
        m_type = FourSplit;
        positions.append(DSplitScreenPlaceholder::Left | DSplitScreenPlaceholder::Top);
        positions.append(DSplitScreenPlaceholder::Right | DSplitScreenPlaceholder::Top);
        positions.append(DSplitScreenPlaceholder::Left | DSplitScreenPlaceholder::Bottom);
        positions.append(DSplitScreenPlaceholder::Right | DSplitScreenPlaceholder::Bottom);
    }
    return positions;
}

DSplitScreenWidget::DSplitScreenWidget(QWidget *parent)
    : DBlurEffectWidget(parent)
{
    this->init();
}

void DSplitScreenWidget::hide()
{
    if (!hideTimer.isActive())
        hideTimer.start(300, this);
}

void DSplitScreenWidget::hideImmediately()
{
    close();
}

bool DSplitScreenWidget::supportSplitScreenByWM(const QWidget *window)
{
    if (auto wid = getWinId(window))
        return supportForSplittingWindowByType(wid, DSplitScreenCell::SupportTwoSplit);

    return false;
}

void DSplitScreenWidget::show(const QPoint &pos)
{
    move(pos);
    QWidget::show();
}

void DSplitScreenWidget::onThemeTypeChanged(DGuiApplicationHelper::ColorType ct)
{
    if (ct == DGuiApplicationHelper::DarkType) {
        setMaskColor(this->palette().window().color());
    } else {
        setMaskColor(QColor(238, 238, 238, qRound(0.8 * 255)));
    }
    for (auto cell : findChildren<DSplitScreenCell *>()) {
        auto dpal = DPaletteHelper::instance()->palette(cell);
        if (ct == DGuiApplicationHelper::DarkType) {
            // TODO
            dpal.setColor(DPalette::FrameBorder, QColor(255 , 255, 255, static_cast<int>(0.1 * 255)));
        } else {
            dpal.setColor(DPalette::FrameBorder, QColor(0, 0, 0, static_cast<int>(0.1 * 255)));
        }
        DPaletteHelper::instance()->setPalette(cell, dpal);
    }

}

void DSplitScreenWidget::onScreenSelected(DSplitScreenCell::Mode type, DSplitScreenPlaceholder::Position position)
{
    splitWindowOnScreenByType(this->parentWidget(), static_cast<quint32>(position), static_cast<quint32>(type));
    isMaxButtonPressAndHold = false;
    hideImmediately();
}

void DSplitScreenWidget::init()
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlag(Qt::ToolTip);
    DPlatformWindowHandle handler(this);
    handler.setShadowRadius(20);

    this->setRadius(18);
    setBlendMode(DBlurEffectWidget::BehindWindowBlend);

    auto layout = new DFlowLayout(this);
    layout->setSpacing(10);
    layout->setContentsMargins(10, 10, 10, 10);
    QVector<DSplitScreenCell::Mode> screenModes;

    auto wid = getWinId(parentWidget());
    QSize contentViewSize;
    if (supportForSplittingWindowByType(wid, DSplitScreenCell::SupportTwoSplit)) {
        screenModes.append(DSplitScreenCell::TwoSplit);
        contentViewSize = TowSplitScreenSize;
    }
    if (supportForSplittingWindowByType(wid, DSplitScreenCell::SupportFourSplit)) {
        screenModes.append(DSplitScreenCell::ThreeSplit | DSplitScreenCell::Left);
        screenModes.append(DSplitScreenCell::ThreeSplit);
        screenModes.append(DSplitScreenCell::FourSplit);
        contentViewSize = FourSplitScreenSize;
    }
    setFixedSize(contentViewSize);
    for (auto mode : screenModes) {
        auto cell = new DSplitScreenCell(mode);
        connect(cell, &DSplitScreenCell::screenSelected, this, &DSplitScreenWidget::onScreenSelected);
        layout->insertWidget(layout->count(), cell);
    }

    onThemeTypeChanged(DGuiApplicationHelper::instance()->themeType());
    qApp->installEventFilter(this);

    QObject::connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
                     this, &DSplitScreenWidget::onThemeTypeChanged);
}

bool DSplitScreenWidget::disableByScreenGeometryAndWindowSize() const
{
    auto window = this->window();
    QRect screenRect;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QDesktopWidget *desktop = qApp->desktop();

    if (Q_LIKELY(window) && Q_LIKELY(desktop)) {
        screenRect = desktop->screenGeometry(window);
    }
#else
    if(auto screen = QGuiApplication::primaryScreen()){
        screenRect = screen->geometry();
    }
#endif
    if (Q_LIKELY(!screenRect.isNull()) && Q_LIKELY(window)) {
        // 窗口尺寸大于屏幕分辨率时 禁用控件
        if(screenRect.width() < window->minimumWidth() || screenRect.height() < window->minimumHeight())
            return true;
    }
    return false;
}

bool DSplitScreenWidget::eventFilter(QObject *o, QEvent *e)
{
    switch (e->type()) {
    case QEvent::Enter:
        if (o == this)
            hideTimer.stop();
        break;

    case QEvent::Leave:
        if (o == this)
            hide();
        break;

    case QEvent::Close:
        if (!o->objectName().compare(QLatin1String("qtooltip_label")))
            break;

        Q_FALLTHROUGH();
    case QEvent::WindowActivate:
    case QEvent::WindowDeactivate:
    case QEvent::FocusIn:
    case QEvent::FocusOut:
    case QEvent::MouseButtonDblClick:
    case QEvent::Wheel:
        hideImmediately();
        break;
    case QEvent::MouseButtonRelease:
        if (!isMaxButtonPressAndHold) {
            hideImmediately();
        }
        break;
    default:
        break;
    }

    return false;
}

void DSplitScreenWidget::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == hideTimer.timerId()) {
        hideTimer.stop();
        hideImmediately();
    }
}

DWIDGET_END_NAMESPACE
