// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "examplewindowinterface.h"

ExampleWindowInterface::ExampleWindowInterface(QWidget *parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

ExampleWindowInterface::~ExampleWindowInterface()
{
    //
}
