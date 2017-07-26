/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DTRASHMANAGER_H
#define DTRASHMANAGER_H

#include <DObject>

#include "dtkwidget_global.h"

#include <QObject>

DWIDGET_BEGIN_NAMESPACE

class DTrashManagerPrivate;
class DTrashManager : public QObject, public DTK_CORE_NAMESPACE::DObject
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
