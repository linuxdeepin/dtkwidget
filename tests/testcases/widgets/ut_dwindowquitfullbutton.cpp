// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QTest>
#include <QDebug>

#include "DWindowQuitFullButton"

DWIDGET_USE_NAMESPACE

class ut_DWindowQuitFullButton : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    QWidget *widget = nullptr;
    DWindowQuitFullButton *button = nullptr;
};

void ut_DWindowQuitFullButton::SetUp()
{
    widget = new QWidget;
    button = new DWindowQuitFullButton(widget);
    widget->resize(300, 100);
}

void ut_DWindowQuitFullButton::TearDown()
{
    if (widget) {
        delete widget;
        widget = nullptr;
    }
}

TEST_F(ut_DWindowQuitFullButton, testut_DWindowQuitFullButtonSetIconSize)
{
    const QSize size(100, 100);
    const QSize buttonSize(120, 120);

    button->setFixedSize(buttonSize);
    ASSERT_EQ(button->size(), buttonSize);

    button->setIconSize(size);
    ASSERT_EQ(button->iconSize(), size);
}

TEST_F(ut_DWindowQuitFullButton, testut_DWindowQuitFullButtonSizeHint)
{
    const QSize sizeHint(12, 12);
    ASSERT_EQ(button->sizeHint(), sizeHint);
}

