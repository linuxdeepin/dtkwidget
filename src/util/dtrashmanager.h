/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DTRASHMANAGER_H
#define DTRASHMANAGER_H

#include <DObject>

#include "dtkwidget_global.h"

#include <QObject>

DWIDGET_BEGIN_NAMESPACE

class DTrashManagerPrivate;
class D_DECL_DEPRECATED_X("Use libdtkcore") DTrashManager : public QObject, public DTK_CORE_NAMESPACE::DObject
{
public:
    static DTrashManager *instance();

    bool trashIsEmpty() const;
    bool cleanTrash();
    bool moveToTrash(const QString &filePath, bool followSymlink = false);

protected:
    DTrashManager();

private:
    D_DECLARE_PRIVATE(DTrashManager)
};

DWIDGET_END_NAMESPACE

#endif // DTRASHMANAGER_H
