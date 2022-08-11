// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QTest>
#include <QDebug>

#include "DTitlebar"

DWIDGET_USE_NAMESPACE

class ut_DTitlebar : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    QWidget *widget = nullptr;
    DTitlebar *titleBar = nullptr;
};

void ut_DTitlebar::SetUp()
{
    widget = new QWidget;
    titleBar = new DTitlebar(widget);
    widget->resize(300, 200);
}

void ut_DTitlebar::TearDown()
{
    widget->deleteLater();
}

TEST_F(ut_DTitlebar, testDTitlebarMenu)
{
    QIcon icon(QIcon::fromTheme("preferences-system"));
    titleBar->setIcon(icon);

    QMenu menu;
    QString action("testDTitlebarMenu");
    menu.addAction(action);
    titleBar->setMenu(&menu);
    ASSERT_EQ(titleBar->menu()->actions()[0]->text(), action);
}

TEST_F(ut_DTitlebar, testDTitlebarCustomWidget)
{
    QWidget w;
    titleBar->setCustomWidget(&w);
    ASSERT_EQ(titleBar->customWidget(), &w);
}

TEST_F(ut_DTitlebar, testDTitlebarAddRemoveWidget)
{
    return;
    QWidget w1, w2, w3;
    titleBar->addWidget(&w1);
    titleBar->addWidget(&w2);
    titleBar->addWidget(&w3);
    titleBar->removeWidget(&w1);
    titleBar->removeWidget(&w2);
    titleBar->removeWidget(&w3);
    // TODO
}

TEST_F(ut_DTitlebar, testDTitlebarSetFixedHeight)
{
    int fixedHeight = 40;
    titleBar->setFixedHeight(fixedHeight);
    ASSERT_EQ(titleBar->height(), fixedHeight);
}

TEST_F(ut_DTitlebar, testDTitlebarBackgroundTransparent)
{
    return;
    bool bgTransparent = false;
    titleBar->setBackgroundTransparent(bgTransparent);
    ASSERT_NE(titleBar->backgroundRole(), QPalette::Base);

    bgTransparent = true;
    titleBar->setBackgroundTransparent(bgTransparent);
    ASSERT_EQ(titleBar->backgroundRole(), QPalette::NoRole);
}

TEST_F(ut_DTitlebar, testDTitlebarsetSeparatorVisible)
{
    bool separatorVisible = true;
    titleBar->setSeparatorVisible(separatorVisible);

    separatorVisible = false;
    titleBar->setSeparatorVisible(separatorVisible);
    // TODO
}

TEST_F(ut_DTitlebar, testDTitlebarSetTitle)
{
    QString title("testDTitlebarSetTitle");
    titleBar->setTitle(title);
    // TODO
}

TEST_F(ut_DTitlebar, testDTitlebarSetIcon)
{
    QIcon icon(QIcon::fromTheme("preferences-system"));
    titleBar->setIcon(icon);
    // TODO
}

TEST_F(ut_DTitlebar, testDTitlebarSetBlurBackground)
{
    bool blurBackground = true;
    titleBar->setBlurBackground(blurBackground);
    blurBackground = false;
    titleBar->setBlurBackground(blurBackground);
    // TODO
}

TEST_F(ut_DTitlebar, testDTitlebarButtonAreaWidth)
{
    return;
    bool blurBackground = true;
    titleBar->setBlurBackground(blurBackground);
    blurBackground = false;
    titleBar->setBlurBackground(blurBackground);
    // TODO
}
