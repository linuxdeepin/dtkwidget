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

#include "editexample.h"

DWIDGET_USE_NAMESPACE

EditExampleWindow::EditExampleWindow(QWidget *parent)
    : PageWindowInterface(parent)
{
    addExampleWindow(new DSearchEditExample(this));
    addExampleWindow(new DLineEditExample(this));
    addExampleWindow(new DIpv4LineEditExample(this));
    addExampleWindow(new DPasswordEditExample(this));
    addExampleWindow(new DFileChooserEditExample(this));
    addExampleWindow(new DSpinBoxExample(this));
    addExampleWindow(new DTextEditExample(this));
    addExampleWindow(new DCrumbTextFormatExample(this));
    addExampleWindow(new DKeySequenceEditExample(this));
}

DSearchEditExample::DSearchEditExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DSearchEditExample::getTitleName() const
{
    return "DSearchEdit";
}

QString DSearchEditExample::getDescriptionInfo() const
{
    return QString();
}

int DSearchEditExample::getFixedHeight() const
{
    return 400;
}

DLineEditExample::DLineEditExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DLineEditExample::getTitleName() const
{
    return "DLineEdit";
}

QString DLineEditExample::getDescriptionInfo() const
{
    return QString();
}

int DLineEditExample::getFixedHeight() const
{
    return 300;
}

DIpv4LineEditExample::DIpv4LineEditExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DIpv4LineEditExample::getTitleName() const
{
    return "DIpv4LineEdit";
}

QString DIpv4LineEditExample::getDescriptionInfo() const
{
    return QString();
}

int DIpv4LineEditExample::getFixedHeight() const
{
    return 300;
}

DPasswordEditExample::DPasswordEditExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DPasswordEditExample::getTitleName() const
{
    return "DPasswordEdit";
}

QString DPasswordEditExample::getDescriptionInfo() const
{
    return QString();
}

int DPasswordEditExample::getFixedHeight() const
{
    return 300;
}

DFileChooserEditExample::DFileChooserEditExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DFileChooserEditExample::getTitleName() const
{
    return "DFileChooserEdit";
}

QString DFileChooserEditExample::getDescriptionInfo() const
{
    return QString();
}

int DFileChooserEditExample::getFixedHeight() const
{
    return 300;
}

DSpinBoxExample::DSpinBoxExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DSpinBoxExample::getTitleName() const
{
    return "DSpinBox";
}

QString DSpinBoxExample::getDescriptionInfo() const
{
    return QString();
}

int DSpinBoxExample::getFixedHeight() const
{
    return 300;
}

DTextEditExample::DTextEditExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DTextEditExample::getTitleName() const
{
    return "DTextEdit";
}

QString DTextEditExample::getDescriptionInfo() const
{
    return QString();
}

int DTextEditExample::getFixedHeight() const
{
    return 300;
}

DCrumbTextFormatExample::DCrumbTextFormatExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DCrumbTextFormatExample::getTitleName() const
{
    return "DCrumbTextFormat";
}

QString DCrumbTextFormatExample::getDescriptionInfo() const
{
    return QString();
}

int DCrumbTextFormatExample::getFixedHeight() const
{
    return 300;
}

DKeySequenceEditExample::DKeySequenceEditExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DKeySequenceEditExample::getTitleName() const
{
    return "DKeySequenceEdit";
}

QString DKeySequenceEditExample::getDescriptionInfo() const
{
    return QString();
}

int DKeySequenceEditExample::getFixedHeight() const
{
    return 300;
}
