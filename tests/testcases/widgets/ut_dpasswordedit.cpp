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

#include "dpasswordedit.h"
DWIDGET_USE_NAMESPACE
class ut_DPasswordEdit : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DPasswordEdit();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DPasswordEdit *target = nullptr;
};

TEST_F(ut_DPasswordEdit, setEchoButtonIsVisible)
{
    target->setEchoButtonIsVisible(true);
    ASSERT_EQ(target->echoButtonIsVisible(), target->isVisible() && target->echoButtonIsVisible());
};

TEST_F(ut_DPasswordEdit, setEchoMode)
{
    target->setEchoMode(QLineEdit::Normal);
    ASSERT_EQ(target->echoMode(), QLineEdit::Normal);
    ASSERT_EQ(target->isEchoMode(), true);
};

TEST_F(ut_DPasswordEdit, setCopyEnabled)
{
    target->setCopyEnabled(true);
    ASSERT_TRUE(target->copyEnabled());

    target->setCopyEnabled(false);
    ASSERT_FALSE(target->copyEnabled());
}

TEST_F(ut_DPasswordEdit, setCutEnabled)
{
    target->setCutEnabled(true);
    ASSERT_TRUE(target->cutEnabled());

    target->setCutEnabled(false);
    ASSERT_FALSE(target->cutEnabled());
}

