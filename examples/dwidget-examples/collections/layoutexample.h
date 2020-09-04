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

#ifndef LAYOUTEXAMPLE_H
#define LAYOUTEXAMPLE_H

#include <QWidget>
#include <QLabel>

#include "dtkwidget_global.h"
#include "examplewindowinterface.h"
#include "pagewindowinterface.h"

class LayoutExampleWindow : public PageWindowInterface
{
    Q_OBJECT

public:
    explicit LayoutExampleWindow(QWidget *parent = nullptr);
};

class DFrameExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DFrameExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DSplitterExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DSplitterExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DVerticalLineExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DVerticalLineExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DHorizontalLineExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DHorizontalLineExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

#endif // LAYOUTEXAMPLE_H
