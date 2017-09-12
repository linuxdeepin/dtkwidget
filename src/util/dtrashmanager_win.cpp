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

#include "dtrashmanager.h"

#include <DObjectPrivate>

#include <QDirIterator>
#include <QStorageInfo>
#include <QCryptographicHash>
#include <QDateTime>
#include <QDebug>

DWIDGET_BEGIN_NAMESPACE

class DTrashManager_ : public DTrashManager {};
Q_GLOBAL_STATIC(DTrashManager_, globalTrashManager)

static QString getNotExistsFileName(const QString &fileName, const QString &targetPath)
{
    QByteArray name = fileName.toUtf8();

    int index = name.lastIndexOf('.');
    QByteArray suffix;

    if (index >= 0)
    {
        suffix = name.mid(index);
    }

    if (suffix.size() > 200)
    {
        suffix = suffix.left(200);
    }

    name.chop(suffix.size());
    name = name.left(200 - suffix.size());

    while (QFile::exists(targetPath + "/" + name + suffix))
    {
        name = QCryptographicHash::hash(name, QCryptographicHash::Md5).toHex();
    }

    return QString::fromUtf8(name + suffix);
}

static bool renameFile(const QFileInfo &fileInfo, const QString &target, QString *errorString = NULL)
{
    if (fileInfo.isFile() || fileInfo.isSymLink())
    {
        QFile file(fileInfo.filePath());

        if (!file.rename(target))
        {
            if (errorString)
            {
                *errorString = file.errorString();
            }

            return false;
        }

        return true;
    }
    else
    {
        QDirIterator iterator(fileInfo.filePath(),
                              QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);

        while (iterator.hasNext())
        {
            iterator.next();

            const QString newFile = iterator.filePath().replace(0, fileInfo.filePath().length(), target);

            if (!QDir().mkpath(QFileInfo(newFile).path()))
            {
                if (errorString)
                {
                    *errorString = QString("Make the %1 path is failed").arg(QFileInfo(newFile).path());
                }

                return false;
            }

            if (!renameFile(iterator.fileInfo(), newFile, errorString))
            {
                return false;
            }
        }

        if (!QDir().rmdir(fileInfo.filePath()))
        {
            if (errorString)
            {
                *errorString = QString("Cannot remove the %1 dir").arg(fileInfo.filePath());
            }

            return false;
        }
    }

    return true;
}

class DTrashManagerPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    DTrashManagerPrivate(DTrashManager *q_ptr)
        : DObjectPrivate(q_ptr) {}

    D_DECLARE_PUBLIC(DTrashManager)
};

DTrashManager *DTrashManager::instance()
{
    return globalTrashManager;
}

bool DTrashManager::trashIsEmpty() const
{
    return false;
}

bool DTrashManager::cleanTrash()
{
    return false;
}

bool DTrashManager::moveToTrash(const QString &filePath, bool followSymlink)
{
    return false;
}

DTrashManager::DTrashManager()
    : QObject()
    , DObject(*new DTrashManagerPrivate(this))
{

}

DWIDGET_END_NAMESPACE
