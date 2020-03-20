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

#ifndef DSLIDER_H
#define DSLIDER_H

#include <QWidget>
#include <QSlider>
#include <QStyleOption>
#include <QPainter>

#include <dtkwidget_global.h>
#include <dobject.h>

DWIDGET_BEGIN_NAMESPACE

class DSliderPrivate;
class LIBDTKWIDGETSHARED_EXPORT DSlider : public QWidget, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DSlider)
    D_DECLARE_PRIVATE(DSlider)
public:
    DSlider(Qt::Orientation orientation = Qt::Horizontal, QWidget *parent = nullptr);

    Qt::Orientation orientation() const;

    QSlider *slider();

    void setLeftIcon(const QIcon &left);
    void setRightIcon(const QIcon &right);

    void setIconSize(const QSize &size);

    void setMinimum(int min);
    int minimum() const;

    void setValue(int value);
    int value() const;

    void setPageStep(int pageStep);
    int pageStep() const;

    void setMaximum(int max);
    int maximum() const;

    void setLeftTicks(const QStringList &info);
    void setRightTicks(const QStringList &info);

    void setAboveTicks(const QStringList &info);
    void setBelowTicks(const QStringList &info);

    void setMarkPositions(QList<int> list);

    void setMouseWheelEnabled(bool enabled);

    void setTipValue(const QString &value);

    QSlider::TickPosition tickPosition() const;
    QSize sizeHint() const override;

    void setHandleVisible(bool b);
    bool handleVisible() const;

Q_SIGNALS:
    void valueChanged(int value);

    void sliderPressed();
    void sliderMoved(int position);
    void sliderReleased();

    void rangeChanged(int min, int max);

    void actionTriggered(int action);

protected:
    DSlider(DSliderPrivate &q, QWidget *parent);

    bool event(QEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
};

class SpecialSlider : public QSlider {
public:
    SpecialSlider(Qt::Orientation orientation, QWidget *parent = nullptr) : QSlider(orientation, parent) {
    }

    void paintEvent(QPaintEvent *ev) {
        Q_UNUSED(ev)
        QPainter p(this);
        QStyleOptionSlider opt;
        initStyleOption(&opt);

        DSlider* dSlider = qobject_cast<DSlider *>(this->parent());

        if (!dSlider)
            return;

        if (dSlider->handleVisible())
            opt.subControls = QStyle::SC_SliderGroove | QStyle::SC_SliderHandle;
        else
            opt.subControls = QStyle::SC_SliderGroove;

        style()->drawComplexControl(QStyle::CC_Slider, &opt, &p, this);
    }
};

DWIDGET_END_NAMESPACE

#endif // DSLIDER_H
