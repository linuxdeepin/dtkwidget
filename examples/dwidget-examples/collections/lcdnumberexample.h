// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LCDNUMBEREXAMPLE_H
#define LCDNUMBEREXAMPLE_H

#include <QWidget>
#include <QLabel>

#include <dtkwidget_global.h>
#include "examplewindowinterface.h"
#include "pagewindowinterface.h"

class LCDNumberExampleWindow : public PageWindowInterface
{
    Q_OBJECT

public:
    explicit LCDNumberExampleWindow(QWidget *parent = nullptr);
};

class DLCDNumberExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DLCDNumberExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

#endif // LCDNUMBEREXAMPLE_H
