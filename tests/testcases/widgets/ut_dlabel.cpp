// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "dlabel.h"
DWIDGET_USE_NAMESPACE
class ut_DLabel : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DLabel();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DLabel *target = nullptr;
};

TEST_F(ut_DLabel, setElideMode)
{
    target->setElideMode(Qt::ElideLeft);
    ASSERT_EQ(target->elideMode(), Qt::ElideLeft);
};

TEST_F(ut_DLabel, setForegroundRole)
{
    target->setForegroundRole(QPalette::WindowText);
    ASSERT_EQ(target->foregroundRole(), QPalette::WindowText);
};
