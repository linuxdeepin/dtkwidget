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

#include "dswitchbutton.h"

DWIDGET_USE_NAMESPACE

class ut_DSwitchButton : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    DSwitchButton *button = nullptr;
    QWidget *widget = nullptr;
};

void ut_DSwitchButton::SetUp()
{
    widget = new QWidget;
    button = new DSwitchButton(widget);
    widget->resize(300, 200);
}

void ut_DSwitchButton::TearDown()
{
    widget->deleteLater();
}

TEST_F(ut_DSwitchButton, testDSwitchButtonClicked)
{
    // 测试 sizeHint
    ASSERT_TRUE(button->sizeHint().width() > 0);
    ASSERT_TRUE(button->sizeHint().height() > 0);

    // 测试按钮按下
    int count = 0;
    QObject::connect(button, &DSwitchButton::clicked, button, [&count](bool isClicked){
        if (count)
            ASSERT_TRUE(isClicked == false);
        else
            ASSERT_TRUE(isClicked == true);
        ++count;
    });
    emit button->clicked(true);
    emit button->clicked(false);

    QObject::disconnect(button, &DSwitchButton::clicked, button, nullptr);

    ASSERT_TRUE(count == 2);
}

TEST_F(ut_DSwitchButton, testDSwitchButtonToggled)
{
    // 测试按钮翻转
    int count = 0;
    QObject::connect(button, &DSwitchButton::toggled, button, [&count](bool isClicked){
        if (count)
            ASSERT_TRUE(isClicked == false);
        else
            ASSERT_TRUE(isClicked == true);
        ++count;
    });

    emit button->toggled(true);
    emit button->toggled(false);

    QObject::disconnect(button, &DSwitchButton::toggled, button, nullptr);

    ASSERT_TRUE(count == 2);
}

TEST_F(ut_DSwitchButton, testDSwitchButtonCheckedChanged)
{
    // 测试按钮翻转
    int count = 0;
    QObject::connect(button, &DSwitchButton::checkedChanged, button, [&count](bool isChecked){
        if (count)
            ASSERT_TRUE(isChecked == false);
        else
            ASSERT_TRUE(isChecked == true);
        ++count;
    });

    emit button->setChecked(true);
    emit button->setChecked(false);

    QObject::disconnect(button, &DSwitchButton::toggled, button, nullptr);

    ASSERT_TRUE(count == 2);
}
