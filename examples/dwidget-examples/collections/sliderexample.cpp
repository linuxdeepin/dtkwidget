/*
 * Copyright (C) 2020 ~ 2020 Deepin Technology Co., Ltd.
 *
 * Author:     sunkang <sunkang@uniontech.com>
 *
 * Maintainer: sunkang <sunkang@uniontech.com>
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

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>

#include <DDialog>
#include <DStyle>
#include <DPushButton>
#include <DMessageBox>
#include <DWarningButton>
#include <DSuggestButton>
#include <DSlider>
#include <DFrame>
#include <DLabel>

#include "sliderexample.h"

DWIDGET_USE_NAMESPACE

SliderExampleWindow::SliderExampleWindow(QWidget *parent)
    : PageWindowInterface(parent)
{
    addExampleWindow(new DSliderExample(this));
}

DSliderExample::DSliderExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    DSlider *vSlider = new DSlider(Qt::Vertical);
    DSlider *hSlider = new DSlider(Qt::Horizontal);
    DSlider *hCalibration = new DSlider(Qt::Horizontal);
    DLabel *hLabel = new DLabel;
    DLabel *vLabel = new DLabel;
    QVBoxLayout *mainLayout = new QVBoxLayout;

    vSlider->setMinimumHeight(283);
    hSlider->setMinimumWidth(479);
    hCalibration->setMinimumWidth(507);
    hCalibration->setBelowTicks({"", "", "", "", "", "", ""});

    hLabel->setScaledContents(true);
    vLabel->setScaledContents(true);

    hLabel->setFixedSize(550, 426);
    vLabel->setFixedSize(550, 350);

    hLabel->setPixmap(QPixmap(":/images/example/DSlider_1.png"));
    vLabel->setPixmap(QPixmap(":/images/example/DSlider_2.png"));

    setLayout(mainLayout);
    mainLayout->setSpacing(10);
    mainLayout->addWidget(vSlider, 0, Qt::AlignCenter);
    mainLayout->addWidget(hSlider, 0, Qt::AlignCenter);
    mainLayout->addWidget(hCalibration, 0, Qt::AlignCenter);
    mainLayout->addWidget(hLabel, 0, Qt::AlignCenter);
    mainLayout->addWidget(vLabel, 0, Qt::AlignCenter);
}

QString DSliderExample::getTitleName() const
{
    return "DSlider";
}

QString DSliderExample::getDescriptionInfo() const
{
    return "圆角矩形滑块可以随意拖动，起止点一\n"
            "定是从左往右递增，滑块以左部分是活\n"
            "动色显示。\n\n"
            "尖角的滑块不可以像圆角矩形滑块那样\n"
            "进行随意拖动，底下对应的有刻度，刻\n"
            "度上产生吸附力，尖角也只能在几个刻\n"
            "度值上调整，更多强调的是用户一个值\n"
            "的取舍。\n "
            "对应的刻度可能有刻度值显示，也可能\n"
            "没有刻度值（界面上的）。";
}

int DSliderExample::getFixedHeight() const
{
    return 1276;
}
