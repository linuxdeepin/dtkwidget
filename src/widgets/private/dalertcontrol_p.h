/*
 * Copyright (C) 2020 ~ 2021 Deepin Technology Co., Ltd.
 *
 * Author:     ck <chenke@uniontech.com>
 *
 * Maintainer: ck <chenke@uniontech.com>
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

#ifndef DALERTCONTROL_P_H
#define DALERTCONTROL_P_H
#include "dalertcontrol.h"
#include <DObjectPrivate>
#include <QPointer>

DWIDGET_BEGIN_NAMESPACE

class DAlertControlPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    Q_DECLARE_PUBLIC(DAlertControl)
public:
    DAlertControlPrivate(DAlertControl *q);

    void updateTooltipPos();

private:
    bool isAlert = false;
    QPointer<DToolTip> tooltip;
    QPointer<DFloatingWidget> frame;
    QWidget *follower{nullptr};
    QWidget *target{nullptr};
    QColor  alertColor;
    Qt::Alignment alignment{Qt::AlignLeft};
};

DWIDGET_END_NAMESPACE

#endif // DALERTCONTROL_P_H
