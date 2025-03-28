// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DALERTCONTROL_P_H
#define DALERTCONTROL_P_H
#include <DAlertControl>
#include <DObjectPrivate>
#include <QPointer>
#include <QTimer>

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
    QPointer<QWidget> follower;
    QPointer<QWidget> target;
    QColor  alertColor;
    Qt::Alignment alignment{Qt::AlignLeft};
    Qt::Alignment messageAlignment{Qt::AlignLeft};
    QTimer timer;
};

DWIDGET_END_NAMESPACE

#endif // DALERTCONTROL_P_H
