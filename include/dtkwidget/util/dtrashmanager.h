// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DTKWIDGET_DTRASHMANAGER_H
#define DTKWIDGET_DTRASHMANAGER_H

#include <DObject>

#include <dtkwidget_global.h>

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

#endif // DTKWIDGET_DTRASHMANAGER_H
