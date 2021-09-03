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

#include "dspinner.h"
#include "private/dslider_p.h"

DWIDGET_USE_NAMESPACE

class ut_DSpinner : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    QWidget *widget = nullptr;
    DSpinner *dspinner = nullptr;
};

void ut_DSpinner::SetUp()
{
    widget = new QWidget;
    dspinner = new DSpinner(widget);
    widget->resize(300, 100);
}

void ut_DSpinner::TearDown()
{
    if (widget) {
        delete widget;
        widget = nullptr;
    }
}

TEST_F(ut_DSpinner, testDSpinnerStart)
{
    dspinner->start();
    ASSERT_TRUE(dspinner->isPlaying());

    dspinner->stop();
    ASSERT_FALSE(dspinner->isPlaying());
}

TEST_F(ut_DSpinner, testDSpinnerPaintEvent)
{
    dspinner->setBackgroundColor(QColor(255,255,255));
    dspinner->paintEvent(nullptr);
    dspinner->start();
    ASSERT_TRUE(dspinner->isPlaying());
}
