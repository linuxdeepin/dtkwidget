/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#include "darrowrectangle.h"
#include "dplatformwindowhandle.h"
#include "dapplication.h"
#include "private/darrowrectangle_p.h"
#include "dstyle.h"

#include <DGuiApplicationHelper>

#ifdef Q_OS_LINUX
#include <X11/extensions/shape.h>
#include <QX11Info>
#endif
#include <QApplication>
#include <QScreen>
#include <QEvent>

DWIDGET_BEGIN_NAMESPACE

/*!
  \class Dtk::Widget::DArrowRectangle
  \inmodule dtkwidget
  \brief DArrowRectangle 提供了可以在四个边中的任意一个边显示箭头的矩形控件.
  \brief The DArrowRectangle class provides a widget that has an arrow on one
  of its four borders.

  通常用于作为其他控件的容器，将其显示在矩形内作为内容控件
  It's usually used as a container of some other widgets,
  see DArrowRectangle::setContent()

  \sa DArrowRectangle::setContent()
 */

/*!
  \enum Dtk::Widget::DArrowRectangle::ArrowDirection
  \brief 箭头方向枚举包含 DArrowRectangle 的箭头可能指向的可能方向.
  \brief The ArrowDirection enum contains the possible directions that
  the DArrowRectangle's arrow may point to.

  \value ArrowLeft
  指示此矩形的箭头将指向左侧
  indicates the arrow of this rectangle will point left
  \value ArrowRight
  指示此矩形的箭头将指向右侧
  indicates the arrow of this rectangle will point right
  \value ArrowTop
  指示此矩形的箭头将指向上方
  indicates the arrow of this rectangle will point up
  \value ArrowBottom
  指示此矩形的箭头将向下指向
  indicates the arrow of this rectangle will point down
 */

/*!
  \enum Dtk::Widget::DArrowRectangle::FloatMode
  \brief FloatMode 表示不同的控件的浮动模式
  \brief The FloatMode enum decide the WindowType when DArrowRectangle show
  
  控件的浮动模式表示控件如何显示在布局中，DArrowRectangle::FloatWindow 表示控件将会以一个单独的窗口显示，而 DArrowRectangle::FloatWidget 则表示控件只能显示在其父控件的布局中，不能超出父控件大小
  
  \value FloatWindow
  窗口模式
  Window will show a separate window
  \value FloatWidget
  控件模式
  Widget must by show in the rect of parentWidget
 */

/*!
  \brief DArrowRectangle::DArrowRectangle constructs an instance of DArrowRectangle.
  \brief 获取 DArrowRectangle 实例

  \a direction is used to initialize the direction of which the arrow
  points to.
  \a parent is the parent widget the arrow rectangle will be attached to.
  \a direction 用于初始化箭头的方向
  \a parent 作为其父控件
 */
DArrowRectangle::DArrowRectangle(ArrowDirection direction, QWidget *parent) :
    QWidget(parent),
    DObject(*new DArrowRectanglePrivate(direction, this))
{
    D_D(DArrowRectangle);

    d->init(FloatWindow);
}

/*!
  \brief 获取 DArrowRectangle 实例，并指定浮动模式.
  \brief DArrowRectangle::DArrowRectangle can set DArrowRectangle show as a window or
  a widget in parentWidget by floatMode

  \a direction 用于初始化箭头的方向
  \a floatMode
  \a parent 作为其父控件
  \sa DArrowRectangle::FloatMode
 */
DArrowRectangle::DArrowRectangle(ArrowDirection direction, FloatMode floatMode, QWidget *parent) :
    QWidget(parent),
    DObject(*new DArrowRectanglePrivate(direction, this))
{
    D_D(DArrowRectangle);

    d->init(floatMode);
}

/*!
  \brief 在指定的坐标位置显示本控件；
  \brief DArrowRectangle::show shows the widget at the given coordinate.
  
  \note 坐标被计算为箭头的位置，所以你不需要自己计算箭头位置
  \note The coordiate is calculated to be the arrow head's position, so you
  don't need to calculate the position yourself.
  
  \a x 控件箭头的x轴坐标
  \a y 控件箭头的y轴坐标
  \a x is the x coordinate of the arrow head.
  \a y is the y coordinate of the arrow head.
 */
void DArrowRectangle::show(int x, int y)
{
    D_D(DArrowRectangle);

    d->show(x, y);
}

/*!
  \brief DArrowRectangle::setContent sets the content of the arrow rectangle.
  \brief 设置要显示在矩形内的内容控件.

  \a content 要显示内容控件
 */
void DArrowRectangle::setContent(QWidget *content)
{
    D_D(DArrowRectangle);

    d->setContent(content);
}

/*!
  \brief 获取内容控件.

  \return 正在显示的内容控件
 */
QWidget *DArrowRectangle::getContent() const
{
    D_DC(DArrowRectangle);

    return d->m_content;
}

/*!
  \brief 根据内容控件的大小自动设置矩形控件的大小.
  \brief DArrowRectangle::resizeWithContent automatically adjust the rectangle's
  size to fit the its content.
 */
void DArrowRectangle::resizeWithContent()
{
    D_D(DArrowRectangle);

    d->resizeWithContent();
}

/*!
  \brief 获取整个矩形控件的大小.
  \brief DArrowRectangle::getFixedSize.

  \return 矩形控件的大小
  \return the size of the whole widget.
 */
QSize DArrowRectangle::getFixedSize()
{
    D_D(DArrowRectangle);

    if (d->m_content) {
        qreal delta = (d->m_handle ? 0 : shadowBlurRadius() + d->m_shadowDistance) + margin();

        switch (d->m_arrowDirection) {
        case ArrowLeft:
        case ArrowRight:
            return QSize(d->m_content->width() + delta * 2 + d->m_arrowHeight - ((d->floatMode == FloatWidget) ? delta : 0),
                         d->m_content->height() + delta * 2);
        case ArrowTop:
        case ArrowBottom:
            return QSize(d->m_content->width() + delta * 2,
                         d->m_content->height() + delta * 2 + d->m_arrowHeight - ((d->floatMode == FloatWidget) ? delta : 0));
        }
    }

    return QSize(0, 0);
}

/*!
  \brief 移动到指定的坐标位置.
  \brief DArrowRectangle::move moves the widget to the coordinate that provided,
  
  参数的作用类似于 DArrowRectangle::show , 移动整个控件直到箭头出现在参数中指定的坐标
  Like the rules in DArrowRectangle::show(int x, int y), it moves the widget so
  that the arrow head's coordinate matches the one that provided.
  
  \a x 控件箭头的x轴坐标
  \a y 控件箭头的y轴坐标
  \a x is the x coordinate of the arrow head.
  \a y is the y coordinate of the arrow head.

  \sa DArrowRectangle::show(int x, int y)
  \sa DArrowRectangle::show
 */
void DArrowRectangle::move(int x, int y)
{
    D_D(DArrowRectangle);

    switch (d->m_arrowDirection) {
    case ArrowLeft:
    case ArrowRight:
        d->verticalMove(x, y);
        break;
    case ArrowTop:
    case ArrowBottom:
        d->horizontalMove(x, y);
        break;
    default:
        QWidget::move(x, y);
        break;
    }
}

// override methods
void DArrowRectangle::paintEvent(QPaintEvent *e)
{
    D_D(DArrowRectangle);

    d->paintEvent(e);
}

void DArrowRectangle::resizeEvent(QResizeEvent *e)
{
    D_D(DArrowRectangle);

    d->resizeEvent(e);
}

bool DArrowRectangle::event(QEvent *e)
{
    switch (e->type()) {
    case QEvent::WindowDeactivate:  Q_EMIT windowDeactivate();    break;
    case QEvent::Polish: {
        D_D(DArrowRectangle);
        if (d->m_radius < 0)
            d->m_radius = DStyleHelper(style()).pixelMetric(DStyle::PM_TopLevelWindowRadius);
        break;
    }
    default:;
    }

    return QWidget::event(e);
}

const QRect DArrowRectanglePrivate::currentScreenRect(const int x, const int y)
{
    for (QScreen *screen : qApp->screens())
        if (screen->geometry().contains(x, y)) {
            return screen->geometry();
        }

    return QRect();
}

/*!
  \property DArrowRectangle::shadowYOffset
  \property DArrowRectangle::shadowYOffset

  \brief 这属性表示小部件及其阴影在y轴上的偏移量.
  \brief the offset of the widget and its shadow on y axis.

  Getter: DArrowRectangle::shadowYOffset Setter: DArrowRectangle::setShadowYOffset
  \sa DArrowRectangle::shadowXOffset
 */
qreal DArrowRectangle::shadowYOffset() const
{
    D_DC(DArrowRectangle);

    return d->m_shadowYOffset;
}

void DArrowRectangle::setShadowYOffset(const qreal &shadowYOffset)
{
    D_D(DArrowRectangle);

    d->m_shadowYOffset = shadowYOffset;

    if (d->m_handle) {
        d->m_handle->setShadowOffset(QPoint(d->m_shadowXOffset, shadowYOffset));
    }
}

/*!
  \brief DArrowRectangle::setLeftRightRadius 设置左右箭头时的圆角.

  \a enable 是否开启.
 */
void DArrowRectangle::setLeftRightRadius(bool enable)
{
    D_D(DArrowRectangle);
    d->leftRightRadius = enable;
}

/*!
  \brief DArrowRectangle::setArrowStyleEnable 设置圆角箭头样式.

  \a enable 是否开启.
 */
void DArrowRectangle::setRadiusArrowStyleEnable(bool enable)
{
    D_D(DArrowRectangle);
    d->radiusArrowStyleEnable = enable;
    setArrowWidth(40);
    setArrowHeight(24);
}

/*!
  \property DArrowRectangle::shadowXOffset

  \brief 这属性表示小部件及其阴影在x轴上的偏移量
  \brief the offset of the widget and its shadow on x axis.
  
  Getter: DArrowRectangle::shadowXOffset Setter: DArrowRectangle::setShadowXOffset
  \sa DArrowRectangle::shadowYOffset
 */
qreal DArrowRectangle::shadowXOffset() const
{
    D_DC(DArrowRectangle);

    return d->m_shadowXOffset;
}

void DArrowRectangle::setShadowXOffset(const qreal &shadowXOffset)
{
    D_D(DArrowRectangle);

    d->m_shadowXOffset = shadowXOffset;

    if (d->m_handle) {
        d->m_handle->setShadowOffset(QPoint(shadowXOffset, d->m_shadowYOffset));
    }
}

/*!
  \property DArrowRectangle::shadowBlurRadius

  \brief 这个属性保存小部件阴影的模糊半径
  \brief This property holds the blur radius of the widget's shadow.
  
  Getter: DArrowRectangle::shadowBlurRadius Setter: DArrowRectangle::setShadowBlurRadius
 */
qreal DArrowRectangle::shadowBlurRadius() const
{
    D_DC(DArrowRectangle);

    return d->m_shadowBlurRadius;
}

void DArrowRectangle::setShadowBlurRadius(const qreal &shadowBlurRadius)
{
    D_D(DArrowRectangle);

    d->m_shadowBlurRadius = shadowBlurRadius;

    if (d->m_handle) {
        d->m_handle->setShadowRadius(shadowBlurRadius);
    }
}

/*!
  \property DArrowRectangle::borderColor

  \brief 这个属性表示控件边框的颜色
  \brief This property holds the border color of this widget.
  
  Getter: DArrowRectangle::borderColor , Setter: DArrowRectangle::setBorderColor
 */
QColor DArrowRectangle::borderColor() const
{
    D_DC(DArrowRectangle);

    return d->m_borderColor;
}

void DArrowRectangle::setBorderColor(const QColor &borderColor)
{
    D_D(DArrowRectangle);

    d->m_borderColor = borderColor;

    if (d->m_handle) {
        d->m_handle->setBorderColor(borderColor);
    }
}

/*!
  \property DArrowRectangle::borderWidth

  \brief 这个属性表示控件边框的宽度
  \brief This property holds the border width of this widget.
  
  Getter: DArrowRectangle::borderWidth , Setter: DArrowRectangle::setBorderWidth
 */
int DArrowRectangle::borderWidth() const
{
    D_DC(DArrowRectangle);

    return d->m_borderWidth;
}

void DArrowRectangle::setBorderWidth(int borderWidth)
{
    D_D(DArrowRectangle);

    d->m_borderWidth = borderWidth;

    if (d->m_handle) {
        d->m_handle->setBorderWidth(borderWidth);
    }
}

/*!
  \property DArrowRectangle::backgroundColor

  \brief 这个属性表示矩形控件的背景颜色
  \brief the background color of this rectangle.
  
  Getter: DArrowRectangle::backgroundColor , Setter: DArrowRectangle::setBackgroundColor
 */
QColor DArrowRectangle::backgroundColor() const
{
    D_DC(DArrowRectangle);

    return d->m_backgroundColor;
}

/*!
  \property DArrowRectangle::arrowDirection

  \brief This property holds the direction of the rectangle's arrow points to.
  \brief 这个属性表示箭头的方向
  
  Getter: DArrowRectangle::arrowDirection , Setter: DArrowRectangle::setArrowDirection
 */
DArrowRectangle::ArrowDirection DArrowRectangle::arrowDirection() const
{
    D_DC(DArrowRectangle);

    return d->m_arrowDirection;
}

void DArrowRectangle::setBackgroundColor(const QColor &backgroundColor)
{
    D_D(DArrowRectangle);

    d->m_backgroundColor = backgroundColor;

    if (d->m_handle && d->m_backgroundColor.toRgb().alpha() < 255) {
        if (!d->m_blurBackground) {
            d->m_blurBackground = new DBlurEffectWidget(this);
            d->m_blurBackground->setBlendMode(DBlurEffectWidget::BehindWindowBlend);
            d->m_blurBackground->resize(size());
            d->m_blurBackground->lower();
            d->m_blurBackground->show();
        }

        d->m_blurBackground->setMaskColor(d->m_backgroundColor);
    } else {
        if (d->m_blurBackground) {
            d->m_blurBackground->hide();
            d->m_blurBackground->setParent(0);
            delete d->m_blurBackground;
            d->m_blurBackground = Q_NULLPTR;
        }
    }
}

/*!
  \brief DArrowRectangle::setBackgroundColor is an overloaded function.
  \brief DArrowRectangle::setBackgroundColor 是一个重载方法
  
  通过改变 DBlurEffectWidget::MaskColorType 来修改控件矩形的背景
  It sets the background color by modifing the mask color of the
  Dtk::Widget::DBlurEffectWidget.

  \a type is the mask color to set.
  \sa DArrowRectangle::backgroundColor DBlurEffectWidget::MaskColorType
 */
void DArrowRectangle::setBackgroundColor(DBlurEffectWidget::MaskColorType type)
{
    D_D(DArrowRectangle);

    if (d->m_blurBackground) {
        d->m_blurBackground->setMaskColor(type);
    }
}

/*!
  \property DArrowRectangle::radius

  \brief 这个属性表示矩形的圆角
  \brief radius of the rectangle
  
  Getter: DArrowRectangle::radius , Setter: DArrowRectangle::setRadius
 */
int DArrowRectangle::radius() const
{
    D_DC(DArrowRectangle);

    return d->m_radius;
}

/*!
  \property DArrowRectangle::arrowHeight

  \brief height of rectangle's arrow
  \brief 这个属性表示箭头的高度
  
  Getter: DArrowRectangle::arrowHeight , Setter: DArrowRectangle::setArrowHeight
  \sa DArrowRectangle::arrowWidth
 */
int DArrowRectangle::arrowHeight() const
{
    D_DC(DArrowRectangle);

    return d->m_arrowHeight;
}

/*!
  \property DArrowRectangle::arrowWidth

  \brief 这个属性表示箭头的宽度
  \brief width of the rectangle's arrow
  
  Getter: DArrowRectangle::arrowWidth , Setter: DArrowRectangle::setArrowWidth
  \sa DArrowRectangle::arrowHeight
 */
int DArrowRectangle::arrowWidth() const
{
    D_DC(DArrowRectangle);

    return d->m_arrowWidth;
}

/*!
  \property DArrowRectangle::arrowX

  \brief the x coordinate of the rectangle's arrow
  \brief 这个属性表示箭头的x轴坐标
  
  Getter: DArrowRectangle::arrowX , Setter: DArrowRectangle::setArrowX
  \sa DArrowRectangle::arrowY
 */
int DArrowRectangle::arrowX() const
{
    D_DC(DArrowRectangle);

    return d->m_arrowX;
}

/*!
  \property DArrowRectangle::arrowY

  \brief 这个属性表示箭头的y轴坐标
  \brief the y coordinate of the rectangle's arrow
  
  Getter: DArrowRectangle::arrowY , Setter: DArrowRectangle::setArrowY
  \sa DArrowRectangle::arrowX
 */
int DArrowRectangle::arrowY() const
{
    D_DC(DArrowRectangle);

    return d->m_arrowY;
}

/*!
  \property DArrowRectangle::margin

  \brief 这个属性表示边距大小
  \brief This property holds the width of the margin
  
  The margin is the distance between the innermost pixel of the rectangle and the
  outermost pixel of its contents.
  The default margin is 0.
  边距是指矩形最里面的像素与其内容最外面的像素之间的距离
  Getter: DArrowRectangle::margin , Setter: DArrowRectangle::setMargin

  \sa DArrowRectangle::setMargin
 */
int DArrowRectangle::margin() const
{
    D_DC(DArrowRectangle);

    return d->m_margin;
}

/*!
  \brief 该函数用于设置箭头方向.

  \a value 箭头方向.

  \sa DArrowRectangle::arrowDirection
 */
void DArrowRectangle::setArrowDirection(ArrowDirection value)
{
    D_D(DArrowRectangle);

    d->m_arrowDirection = value;
}

/*!
  \brief 该函数用于设置整个控件固定的宽度
  
  \a value 宽度大小
 */
void DArrowRectangle::setWidth(int value)
{
    setFixedWidth(value);
}

/*!
  \brief 设置整个控件固定的高度
  
  \a value 高度大小
 */
void DArrowRectangle::setHeight(int value)
{
    setFixedHeight(value);
}

/*!
  \brief 该函数用于设置圆角大小.

  \a value 圆角大小.

  \sa DArrowRectangle::radius
 */
void DArrowRectangle::setRadius(int value)
{
    D_D(DArrowRectangle);

    d->m_radius = value;
}

/*!
  \brief 设置箭头高度.

  \a value 箭头高度.

  \sa DArrowRectangle::arrowHeight
 */
void DArrowRectangle::setArrowHeight(int value)
{
    D_D(DArrowRectangle);

    d->m_arrowHeight = value;
}

/*!
  \brief 设置箭头宽度.

  \a value 箭头宽度.

  \sa DArrowRectangle::arrowWidth
 */
void DArrowRectangle::setArrowWidth(int value)
{
    D_D(DArrowRectangle);

    d->m_arrowWidth = value;
}

/*!
  \brief 设置箭头 x 坐标的值.

  \a value x 坐标的值.

  \sa DArrowRectangle::arrowX
 */
void DArrowRectangle::setArrowX(int value)
{
    D_D(DArrowRectangle);

    d->m_arrowX = value;
}

/*!
  \brief 设置箭头 y 坐标的值.

  \a value y 坐标的值.

  \sa DArrowRectangle::arrowY
 */
void DArrowRectangle::setArrowY(int value)
{
    D_D(DArrowRectangle);

    d->m_arrowY = value;
}

/*!
  \brief 设置边距大小.

  \a value 边距大小.

  \sa DArrowRectangle::margin
 */
void DArrowRectangle::setMargin(int value)
{
    D_D(DArrowRectangle);

    d->m_margin = value;
}

QPainterPath DArrowRectanglePrivate::getLeftCornerPath()
{
    D_Q(DArrowRectangle);

    QRect rect = q->rect();

    if (!m_handle) {
        qreal delta = q->shadowBlurRadius() + m_shadowDistance;

        if (DApplication::isDXcbPlatform())
            rect = rect.marginsRemoved(QMargins((DArrowRectangle::FloatWidget == floatMode) ? 0 : delta, delta, delta - q->margin(), delta));
        else {
            QMargins margins((DArrowRectangle::FloatWidget == floatMode) ? 0 : 0, delta,
                             (DArrowRectangle::FloatWidget == floatMode) ? delta - q->margin() : delta *2, delta);
            if (rect.width() > (margins.left() + margins.right()) &&
                rect.height() > (margins.top() + margins.bottom()))
                rect = rect.marginsRemoved(margins);
        }
    }

    QPoint cornerPoint(rect.x(), rect.y() + (m_arrowY > 0 ? m_arrowY : (rect.height() / 2)));
    QPoint topLeft(rect.x() + m_arrowHeight, rect.y());
    QPoint topRight(rect.x() + rect.width(), rect.y());
    QPoint bottomRight(rect.x() + rect.width(), rect.y() + rect.height());
    QPoint bottomLeft(rect.x() + m_arrowHeight, rect.y() + rect.height());
    int radius = m_radius > (rect.height() / 2) ? (rect.height() / 2) : m_radius;
    int arrowWidth = m_arrowWidth - m_arrowHeight;
    int widgetRadius = rect.height() > 4 * radius ? 2 * radius : radius;
    if (!radiusEnabled()) {
        radius = 0;
    }

    QPainterPath border;
    border.moveTo(topLeft.x() + arrowWidth, topLeft.y());
    border.lineTo(topRight.x() - radius, topRight.y());
    border.arcTo(topRight.x() - 2 * radius, topRight.y(), 2 * radius, 2 * radius, 90, -90);
    border.lineTo(bottomRight.x(), bottomRight.y() - radius);
    border.arcTo(bottomRight.x() - 2 * radius, bottomRight.y() - 2 * radius, 2 * radius, 2 * radius, 0, -90);

    if (radiusArrowStyleEnable) {
        border.lineTo(bottomLeft.x() + arrowWidth, bottomLeft.y());
        border.arcTo(bottomLeft.x(), bottomLeft.y() - widgetRadius, widgetRadius, widgetRadius, -90, -90);
        border.lineTo(bottomLeft.x(), cornerPoint.y() + m_arrowWidth / 2 + radius / 2);

        border.cubicTo(QPointF(bottomLeft.x(), cornerPoint.y() + m_arrowWidth / 2 + radius / 2),
                       QPointF(bottomLeft.x(), cornerPoint.y() + m_arrowWidth / 2),
                       QPointF(cornerPoint.x() + m_arrowHeight / 2, cornerPoint.y() + m_arrowWidth / 4.5));

        border.cubicTo(QPointF(cornerPoint.x() + m_arrowHeight / 2, cornerPoint.y() + m_arrowWidth / 4.5),
                       QPointF(cornerPoint),
                       QPointF(cornerPoint.x() + m_arrowHeight / 2, cornerPoint.y() - m_arrowWidth / 4.5));

        border.cubicTo(QPointF(cornerPoint.x() + m_arrowHeight / 2, cornerPoint.y() - m_arrowWidth / 4.5),
                       QPointF(cornerPoint.x() + m_arrowHeight, cornerPoint.y() - m_arrowWidth / 2),
                       QPointF(topLeft.x(), cornerPoint.y() - m_arrowWidth / 2 - radius / 2));

        border.lineTo(topLeft.x(), cornerPoint.y() - m_arrowWidth / 2 - radius / 2);
        border.arcTo(topLeft.x(), topLeft.y(), widgetRadius, widgetRadius, -180, -90);
    } else {
        if (leftRightRadius) {
            border.lineTo(bottomLeft.x() + arrowWidth, bottomLeft.y());
            border.arcTo(bottomLeft.x(), bottomLeft.y() - widgetRadius, widgetRadius, widgetRadius, -90, -90);
            border.lineTo(bottomLeft.x(), cornerPoint.y() + m_arrowWidth / 2);
        } else {
            border.lineTo(bottomLeft.x(), bottomLeft.y());

            if (cornerPoint.y() > m_arrowWidth)
                border.lineTo(bottomLeft.x(), cornerPoint.y() + m_arrowWidth / 2);
        }

        border.lineTo(cornerPoint);

        if (leftRightRadius) {
            border.lineTo(topLeft.x(), cornerPoint.y() - m_arrowWidth / 2);
            border.lineTo(topLeft.x(), topLeft.y() + radius);
            border.arcTo(topLeft.x(), topLeft.y(), widgetRadius, widgetRadius, -180, -90);
        } else {
            if (cornerPoint.y() > m_arrowWidth)
                border.lineTo(topLeft.x(), cornerPoint.y() - m_arrowWidth / 2);

            border.lineTo(topLeft.x(), topLeft.y());
        }
    }

    return border;
}

QPainterPath DArrowRectanglePrivate::getRightCornerPath()
{
    D_Q(DArrowRectangle);

    QRect rect = q->rect();

    if (!m_handle) {
        qreal delta = q->shadowBlurRadius() + m_shadowDistance;

        if (DApplication::isDXcbPlatform())
            rect = rect.marginsRemoved(QMargins((DArrowRectangle::FloatWidget == floatMode) ? delta - q->margin() : delta, delta, (DArrowRectangle::FloatWidget == floatMode) ? 0 : delta, delta));
        else {
            QMargins margins((DArrowRectangle::FloatWidget == floatMode) ? delta -q->margin() : delta *2, delta, (DArrowRectangle::FloatWidget == floatMode) ? 0 : 0, delta);
            if (rect.width() > (margins.left() + margins.right()) &&
                rect.height() > (margins.top() + margins.bottom()))
                rect = rect.marginsRemoved(margins);
        }
    }

    QPoint cornerPoint(rect.x() + rect.width(), rect.y() + (m_arrowY > 0 ? m_arrowY : rect.height() / 2));
    QPoint topLeft(rect.x(), rect.y());
    QPoint topRight(rect.x() + rect.width() - m_arrowHeight, rect.y());
    QPoint bottomRight(rect.x() + rect.width() - m_arrowHeight, rect.y() + rect.height());
    QPoint bottomLeft(rect.x(), rect.y() + rect.height());
    int radius = this->m_radius > (rect.height() / 2) ? rect.height() / 2 : this->m_radius;
    int widgetRadius = rect.height() >= 4 * radius ? 2 * radius : radius;

    if (!radiusEnabled()) {
        radius = 0;
    }

    QPainterPath border;
    border.moveTo(topLeft.x(), topLeft.y());

    if (radiusArrowStyleEnable) {
        border.lineTo(topRight.x() - radius, topRight.y());
        border.arcTo(topRight.x() - widgetRadius, topRight.y(), widgetRadius, widgetRadius, 90, -90);
        border.lineTo(topRight.x(), cornerPoint.y() - m_arrowWidth / 2 - radius / 2);

        border.cubicTo(QPointF(topRight.x(), cornerPoint.y() - m_arrowWidth / 2 - radius / 2),
                       QPointF(topRight.x(), cornerPoint.y() - m_arrowWidth / 2),
                       QPointF(cornerPoint.x() - m_arrowHeight / 2, cornerPoint.y() - m_arrowWidth / 4.5));

        border.cubicTo(QPointF(cornerPoint.x() - m_arrowHeight / 2, cornerPoint.y() - m_arrowWidth / 4.5),
                       QPointF(cornerPoint),
                       QPointF(cornerPoint.x() - m_arrowHeight / 2, cornerPoint.y() + m_arrowWidth / 4.5));

        border.cubicTo(QPointF(cornerPoint.x() - m_arrowHeight / 2, cornerPoint.y() + m_arrowWidth / 4.5),
                       QPointF(bottomRight.x(), cornerPoint.y() + m_arrowWidth / 2),
                       QPointF(bottomRight.x(), cornerPoint.y() + m_arrowWidth / 2 + radius / 2));

        border.lineTo(bottomRight.x(), cornerPoint.y() + m_arrowWidth / 2 + radius / 2);
        border.arcTo(bottomRight.x() - widgetRadius, bottomRight.y() - widgetRadius, widgetRadius, widgetRadius, 0, -90);
    } else {
        if (leftRightRadius) {
            border.lineTo(topRight.x() - radius, topRight.y());
            border.arcTo(topRight.x() - widgetRadius, topRight.y(), widgetRadius, widgetRadius, 90, -90);
            border.lineTo(topRight.x(), cornerPoint.y() - m_arrowWidth / 2);
        } else {
            border.lineTo(topRight.x(), topRight.y());

            if (cornerPoint.y() > m_arrowWidth)
                border.lineTo(topRight.x(), cornerPoint.y() - m_arrowWidth / 2);
        }

        border.lineTo(cornerPoint);

        if (leftRightRadius) {
            border.lineTo(bottomRight.x(), cornerPoint.y() + m_arrowWidth / 2);
            border.lineTo(bottomRight.x(), bottomRight.y() - radius);
            border.arcTo(bottomRight.x() - widgetRadius, bottomRight.y() - widgetRadius, widgetRadius, widgetRadius, 0, -90);

        } else if (!leftRightRadius){
            if (cornerPoint.y() > m_arrowWidth)
                border.lineTo(bottomRight.x(), cornerPoint.y() + m_arrowWidth / 2);

            border.lineTo(bottomRight.x(), bottomRight.y());
        }
    }

    border.lineTo(bottomLeft.x() + radius, bottomLeft.y());
    border.arcTo(bottomLeft.x(), bottomLeft.y() - 2 * radius, 2 * radius, 2 * radius, -90, -90);
    border.lineTo(topLeft.x(), topLeft.y() + radius);
    border.arcTo(topLeft.x(), topLeft.y(), 2 * radius, 2 * radius, 180, -90);

    return border;
}

QPainterPath DArrowRectanglePrivate::getTopCornerPath()
{
    D_Q(DArrowRectangle);

    QRect rect = q->rect();

    if (!m_handle) {
        qreal delta = q->shadowBlurRadius() + m_shadowDistance;
        if (DApplication::isDXcbPlatform())
            rect = rect.marginsRemoved(QMargins(delta, (DArrowRectangle::FloatWidget == floatMode) ? 0 : delta,
                                                delta, (DArrowRectangle::FloatWidget == floatMode) ? delta -q->margin() : delta));
        else {
            QMargins margins(delta, 0, delta, (DArrowRectangle::FloatWidget == floatMode) ? delta - q->margin() : delta * 2);
            if (rect.width() > (margins.left() + margins.right()) &&
                rect.height() > (margins.top() + margins.bottom()))
                rect = rect.marginsRemoved(margins);
        }
    }

    QPoint cornerPoint(rect.x() + (m_arrowX > 0 ? m_arrowX : rect.width() / 2), rect.y());
    QPoint topLeft(rect.x(), rect.y() + m_arrowHeight);
    QPoint topRight(rect.x() + rect.width(), rect.y() + m_arrowHeight);
    QPoint bottomRight(rect.x() + rect.width(), rect.y() + rect.height());
    QPoint bottomLeft(rect.x(), rect.y() + rect.height());
    int radius = this->m_radius > (rect.height() / 2 - m_arrowHeight) ? rect.height() / 2 - m_arrowHeight : this->m_radius;
    if (!radiusEnabled()) {
        radius = 0;
    }

    QPainterPath border;
    border.moveTo(topLeft.x() + radius, topLeft.y());

    if (radiusArrowStyleEnable) {
        border.lineTo(cornerPoint.x() - m_arrowWidth / 2 - radius / 2, cornerPoint.y() + m_arrowHeight);
        border.cubicTo(QPointF(cornerPoint.x() - m_arrowWidth / 2 - radius / 2, topLeft.y()),
                       QPointF(cornerPoint.x() - m_arrowWidth / 2, topLeft.y()),
                       QPointF(cornerPoint.x() - m_arrowWidth / 4.5, cornerPoint.y() + m_arrowHeight / 2));

        border.cubicTo(QPointF(cornerPoint.x() - m_arrowWidth / 4.5, cornerPoint.y() + m_arrowHeight / 2),
                       QPointF(cornerPoint),
                       QPointF(cornerPoint.x() + m_arrowWidth / 4.5, cornerPoint.y() + m_arrowHeight / 2));

        border.cubicTo(QPointF(cornerPoint.x() + m_arrowWidth / 4.5, cornerPoint.y() + m_arrowHeight / 2),
                       QPointF(cornerPoint.x() + m_arrowWidth / 2, topLeft.y()),
                       QPointF(cornerPoint.x() + m_arrowWidth / 2 + radius / 2, cornerPoint.y() + m_arrowHeight));
        border.lineTo(cornerPoint.x() + m_arrowWidth / 2 + radius / 2, cornerPoint.y() + m_arrowHeight);
    } else {
        border.lineTo(cornerPoint.x() - m_arrowWidth / 2, cornerPoint.y() + m_arrowHeight);
        border.lineTo(cornerPoint);
        border.lineTo(cornerPoint.x() + m_arrowWidth / 2, cornerPoint.y() + m_arrowHeight);
    }

    border.lineTo(topRight.x() - radius, topRight.y());
    border.arcTo(topRight.x() - 2 * radius, topRight.y(), 2 * radius, 2 * radius, 90, -90);
    border.lineTo(bottomRight.x(), bottomRight.y() - radius);
    border.arcTo(bottomRight.x() - 2 * radius, bottomRight.y() - 2 * radius, 2 * radius, 2 * radius, 0, -90);
    border.lineTo(bottomLeft.x() + radius, bottomLeft.y());
    border.arcTo(bottomLeft.x(), bottomLeft.y() - 2 * radius, 2 * radius, 2 * radius, - 90, -90);
    border.lineTo(topLeft.x(), topLeft.y() + radius);
    border.arcTo(topLeft.x(), topLeft.y(), 2 * radius, 2 * radius, 180, -90);

    return border;
}

QPainterPath DArrowRectanglePrivate::getBottomCornerPath()
{
    D_QC(DArrowRectangle);

    QRect rect = q->rect();

    if (!m_handle) {
        qreal delta = q->shadowBlurRadius() + m_shadowDistance;

        if (DApplication::isDXcbPlatform())
            rect = rect.marginsRemoved(QMargins(delta, (DArrowRectangle::FloatWidget == floatMode) ? delta -q->margin() : delta, delta, (DArrowRectangle::FloatWidget == floatMode) ? 0 : delta));
        else {
            QMargins margins(delta, (DArrowRectangle::FloatWidget == floatMode) ? delta - q->margin() : delta *2, delta, 0);
            if (rect.width() > (margins.left() + margins.right()) &&
                rect.height() > (margins.top() + margins.bottom()))
                rect = rect.marginsRemoved(margins);
        }
    }

    QPoint cornerPoint(rect.x() + (m_arrowX > 0 ? m_arrowX : qRound(double(rect.width()) / 2)), rect.y() + rect.height());
    QPoint topLeft(rect.x(), rect.y());
    QPoint topRight(rect.x() + rect.width(), rect.y());
    QPoint bottomRight(rect.x() + rect.width(), rect.y() + rect.height() - m_arrowHeight);
    QPoint bottomLeft(rect.x(), rect.y() + rect.height() - m_arrowHeight);
    int radius = this->m_radius > (rect.height() / 2 - m_arrowHeight) ? rect.height() / 2 - m_arrowHeight : this->m_radius;
    if (!radiusEnabled()) {
        radius = 0;
    }

    QPainterPath border;
    border.moveTo(topLeft.x() + radius, topLeft.y());
    border.lineTo(topRight.x() - radius, topRight.y());
    border.arcTo(topRight.x() - 2 * radius, topRight.y(), 2 * radius, 2 * radius, 90, -90);
    border.lineTo(bottomRight.x(), bottomRight.y() - radius);
    border.arcTo(bottomRight.x() - 2 * radius, bottomRight.y() - 2 * radius, 2 * radius, 2 * radius, 0, -90);

    if (radiusArrowStyleEnable) {
        border.lineTo(cornerPoint.x() + m_arrowWidth / 2 + radius / 2, cornerPoint.y() - m_arrowHeight);

        border.cubicTo(QPointF(cornerPoint.x() + m_arrowWidth / 2 + radius / 2, cornerPoint.y() - m_arrowHeight),
                       QPointF(cornerPoint.x() + m_arrowWidth / 2, bottomRight.y()),
                       QPointF(cornerPoint.x() + m_arrowWidth / 4.5, cornerPoint.y() - m_arrowHeight / 2));

        border.cubicTo(QPointF(cornerPoint.x() + m_arrowWidth / 4.5 , cornerPoint.y() - m_arrowHeight / 2),
                       QPointF(cornerPoint),
                       QPointF(cornerPoint.x() - m_arrowWidth / 4.5 , cornerPoint.y() - m_arrowHeight / 2));

        border.cubicTo(QPointF(cornerPoint.x() - m_arrowWidth / 4.5 , cornerPoint.y() - m_arrowHeight / 2),
                       QPointF(cornerPoint.x() - m_arrowWidth / 2, bottomLeft.y()),
                       QPointF(cornerPoint.x() - m_arrowWidth / 2 - radius / 2, cornerPoint.y() - m_arrowHeight));

        border.lineTo(cornerPoint.x() - m_arrowWidth / 2 - radius / 2, cornerPoint.y() - m_arrowHeight);
    } else {
        border.lineTo(cornerPoint.x() + m_arrowWidth / 2, cornerPoint.y() - m_arrowHeight);
        border.lineTo(cornerPoint);
        border.lineTo(cornerPoint.x() - m_arrowWidth / 2, cornerPoint.y() - m_arrowHeight);
    }

    border.lineTo(bottomLeft.x() + radius, bottomLeft.y());
    border.arcTo(bottomLeft.x(), bottomLeft.y() - 2 * radius, 2 * radius, 2 * radius, -90, -90);
    border.lineTo(topLeft.x(), topLeft.y() + radius);
    border.arcTo(topLeft.x(), topLeft.y(), 2 * radius, 2 * radius, 180, -90);

    return border;
}

void DArrowRectanglePrivate::verticalMove(int x, int y)
{
    D_Q(DArrowRectangle);

    const QRect dRect = currentScreenRect(x, y);
    qreal delta = m_handle ? 0 : (q->shadowBlurRadius() - m_shadowDistance);

    int lRelativeY = y - dRect.y() - (q->height() - delta) / 2;
    int rRelativeY = y - dRect.y() + (q->height() - delta) / 2 - dRect.height();
    int absoluteY = 0;

    if (lRelativeY < 0) { //out of screen in top side
        //arrowY use relative coordinates
        q->setArrowY(q->height() / 2 - delta + lRelativeY);
        absoluteY = dRect.y() - delta;
    } else if (rRelativeY > 0) { //out of screen in bottom side
        q->setArrowY(q->height() / 2 - delta / 2 + rRelativeY);
        absoluteY = dRect.y() + dRect.height() - q->height() + delta;
    } else {
        q->setArrowY(0);
        absoluteY = y - q->height() / 2;
    }

    switch (m_arrowDirection) {
    case DArrowRectangle::ArrowLeft:
        q->QWidget::move(x, absoluteY);
        break;
    case DArrowRectangle::ArrowRight:
        q->QWidget::move(x - q->width(), absoluteY);
        break;
    default:
        break;
    }
}

void DArrowRectanglePrivate::horizontalMove(int x, int y)
{
    D_Q(DArrowRectangle);

    const QRect dRect = currentScreenRect(x, y);
    qreal delta = m_handle ? 0 : (q->shadowBlurRadius() - m_shadowDistance);

    int lRelativeX = x - dRect.x() - (q->width() - delta) / 2;
    int rRelativeX = x - dRect.x() + (q->width() - delta) / 2 - dRect.width();
    int absoluteX = 0;

    if (lRelativeX < 0) { //out of screen in left side
        //arrowX use relative coordinates
        q->setArrowX((q->width() - delta) / 2 + lRelativeX);
        absoluteX = dRect.x() - delta;
    } else if (rRelativeX > 0) { //out of screen in right side
        q->setArrowX(q->width() / 2 - delta * 2 + rRelativeX);
        absoluteX = dRect.x() + dRect.width() - q->width() + delta;
    } else {
        q->setArrowX(0);
        absoluteX = x - (m_arrowX > 0 ? m_arrowX : (q->width() / 2));
    }

    switch (m_arrowDirection) {
    case DArrowRectangle::ArrowTop:
        q->QWidget::move(absoluteX, y);
        break;
    case DArrowRectangle::ArrowBottom:
        q->QWidget::move(absoluteX, y - q->height());
        break;
    default:
        break;
    }
}

void DArrowRectanglePrivate::updateClipPath()
{
    D_Q(DArrowRectangle);

    QPainterPath path;

    switch (m_arrowDirection) {
    case DArrowRectangle::ArrowLeft:
        path = getLeftCornerPath();
        break;
    case DArrowRectangle::ArrowRight:
        path = getRightCornerPath();
        break;
    case DArrowRectangle::ArrowTop:
        path = getTopCornerPath();
        break;
    case DArrowRectangle::ArrowBottom:
        path = getBottomCornerPath();
        break;
    default:
        path = getRightCornerPath();
    }

    if (m_handle) {
        m_handle->setClipPath(path);
    } else {
        // clipPath without handle
        QPainterPathStroker stroker;
        stroker.setCapStyle(Qt::RoundCap);
        stroker.setJoinStyle(Qt::RoundJoin);
        stroker.setWidth(2);
        QPainterPath outPath = stroker.createStroke(path);
        QPolygon polygon = outPath.united(path).toFillPolygon().toPolygon();

        q->clearMask();
        q->setMask(polygon);
    }
}

bool DArrowRectanglePrivate::radiusEnabled()
{
    D_Q(DArrowRectangle);
    if (q->property("_d_radius_force").toBool())
        return true;

    if (m_wmHelper && !m_wmHelper->hasComposite()) {
        return false;
    }

    return true;
}

DArrowRectangle::~DArrowRectangle()
{

}

Dtk::Widget::DArrowRectanglePrivate::DArrowRectanglePrivate(DArrowRectangle::ArrowDirection direction, DArrowRectangle *q)
    : DObjectPrivate(q)
    , m_arrowDirection(direction)
{
    if (direction == DArrowRectangle::ArrowLeft || direction == DArrowRectangle::ArrowRight) {
        m_arrowWidth = 16;
        m_arrowHeight = 16;
    }
}

void DArrowRectanglePrivate::init(DArrowRectangle::FloatMode mode)
{
    D_Q(DArrowRectangle);

    floatMode = mode;

    if (floatMode) {
        q->setWindowFlags(Qt::Widget);
        q->setAttribute(Qt::WA_TranslucentBackground);
    } else {
        q->setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
        q->setAttribute(Qt::WA_TranslucentBackground);
    }

    if (!DWindowManagerHelper::instance()->hasBlurWindow()
                    && DGuiApplicationHelper::instance()->isTabletEnvironment()) {
        q->setAttribute(Qt::WA_TranslucentBackground, false);
    }

    if (DApplication::isDXcbPlatform() && (DArrowRectangle::FloatWindow == floatMode)) {
        m_handle = new DPlatformWindowHandle(q, q);
        m_handle->setTranslucentBackground(true);

        m_blurBackground = new DBlurEffectWidget(q);
        m_blurBackground->setBlendMode(DBlurEffectWidget::BehindWindowBlend);
        m_blurBackground->setAccessibleName("DArrowRectangleBlurBackground");

        m_wmHelper = DWindowManagerHelper::instance();

        q->connect(m_wmHelper, &DWindowManagerHelper::hasCompositeChanged, q, [q, this](){
            q->update();
            this->updateClipPath();
        }, Qt::QueuedConnection);
    } else {
        m_wmHelper = nullptr;
    }
}

void DArrowRectanglePrivate::show(int x, int y)
{
    D_Q(DArrowRectangle);

    // if show with FloatMode, must has a parent
    if (DArrowRectangle::FloatWidget == floatMode && !q->parentWidget()) {
        qCritical() << q << "Must has parentWidget when show in FloatWidget mode";
        Q_ASSERT_X(q->parentWidget(), "DArrowRectanglePrivate::show", "Must has parentWidget when show in FloatWidget mode");
    }

    q->resizeWithContent();

    m_lastPos = QPoint(x, y);
    q->move(x, y);//Overload function
    if (!q->isVisible()) {
        q->QWidget::show();
        q->QWidget::activateWindow();
    }

    q->update();
    updateClipPath();
}

void DArrowRectanglePrivate::setContent(QWidget *content)
{
    D_Q(DArrowRectangle);

    if (!content) {
        return;
    }

    m_content = content;
    m_content->setParent(q);
    m_content->show();

    qreal delta = (m_handle ? 0 : q->shadowBlurRadius() + m_shadowDistance);
    qreal deltaMargin = (m_handle ? 0 : q->shadowBlurRadius() + m_shadowDistance) + q->margin();

    q->resizeWithContent();

    switch (m_arrowDirection) {
    case DArrowRectangle::ArrowLeft:
        if (DApplication::isDXcbPlatform())
            m_content->move(m_arrowHeight + ((DArrowRectangle::FloatWidget == floatMode) ? q->margin() : deltaMargin), deltaMargin);
        else
            m_content->move(m_arrowHeight + q->margin(), deltaMargin);
        break;
    case DArrowRectangle::ArrowRight:
        if (DApplication::isDXcbPlatform())
            m_content->move((DArrowRectangle::FloatWidget == floatMode) ? deltaMargin - q->margin() : deltaMargin, deltaMargin);
        else
            m_content->move((DArrowRectangle::FloatWidget == floatMode) ? delta : delta *2 + q->margin(), deltaMargin);
        break;
    case DArrowRectangle::ArrowTop:
        if (DApplication::isDXcbPlatform())
            m_content->move(deltaMargin, ((DArrowRectangle::FloatWidget == floatMode) ? q->margin() : deltaMargin) + m_arrowHeight);
        else
            m_content->move(deltaMargin, m_arrowHeight + deltaMargin - delta);
        break;
    case DArrowRectangle::ArrowBottom:
        if (DApplication::isDXcbPlatform())
            m_content->move(deltaMargin, ((DArrowRectangle::FloatWidget == floatMode) ? deltaMargin -q->margin() : deltaMargin));
        else
            m_content->move(deltaMargin, (DArrowRectangle::FloatWidget == floatMode) ? delta : deltaMargin + delta);
        break;
    }

    q->update();
}

void DArrowRectanglePrivate::resizeWithContent()
{
    D_Q(DArrowRectangle);

    if (m_content.isNull()) {
        return;
    }

    q->setFixedSize(q->getFixedSize());

#ifdef Q_OS_LINUX
    const qreal ratio = qApp->devicePixelRatio();
    if (!m_handle && !floatMode && DGUI_NAMESPACE::DGuiApplicationHelper::instance()->isXWindowPlatform()) {
        XRectangle m_contentXRect;
        m_contentXRect.x = m_content->pos().x() * ratio;
        m_contentXRect.y = m_content->pos().y() * ratio;
        m_contentXRect.width = m_content->width() * ratio;
        m_contentXRect.height = m_content->height() * ratio;
        XShapeCombineRectangles(QX11Info::display(), q->winId(), ShapeInput,
                                0,
                                0,
                                &m_contentXRect, 1, ShapeSet, YXBanded);
    }
#endif
}

void DArrowRectanglePrivate::paintEvent(QPaintEvent *e)
{
    D_Q(DArrowRectangle);
    if (m_blurBackground) {
        return;
    }

    QColor bk_color;

    if (!m_backgroundColor.isValid()) {
        bk_color = qApp->palette().background().color();
    } else {
        bk_color = m_backgroundColor;
    }

    QPainter painter(q);

    if (m_handle) {
        painter.fillRect(e->rect(), bk_color);
    } else {
        painter.setRenderHint(QPainter::Antialiasing);

        QPainterPath border;

        switch (m_arrowDirection) {
        case DArrowRectangle::ArrowLeft:
            border = getLeftCornerPath();
            break;
        case DArrowRectangle::ArrowRight:
            border = getRightCornerPath();
            break;
        case DArrowRectangle::ArrowTop:
            border = getTopCornerPath();
            break;
        case DArrowRectangle::ArrowBottom:
            border = getBottomCornerPath();
            break;
        default:
            border = getRightCornerPath();
        }

        painter.setClipPath(border);
        painter.fillPath(border, QBrush(bk_color));

        QPen strokePen;
        strokePen.setColor(m_borderColor);
        strokePen.setWidth(m_borderWidth);
        painter.strokePath(border, strokePen);
    }
}

void DArrowRectanglePrivate::resizeEvent(QResizeEvent *e)
{
    D_Q(DArrowRectangle);

    q->QWidget::resizeEvent(e);

    if (m_blurBackground) {
        m_blurBackground->resize(e->size());
    }

    updateClipPath();
}

DWIDGET_END_NAMESPACE
