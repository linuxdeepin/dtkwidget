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

#include "dipv4lineedit.h"

DWIDGET_USE_NAMESPACE

class ut_DIpv4LineEdit : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    QWidget *widget = nullptr;
    DIpv4LineEdit *lineEdit = nullptr;
};

void ut_DIpv4LineEdit::SetUp()
{
    widget = new QWidget;
    lineEdit = new DIpv4LineEdit(widget);
    widget->resize(300, 100);
}

void ut_DIpv4LineEdit::TearDown()
{
    if (widget) {
        delete widget;
        widget = nullptr;
    }
}

TEST_F(ut_DIpv4LineEdit, testDIpv4LineEditSetText)
{
    const QString ipAddress("255.255.255.255");
    lineEdit->setText(ipAddress);
    ASSERT_EQ(ipAddress, lineEdit->text());
}

TEST_F(ut_DIpv4LineEdit, testDIpv4LineEditSetTextAndClear)
{
    const QString ipAddress;
    lineEdit->clear();
    ASSERT_EQ(ipAddress, lineEdit->text());
}

TEST_F(ut_DIpv4LineEdit, testDIpv4LineEditSelectText)
{
    const QString ipAddress("255.255.255.255");
    lineEdit->setText(ipAddress);
    lineEdit->selectAll();
    lineEdit->setSelection(0, -1);
    // TODO: 加接口测试
}

TEST_F(ut_DIpv4LineEdit, testDIpv4LineEditSetReadOnly)
{
    lineEdit->setReadOnly(true);
    ASSERT_TRUE(lineEdit->isReadOnly());
    lineEdit->setReadOnly(false);
    ASSERT_FALSE(lineEdit->isReadOnly());
}

TEST_F(ut_DIpv4LineEdit, testDIpv4LineEditSetDisplayText)
{
    const QString displayText("please input ip address.");
    lineEdit->setText(displayText);
    ASSERT_EQ(lineEdit->displayText(), displayText);
}

TEST_F(ut_DIpv4LineEdit, testDIpv4LineEditSetMaxLength)
{
    int maxLength = 16;
    lineEdit->setMaxLength(maxLength);
    ASSERT_EQ(lineEdit->maxLength(), maxLength);
}

TEST_F(ut_DIpv4LineEdit, testDIpv4LineEditSetFrame)
{
    bool hasFrame = false;
    lineEdit->setFrame(hasFrame);
    ASSERT_FALSE(lineEdit->hasFrame());

    hasFrame = true;
    lineEdit->setFrame(hasFrame);
    ASSERT_TRUE(lineEdit->hasFrame());
}
