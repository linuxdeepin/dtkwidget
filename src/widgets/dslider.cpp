/*
 * Copyright (C) 2011 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     wp <wangpeng_cm@deepin.com>
 *
 * Maintainer: wp <wangpeng_cm@deepin.com>
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

#include "dslider.h"
#include "private/dslider_p.h"

#include <QPainter>
#include <QEvent>
#include <QDebug>

#include <DApplicationHelper>
#include <DFontSizeManager>

DWIDGET_BEGIN_NAMESPACE

class SliderStrip : public QWidget
{
public:
    SliderStrip(Qt::Orientation orie, QWidget *parent = nullptr)
        : QWidget(parent)
        , orient(orie)
    {
        DFontSizeManager::instance()->bind(this, DFontSizeManager::T7);
    }
    QSize sizeHint() const override;

    void setScaleInfo(QStringList scaleInfo, QSlider::TickPosition tickPosition);
protected:
    void paintEvent(QPaintEvent *event) override;
    bool event(QEvent *e) override;
private:
    void drawSCale();
private:
    Qt::Orientation orient;
    QSlider::TickPosition tick;
    QStringList scaleInfo;
};

DSlider::DSlider(Qt::Orientation orientation, QWidget *parent)
    : QWidget(parent)
    , DObject(*new DSliderPrivate(this))
{
    Q_D(DSlider);
    d->orientation = orientation;
    d->init();
}

DSlider::DSlider(DSliderPrivate &q, QWidget *parent)
    : QWidget(parent)
    , DObject(q)
{

}

Qt::Orientation DSlider::orientation() const
{
    D_DC(DSlider);
    return d->slider->orientation();
}

QSlider *DSlider::slider()
{
    Q_D(DSlider);
    return d->slider;
}

void DSlider::setLeftIcon(const QIcon &left)
{
    D_D(DSlider);

    if (d->leftIcon == nullptr) {
        d->leftIcon = new DIconButton(nullptr);
        d->leftIcon->setFlat(true);
        d->leftIcon->setFocusPolicy(Qt::NoFocus);
        d->leftIcon->setAttribute(Qt::WA_TransparentForMouseEvents);

        if (orientation() == Qt::Horizontal) {
            d->layout->addWidget(d->leftIcon, 1, 0, Qt::AlignVCenter);
        } else {
            d->layout->addWidget(d->leftIcon, 0, 1, Qt::AlignHCenter);
        }

        if (d->iconSize.isValid())
            d->leftIcon->setIconSize(d->iconSize);
    }
    d->leftIcon->setIcon(left);
}

void DSlider::setRightIcon(const QIcon &right)
{
    D_D(DSlider);

    if (d->rightIcon == nullptr) {
        d->rightIcon = new DIconButton(nullptr);
        d->rightIcon->setFlat(true);
        d->rightIcon->setFocusPolicy(Qt::NoFocus);
        d->rightIcon->setAttribute(Qt::WA_TransparentForMouseEvents);

        if (orientation() == Qt::Horizontal) {
            d->layout->addWidget(d->rightIcon, 1, 2, Qt::AlignVCenter);
        } else {
            d->layout->addWidget(d->rightIcon, 2, 1, Qt::AlignHCenter);
        }

        if (d->iconSize.isValid())
            d->rightIcon->setIconSize(d->iconSize);
    }
    d->rightIcon->setIcon(right);
}

void DSlider::setIconSize(const QSize &size)
{
    D_D(DSlider);

    d->iconSize = size;

    if (d->leftIcon != nullptr) {
        d->leftIcon->setIconSize(size);
    }

    if (d->rightIcon != nullptr) {
        d->rightIcon->setIconSize(size);
    }
}

void DSlider::setMinimum(int min)
{
    D_D(DSlider);
    d->slider->setMinimum(min);
}

int DSlider::minimum() const
{
    D_DC(DSlider);
    return d->slider->minimum();
}

void DSlider::setValue(int value)
{
    D_D(DSlider);
    d->slider->setValue(value);
}

int DSlider::value() const
{
    D_DC(DSlider);
    return d->slider->value();
}

void DSlider::setPageStep(int pageStep)
{
    D_D(DSlider);
    d->slider->setPageStep(pageStep);
}

int DSlider::pageStep() const
{
    D_DC(DSlider);
    return d->slider->pageStep();
}

void DSlider::setMaximum(int max)
{
    D_D(DSlider);
    d->slider->setMaximum(max);
}

int DSlider::maximum() const
{
    D_DC(DSlider);
    return d->slider->maximum();
}

void DSlider::setLeftTicks(const QStringList &info)
{
    D_D(DSlider);

    if (info.isEmpty()) {
        if (d->left) {
            d->left->deleteLater();
            d->left = nullptr;
        }

        return;
    }

    if (d->left == nullptr) {
        d->left = new SliderStrip(orientation());
        d->left->setScaleInfo(info, QSlider::TicksLeft);

        if (orientation() == Qt::Horizontal) {
            d->layout->addWidget(d->left, 0, 1, Qt::AlignTop);
        } else {
            d->layout->addWidget(d->left, 1, 0, Qt::AlignRight);
        }
    }
    d->left->setScaleInfo(info, QSlider::TicksLeft);
}

void DSlider::setRightTicks(const QStringList &info)
{
    D_D(DSlider);

    if (info.isEmpty()) {
        if (d->right) {
            d->right->deleteLater();
            d->right = nullptr;
        }

        return;
    }

    if (d->right == nullptr) {
        d->right = new SliderStrip(orientation());
        d->right->setScaleInfo(info, QSlider::TicksRight);

        if (orientation() == Qt::Horizontal) {
            d->layout->addWidget(d->right, 2, 1, Qt::AlignTop);
        } else {
            d->layout->addWidget(d->right, 1, 2, Qt::AlignLeft);
        }
    }
    d->right->setScaleInfo(info, QSlider::TicksRight);
}

void DSlider::setAboveTicks(const QStringList &info)
{
    setLeftTicks(info);
}

void DSlider::setBelowTicks(const QStringList &info)
{
    setRightTicks(info);
}

QSlider::TickPosition DSlider::tickPosition() const
{
    D_DC(DSlider);

    if (d->left && d->right) {
        return QSlider::TicksBothSides;
    }

    if (d->left) {
        return QSlider::TicksLeft;
    }

    if (d->right) {
        return QSlider::TicksRight;
    }

    return QSlider::NoTicks;
}

DSliderPrivate::DSliderPrivate(DSlider *q)
    : DObjectPrivate(q)
    , slider(nullptr)
    , leftIcon(nullptr)
    , rightIcon(nullptr)
    , left(nullptr)
    , right(nullptr)
{

}

void DSliderPrivate::init()
{
    D_Q(DSlider);

    slider = new QSlider(orientation);
    q->connect(slider, &QSlider::valueChanged, q, &DSlider::valueChanged);
    q->connect(slider, &QSlider::sliderPressed, q, &DSlider::sliderPressed);
    q->connect(slider, &QSlider::sliderMoved, q, &DSlider::sliderMoved);
    q->connect(slider, &QSlider::sliderReleased, q, &DSlider::sliderReleased);
    q->connect(slider, &QSlider::rangeChanged, q, &DSlider::rangeChanged);
    q->connect(slider, &QSlider::actionTriggered, q, &DSlider::actionTriggered);

    layout = new QGridLayout(q);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(slider, 1, 1);

    if (q->orientation() == Qt::Horizontal) {
        q->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    } else {
        q->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
    }
    slider->setTickPosition(QSlider::NoTicks);
}

QSize SliderStrip::sizeHint() const
{
    QSize size = QWidget::sizeHint();
    int fontSize = fontMetrics().height();

    if (orient == Qt::Horizontal) {
        size.setHeight(fontSize * 1.5);
    } else {
        int max = 0;

        for (int i = 0;  i < scaleInfo.count(); i++) {
            max = qMax(max, fontMetrics().width(scaleInfo[i]));
        }
        size.setWidth(max + fontSize / 2);
    }

    return size;
}

void SliderStrip::setScaleInfo(QStringList scaleInfo, QSlider::TickPosition tickPosition)
{
    this->scaleInfo = scaleInfo;
    this->tick = tickPosition;
}

void SliderStrip::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter pa(this);
    pa.setPen(QPen(DApplicationHelper::instance()->palette(this).textTips(), 1));

    int tickSize = fontMetrics().height() / 2;
    int offsetSize = style()->pixelMetric(QStyle::PM_SliderLength, nullptr, this) / 2;
    qreal startX = 0, startY = 0, endX = 0, endY = 0;
    int width = this->width(), height = this->height();    //slider宽高
    int paragraph = scaleInfo.count();    //刻度个数
    qreal average = 0;    //每一段的距离
    qreal textPos = 0;    //字体位置

    if (orient == Qt::Horizontal) {
        width -= 2 * offsetSize;
        startX += offsetSize;
        endX = startX;
        average = width / static_cast<qreal>(paragraph - 1);

        if (tick == QSlider::TicksBelow) {
            endY = tickSize;
            textPos = endY;
        } else if (tick == QSlider::TicksAbove) {
            startY = height;
            endY = startY - tickSize;
        }

        pa.drawLine(QPointF(startX, startY), QPointF(endX, endY));
        pa.drawText(QRectF(endX - offsetSize, textPos, width, height - tickSize), Qt::AlignLeft, scaleInfo[0]);

        for (int i = 1; i < paragraph - 1; i++) {
            startX += average;
            endX = startX;
            pa.drawLine(QPointF(startX, startY), QPointF(endX, endY));
            pa.drawText(QRectF(endX - width / 2, textPos, width, height - tickSize), Qt::AlignHCenter, scaleInfo[i]);
        }

        if (paragraph > 1) {
            startX += average;
            endX = startX;
            pa.drawLine(QPointF(startX, startY), QPointF(endX, endY));
            pa.drawText(QRectF(endX - width + offsetSize, textPos, width, height - tickSize), Qt::AlignRight, scaleInfo[paragraph - 1]);
        }
    } else {
        startY = offsetSize;
        height -= offsetSize * 2;
        endY = startY;
        average = height / static_cast<qreal>(paragraph - 1);
        Qt::Alignment text_flags = Qt::AlignVCenter;

        if (tick == QSlider::TicksLeft) {
            startX = width;
            endX = startX - tickSize;
            textPos = 0;
            text_flags |= Qt::AlignRight;
        } else if (tick == QSlider::TicksRight) {
            endX = tickSize;
            textPos = endX;
            text_flags |= Qt::AlignLeft;
        }

        for (int i = 0; i < paragraph; i++) {
            pa.drawLine(QPointF(startX, startY), QPointF(endX, endY));
            pa.drawText(QRectF(textPos, endY - average / 2, width - tickSize, average), text_flags, scaleInfo[i]);
            startY += average;
            endY = startY;
        }
    }
}

bool SliderStrip::event(QEvent *e)
{
    if (e->type() == QEvent::FontChange || e->type() == QEvent::ApplicationFontChange) {
        updateGeometry();
    }

    return QWidget::event(e);
}

DWIDGET_END_NAMESPACE
