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

#include <QGraphicsProxyWidget>
#include <QLabel>

#include "dloadingindicator.h"
#include "private/dloadingindicator_p.h"
#include "dthememanager.h"

DWIDGET_BEGIN_NAMESPACE

DLoadingIndicatorPrivate::DLoadingIndicatorPrivate(DLoadingIndicator *qq) :
    DObjectPrivate(qq)
{

}

void DLoadingIndicatorPrivate::init()
{
    D_Q(DLoadingIndicator);

    q->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    q->setScene(new QGraphicsScene(q));
    q->setRenderHint(QPainter::SmoothPixmapTransform);
    q->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    q->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    q->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    q->viewport()->setAccessibleName("DLoadingIndicatorViewport");

    rotateAni.setDuration(1000);
    rotateAni.setEasingCurve(QEasingCurve::OutInQuad);

    rotateAni.setLoopCount(-1);
    rotateAni.setStartValue(QVariant(qreal(0.0)));
    rotateAni.setEndValue(QVariant(qreal(360.0)));

    q->connect(&rotateAni, SIGNAL(valueChanged(QVariant)), q, SLOT(setRotate(QVariant)));
}

void DLoadingIndicatorPrivate::setLoadingItem(QGraphicsItem *item)
{
    D_QC(DLoadingIndicator);

    QSizeF itemSize = item->boundingRect().size();

    item->setPos((q->width()-itemSize.width())/2,
                 (q->height()-itemSize.height())/2);
    item->setTransformOriginPoint(itemSize.width()/2, itemSize.height()/2);

    q->scene()->clear();
    q->scene()->addItem(item);
}

/*!
  \class Dtk::Widget::DLoadingIndicator
  \inmodule dtkwidget
  \brief The DLoadingIndicator class provides a widget that showing loading animation.
  \~english
  DLoadingIndicator renders and rotates the source set by
  DLoadingIndicator::setWidgetSource or DLoadingIndicator::setImageSource,
  the rotation animation is runing in a given duration and at a given easing curve,
  those two properties can be accessed by DLoadingIndicator::aniDuration and
  DLoadingIndicator::aniEasingType .
  
  
  \class Dtk::Widget::DLoadingIndicator
  \inmodule dtkwidget
  \brief DLoadingIndicator 类提供了一种用来显示“加载中”动画的控件。
  
  使用者通过设置 DLoadingIndicator::imageSource 属性或者
  DLoadingIndicator::widgetSource 属性来提供进行旋转动画的图像源，
  通过 start 和 stop 来开始和停止动画。动画开始后图像源被不停进行旋转，
  旋转的动画的周期和曲线可以通过
  DLoadingIndicator::aniDuration 和 DLoadingIndicator::aniEasingType
  两个属性进行分别控制。
  
  \note DLoadingIndicator 基于 Qt Graphics View Framework 工作。
 */

/*!
  \enum Dtk::Widget::DLoadingIndicator::RotationDirection
  \brief The RotationDirection enum contains the possible rotation
  directions of the DLoadingIndicator widget.
  \brief 枚举类型包含“加载中”动画旋转的可选方向.

  \value Clockwise
  the animation will rotate clockwise.
  顺时针旋转
  \value Counterclockwise
  the animation will rotate counterclockwise.
  逆时针旋转
 */

/*!
  \brief DLoadingIndicator::DLoadingIndicator constructs an instance of DLoadingIndicator.
  \a parent is passed to QGraphicsView constructor.
  
  
  \brief DLoadingIndicator::DLoadingIndicator 函数是 DLoadingIndicator
  的构造函数。
  \a parent 指定了控件的父控件。
 */
DLoadingIndicator::DLoadingIndicator(QWidget *parent) :
    QGraphicsView(parent),
    DObject(*new DLoadingIndicatorPrivate(this))
{
    d_func()->init();
}

DLoadingIndicator::~DLoadingIndicator()
{
    D_DC(DLoadingIndicator);

    if(d->widgetSource)
        d->widgetSource->deleteLater();
}

/*!
  \property DLoadingIndicator::backgroundColor
  \brief This property holds the background color of this widget.
  
  
  \property DLoadingIndicator::backgroundColor
  \brief DLoadingIndicator::backgroundColor 属性用来控制 DLoadingIndicator
  控件的背景色。
 */
QColor DLoadingIndicator::backgroundColor() const
{
    return scene()->backgroundBrush().color();
}

void DLoadingIndicator::setRotate(QVariant angle)
{
    if(!scene()->items().isEmpty())
        scene()->items().first()->setRotation(angle.toReal());
}

void DLoadingIndicator::setWidgetSource(QWidget *widgetSource)
{
    D_D(DLoadingIndicator);

    if(d->widgetSource)
        d->widgetSource->deleteLater();

    d->widgetSource = widgetSource;

    QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget;

    proxy->setWidget(widgetSource);

    d->setLoadingItem(proxy);
}

void DLoadingIndicator::setImageSource(const QPixmap &imageSource)
{
    D_D(DLoadingIndicator);

    QGraphicsPixmapItem * item = new QGraphicsPixmapItem(imageSource);

    if(d->smooth)
        item->setTransformationMode(Qt::SmoothTransformation);

    d->setLoadingItem(item);
}

void DLoadingIndicator::setAniEasingType(QEasingCurve::Type aniEasingType)
{
    setAniEasingCurve(aniEasingType);
}

void DLoadingIndicator::setSmooth(bool smooth)
{
    D_D(DLoadingIndicator);

    if(d->smooth == smooth)
        return;

    d->smooth = smooth;

    QGraphicsPixmapItem * item = nullptr;

    if(!scene()->items().isEmpty())
        item = dynamic_cast<QGraphicsPixmapItem*>(scene()->items().first());

    if(smooth){
        setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
        if(item)
            item->setTransformationMode(Qt::SmoothTransformation);
    }else{
        setRenderHint(QPainter::SmoothPixmapTransform, false);
        setRenderHint(QPainter::Antialiasing, false);
        if(item)
            item->setTransformationMode(Qt::FastTransformation);
    }
}

void DLoadingIndicator::setDirection(DLoadingIndicator::RotationDirection direction)
{
    D_D(DLoadingIndicator);

    if (d->direction == direction)
        return;

    d->direction = direction;

    if(direction == Clockwise) {
        d->rotateAni.setStartValue(QVariant(qreal(0.0)));
        d->rotateAni.setEndValue(QVariant(qreal(360.0)));
    } else {
        d->rotateAni.setStartValue(QVariant(qreal(0.0)));
        d->rotateAni.setEndValue(QVariant(qreal(-360.0)));
    }

    Q_EMIT directionChanged(direction);
}

void DLoadingIndicator::resizeEvent(QResizeEvent *e)
{
    QGraphicsView::resizeEvent(e);

    setSceneRect(QRectF(rect()));

    for(QGraphicsItem *item : items()) {
        QSizeF itemSize = item->boundingRect().size();

        item->setPos((width()-itemSize.width())/2,
                                 (height()-itemSize.height())/2);
    }
}

void DLoadingIndicator::setLoading(bool flag)
{
    if (flag == true){
        start();
    } else {
        stop();
    }
}

void DLoadingIndicator::setAniDuration(int msecs)
{
    D_D(DLoadingIndicator);

    d->rotateAni.setDuration(msecs);
}

/*!
  \brief DLoadingIndicator::setAniEasingCurve sets an easing curve on the loading animation.
  \a easing
  
  This property defines the easing curve of the animation.
  By default, a linear easing curve is used, resulting in linear interpolation.
  Other curves are provided, for instance, QEasingCurve::InCirc,
  which provides a circular entry curve.
  Another example is QEasingCurve::InOutElastic,
  which provides an elastic effect on the values of the interpolated variant.
  
  
  \brief DLoadingIndicator::setAniEasingCurve 是对
  DLoadingIndicator::setAniEasingType 的补充，使用者除了使用
  QEasingCurve::Type 中提供的几种预设的动画曲线，可以自定义 QEasingCurve 来
  实现更为灵巧的动画。
 */
void DLoadingIndicator::setAniEasingCurve(const QEasingCurve & easing)
{
    D_D(DLoadingIndicator);

    d->rotateAni.setEasingCurve(easing);
}

void DLoadingIndicator::setBackgroundColor(const QColor &color)
{
    scene()->setBackgroundBrush(color);
}

/*!
  \property DLoadingIndicator::loading
  \brief This property holds the animation state.
  
  It returns true if the animation is running, false otherwise, you can also
  set value on this property to run or stop the animation.
  
  
  \property DLoadingIndicator::loading
  \brief DLoadingIndicator::loading 属性用来控制动画是否开启。
  
  设置为 true 开始动画， false 停止动画。
  
  
  \sa start(), stop()
 */
bool DLoadingIndicator::loading() const
{
    D_DC(DLoadingIndicator);

    return d->rotateAni.state() == QVariantAnimation::Running;
}

/*!
  \property DLoadingIndicator::widgetSource
  \brief This property holds the widget to be rendered as the content of the
  loading indicator.
  
  
  \property DLoadingIndicator::widgetSource
  \brief DLoadingIndicator::widgetSource 属性用来生成旋转动画中使用的图像。
  
  \sa imageSource
 */
QWidget *DLoadingIndicator::widgetSource() const
{
    D_DC(DLoadingIndicator);

    return d->widgetSource;
}

/*!
  \property DLoadingIndicator::imageSource
  \brief This property holds the image to be rendered as the content of the
  loading indicator.
  
  
  \property DLoadingIndicator::imageSource
  \brief DLoadingIndicator::imageSource 属性用来生成旋转动画中使用的图像。
  
  \sa widgetSource
 */
QPixmap DLoadingIndicator::imageSource() const
{
    QGraphicsPixmapItem * item = nullptr;

    if(!scene()->items().isEmpty())
        item = dynamic_cast<QGraphicsPixmapItem*>(scene()->items().first());

    return item ? item->pixmap() : QPixmap();
}

/*!
  \property DLoadingIndicator::aniDuration
  \brief This property holds the duration of the loading animation.
  
  
  \property DLoadingIndicator::aniDuration
  \brief DLoadingIndicator::aniDuration 属性用来控制旋转动画的周期。
 */
int DLoadingIndicator::aniDuration() const
{
    D_DC(DLoadingIndicator);

    return d->rotateAni.duration();
}

/*!
  \property DLoadingIndicator::aniEasingType
  \brief This property holds the easing type of the easing curve used by the
  loading animation.
  
  
  \property DLoadingIndicator::aniEasingType
  \brief DLoadingIndicator::aniEasingType 属性用来控制旋转动画的动画曲线。
  
  \sa setAniEasingCurve
 */
QEasingCurve::Type DLoadingIndicator::aniEasingType() const
{
    D_DC(DLoadingIndicator);

    return d->rotateAni.easingCurve().type();
}

QSize DLoadingIndicator::sizeHint() const
{
    return scene()->sceneRect().size().toSize();
}

/*!
  \property DLoadingIndicator::smooth
  \brief This property holds whether the rendered content are antialiased or
  smoothly filtered.
  
  Smooth filtering gives better visual quality, but it may be slower on
  some hardware.
  
  
  \property DLoadingIndicator::smooth
  \brief DLoadingIndicator::smooth 属性代表是否要对控件启用平滑处理。
  
  平滑处理后，控件会被进行反锯齿或者进行遮罩以达到更好的显示效果，但是在性能较差
  的机器上会比较慢。
 */
bool DLoadingIndicator::smooth() const
{
    D_DC(DLoadingIndicator);

    return d->smooth;
}

/*!
  \property DLoadingIndicator::direction
  \brief This property holds the direction used while doing the rotation animation.
  
  
  \property DLoadingIndicator::direction
  \brief DLoadingIndicator::direction 用来控制旋转动画的旋转方向。
  
  默认为顺时针方向。
  
  
  \sa DLoadingIndicator::RotationDirection
 */

/*!
  DLoadingIndicator::direction
 */
DLoadingIndicator::RotationDirection DLoadingIndicator::direction() const
{
    D_DC(DLoadingIndicator);

    return d->direction;
}

/*!
  \property DLoadingIndicator::rotate
  \brief This property holds the current rotation of the content.
  
  This property is usually used to correct the rotation of the content after
  calling DLoadingIndicator::stop to stop the animation.
  
  
  \property DLoadingIndicator::rotate
  \brief DLoadingIndicator::rotate 属性用来控制当前控件内容旋转的角度。
  
  
  \sa QGraphicsItem::rotation
 */
qreal DLoadingIndicator::rotate() const
{
    if(!scene()->items().isEmpty())
        return scene()->items().first()->rotation();

    return 0;
}

/*!
  \brief DLoadingIndicator::start starts the loading animation.
  
  
  \brief DLoadingIndicator::start 启动旋转动画。
 */
void DLoadingIndicator::start()
{
    D_D(DLoadingIndicator);

    d->rotateAni.start();
}

/*!
  \brief DLoadingIndicator::stop stops the loading animation.
  
  
  \brief DLoadingIndicator::stop 停止旋转动画。
 */
void DLoadingIndicator::stop()
{
    D_D(DLoadingIndicator);

    d->rotateAni.stop();
}


DWIDGET_END_NAMESPACE
