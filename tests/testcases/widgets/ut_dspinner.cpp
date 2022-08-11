// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
