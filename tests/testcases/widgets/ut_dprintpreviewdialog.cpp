// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "dprintpreviewdialog.h"
DWIDGET_USE_NAMESPACE
class ut_DPrintPreviewDialog : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DPrintPreviewDialog();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DPrintPreviewDialog *target = nullptr;
};

TEST_F(ut_DPrintPreviewDialog, docName)
{
    target->docName();
};

TEST_F(ut_DPrintPreviewDialog, isAsynPreview)
{
    target->isAsynPreview();
};

TEST_F(ut_DPrintPreviewDialog, paintRequested)
{
//target->paintRequested(1, 1);
};

TEST_F(ut_DPrintPreviewDialog, printFromPath)
{
    target->printFromPath();
};

TEST_F(ut_DPrintPreviewDialog, setAsynPreview)
{
    //target->setAsynPreview(1);
    //ASSERT_EQ(target->asynPreview(), 1);
};

TEST_F(ut_DPrintPreviewDialog, setDocName)
{
    target->setDocName("setDocName");
    ASSERT_EQ(target->docName(), "setDocName");
};

//TEST_F(ut_DPrintPreviewDialog, setPrintFromPath)
//{
//    target->setPrintFromPath("setPrintFromPath");
//    ASSERT_EQ(target->printFromPath(), "setPrintFromPath");
//};
