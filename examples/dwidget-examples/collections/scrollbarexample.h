// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SCROLLBAREXAMPLE_H
#define SCROLLBAREXAMPLE_H

#include <QWidget>
#include <QLabel>

#include <dtkwidget_global.h>
#include "examplewindowinterface.h"
#include "pagewindowinterface.h"

class ScrollBarExampleWindow : public PageWindowInterface
{
    Q_OBJECT

public:
    explicit ScrollBarExampleWindow(QWidget *parent = nullptr);
};

class DScrollBarExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DScrollBarExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

#endif // SCROLLBAREXAMPLE_H
