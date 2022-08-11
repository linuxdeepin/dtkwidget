// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "dpasswordedit.h"
DWIDGET_USE_NAMESPACE
class ut_DPasswordEdit : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DPasswordEdit();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DPasswordEdit *target = nullptr;
};

TEST_F(ut_DPasswordEdit, setEchoButtonIsVisible)
{
    target->setEchoButtonIsVisible(true);
    ASSERT_EQ(target->echoButtonIsVisible(), target->isVisible() && target->echoButtonIsVisible());
};

TEST_F(ut_DPasswordEdit, setEchoMode)
{
    target->setEchoMode(QLineEdit::Normal);
    ASSERT_EQ(target->echoMode(), QLineEdit::Normal);
    ASSERT_EQ(target->isEchoMode(), true);
};

TEST_F(ut_DPasswordEdit, setCopyEnabled)
{
    target->setCopyEnabled(true);
    ASSERT_TRUE(target->copyEnabled());

    target->setCopyEnabled(false);
    ASSERT_FALSE(target->copyEnabled());
}

TEST_F(ut_DPasswordEdit, setCutEnabled)
{
    target->setCutEnabled(true);
    ASSERT_TRUE(target->cutEnabled());

    target->setCutEnabled(false);
    ASSERT_FALSE(target->cutEnabled());
}

