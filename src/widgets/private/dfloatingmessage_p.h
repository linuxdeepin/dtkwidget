// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DFLOATINGMESSAGE_P_H
#define DFLOATINGMESSAGE_P_H

#include <DFloatingMessage>
#include "dfloatingwidget_p.h"

QT_BEGIN_NAMESPACE
class QTimer;
class QWidget;
class QHBoxLayout;
class DIconButton;
class QLabel;
class MessageLabel;
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

class DFloatingMessagePrivate : public DFloatingWidgetPrivate
{
public:
    explicit DFloatingMessagePrivate(DFloatingMessage *qq);
    ~DFloatingMessagePrivate();

    void init();

    QTimer *timer = nullptr;
    QWidget *widget = nullptr;
    QHBoxLayout *hBoxLayout = nullptr;
    DIconButton *iconButton = nullptr;
    MessageLabel *labMessage = nullptr;
    QWidget *content = nullptr;
    DIconButton *closeButton = nullptr;
    DFloatingMessage::MessageType notifyType = DFloatingMessage::MessageType::TransientType;

    D_DECLARE_PUBLIC(DFloatingMessage)
};

DWIDGET_END_NAMESPACE

#endif // DFLOATINGMESSAGE_P_H
