// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QTest>
#include <QDebug>

#include "DWindowMaxButton"

DWIDGET_USE_NAMESPACE

class ut_DWindowMaxButton : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    QWidget *widget = nullptr;
    DWindowMaxButton *button = nullptr;
};

void ut_DWindowMaxButton::SetUp()
{
    widget = new QWidget;
    button = new DWindowMaxButton(widget);
    widget->resize(300, 150);
}

void ut_DWindowMaxButton::TearDown()
{
    if (widget) {
        delete widget;
        widget = nullptr;
    }
}

TEST_F(ut_DWindowMaxButton, testDWindowMaxButtonSetIconSize)
{
    const QSize size(100, 100);
    const QSize buttonSize(120, 120);

    button->setFixedSize(buttonSize);
    ASSERT_EQ(button->size(), buttonSize);

    button->setIconSize(size);
    ASSERT_EQ(button->iconSize(), size);
}

TEST_F(ut_DWindowMaxButton, testDWindowMaxButtonSignal)
{
    bool maximized;
    QObject::connect(button, &DWindowMaxButton::maximizedChanged, [&maximized](bool isMaximized){
        ASSERT_EQ(maximized, isMaximized);
    });
    maximized = true;
    button->setMaximized(maximized);

    maximized = false;
    button->setMaximized(maximized);
}
