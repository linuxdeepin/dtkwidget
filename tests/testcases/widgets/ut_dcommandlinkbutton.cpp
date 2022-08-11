// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "dcommandlinkbutton.h"
DWIDGET_USE_NAMESPACE
class ut_DCommandLinkButton : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DCommandLinkButton("DCommandLinkButton");
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DCommandLinkButton *target = nullptr;
};

TEST_F(ut_DCommandLinkButton, sizeHint)
{
    ASSERT_GE(target->sizeHint().width(), target->fontMetrics().size(0, target->text()).width());
};
