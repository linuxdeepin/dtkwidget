/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef DCIRCLEPROGRESS_P_H
#define DCIRCLEPROGRESS_P_H

#include "dcircleprogress.h"

#include <DObjectPrivate>

#include <QPainter>

DWIDGET_BEGIN_NAMESPACE

class DCircleProgressPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    Q_DECLARE_PUBLIC(DCircleProgress)

public:
    DCircleProgressPrivate(DCircleProgress *q);

private:
    void paint(QPainter *painter);

private:
    int m_lineWidth = 3;
    int m_currentValue = 40;
    int m_minmumValue = 0;
    int m_maximumValue = 100;

    QLabel m_topLabel;
    QLabel m_bottomLabel;

    QColor m_chunkColor = Qt::cyan;
    QColor m_backgroundColor = Qt::darkCyan;
};

DWIDGET_END_NAMESPACE

#endif // DCIRCLEPROGRESS_P_H
