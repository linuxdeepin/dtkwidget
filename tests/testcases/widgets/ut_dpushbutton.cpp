// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
