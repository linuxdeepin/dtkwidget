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

TEST_F(ut_DWaterProgress, testDwaterProressPaintEvent)
{
    widget->show();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(widget, 100));
}

TEST_F(ut_DWaterProgress, testDwaterProressChangeEvent)
{
    widget->show();
    progress->setPalette(QPalette(QColor(255,255,255)));
    ASSERT_TRUE(QTest::qWaitForWindowExposed(widget, 100));
}
