// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
