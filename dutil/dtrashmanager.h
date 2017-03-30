#ifndef DTRASHMANAGER_H
#define DTRASHMANAGER_H

#include "dutil_global.h"
#include "dobject.h"

#include <QObject>

DUTIL_BEGIN_NAMESPACE

class DTrashManagerPrivate;
class DTrashManager : public QObject, public DObject
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

DUTIL_END_NAMESPACE

#endif // DTRASHMANAGER_H
