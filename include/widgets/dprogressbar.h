// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DPROGRESSBAR_H
#define DPROGRESSBAR_H

#include <dtkwidget_global.h>
#include <DObject>
#include <QProgressBar>

DWIDGET_BEGIN_NAMESPACE

class DProgressBar : public QProgressBar, public DCORE_NAMESPACE::DObject
{
public:
    explicit DProgressBar(QWidget *parent = nullptr);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
};

DWIDGET_END_NAMESPACE

#endif // DPROGRESSBAR_H
