#ifndef DDESKTOPSERVICES_H
#define DDESKTOPSERVICES_H

#include "dutil_global.h"

#include <QUrl>

DUTIL_BEGIN_NAMESPACE

class DDesktopServices
{
public:
    static bool showFolder(QString localFilePath, const QString &startupId = QString());
    static bool showFolders(const QList<QString> localFilePaths, const QString &startupId = QString());
    static bool showFolder(QUrl url, const QString &startupId = QString());
    static bool showFolders(const QList<QUrl> urls, const QString &startupId = QString());

    static bool showFileItemPropertie(QString localFilePath, const QString &startupId = QString());
    static bool showFileItemProperties(const QList<QString> localFilePaths, const QString &startupId = QString());
    static bool showFileItemPropertie(QUrl url, const QString &startupId = QString());
    static bool showFileItemProperties(const QList<QUrl> urls, const QString &startupId = QString());

    static bool showFileItem(QString localFilePath, const QString &startupId = QString());
    static bool showFileItems(const QList<QString> localFilePaths, const QString &startupId = QString());
    static bool showFileItem(QUrl url, const QString &startupId = QString());
    static bool showFileItems(const QList<QUrl> urls, const QString &startupId = QString());

    static QString errorMessage();
};

DUTIL_END_NAMESPACE

#endif // DDESKTOPSERVICES_H
