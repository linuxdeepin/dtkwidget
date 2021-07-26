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

#include "DPushButton"

DWIDGET_USE_NAMESPACE

class ut_DPushButton : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    DPushButton *button = nullptr;
    QWidget *widget = nullptr;
};

void ut_DPushButton::SetUp()
{
    widget = new QWidget;
    button = new DPushButton(widget);
    widget->resize(300, 200);
}

void ut_DPushButton::TearDown()
{
    widget->deleteLater();
}

TEST_F(ut_DPushButton, testDPushButtonClicked)
{
    // 测试 sizeHint
    ASSERT_TRUE(button->sizeHint().width() > 0);
    ASSERT_TRUE(button->sizeHint().height() > 0);

    // 测试按钮按下
    bool clickState;
    QObject::connect(button, &DPushButton::clicked, button, [&clickState](bool isClicked){
        ASSERT_EQ(clickState, isClicked);
    });
    clickState = true;
    emit button->clicked(clickState);
    clickState = false;
    emit button->clicked(clickState);

    QObject::disconnect(button, &DPushButton::clicked, button, nullptr);
}

TEST_F(ut_DPushButton, testDPushButtonToggled)
{
    // 测试按钮翻转
    bool toggleState;
    QObject::connect(button, &DPushButton::toggled, button, [&toggleState](bool isClicked){
        ASSERT_EQ(toggleState, isClicked);
    });

    toggleState = true;
    emit button->toggled(toggleState);
    toggleState = false;
    emit button->toggled(toggleState);

    QObject::disconnect(button, &DPushButton::toggled, button, nullptr);
}
