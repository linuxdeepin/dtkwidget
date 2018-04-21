/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef DMPRISCONTROL_H
#define DMPRISCONTROL_H

#include "dobject.h"
#include "dtkwidget_global.h"

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
