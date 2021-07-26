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
