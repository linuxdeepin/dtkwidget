// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef IMAGEVIEWEREXAMPLE_H
#define IMAGEVIEWEREXAMPLE_H

#include <dtkwidget_global.h>
#include "examplewindowinterface.h"
#include "pagewindowinterface.h"

DWIDGET_USE_NAMESPACE

class ImageViewerExampleWindow : public PageWindowInterface
{
    Q_OBJECT

public:
    explicit ImageViewerExampleWindow(QWidget *parent = nullptr);
};

class ImageViewerExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit ImageViewerExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

#endif  // IMAGEVIEWEREXAMPLE_H
