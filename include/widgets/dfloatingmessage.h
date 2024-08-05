// SPDX-FileCopyrightText: 2019 - 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DFLOATINGMESSAGE_H
#define DFLOATINGMESSAGE_H

#include <dtkwidget_global.h>
#include <dfloatingwidget.h>
#include <DObject>
#include <DDciIcon>
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
    MessageType messageType() const;

    void setIcon(const QIcon &ico);
    void setIcon(const DDciIcon &icon);
    void setMessage(const QString &str);
    void setWidget(QWidget *w);
    void setDuration(int msec);

    virtual QSize sizeHint() const override;

Q_SIGNALS:
    void closeButtonClicked();
    void messageClosed();

protected:
    using DFloatingWidget::setWidget;

    virtual void changeEvent(QEvent *event) override;
private:
    void showEvent(QShowEvent *event) override;
};

DWIDGET_END_NAMESPACE

#endif // DFLOATINGMESSAGE_H
