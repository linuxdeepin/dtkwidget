// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "dsuggestbutton.h"
DWIDGET_USE_NAMESPACE
class ut_DSuggestButton : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DSuggestButton("DSuggestButton");
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DSuggestButton *target = nullptr;
};

TEST_F(ut_DSuggestButton, DSuggestButton)
{
    ASSERT_EQ(target->text(), "DSuggestButton");
};
