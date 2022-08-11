// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
