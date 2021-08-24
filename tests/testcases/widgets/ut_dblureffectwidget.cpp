/*
 * Copyright (C) 2021 ~ 2021 Deepin Technology Co., Ltd.
 *
 * Author:     Wang Peng <wangpenga@uniontech.com>
 *
 * Maintainer: Wang Peng <wangpenga@uniontech.com>
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

#include <gtest/gtest.h>
#include <QTest>
#include <QDebug>

#include <QPen>
#include <QPainter>
#include <QPainterPath>

#include "dblureffectwidget.h"
#include "private/dblureffectwidget_p.h"

DWIDGET_USE_NAMESPACE

class ut_DBlurEffectWidget : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    DBlurEffectWidget *widget = nullptr;
};

void ut_DBlurEffectWidget::SetUp()
{
    widget = new DBlurEffectWidget;
}

void ut_DBlurEffectWidget::TearDown()
{
    widget->deleteLater();
}

TEST_F(ut_DBlurEffectWidget, testBlurEffectWidgetFrontEnd)
{
//    // 测试信号槽的有效性
//    QObject::connect(widget, &DBlurEffectWidget::modeChanged, widget, [this](DBlurEffectWidget::BlurMode mode){
//        ASSERT_TRUE(widget->mode() == DBlurEffectWidget::BlurMode::GaussianBlur);
//    });

//    QObject::connect(widget, &DBlurEffectWidget::fullChanged, widget, [this](bool isFull){
//        ASSERT_TRUE(widget->isFull() == isFull);
//    });
//    widget->setFull(true);

//    // 测试 set mode 以及 mode change 的有效性
//    widget->setMode(DBlurEffectWidget::BlurMode::GaussianBlur);
//    ASSERT_TRUE(widget->mode() == DBlurEffectWidget::BlurMode::GaussianBlur);

//    widget->setRadius(5);
//    ASSERT_TRUE(widget->radius() == 5);

//    // 测试 setMaskPath
//    QPainter painter;
//    painter.setPen(Qt::yellow); //设置画笔颜色
//    QPainterPath path;
//    path.moveTo(widget->width()/2, widget->height()/2);
//    path.arcTo(widget->width()/2 - 100, widget->height()/2 - 100, 200, 200, 30, 120);
//    painter.drawPath(path);

//    widget->setMaskPath(path);
//    ASSERT_TRUE(widget->d_func()->maskPath == path);

//    widget->setBlendMode(DBlurEffectWidget::InWindowBlend);
//    ASSERT_TRUE(widget->blendMode() == DBlurEffectWidget::InWindowBlend);

//    widget->setMaskAlpha(155);
//    ASSERT_TRUE(widget->maskAlpha() == 155);
}

//TEST_F(ut_DBlurEffectWidget, testBlurEffectWidgetBackEnd)
//{
//    DBlurEffectWidget::MaskColorType type = DBlurEffectWidget::LightColor;
//    widget->setMaskColor(type);
//    ASSERT_TRUE(widget->d_func()->maskColorType == type);

//    widget->setBlurEnabled(true);
//    ASSERT_TRUE(widget->blurEnabled() == true);

//    QImage img(QIcon::fromTheme("icon_Window").pixmap(QSize(10,10)).toImage());
//    widget->setSourceImage(img);
//    ASSERT_TRUE(widget->d_func()->sourceImage == img);

//    int blurRectXRadius = 8;
//    widget->setBlurRectXRadius(blurRectXRadius);
//    ASSERT_EQ(widget->d_func()->blurRectXRadius, blurRectXRadius);

//    int blurRectYRadius = 8;
//    widget->setBlurRectYRadius(blurRectYRadius);
//    ASSERT_EQ(widget->d_func()->blurRectYRadius, blurRectYRadius);

//    const QFont font("Arial");
//    widget->setFont(font);
//    qDebug() << widget->font().family();
//    ASSERT_TRUE(widget->font().family() == font.family());
//}

