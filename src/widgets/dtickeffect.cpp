// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dtickeffect.h"
#include "private/dtickeffect_p.h"

#include <QHBoxLayout>
#include <QPixmap>
#include <QWidget>
#include <QPainter>
#include <QEvent>

DWIDGET_BEGIN_NAMESPACE

/*!
  \class Dtk::Widget::DTickEffect
  \inmodule dtkwidget
  \brief 这个类提供了跑马灯效果，可以让控件的内容滚动起来.
  
  \note 注意: 该类目前只支持滚动可视区域，如果需要滚动完整的界面，需要使用QScrollarea封装一下
 */

/*!
  \enum Dtk::Widget::DTickEffect::Direction
  滚动方向的枚举值.

  \value LeftToRight
  \brief 从左往右滚动
  \value RightToLeft
  \brief  从右往左滚动
  \value TopToBottom
  \brief  从上往下滚动
  \value BottomToTop
  \brief  从下往上滚动
 */

/*!
  \fn void DTickEffect::finished()
  调用stop方法以后会发出该信号
 */
/*!
  \fn void DTickEffect::stateChanged()
  当方向发生改变时会发出该信号
 */

/*!
  \brief DTickEffect的构造函数
  
  \a widget 设置动画绑定的对象
  \a parent
 */
DTickEffect::DTickEffect(QWidget *widget, QWidget *parent)
    : QGraphicsEffect(parent)
    , DObject(*new DTickEffectPrivate(this))
{
    D_D(DTickEffect);

    widget->setGraphicsEffect(this);
    widget->installEventFilter(this);

    d->content = widget;
    d->init();
    setDirection(DTickEffect::LeftToRight);

    connect(d->runAnimation, &QVariantAnimation::valueChanged, this, &DTickEffect::update);
    connect(d->runAnimation, &QVariantAnimation::finished, this, &DTickEffect::finished);
}

void DTickEffect::draw(QPainter *painter)
{
    D_D(DTickEffect);

    QPoint offset;
    QPixmap pixmap;

    if (sourceIsPixmap())
        pixmap = sourcePixmap(Qt::LogicalCoordinates, &offset, QGraphicsEffect::NoPad);
    else
        pixmap = sourcePixmap(Qt::DeviceCoordinates, &offset, QGraphicsEffect::NoPad);

    const QPoint p { d->runAnimation->currentValue().toPoint() };
    const QSize size { pixmap.size() / d->content->devicePixelRatioF() };


    switch (d->direction) {
    case LeftToRight:
        offset = QPoint(-size.width() + p.x(), p.y());
        break;
    case RightToLeft:
        offset = QPoint(size.width() + p.x(), p.y());
        break;
    case TopToBottom:
        offset = QPoint(p.x(), -size.height() + p.y());
        break;
    case BottomToTop:
        offset = QPoint(p.x(), size.height() + p.y());
        break;
    default:
        break;
    }

    painter->drawPixmap(d->runAnimation->currentValue().toPoint(), pixmap);
    painter->drawPixmap(offset, pixmap);
}

bool DTickEffect::eventFilter(QObject *watched, QEvent *event)
{
    D_D(DTickEffect);

    if (watched == d->content && event->type() == QEvent::Resize) {
        d->initDirection();
    }

    return QGraphicsEffect::eventFilter(watched, event);
}

/*!
  \brief 开始播放
  
 */
void DTickEffect::play()
{
    D_D(DTickEffect);

    d->runAnimation->start();

    Q_EMIT stateChanged();
}

/*!
  \brief 停止播放
  
 */
void DTickEffect::stop()
{
    D_D(DTickEffect);

    d->runAnimation->stop();

    Q_EMIT stateChanged();
}

/*!
  \brief 暂停播放
  
 */
void DTickEffect::pause()
{
    D_D(DTickEffect);

    d->runAnimation->pause();

    Q_EMIT stateChanged();
}

/*!
  \brief 恢复播放
  
 */
void DTickEffect::resume()
{
    D_D(DTickEffect);

    d->runAnimation->resume();

    Q_EMIT stateChanged();
}

/*!
  \brief 设置滚动的方向
  
  \a direction
 */
void DTickEffect::setDirection(DTickEffect::Direction direction)
{
    D_D(DTickEffect);

    if (d->direction == direction)
        return;

    d->direction = direction;

    d->initDirection();

    stop();
    play();
}

void DTickEffect::setFixedPixelMove(const int pixel)
{
    D_D(DTickEffect);

    if (d->fixPixel == pixel) return;

    d->fixPixel = pixel;

    d->initDirection();
}

DTickEffectPrivate::DTickEffectPrivate(DTickEffect *qq)
    : DObjectPrivate(qq)
{
}

DTickEffectPrivate::~DTickEffectPrivate()
{
    runAnimation->deleteLater();
}

void DTickEffectPrivate::init()
{
    runAnimation = new QVariantAnimation;
    runAnimation->setLoopCount(-1);
    fixPixel = 30;
}

void DTickEffectPrivate::initDirection()
{
    switch (direction) {
    case DTickEffect::LeftToRight:
        runAnimation->setStartValue(QPoint(content->x(), content->y()));
        runAnimation->setEndValue(QPoint(content->width(), content->y()));
        runAnimation->setDuration((content->width() / fixPixel) * 1000);
        break;
    case DTickEffect::RightToLeft:
        runAnimation->setStartValue(QPoint(content->x(), content->y()));
        runAnimation->setEndValue(QPoint(-content->width(), content->y()));
        runAnimation->setDuration((content->width() / fixPixel) * 1000);
        break;
    case DTickEffect::TopToBottom:
        runAnimation->setStartValue(QPoint(content->x(), content->y()));
        runAnimation->setEndValue(QPoint(content->x(), content->height()));
        runAnimation->setDuration((content->height() / fixPixel) * 1000);
        break;
    case DTickEffect::BottomToTop:
        runAnimation->setStartValue(QPoint(content->x(), content->y()));
        runAnimation->setEndValue(QPoint(content->x(), -content->height()));
        runAnimation->setDuration((content->height() / fixPixel) * 1000);
        break;
    default:
        break;
    }
}

DWIDGET_END_NAMESPACE
