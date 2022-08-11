// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DMPRISCONTROL_H
#define DMPRISCONTROL_H

#include <DObject>
#include <dtkwidget_global.h>

#include <QFrame>

DWIDGET_BEGIN_NAMESPACE

class DMPRISControlPrivate;
class LIBDTKWIDGETSHARED_EXPORT DMPRISControl : public QFrame, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DMPRISControl)
    D_DECLARE_PRIVATE(DMPRISControl)

public:
    explicit DMPRISControl(QWidget *parent = 0);

    bool isWorking() const;

Q_SIGNALS:
    void mprisAcquired() const;
    void mprisChanged() const;
    void mprisLosted() const;

public Q_SLOTS:
    void setPictureVisible(bool visible);
    void setPictureSize(const QSize &size);

protected:
    void showEvent(QShowEvent *event);

    D_PRIVATE_SLOT(void _q_onMetaDataChanged())
    D_PRIVATE_SLOT(void _q_onPlaybackStatusChanged())
    D_PRIVATE_SLOT(void _q_onPrevClicked())
    D_PRIVATE_SLOT(void _q_onPlayClicked())
    D_PRIVATE_SLOT(void _q_onPauseClicked())
    D_PRIVATE_SLOT(void _q_onNextClicked())
    D_PRIVATE_SLOT(void _q_loadMPRISPath(const QString &))
    D_PRIVATE_SLOT(void _q_removeMPRISPath(const QString &))
    D_PRIVATE_SLOT(void _q_onCanControlChanged(bool canControl))
};

DWIDGET_END_NAMESPACE

#endif // DMPRISCONTROL_H
