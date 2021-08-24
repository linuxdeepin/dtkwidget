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
