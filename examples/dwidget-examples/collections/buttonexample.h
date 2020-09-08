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

#ifndef BUTTONEXAMPLE_H
#define BUTTONEXAMPLE_H
#include <QWidget>
#include <QLabel>

#include "dtkwidget_global.h"
#include "examplewindowinterface.h"
#include "pagewindowinterface.h"

class ButtonExampleWindow : public PageWindowInterface
{
    Q_OBJECT

public:
    explicit ButtonExampleWindow(QWidget *parent = nullptr);
};

class DPushButtonExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DPushButtonExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DWarningButtonExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DWarningButtonExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DSuggestButtonExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DSuggestButtonExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DToolButtonExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DToolButtonExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DIconButtonExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DIconButtonExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DButtonBoxExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DButtonBoxExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DFloatingButtonExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DFloatingButtonExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DSwitchButtonExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DSwitchButtonExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DRadioButtonExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DRadioButtonExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DCheckButtonExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DCheckButtonExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DComboBoxExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DComboBoxExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DFontComboBoxExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DFontComboBoxExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

#endif // BUTTONEXAMPLE_H
