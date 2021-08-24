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
