// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dprogressbar.h"

#include <QStyle>
#include <QStyleOptionProgressBar>

DWIDGET_BEGIN_NAMESPACE

DProgressBar::DProgressBar(QWidget *parent)
    : QProgressBar(parent)
{

}

QSize DProgressBar::sizeHint() const
{
    if (isTextVisible()) {
        return QProgressBar::sizeHint();
    }

    QStyleOptionProgressBar opt;
    initStyleOption(&opt);
    return style()->sizeFromContents(QStyle::CT_ProgressBar, &opt, QSize(1, 1), this);
}

QSize DProgressBar::minimumSizeHint() const
{
    if (isTextVisible()) {
        return QProgressBar::minimumSizeHint();
    }

    return sizeHint();
}

DWIDGET_END_NAMESPACE
