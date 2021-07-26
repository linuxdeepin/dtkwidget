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
