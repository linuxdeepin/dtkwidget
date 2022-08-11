// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QTest>
#include <QDebug>

#include "DPageIndicator"

DWIDGET_USE_NAMESPACE

class ut_DPageIndicator : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    QWidget *widget = nullptr;
    DPageIndicator *indicator = nullptr;
};

void ut_DPageIndicator::SetUp()
{
    widget = new QWidget;
    indicator = new DPageIndicator(widget);
    widget->resize(300, 100);
}

void ut_DPageIndicator::TearDown()
{
    if (widget) {
        delete widget;
        widget = nullptr;
    }
}

TEST_F(ut_DPageIndicator, testDPageIndicatorPageCount)
{
    int pageCount = 10;
    indicator->setPageCount(pageCount);
    ASSERT_EQ(indicator->pageCount(), pageCount);
}

TEST_F(ut_DPageIndicator, testDPageIndicatorCurrentPage)
{
    int pageCount = 10;
    indicator->setPageCount(pageCount);
    for (int i = 0; i < indicator->pageCount(); ++i) {
        indicator->setCurrentPage(i);
        ASSERT_EQ(indicator->currentPageIndex(), i);
    }
}

TEST_F(ut_DPageIndicator, testDPageIndicatorNextAndPrevious)
{
    int pageCount = 10;
    indicator->setPageCount(pageCount);

    indicator->setCurrentPage(5);
    indicator->previousPage();
    ASSERT_EQ(indicator->currentPageIndex(), 4);

    indicator->setCurrentPage(5);
    indicator->nextPage();
    ASSERT_EQ(indicator->currentPageIndex(), 6);
}
