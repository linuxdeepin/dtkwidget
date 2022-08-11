// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "dsearchcombobox.h"
DWIDGET_USE_NAMESPACE
class ut_DSearchComboBox : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DSearchComboBox();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DSearchComboBox *target = nullptr;
};

TEST_F(ut_DSearchComboBox, setEditable)
{
    target->setEditable(true);
    ASSERT_EQ(target->isEditable(), true);
};

TEST_F(ut_DSearchComboBox, showPopup)
{
    target->addItem("test");
    target->showPopup();
};
