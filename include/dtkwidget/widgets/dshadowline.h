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
