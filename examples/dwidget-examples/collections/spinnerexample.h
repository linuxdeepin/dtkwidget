// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SPINNEREXAMPLE_H
#define SPINNEREXAMPLE_H

#include <QWidget>
#include <QLabel>

#include <dtkwidget_global.h>
#include "examplewindowinterface.h"
#include "pagewindowinterface.h"

class SpinnerExampleWindow : public PageWindowInterface
{
    Q_OBJECT

public:
    explicit SpinnerExampleWindow(QWidget *parent = nullptr);
};

class DSpinnerExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DSpinnerExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

#endif // SPINNEREXAMPLE_H
