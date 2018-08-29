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

#include "dborderlesswidget.h"

#include <QDebug>
#include <QTimer>
#include <QWidget>
#include <QMouseEvent>
#include <QLayout>
#include <QTemporaryFile>
#include <QImage>
#include <QPainter>
#include <QGuiApplication>
#include <QWindow>

#include <DObjectPrivate>

#include "dtitlebar.h"
#include "private/dborderlesswidget_p.h"

#ifdef Q_OS_LINUX
#include "../platforms/x11/xutil.h"
#endif

#include "dwidgetutil.h"

DWIDGET_BEGIN_NAMESPACE

const int WindowGlowRadius = 40;
const int WindowRadius = 4;
const int WindowBorder = 1;
const int WindowHandleWidth = 10;

const QColor BorderColor = QColor(0, 0, 0, 60);
const QColor BackgroundTopColor = QColor(255, 255, 255);
const QColor BackgroundBottonColor = QColor(255, 255, 255);

const QColor TipsBorderColor = QColor(255, 255, 255, 255 * 0.2);
const QColor TipsBackground = QColor(0, 0, 0);

/// shadow
#define SHADOW_COLOR_NORMAL QColor(0, 0, 0, 255 * 0.15)
#define SHADOW_COLOR_ACTIVE QColor(0, 0, 0, 255 * 0.3)

DBorderlessWidgetPrivate::DBorderlessWidgetPrivate(DBorderlessWidget *q) : DObjectPrivate(q)
{
    leftPressed = false;
    resizable = true;
}

void DBorderlessWidgetPrivate::init()
{
    D_Q(DBorderlessWidget);

    dwindowFlags = Qt::Window | Qt::WindowTitleHint |
                   Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint |
                   Qt::WindowCloseButtonHint | Qt::WindowFullscreenButtonHint;
    m_NormalShadowWidth = WindowGlowRadius;
    m_ShadowWidth = WindowGlowRadius;
    shadowColor = SHADOW_COLOR_ACTIVE;
    shadowOffset = QPoint(0, 10);
    m_Radius = WindowRadius;
    m_Border = WindowBorder;
    m_ResizeHandleWidth = WindowHandleWidth;
    m_MousePressed = false;
    m_backgroundColor = BackgroundTopColor;

    rootLayout = new QVBoxLayout(q);

    rootLayout->setSpacing(0);
    updateContentsMargins();

    windowWidget = new QWidget;
    QVBoxLayout *windowLayout = new QVBoxLayout(windowWidget);
    windowLayout->setMargin(0);
    windowLayout->setSpacing(0);

    rootLayout->addWidget(windowWidget);

    titlebar = new DTitlebar;
    contentWidget = new QWidget;
    QVBoxLayout *contentWidgetLayout = new QVBoxLayout;
    contentWidgetLayout->setSpacing(0);
    contentWidgetLayout->setMargin(0);
    contentWidget->setLayout(contentWidgetLayout);
    contentWidget->setContentsMargins(m_Border, 0, m_Border, m_Border);
    contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    windowLayout->addWidget(titlebar);
    windowLayout->addWidget(contentWidget);

    auto filter = new FilterMouseMove(windowWidget);
    windowWidget->installEventFilter(filter);
    filter->m_rootWidget = q;

    q->connect(titlebar, &DTitlebar::optionClicked, q, &DBorderlessWidget::optionClicked);
    q->connect(titlebar, &DTitlebar::mouseMoving, q, &DBorderlessWidget::moveWindow);

    q->connect(titlebar, SIGNAL(mousePressed(Qt::MouseButtons)), q, SLOT(_q_onTitleBarMousePressed(Qt::MouseButtons)));

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

QSize DBorderlessWidgetPrivate::externSize(const QSize &size) const
{
    return QSize(size.width() + (m_ShadowWidth + m_Border) * 2,
                 size.height() + (m_ShadowWidth + m_Border) * 2);
}

QMargins DBorderlessWidgetPrivate::externMargins() const
{
    return rootLayout->contentsMargins() + contentWidget->contentsMargins();
}

int DBorderlessWidgetPrivate::externWidth() const
{
    return m_ShadowWidth + m_Border;
}

void DBorderlessWidgetPrivate::updateContentsMargins()
{
    rootLayout->setContentsMargins(m_ShadowWidth + m_Border - shadowOffset.x(),
                                   m_ShadowWidth + m_Border - shadowOffset.y(),
                                   m_ShadowWidth + m_Border + shadowOffset.x(),
                                   m_ShadowWidth + m_Border + shadowOffset.y());
}

void DBorderlessWidgetPrivate::_q_onTitleBarMousePressed(Qt::MouseButtons buttons) const
{
#ifdef Q_OS_LINUX
    D_QC(DBorderlessWidget);

    if (buttons != Qt::LeftButton) {
        XUtils::CancelMoveWindow(q, Qt::LeftButton);
    }
#else
    Q_UNUSED(buttons);
#endif
}

DBorderlessWidget::DBorderlessWidget(QWidget *parent): DBorderlessWidget(*new DBorderlessWidgetPrivate(this), parent)
{

}

DBorderlessWidget::DecorationFlags DBorderlessWidget::decorationFlags()
{
    D_D(DBorderlessWidget);
    return d->decorationFlags;
}

void DBorderlessWidget::setDecorationFlags(DBorderlessWidget::DecorationFlags flags)
{
    D_D(DBorderlessWidget);
    d->decorationFlags = flags;

    if (flags & ShowTitlebarSeparator) {
        d->titlebar->setSeparatorVisible(true);
    } else {
        d->titlebar->setSeparatorVisible(false);
    }
}

DBorderlessWidget::DBorderlessWidget(DBorderlessWidgetPrivate &dd, QWidget *parent)
    : QWidget(parent), DObject(dd)
{
    D_D(DBorderlessWidget);
    d->init();
    QWidget::setMouseTracking(true);

    QWidget::setAttribute(Qt::WA_TranslucentBackground, true);
    QWidget::setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    setWindowFlags(windowFlags());
    setDecorationFlags(decorationFlags());

    DBorderlessWidget::adjustSize();
#ifdef Q_OS_LINUX
    XUtils::SetMouseTransparent(this, true);
#endif
#ifdef DTK_TITLE_DRAG_WINDOW
    connect(d->titlebar, &DTitlebar::mousePosMoving,
    this, [ = ](Qt::MouseButton /*botton*/, QPoint pos) {
        move(pos - d->m_LastMousePos);
    });

    connect(d->titlebar, &DTitlebar::mousePosPressed,
    this, [ = ](Qt::MouseButtons /*botton*/, QPoint pos) {
        // TODO: fix margin
        pos.setY(pos.y() - 10);
        d->m_LastMousePos = pos - this->mapToParent(this->pos());
    });
#endif
}

void DBorderlessWidget::enterEvent(QEvent *e)
{
//    qDebug() << "enterEvent" ;
    return QWidget::enterEvent(e);
}

void DBorderlessWidget::leaveEvent(QEvent *e)
{
//    qDebug() << "leaveEvent";
    return QWidget::leaveEvent(e);
}

void DBorderlessWidget::changeEvent(QEvent *event)
{
    D_D(DBorderlessWidget);

    if (event->type() == QEvent::WindowStateChange) {
        d->updateContentsMargins();
    }

    QWidget::changeEvent(event);
}

void DBorderlessWidget::mouseMoveEvent(QMouseEvent *event)
{
#ifdef Q_OS_LINUX
    D_D(DBorderlessWidget);

    const int x = event->x();
    const int y = event->y();

    if (d->resizingCornerEdge == XUtils::CornerEdge::kInvalid && d->resizable) {
        XUtils::UpdateCursorShape(this, x, y, d->externMargins(), d->m_ResizeHandleWidth);
    }
#endif

    return QWidget::mouseMoveEvent(event);
}

void DBorderlessWidget::mousePressEvent(QMouseEvent *event)
{
#ifdef Q_OS_LINUX
    D_D(DBorderlessWidget);

    const int x = event->x();
    const int y = event->y();
    if (event->button() == Qt::LeftButton) {

        const XUtils::CornerEdge ce = XUtils::GetCornerEdge(this, x, y, d->externMargins(), d->m_ResizeHandleWidth);
        if (ce != XUtils::CornerEdge::kInvalid) {
            d->resizingCornerEdge = ce;
            XUtils::StartResizing(this, QCursor::pos(), ce);
        }

    }
#endif
    return QWidget::mousePressEvent(event);
}

void DBorderlessWidget::mouseReleaseEvent(QMouseEvent *event)
{
#ifdef Q_OS_LINUX
    D_D(DBorderlessWidget);
    d->resizingCornerEdge = XUtils::CornerEdge::kInvalid;
#endif
    return QWidget::mouseReleaseEvent(event);
}

void DBorderlessWidget::showMinimized()
{
#ifdef Q_OS_LINUX
    XUtils::ShowMinimizedWindow(this, true);
#endif
    QWidget::showMinimized();
}

void DBorderlessWidget::showMaximized()
{
#ifdef Q_OS_LINUX
    XUtils::ShowMaximizedWindow(this);
#endif
    this->show();
    this->activateWindow();
    this->raise();
}

void DBorderlessWidget::setContentsMargins(int left, int top, int right, int bottom)
{
    D_D(DBorderlessWidget);
    d->contentWidget->setContentsMargins(left, top, right, bottom);
}

void DBorderlessWidget::setContentsMargins(const QMargins &margins)
{
    D_D(DBorderlessWidget);
    d->contentWidget->setContentsMargins(margins);
}

void DBorderlessWidget::getContentsMargins(int *left, int *top, int *right, int *bottom) const
{
    D_DC(DBorderlessWidget);
    d->contentWidget->getContentsMargins(left, top, right, bottom);
}

QMargins DBorderlessWidget::contentsMargins() const
{
    D_DC(DBorderlessWidget);
    return d->contentWidget->contentsMargins();
}

void DBorderlessWidget::showFullScreen()
{
#ifdef Q_OS_LINUX
    XUtils::ShowFullscreenWindow(this, true);
#endif
    this->show();
    this->activateWindow();
    this->raise();
}

void DBorderlessWidget::moveWindow(Qt::MouseButton botton)
{
#ifdef Q_OS_LINUX
    XUtils::MoveWindow(this, botton);
#endif
}

void DBorderlessWidget::toggleMaximizedWindow()
{
#ifdef Q_OS_LINUX
    XUtils::ToggleMaximizedWindow(this);
#endif
}

void DBorderlessWidget::showNormal()
{
#ifdef Q_OS_LINUX
    XUtils::ShowNormalWindow(this);
#endif
}


void DBorderlessWidget::removeLayout()
{
    qDeleteAll(this->children());
}

Qt::WindowFlags DBorderlessWidget::windowFlags()
{
    D_D(DBorderlessWidget);
    return d->dwindowFlags;
}

///
/// \brief setWindowFlags
/// \param type
///
void DBorderlessWidget::setWindowFlags(Qt::WindowFlags type)
{
    D_D(DBorderlessWidget);
    d->dwindowFlags = type;
    d->titlebar->setWindowFlags(type);
}

int DBorderlessWidget::titlebarHeight() const
{
    D_DC(DBorderlessWidget);
    return d->titlebar->height();
}

void DBorderlessWidget::setTitlebarFixedHeight(int h)
{
    D_D(DBorderlessWidget);
    d->titlebar->setFixedHeight(h);
    DBorderlessWidget::adjustSize();
}

void DBorderlessWidget::setTitle(const QString &t)
{
    D_D(DBorderlessWidget);
    d->titlebar->setTitle(t);
}

void DBorderlessWidget::setTitleIcon(const QPixmap &icon)
{
    D_D(DBorderlessWidget);
    d->titlebar->setIcon(icon);
}

DTitlebar *DBorderlessWidget::titlebar() const
{
    D_DC(DBorderlessWidget);

    return d->titlebar;
}

void DBorderlessWidget::setTitlebarMenu(QMenu *menu)
{
    D_D(DBorderlessWidget);
    d->titlebar->setMenu(menu);
}

QMenu *DBorderlessWidget::titleBarMenu() const
{
    D_DC(DBorderlessWidget);
    return d->titlebar->menu();
}

void DBorderlessWidget::setTitlebarWidget(QWidget *w, bool fixCenterPos)
{
    D_D(DBorderlessWidget);
    d->titlebar->setCustomWidget(w, Qt::AlignCenter, fixCenterPos);
}

void DBorderlessWidget::setTitlebarWidget(QWidget *w, Qt::AlignmentFlag wflag, bool fixCenterPos)
{
    D_D(DBorderlessWidget);
    d->titlebar->setCustomWidget(w, wflag, fixCenterPos);
}

// TODO: fix layout
QLayout *DBorderlessWidget::layout() const
{
    D_DC(DBorderlessWidget);
    return d->contentWidget->layout();
}

void DBorderlessWidget::setLayout(QLayout *l)
{
    D_D(DBorderlessWidget);

    qDeleteAll(d->contentWidget->children());
    d->contentWidget->setLayout(l);
    d->contentWidget->adjustSize();
    d->windowWidget->adjustSize();
    DBorderlessWidget::resize(d->windowWidget->size());
}

int DBorderlessWidget::radius() const
{
    D_DC(DBorderlessWidget);
    return d->m_Radius;
}

void DBorderlessWidget::setRadius(int r)
{
    D_D(DBorderlessWidget);
    if (r > d->m_Border * 2) {
        r = d->m_Border * 2;
    }
    d->m_Radius = r;
}

int DBorderlessWidget::shadowWidth() const
{
    D_DC(DBorderlessWidget);
    return d->m_Radius;
}

void DBorderlessWidget::setShadowWidth(int r)
{
    D_D(DBorderlessWidget);

    if (d->m_ShadowWidth == r) {
        return;
    }

    d->m_Radius = r;
    d->m_ShadowWidth = r;

    d->updateContentsMargins();
    drawShadowPixmap();
    update();
}

QColor DBorderlessWidget::backgroundColor() const
{
    D_DC(DBorderlessWidget);

    return d->m_backgroundColor;
}

QColor DBorderlessWidget::shadowColor() const
{
    D_DC(DBorderlessWidget);

    return d->shadowColor;
}

QPoint DBorderlessWidget::shadowOffset() const
{
    D_DC(DBorderlessWidget);

    return d->shadowOffset;
}

void DBorderlessWidget::setBackgroundColor(QColor backgroundColor)
{
    D_D(DBorderlessWidget);

    if (d->m_backgroundColor == backgroundColor) {
        return;
    }

    d->m_backgroundColor = backgroundColor;
    Q_EMIT backgroundColorChanged(backgroundColor);
}

void DBorderlessWidget::setShadowColor(QColor shadowColor)
{
    D_D(DBorderlessWidget);

    if (d->shadowColor == shadowColor) {
        return;
    }

    d->shadowColor = shadowColor;

    drawShadowPixmap();
    update();

    Q_EMIT shadowColorChanged(shadowColor);
}

void DBorderlessWidget::setShadowOffset(QPoint shadowOffset)
{
    D_D(DBorderlessWidget);

    if (d->shadowOffset == shadowOffset) {
        return;
    }

    d->shadowOffset = shadowOffset;

    d->updateContentsMargins();
    update();

    Q_EMIT shadowOffsetChanged(shadowOffset);
}

void DBorderlessWidget::drawShadowPixmap()
{
    D_D(DBorderlessWidget);

    QPixmap pixmap(QWidget::size() - QSize(d->m_ShadowWidth * 2, d->m_ShadowWidth * 2));

    pixmap.fill(Qt::black);

    d->shadowPixmap = QPixmap::fromImage(Dtk::Widget::dropShadow(pixmap, d->m_ShadowWidth, d->shadowColor));
}

int DBorderlessWidget::border() const
{
    D_DC(DBorderlessWidget);
    return d->m_Border;
}

void DBorderlessWidget::setBorder(int b)
{
    D_D(DBorderlessWidget);
    if (b < 0) { b = 0; }
    d->m_Border = b;
    if (d->m_Radius > b * 2) {
        d->m_Radius = b * 2;
    }
}

const QPixmap &DBorderlessWidget::backgroundImage() const
{
    D_DC(DBorderlessWidget);
    return d->m_Background;
}

void DBorderlessWidget::setBackgroundImage(const QPixmap &srcPixmap)
{
    D_D(DBorderlessWidget);

    int radius = d->m_Radius;
    int windowExtern = d->m_ShadowWidth + d->m_Border * 2;
    QRect windowRect = QWidget::rect().marginsRemoved(
                           QMargins(windowExtern, windowExtern, windowExtern, windowExtern));
    QSize sz = windowRect.size();
    QPixmap backgroundPixmap = srcPixmap.scaled(sz, Qt::KeepAspectRatioByExpanding);

    QPixmap maskPixmap(sz);
    maskPixmap.fill(Qt::transparent);
    QPainterPath path;
    path.addRoundRect(QRectF(0, 0, sz.width(), sz.height()), radius / 2);
    QPainter bkPainter(&maskPixmap);
    bkPainter.setRenderHint(QPainter::Antialiasing);
    bkPainter.setPen(QPen(Qt::white, 1));
    bkPainter.fillPath(path, QBrush(Qt::red));

    QPainter::CompositionMode mode = QPainter::CompositionMode_SourceIn;
    QImage resultImage = QImage(sz, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&resultImage);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(resultImage.rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, maskPixmap.toImage());
    painter.setCompositionMode(mode);
    painter.drawImage(0, 0, backgroundPixmap.toImage());
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    painter.end();

    d->m_Background = QPixmap::fromImage(resultImage);
}

void DBorderlessWidget::setFixedSize(const QSize &size)
{
    D_D(DBorderlessWidget);
    d->resizable = false;
    d->titlebar->setMinimumWidth(size.width());
    d->windowWidget->setFixedSize(size);
    d->contentWidget->setFixedSize(size.width(), size.height() - titlebarHeight());
    QSize externSize = d->externSize(size);
    QWidget::setFixedSize(externSize);
    setWindowFlags(windowFlags() & ~ Qt::WindowMaximizeButtonHint);

#ifdef Q_OS_LINUX
    XUtils::DisableResize(this);
#endif
}

void DBorderlessWidget::setFixedSize(int w, int h)
{
    DBorderlessWidget::setFixedSize(QSize(w, h));
}

void DBorderlessWidget::setFixedWidth(int w)
{
    D_D(DBorderlessWidget);
    d->titlebar->setFixedWidth(w);
    d->windowWidget->setFixedWidth(w);
    QWidget::setFixedWidth(w + d->m_ShadowWidth + d->m_Border);
}

void DBorderlessWidget::resize(int width, int height)
{
    resize(QSize(width, height));
}

void DBorderlessWidget::resize(const QSize &size)
{
    D_D(DBorderlessWidget);
    d->windowWidget->resize(size);
    d->titlebar->resize(size.width(), d->titlebar->height());

    QSize externSize = d->externSize(size);
    if (d->resizable) {
        QWidget::setMinimumSize(externSize);
        QWidget::resize(externSize);
    } else {
        QWidget::setMaximumSize(externSize);
    }
}

void DBorderlessWidget::adjustSize()
{
    D_D(DBorderlessWidget);
    d->windowWidget->adjustSize();
    QSize externSize = d->externSize(d->windowWidget->size());
    if (d->resizable) {
        QWidget::setMinimumSize(externSize);
        QWidget::resize(externSize);
    } else {
        QWidget::setMaximumSize(externSize);
        QWidget::resize(externSize);
    }
}

QSize DBorderlessWidget::size() const
{
    D_DC(DBorderlessWidget);
    return d->windowWidget->size();
}


void DBorderlessWidget::move(int x, int y)
{
    D_D(DBorderlessWidget);
    QWidget::move(x - d->externWidth(), y - d->externWidth());
}

void DBorderlessWidget::move(const QPoint &p)
{
    DBorderlessWidget::move(p.x(), p.y());
}

QRect DBorderlessWidget::frameGeometry() const
{
    D_DC(DBorderlessWidget);
    return d->windowWidget->frameGeometry();
}

const QRect &DBorderlessWidget::geometry() const
{
    D_DC(DBorderlessWidget);
    return d->windowWidget->geometry();
}

QRect DBorderlessWidget::normalGeometry() const
{
    D_DC(DBorderlessWidget);
    return d->windowWidget->normalGeometry();
}

int DBorderlessWidget::x() const
{
    D_DC(DBorderlessWidget);
    return d->windowWidget->x();
}

int DBorderlessWidget::y() const
{
    D_DC(DBorderlessWidget);
    return d->windowWidget->y();
}

QPoint DBorderlessWidget::pos() const
{
    D_DC(DBorderlessWidget);
    return d->windowWidget->pos();
}

QSize DBorderlessWidget::frameSize() const
{
    D_DC(DBorderlessWidget);
    return d->windowWidget->frameSize();
}

int DBorderlessWidget::width() const
{
    D_DC(DBorderlessWidget);
    return d->windowWidget->width();
}

int DBorderlessWidget::height() const
{
    D_DC(DBorderlessWidget);
    return d->windowWidget->height();
}

inline QRect DBorderlessWidget::rect() const
{
    D_DC(DBorderlessWidget);
    return d->windowWidget->rect();
}

QRect DBorderlessWidget::childrenRect() const
{
    D_DC(DBorderlessWidget);
    return d->windowWidget->childrenRect();
}

QRegion DBorderlessWidget::childrenRegion() const
{
    D_DC(DBorderlessWidget);
    return d->windowWidget->childrenRegion();
}

void DBorderlessWidget::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);
}

void DBorderlessWidget::resizeEvent(QResizeEvent *e)
{
    D_D(DBorderlessWidget);
#ifdef Q_OS_LINUX
    int resizeHandleWidth = d->resizable ? d->m_ResizeHandleWidth : 0;
    XUtils::SetWindowExtents(this, d->externMargins(), resizeHandleWidth);
#endif

    drawShadowPixmap();
    d->titlebar->resize(e->size().width(), d->titlebar->height());
    QWidget::resizeEvent(e);
}

void DBorderlessWidget::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent(e);
}

void DBorderlessWidget::paintEvent(QPaintEvent * /*e*/)
{
    D_D(DBorderlessWidget);
    int radius = d->m_Radius;
    //. TODO: border not  part of window?
    int windowExtern = d->m_ShadowWidth + d->m_Border * 2;
    int contentExtern = d->m_ShadowWidth + d->m_Border * 2;

    QPainter painter(this);

    painter.drawPixmap(0, 0, d->shadowPixmap);

    QRect contentRect = QWidget::rect().marginsRemoved(QMargins(contentExtern, contentExtern, contentExtern, contentExtern));
    QRect windowRect = QWidget::rect().marginsRemoved(QMargins(windowExtern, windowExtern, windowExtern, windowExtern));

    contentRect.moveTopLeft(contentRect.topLeft() - d->shadowOffset);
    windowRect.moveTopLeft(windowRect.topLeft() - d->shadowOffset);

    if (! d->m_Background.isNull()) {
        painter.drawPixmap(contentRect, d->m_Background);
    } else {
        QPoint topLeft(windowRect.x(), windowRect.y());
        QPoint bottomRight(windowRect.x() + windowRect.width(), windowRect.y() + windowRect.height());
        QPainterPath border;
        border.addRoundedRect(windowRect, radius, radius);

        QLinearGradient linearGradient(topLeft, QPoint(topLeft.x(), bottomRight.y()));
        linearGradient.setColorAt(0.0, d->m_backgroundColor);
        linearGradient.setColorAt(0.2, d->m_backgroundColor);
        linearGradient.setColorAt(1.0, d->m_backgroundColor);

        QPen borderPen(BorderColor);

        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(QBrush(linearGradient));
        painter.strokePath(border, borderPen);
        painter.fillPath(border, /*palette().background()*/QBrush(linearGradient));
    }
}

FilterMouseMove::FilterMouseMove(QObject *object) : QObject(object)
{

}

FilterMouseMove::~FilterMouseMove()
{

}

bool FilterMouseMove::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type()) {
    case QEvent::Enter: {
        if (qobject_cast<QWidget *>(obj) != qobject_cast<QWidget *>(this->parent())) {
            break;
        }
        if (m_rootWidget) {
#ifdef Q_OS_LINUX
            XUtils::ResetCursorShape(m_rootWidget);
#endif
        }
        break;
    }
    default: {
    }
    }
    return QObject::eventFilter(obj, event);
}

DWIDGET_END_NAMESPACE

#include "moc_dborderlesswidget.cpp"
