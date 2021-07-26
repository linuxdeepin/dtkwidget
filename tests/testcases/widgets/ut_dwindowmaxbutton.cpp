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
