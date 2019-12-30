/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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
 * \~english \class DArrowRectangle
 * \~english \brief The DArrowRectangle class provides a widget that has an arrow on one
 * \~english of its four borders.
 *
 * \~english It's usually used as a container of some other widgets,
 * \~english see DArrowRectangle::setContent()
 */

/*!
 * \~chinese \class DArrowRectangle
 * \~chinese \brief DArrowRectangle 提供了可以在四个边中的任意一个边显示箭头的矩形控件
 *
 * \~chinese 通常用于作为其他控件的容器，将其显示在矩形内作为内容控件
 * \~chinese \sa DArrowRectangle::setContent()
 */





/**
 * \~english \enum DArrowRectangle::ArrowDirection
 * \~english \brief The ArrowDirection enum contains the possible directions that
 * \~english the DArrowRectangle's arrow may point to.
 * \~english \var DArrowRectangle::ArrowDirection DArrowRectangle::ArrowLeft
 * \~english indicates the arrow of this rectangle will point left
 * \~english \var DArrowRectangle::ArrowDirection DArrowRectangle::ArrowRight
 * \~english indicates the arrow of this rectangle will point right
 * \~english \var DArrowRectangle::ArrowDirection DArrowRectangle::ArrowTop
 * \~english indicates the arrow of this rectangle will point up
 * \~english \var DArrowRectangle::ArrowDirection DArrowRectangle::ArrowBottom
 * \~english indicates the arrow of this rectangle will point down
 */

/**
 * \~chinese \enum DArrowRectangle::ArrowDirection
 * \~chinese \brief 箭头方向枚举包含 DArrowRectangle 的箭头可能指向的可能方向
 * \~chinese \var DArrowRectangle::ArrowDirection DArrowRectangle::ArrowLeft
 * \~chinese 指示此矩形的箭头将指向左侧
 * \~chinese \var DArrowRectangle::ArrowDirection DArrowRectangle::ArrowRight
 * \~chinese 指示此矩形的箭头将指向右侧
 * \~chinese \var DArrowRectangle::ArrowDirection DArrowRectangle::ArrowTop
 * \~chinese 指示此矩形的箭头将指向上方
 * \~chinese \var DArrowRectangle::ArrowDirection DArrowRectangle::ArrowBottom
 * \~chinese 指示此矩形的箭头将向下指向
 */

/*!
 * \~english \enum DArrowRectangle::FloatMode
 * \~english \brief The FloatMode enum decide the WindowType when DArrowRectangle show
 * \~english \var DArrowRectangle::FloatMode DArrowRectangle::FloatWindow
 * \~english Window will show a separate window
 * \~english \var DArrowRectangle::FloatMode DArrowRectangle::FloatWidget
 * \~english Widget must by show in the rect of parentWidget
 */

/*!
 * \~chinese \enum DArrowRectangle::FloatMode
 * \~chinese \brief FloatMode 表示不同的控件的浮动模式
 *
 * \~chinese 控件的浮动模式表示控件如何显示在布局中，DArrowRectangle::FloatWindow 表示控件将会以一个单独的窗口显示，而 DArrowRectangle::FloatWidget 则表示控件只能显示在其父控件的布局中，不能超出父控件大小
 *
 * \~chinese \var DArrowRectangle::FloatMode DArrowRectangle::FloatWindow
 * \~chinese 窗口模式
 * \~chinese \var DArrowRectangle::FloatMode DArrowRectangle::FloatWidget
 * \~chinese 控件模式
 */






/*!
 * \~english \brief DArrowRectangle::DArrowRectangle constructs an instance of DArrowRectangle.
 * \~english \param direction is used to initialize the direction of which the arrow
 * \~english points to.
 * \~english \param parent is the parent widget the arrow rectangle will be attached to.
 */

/*!
 * \~chinese \brief 获取 DArrowRectangle 实例
 * \~chinese \param direction 用于初始化箭头的方向
 * \~chinese \param parent 作为其父控件
 */
DArrowRectangle::DArrowRectangle(ArrowDirection direction, QWidget *parent) :
    QWidget(parent),
    DObject(*new DArrowRectanglePrivate(direction, this))
{
    D_D(DArrowRectangle);

    d->init(FloatWindow);
}

/*!
 * \~english \brief DArrowRectangle::DArrowRectangle can set DArrowRectangle show as a window or
 * \~english a widget in parentWidget by floatMode
 * \~english \param direction
 * \~english \param floatMode
 * \~english \param parent
 */

/*!
 * \~chinese \brief 获取 DArrowRectangle 实例，并指定浮动模式
 * \~chinese \param direction 用于初始化箭头的方向
 * \~chinese \param floatMode
 * \~chinese \param parent 作为其父控件
 * \~chinese \sa DArrowRectangle::FloatMode
 */
DArrowRectangle::DArrowRectangle(ArrowDirection direction, FloatMode floatMode, QWidget *parent) :
    QWidget(parent),
    DObject(*new DArrowRectanglePrivate(direction, this))
{
    D_D(DArrowRectangle);

    d->init(floatMode);
}


/*!
 * \~english \brief DArrowRectangle::show shows the widget at the given coordinate.
 *
 * \~english \note The coordiate is calculated to be the arrow head's position, so you
 * \~english don't need to calculate the position yourself.
 *
 * \~english \param x is the x coordinate of the arrow head.
 * \~english \param y is the y coordinate of the arrow head.
 */

/*!
 * \~chinese \brief 在指定的坐标位置显示本控件
 *
 * \~chinese \note 坐标被计算为箭头的位置，所以你不需要自己计算箭头位置
 *
 * \~chinese \param x 控件箭头的x轴坐标
 * \~chinese \param y 控件箭头的y轴坐标
 */
void DArrowRectangle::show(int x, int y)
{
    D_D(DArrowRectangle);

    d->show(x, y);
}

/*!
 * \~english \brief DArrowRectangle::setContent sets the content of the arrow rectangle.
 * \~english \param content
 */

/*!
 * \~chinese \brief 设置要显示在矩形内的内容控件
 * \~chinese \param 要显示内容控件
 */
void DArrowRectangle::setContent(QWidget *content)
{
    D_D(DArrowRectangle);

    d->setContent(content);
}

/*!
 * \~english \brief 获取内容控件
 * \~english \return 正在显示的内容控件
 */
QWidget *DArrowRectangle::getContent() const
{
    D_DC(DArrowRectangle);

    return d->m_content;
}

/*!
 * \~english \brief DArrowRectangle::resizeWithContent automatically adjust the rectangle's
 * \~english size to fit the its content.
 */

/*!
 * \~chinese \brief 根据内容控件的大小自动设置矩形控件的大小
 */
void DArrowRectangle::resizeWithContent()
{
    D_D(DArrowRectangle);

    d->resizeWithContent();
}

/*!
 * \~english \brief DArrowRectangle::getFixedSize
 * \~english \return the size of the whole widget.
 */

/*!
 * \~chinese \brief 获取整个矩形控件的大小
 * \~chinese \return 矩形控件的大小
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
 * \~english \brief DArrowRectangle::move moves the widget to the coordinate that provided,
 *
 * \~english Like the rules in DArrowRectangle::show(int x, int y), it moves the widget so
 * \~english that the arrow head's coordinate matches the one that provided.
 *
 * \~english \param x is the x coordinate of the arrow head.
 * \~english \param y is the y coordinate of the arrow head.
 *
 * \~english \see DArrowRectangle::show(int x, int y)
 */

/*!
 * \~chinese \brief 移动到指定的坐标位置
 *
 * \~chinese 参数的作用类似于 DArrowRectangle::show , 移动整个控件直到箭头出现在参数中指定的坐标
 *
 * \~chinese \param x 控件箭头的x轴坐标
 * \~chinese \param y 控件箭头的y轴坐标
 *
 * \~chinese \sa DArrowRectangle::show
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
 * \~english \property DArrowRectangle::shadowYOffset
 * \~english \brief the offset of the widget and its shadow on y axis.
 */

/*!
 * \~chinese \property DArrowRectangle::shadowYOffset
 * \~chinese \brief 这属性表示小部件及其阴影在y轴上的偏移量
 *
 * \~chinese Getter: DArrowRectangle::shadowYOffset Setter: DArrowRectangle::setShadowYOffset
 * \~chinese \sa DArrowRectangle::shadowXOffset
 */
qreal DArrowRectangle::shadowYOffset() const
{
    D_DC(DArrowRectangle);

    return d->m_shadowYOffset;
}

/**
 * \~chinese \sa DArrowRectangle::shadowYOffset
 */
void DArrowRectangle::setShadowYOffset(const qreal &shadowYOffset)
{
    D_D(DArrowRectangle);

    d->m_shadowYOffset = shadowYOffset;

    if (d->m_handle) {
        d->m_handle->setShadowOffset(QPoint(d->m_shadowXOffset, shadowYOffset));
    }
}

/*!
 * \~english \property DArrowRectangle::shadowXOffset
 * \~english \brief the offset of the widget and its shadow on x axis.
 */

/*!
 * \~chinese \property DArrowRectangle::shadowXOffset
 * \~chinese \brief 这属性表示小部件及其阴影在x轴上的偏移量
 *
 * \~chinese Getter: DArrowRectangle::shadowXOffset Setter: DArrowRectangle::setShadowXOffset
 * \~chinese \sa DArrowRectangle::shadowYOffset
 */
qreal DArrowRectangle::shadowXOffset() const
{
    D_DC(DArrowRectangle);

    return d->m_shadowXOffset;
}

/**
 * \~chinese \sa DArrowRectangle::shadowXOffset
 */
void DArrowRectangle::setShadowXOffset(const qreal &shadowXOffset)
{
    D_D(DArrowRectangle);

    d->m_shadowXOffset = shadowXOffset;

    if (d->m_handle) {
        d->m_handle->setShadowOffset(QPoint(shadowXOffset, d->m_shadowYOffset));
    }
}

/*!
 * \~english \property DArrowRectangle::shadowBlurRadius
 * \~english \brief This property holds the blur radius of the widget's shadow.
 */

/*!
 * \~chinese \property DArrowRectangle::shadowBlurRadius
 * \~chinese \brief 这个属性保存小部件阴影的模糊半径
 *
 * \~chinese Getter: DArrowRectangle::shadowBlurRadius Setter: DArrowRectangle::setShadowBlurRadius
 * \~chinese \sa DArrowRectangle::shadowBlurRadius
 */
qreal DArrowRectangle::shadowBlurRadius() const
{
    D_DC(DArrowRectangle);

    return d->m_shadowBlurRadius;
}

/*!
 * \~chinese \sa DArrowRectangle::setShadowBlurRadius
 */
void DArrowRectangle::setShadowBlurRadius(const qreal &shadowBlurRadius)
{
    D_D(DArrowRectangle);

    d->m_shadowBlurRadius = shadowBlurRadius;

    if (d->m_handle) {
        d->m_handle->setShadowRadius(shadowBlurRadius);
    }
}

/*!
 * \~english \property DArrowRectangle::borderColor
 * \~english \brief This property holds the border color of this widget.
 */

/*!
 * \~chinese \property DArrowRectangle::borderColor
 * \~chinese \brief 这个属性表示控件边框的颜色
 *
 * \~chinese Getter: DArrowRectangle::borderColor , Setter: DArrowRectangle::setBorderColor
 */
QColor DArrowRectangle::borderColor() const
{
    D_DC(DArrowRectangle);

    return d->m_borderColor;
}

/*!
 * \~chinese \sa DArrowRectangle::borderColor
 */
void DArrowRectangle::setBorderColor(const QColor &borderColor)
{
    D_D(DArrowRectangle);

    d->m_borderColor = borderColor;

    if (d->m_handle) {
        d->m_handle->setBorderColor(borderColor);
    }
}

/*!
 * \~english \property DArrowRectangle::borderWidth
 * \~english \brief This property holds the border width of this widget.
 */

/*!
 * \~chinese \property DArrowRectangle::borderWidth
 * \~chinese \brief 这个属性表示控件边框的宽度
 *
 * \~chinese Getter: DArrowRectangle::borderWidth , Setter: DArrowRectangle::setBorderWidth
 */
int DArrowRectangle::borderWidth() const
{
    D_DC(DArrowRectangle);

    return d->m_borderWidth;
}

/*!
 * \~chinese \sa DArrowRectangle::borderWidth
 */
void DArrowRectangle::setBorderWidth(int borderWidth)
{
    D_D(DArrowRectangle);

    d->m_borderWidth = borderWidth;

    if (d->m_handle) {
        d->m_handle->setBorderWidth(borderWidth);
    }
}

/*!
 * \~english \property DArrowRectangle::backgroundColor
 * \~english \brief the background color of this rectangle.
 */

/*!
 * \~chinese \property DArrowRectangle::backgroundColor
 * \~chinese \brief 这个属性表示矩形控件的背景颜色
 *
 * \~chinese Getter: DArrowRectangle::backgroundColor , Setter: DArrowRectangle::setBackgroundColor
 */
QColor DArrowRectangle::backgroundColor() const
{
    D_DC(DArrowRectangle);

    return d->m_backgroundColor;
}

/*!
 * \~english \property DArrowRectangle::arrowDirection
 * \~english \brief This property holds the direction of the rectangle's arrow points to.
 */

/*!
 * \~chinese \property DArrowRectangle::arrowDirection
 * \~chinese \brief 这个属性表示箭头的方向
 *
 * \~chinese Getter: DArrowRectangle::arrowDirection , Setter: DArrowRectangle::setArrowDirection
 */
DArrowRectangle::ArrowDirection DArrowRectangle::arrowDirection() const
{
    D_DC(DArrowRectangle);

    return d->m_arrowDirection;
}

/*!
 * \~chinese \sa DArrowRectangle::backgroundColor
 */
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
 * \~english \brief DArrowRectangle::setBackgroundColor is an overloaded function.
 *
 * \~english It sets the background color by modifing the mask color of the
 * \~english Dtk::Widget::DBlurEffectWidget.
 *
 * \~english \param type is the mask color to set.
 */

/*!
 * \~chinese \brief DArrowRectangle::setBackgroundColor 是一个重载方法
 *
 * \~chinese 通过改变 DBlurEffectWidget::MaskColorType 来修改控件矩形的背景
 *
 * \~chinese \param type is the mask color to set.
 * \~chinese \sa DArrowRectangle::backgroundColor and DBlurEffectWidget::MaskColorType
 */
void DArrowRectangle::setBackgroundColor(DBlurEffectWidget::MaskColorType type)
{
    D_D(DArrowRectangle);

    if (d->m_blurBackground) {
        d->m_blurBackground->setMaskColor(type);
    }
}

/*!
 * \~english \property DArrowRectangle::radius
 * \~english \brief radius of the rectangle
 */

/*!
 * \~chinese \property DArrowRectangle::radius
 * \~chinese \brief 这个属性表示矩形的圆角
 *
 * \~chinese Getter: DArrowRectangle::radius , Setter: DArrowRectangle::setRadius
 */
int DArrowRectangle::radius() const
{
    D_DC(DArrowRectangle);

    return d->m_radius;
}

/*!
 * \~english \property DArrowRectangle::arrowHeight
 * \~english \brief height of rectangle's arrow
 */

/*!
 * \~chinese \property DArrowRectangle::arrowHeight
 * \~chinese \brief 这个属性表示箭头的高度
 *
 * \~chinese Getter: DArrowRectangle::arrowHeight , Setter: DArrowRectangle::setArrowHeight
 * \~chinese \sa DArrowRectangle::arrowWidth
 */
int DArrowRectangle::arrowHeight() const
{
    D_DC(DArrowRectangle);

    return d->m_arrowHeight;
}

/*!
 * \~english \property DArrowRectangle::arrowWidth
 * \~english \brief width of the rectangle's arrow
 */

/*!
 * \~chinese \property DArrowRectangle::arrowWidth
 * \~chinese \brief 这个属性表示箭头的宽度
 *
 * \~chinese Getter: DArrowRectangle::arrowWidth , Setter: DArrowRectangle::setArrowWidth
 * \~chinese \sa DArrowRectangle::arrowHeight
 */
int DArrowRectangle::arrowWidth() const
{
    D_DC(DArrowRectangle);

    return d->m_arrowWidth;
}

/*!
 * \~english \property DArrowRectangle::arrowX
 * \~english \brief the x coordinate of the rectangle's arrow
 */

/*!
 * \~chinese \property DArrowRectangle::arrowX
 * \~chinese \brief 这个属性表示箭头的x轴坐标
 *
 * \~chinese Getter: DArrowRectangle::arrowX , Setter: DArrowRectangle::setArrowX
 * \~chinese \sa DArrowRectangle::arrowY
 */
int DArrowRectangle::arrowX() const
{
    D_DC(DArrowRectangle);

    return d->m_arrowX;
}

/*!
 * \~english \property DArrowRectangle::arrowY
 * \~english \brief the y coordinate of the rectangle's arrow
 */

/*!
 * \~chinese \property DArrowRectangle::arrowY
 * \~chinese \brief 这个属性表示箭头的y轴坐标
 *
 * \~chinese Getter: DArrowRectangle::arrowY , Setter: DArrowRectangle::setArrowY
 * \~chinese \sa DArrowRectangle::arrowX
 */
int DArrowRectangle::arrowY() const
{
    D_DC(DArrowRectangle);

    return d->m_arrowY;
}

/*!
 * \~english \property DArrowRectangle::margin
 * \~english \brief This property holds the width of the margin
 *
 * \~english The margin is the distance between the innermost pixel of the rectangle and the
 * \~english outermost pixel of its contents.
 *
 * \~english The default margin is 0.
 */

/*!
 * \~chinese \property DArrowRectangle::margin
 * \~chinese \brief 这个属性表示边距大小
 *
 * \~chinese 边距是指矩形最里面的像素与其内容最外面的像素之间的距离
 * \~chinese Getter: DArrowRectangle::margin , Setter: DArrowRectangle::setMargin
 * \~chinese \sa DArrowRectangle::margin
 */
int DArrowRectangle::margin() const
{
    D_DC(DArrowRectangle);

    return d->m_margin;
}

/*!
 * \~chinese \sa DArrowRectangle::arrowDirection
 */
void DArrowRectangle::setArrowDirection(ArrowDirection value)
{
    D_D(DArrowRectangle);

    d->m_arrowDirection = value;
}

/**
 * \~chinese \brief 设置整个控件固定的宽度
 * \~chinese
 * \~chinese \param value 宽度大小
 */
void DArrowRectangle::setWidth(int value)
{
    setFixedWidth(value);
}

/**
 * \~chinese \brief 设置整个控件固定的高度
 * \~chinese
 * \~chinese \param value 高度大小
 */
void DArrowRectangle::setHeight(int value)
{
    setFixedHeight(value);
}

/**
 * \~chinese \sa DArrowRectangle::radius
 */
void DArrowRectangle::setRadius(int value)
{
    D_D(DArrowRectangle);

    d->m_radius = value;
}

/**
 * \~chinese \sa DArrowRectangle::arrowHeight
 */
void DArrowRectangle::setArrowHeight(int value)
{
    D_D(DArrowRectangle);

    d->m_arrowHeight = value;
}

/**
 * \~chinese \sa DArrowRectangle::arrowWidth
 */
void DArrowRectangle::setArrowWidth(int value)
{
    D_D(DArrowRectangle);

    d->m_arrowWidth = value;
}

/**
 * \~chinese \sa DArrowRectangle::arrowX
 */
void DArrowRectangle::setArrowX(int value)
{
    D_D(DArrowRectangle);

    d->m_arrowX = value;
}

/**
 * \~chinese \sa DArrowRectangle::arrowY
 */
void DArrowRectangle::setArrowY(int value)
{
    D_D(DArrowRectangle);

    d->m_arrowY = value;
}

/**
 * \~chinese \sa DArrowRectangle::margin
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

        rect = rect.marginsRemoved(QMargins((DArrowRectangle::FloatWidget == floatMode) ? 0 : delta, delta, delta, delta));
    }

    QPoint cornerPoint(rect.x(), rect.y() + (m_arrowY > 0 ? m_arrowY : (rect.height() / 2)));
    QPoint topLeft(rect.x() + m_arrowHeight, rect.y());
    QPoint topRight(rect.x() + rect.width(), rect.y());
    QPoint bottomRight(rect.x() + rect.width(), rect.y() + rect.height());
    QPoint bottomLeft(rect.x() + m_arrowHeight, rect.y() + rect.height());
    int radius =  m_radius > (rect.height() / 2) ? (rect.height() / 2) : m_radius;
    if (!radiusEnabled()) {
        radius = 0;
    }

    QPainterPath border;
    border.moveTo(topLeft.x() + radius, topLeft.y());
    border.lineTo(topRight.x() - radius, topRight.y());
    border.arcTo(topRight.x() - 2 * radius, topRight.y(), 2 * radius, 2 * radius, 90, -90);
    border.lineTo(bottomRight.x(), bottomRight.y() - radius);
    border.arcTo(bottomRight.x() - 2 * radius, bottomRight.y() - 2 * radius, 2 * radius, 2 * radius, 0, -90);
    border.lineTo(bottomLeft.x() + radius, bottomLeft.y());
    border.arcTo(bottomLeft.x(), bottomLeft.y() - 2 * radius, 2 * radius, 2 * radius, -90, -90);
    border.lineTo(cornerPoint.x() + m_arrowHeight, cornerPoint.y() + m_arrowWidth / 2);
    border.lineTo(cornerPoint);
    border.lineTo(cornerPoint.x() + m_arrowHeight, cornerPoint.y() - m_arrowWidth / 2);
    border.lineTo(topLeft.x(), topLeft.y() + radius);
    border.arcTo(topLeft.x(), topLeft.y(), 2 * radius, 2 * radius, -180, -90);

    return border;
}

QPainterPath DArrowRectanglePrivate::getRightCornerPath()
{
    D_Q(DArrowRectangle);

    QRect rect = q->rect();

    if (!m_handle) {
        qreal delta = q->shadowBlurRadius() + m_shadowDistance;

        rect = rect.marginsRemoved(QMargins(delta, delta, (DArrowRectangle::FloatWidget == floatMode) ? 0 : delta, delta));
    }

    QPoint cornerPoint(rect.x() + rect.width(), rect.y() + (m_arrowY > 0 ? m_arrowY : rect.height() / 2));
    QPoint topLeft(rect.x(), rect.y());
    QPoint topRight(rect.x() + rect.width() - m_arrowHeight, rect.y());
    QPoint bottomRight(rect.x() + rect.width() - m_arrowHeight, rect.y() + rect.height());
    QPoint bottomLeft(rect.x(), rect.y() + rect.height());
    int radius = this->m_radius > (rect.height() / 2) ? rect.height() / 2 : this->m_radius;
    if (!radiusEnabled()) {
        radius = 0;
    }

    QPainterPath border;
    border.moveTo(topLeft.x() + radius, topLeft.y());
    border.lineTo(topRight.x() - radius, topRight.y());
    border.arcTo(topRight.x() - 2 * radius, topRight.y(), 2 * radius, 2 * radius, 90, -90);
    border.lineTo(cornerPoint.x() - m_arrowHeight, cornerPoint.y() - m_arrowWidth / 2);
    border.lineTo(cornerPoint);
    border.lineTo(cornerPoint.x() - m_arrowHeight, cornerPoint.y() + m_arrowWidth / 2);
    border.lineTo(bottomRight.x(), bottomRight.y() - radius);
    border.arcTo(bottomRight.x() - 2 * radius, bottomRight.y() - 2 * radius, 2 * radius, 2 * radius, 0, -90);
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
        rect = rect.marginsRemoved(QMargins(delta, (DArrowRectangle::FloatWidget == floatMode) ? 0 : delta, delta, delta));
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
    border.lineTo(cornerPoint.x() - m_arrowWidth / 2, cornerPoint.y() + m_arrowHeight);
    border.lineTo(cornerPoint);
    border.lineTo(cornerPoint.x() + m_arrowWidth / 2, cornerPoint.y() + m_arrowHeight);
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

        rect = rect.marginsRemoved(QMargins(delta, delta, delta, (DArrowRectangle::FloatWidget == floatMode) ?  0 : delta));
    }

    QPoint cornerPoint(rect.x() + (m_arrowX > 0 ? m_arrowX : qRound(double(rect.width()) / 2)), rect.y()  + rect.height());
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
    border.lineTo(cornerPoint.x() + m_arrowWidth / 2, cornerPoint.y() - m_arrowHeight);
    border.lineTo(cornerPoint);
    border.lineTo(cornerPoint.x() - m_arrowWidth / 2, cornerPoint.y() - m_arrowHeight);
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

    if (!m_handle) {
        return;
    }

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

    m_handle->setClipPath(path);
}

bool DArrowRectanglePrivate::radiusEnabled()
{
    if (m_wmHelper && !m_wmHelper->hasComposite()) {
        return false;
    }

    return true;
}

DArrowRectangle::~DArrowRectangle()
{

}

Dtk::Widget::DArrowRectanglePrivate::DArrowRectanglePrivate(DArrowRectangle::ArrowDirection direction, DArrowRectangle *q)
    : DObjectPrivate(q),

      m_arrowDirection(direction)
{

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

    if (DApplication::isDXcbPlatform() && (DArrowRectangle::FloatWindow == floatMode)) {
        m_handle = new DPlatformWindowHandle(q);
        m_handle->setTranslucentBackground(true);

        m_blurBackground = new DBlurEffectWidget(q);
        m_blurBackground->setBlendMode(DBlurEffectWidget::BehindWindowBlend);

        m_wmHelper = DWindowManagerHelper::instance();

        q->connect(m_wmHelper, &DWindowManagerHelper::hasCompositeChanged, q, static_cast<void (DArrowRectangle::*)()>(&DArrowRectangle::update), Qt::QueuedConnection);
    } else {
        DGraphicsGlowEffect *glowEffect = new DGraphicsGlowEffect;
        glowEffect->setBlurRadius(q->shadowBlurRadius());
        glowEffect->setDistance(m_shadowDistance);
        glowEffect->setXOffset(q->shadowXOffset());
        glowEffect->setYOffset(q->shadowYOffset());
        q->setGraphicsEffect(glowEffect);

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

    qreal delta = (m_handle ? 0 : q->shadowBlurRadius() + m_shadowDistance) + q->margin();

    q->resizeWithContent();

    switch (m_arrowDirection) {
    case DArrowRectangle::ArrowLeft:
        m_content->move(m_arrowHeight + ((DArrowRectangle::FloatWidget == floatMode) ? 0 : delta), delta);
        break;
    case DArrowRectangle::ArrowRight:
        m_content->move(delta, delta);
        break;
    case DArrowRectangle::ArrowTop:
        m_content->move(delta, ((DArrowRectangle::FloatWidget == floatMode) ? 0 : delta) + m_arrowHeight);
        break;
    case DArrowRectangle::ArrowBottom:
        m_content->move(delta, delta);
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
