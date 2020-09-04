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

#include "listviewexample.h"

DWIDGET_USE_NAMESPACE

ListViewExampleWindow::ListViewExampleWindow(QWidget *parent)
    : PageWindowInterface(parent)
{
    addExampleWindow(new DBackgroundGroupExample(this));
    addExampleWindow(new DListViewExample(this));
    addExampleWindow(new DGroupBoxExample(this));
    addExampleWindow(new DTreeViewExample(this));
    addExampleWindow(new DColumnViewExample(this));
}

DBackgroundGroupExample::DBackgroundGroupExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DBackgroundGroupExample::getTitleName() const
{
    return "DBackgroundGroup";
}

QString DBackgroundGroupExample::getDescriptionInfo() const
{
    return QString();
}

int DBackgroundGroupExample::getFixedHeight() const
{
    return 300;
}

DListViewExample::DListViewExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DListViewExample::getTitleName() const
{
    return "DListView";
}

QString DListViewExample::getDescriptionInfo() const
{
    return QString();
}

int DListViewExample::getFixedHeight() const
{
    return 300;
}

DGroupBoxExample::DGroupBoxExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DGroupBoxExample::getTitleName() const
{
    return "DGroupBox";
}

QString DGroupBoxExample::getDescriptionInfo() const
{
    return QString();
}

int DGroupBoxExample::getFixedHeight() const
{
    return 300;
}

DTreeViewExample::DTreeViewExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DTreeViewExample::getTitleName() const
{
    return "DTreeView";
}

QString DTreeViewExample::getDescriptionInfo() const
{
    return QString();
}

int DTreeViewExample::getFixedHeight() const
{
    return 300;
}

DColumnViewExample::DColumnViewExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DColumnViewExample::getTitleName() const
{
    return "DColumnView";
}

QString DColumnViewExample::getDescriptionInfo() const
{
    return QString();
}

int DColumnViewExample::getFixedHeight() const
{
    return 300;
}
