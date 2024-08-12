// SPDX-FileCopyrightText: 2011 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DSEARCHEDIT_P_H
#define DSEARCHEDIT_P_H

#include "dlineedit_p.h"
#include <DSearchEdit>

#include <QLabel>
#include <QPropertyAnimation>

QT_BEGIN_NAMESPACE
class QAudioInput;
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

class VoiceDevice;
class DIconButton;
class DSearchEditPrivate : DLineEditPrivate
{
public:
    DSearchEditPrivate(DSearchEdit *q);
    ~DSearchEditPrivate();

    void init();

    void _q_toEditMode(bool focus);
    void _q_onVoiceActionTrigger(bool checked);
    void _q_clearFocus();

public:
    QAction *action;
    QString placeHolder;
    QString placeholderText;

    QWidget *iconWidget;
    QLabel *label;
    QPropertyAnimation *animation;

#ifdef ENABLE_AI
    QAction *voiceAction = nullptr;
    QAudioInput *voiceInput = nullptr;
    VoiceDevice *voiceIODevice = nullptr;
#endif

protected:
    Q_DECLARE_PUBLIC(DSearchEdit)
};

DWIDGET_END_NAMESPACE

#endif // DSEARCHEDIT_P_H
