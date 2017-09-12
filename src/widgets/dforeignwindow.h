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

#ifndef DFOREIGNWINDOW_H
#define DFOREIGNWINDOW_H

#include "dobject.h"
#include "dtkwidget_global.h"

#include <QWindow>

DWIDGET_BEGIN_NAMESPACE

class DForeignWindowPrivate;
class DForeignWindow : public QWindow, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
    Q_PROPERTY(QString wmClass READ wmClass NOTIFY wmClassChanged)
    Q_PROPERTY(quint32 pid READ pid NOTIFY pidChanged)

public:
    explicit DForeignWindow(QWindow *parent = 0);

    static DForeignWindow *fromWinId(WId id);

    QString wmClass() const;
    quint32 pid() const;

Q_SIGNALS:
    void wmClassChanged();
    void pidChanged();

protected:
    bool event(QEvent *) Q_DECL_OVERRIDE;

private:
    D_DECLARE_PRIVATE(DForeignWindow)
};

DWIDGET_END_NAMESPACE

#endif // DFOREIGNWINDOW_H
