// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "danchors.h"
DWIDGET_USE_NAMESPACE

class ut_DAnchorsBase : public testing::Test
{
protected:
    void SetUp() override
    {
        parentWidget = new QWidget();
        target = new DAnchorsBase(parentWidget);
    }
    void TearDown() override
    {
        if (parentWidget) {
            delete parentWidget;
            parentWidget = nullptr;
        }
    }
    DAnchorsBase *target = nullptr;
    QWidget *parentWidget = nullptr;
};

TEST_F(ut_DAnchorsBase, setBottomMargin)
{
    target->setBottomMargin(1);
    ASSERT_EQ(target->bottomMargin(), 1);
};

TEST_F(ut_DAnchorsBase, setEnabled)
{
    target->setEnabled(true);
    ASSERT_EQ(target->enabled(), true);
};

TEST_F(ut_DAnchorsBase, setHorizontalCenterOffset)
{
    target->setHorizontalCenterOffset(1);
    ASSERT_EQ(target->horizontalCenterOffset(), 1);
};

TEST_F(ut_DAnchorsBase, setLeftMargin)
{
    target->setLeftMargin(1);
    ASSERT_EQ(target->leftMargin(), 1);
};

TEST_F(ut_DAnchorsBase, setMargins)
{
    target->setMargins(1);
    ASSERT_EQ(target->margins(), 1);
};

TEST_F(ut_DAnchorsBase, setRightMargin)
{
    target->setRightMargin(1);
    ASSERT_EQ(target->rightMargin(), 1);
};

TEST_F(ut_DAnchorsBase, setTopMargin)
{
    target->setTopMargin(1);
    ASSERT_EQ(target->topMargin(), 1);
};

TEST_F(ut_DAnchorsBase, setVerticalCenterOffset)
{
    target->setVerticalCenterOffset(1);
    ASSERT_EQ(target->verticalCenterOffset(), 1);
};
