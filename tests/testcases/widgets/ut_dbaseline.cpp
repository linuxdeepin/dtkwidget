// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "dbaseline.h"
DWIDGET_USE_NAMESPACE
class ut_DBaseLine : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DBaseLine();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DBaseLine *target = nullptr;
};

TEST_F(ut_DBaseLine, setLeftMargin)
{
    target->setLeftMargin(1);
    ASSERT_EQ(target->leftMargin(), 1);
};

TEST_F(ut_DBaseLine, setRightMargin)
{
    target->setRightMargin(1);
    ASSERT_EQ(target->rightMargin(), 1);
};
