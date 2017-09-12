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

#ifndef DWINDOWMANAGERHELPER_H
#define DWINDOWMANAGERHELPER_H

#include <QWindow>

#include <DObject>

#include "dtkwidget_global.h"

DWIDGET_BEGIN_NAMESPACE

class DForeignWindow;
class DWindowManagerHelperPrivate;
class DWindowManagerHelper : public QObject, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(bool hasBlurWindow READ hasBlurWindow NOTIFY hasBlurWindowChanged)
    Q_PROPERTY(bool hasComposite READ hasComposite NOTIFY hasCompositeChanged)

public:
    ~DWindowManagerHelper();

    static DWindowManagerHelper *instance();

    bool hasBlurWindow() const;
    bool hasComposite() const;

    QList<DForeignWindow*> currentWorkspaceWindows() const;

Q_SIGNALS:
    void windowManagerChanged();
    void hasBlurWindowChanged();
    void hasCompositeChanged();
    void windowListChanged();

protected:
    explicit DWindowManagerHelper(QObject *parent = 0);

private:
    D_DECLARE_PRIVATE(DWindowManagerHelper)
};

DWIDGET_END_NAMESPACE

#endif // DWINDOWMANAGERHELPER_H
