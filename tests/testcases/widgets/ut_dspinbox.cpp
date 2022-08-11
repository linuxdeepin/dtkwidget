// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <DLineEdit>

#include "dspinbox.h"
DWIDGET_USE_NAMESPACE
class ut_DDoubleSpinBox : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DDoubleSpinBox();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DDoubleSpinBox *target = nullptr;
};

TEST_F(ut_DDoubleSpinBox, lineEdit)
{
    ASSERT_TRUE(target->lineEdit());
};

TEST_F(ut_DDoubleSpinBox, setAlert)
{
    target->setAlert(true);
    ASSERT_EQ(target->isAlert(), true);
};

TEST_F(ut_DDoubleSpinBox, setEnabledEmbedStyle)
{
    target->setEnabledEmbedStyle(true);
    ASSERT_EQ(target->property("_d_dtk_spinBox").toBool(), true);
};

class ut_DSpinBox : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DSpinBox();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DSpinBox *target = nullptr;
};

TEST_F(ut_DSpinBox, lineEdit)
{
    ASSERT_TRUE(target->lineEdit());
};

TEST_F(ut_DSpinBox, setAlert)
{
    target->setAlert(true);
    ASSERT_EQ(target->isAlert(), true);
};

TEST_F(ut_DSpinBox, setEnabledEmbedStyle)
{
    target->setEnabledEmbedStyle(true);
    ASSERT_EQ(target->property("_d_dtk_spinBox").toBool(), true);
};
