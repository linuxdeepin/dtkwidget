// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "dbuttonbox.h"
DWIDGET_USE_NAMESPACE
class ut_DButtonBox : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DButtonBox();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DButtonBox *target = nullptr;
};

TEST_F(ut_DButtonBox, setButtonList)
{
    QList<DButtonBoxButton*> buttons;
    buttons.push_back(new DButtonBoxButton("DButtonBoxButton"));
    target->setButtonList(buttons, true);
    ASSERT_EQ(target->buttonList().size(), buttons.size());
    qDeleteAll(buttons);
};

TEST_F(ut_DButtonBox, setId)
{
    auto btn = new DButtonBoxButton("DButtonBoxButton");
    target->setId(btn, 1);

    ASSERT_EQ(target->id(btn), 1);
    ASSERT_EQ(target->button(1), btn);

    btn->deleteLater();
};

TEST_F(ut_DButtonBox, setOrientation)
{
    target->setOrientation(Qt::Horizontal);
    ASSERT_EQ(target->orientation(), Qt::Horizontal);
};

class ut_DButtonBoxButton : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DButtonBoxButton("DButtonBoxButton");
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DButtonBoxButton *target = nullptr;
};

TEST_F(ut_DButtonBoxButton, iconSize)
{
    target->setIconSize(QSize(10, 10));
    ASSERT_EQ(target->iconSize(), QSize(10, 10));
};

TEST_F(ut_DButtonBoxButton, minimumSizeHint)
{
    target->minimumSizeHint();
};

TEST_F(ut_DButtonBoxButton, setIcon)
{
    target->setIcon(QIcon::fromTheme("preferences-system"));
    ASSERT_EQ(target->icon().name(), QIcon::fromTheme("preferences-system").name());
};
