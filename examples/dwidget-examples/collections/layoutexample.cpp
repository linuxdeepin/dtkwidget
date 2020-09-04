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

#include "layoutexample.h"

DWIDGET_USE_NAMESPACE

LayoutExampleWindow::LayoutExampleWindow(QWidget *parent)
    : PageWindowInterface(parent)
{
    addExampleWindow(new DFrameExample(this));
    addExampleWindow(new DSplitterExample(this));
    addExampleWindow(new DVerticalLineExample(this));
    addExampleWindow(new DHorizontalLineExample(this));
}

DFrameExample::DFrameExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DFrameExample::getTitleName() const
{
    return "DFrame";
}

QString DFrameExample::getDescriptionInfo() const
{
    return QString();
}

int DFrameExample::getFixedHeight() const
{
    return 300;
}

DSplitterExample::DSplitterExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DSplitterExample::getTitleName() const
{
    return "DSplitter";
}

QString DSplitterExample::getDescriptionInfo() const
{
    return QString();
}

int DSplitterExample::getFixedHeight() const
{
    return 300;
}

DVerticalLineExample::DVerticalLineExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DVerticalLineExample::getTitleName() const
{
    return "DVerticalLine";
}

QString DVerticalLineExample::getDescriptionInfo() const
{
    return QString();
}

int DVerticalLineExample::getFixedHeight() const
{
    return 300;
}

DHorizontalLineExample::DHorizontalLineExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DHorizontalLineExample::getTitleName() const
{
    return "DHorizontalLine";
}

QString DHorizontalLineExample::getDescriptionInfo() const
{
    return QString();
}

int DHorizontalLineExample::getFixedHeight() const
{
    return 300;
}
