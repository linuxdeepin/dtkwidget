/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dfilesystemwatcher.h"
#include "private/dfilesystemwatcher_win_p.h"

DUTIL_BEGIN_NAMESPACE

DFileSystemWatcherPrivate::DFileSystemWatcherPrivate(int fd, DFileSystemWatcher *qq)
    : DObjectPrivate(qq)
{

}

DFileSystemWatcherPrivate::~DFileSystemWatcherPrivate()
{

}

/*!
    \class DFileSystemWatcher
    \inmodule QtCore
    \brief The DFileSystemWatcher class provides an interface for monitoring files and directories for modifications.
    \ingroup io
    \since 4.2
    \reentrant

    DFileSystemWatcher monitors the file system for changes to files
    and directories by watching a list of specified paths.

    Call addPath() to watch a particular file or directory. Multiple
    paths can be added using the addPaths() function. Existing paths can
    be removed by using the removePath() and removePaths() functions.

    DFileSystemWatcher examines each path added to it. Files that have
    been added to the DFileSystemWatcher can be accessed using the
    files() function, and directories using the directories() function.

    The fileChanged() signal is emitted when a file has been modified,
    renamed or removed from disk. Similarly, the directoryChanged()
    signal is emitted when a directory or its contents is modified or
    removed.  Note that DFileSystemWatcher stops monitoring files once
    they have been renamed or removed from disk, and directories once
    they have been removed from disk.

    \note On systems running a Linux kernel without inotify support,
    file systems that contain watched paths cannot be unmounted.

    \note Windows CE does not support directory monitoring by
    default as this depends on the file system driver installed.

    \note The act of monitoring files and directories for
    modifications consumes system resources. This implies there is a
    limit to the number of files and directories your process can
    monitor simultaneously. On all BSD variants, for
    example, an open file descriptor is required for each monitored
    file. Some system limits the number of open file descriptors to 256
    by default. This means that addPath() and addPaths() will fail if
    your process tries to add more than 256 files or directories to
    the file system monitor. Also note that your process may have
    other file descriptors open in addition to the ones for files
    being monitored, and these other open descriptors also count in
    the total. OS X uses a different backend and does not
    suffer from this issue.


    \sa QFile, QDir
*/


/*!
    Constructs a new file system watcher object with the given \a parent.
*/
DFileSystemWatcher::DFileSystemWatcher(QObject *parent)
    : QObject(parent)
    , DObject()
{

}

/*!
    Constructs a new file system watcher object with the given \a parent
    which monitors the specified \a paths list.
*/
DFileSystemWatcher::DFileSystemWatcher(const QStringList &paths, QObject *parent)
    : DFileSystemWatcher(parent)
{
    addPaths(paths);
}

/*!
    Destroys the file system watcher.
*/
DFileSystemWatcher::~DFileSystemWatcher()
{ }

/*!
    Adds \a path to the file system watcher if \a path exists. The
    path is not added if it does not exist, or if it is already being
    monitored by the file system watcher.

    If \a path specifies a directory, the directoryChanged() signal
    will be emitted when \a path is modified or removed from disk;
    otherwise the fileChanged() signal is emitted when \a path is
    modified, renamed or removed.

    If the watch was successful, true is returned.

    Reasons for a watch failure are generally system-dependent, but
    may include the resource not existing, access failures, or the
    total watch count limit, if the platform has one.

    \note There may be a system dependent limit to the number of
    files and directories that can be monitored simultaneously.
    If this limit is been reached, \a path will not be monitored,
    and false is returned.

    \sa addPaths(), removePath()
*/
bool DFileSystemWatcher::addPath(const QString &path)
{
    return false;
}

/*!
    Adds each path in \a paths to the file system watcher. Paths are
    not added if they not exist, or if they are already being
    monitored by the file system watcher.

    If a path specifies a directory, the directoryChanged() signal
    will be emitted when the path is modified or removed from disk;
    otherwise the fileChanged() signal is emitted when the path is
    modified, renamed, or removed.

    The return value is a list of paths that could not be watched.

    Reasons for a watch failure are generally system-dependent, but
    may include the resource not existing, access failures, or the
    total watch count limit, if the platform has one.

    \note There may be a system dependent limit to the number of
    files and directories that can be monitored simultaneously.
    If this limit has been reached, the excess \a paths will not
    be monitored, and they will be added to the returned QStringList.

    \sa addPath(), removePaths()
*/
QStringList DFileSystemWatcher::addPaths(const QStringList &paths)
{
    return QStringList();
}

/*!
    Removes the specified \a path from the file system watcher.

    If the watch is successfully removed, true is returned.

    Reasons for watch removal failing are generally system-dependent,
    but may be due to the path having already been deleted, for example.

    \sa removePaths(), addPath()
*/
bool DFileSystemWatcher::removePath(const QString &path)
{
    return false;
}

/*!
    Removes the specified \a paths from the file system watcher.

    The return value is a list of paths which were not able to be
    unwatched successfully.

    Reasons for watch removal failing are generally system-dependent,
    but may be due to the path having already been deleted, for example.

    \sa removePath(), addPaths()
*/
QStringList DFileSystemWatcher::removePaths(const QStringList &paths)
{
    return QStringList();
}

/*!
    \fn void DFileSystemWatcher::fileChanged(const QString &path)

    This signal is emitted when the file at the specified \a path is
    modified, renamed or removed from disk.

    \sa directoryChanged()
*/

/*!
    \fn void DFileSystemWatcher::directoryChanged(const QString &path)

    This signal is emitted when the directory at a specified \a path
    is modified (e.g., when a file is added or deleted) or removed
    from disk. Note that if there are several changes during a short
    period of time, some of the changes might not emit this signal.
    However, the last change in the sequence of changes will always
    generate this signal.

    \sa fileChanged()
*/

/*!
    \fn QStringList DFileSystemWatcher::directories() const

    Returns a list of paths to directories that are being watched.

    \sa files()
*/

/*!
    \fn QStringList DFileSystemWatcher::files() const

    Returns a list of paths to files that are being watched.

    \sa directories()
*/

QStringList DFileSystemWatcher::directories() const
{
    return QStringList();
}

QStringList DFileSystemWatcher::files() const
{
    return QStringList();
}

DUTIL_END_NAMESPACE

#include "moc_dfilesystemwatcher.cpp"
