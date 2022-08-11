// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QLayout>

#include "dbackgroundgroup.h"
DWIDGET_USE_NAMESPACE
class ut_DBackgroundGroup : public testing::Test
{
protected:
    void SetUp() override
    {
        layout = new QHBoxLayout();
        target = new DBackgroundGroup(layout);
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DBackgroundGroup *target = nullptr;
    QLayout* layout = nullptr;
};

TEST_F(ut_DBackgroundGroup, setItemMargins)
{
    QMargins margin;
    margin.setLeft(10);
    target->setItemMargins(margin);
    ASSERT_EQ(target->itemMargins(), margin);
};

TEST_F(ut_DBackgroundGroup, setLayout)
{
    ASSERT_EQ(target->layout(), layout);
};

TEST_F(ut_DBackgroundGroup, setUseWidgetBackground)
{
    target->setUseWidgetBackground(true);
    ASSERT_EQ(target->useWidgetBackground(), true);
};
