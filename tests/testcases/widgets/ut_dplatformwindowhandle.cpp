/*
 * Copyright (C) 2021 ~ 2021 Uniontech Software Technology Co.,Ltd.
 *
 * Author:     Wang Peng <993381@qq.com>
 *
 * Maintainer: Wang Peng <wangpenga@uniontech.com>>
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

#include <DGuiApplicationHelper>
#include <DStyleOptionButton>
#include <QTest>
#include <dplatformwindowhandle.h>
#include <dplatformhandle.h>
#include <QPainterPath>

#include "dstyle.h"
DWIDGET_USE_NAMESPACE
class ut_DPlatformWindowHandle : public testing::Test
{
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(ut_DPlatformWindowHandle, test)
{
    QWidget widget;
    DPlatformWindowHandle handle(&widget);
    handle.enableDXcbForWindow(&widget);

    handle.enableDXcbForWindow(&widget, true);
    handle.enableDXcbForWindow(&widget, false);
    handle.isEnabledDXcb(&widget);

    DPlatformHandle::WMBlurArea area;
    area.x= 0;
    area.y= 0;
    area.width = 10;
    area.height= 10;
    area.xRadius = 0;
    area.yRaduis = 0;

    QVector<DPlatformHandle::WMBlurArea> vec;
    vec.push_back(area);

    handle.setWindowBlurAreaByWM(&widget, vec);

    QList<QPainterPath> list;
    list.push_back(QPainterPath(QPointF(10.0, 10.0)));
    handle.setWindowBlurAreaByWM(&widget, list);
}
