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
    target->paintRequested(1, 1);
};

TEST_F(ut_DPrintPreviewDialog, printFromPath)
{
    target->printFromPath();
};

TEST_F(ut_DPrintPreviewDialog, setAsynPreview)
{
    target->setAsynPreview(1);
    ASSERT_EQ(target->asynPreview(), 1);
};

TEST_F(ut_DPrintPreviewDialog, setDocName)
{
    target->setDocName("setDocName");
    ASSERT_EQ(target->docName(), "setDocName");
};

TEST_F(ut_DPrintPreviewDialog, setPrintFromPath)
{
    target->setPrintFromPath("setPrintFromPath");
    ASSERT_EQ(target->printFromPath(), "setPrintFromPath");
};
