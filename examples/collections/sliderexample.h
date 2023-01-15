// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SLIDEREXAMPLE_H
#define SLIDEREXAMPLE_H

#include <QWidget>
#include <QLabel>

#include <dtkwidget_global.h>
#include "examplewindowinterface.h"
#include "pagewindowinterface.h"

class SliderExampleWindow : public PageWindowInterface
{
    Q_OBJECT

public:
    explicit SliderExampleWindow(QWidget *parent = nullptr);
};

class DSliderExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DSliderExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

#endif // SLIDEREXAMPLE_H
