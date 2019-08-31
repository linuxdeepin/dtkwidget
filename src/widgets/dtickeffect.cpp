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

#include "dtickeffect.h"
#include "private/dtickeffect_p.h"

#include <QHBoxLayout>
#include <QPixmap>
#include <QWidget>
#include <QPainter>
#include <QEvent>

DWIDGET_BEGIN_NAMESPACE

/*!
 * \~chinese \class DTickEffect
 * \~chinese \brief 这个类提供了跑马灯效果，可以让控件的内容滚动起来
 *
 * 注意: 该类目前只支持滚动可视区域，如果需要滚动完整的界面，需要使用QScrollarea封装一下
 *
 * \~chinese \enum DTickEffect::Direction 滚动方向的枚举值
 * \~chinese \var DTickEffect::Direction DTickEffect::LeftToRight
 * \~chinese \brief 从左往右滚动
 * \~chinese \var DTickEffect::Direction DTickEffect::RightToLeft
 * \~chinese \brief  从右往左滚动
 * \~chinese \var DTickEffect::Direction DTickEffect::TopToBottom
 * \~chinese \brief  从上往下滚动
 * \~chinese \var DTickEffect::Direction DTickEffect::BottomToTop
 * \~chinese \brief  从下往上滚动
 *
 * \~chinese \fn DTickEffect::finished 调用stop方法以后会发出该信号
 * \~chinese \fn DTickEffect::stateChanged 当方向发生改变时会发出该信号
 */


/*!
 * \~chinese \brief DTickEffect的构造函数
 *
 * @param widget 设置动画绑定的对象
 * @param parent
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
 * \~chinese \brief 开始播放
 *
 */
void DTickEffect::play()
{
    D_D(DTickEffect);

    d->runAnimation->start();

    Q_EMIT stateChanged();
}

/*!
 * \~chinese \brief 停止播放
 *
 */
void DTickEffect::stop()
{
    D_D(DTickEffect);

    d->runAnimation->stop();

    Q_EMIT stateChanged();
}

/*!
 * \~chinese \brief 暂停播放
 *
 */
void DTickEffect::pause()
{
    D_D(DTickEffect);

    d->runAnimation->pause();

    Q_EMIT stateChanged();
}

/*!
 * \~chinese \brief 恢复播放
 *
 */
void DTickEffect::resume()
{
    D_D(DTickEffect);

    d->runAnimation->resume();

    Q_EMIT stateChanged();
}

/*!
 * \~chinese \brief 设置滚动的方向
 *
 * @param direction
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
