/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef DSEPARATORVERTICAL_H
#define DSEPARATORVERTICAL_H

#include <QWidget>
#include <QHBoxLayout>

#include "dtkwidget_global.h"

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DSeparatorVertical : public QWidget
{
    Q_OBJECT
public:
    explicit DSeparatorVertical(QWidget *parent = 0);

private:
    QWidget *m_leftRec = NULL;
    QWidget *m_rightRec = NULL;
};

DWIDGET_END_NAMESPACE

#endif // DSEPARATORVERTICAL_H
