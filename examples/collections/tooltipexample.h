// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef TOOLTIPEXAMPLE_H
#define TOOLTIPEXAMPLE_H

#include <QWidget>
#include <QLabel>

#include <dtkwidget_global.h>
#include "examplewindowinterface.h"
#include "pagewindowinterface.h"

class ToolTipExampleWindow : public PageWindowInterface
{
    Q_OBJECT

public:
    explicit ToolTipExampleWindow(QWidget *parent = nullptr);
};

class DToolTipExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DToolTipExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DArrowRectangleExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DArrowRectangleExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

#endif // TOOLTIPEXAMPLE_H
