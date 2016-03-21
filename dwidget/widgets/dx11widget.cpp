#include "dx11widget.h"

#include <QDebug>
#include <QTimer>
#include <QWidget>
#include <QMouseEvent>
#include <QLayout>

#include <DObjectPrivate>
#include <DGraphicsDropShadowEffect>
#include <DTitlebar>

#include <QX11Info>

#if QT_VERSION >= 0x050300
#include <QtPlatformHeaders/QXcbWindowFunctions>
#endif

#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/extensions/shape.h>

namespace XUtils
{
// From the WM spec
#define _NET_WM_MOVERESIZE_INVALID          -1
#define _NET_WM_MOVERESIZE_SIZE_TOPLEFT      0
#define _NET_WM_MOVERESIZE_SIZE_TOP          1
#define _NET_WM_MOVERESIZE_SIZE_TOPRIGHT     2
#define _NET_WM_MOVERESIZE_SIZE_RIGHT        3
#define _NET_WM_MOVERESIZE_SIZE_BOTTOMRIGHT  4
#define _NET_WM_MOVERESIZE_SIZE_BOTTOM       5
#define _NET_WM_MOVERESIZE_SIZE_BOTTOMLEFT   6
#define _NET_WM_MOVERESIZE_SIZE_LEFT         7
#define _NET_WM_MOVERESIZE_MOVE              8   /* movement only */
#define _NET_WM_MOVERESIZE_SIZE_KEYBOARD     9   /* size via keyboard */
#define _NET_WM_MOVERESIZE_MOVE_KEYBOARD    10   /* move via keyboard */
#define _NET_WM_MOVERESIZE_CANCEL           11   /* cancel operation */

#define _NET_WM_STATE_REMOVE        0    /* remove/unset property */
#define _NET_WM_STATE_ADD           1    /* add/set property */
#define _NET_WM_STATE_TOGGLE        2    /* toggle property  */

const char kAtomNameHidden[] = "_NET_WM_STATE_HIDDEN";
const char kAtomNameFullscreen[] = "_NET_WM_STATE_FULLSCREEN";
const char kAtomNameMaximizedHorz[] = "_NET_WM_STATE_MAXIMIZED_HORZ";
const char kAtomNameMaximizedVert[] = "_NET_WM_STATE_MAXIMIZED_VERT";
const char kAtomNameMoveResize[] = "_NET_WM_MOVERESIZE";
const char kAtomNameWmState[] = "_NET_WM_STATE";
const char kAtomNameWmStateAbove[] = "_NET_WM_STATE_ABOVE";
const char kAtomNameWmStateStaysOnTop[] = "_NET_WM_STATE_STAYS_ON_TOP";
const char kAtomNameWmSkipTaskbar[] = "_NET_WM_STATE_SKIP_TASKBAR";
const char kAtomNameWmSkipPager[] = "_NET_WM_STATE_SKIP_PAGER";

enum class CornerEdge
{
    kInvalid = 0,
    kTop = 1,
    kRight = 2,
    kBottom = 4,
    kLeft = 8,
    kTopLeft = 1 | 8,
    kTopRight = 1 | 2,
    kBottomLeft = 4 | 8,
    kBottomRight = 4 | 2,
};

enum class XCursorType
{
    kInvalid = -1,
    kArrow = 2,
    kTop = 138,
    kTopRight = 136,
    kRight = 96,
    kBottomRight = 14,
    kBottom = 16,
    kBottomLeft = 12,
    kLeft = 70,
    kTopLeft = 134,
};

static int CornerEdge2WmGravity(const CornerEdge &ce)
{
    switch (ce) {
    case CornerEdge::kTopLeft:     return _NET_WM_MOVERESIZE_SIZE_TOPLEFT;
    case CornerEdge::kTop:         return _NET_WM_MOVERESIZE_SIZE_TOP;
    case CornerEdge::kTopRight:    return _NET_WM_MOVERESIZE_SIZE_TOPRIGHT;
    case CornerEdge::kRight:       return _NET_WM_MOVERESIZE_SIZE_RIGHT;
    case CornerEdge::kBottomRight: return _NET_WM_MOVERESIZE_SIZE_BOTTOMRIGHT;
    case CornerEdge::kBottom:      return _NET_WM_MOVERESIZE_SIZE_BOTTOM;
    case CornerEdge::kBottomLeft:  return _NET_WM_MOVERESIZE_SIZE_BOTTOMLEFT;
    case CornerEdge::kLeft:        return _NET_WM_MOVERESIZE_SIZE_LEFT;
    default:                       return _NET_WM_MOVERESIZE_INVALID;
    }
}

static XCursorType CornerEdge2XCursor(const CornerEdge &ce)
{
    switch (ce) {
    case CornerEdge::kTop:         return XCursorType::kTop;
    case CornerEdge::kTopRight:    return XCursorType::kTopRight;
    case CornerEdge::kRight:       return XCursorType::kRight;
    case CornerEdge::kBottomRight: return XCursorType::kBottomRight;
    case CornerEdge::kBottom:      return XCursorType::kBottom;
    case CornerEdge::kBottomLeft:  return XCursorType::kBottomLeft;
    case CornerEdge::kLeft:        return XCursorType::kLeft;
    case CornerEdge::kTopLeft:     return XCursorType::kTopLeft;
    default:                       return XCursorType::kInvalid;
    }
}

void ChangeWindowMaximizedState(QWidget *widget, int wm_state)
{
    const auto display = QX11Info::display();
    const auto screen = QX11Info::appScreen();

    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    const Atom net_wm_state = XInternAtom(display, kAtomNameWmState, false);
    const Atom vertical_maximized = XInternAtom(display,
                                    kAtomNameMaximizedVert,
                                    false);
    const Atom horizontal_maximized = XInternAtom(display,
                                      kAtomNameMaximizedHorz,
                                      false);

    xev.xclient.type = ClientMessage;
    xev.xclient.message_type = net_wm_state;
    xev.xclient.display = display;
    xev.xclient.window = widget->winId();
    xev.xclient.format = 32;

    xev.xclient.data.l[0] = wm_state;
    xev.xclient.data.l[1] = vertical_maximized;
    xev.xclient.data.l[2] = horizontal_maximized;
    xev.xclient.data.l[3] = 1;

    XSendEvent(display,
               QX11Info::appRootWindow(screen),
               false,
               SubstructureRedirectMask | SubstructureNotifyMask,
               &xev);
    XFlush(display);
}

CornerEdge GetCornerEdge(QWidget *widget, int x, int y, int border_width)
{
    const int window_width = widget->width();
    const int window_height = widget->height();
    unsigned int ce = static_cast<unsigned int>(CornerEdge::kInvalid);

    if (y <= border_width) {
        ce = ce | static_cast<unsigned int>(CornerEdge::kTop);
    }
    if (x <= border_width) {
        ce = ce | static_cast<unsigned int>(CornerEdge::kLeft);
    }
    if (window_height - border_width <= y && y <= window_height) {
        ce = ce | static_cast<unsigned int>(CornerEdge::kBottom);
    }
    if (window_width - border_width <= x && x <= window_width) {
        ce = ce | static_cast<unsigned int>(CornerEdge::kRight);
    }
    return static_cast<CornerEdge>(ce);
}

void SendMoveResizeMessage(QWidget *widget, int action, int button)
{
    const auto display = QX11Info::display();
    const auto screen = QX11Info::appScreen();

    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    const Atom net_move_resize = XInternAtom(display, kAtomNameMoveResize, false);
    xev.xclient.type = ClientMessage;
    xev.xclient.message_type = net_move_resize;
    xev.xclient.display = display;
    xev.xclient.window = widget->winId();
    xev.xclient.format = 32;

    const auto global_position = QCursor::pos();
    xev.xclient.data.l[0] = global_position.x();
    xev.xclient.data.l[1] = global_position.y();
    xev.xclient.data.l[2] = action;
    xev.xclient.data.l[3] = button;
    xev.xclient.data.l[4] = 1;  // source indication
    XUngrabPointer(display, QX11Info::appTime());

    XSendEvent(display,
               QX11Info::appRootWindow(screen),
               false,
               SubstructureRedirectMask | SubstructureNotifyMask,
               &xev);
    XFlush(display);
}

bool IsCornerEdget(QWidget *widget, int x, int y, int border_width)
{
    return GetCornerEdge(widget, x, y, border_width) != CornerEdge::kInvalid;
}

void MoveWindow(QWidget *widget)
{
    SendMoveResizeMessage(widget, _NET_WM_MOVERESIZE_MOVE, Button1);
}

void MoveResizeWindow(QWidget *widget, int x, int y, int border_width)
{
    const CornerEdge ce = GetCornerEdge(widget, x, y, border_width);
    if (ce != CornerEdge::kInvalid) {
        const int action = CornerEdge2WmGravity(ce);
        SendMoveResizeMessage(widget, action, Button1);
    }
}

void ResetCursorShape(QWidget *widget)
{
    const auto display = QX11Info::display();
    const WId window_id = widget->winId();
    XUndefineCursor(display, window_id);
    XFlush(display);
}

bool SetCursorShape(QWidget *widget, int cursor_id)
{
    const auto display = QX11Info::display();
    const WId window_id = widget->winId();
    const Cursor cursor = XCreateFontCursor(display, cursor_id);
    if (!cursor) {
        qWarning() << "[ui]::SetCursorShape() call XCreateFontCursor() failed";
        return false;
    }
    const int result = XDefineCursor(display, window_id, cursor);
    XFlush(display);
    return result == Success;
}

void ShowFullscreenWindow(QWidget *widget, bool is_fullscreen)
{
    const auto display = QX11Info::display();
    const auto screen = QX11Info::appScreen();

    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    const Atom net_wm_state = XInternAtom(display, kAtomNameWmState, false);
    const Atom fullscreen = XInternAtom(display, kAtomNameFullscreen, false);
    xev.xclient.type = ClientMessage;
    xev.xclient.message_type = net_wm_state;
    xev.xclient.display = display;
    xev.xclient.window = widget->winId();
    xev.xclient.format = 32;

    if (is_fullscreen) {
        xev.xclient.data.l[0] = _NET_WM_STATE_ADD;
    } else {
        xev.xclient.data.l[0] = _NET_WM_STATE_REMOVE;
    }
    xev.xclient.data.l[1] = fullscreen;
    xev.xclient.data.l[2] = 0;
    xev.xclient.data.l[3] = 1;

    XSendEvent(display,
               QX11Info::appRootWindow(screen),
               false,
               SubstructureRedirectMask | SubstructureNotifyMask,
               &xev
              );
    XFlush(display);
}

void ShowMaximizedWindow(QWidget *widget)
{
    ChangeWindowMaximizedState(widget, _NET_WM_STATE_ADD);
}

void ShowMinimizedWindow(QWidget *widget, bool minimized)
{
    const auto display = QX11Info::display();
    const auto screen = QX11Info::appScreen();

    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    const Atom net_wm_state = XInternAtom(display, kAtomNameWmState, false);
    const Atom hidden = XInternAtom(display, kAtomNameHidden, false);
    xev.xclient.type = ClientMessage;
    xev.xclient.message_type = net_wm_state;
    xev.xclient.display = display;
    xev.xclient.window = widget->winId();
    xev.xclient.format = 32;

    if (minimized) {
        xev.xclient.data.l[0] = _NET_WM_STATE_ADD;
    } else {
        xev.xclient.data.l[0] = _NET_WM_STATE_REMOVE;
    }
    xev.xclient.data.l[1] = hidden;
    xev.xclient.data.l[2] = 0;
    xev.xclient.data.l[3] = 1;

    XSendEvent(display,
               QX11Info::appRootWindow(screen),
               false,
               SubstructureRedirectMask | SubstructureNotifyMask,
               &xev
              );

    XIconifyWindow(display, widget->winId(), screen);
    XFlush(display);
}

void ShowNormalWindow(QWidget *widget)
{
    ChangeWindowMaximizedState(widget, _NET_WM_STATE_REMOVE);
}

void ToggleMaximizedWindow(QWidget *widget)
{
    ChangeWindowMaximizedState(widget, _NET_WM_STATE_TOGGLE);
}

bool UpdateCursorShape(QWidget *widget, int x, int y, int border_width)
{
    const CornerEdge ce = GetCornerEdge(widget, x, y, border_width);
    const XCursorType x_cursor = CornerEdge2XCursor(ce);
    if (x_cursor != XCursorType::kInvalid) {
        return SetCursorShape(widget, static_cast<unsigned int>(x_cursor));
    } else {
        ResetCursorShape(widget);
        return false;
    }
}

void SkipTaskbarPager(QWidget *widget)
{
    Q_ASSERT(widget);

    // DEs like XFCE, will require the following codes to hide the window in the TaskBar
    QTimer::singleShot(0, [widget]() { // Wait till the window is ready
        // DEs like KDE, setting _NET_WM_WINDOW_TYPE to Utility will surfice.
#if QT_VERSION >= 0x050300
        QXcbWindowFunctions::setWmWindowType(widget->windowHandle(),
                                             QXcbWindowFunctions::Utility);
#endif
        const auto display = QX11Info::display();
        const auto screen = QX11Info::appScreen();

        const auto wmStateAtom = XInternAtom(display, kAtomNameWmState, false);
        const auto taskBarAtom = XInternAtom(display, kAtomNameWmSkipTaskbar, false);
        const auto noPagerAtom = XInternAtom(display, kAtomNameWmSkipPager, false);

        XEvent xev;
        memset(&xev, 0, sizeof(xev));

        xev.xclient.type = ClientMessage;
        xev.xclient.message_type = wmStateAtom;
        xev.xclient.display = display;
        xev.xclient.window = widget->winId();;
        xev.xclient.format = 32;

        xev.xclient.data.l[0] = _NET_WM_STATE_ADD;
        xev.xclient.data.l[1] = taskBarAtom;
        xev.xclient.data.l[2] = noPagerAtom;
        xev.xclient.data.l[3] = 1;

        XSendEvent(display,
                   QX11Info::appRootWindow(screen),
                   false,
                   SubstructureRedirectMask | SubstructureNotifyMask,
                   &xev);
        XFlush(display);
    });
}

void SetStayOnTop(QWidget *widget, bool on)
{
    Q_ASSERT(widget);

    const auto display = QX11Info::display();
    const auto screen = QX11Info::appScreen();

    const auto wmStateAtom = XInternAtom(display, kAtomNameWmState, false);
    const auto stateAboveAtom = XInternAtom(display, kAtomNameWmStateAbove, false);
    const auto stateStaysOnTopAtom = XInternAtom(display,
                                     kAtomNameWmStateStaysOnTop,
                                     false);

    XEvent xev;
    memset(&xev, 0, sizeof(xev));

    xev.xclient.type = ClientMessage;
    xev.xclient.message_type = wmStateAtom;
    xev.xclient.display = display;
    xev.xclient.window = widget->winId();
    xev.xclient.format = 32;

    xev.xclient.data.l[0] = on ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
    xev.xclient.data.l[1] = stateAboveAtom;
    xev.xclient.data.l[2] = stateStaysOnTopAtom;
    xev.xclient.data.l[3] = 1;

    XSendEvent(display,
               QX11Info::appRootWindow(screen),
               false,
               SubstructureRedirectMask | SubstructureNotifyMask,
               &xev);
    XFlush(display);
}

void SetMouseTransparent(QWidget *widget, bool on)
{
    Q_ASSERT(widget);

    const auto display = QX11Info::display();
    XRectangle XRect;
    XRect.x = 0;
    XRect.y = 0;
    int nRects = 0;

    if (on) {
        XRect.width = 0;
        XRect.height = 0;
        nRects = 0;
    } else {
        XRect.width = widget->width();
        XRect.height = widget->height();
        nRects = 1;
    }
    XShapeCombineRectangles(display, widget->winId(), ShapeInput,
                            0, 0,
                            &XRect, nRects, ShapeSet, YXBanded);
}

void SetWindowExtents(QWidget *widget, unsigned long WindowGlowRadius)
{
    Atom frameExtents;
    unsigned long value[4] = {
        WindowGlowRadius,
        WindowGlowRadius,
        WindowGlowRadius,
        WindowGlowRadius
    };
    frameExtents = XInternAtom(QX11Info::display(), "_GTK_FRAME_EXTENTS", False);
    if (frameExtents == None) {
        qWarning() << "Failed to create atom with name DEEPIN_WINDOW_SHADOW";
        return;
    }
    XChangeProperty(QX11Info::display(),
                    widget->winId(),
                    frameExtents,
                    XA_CARDINAL,
                    32,
                    PropModeReplace,
                    (unsigned char *)value,
                    4);
}


void PropagateSizeHints(QWidget *w)
{
    const auto display = QX11Info::display();
    XSizeHints *sh = XAllocSizeHints();
    sh->flags = PPosition | PSize | PMinSize | PMaxSize | PResizeInc;
    sh->x = w->x();
    sh->y = w->y();
    sh->min_width = w->minimumWidth();
    sh->min_height = w->minimumHeight();
    sh->base_width = w->baseSize().width();
    sh->base_height = w->baseSize().height();
    sh->max_width = w->maximumWidth();
    sh->max_height = w->maximumHeight();
    sh->width_inc =  w->sizeIncrement().width();
    sh->height_inc = w->sizeIncrement().height();
    XSetWMNormalHints(display, w->winId(), sh);
    XFree(sh);
}

}

DWIDGET_BEGIN_NAMESPACE

const int WindowGlowRadius = 8;
const int WindowsRadius = 4;

const QColor BorderColor = QColor(216, 216, 216);
const QColor ShadowColor = QColor(23, 23, 23, 128);
const QColor BackgroundTopColor = QColor(250, 251, 252);
const QColor BackgroundBottonColor = QColor(250, 251, 252);
const QColor TipsBorderColor = QColor(255, 255, 255, 255 * 0.2);
const QColor TipsBackground = QColor(0, 0, 0);

class DX11WidgetPrivate: public DObjectPrivate
{
    D_DECLARE_PUBLIC(DX11Widget)
public:
    explicit DX11WidgetPrivate(DX11Widget *q);

    void init();

    bool leftPressed;
    bool resizable;

    int                 m_ShadowWidth;
    int                 m_NormalShadowWidth;
    int                 m_Radius;
    bool                m_MousePressed;
    QPoint              m_LastMousePos;
    Qt::WindowFlags     dwindowFlags;

    QPixmap             m_Background;
    QVBoxLayout         *rootLayout;
    DTitlebar           *titlebar;
    QWidget             *contentWidget;
    DGraphicsGlowEffect *m_Shadow;
};

DX11WidgetPrivate::DX11WidgetPrivate(DX11Widget *q) : DObjectPrivate(q)
{
    leftPressed = false;
    resizable = true;
}

void DX11WidgetPrivate::init()
{
    D_Q(DX11Widget);

    dwindowFlags = Qt::Window | Qt::WindowTitleHint |
                   Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint |
                   Qt::WindowCloseButtonHint | Qt::WindowFullscreenButtonHint;
    m_NormalShadowWidth = WindowGlowRadius;
    m_ShadowWidth = WindowGlowRadius;
    m_Radius = WindowsRadius;
    m_MousePressed = false;
    m_Shadow = nullptr;

    rootLayout = new QVBoxLayout;
    rootLayout->setMargin(0);
    rootLayout->setSpacing(0);

    titlebar = new DTitlebar;
    contentWidget = new QWidget;
    rootLayout->addWidget(titlebar);
    rootLayout->addWidget(contentWidget);
    rootLayout->setAlignment(contentWidget, Qt::AlignCenter);

    q->connect(titlebar, &DTitlebar::closeClicked, q, &DX11Widget::close);
    q->connect(titlebar, &DTitlebar::maximumClicked, q, &DX11Widget::showMaximized);
    q->connect(titlebar, &DTitlebar::restoreClicked, q, &DX11Widget::showNormal);
    q->connect(titlebar, &DTitlebar::minimumClicked, q, &DX11Widget::showMinimized);

}

DX11Widget::DX11Widget(DX11Widget *parent): QWidget(parent), DObject(*new DX11WidgetPrivate(this))
{
    D_D(DX11Widget);

    d->init();
    QWidget::setLayout(d->rootLayout);

    QWidget::setAttribute(Qt::WA_TranslucentBackground, true);
    QWidget::setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    setWindowFlags(windowFlags());
    resize(size());

    setShadow();
    XUtils::SetWindowExtents(this, WindowGlowRadius);
}

DX11Widget::DX11Widget(DObjectPrivate &dd, DX11Widget *parent)
    : QWidget(parent), DObject(dd)
{

}

void DX11Widget::mouseMoveEvent(QMouseEvent *event)
{
    D_D(DX11Widget);
    if (true == d->leftPressed) {
        XUtils::MoveWindow(this);
    }
    return QWidget::mouseMoveEvent(event);
}

void DX11Widget::mousePressEvent(QMouseEvent *event)
{
    D_D(DX11Widget);
    d->leftPressed = true;
    if (event->button() == Qt::LeftButton && d->resizable) {
        XUtils::MoveResizeWindow(this, event->x(), event->y(), 10);
    }

    return QWidget::mousePressEvent(event);
}

void DX11Widget::mouseReleaseEvent(QMouseEvent *event)
{
    D_D(DX11Widget);
    d->leftPressed = false;
    return QWidget::mouseReleaseEvent(event);
}

void DX11Widget::showMinimized()
{
    XUtils::ShowMinimizedWindow(this, true);
}

void DX11Widget::showMaximized()
{
    D_D(DX11Widget);
    d->m_ShadowWidth = 0;
    XUtils::ShowMaximizedWindow(this);
    this->show();
    this->activateWindow();
    this->raise();
}

void DX11Widget::setContentsMargins(int left, int top, int right, int bottom)
{
    D_D(DX11Widget);
    d->contentWidget->setContentsMargins(left, top, right, bottom);
}

void DX11Widget::setContentsMargins(const QMargins &margins)
{
    D_D(DX11Widget);
    d->contentWidget->setContentsMargins(margins);
}

void DX11Widget::getContentsMargins(int *left, int *top, int *right, int *bottom) const
{
    D_DC(DX11Widget);
    d->contentWidget->getContentsMargins(left, top, right, bottom);
}

QMargins DX11Widget::contentsMargins() const
{
    D_DC(DX11Widget);
    return d->contentWidget->contentsMargins();
}

void DX11Widget::showFullScreen()
{
    D_D(DX11Widget);
    d->m_ShadowWidth = 0;

    XUtils::ShowFullscreenWindow(this, true);
    this->show();
    this->activateWindow();
    this->raise();
}

void DX11Widget::showNormal()
{
    D_D(DX11Widget);
    d->m_ShadowWidth = d->m_NormalShadowWidth;
    XUtils::ShowNormalWindow(this);
}


void DX11Widget::removeLayout()
{
    qDebug() << "removeLayout";
    qDeleteAll(this->children());
}

Qt::WindowFlags DX11Widget::windowFlags()
{
    D_D(DX11Widget);
    return d->dwindowFlags;
}

///
/// \brief setWindowFlags
/// \param type
///
void DX11Widget::setWindowFlags(Qt::WindowFlags type)
{
    D_D(DX11Widget);
    d->dwindowFlags = type;
    d->titlebar->setWindowFlags(type);
}

int DX11Widget::titleHeight() const
{
    D_DC(DX11Widget);
    return d->titlebar->height();
}

void DX11Widget::setTitleFixedHeight(int h)
{
    D_D(DX11Widget);
    d->titlebar->setFixedHeight(h);
}

void DX11Widget::setTitle(const QString &t)
{
    D_D(DX11Widget);
    d->titlebar->setTitle(t);
}

void DX11Widget::setIcon(const QPixmap &icon)
{
    D_D(DX11Widget);
    d->titlebar->setIcon(icon);
}

QLayout *DX11Widget::layout() const
{
    D_DC(DX11Widget);
    return d->contentWidget->layout();
}

void DX11Widget::setLayout(QLayout *l)
{
    D_D(DX11Widget);
    qDeleteAll(d->contentWidget->children());
    d->contentWidget->setLayout(l);
}

int DX11Widget::radius() const
{
    D_DC(DX11Widget);
    return d->m_Radius;
}

void DX11Widget::setRadius(int r)
{
    D_D(DX11Widget);
    d->m_Radius = r;
}

int DX11Widget::shadowWidth() const
{
    D_DC(DX11Widget);
    return d->m_Radius;
}

void DX11Widget::setShadowWidth(int r)
{
    D_D(DX11Widget);
    d->m_Radius = r;
}

const QPixmap &DX11Widget::backgroundImage() const
{
    D_DC(DX11Widget);
    return d->m_Background;
}

void DX11Widget::setBackgroundImage(const QPixmap &bk)
{
    D_D(DX11Widget);
    d->m_Background = bk;
}

void DX11Widget::setFixedSize(const QSize &size)
{
    D_D(DX11Widget);
    d->resizable = false;
//    XUtils::PropagateSizeHints(this);
    QWidget::resize(size.width() + d->m_ShadowWidth * 2, size.height() + d->m_ShadowWidth * 2 + titleHeight());
    d->contentWidget->setFixedSize(size);
//    QWidget::setMinimumSize(size);
//    QWidget::setMaximumSize(size);
    setWindowFlags(windowFlags() & ~ Qt::WindowMaximizeButtonHint);
}

void DX11Widget::setFixedSize(int w, int h)
{
    DX11Widget::setFixedSize(QSize(w, h));
}

void DX11Widget::resize(const QSize &size)
{
    D_D(DX11Widget);
    QWidget::resize(size.width() + d->m_ShadowWidth * 2, size.height() + d->m_ShadowWidth * 2 + titleHeight());
    d->contentWidget->resize(size);
}

void DX11Widget::showEvent(QShowEvent *e)
{
    D_D(DX11Widget);
    QWidget::showEvent(e);
}

void DX11Widget::resizeEvent(QResizeEvent *e)
{
    D_D(DX11Widget);
    if (!d->resizable) {
        return;
    }
    QWidget::resizeEvent(e);
}

void DX11Widget::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent(e);
}

void DX11Widget::setShadow()
{
    D_D(DX11Widget);

    d->m_Shadow = new DGraphicsGlowEffect(this);
    d->m_Shadow->setOffset(0, 0);
    d->m_Shadow->setBlurRadius(d->m_ShadowWidth);
    this->setGraphicsEffect(d->m_Shadow);
}

void DX11Widget::paintEvent(QPaintEvent */*e*/)
{
    D_D(DX11Widget);
    int glowRadius = d->m_ShadowWidth;
    int radius = d->m_Radius;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QRect rect = this->rect().marginsRemoved(QMargins(glowRadius, glowRadius, glowRadius, glowRadius));

    if (! d->m_Background.isNull()) {
        painter.drawPixmap(rect, d->m_Background);
    } else {
        QPoint topLeft(rect.x(), rect.y());
        QPoint bottomRight(rect.x() + rect.width(), rect.y() + rect.height());
        QPainterPath border;
        border.addRoundedRect(rect, radius, radius);

        QLinearGradient linearGradient(topLeft, QPoint(topLeft.x(), bottomRight.y()));
        linearGradient.setColorAt(0.0, BackgroundTopColor);
        linearGradient.setColorAt(0.2, BackgroundBottonColor);
        linearGradient.setColorAt(1.0, BackgroundBottonColor);

        QPen borderPen(BorderColor);
        painter.setBrush(QBrush(linearGradient));
        painter.strokePath(border, borderPen);
        painter.fillPath(border, QBrush(linearGradient));
    }
}


DWIDGET_END_NAMESPACE
