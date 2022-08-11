// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LISTVIEWEXAMPLE_H
#define LISTVIEWEXAMPLE_H

#include <QWidget>
#include <QLabel>

#include <dtkwidget_global.h>
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

class DHeaderViewExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DHeaderViewExample(QWidget *parent = nullptr);

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
