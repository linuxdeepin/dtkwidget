// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "dsearchedit.h"
DWIDGET_USE_NAMESPACE
class ut_DSearchEdit : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DSearchEdit();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DSearchEdit *target = nullptr;
};

TEST_F(ut_DSearchEdit, clear)
{
    target->setText("setText");
    target->clear();
    ASSERT_EQ(target->text(), "");
};

TEST_F(ut_DSearchEdit, clearEdit)
{
    target->setText("setText");
    target->clearEdit();
    ASSERT_EQ(target->text(), "");
};

TEST_F(ut_DSearchEdit, isVoiceInput)
{
    ASSERT_EQ(target->isVoiceInput(), false);
};

TEST_F(ut_DSearchEdit, setPlaceHolder)
{
    target->setPlaceHolder("setPlaceHolder");
    ASSERT_EQ(target->placeHolder(), "setPlaceHolder");
};

TEST_F(ut_DSearchEdit, setPlaceholderText)
{
    target->setPlaceholderText("setPlaceholderText");
    ASSERT_EQ(target->placeholderText(), "setPlaceholderText");
};
