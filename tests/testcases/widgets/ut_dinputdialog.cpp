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

#include "dinputdialog.h"
DWIDGET_USE_NAMESPACE
class ut_DInputDialog : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DInputDialog();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DInputDialog *target = nullptr;
};

TEST_F(ut_DInputDialog, setCancelButtonText)
{
    target->setCancelButtonText("setCancelButtonText");
    ASSERT_EQ(target->cancelButtonText(), "setCancelButtonText");
};

TEST_F(ut_DInputDialog, setComboBoxCurrentIndex)
{
    target->setComboBoxItems({"setComboBoxItems", "setComboBoxItems2"});
    target->setComboBoxCurrentIndex(1);
    ASSERT_EQ(target->comboBoxCurrentIndex(), 1);
};

TEST_F(ut_DInputDialog, setComboBoxEditable)
{
    target->setComboBoxEditable(true);
    ASSERT_EQ(target->isComboBoxEditable(), true);
};

TEST_F(ut_DInputDialog, setComboBoxItems)
{
    target->setComboBoxItems({"setComboBoxItems"});
    ASSERT_EQ(target->comboBoxItems().size(), QStringList{"setComboBoxItems"}.size());
};

TEST_F(ut_DInputDialog, setDoubleDecimals)
{
    target->setDoubleDecimals(1);
    ASSERT_EQ(target->doubleDecimals(), 1);
};

TEST_F(ut_DInputDialog, setDoubleMaximum)
{
    target->setDoubleMaximum(0);
    ASSERT_EQ(target->doubleMaximum(), 0);
};

TEST_F(ut_DInputDialog, setDoubleMinimum)
{
    target->setDoubleMinimum(0);
    ASSERT_EQ(target->doubleMinimum(), 0);
};

TEST_F(ut_DInputDialog, setDoubleRange)
{
    target->setDoubleRange(0, 0);
};

TEST_F(ut_DInputDialog, setDoubleValue)
{
    target->setDoubleValue(0);
    ASSERT_EQ(target->doubleValue(), 0);
};

TEST_F(ut_DInputDialog, setInputMode)
{
    target->setInputMode(DInputDialog::TextInput);
    ASSERT_EQ(target->inputMode(), DInputDialog::TextInput);
};

TEST_F(ut_DInputDialog, setIntMaximum)
{
    target->setIntMaximum(1);
    ASSERT_EQ(target->intMaximum(), 1);
};

TEST_F(ut_DInputDialog, setIntMinimum)
{
    target->setIntMinimum(1);
    ASSERT_EQ(target->intMinimum(), 1);
};

TEST_F(ut_DInputDialog, setIntRange)
{
    target->setIntRange(1, 1);
};

TEST_F(ut_DInputDialog, setIntStep)
{
    target->setIntStep(1);
    ASSERT_EQ(target->intStep(), 1);
};

TEST_F(ut_DInputDialog, setIntValue)
{
    target->setIntValue(1);
    ASSERT_EQ(target->intValue(), 1);
};

TEST_F(ut_DInputDialog, setOkButtonEnabled)
{
    target->setOkButtonEnabled(true);
    ASSERT_EQ(target->okButtonIsEnabled(), true);
};

TEST_F(ut_DInputDialog, setOkButtonText)
{
    target->setOkButtonText("setOkButtonText");
    ASSERT_EQ(target->okButtonText(), "setOkButtonText");
};

TEST_F(ut_DInputDialog, setTextAlert)
{
    target->setTextAlert(true);
    ASSERT_EQ(target->isTextAlert(), true);
};

TEST_F(ut_DInputDialog, setTextEchoMode)
{
    target->setTextEchoMode(QLineEdit::Normal);
    ASSERT_EQ(target->textEchoMode(), QLineEdit::Normal);
};

TEST_F(ut_DInputDialog, setTextValue)
{
    target->setTextValue("setTextValue");
    ASSERT_EQ(target->textValue(), "setTextValue");
};
