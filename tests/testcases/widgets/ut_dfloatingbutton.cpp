// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "dfloatingbutton.h"
DWIDGET_USE_NAMESPACE
class ut_DFloatingButton : public testing::Test
{
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(ut_DFloatingButton, DFloatingButton1)
{
    DFloatingButton *target = new DFloatingButton(nullptr);
    ASSERT_EQ(target->backgroundRole(), QPalette::Highlight);
    target->deleteLater();
};
