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

#include "dheaderline.h"
DWIDGET_USE_NAMESPACE
class ut_DHeaderLine : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DHeaderLine();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DHeaderLine *target = nullptr;
};

TEST_F(ut_DHeaderLine, setContent)
{
    QWidget *content = new QWidget();
    target->setContent(content);
    ASSERT_EQ(target->rightLayout()->count(), 1);
};

TEST_F(ut_DHeaderLine, setTitle)
{
    target->setTitle("setTitle");
    ASSERT_EQ(target->title(), "setTitle");
};
