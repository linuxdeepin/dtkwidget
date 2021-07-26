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

#include "dboxwidget.h"

DWIDGET_USE_NAMESPACE

class ut_DBoxWidget : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    QWidget *widget = nullptr;
};

void ut_DBoxWidget::SetUp()
{
    widget = new QWidget;
}

void ut_DBoxWidget::TearDown()
{
    widget->deleteLater();
}

TEST_F(ut_DBoxWidget, createMimeDataFromSelection)
{

    widget->resize(200, 200);

    QBoxLayout::Direction direction = QBoxLayout::LeftToRight;
    DBoxWidget box(direction, widget);

    // 测试方向设置是否正确
    ASSERT_EQ(box.direction() == direction, true);
    for (direction = QBoxLayout::LeftToRight; direction < QBoxLayout::Up;) {
        box.setDirection(direction);
        ASSERT_EQ(box.direction() == direction, true);

        int direct = direction;
        direction = (QBoxLayout::Direction)(direct + 1);
    }

    // 测试设置大小是否正确
    QSize size(200, 200);
    box.setFixedSize(size);

    ASSERT_TRUE(box.size() == size);

    // 测试设置方向的信号是否正确
    QObject::connect(&box, &DBoxWidget::directionChanged, widget, [this, &box](QBoxLayout::Direction direction){
        ASSERT_TRUE(box.direction() == direction);
    });

    box.setDirection(QBoxLayout::LeftToRight);
}
