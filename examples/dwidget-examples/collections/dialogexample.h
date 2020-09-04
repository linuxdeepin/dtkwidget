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

#ifndef DIALOGEXAMPLE_H
#define DIALOGEXAMPLE_H

#include <QWidget>
#include <QLabel>

#include "dtkwidget_global.h"
#include "examplewindowinterface.h"
#include "pagewindowinterface.h"

class DialogExampleWindow : public PageWindowInterface
{
    Q_OBJECT

public:
    explicit DialogExampleWindow(QWidget *parent = nullptr);
};

class DDialogExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DDialogExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DFileDialogExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DFileDialogExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DMessageManagerExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DMessageManagerExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

#endif // DIALOGEXAMPLE_H
