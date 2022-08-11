// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QTest>
#include <QDebug>

#include "dwarningbutton.h"

DWIDGET_USE_NAMESPACE

class ut_DWarningButton : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    DWarningButton *button = nullptr;
    QWidget *widget = nullptr;
};

void ut_DWarningButton::SetUp()
{
    widget = new QWidget;
    button = new DWarningButton(widget);
    widget->resize(300, 200);
}

void ut_DWarningButton::TearDown()
{
    widget->deleteLater();
}

TEST_F(ut_DWarningButton, testDWarningButton)
{
    // 测试 Icon 是否正确设置
    QIcon icon(QIcon::fromTheme("preferences-system"));
    button->setIcon(icon);
    QIcon tmp(button->icon());
    ASSERT_TRUE(button->icon().data_ptr() == tmp.data_ptr());

    // 测试文本是否设置正确
    QString btStr("aaaaaaaa");
    button->setText(btStr);
    ASSERT_TRUE(button->text() == btStr);

    // 测试 flat 是否设置成功
    button->setFlat(true);
    ASSERT_TRUE(button->isFlat());
}
