/*
 * Copyright (C) 2020 ~ 2020 Deepin Technology Co., Ltd.
 *
 * Author:     sunkang <sunkang@uniontech.com>
 *
 * Maintainer: sunkang <sunkang@uniontech.com>
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

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>

#include <DDialog>
#include <DStyle>
#include <DPushButton>
#include <DMessageBox>
#include <DWarningButton>
#include <DSuggestButton>
#include <DFrame>

#include "windowexample.h"

DWIDGET_USE_NAMESPACE

WindowExampleWindow::WindowExampleWindow(QWidget *parent)
    : PageWindowInterface(parent)
{
    addExampleWindow(new DTitleBarExample(this));
    addExampleWindow(new DMainWindowExample(this));
    addExampleWindow(new DStatusBarExample(this));
    addExampleWindow(new DTabBarExample(this));
}

DTitleBarExample::DTitleBarExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DTitleBarExample::getTitleName() const
{
    return "DTitleBar";
}

QString DTitleBarExample::getDescriptionInfo() const
{
    return QString();
}

int DTitleBarExample::getFixedHeight() const
{
    return 300;
}

DMainWindowExample::DMainWindowExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DMainWindowExample::getTitleName() const
{
    return "DMainWindow";
}

QString DMainWindowExample::getDescriptionInfo() const
{
    return QString();
}

int DMainWindowExample::getFixedHeight() const
{
    return 300;
}

DStatusBarExample::DStatusBarExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DStatusBarExample::getTitleName() const
{
    return "DStatusBar";
}

QString DStatusBarExample::getDescriptionInfo() const
{
    return QString();
}

int DStatusBarExample::getFixedHeight() const
{
    return 300;
}

DTabBarExample::DTabBarExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DTabBarExample::getTitleName() const
{
    return "DTabBar";
}

QString DTabBarExample::getDescriptionInfo() const
{
    return QString();
}

int DTabBarExample::getFixedHeight() const
{
    return 300;
}
