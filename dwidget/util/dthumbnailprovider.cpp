/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dthumbnailprovider.h"
#include "private/dobject_p.h"

#include <QCryptographicHash>
#include <QDir>
#include <QDateTime>
#include <QImageReader>
#include <QQueue>
#include <QMimeType>
#include <QMimeDatabase>
#include <QReadWriteLock>
#include <QWaitCondition>
#include <QPainter>
#include <QUrl>
#include <QStandardPaths>
#include <QDebug>

DWIDGET_BEGIN_NAMESPACE

#define FORMAT ".png"
#define THUMBNAIL_PATH QStandardPaths::writableLocation(QStandardPaths::GenericCacheLocation) + "/thumbnails"
#define THUMBNAIL_FAIL_PATH THUMBNAIL_PATH"/fail"
#define THUMBNAIL_LARGE_PATH THUMBNAIL_PATH"/large"
#define THUMBNAIL_NORMAL_PATH THUMBNAIL_PATH"/normal"
#define THUMBNAIL_SMALL_PATH THUMBNAIL_PATH"/small"

inline QByteArray dataToMd5Hex(const QByteArray &data)
{
    return QCryptographicHash::hash(data, QCryptographicHash::Md5).toHex();
}

class DThumbnailProviderPrivate : public DObjectPrivate
{
public:
    DThumbnailProviderPrivate(DThumbnailProvider *qq);

    void init();

    QString sizeToFilePath(DThumbnailProvider::Size size) const;

    QString errorString;
    // MAX
    qint64 defaultSizeLimit = INT64_MAX;
    QHash<QMimeType, qint64> sizeLimitHash;
    QMimeDatabase mimeDatabase;

    static QSet<QString> hasThumbnailMimeHash;

    struct ProduceInfo {
        QFileInfo fileInfo;
        DThumbnailProvider::Size size;
        DThumbnailProvider::CallBack callback;
    };

    QQueue<ProduceInfo> produceQueue;
    QSet<QPair<QString, DThumbnailProvider::Size>> discardedProduceInfos;

    bool running = true;

    QWaitCondition waitCondition;
    QReadWriteLock dataReadWriteLock;

    D_DECLARE_PUBLIC(DThumbnailProvider)
};

QSet<QString> DThumbnailProviderPrivate::hasThumbnailMimeHash;

DThumbnailProviderPrivate::DThumbnailProviderPrivate(DThumbnailProvider *qq)
    : DObjectPrivate(qq)
{

}

void DThumbnailProviderPrivate::init()
{

}

QString DThumbnailProviderPrivate::sizeToFilePath(DThumbnailProvider::Size size) const
{
    switch (size) {
    case DThumbnailProvider::Small:
        return THUMBNAIL_SMALL_PATH;
    case DThumbnailProvider::Normal:
        return THUMBNAIL_NORMAL_PATH;
    case DThumbnailProvider::Large:
        return THUMBNAIL_LARGE_PATH;
    }

    return QString();
}

class DFileThumbnailProviderPrivate : public DThumbnailProvider {};
Q_GLOBAL_STATIC(DFileThumbnailProviderPrivate, ftpGlobal)

DThumbnailProvider *DThumbnailProvider::instance()
{
    return ftpGlobal;
}

bool DThumbnailProvider::hasThumbnail(const QFileInfo &info) const
{
    Q_D(const DThumbnailProvider);

    if (!info.isReadable() || !info.isFile())
        return false;

    qint64 fileSize = info.size();

    if (fileSize <= 0)
        return false;

    const QMimeType &mime = d->mimeDatabase.mimeTypeForFile(info);

    if (fileSize > sizeLimit(mime))
        return false;

    return hasThumbnail(mime);
}

bool DThumbnailProvider::hasThumbnail(const QMimeType &mimeType) const
{
    const QString &mime = mimeType.name();

    if (DThumbnailProviderPrivate::hasThumbnailMimeHash.isEmpty()) {
        const QList<QByteArray> &mimeTypes = QImageReader::supportedMimeTypes();

        if (mimeTypes.isEmpty()) {
            DThumbnailProviderPrivate::hasThumbnailMimeHash.insert("");

            return false;
        }

        DThumbnailProviderPrivate::hasThumbnailMimeHash.reserve(mimeTypes.size());

        for (const QByteArray &t : mimeTypes) {
            DThumbnailProviderPrivate::hasThumbnailMimeHash.insert(QString::fromLocal8Bit(t));
        }
    }

    return DThumbnailProviderPrivate::hasThumbnailMimeHash.contains(mime);
}

QString DThumbnailProvider::thumbnailFilePath(const QFileInfo &info, Size size) const
{
    Q_D(const DThumbnailProvider);

    const QString &absolutePath = info.absolutePath();
    const QString &absoluteFilePath = info.absoluteFilePath();

    if (absolutePath == d->sizeToFilePath(Small)
            || absolutePath == d->sizeToFilePath(Normal)
            || absolutePath == d->sizeToFilePath(Large)
            || absolutePath == THUMBNAIL_FAIL_PATH) {
        return absoluteFilePath;
    }

    const QString thumbnailName = dataToMd5Hex(QUrl::fromLocalFile(absoluteFilePath).toString(QUrl::FullyEncoded).toLocal8Bit()) + FORMAT;
    QString thumbnail = d->sizeToFilePath(size) + QDir::separator() + thumbnailName;

    if (!QFile::exists(thumbnail)) {
        return QString();
    }

    QImage image(thumbnail);

    if (image.text(QT_STRINGIFY(Thumb::MTime)).toInt() != (int)info.lastModified().toTime_t()) {
        QFile::remove(thumbnail);

        Q_EMIT thumbnailChanged(absoluteFilePath, QString());

        return QString();
    }

    return thumbnail;
}

QString DThumbnailProvider::createThumbnail(const QFileInfo &info, DThumbnailProvider::Size size)
{
    Q_D(DThumbnailProvider);

    d->errorString.clear();

    const QString &absolutePath = info.absolutePath();
    const QString &absoluteFilePath = info.absoluteFilePath();

    if (absolutePath == d->sizeToFilePath(Small)
            || absolutePath == d->sizeToFilePath(Normal)
            || absolutePath == d->sizeToFilePath(Large)
            || absolutePath == THUMBNAIL_FAIL_PATH) {
        return absoluteFilePath;
    }

    if (!hasThumbnail(info)) {
        d->errorString = QStringLiteral("This file has not support thumbnail: ") + absoluteFilePath;

        //!Warnning: Do not store thumbnails to the fail path
        return QString();
    }

    const QString fileUrl = QUrl::fromLocalFile(absoluteFilePath).toString(QUrl::FullyEncoded);
    const QString thumbnailName = dataToMd5Hex(fileUrl.toLocal8Bit()) + FORMAT;

    // the file is in fail path
    QString thumbnail = THUMBNAIL_FAIL_PATH + QDir::separator() + thumbnailName;

    if (QFile::exists(thumbnail)) {
        QImage image(thumbnail);

        if (image.text(QT_STRINGIFY(Thumb::MTime)).toInt() != (int)info.lastModified().toTime_t()) {
            QFile::remove(thumbnail);
        } else {
            return QString();
        }
    }// end

    QScopedPointer<QImage> image(new QImage(QSize(size, size), QImage::Format_ARGB32_Premultiplied));
    QImageReader reader(absoluteFilePath);

    if (!reader.canRead()) {
        reader.setFormat(d->mimeDatabase.mimeTypeForFile(info).name().toLocal8Bit());

        if (!reader.canRead()) {
            d->errorString = reader.errorString();
        }
    }

    if (d->errorString.isEmpty()) {
        const QSize &imageSize = reader.size();

        if (imageSize.isValid()) {
            if (imageSize.width() >= size || imageSize.height() >= size) {
                reader.setScaledSize(reader.size().scaled(size, size, Qt::KeepAspectRatio));
            }

            if (!reader.read(image.data())) {
                d->errorString = reader.errorString();
            }
        } else {
            d->errorString = "Fail to read image file attribute data:" + info.absoluteFilePath();
        }
    }

    // successful
    if (d->errorString.isEmpty()) {
        thumbnail = d->sizeToFilePath(size) + QDir::separator() + thumbnailName;
    } else {
        //fail
        image.reset(new QImage(1, 1, QImage::Format_Mono));
    }

    image->setText(QT_STRINGIFY(Thumb::URL), fileUrl);
    image->setText(QT_STRINGIFY(Thumb::MTime), QString::number(info.lastModified().toTime_t()));

    // create path
    QFileInfo(thumbnail).absoluteDir().mkpath(".");

    if (!image->save(thumbnail, Q_NULLPTR, 80)) {
        d->errorString = QStringLiteral("Can not save image to ") + thumbnail;
    }

    if (d->errorString.isEmpty()) {
        Q_EMIT createThumbnailFinished(absoluteFilePath, thumbnail);
        Q_EMIT thumbnailChanged(absoluteFilePath, thumbnail);

        return thumbnail;
    }

    // fail
    Q_EMIT createThumbnailFailed(absoluteFilePath);

    return QString();
}

void DThumbnailProvider::appendToProduceQueue(const QFileInfo &info, DThumbnailProvider::Size size, DThumbnailProvider::CallBack callback)
{
    DThumbnailProviderPrivate::ProduceInfo produceInfo;

    produceInfo.fileInfo = info;
    produceInfo.size = size;
    produceInfo.callback = callback;

    Q_D(DThumbnailProvider);

    if (isRunning()) {
        QWriteLocker locker(&d->dataReadWriteLock);
        d->produceQueue.append(std::move(produceInfo));
        locker.unlock();
        d->waitCondition.wakeAll();
    } else {
        d->produceQueue.append(std::move(produceInfo));
        start();
    }
}

void DThumbnailProvider::removeInProduceQueue(const QFileInfo &info, DThumbnailProvider::Size size)
{
    Q_D(DThumbnailProvider);

    if (isRunning()) {
        QWriteLocker locker(&d->dataReadWriteLock);
        Q_UNUSED(locker)
    }

    d->discardedProduceInfos.insert(qMakePair(info.absoluteFilePath(), size));
}

QString DThumbnailProvider::errorString() const
{
    Q_D(const DThumbnailProvider);

    return d->errorString;
}

qint64 DThumbnailProvider::defaultSizeLimit() const
{
    Q_D(const DThumbnailProvider);

    return d->defaultSizeLimit;
}

void DThumbnailProvider::setDefaultSizeLimit(qint64 size)
{
    Q_D(DThumbnailProvider);

    d->defaultSizeLimit = size;
}

qint64 DThumbnailProvider::sizeLimit(const QMimeType &mimeType) const
{
    Q_D(const DThumbnailProvider);

    return d->sizeLimitHash.value(mimeType, d->defaultSizeLimit);
}

void DThumbnailProvider::setSizeLimit(const QMimeType &mimeType, qint64 size)
{
    Q_D(DThumbnailProvider);

    d->sizeLimitHash[mimeType] = size;
}

DThumbnailProvider::DThumbnailProvider(QObject *parent)
    : QThread(parent)
    , DObject(*new DThumbnailProviderPrivate(this))
{
    d_func()->init();
}

DThumbnailProvider::~DThumbnailProvider()
{
    Q_D(DThumbnailProvider);

    d->running = false;
    d->waitCondition.wakeAll();
    wait();
}

void DThumbnailProvider::run()
{
    Q_D(DThumbnailProvider);

    Q_FOREVER {
        QWriteLocker locker(&d->dataReadWriteLock);

        if (d->produceQueue.isEmpty()) {
            d->waitCondition.wait(&d->dataReadWriteLock);
        }

        if (!d->running)
            return;

        const DThumbnailProviderPrivate::ProduceInfo &task = d->produceQueue.dequeue();
        const QPair<QString, DThumbnailProvider::Size> &tmpKey = qMakePair(task.fileInfo.absoluteFilePath(), task.size);

        if (d->discardedProduceInfos.contains(tmpKey)) {
            d->discardedProduceInfos.remove(tmpKey);
            locker.unlock();
            continue;
        }

        locker.unlock();

        const QString &thumbnail = createThumbnail(task.fileInfo, task.size);

        if (task.callback)
            task.callback(thumbnail);
    }
}

DWIDGET_END_NAMESPACE
