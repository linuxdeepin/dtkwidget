/*
* Copyright (C) 2021 ~ 2021 Uniontech Software Technology Co.,Ltd.
*
* Author:     Ye ShanShan <yeshanshan@uniontech.com>
*
* Maintainer: Ye ShanShan <yeshanshan@uniontech.com>>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
