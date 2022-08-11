// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EXAMPLEWINDOWINTERFACE_H
#define EXAMPLEWINDOWINTERFACE_H
#include <QWidget>

class ExampleWindowInterface : public QWidget
{
public:
    explicit ExampleWindowInterface(QWidget *parent);
    virtual ~ExampleWindowInterface();

public:
    virtual QString getTitleName() const = 0;
    virtual QString getDescriptionInfo() const = 0;
    virtual int getFixedHeight() const = 0;
};

#endif // EXAMPLEWINDOWINTERFACE_H
