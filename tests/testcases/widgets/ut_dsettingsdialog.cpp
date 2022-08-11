// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "dsettingsdialog.h"
DWIDGET_USE_NAMESPACE
class ut_DSettingsDialog : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DSettingsDialog();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DSettingsDialog *target = nullptr;
};

TEST_F(ut_DSettingsDialog, setGroupVisible)
{
    target->setGroupVisible("setGroupVisible", true);
    ASSERT_EQ(target->groupIsVisible("setGroupVisible"), target->isVisible() && target->groupIsVisible("setGroupVisible"));
};
