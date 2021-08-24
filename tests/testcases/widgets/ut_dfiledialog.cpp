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

#include "dfiledialog.h"
DWIDGET_USE_NAMESPACE
class ut_DFileDialog : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DFileDialog();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DFileDialog *target = nullptr;
};

TEST_F(ut_DFileDialog, addComboBox)
{
    target->addComboBox("addComboBox", {"box1", "box2"});
    ASSERT_EQ(target->property("_dtk_widget_custom_combobox_list").toStringList().size(), 1);

};

TEST_F(ut_DFileDialog, addComboBox2)
{
    target->addComboBox("addComboBox", {true, {"box1", "box2"}, "box2"});
    ASSERT_EQ(target->property("_dtk_widget_custom_combobox_list").toStringList().size(), 1);
};

TEST_F(ut_DFileDialog, addLineEdit)
{
    target->addLineEdit("addLineEdit");
    ASSERT_EQ(target->property("_dtk_widget_custom_lineedit_list").toStringList().size(), 1);
};

TEST_F(ut_DFileDialog, addLineEdit2)
{
    DFileDialog::DLineEditOptions options;
    options.defaultValue = "edit";
    target->addLineEdit("addLineEdit", options);
    ASSERT_EQ(target->property("_dtk_widget_custom_lineedit_list").toStringList().size(), 1);
};

TEST_F(ut_DFileDialog, setAllowMixedSelection)
{
    target->setAllowMixedSelection(true);
    ASSERT_EQ(target->property("_dtk_widget_filedialog_mixed_selection"), true);
};
