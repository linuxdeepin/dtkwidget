// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QTest>
#include <QDebug>

#include "darrowlineexpand.h"

#include <QLabel>

DWIDGET_USE_NAMESPACE

class ut_DArrowLineExpand : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    QWidget *widget = nullptr;
    DArrowLineExpand *expand = nullptr;
};

void ut_DArrowLineExpand::SetUp()
{
    widget = new QWidget;
    widget->resize(300, 200);
    expand = new DArrowLineExpand(widget);
    expand->resize(100, 30);
}

void ut_DArrowLineExpand::TearDown()
{
    widget->deleteLater();
}

TEST_F(ut_DArrowLineExpand, ut_DArrowLineExpandSetTitle)
{
    const QString title("xxxxxxxx");
    expand->setTitle(title);
    // TODO
}

TEST_F(ut_DArrowLineExpand, ut_DArrowLineExpandSetExpand)
{
    bool expanded = false;
    expand->setExpand(expanded);
    expanded = true;
    expand->setExpand(expanded);
    // TODO
}

