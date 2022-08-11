// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DIALOGEXAMPLE_H
#define DIALOGEXAMPLE_H

#include <QWidget>
#include <QLabel>

#include <dtkwidget_global.h>
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
