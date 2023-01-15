// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LAYOUTEXAMPLE_H
#define LAYOUTEXAMPLE_H

#include <QWidget>
#include <QLabel>

#include <dtkwidget_global.h>
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
