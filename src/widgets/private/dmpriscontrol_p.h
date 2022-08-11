// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DMPRISCONTROL_P_H
#define DMPRISCONTROL_P_H

#include <DObjectPrivate>
#include <DFloatingButton>

#include <DImageButton>
#include <dmpriscontrol.h>
#include <dtickeffect.h>
#include "private/mpris/dbusmpris.h"
#include "private/mpris/dmprismonitor.h"

#include <QScrollArea>

DWIDGET_BEGIN_NAMESPACE

class DMPRISControlPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate {
    D_DECLARE_PUBLIC(DMPRISControl)

public:
    explicit DMPRISControlPrivate(DMPRISControl *q);

    void init();

public Q_SLOTS:
    void _q_onPrevClicked();
    void _q_onPlayClicked();
    void _q_onPauseClicked();
    void _q_onNextClicked();

    void _q_onMetaDataChanged();
    void _q_onPlaybackStatusChanged();

    void _q_loadMPRISPath(const QString &path);
    void _q_removeMPRISPath(const QString &path);

    void _q_onCanControlChanged(bool canControl);

public:
    DMPRISMonitor *m_mprisMonitor;
    DBusMPRIS *    m_mprisInter;

    QScrollArea *m_titleScrollArea;
    QLabel *     m_title;
    QLabel *     m_picture;
    DTickEffect *m_tickEffect;

    QWidget *     m_controlWidget;
    DFloatingButton *m_prevBtn;
    DFloatingButton *m_playBtn;
    DFloatingButton *m_nextBtn;

    bool m_playStatus = false;
    bool m_clickedStatus = false;
    bool m_pictureVisible;

    QString     m_lastPath;
    QStringList m_mprisPaths;
};

DWIDGET_END_NAMESPACE

#endif  // DMPRISCONTROL_P_H
