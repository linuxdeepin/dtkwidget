// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EDITEXAMPLE_H
#define EDITEXAMPLE_H

#include <dtkwidget_global.h>
#include "examplewindowinterface.h"
#include "pagewindowinterface.h"

class QWidget;
class QLabel;

class EditExampleWindow : public PageWindowInterface
{
    Q_OBJECT

public:
    explicit EditExampleWindow(QWidget *parent = nullptr);
};

class DSearchEditExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DSearchEditExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DLineEditExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DLineEditExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DIpv4LineEditExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DIpv4LineEditExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DPasswordEditExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DPasswordEditExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DFileChooserEditExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DFileChooserEditExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DSpinBoxExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DSpinBoxExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DTextEditExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DTextEditExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DCrumbTextFormatExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DCrumbTextFormatExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DKeySequenceEditExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DKeySequenceEditExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

#endif // EDITEXAMPLE_H
