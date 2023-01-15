// SPDX-FileCopyrightText: 2020 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef BUTTONEXAMPLE_H
#define BUTTONEXAMPLE_H
#include <QWidget>
#include <QLabel>

#include <dtkwidget_global.h>
#include "examplewindowinterface.h"
#include "pagewindowinterface.h"

class ButtonExampleWindow : public PageWindowInterface
{
    Q_OBJECT

public:
    explicit ButtonExampleWindow(QWidget *parent = nullptr);
};

class DPushButtonExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DPushButtonExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DWarningButtonExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DWarningButtonExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DSuggestButtonExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DSuggestButtonExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DToolButtonExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DToolButtonExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DIconButtonExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DIconButtonExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DButtonBoxExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DButtonBoxExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
    virtual void changeEvent(QEvent *event) override;
    void updateButtonsSize();
};

class DFloatingButtonExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DFloatingButtonExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DSwitchButtonExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DSwitchButtonExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DRadioButtonExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DRadioButtonExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DCheckButtonExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DCheckButtonExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DComboBoxExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DComboBoxExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DFontComboBoxExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DFontComboBoxExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DSearchComboBoxExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DSearchComboBoxExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

#endif // BUTTONEXAMPLE_H
