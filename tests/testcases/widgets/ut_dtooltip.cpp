// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "dtooltip.h"
DWIDGET_USE_NAMESPACE
class ut_DToolTip : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DToolTip("DToolTip");
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DToolTip *target = nullptr;
};

TEST_F(ut_DToolTip, sizeHint)
{
    ASSERT_GE(target->sizeHint().width(), target->fontMetrics().size(Qt::TextSingleLine, target->text()).width());
};
