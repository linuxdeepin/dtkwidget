/*
 * Copyright (C) 2011 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     wp <wangpeng_cm@deepin.com>
 *
 * Maintainer: wp <wangpeng_cm@deepin.com>
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

#ifndef DSEARCHEDIT_P_H
#define DSEARCHEDIT_P_H

#include "dlineedit_p.h"
#include "dsearchedit.h"

#include <QLabel>

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

public:
    QAction *action;
    QString placeHolder;

    QWidget *iconWidget;

#ifdef ENABLE_XFYUN
    QAction *voiceAction;
    QAudioInput *voiceInput = nullptr;
    VoiceDevice *voiceIODevice = nullptr;
#endif

protected:
    Q_DECLARE_PUBLIC(DSearchEdit)
};

DWIDGET_END_NAMESPACE

#endif // DSEARCHEDIT_P_H
