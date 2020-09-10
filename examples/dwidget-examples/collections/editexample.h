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

#ifndef EDITEXAMPLE_H
#define EDITEXAMPLE_H

#include "dtkwidget_global.h"
#include "examplewindowinterface.h"
#include "pagewindowinterface.h"

class QWidget;
class QLabel;

class EditExampleWindow : public PageWindowInterface
{
    Q_OBJECT

public:
    explicit EditExampleWindow(QWidget *parent = nullptr);
};

class DSearchEditExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DSearchEditExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DLineEditExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DLineEditExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DIpv4LineEditExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DIpv4LineEditExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DPasswordEditExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DPasswordEditExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DFileChooserEditExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DFileChooserEditExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DSpinBoxExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DSpinBoxExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DTextEditExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DTextEditExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DCrumbTextFormatExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DCrumbTextFormatExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DKeySequenceEditExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DKeySequenceEditExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

#endif // EDITEXAMPLE_H
