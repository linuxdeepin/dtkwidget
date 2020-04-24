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

#include "bartab.h"

#include <QBoxLayout>

#include "dcircleprogress.h"
#include "dwaterprogress.h"
#include "dcoloredprogressbar.h"
#include "dslider.h"

BarTab::BarTab(QWidget *parent) : QFrame(parent)
{
    DTK_WIDGET_NAMESPACE::DCircleProgress *circleProgess = new DTK_WIDGET_NAMESPACE::DCircleProgress(this);
    circleProgess->setFixedSize(100, 100);

    DTK_WIDGET_NAMESPACE::DWaterProgress *waterProgess = new DTK_WIDGET_NAMESPACE::DWaterProgress(this);
    waterProgess->setFixedSize(100, 100);

    DTK_WIDGET_NAMESPACE::DSlider *slider = new DTK_WIDGET_NAMESPACE::DSlider(Qt::Horizontal, this);
    slider->setMinimum(0);
    slider->setMaximum(100);
    slider->setFixedWidth(100);
    slider->setFixedHeight(20);

    DTK_WIDGET_NAMESPACE::DSlider *sliderSize = new DTK_WIDGET_NAMESPACE::DSlider(Qt::Horizontal, this);
    sliderSize->setMinimum(100);
    sliderSize->setMaximum(400);
    sliderSize->setFixedWidth(100);
    sliderSize->setFixedHeight(20);

    DTK_WIDGET_NAMESPACE::DColoredProgressBar *coloredpb = new DTK_WIDGET_NAMESPACE::DColoredProgressBar(this);
    QLinearGradient lg(0, 0.5, 1, 0.5);
    lg.setCoordinateMode(QGradient::CoordinateMode::ObjectBoundingMode);

    lg.setColorAt(0, QColor(0xFF0080FF));
    lg.setColorAt(1, QColor(0xFF06BEFD));
    coloredpb->addThreshold(0, lg);

    lg.setColorAt(0, QColor(0xFFF8AE2C));
    lg.setColorAt(1, QColor(0xFFF8C366));
    coloredpb->addThreshold(70, lg);

    lg.setColorAt(0, QColor(0xFFFF6170));
    lg.setColorAt(1, QColor(0xFFFF8C99));
    coloredpb->addThreshold(90, lg);

    auto layout = new QVBoxLayout(this);
    auto sliderLayout = new QHBoxLayout;
    sliderLayout->addWidget(slider, 0, Qt::AlignHCenter | Qt::AlignTop);
    sliderLayout->addWidget(sliderSize, 0, Qt::AlignHCenter | Qt::AlignTop);

    auto progressLayout = new QHBoxLayout;
    progressLayout->addWidget(circleProgess, 0, Qt::AlignHCenter | Qt::AlignTop);
    progressLayout->addWidget(waterProgess, 0, Qt::AlignHCenter | Qt::AlignTop);
    progressLayout->addWidget(coloredpb, 0, Qt::AlignHCenter | Qt::AlignTop);

    layout->addLayout(sliderLayout);
    layout->addLayout(progressLayout);
    layout->addStretch();

    connect(slider, &DTK_WIDGET_NAMESPACE::DSlider::valueChanged, [circleProgess, waterProgess, coloredpb](const int value) {
        circleProgess->setText(QString::number(value) + "%");
        waterProgess->setValue(value);
        circleProgess->setValue(value);
        coloredpb->setValue(value);
    });
    connect(sliderSize, &DTK_WIDGET_NAMESPACE::DSlider::valueChanged, [circleProgess, waterProgess](const int value) {
        waterProgess->setFixedSize(value, value);
        circleProgess->setFixedSize(value, value);
    });

    waterProgess->setValue(60);
    waterProgess->start();
    sliderSize->setValue(100);
}

