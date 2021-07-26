/*
 * Copyright (C) 2021 ~ 2021 Deepin Technology Co., Ltd.
 *
 * Author:     Wang Peng <wangpenga@uniontech.com>
 *
 * Maintainer: Wang Peng <wangpenga@uniontech.com>
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
