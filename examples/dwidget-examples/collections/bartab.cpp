/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "bartab.h"

#include <QBoxLayout>

#include "dcircleprogress.h"
#include "dwaterprogress.h"
#include "dslider.h"

BarTab::BarTab(QWidget *parent) : QFrame(parent)
{
    setStyleSheet("background-color:gray;");

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

    auto layout = new QVBoxLayout(this);
    auto sliderLayout = new QHBoxLayout;
    sliderLayout->addWidget(slider, 0, Qt::AlignHCenter | Qt::AlignTop);
    sliderLayout->addWidget(sliderSize, 0, Qt::AlignHCenter | Qt::AlignTop);

    auto progressLayout = new QHBoxLayout;
    progressLayout->addWidget(circleProgess, 0, Qt::AlignHCenter | Qt::AlignTop);
    progressLayout->addWidget(waterProgess, 0, Qt::AlignHCenter | Qt::AlignTop);

    layout->addLayout(sliderLayout);
    layout->addLayout(progressLayout);
    layout->addStretch();

    connect(slider, &DTK_WIDGET_NAMESPACE::DSlider::valueChanged, [circleProgess, waterProgess](const int value) {
        circleProgess->setText(QString::number(value) + "%");
        waterProgess->setValue(value);
        circleProgess->setValue(value);
    });
    connect(sliderSize, &DTK_WIDGET_NAMESPACE::DSlider::valueChanged, [circleProgess, waterProgess](const int value) {
        waterProgess->setFixedSize(value, value);
        circleProgess->setFixedSize(value, value);
    });

    waterProgess->setValue(60);
    waterProgess->start();
    sliderSize->setValue(100);
}

