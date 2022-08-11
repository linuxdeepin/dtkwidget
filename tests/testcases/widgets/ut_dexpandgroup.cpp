// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "dexpandgroup.h"
DWIDGET_USE_NAMESPACE
class ut_DExpandGroup : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DExpandGroup();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DExpandGroup *target = nullptr;
};

TEST_F(ut_DExpandGroup, addExpand)
{
    target->addExpand(0, 1);
};

TEST_F(ut_DExpandGroup, checkedExpand)
{
    target->checkedExpand();
};

TEST_F(ut_DExpandGroup, checkedId)
{
    target->checkedId();
};

TEST_F(ut_DExpandGroup, expand)
{
    target->expand(1);
};

TEST_F(ut_DExpandGroup, expands)
{
    target->expands();
};

TEST_F(ut_DExpandGroup, id)
{
    target->id(0);
};

TEST_F(ut_DExpandGroup, removeExpand)
{
    target->removeExpand(0);
};

TEST_F(ut_DExpandGroup, setId)
{
    target->setId(0, 1);
};
