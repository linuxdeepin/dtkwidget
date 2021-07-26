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

#include "dprogressbar.h"

DWIDGET_USE_NAMESPACE

class ut_DProgressBar: public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    DProgressBar *bar = nullptr;
    QWidget *widget = nullptr;
};

void ut_DProgressBar::SetUp()
{
    widget = new QWidget;
    bar = new DProgressBar(widget);
    widget->resize(300, 100);
}

void ut_DProgressBar::TearDown()
{
    widget->deleteLater();
}

TEST_F(ut_DProgressBar, testDProgressBar)
{
    // 测试数值的设置是否正确
    for (int i = 0; i < 101; ++i) {
        bar->setValue(i);
        ASSERT_TRUE(bar->value() == i);
    }

    // 测试字体的设置是否正确
    QFont arial("Arial");
    bar->setFont(arial);
    ASSERT_TRUE(bar->font().family() == arial.family());
}
