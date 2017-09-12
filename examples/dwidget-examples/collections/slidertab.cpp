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

#include "dslider.h"
#include "slidertab.h"

DWIDGET_USE_NAMESPACE

SliderTab::SliderTab(QWidget *parent) : QLabel(parent)
{
    setStyleSheet("SliderTab{background-color:  #1A1B1B}");

    DSlider * slider = new DSlider(Qt::Horizontal, this);
    slider->setFixedWidth(300);
    slider->setFixedHeight(40);
    slider->move(20, 50);

    DSlider *sharpSlider = new DSlider(Qt::Horizontal, this);
    sharpSlider->setHandleType(DSlider::SharpHandler);
    sharpSlider->setFixedWidth(300);
    sharpSlider->setFixedHeight(40);
    sharpSlider->move(20, 100);

}

