#include "dtrashmanager.h"
#include "private/dobject_p.h"

#include <QDirIterator>
#include <QStorageInfo>
#include <QCryptographicHash>
#include <QDateTime>
#include <QDebug>

#define TRASH_PATH QDir::homePath() + "/.local/share/Trash"
#define TRASH_INFO_PATH TRASH_PATH"/info"
#define TRASH_FILES_PATH TRASH_PATH"/files"

DUTIL_BEGIN_NAMESPACE

class DTrashManager_ : public DTrashManager {};
Q_GLOBAL_STATIC(DTrashManager_, globalTrashManager)

static QString getNotExistsFileName(const QString &fileName, const QString &targetPath)
{
    QByteArray name = fileName.toUtf8();

    int index = name.lastIndexOf('.');
    QByteArray suffix;

    if (index >= 0)
        suffix = name.mid(index);

    if (suffix.size() > 200)
        suffix = suffix.left(200);

    name.chop(suffix.size());
    name = name.left(200 - suffix.size());

    while (QFile::exists(targetPath + "/" + name + suffix)) {
        name = QCryptographicHash::hash(name, QCryptographicHash::Md5).toHex();
    }

    return QString::fromUtf8(name + suffix);
}

static bool writeTrashInfo(const QString &fileBaseName, const QString &sourceFilePath, const QDateTime &datetime, QString *errorString = NULL)
{
    QFile metadata(TRASH_INFO_PATH"/" + fileBaseName + ".trashinfo");

    if (metadata.exists()) {
        if (errorString)
            *errorString = QString("The %1 file is exists").arg(metadata.fileName());

        return false;
    }

    if (!metadata.open( QIODevice::WriteOnly)) {
        if (errorString)
            *errorString = metadata.errorString();

        return false;
    }

    QByteArray data;

    data.append("[Trash Info]\n");
    data.append("Path=").append(sourceFilePath.toUtf8().toPercentEncoding("/")).append("\n");
    data.append("DeletionDate=").append(datetime.toString(Qt::ISODate)).append("\n");

    qint64 size = metadata.write(data);
    metadata.close();

    if (size <= 0) {
        if (errorString)
            *errorString = metadata.errorString();

        return false;
    }

    return true;
}

static bool renameFile(const QFileInfo &fileInfo, const QString &target, QString *errorString = NULL)
{
    if (fileInfo.isFile() || fileInfo.isSymLink()) {
        QFile file(fileInfo.filePath());

        if (!file.rename(target)) {
            if (errorString)
                *errorString = file.errorString();

            return false;
        }

        return true;
    } else {
        QDirIterator iterator(fileInfo.filePath(),
                              QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);

        while (iterator.hasNext()) {
            iterator.next();

            const QString newFile = iterator.filePath().replace(0, fileInfo.filePath().length(), target);

            if (!QDir().mkpath(QFileInfo(newFile).path())) {
                if (errorString)
                    *errorString = QString("Make the %1 path is failed").arg(QFileInfo(newFile).path());

                return false;
            }

            if (!renameFile(iterator.fileInfo(), newFile, errorString)) {
                return false;
            }
        }

        if (!QDir().rmdir(fileInfo.filePath())) {
            if (errorString)
                *errorString = QString("Cannot remove the %1 dir").arg(fileInfo.filePath());

            return false;
        }
    }

    return true;
}

class DTrashManagerPrivate : public DObjectPrivate
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
    QDirIterator iterator(TRASH_INFO_PATH,
//                          QStringList() << "*.trashinfo",
                          QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden);

    return !iterator.hasNext();
}

bool DTrashManager::cleanTrash()
{
    QDirIterator iterator_info(TRASH_INFO_PATH,
//                               QStringList() << "*.trashinfo",
                               QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden);

    bool ok = true;

    while (iterator_info.hasNext()) {
        if (!QFile::remove(iterator_info.next()))
            ok = false;
    }

    QDirIterator iterator_files(TRASH_FILES_PATH,
                                QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden,
                                QDirIterator::Subdirectories);

    while (iterator_files.hasNext()) {
        if (!QFile::remove(iterator_files.next()))
            ok = false;
    }

    return ok;
}

bool DTrashManager::moveToTrash(const QString &filePath, bool followSymlink)
{
    QFileInfo fileInfo(filePath);

    if (!fileInfo.exists() && (followSymlink || !fileInfo.isSymLink()))
        return false;

    QDir trashDir(TRASH_FILES_PATH);
    QStorageInfo storageInfo(fileInfo.filePath());
    QStorageInfo trashStorageInfo(trashDir);

    if (storageInfo != trashStorageInfo) {
        return false;
    }

    if (!trashDir.mkpath(TRASH_INFO_PATH)) {
        return false;
    }

    if (!trashDir.mkpath(TRASH_FILES_PATH)) {
        return false;
    }

    if (followSymlink && fileInfo.isSymLink()) {
        fileInfo.setFile(fileInfo.symLinkTarget());
    }

    const QString &fileName = getNotExistsFileName(fileInfo.fileName(), TRASH_FILES_PATH);

    if (!writeTrashInfo(fileName, fileInfo.filePath(), QDateTime::currentDateTime()))
        return false;

    const QString &newFilePath = TRASH_FILES_PATH"/" + fileName;

    return renameFile(fileInfo, newFilePath);
}

DTrashManager::DTrashManager()
    : QObject()
    , DObject(*new DTrashManagerPrivate(this))
{

}

DUTIL_END_NAMESPACE
