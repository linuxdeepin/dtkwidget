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

#include "graphicseffecttab.h"

#include <DBlurEffectWidget>

#include <QLabel>
#include <QDebug>
DWIDGET_USE_NAMESPACE

GraphicsEffectTab::GraphicsEffectTab(QWidget *parent) : QWidget(parent)
{
    auto *mainLayout = new QHBoxLayout(this);

    DBlurEffectWidget *blurWidget = new DBlurEffectWidget(this);
    blurWidget->setRadius(30);
    blurWidget->setFixedSize(200, 200);
    blurWidget->setBlurRectXRadius(100);
    blurWidget->setBlurRectYRadius(100);
    blurWidget->setMaskColor(DBlurEffectWidget::LightColor);

    QPainterPath clipPath;

    clipPath.addRect(0, 50, 200, 150);
    blurWidget->setMaskPath(clipPath);

    QVBoxLayout *blurLayout = new QVBoxLayout(blurWidget);

    QPushButton *glowEffectButton = new QPushButton("GlowEffect");
    glowEffectButton->setFixedWidth(100);

    DGraphicsGlowEffect *effect = new DGraphicsGlowEffect();
    effect->setBlurRadius(5.0);
    effect->setDistance(15);
    effect->setYOffset(3);
    effect->setColor(Qt::darkCyan);
    glowEffectButton->setGraphicsEffect(effect);

    blurLayout->addWidget(glowEffectButton, 0, Qt::AlignCenter);


    DBlurEffectWidget *blurWidget_behind = new DBlurEffectWidget(this);

    blurWidget_behind->setBlendMode(DBlurEffectWidget::BehindWindowBlend);
    blurWidget_behind->setFixedSize(200, 200);
    blurWidget_behind->setBlurRectXRadius(10);
    blurWidget_behind->setBlurRectYRadius(20);
    blurWidget_behind->setMaskColor(Qt::black);

    mainLayout->addWidget(blurWidget, 0, Qt::AlignRight | Qt::AlignVCenter);
    mainLayout->addWidget(blurWidget_behind, 0, Qt::AlignLeft | Qt::AlignVCenter);
}

GraphicsEffectTab::~GraphicsEffectTab()
{

}

void GraphicsEffectTab::paintEvent(QPaintEvent *pe)
{
    QPainter paint(this);
    QPixmap bkgnd(":/images/default_background.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    paint.drawPixmap(this->rect(), bkgnd);
    QWidget::paintEvent(pe);
}

