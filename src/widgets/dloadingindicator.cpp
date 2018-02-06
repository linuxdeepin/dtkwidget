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
 * \class DLoadingIndicator
 * \brief The DLoadingIndicator class provides a widget that showing loading animation.
 *
 * DLoadingIndicator renders and rotates the source set by
 * DLoadingIndicator::setWidgetSource or DLoadingIndicator::setImageSource,
 * the rotation animation is runing in a given duration and at a given easing curve,
 * those two properties can be accessed by DLoadingIndicator::aniDuration and
 * DLoadingIndicator::aniEasingType .
 */

/*!
 * \brief DLoadingIndicator::DLoadingIndicator constructs an instance of DLoadingIndicator.
 * \param parent is passed to QGraphicsView constructor.
 */
DLoadingIndicator::DLoadingIndicator(QWidget *parent) :
    QGraphicsView(parent),
    DObject(*new DLoadingIndicatorPrivate(this))
{
    DThemeManager::registerWidget(this);

    d_func()->init();
}

DLoadingIndicator::~DLoadingIndicator()
{
    D_DC(DLoadingIndicator);

    if(d->widgetSource)
        d->widgetSource->deleteLater();
}

/*!
 * \property DLoadingIndicator::backgroundColor
 * \brief This property holds the background color of this widget.
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
 * \brief DLoadingIndicator::setAniEasingCurve sets an easing curve on the loading animation.
 * \param easing
 *
 * This property defines the easing curve of the animation.
 * By default, a linear easing curve is used, resulting in linear interpolation.
 * Other curves are provided, for instance, QEasingCurve::InCirc,
 * which provides a circular entry curve.
 * Another example is QEasingCurve::InOutElastic,
 * which provides an elastic effect on the values of the interpolated variant.
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
 * \property DLoadingIndicator::loading
 * \brief This property holds the animation state.
 *
 * It returns true if the animation is running, false otherwise, you can also
 * set value on this property to run or stop the animation.
 */
bool DLoadingIndicator::loading() const
{
    D_DC(DLoadingIndicator);

    return d->rotateAni.state() == QVariantAnimation::Running;
}

/*!
 * \property DLoadingIndicator::widgetSource
 * \brief This property holds the widget to be rendered as the content of the
 * loading indicator.
 */
QWidget *DLoadingIndicator::widgetSource() const
{
    D_DC(DLoadingIndicator);

    return d->widgetSource;
}

/*!
 * \property DLoadingIndicator::imageSource
 * \brief This property holds the image to be rendered as the content of the
 * loading indicator.
 */
QPixmap DLoadingIndicator::imageSource() const
{
    QGraphicsPixmapItem * item = nullptr;

    if(!scene()->items().isEmpty())
        item = dynamic_cast<QGraphicsPixmapItem*>(scene()->items().first());

    return item ? item->pixmap() : QPixmap();
}

/*!
 * \property DLoadingIndicator::aniDuration
 * \brief This property holds the duration of the loading animation.
 */
int DLoadingIndicator::aniDuration() const
{
    D_DC(DLoadingIndicator);

    return d->rotateAni.duration();
}

/*!
 * \property DLoadingIndicator::aniEasingType
 * \brief This property holds the easing type of the easing curve used by the
 * loading animation.
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
 * \property DLoadingIndicator::smooth
 * \brief This property holds whether the rendered content are antialiased or
 * smoothly filtered.
 *
 * Smooth filtering gives better visual quality, but it may be slower on
 * some hardware.
 */
bool DLoadingIndicator::smooth() const
{
    D_DC(DLoadingIndicator);

    return d->smooth;
}

/*!
 * \property DLoadingIndicator::direction
 * \brief This property holds the direction used while doing the rotation animation.
 */
DLoadingIndicator::RotationDirection DLoadingIndicator::direction() const
{
    D_DC(DLoadingIndicator);

    return d->direction;
}

/*!
 * \property DLoadingIndicator::rotate
 * \brief This property holds the current rotation of the content.
 *
 * This property is usually used to correct the rotation of the content after
 * calling DLoadingIndicator::stop to stop the animation.
 */
qreal DLoadingIndicator::rotate() const
{
    if(!scene()->items().isEmpty())
        return scene()->items().first()->rotation();

    return 0;
}

/*!
 * \brief DLoadingIndicator::start starts the loading animation.
 */
void DLoadingIndicator::start()
{
    D_D(DLoadingIndicator);

    d->rotateAni.start();
}

/*!
 * \brief DLoadingIndicator::stop stops the loading animation.
 */
void DLoadingIndicator::stop()
{
    D_D(DLoadingIndicator);

    d->rotateAni.stop();
}


DWIDGET_END_NAMESPACE
