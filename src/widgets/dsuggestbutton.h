// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DSUGGESTBUTTON_H
#define DSUGGESTBUTTON_H

#include <dtkwidget_global.h>
#include <DObject>

#include <QPushButton>

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DSuggestButton : public QPushButton, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

public:
    explicit DSuggestButton(QWidget *parent = nullptr);
    explicit DSuggestButton(const QString &text, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

DWIDGET_END_NAMESPACE

#endif // DSUGGESTBUTTON_H
