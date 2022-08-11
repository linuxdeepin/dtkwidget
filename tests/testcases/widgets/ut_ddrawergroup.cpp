// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QSignalSpy>
#include <DDrawer>

#include "ddrawergroup.h"
DWIDGET_USE_NAMESPACE
class ut_DDrawerGroup : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DDrawerGroup();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DDrawerGroup *target = nullptr;
};

TEST_F(ut_DDrawerGroup, addExpand)
{
    DDrawer *drawer = new DDrawer();
    target->addExpand(drawer, 1);
    ASSERT_EQ(target->expand(1), drawer);
    ASSERT_EQ(target->expands().size(), 1);
};

TEST_F(ut_DDrawerGroup, checkedExpand)
{
    DDrawer *drawer = new DDrawer();
    target->addExpand(drawer, 1);
    ASSERT_EQ(target->checkedExpand(), nullptr);
};

TEST_F(ut_DDrawerGroup, checkedId)
{
    DDrawer *drawer = new DDrawer();
    target->addExpand(drawer, 1);
    ASSERT_EQ(target->checkedId(), -1);
};

TEST_F(ut_DDrawerGroup, removeExpand)
{
    DDrawer *drawer = new DDrawer();
    target->addExpand(drawer, 1);
    target->removeExpand(drawer);

    ASSERT_EQ(target->expands().size(), 0);
};

TEST_F(ut_DDrawerGroup, setId)
{
    DDrawer *drawer = new DDrawer();
    target->addExpand(drawer, 1);
    target->setId(drawer, 2);
    ASSERT_EQ(target->id(drawer), 2);
};
