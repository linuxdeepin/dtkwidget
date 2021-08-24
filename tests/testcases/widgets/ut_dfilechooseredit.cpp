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

#include "dfilechooseredit.h"
DWIDGET_USE_NAMESPACE
class ut_DFileChooserEdit : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DFileChooserEdit();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DFileChooserEdit *target = nullptr;
};

TEST_F(ut_DFileChooserEdit, setDialogDisplayPosition)
{
    target->setDialogDisplayPosition(DFileChooserEdit::FollowParentWindow);
    ASSERT_EQ(target->dialogDisplayPosition(), DFileChooserEdit::FollowParentWindow);
};

TEST_F(ut_DFileChooserEdit, setDirectoryUrl)
{
    target->setDirectoryUrl(QUrl("file:///"));
    ASSERT_EQ(target->directoryUrl(), QUrl("file:///"));
};

TEST_F(ut_DFileChooserEdit, setFileDialog)
{
    QFileDialog *dialog = new QFileDialog();
    target->setFileDialog(dialog);
    ASSERT_EQ(target->fileDialog(), dialog);
};

TEST_F(ut_DFileChooserEdit, setFileMode)
{
    target->setFileMode(QFileDialog::AnyFile);
    ASSERT_EQ(target->fileMode(), QFileDialog::AnyFile);
};

TEST_F(ut_DFileChooserEdit, setNameFilters)
{
    target->setNameFilters({"setNameFilters"});
    ASSERT_EQ(target->nameFilters().size(), 1);
};
