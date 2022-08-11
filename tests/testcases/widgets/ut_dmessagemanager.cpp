// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "dmessagemanager.h"
DWIDGET_USE_NAMESPACE
class ut_DMessageManager : public testing::Test
{
protected:
    void SetUp() override
    {
        target = DMessageManager::instance();
    }
    void TearDown() override
    {
    }
    DMessageManager *target = nullptr;
};

TEST_F(ut_DMessageManager, setContentMargens)
{
    QWidget *par = new QWidget();
    QWidget *content = new QWidget(par);
    content->setObjectName("_d_message_manager_content");
    QMargins margin(1, 1, 1, 1);
    target->setContentMargens(par, margin);
    ASSERT_EQ(content->contentsMargins(), margin);
};
