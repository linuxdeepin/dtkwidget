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

#include <QIcon>

#include "dwidgetutil.h"
DWIDGET_USE_NAMESPACE

class ut_DWidgetUtil : public testing::Test
{
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(ut_DWidgetUtil, dropShadow)
{
    QPixmap px(10, 10);
    px.fill(Qt::black);
    QImage image = dropShadow(px, 2, Qt::red);
    ASSERT_NE(image.pixel(4, 4), QColor(Qt::red).rgb());

};

TEST_F(ut_DWidgetUtil, getCircleIcon)
{
    QPixmap px(10, 10);
    QIcon icon = getCircleIcon(px, 2);
    ASSERT_EQ(icon.actualSize(QSize(100, 100)), QSize(2, 2));
};

TEST_F(ut_DWidgetUtil, getCircleIcon2)
{
    QIcon icon = QPixmap(10, 10);
    QIcon icon2 = getCircleIcon(icon, 2);
    ASSERT_EQ(icon2.actualSize(QSize(100, 100)), QSize(2, 2));
};
