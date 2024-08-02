// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef PROGRESSBAREXAMPLE_H
#define PROGRESSBAREXAMPLE_H

#include <dtkwidget_global.h>
#include "examplewindowinterface.h"
#include "pagewindowinterface.h"

class ProgressBarExampleWindow : public PageWindowInterface
{
    Q_OBJECT

public:
    explicit ProgressBarExampleWindow(QWidget *parent = nullptr);
};

class DProgressBarExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DProgressBarExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DWaterProgressExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DWaterProgressExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DColoredProgressBarExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DColoredProgressBarExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DIndeterminateProgressBarExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DIndeterminateProgressBarExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

#endif // PROGRESSBAREXAMPLE_H
