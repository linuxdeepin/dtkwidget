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
#include <QSignalSpy>

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
    widget->show();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(widget, 100));

    QWidget *widget = new QWidget();
    titleBar->addWidget(widget);
    titleBar->removeWidget(widget);
}

TEST_F(ut_DTitlebar, testDTitlebarSetFixedHeight)
{
    int fixedHeight = 40;
    titleBar->setFixedHeight(fixedHeight);
    ASSERT_EQ(titleBar->height(), fixedHeight);
}

TEST_F(ut_DTitlebar, testDTitlebarBackgroundTransparent)
{
    bool bgTransparent = false;
    titleBar->setBackgroundTransparent(bgTransparent);
    ASSERT_EQ(titleBar->backgroundRole(), QPalette::Base);

    bgTransparent = true;
    titleBar->setBackgroundTransparent(bgTransparent);
    ASSERT_EQ(titleBar->backgroundRole(), QPalette::Window);
}

TEST_F(ut_DTitlebar, testDTitlebarsetSeparatorVisible)
{
    bool separatorVisible = true;
    titleBar->setSeparatorVisible(separatorVisible);
    ASSERT_EQ(titleBar->separatorVisible(), titleBar->isVisible() && true);

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
    ASSERT_EQ(titleBar->blurBackground(), true);
    blurBackground = false;
    titleBar->setBlurBackground(blurBackground);
    // TODO
}

TEST_F(ut_DTitlebar, setAutoHideOnFullscreen)
{
    titleBar->setAutoHideOnFullscreen(true);
    ASSERT_EQ(titleBar->autoHideOnFullscreen(), true);
}

TEST_F(ut_DTitlebar, setMenuDisabled)
{
    titleBar->setMenuDisabled(true);
    ASSERT_EQ(titleBar->menuIsDisabled(), true);
}

TEST_F(ut_DTitlebar, setQuitMenuDisabled)
{
    titleBar->setQuitMenuDisabled(true);
    ASSERT_EQ(titleBar->quitMenuIsDisabled(), true);
}

TEST_F(ut_DTitlebar, setQuitMenuVisible)
{
    titleBar->setQuitMenuVisible(true);
}

TEST_F(ut_DTitlebar, setSwitchThemeMenuVisible)
{
    titleBar->setSwitchThemeMenuVisible(true);
    ASSERT_EQ(titleBar->switchThemeMenuIsVisible(), titleBar->isVisible() && true);
}

TEST_F(ut_DTitlebar, setMenuVisible)
{
    titleBar->setMenuVisible(true);
    ASSERT_EQ(titleBar->menuIsVisible(), true);
}

TEST_F(ut_DTitlebar, setDisableFlags)
{
    widget->show();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(widget, 100));

    titleBar->setDisableFlags(Qt::Window);
    ASSERT_EQ(titleBar->disableFlags(), Qt::Window);
}

TEST_F(ut_DTitlebar, minimumSizeHint)
{
    ASSERT_EQ(titleBar->minimumSizeHint(), titleBar->sizeHint());
}

TEST_F(ut_DTitlebar, event)
{
    widget->showFullScreen();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(widget, 100));

    qRegisterMetaType<Qt::MouseButtons>();
    {
        QSignalSpy spy(titleBar, &DTitlebar::mousePressed);
        QTest::mousePress(titleBar, Qt::LeftButton);
        ASSERT_EQ(spy.count(), 1);
    }
    {
        QTest::mouseRelease(titleBar, Qt::LeftButton);
    }
    {
        QSignalSpy spy(titleBar, &DTitlebar::mouseMoving);
        QTest::mouseMove(titleBar);
    }
    {
        QSignalSpy spy(titleBar, &DTitlebar::doubleClicked);
        QTest::mouseDClick(titleBar, Qt::LeftButton);
        ASSERT_EQ(spy.count(), 1);
    }
}

TEST_F(ut_DTitlebar, toggleWindowState)
{
    titleBar->toggleWindowState();

    titleBar->buttonAreaWidth();

    titleBar->setFullScreenButtonVisible(true);
}

