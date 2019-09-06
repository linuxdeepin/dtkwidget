/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zhangwengeng <zhangwengeng_cm@deepin.com>
 *
 * Maintainer: zhangwengeng <zhangwengeng_cm@deepin.com>
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

#include "dfloatingmessage.h"
#include "private/dfloatingmessage_p.h"

#include <QHBoxLayout>
#include <QTimer>
#include <QLabel>

DWIDGET_BEGIN_NAMESPACE

DFloatingMessagePrivate::DFloatingMessagePrivate(DFloatingMessage *qq)
    : DFloatingWidgetPrivate(qq)
{

}

DFloatingMessagePrivate::~DFloatingMessagePrivate()
{

}

void DFloatingMessagePrivate::init()
{
    D_Q(DFloatingMessage);

    timer = nullptr;
    content = nullptr;
    icoBtnRight = nullptr;
    notifyType = notifyType;
    widget = new QWidget();
    q->DFloatingWidget::setWidget(widget);
    hBoxLayout = new QHBoxLayout(widget);
    icoBtnLeft = new DIconButton(nullptr);
    labMessage = new QLabel();

    hBoxLayout->addWidget(icoBtnLeft);
    hBoxLayout->addWidget(labMessage);

    if (notifyType == DFloatingMessage::MessageType::TransientType) {  //临时消息
        timer = new QTimer(q);
        timer->setInterval(4000);
        timer->setSingleShot(true);
        q->connect(timer, &QTimer::timeout, q, &DFloatingMessage::close);
    } else {  //常驻消息
        content  = nullptr;
        icoBtnRight = new DIconButton(DStyle::SP_CloseButton);

        hBoxLayout->addWidget(icoBtnRight);
        q->connect(icoBtnRight, &DIconButton::clicked, q, &DFloatingMessage::close);
    }
}

DFloatingMessage::DFloatingMessage(QWidget *parent, MessageType notifyType)
    : DFloatingWidget(*new DFloatingMessagePrivate(this), parent)
{
    D_D(DFloatingMessage);

    d->notifyType = notifyType;
    d->init();
}

void DFloatingMessage::setIcon(const QIcon &ico)
{
    D_D(DFloatingMessage);

    d->icoBtnLeft->setIcon(ico);
}

void DFloatingMessage::setMessage(const QString &str)
{
    D_D(DFloatingMessage);

    d->labMessage->setText(str);
}

void DFloatingMessage::setWidget(QWidget *w)
{
    D_D(DFloatingMessage);

    if (d->content)
        delete d->hBoxLayout->takeAt(2);

    d->content = w;
    d->layout->insertWidget(2, d->content);
}

void DFloatingMessage::setTimeInterval(int msec)
{
    D_D(DFloatingMessage);

    if (d->timer == nullptr)
        return;

    d->timer->setInterval(msec);
}

void DFloatingMessage::showEvent(QShowEvent *event)
{
    D_D(DFloatingMessage);

    if (d->timer)
        d->timer->start();

    DFloatingWidget::showEvent(event);
}

DWIDGET_END_NAMESPACE

