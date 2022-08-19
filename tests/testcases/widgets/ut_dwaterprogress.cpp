// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QTest>
#include <QDebug>

#include "dwaterprogress.h"

DWIDGET_USE_NAMESPACE

class ut_DWaterProgress : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    DWaterProgress *progress = nullptr;
    QWidget *widget = nullptr;
};

void ut_DWaterProgress::SetUp()
{
    widget = new QWidget;
    progress = new DWaterProgress(widget);
    widget->resize(300, 200);
}

void ut_DWaterProgress::TearDown()
{
    widget->deleteLater();
}

TEST_F(ut_DWaterProgress, testDwaterProress)
{
    QObject::connect(progress, &DWaterProgress::valueChanged, widget, [this](){
        ASSERT_TRUE(progress->value() > -1 && progress->value() < 101);
    });

    progress->start();
    for (int var = 0; var < 101; ++var) {
        progress->setValue(var);
        ASSERT_TRUE(progress->value() == var);
    }
    progress->stop();
}

TEST_F(ut_DWaterProgress, testDwaterProressSetValue)
{
    progress->setValue(50);
    ASSERT_TRUE(progress->value() == 50);
}

// TODO break
TEST_F(ut_DWaterProgress, testDwaterProressPaintEvent)
{
    widget->show();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(widget, 100));
}
// break
TEST_F(ut_DWaterProgress, testDwaterProressChangeEvent)
{
    widget->show();
    progress->setPalette(QPalette(QColor(255,255,255)));
    ASSERT_TRUE(QTest::qWaitForWindowExposed(widget, 100));
}
