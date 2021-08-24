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

#include "dmessagemanager.h"
DWIDGET_USE_NAMESPACE
class ut_DMessageManager : public testing::Test
{
protected:
    void SetUp() override
    {
        target = DMessageManager::instance();
    }
    void TearDown() override
    {
    }
    DMessageManager *target = nullptr;
};

TEST_F(ut_DMessageManager, setContentMargens)
{
    QWidget *par = new QWidget();
    QWidget *content = new QWidget(par);
    content->setObjectName("_d_message_manager_content");
    QMargins margin(1, 1, 1, 1);
    target->setContentMargens(par, margin);
    ASSERT_EQ(content->contentsMargins(), margin);
};
