// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QTest>
#include <QDebug>

#include "DWindowMinButton"

DWIDGET_USE_NAMESPACE

class ut_DWindowMinButton : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    QWidget *widget = nullptr;
    DWindowMinButton *button = nullptr;
};

void ut_DWindowMinButton::SetUp()
{
    widget = new QWidget;
    button = new DWindowMinButton(widget);
    widget->resize(300, 150);
}

void ut_DWindowMinButton::TearDown()
{
    if (widget) {
        delete widget;
        widget = nullptr;
    }
}

TEST_F(ut_DWindowMinButton, testDWindowMinButtonSetIconSize)
{
    const QSize size(100, 100);
    const QSize buttonSize(120, 120);

    button->setFixedSize(buttonSize);
    ASSERT_EQ(button->size(), buttonSize);

    button->setIconSize(size);
    ASSERT_EQ(button->iconSize(), size);
}

TEST_F(ut_DWindowMinButton, testDWindowMinButtonSizeHint)
{
    const QSize sizeHint(12, 12);
    ASSERT_EQ(button->sizeHint(), sizeHint);
}
