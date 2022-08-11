// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QTest>
#include <QDebug>

#include "DWindowOptionButton"

DWIDGET_USE_NAMESPACE

class ut_DWindowOptionButton : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    QWidget *widget = nullptr;
    DWindowOptionButton *button = nullptr;
};

void ut_DWindowOptionButton::SetUp()
{
    widget = new QWidget;
    button = new DWindowOptionButton(widget);
    widget->resize(300, 100);
}

void ut_DWindowOptionButton::TearDown()
{
    if (widget) {
        delete widget;
        widget = nullptr;
    }
}

TEST_F(ut_DWindowOptionButton, testDWindowOptionButtonSetIconSize)
{
    const QSize size(100, 100);
    const QSize buttonSize(120, 120);

    button->setFixedSize(buttonSize);
    ASSERT_EQ(button->size(), buttonSize);

    button->setIconSize(size);
    ASSERT_EQ(button->iconSize(), size);
}

TEST_F(ut_DWindowOptionButton, testDWindowOptionButtonSizeHint)
{
    const QSize sizeHint(12, 12);
    ASSERT_EQ(button->sizeHint(), sizeHint);
}

