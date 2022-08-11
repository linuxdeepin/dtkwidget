// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DCIRCLEPROGRESS_P_H
#define DCIRCLEPROGRESS_P_H

#include <dcircleprogress.h>

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
