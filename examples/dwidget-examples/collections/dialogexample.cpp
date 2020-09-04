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

#include "dialogexample.h"

DWIDGET_USE_NAMESPACE

DialogExampleWindow::DialogExampleWindow(QWidget *parent)
    : PageWindowInterface(parent)
{
    addExampleWindow(new DDialogExample(this));
    addExampleWindow(new DFileDialogExample(this));
    addExampleWindow(new DMessageManagerExample(this));
}

DDialogExample::DDialogExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DDialogExample::getTitleName() const
{
    return "DDialog";
}

QString DDialogExample::getDescriptionInfo() const
{
    return QString();
}

int DDialogExample::getFixedHeight() const
{
    return 300;
}

DFileDialogExample::DFileDialogExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DFileDialogExample::getTitleName() const
{
    return "DFileDialog";
}

QString DFileDialogExample::getDescriptionInfo() const
{
    return QString();
}

int DFileDialogExample::getFixedHeight() const
{
    return 300;
}

DMessageManagerExample::DMessageManagerExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DMessageManagerExample::getTitleName() const
{
    return "DMessageManager";
}

QString DMessageManagerExample::getDescriptionInfo() const
{
    return QString();
}

int DMessageManagerExample::getFixedHeight() const
{
    return 300;
}
