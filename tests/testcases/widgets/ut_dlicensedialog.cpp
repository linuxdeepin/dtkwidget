// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include "dlicensedialog.h"
DWIDGET_USE_NAMESPACE
class ut_DLicenseDialog : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DLicenseDialog();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DLicenseDialog *target = nullptr;

};

TEST_F(ut_DLicenseDialog, setFile)
{
    target->setFile(":/data/example-license.json");
    target->load();
    ASSERT_TRUE(target->isValid());
};



TEST_F(ut_DLicenseDialog, isValid)
{
    target->setFile("XXXXXXX");
    target->load();
    ASSERT_FALSE(target->isValid());
};
