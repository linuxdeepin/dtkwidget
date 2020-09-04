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

#include "tooltipexample.h"

DWIDGET_USE_NAMESPACE

ToolTipExampleWindow::ToolTipExampleWindow(QWidget *parent)
    : PageWindowInterface(parent)
{
    addExampleWindow(new DToolTipExample(this));
    addExampleWindow(new DArrowRectangleExample(this));
    addExampleWindow(new DSpinnerExample(this));
}

DToolTipExample::DToolTipExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DToolTipExample::getTitleName() const
{
    return "DToolTip";
}

QString DToolTipExample::getDescriptionInfo() const
{
    return QString();
}

int DToolTipExample::getFixedHeight() const
{
    return 300;
}

DArrowRectangleExample::DArrowRectangleExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DArrowRectangleExample::getTitleName() const
{
    return "DArrowRectangle";
}

QString DArrowRectangleExample::getDescriptionInfo() const
{
    return QString();
}

int DArrowRectangleExample::getFixedHeight() const
{
    return 300;
}

DSpinnerExample::DSpinnerExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DSpinnerExample::getTitleName() const
{
    return "DSpinner";
}

QString DSpinnerExample::getDescriptionInfo() const
{
    return QString();
}

int DSpinnerExample::getFixedHeight() const
{
    return 300;
}
