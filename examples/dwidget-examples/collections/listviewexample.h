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

#ifndef LISTVIEWEXAMPLE_H
#define LISTVIEWEXAMPLE_H

#include <QWidget>
#include <QLabel>

#include "dtkwidget_global.h"
#include "examplewindowinterface.h"
#include "pagewindowinterface.h"

class ListViewExampleWindow : public PageWindowInterface
{
    Q_OBJECT

public:
    explicit ListViewExampleWindow(QWidget *parent = nullptr);
};

class DBackgroundGroupExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DBackgroundGroupExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DListViewExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DListViewExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DGroupBoxExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DGroupBoxExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DTreeViewExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DTreeViewExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DColumnViewExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DColumnViewExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

#endif // LISTVIEWEXAMPLE_H
