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

#ifndef DFLOATINGMESSAGE_H
#define DFLOATINGMESSAGE_H

#include <dtkwidget_global.h>
#include <dfloatingwidget.h>
#include <DObject>

#include <DIconButton>

DWIDGET_BEGIN_NAMESPACE

class DFloatingMessagePrivate;
class DFloatingMessage : public DFloatingWidget
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DFloatingMessage)

public:
    enum MessageType {
        TransientType,  //临时的消息,
        ResidentType    //常驻的消息
    };

    explicit DFloatingMessage(MessageType notifyType = MessageType::TransientType, QWidget *parent = nullptr);

    void setIcon(const QIcon &ico);
    void setMessage(const QString &str);
    void setWidget(QWidget *w);
    void setDuration(int msec);

protected:
    using DFloatingWidget::setWidget;

private:
    void showEvent(QShowEvent *event) override;
};

DWIDGET_END_NAMESPACE

#endif // DFLOATINGMESSAGE_H
