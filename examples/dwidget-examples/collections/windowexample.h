// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef WINDOWEXAMPLE_H
#define WINDOWEXAMPLE_H

#include <QWidget>
#include <QLabel>

#include <dtkwidget_global.h>
#include "examplewindowinterface.h"
#include "pagewindowinterface.h"

class WindowExampleWindow : public PageWindowInterface
{
    Q_OBJECT

public:
    explicit WindowExampleWindow(QWidget *parent = nullptr);
};

class DTitleBarExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DTitleBarExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DMainWindowExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DMainWindowExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DStatusBarExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DStatusBarExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DSizegripExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DSizegripExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DTabBarExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DTabBarExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

#endif // WINDOWEXAMPLE_H
