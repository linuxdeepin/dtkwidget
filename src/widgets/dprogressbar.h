/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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
