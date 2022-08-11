// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "ddialogclosebutton.h"
DWIDGET_USE_NAMESPACE
class ut_DDialogCloseButton : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DDialogCloseButton();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DDialogCloseButton *target = nullptr;
};

TEST_F(ut_DDialogCloseButton, DDialogCloseButton)
{
    ASSERT_EQ(target->isFlat(), true);
};
