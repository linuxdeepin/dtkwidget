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

#include "dprintpreviewwidget.h"
DWIDGET_USE_NAMESPACE
class ut_DPrintPreviewWidget : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DPrintPreviewWidget();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DPrintPreviewWidget *target = nullptr;
};

TEST_F(ut_DPrintPreviewWidget, currentPage)
{
    target->currentPage();
};

TEST_F(ut_DPrintPreviewWidget, getColorMode)
{
    target->getColorMode();
};

TEST_F(ut_DPrintPreviewWidget, getScale)
{
    target->getScale();
};

TEST_F(ut_DPrintPreviewWidget, imposition)
{
    target->imposition();
};

TEST_F(ut_DPrintPreviewWidget, isAsynPreview)
{
    target->isAsynPreview();
};

TEST_F(ut_DPrintPreviewWidget, isPageByPage)
{
    target->isPageByPage(1, true);
};

TEST_F(ut_DPrintPreviewWidget, order)
{
    target->order();
};

TEST_F(ut_DPrintPreviewWidget, originPageCount)
{
    target->originPageCount();
};

TEST_F(ut_DPrintPreviewWidget, pageRangeMode)
{
    target->pageRangeMode();
};

TEST_F(ut_DPrintPreviewWidget, pagesCount)
{
    target->pagesCount();
};

TEST_F(ut_DPrintPreviewWidget, paintRequested)
{
    target->paintRequested(1, 1);
};

TEST_F(ut_DPrintPreviewWidget, print)
{
    target->print(true);
};

TEST_F(ut_DPrintPreviewWidget, printFromPath)
{
    target->printFromPath();
};

TEST_F(ut_DPrintPreviewWidget, printerColorModel)
{
    target->printerColorModel();
};

TEST_F(ut_DPrintPreviewWidget, refreshBegin)
{
    target->refreshBegin();
};

TEST_F(ut_DPrintPreviewWidget, refreshEnd)
{
    target->refreshEnd();
};

TEST_F(ut_DPrintPreviewWidget, reviewChange)
{
    target->reviewChange(true);
};

TEST_F(ut_DPrintPreviewWidget, setAsynPreview)
{
    target->setAsynPreview(1);
    ASSERT_EQ(target->asynPreview(), 1);
};

TEST_F(ut_DPrintPreviewWidget, setColorMode)
{
    target->setColorMode(1);
    ASSERT_EQ(target->colorMode(), 1);
};

TEST_F(ut_DPrintPreviewWidget, setConfidentialWaterMark)
{
    target->setConfidentialWaterMark();
};

TEST_F(ut_DPrintPreviewWidget, setCurrentPage)
{
    target->setCurrentPage(1);
    ASSERT_EQ(target->currentPage(), 1);
};

TEST_F(ut_DPrintPreviewWidget, setCurrentTargetPage)
{
    target->setCurrentTargetPage(1);
    ASSERT_EQ(target->currentTargetPage(), 1);
};

TEST_F(ut_DPrintPreviewWidget, setCustomWaterMark)
{
    target->setCustomWaterMark("setCustomWaterMark");
    ASSERT_EQ(target->customWaterMark(), "setCustomWaterMark");
};

TEST_F(ut_DPrintPreviewWidget, setDraftWaterMark)
{
    target->setDraftWaterMark();
};

TEST_F(ut_DPrintPreviewWidget, setImposition)
{
    target->setImposition(0);
    ASSERT_EQ(target->imposition(), 0);
};

TEST_F(ut_DPrintPreviewWidget, setOrder)
{
    target->setOrder(0);
    ASSERT_EQ(target->order(), 0);
};

TEST_F(ut_DPrintPreviewWidget, setOrientation)
{
    target->setOrientation(1);
    ASSERT_EQ(target->orientation(), 1);
};

TEST_F(ut_DPrintPreviewWidget, setPageRange)
{
    target->setPageRange(1);
    ASSERT_EQ(target->pageRange(), 1);
};

TEST_F(ut_DPrintPreviewWidget, setPageRange)
{
    target->setPageRange(1, 1);
};

TEST_F(ut_DPrintPreviewWidget, setPageRangeALL)
{
    target->setPageRangeALL();
};

TEST_F(ut_DPrintPreviewWidget, setPageRangeMode)
{
    target->setPageRangeMode(0);
    ASSERT_EQ(target->pageRangeMode(), 0);
};

TEST_F(ut_DPrintPreviewWidget, setPrintFromPath)
{
    target->setPrintFromPath("setPrintFromPath");
    ASSERT_EQ(target->printFromPath(), "setPrintFromPath");
};

TEST_F(ut_DPrintPreviewWidget, setPrintMode)
{
    target->setPrintMode(1);
    ASSERT_EQ(target->printMode(), 1);
};

TEST_F(ut_DPrintPreviewWidget, setReGenerate)
{
    target->setReGenerate(true);
    ASSERT_EQ(target->reGenerate(), true);
};

TEST_F(ut_DPrintPreviewWidget, setSampleWaterMark)
{
    target->setSampleWaterMark();
};

TEST_F(ut_DPrintPreviewWidget, setScale)
{
    target->setScale(0);
    ASSERT_EQ(target->scale(), 0);
};

TEST_F(ut_DPrintPreviewWidget, setTextWaterMark)
{
    target->setTextWaterMark("setTextWaterMark");
    ASSERT_EQ(target->textWaterMark(), "setTextWaterMark");
};

TEST_F(ut_DPrintPreviewWidget, setVisible)
{
    target->setVisible(true);
    ASSERT_EQ(target->visible(), true);
};

TEST_F(ut_DPrintPreviewWidget, setWaterMargImage)
{
    target->setWaterMargImage(0);
    ASSERT_EQ(target->waterMargImage(), 0);
};

TEST_F(ut_DPrintPreviewWidget, setWaterMarkColor)
{
    target->setWaterMarkColor(Qt::red);
    ASSERT_EQ(target->waterMarkColor(), Qt::red);
};

TEST_F(ut_DPrintPreviewWidget, setWaterMarkFont)
{
    target->setWaterMarkFont(0);
    ASSERT_EQ(target->waterMarkFont(), 0);
};

TEST_F(ut_DPrintPreviewWidget, setWaterMarkLayout)
{
    target->setWaterMarkLayout(1);
    ASSERT_EQ(target->waterMarkLayout(), 1);
};

TEST_F(ut_DPrintPreviewWidget, setWaterMarkOpacity)
{
    target->setWaterMarkOpacity(0);
    ASSERT_EQ(target->waterMarkOpacity(), 0);
};

TEST_F(ut_DPrintPreviewWidget, setWaterMarkRotate)
{
    target->setWaterMarkRotate(0);
    ASSERT_EQ(target->waterMarkRotate(), 0);
};

TEST_F(ut_DPrintPreviewWidget, setWaterMarkScale)
{
    target->setWaterMarkScale(0);
    ASSERT_EQ(target->waterMarkScale(), 0);
};

TEST_F(ut_DPrintPreviewWidget, setWaterMarkType)
{
    target->setWaterMarkType(1);
    ASSERT_EQ(target->waterMarkType(), 1);
};

TEST_F(ut_DPrintPreviewWidget, targetPageCount)
{
    target->targetPageCount(1);
};

TEST_F(ut_DPrintPreviewWidget, totalPages)
{
    target->totalPages(1);
};

TEST_F(ut_DPrintPreviewWidget, turnBack)
{
    target->turnBack();
};

TEST_F(ut_DPrintPreviewWidget, turnBegin)
{
    target->turnBegin();
};

TEST_F(ut_DPrintPreviewWidget, turnEnd)
{
    target->turnEnd();
};

TEST_F(ut_DPrintPreviewWidget, turnFront)
{
    target->turnFront();
};

TEST_F(ut_DPrintPreviewWidget, turnPageAble)
{
    target->turnPageAble();
};

TEST_F(ut_DPrintPreviewWidget, updatePreview)
{
    target->updatePreview();
};

TEST_F(ut_DPrintPreviewWidget, updateView)
{
    target->updateView();
};

TEST_F(ut_DPrintPreviewWidget, updateWaterMark)
{
    target->updateWaterMark();
};

class ut_DPrinter : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DPrinter();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DPrinter *target = nullptr;
};

TEST_F(ut_DPrinter, getPrinterPages)
{
    target->getPrinterPages();
};

TEST_F(ut_DPrinter, setPreviewMode)
{
    target->setPreviewMode(true);
    ASSERT_EQ(target->previewMode(), true);
};
