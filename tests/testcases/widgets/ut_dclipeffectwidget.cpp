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

#include "dclipeffectwidget.h"
DWIDGET_USE_NAMESPACE
class ut_DClipEffectWidget : public testing::Test
{
protected:
    void SetUp() override
    {
        effectWidget = new QWidget();
        target = new DClipEffectWidget(effectWidget);
    }
    void TearDown() override
    {
        if (effectWidget) {
            delete effectWidget;
            effectWidget = nullptr;
        }
    }
    DClipEffectWidget *target = nullptr;
    QWidget* effectWidget = nullptr;
};

TEST_F(ut_DClipEffectWidget, setClipPath)
{
    QPainterPath paths;
    paths.addRect(QRect(0, 0, 10, 10));
    target->setClipPath(paths);
    ASSERT_EQ(target->clipPath(), paths);
}

TEST_F(ut_DClipEffectWidget, setMargins)
{
    target->setMargins(QMargins(1, 1, 1, 1));
    ASSERT_EQ(target->margins(), QMargins(1, 1, 1, 1));
}

TEST_F(ut_DClipEffectWidget, resizeEvent)
{
    target->setFixedSize(100, 10);
    ASSERT_EQ(target->size(), QSize(100, 10));
}

TEST_F(ut_DClipEffectWidget, showEvent)
{
    effectWidget->show();
    target->show();
    ASSERT_TRUE(target->isVisible());
}

TEST_F(ut_DClipEffectWidget, hideEvent)
{
    target->hide();
    ASSERT_TRUE(!target->isVisible());
}
