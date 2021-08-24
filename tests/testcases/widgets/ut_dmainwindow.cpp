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
#include <qglobal.h>

#include "dthememanager.h"
#include "private/dmainwindow_p.h"

DWIDGET_USE_NAMESPACE

class ut_DMainWindow : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    DMainWindow *window = nullptr;
};

void ut_DMainWindow::SetUp()
{
    window = new DMainWindow;
    window->resize(300, 200);
}

void ut_DMainWindow::TearDown()
{
    window->deleteLater();
}

TEST_F(ut_DMainWindow, testDMainWindowFrontEnd)
{

    // 测试边框
    QRect rect1(0, 0, 300, 200);
    QRegion r1(rect1);
    window->setFrameMask(r1);

    // 测试 titlebar 阴影效果
    bool isTitlebarShadowEnabled = true;
    window->setTitlebarShadowEnabled(isTitlebarShadowEnabled);
    ASSERT_TRUE(window->d_func()->titleShadow != nullptr);

    if (qgetenv("QT_QPA_PLATFORM") == QByteArray("offscreen"))
        return;

    window->setBorderColor(QColor(Qt::red));
    ASSERT_EQ(window->borderColor(), QColor(Qt::red));

    // 测试裁剪路径
    QPainterPath clipPath;
    clipPath.addRoundedRect(QRect(QPoint(0, 0), QSize(300, 200)), 0, 4);
    window->setClipPath(clipPath);
    ASSERT_TRUE(window->clipPath() == clipPath);

    // 测试窗口圆角
    window->setWindowRadius(150);
    ASSERT_TRUE(window->windowRadius() == 150);

    // 测试窗口边框宽度
    window->setBorderWidth(20);
    ASSERT_TRUE(window->borderWidth() == 20);

    // 测试窗口阴影偏移
    QPoint p(10, 10);
    window->setShadowOffset(p);
    ASSERT_TRUE(window->shadowOffset() == p);
}

TEST_F(ut_DMainWindow, testDMainWindowBackEnd)
{
    if (qgetenv("QT_QPA_PLATFORM") == QByteArray("offscreen"))
        return;

    // 测试模糊窗口
    bool isEnableBlurWindow = true;
    window->setEnableBlurWindow(isEnableBlurWindow);
    ASSERT_TRUE(window->enableBlurWindow());

    // 测试鼠标移动功能
    bool isEnableSystemMove = true;
    window->setEnableSystemMove(isEnableSystemMove);
    ASSERT_TRUE(window->enableSystemMove());

    // 测试窗口缩放
    bool isEnableSystemResize = true;
    window->setEnableSystemResize(isEnableSystemResize);
    ASSERT_TRUE(window->enableSystemResize());

    // 测试半透明的背景
    bool isTranslucentBackground = true;
    window->setTranslucentBackground(isTranslucentBackground);
    ASSERT_TRUE(window->translucentBackground());

    // 测试裁剪路径的输入 mask
    bool isAutoInputMaskByClipPath = true;
    window->setAutoInputMaskByClipPath(isAutoInputMaskByClipPath);
    ASSERT_TRUE(window->autoInputMaskByClipPath());
}
