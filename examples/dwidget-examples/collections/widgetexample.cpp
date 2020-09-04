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

#include "widgetexample.h"

DWIDGET_USE_NAMESPACE

WidgetExampleWindow::WidgetExampleWindow(QWidget *parent)
    : PageWindowInterface(parent)
{
    addExampleWindow(new DCalendarWidgetExample(this));
    addExampleWindow(new DTableWidgetExample(this));
}

DCalendarWidgetExample::DCalendarWidgetExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DCalendarWidgetExample::getTitleName() const
{
    return "DCalendarWidget";
}

QString DCalendarWidgetExample::getDescriptionInfo() const
{
    return QString();
}

int DCalendarWidgetExample::getFixedHeight() const
{
    return 300;
}

DTableWidgetExample::DTableWidgetExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DTableWidgetExample::getTitleName() const
{
    return "DTableWidget";
}

QString DTableWidgetExample::getDescriptionInfo() const
{
    return QString();
}

int DTableWidgetExample::getFixedHeight() const
{
    return 300;
}
