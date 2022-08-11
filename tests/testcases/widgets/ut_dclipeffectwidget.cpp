// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
