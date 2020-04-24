/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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
