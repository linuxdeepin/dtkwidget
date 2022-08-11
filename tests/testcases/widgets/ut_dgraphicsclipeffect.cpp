// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "dgraphicsclipeffect.h"
DWIDGET_USE_NAMESPACE
class ut_DGraphicsClipEffect : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DGraphicsClipEffect();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DGraphicsClipEffect *target = nullptr;
};

TEST_F(ut_DGraphicsClipEffect, setClipPath)
{
    QPainterPath path;
    path.addRect(QRect(1, 1, 1, 1));
    target->setClipPath(path);
    ASSERT_EQ(target->clipPath(), path);
};

TEST_F(ut_DGraphicsClipEffect, setMargins)
{
    QMargins margin(1, 1, 1, 1);
    target->setMargins(margin);
    ASSERT_EQ(target->margins(), margin);
};
