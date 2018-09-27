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

/*!
 * \~chinese @class DBorderlessWidget
 *
 * \~chinese @brief DBorderlessWidget 是DTK中用于提供无边框的功能的类，主要用于Windows。
 * \~chinese - 通过设置窗口的WindowFlags为Qt::Window | Qt::FramelessWindowHint来实现
 */

/*!
 * \~chinese \fn DBorderlessWidget::optionClicked()
 * \~chinese 会在标题栏被点击时触发
 * \~chinese \fn DBorderlessWidget::backgroundColorChanged(QColor backgroundColor)
 * \~chinese 会在背景颜色发生改变时时触发
 * \~chinese \fn DBorderlessWidget::shadowColorChanged(QColor shadowColor)
 * \~chinese 会在阴影颜色发生改变时触发
 * \~chinese \fn DBorderlessWidget::shadowOffsetChanged(QPoint shadowOffset)
 * \~chinese 会在阴影偏移被设置时触发
 */

/*!
 * \~chinese \enum DBorderlessWidget::DecorationFlag
 * \~chinese @brief DBorderlessWidget::DecorationFlag 定义了修饰的标志
 */

/*!
 * \~chinese \var DBorderlessWidget::ShowTitlebarSeparator
 * \~chinese @brief 显示标题栏分隔栏
 */

/*!
 * @brief 用于构建DBorderlessWidget实例的构造函数
 * @param parent 接收QWidget的parent指针
 */

DBorderlessWidget::DBorderlessWidget(QWidget *parent): DBorderlessWidget(*new DBorderlessWidgetPrivate(this), parent)
{

}

/*!
 * \~chinese @brief 返回装饰标志
 * \~chinese @return DBorderlessWidget::DecorationFlags
 */

DBorderlessWidget::DecorationFlags DBorderlessWidget::decorationFlags()
{
    D_D(DBorderlessWidget);
    return d->decorationFlags;
}

/*!
 * \~chinese @brief 设置当前的修饰标志
 * \~chinese @param flags 接收一个DBorderlessWidget::DecorationFlags的枚举
 */

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

/*!
 * \~chinese @brief 用于构建DBorderlessWidget实例的构造函数
 *
 * \~chinese @param dd 接收私有类
 * \~chinese @param parent 设置parent
 */

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

/*!
* \~chinese \brief 该方法会在Linux下调用XUtils::ShowMinimizedWindow(this, true);
*/
void DBorderlessWidget::showMinimized()
{
#ifdef Q_OS_LINUX
    XUtils::ShowMinimizedWindow(this, true);
#endif
    QWidget::showMinimized();
}

/*!
 * \~chinese @brief 该方法会在Linux下调用XUtils::ShowMaximizedWindow(this, true);
 */
void DBorderlessWidget::showMaximized()
{
#ifdef Q_OS_LINUX
    XUtils::ShowMaximizedWindow(this);
#endif
    this->show();
    this->activateWindow();
    this->raise();
}

/*!
 * \~chinese @brief 设置内部控件的外边距
 * \~chinese
 * \~chinese @param left 设置左边缘的距离
 * \~chinese @param top 设置上边缘的距离
 * \~chinese @param right 设置右边缘的距离
 * \~chinese @param bottom 设置下边缘的距离
 */
void DBorderlessWidget::setContentsMargins(int left, int top, int right, int bottom)
{
    D_D(DBorderlessWidget);
    d->contentWidget->setContentsMargins(left, top, right, bottom);
}

/*!
 * \~chinese @brief 重载的方法，接收QMargins对象来设置内部控件的外边距
 *
 * \~chinese @param margins QMargins对象
 */
void DBorderlessWidget::setContentsMargins(const QMargins &margins)
{
    D_D(DBorderlessWidget);
    d->contentWidget->setContentsMargins(margins);
}

/*!
 * \~chinese @brief 获取内部控件的外边距
 *
 * \~chinese @param left 左边距的指针
 * \~chinese @param top 上边距的指针
 * \~chinese @param right 右边距的指针
 * \~chinese @param bottom 下边距的指针
 */
void DBorderlessWidget::getContentsMargins(int *left, int *top, int *right, int *bottom) const
{
    D_DC(DBorderlessWidget);
    d->contentWidget->getContentsMargins(left, top, right, bottom);
}

/*!
 * \~chinese @brief 获取内部控件的外边距
 *
 * \~chinese @return QMargins 返回QMargins对象
 */
QMargins DBorderlessWidget::contentsMargins() const
{
    D_DC(DBorderlessWidget);
    return d->contentWidget->contentsMargins();
}

/*!
 * \~chinese @brief 显示全屏
 * \~chinese 会在Linux下调用XUtils::ShowFullscreenWindow(this, true);
 */
void DBorderlessWidget::showFullScreen()
{
#ifdef Q_OS_LINUX
    XUtils::ShowFullscreenWindow(this, true);
#endif
    this->show();
    this->activateWindow();
    this->raise();
}

/*!
 * \~chinese @brief 移动窗口
 * \~chinese @param botton 接收Qt::MouseButton枚举值
 */
void DBorderlessWidget::moveWindow(Qt::MouseButton botton)
{
#ifdef Q_OS_LINUX
    XUtils::MoveWindow(this, botton);
#endif
}

/*!
 * \~chinese @brief 切换窗口为最大化或者还原
 *
 */
void DBorderlessWidget::toggleMaximizedWindow()
{
#ifdef Q_OS_LINUX
    XUtils::ToggleMaximizedWindow(this);
#endif
}

/*!
 * \~chinese @brief 窗口将以标准模式显示
 *
 */
void DBorderlessWidget::showNormal()
{
#ifdef Q_OS_LINUX
    XUtils::ShowNormalWindow(this);
#endif
}

/*!
 * \~chinese @brief 删除当前的布局
 *
 */
void DBorderlessWidget::removeLayout()
{
    qDeleteAll(this->children());
}

/*!
 * \~chinese @brief 返回当前的WindowFlags
 *
 * \~chinese @return Qt::WindowFlags Qt的窗口标志枚举值
 */
Qt::WindowFlags DBorderlessWidget::windowFlags()
{
    D_D(DBorderlessWidget);
    return d->dwindowFlags;
}

/*!
 * \~english @brief set the required flags for the window
 *
 * \~english @param type Qt::WindowFlags enum
 *
 * \~chinese @brief 设置窗口需要的标志位
 * \~chinese @param type Qt::WindowFlags 枚举值
 */
void DBorderlessWidget::setWindowFlags(Qt::WindowFlags type)
{
    D_D(DBorderlessWidget);
    d->dwindowFlags = type;
    d->titlebar->setWindowFlags(type);
}

/*!
 * \~chinese @brief 返回标题栏的高度
 *
 * \~chinese @return int 标题栏的高度
 */
int DBorderlessWidget::titlebarHeight() const
{
    D_DC(DBorderlessWidget);
    return d->titlebar->height();
}

/*!
 * \~chinese @brief 设置标题栏的固定高度
 *
 * \~chinese @param h 高度
 */
void DBorderlessWidget::setTitlebarFixedHeight(int h)
{
    D_D(DBorderlessWidget);
    d->titlebar->setFixedHeight(h);
    DBorderlessWidget::adjustSize();
}

/*!
 * \~chinese @brief 设置标题
 *
 * \~chinese @param t 标题内容
 */
void DBorderlessWidget::setTitle(const QString &t)
{
    D_D(DBorderlessWidget);
    d->titlebar->setTitle(t);
}

/*!
 * \~chinese @brief 设置标题栏的图标
 *
 * \~chinese @param icon 图标对象
 */
void DBorderlessWidget::setTitleIcon(const QPixmap &icon)
{
    D_D(DBorderlessWidget);
    d->titlebar->setIcon(icon);
}

/*!
 * \~chinese @brief 返回标题栏对象
 *
 * \~chinese @return DTitlebar* 标题栏对象指针
 */
DTitlebar *DBorderlessWidget::titlebar() const
{
    D_DC(DBorderlessWidget);

    return d->titlebar;
}

/*!
 * \~chinese @brief 设置标题栏菜单
 *
 * \~chinese @param menu 菜单对象指针
 */
void DBorderlessWidget::setTitlebarMenu(QMenu *menu)
{
    D_D(DBorderlessWidget);
    d->titlebar->setMenu(menu);
}

/*!
 * \~chinese @brief 返回标题栏的菜单对象
 *
 * \~chinese @return QMenu* 菜单对象的指针
 */
QMenu *DBorderlessWidget::titleBarMenu() const
{
    D_DC(DBorderlessWidget);
    return d->titlebar->menu();
}

/*!
 * \~chinese @brief 设置标题栏的控件
 *
 * \~chinese @param w 要添加在标题栏的QWidget控件
 * \~chinese @param fixCenterPos 从中心偏移的距离
 */
void DBorderlessWidget::setTitlebarWidget(QWidget *w, bool fixCenterPos)
{
    D_D(DBorderlessWidget);
    d->titlebar->setCustomWidget(w, Qt::AlignCenter, fixCenterPos);
}

/*!
 * \~chinese @brief 重载的方法，增加了设置对齐方式的参数
 *
 * \~chinese @param w 要添加在标题栏的QWidget控件
 * \~chinese @param wflag 设置对齐方式
 * \~chinese @param fixCenterPos 从中心偏移的距离
 */
void DBorderlessWidget::setTitlebarWidget(QWidget *w, Qt::AlignmentFlag wflag, bool fixCenterPos)
{
    D_D(DBorderlessWidget);
    d->titlebar->setCustomWidget(w, wflag, fixCenterPos);
}


/*!
 * \~chinese @brief 返回当前的布局
 *
 */
// TODO: fix layout
QLayout *DBorderlessWidget::layout() const
{
    D_DC(DBorderlessWidget);
    return d->contentWidget->layout();
}

/*!
 * \~chinese @brief 设置布局
 *
 * \~chinese @param l 布局对象
 */
void DBorderlessWidget::setLayout(QLayout *l)
{
    D_D(DBorderlessWidget);

    qDeleteAll(d->contentWidget->children());
    d->contentWidget->setLayout(l);
    d->contentWidget->adjustSize();
    d->windowWidget->adjustSize();
    DBorderlessWidget::resize(d->windowWidget->size());
}

/*!
 * \~chinese @brief 返回控件的圆角半径
 *
 * \~chinese @return int 半径
 */
int DBorderlessWidget::radius() const
{
    D_DC(DBorderlessWidget);
    return d->m_Radius;
}

/*!
 * \~chinese @brief 设置控件的圆角半径
 *
 * \~chinese @param r 半径
 */
void DBorderlessWidget::setRadius(int r)
{
    D_D(DBorderlessWidget);
    if (r > d->m_Border * 2) {
        r = d->m_Border * 2;
    }
    d->m_Radius = r;
}

/*!
 * \~chinese @brief 返回阴影的宽度
 *
 * \~chinese @return int 阴影宽度
 */
int DBorderlessWidget::shadowWidth() const
{
    D_DC(DBorderlessWidget);
    return d->m_Radius;
}

/*!
 * \~chinese @brief 设置阴影的宽度
 *
 * \~chinese @param r 阴影的宽度
 */
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

/*!
 * \~chinese @property DBorderlessWidget::backgroundColor
 * \~chinese @brief 返回背景颜色
 *
 * \~chinese @return QColor 颜色对象
 */
QColor DBorderlessWidget::backgroundColor() const
{
    D_DC(DBorderlessWidget);

    return d->m_backgroundColor;
}

/*!
 * \~chinese @brief 返回阴影的颜色
 *
 * \~chinese @return QColor 颜色对象
 */
QColor DBorderlessWidget::shadowColor() const
{
    D_DC(DBorderlessWidget);

    return d->shadowColor;
}

/*!
 * \~chinese @brief 返回阴影的偏移
 *
 * \~chinese @return QPoint 坐标对象
 */
QPoint DBorderlessWidget::shadowOffset() const
{
    D_DC(DBorderlessWidget);

    return d->shadowOffset;
}

/*!
 * \~chinese @brief 设置背景颜色
 *
 * \~chinese @param backgroundColor 颜色对象
 */
void DBorderlessWidget::setBackgroundColor(QColor backgroundColor)
{
    D_D(DBorderlessWidget);

    if (d->m_backgroundColor == backgroundColor) {
        return;
    }

    d->m_backgroundColor = backgroundColor;
    Q_EMIT backgroundColorChanged(backgroundColor);
}

/*!
 * \~chinese @brief 设置阴影颜色
 *
 * \~chinese @param shadowColor 颜色对象
 */
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

/*!
 * \~chinese @brief 设置阴影的偏移
 *
 * \~chinese @param shadowOffset 偏移的坐标对象
 */
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

/*!
 * \~chinese @brief 绘制阴影的图像
 *
 */
void DBorderlessWidget::drawShadowPixmap()
{
    D_D(DBorderlessWidget);

    QPixmap pixmap(QWidget::size() - QSize(d->m_ShadowWidth * 2, d->m_ShadowWidth * 2));

    pixmap.fill(Qt::black);

    d->shadowPixmap = QPixmap::fromImage(Dtk::Widget::dropShadow(pixmap, d->m_ShadowWidth, d->shadowColor));
}

/*!
 * \~chinese @brief 返回边框的大小
 *
 * \~chinese @return int 边框大小
 */
int DBorderlessWidget::border() const
{
    D_DC(DBorderlessWidget);
    return d->m_Border;
}

/*!
 * \~chinese @brief 设置边框的大小
 *
 * \~chinese @param b 设置的大小
 */
void DBorderlessWidget::setBorder(int b)
{
    D_D(DBorderlessWidget);
    if (b < 0) { b = 0; }
    d->m_Border = b;
    if (d->m_Radius > b * 2) {
        d->m_Radius = b * 2;
    }
}

/*!
 * \~chinese @brief 返回背景的图像
 *
 * \~chinese @return const QPixmap& 背景的图像对象
 */
const QPixmap &DBorderlessWidget::backgroundImage() const
{
    D_DC(DBorderlessWidget);
    return d->m_Background;
}

/*!
 * \~chinese @brief 设置背景的图像数据
 *
 * \~chinese @param srcPixmap 图像数据
 */
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

/*!
 * \~chinese @brief 设置固定大小
 *
 * \~chinese @param size 大小
 */
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

/*!
 * \~chinese @brief 重载的方法，设置固定大小
 *
 * \~chinese @param w 固定的宽度
 * \~chinese @param h 固定的高度
 */
void DBorderlessWidget::setFixedSize(int w, int h)
{
    DBorderlessWidget::setFixedSize(QSize(w, h));
}

/*!
 * \~chinese @brief 设置固定宽度
 *
 * \~chinese @param w 固定的宽度
 */
void DBorderlessWidget::setFixedWidth(int w)
{
    D_D(DBorderlessWidget);
    d->titlebar->setFixedWidth(w);
    d->windowWidget->setFixedWidth(w);
    QWidget::setFixedWidth(w + d->m_ShadowWidth + d->m_Border);
}

/*!
 * \~chinese @brief 调整控件的大小
 * \~chinese - 可以调整的范围介于minimumSize和maxmumSize之间
 *
 * @param width 设置宽度
 * @param height 设置高度
 */
void DBorderlessWidget::resize(int width, int height)
{
    resize(QSize(width, height));
}

/*!
 * \~chinese @brief 这是调整大小的重载方法，接收一个QSzie对象
 *
 * \~chinese @param size 要调整的大小
 */
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

/*!
 * \~chinese @brief 立即调整大小
 *
 */
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

/*!
 * \~chinese @brief 返回控件当前的大小
 *
 * \~chinese @return QSize
 */
QSize DBorderlessWidget::size() const
{
    D_DC(DBorderlessWidget);
    return d->windowWidget->size();
}

/*!
 * \~chinese @brief 移动控件到某一点
 *
 * \~chinese @param x X坐标
 * \~chinese @param y Y坐标
 */
void DBorderlessWidget::move(int x, int y)
{
    D_D(DBorderlessWidget);
    QWidget::move(x - d->externWidth(), y - d->externWidth());
}

/*!
 * \~chinese @brief 这是重载的方法，接收一个坐标对象
 *
 * \~chinese @param p 要移动到的坐标
 */
void DBorderlessWidget::move(const QPoint &p)
{
    DBorderlessWidget::move(p.x(), p.y());
}

/*!
 * \~chinese @brief 返回当前的几何尺寸，包含阴影
 *
 * \~chinese @return QRect 当前的几何大小
 */
QRect DBorderlessWidget::frameGeometry() const
{
    D_DC(DBorderlessWidget);
    return d->windowWidget->frameGeometry();
}

/*!
 * \~chinese @brief 返回当前的几何尺寸，不包含阴影的区域
 *
 * @return const QRect& 当前的几何大小
 */
const QRect &DBorderlessWidget::geometry() const
{
    D_DC(DBorderlessWidget);
    return d->windowWidget->geometry();
}

/*!
 * \~chinese @brief 此方法返回窗口小部件的几何
 * \~chinese - 对于子窗口和控件， 此方法始终包含空矩形
 *
 * @return QRect
 */
QRect DBorderlessWidget::normalGeometry() const
{
    D_DC(DBorderlessWidget);
    return d->windowWidget->normalGeometry();
}

/*!
 * \~chinese @brief 返回当前的X坐标
 *
 * \~chinese @return int
 */
int DBorderlessWidget::x() const
{
    D_DC(DBorderlessWidget);
    return d->windowWidget->x();
}

/*!
 * \~chinese @brief 返回当前的Y坐标
 *
 * \~chinese @return int
 */
int DBorderlessWidget::y() const
{
    D_DC(DBorderlessWidget);
    return d->windowWidget->y();
}

/*!
 * \~chinese @brief 返回当前的坐标
 *
 * \~chinese @return QPoint 相对于父窗口的坐标
 */
QPoint DBorderlessWidget::pos() const
{
    D_DC(DBorderlessWidget);
    return d->windowWidget->pos();
}

/*!
 * \~chinese @brief 返回窗口外部框架的大小
 *
 * \~chinese @return QSize 框架的大小
 */
QSize DBorderlessWidget::frameSize() const
{
    D_DC(DBorderlessWidget);
    return d->windowWidget->frameSize();
}

/*!
 * \~chinese @brief 返回当前的宽度
 *
 * \~chinese @return int 当前的宽度
 */
int DBorderlessWidget::width() const
{
    D_DC(DBorderlessWidget);
    return d->windowWidget->width();
}

/*!
 * \~chinese @brief 返回当前的高度
 *
 * \~chinese @return int 当前的高度
 */
int DBorderlessWidget::height() const
{
    D_DC(DBorderlessWidget);
    return d->windowWidget->height();
}

/*!
 * \~chinese @brief 返回当前的大小
 *
 * \~chinese @return QRect 当前的大小
 */
inline QRect DBorderlessWidget::rect() const
{
    D_DC(DBorderlessWidget);
    return d->windowWidget->rect();
}

/*!
 * \~chinese @brief 返回内部控件的大小
 *
 * \~chinese @return QRect 内部控件的大小
 */
QRect DBorderlessWidget::childrenRect() const
{
    D_DC(DBorderlessWidget);
    return d->windowWidget->childrenRect();
}

/*!
 * \~chinese @brief 返回内部控件的区域大小
 *
 * \~chinese @return QRegion 内部控件的区域大小
 */
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
