// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "private/dmpriscontrol_p.h"
#include "dmpriscontrol.h"
DWIDGET_USE_NAMESPACE
class ut_DMPRISControl : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DMPRISControl();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DMPRISControl *target = nullptr;
};

TEST_F(ut_DMPRISControl, isWorking)
{
    DMPRISControlPrivate* d = target->d_func();
    ASSERT_EQ(target->isWorking(), d->m_mprisInter != nullptr);
};
