// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DSHADOWLINE_H
#define DSHADOWLINE_H

#include <dtkwidget_global.h>
#include <DObject>

#include <QWidget>

DWIDGET_BEGIN_NAMESPACE

class DShadowLinePrivate;
class LIBDTKWIDGETSHARED_EXPORT DShadowLine : public QWidget, public DCORE_NAMESPACE::DObject
{
    D_DECLARE_PRIVATE(DShadowLine)

public:
    explicit DShadowLine(QWidget *parent = nullptr);

    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event) override;
};

DWIDGET_END_NAMESPACE

#endif // DSHADOWLINE_H
