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

#include "ddesktopservices.h"

#include <QDBusInterface>
#include <QDBusPendingCall>
#include <QDebug>
#include <QFile>
#include <QMediaPlayer>
#include <QGSettings/QGSettings>
#include <QSound>

DWIDGET_BEGIN_NAMESPACE

#define EASY_CALL_DBUS(name)\
    QDBusInterface *interface = fileManager1DBusInterface();\
    return interface && interface->call(#name, urls2uris(urls), startupId).type() != QDBusMessage::ErrorMessage;

static const QStringList SOUND_EFFECT_LIST {
    "message",
    "camera-shutter",
    "trash-empty",
    "x-deepin-app-sent-to-desktop",
    "desktop-login",
    "system-shutdown",
    "desktop-logout",
    "suspend-resume",
    "audio-volume-change",
    "power-unplug-battery-low",
    "power-plug",
    "power-unplug",
    "device-added",
    "device-removed",
    "dialog-error",
};

static QDBusInterface *fileManager1DBusInterface()
{
    static QDBusInterface interface(QStringLiteral("org.freedesktop.FileManager1"),
                                        QStringLiteral("/org/freedesktop/FileManager1"),
                                        QStringLiteral("org.freedesktop.FileManager1"));
    return &interface;
}

static QStringList urls2uris(const QList<QUrl> &urls)
{
    QStringList list;

    list.reserve(urls.size());

    for (const QUrl &url : urls) {
        list << url.toString();
    }

    return list;
}

static QList<QUrl> path2urls(const QList<QString> &paths)
{
    QList<QUrl> list;

    list.reserve(paths.size());

    for (const QString &path : paths) {
        list << QUrl::fromLocalFile(path);
    }

    return list;
}

static QMediaPlayer *soundEffectPlayer()
{
    static QMediaPlayer *player = Q_NULLPTR;

    if (!player) {
        player = new QMediaPlayer;
        player->setVolume(70);
    }

    return player;
}

static QString soundEffectFilePath(const QString &name)
{
    // TODO: super simple version of sound theme file search shema :)
    // will need to be replaced by more advanced approch like libcanberra.
    QString temp = QString("/usr/share/sounds/deepin/stereo/%1").arg(name);

    const QString tempWav = temp + ".wav";
    if (QFile::exists(tempWav)) {
        return tempWav;
    }

    const QString tempOgg = temp + ".ogg";
    if (QFile::exists(tempOgg)) {
        return tempOgg;
    }

    return QString();
}

/*!
 * \brief Converts \p name from 'key-name' notation
 *        to camel case 'keyName' or 'KeyName'.
 * This function converts GSettings key names to names
 * suitable for Qt getters and setters.
 *
 * Well fsck this.
 *
 * \param name key name
 * \return key name converted to camel case
 * \internal
 */
static QString GSettingsKeyToCamelCase(const QString name)
{
    QStringList parts = name.split('-', QString::SkipEmptyParts);
    for (int i=1; i<parts.size(); ++i)
        parts[i][0] = parts[i][0].toUpper();

    return parts.join("");
}

static bool systemSoundEffectEnabled(const QString &name)
{
    QGSettings settings("com.deepin.dde.sound-effect");

    const bool effEnabled = settings.get("enabled").toBool();
    QString newName = GSettingsKeyToCamelCase(name);

    if (effEnabled) {
        const QStringList list = settings.keys();
        if (!list.contains(newName)) {
            return false;
        }

        return settings.get(newName).toBool();
    }

    return effEnabled;
}

bool DDesktopServices::showFolder(QString localFilePath, const QString &startupId)
{
    return showFolder(QUrl::fromLocalFile(localFilePath), startupId);
}

bool DDesktopServices::showFolders(const QList<QString> localFilePaths, const QString &startupId)
{
    return showFolders(path2urls(localFilePaths), startupId);
}

bool DDesktopServices::showFolder(QUrl url, const QString &startupId)
{
    return showFolders(QList<QUrl>() << url, startupId);
}

bool DDesktopServices::showFolders(const QList<QUrl> urls, const QString &startupId)
{
    EASY_CALL_DBUS(ShowFolders)
}

bool DDesktopServices::showFileItemPropertie(QString localFilePath, const QString &startupId)
{
    return showFileItemPropertie(QUrl::fromLocalFile(localFilePath), startupId);
}

bool DDesktopServices::showFileItemProperties(const QList<QString> localFilePaths, const QString &startupId)
{
    return showFileItemProperties(path2urls(localFilePaths), startupId);
}

bool DDesktopServices::showFileItemPropertie(QUrl url, const QString &startupId)
{
    return showFileItemProperties(QList<QUrl>() << url, startupId);
}

bool DDesktopServices::showFileItemProperties(const QList<QUrl> urls, const QString &startupId)
{
    EASY_CALL_DBUS(ShowItemProperties)
}

bool DDesktopServices::showFileItem(QString localFilePath, const QString &startupId)
{
    return showFileItem(QUrl::fromLocalFile(localFilePath), startupId);
}

bool DDesktopServices::showFileItems(const QList<QString> localFilePaths, const QString &startupId)
{
    return showFileItems(path2urls(localFilePaths), startupId);
}

bool DDesktopServices::showFileItem(QUrl url, const QString &startupId)
{
    return showFileItems(QList<QUrl>() << url, startupId);
}

bool DDesktopServices::showFileItems(const QList<QUrl> urls, const QString &startupId)
{
    EASY_CALL_DBUS(ShowItems)
}

bool DDesktopServices::trash(QString localFilePath)
{
    return trash(QUrl::fromLocalFile(localFilePath));
}

bool DDesktopServices::trash(const QList<QString> localFilePaths)
{
    return trash(path2urls(localFilePaths));
}

bool DDesktopServices::trash(QUrl url)
{
    return trash(QList<QUrl>() << url);
}

bool DDesktopServices::trash(const QList<QUrl> urls)
{
    QDBusInterface *interface = fileManager1DBusInterface();
    return interface && interface->call("Trash", urls2uris(urls)).type() != QDBusMessage::ErrorMessage;
}

bool DDesktopServices::playSystemSoundEffect(const DDesktopServices::SystemSoundEffect &effect)
{
    return playSystemSoundEffect(SOUND_EFFECT_LIST.at(static_cast<int>(effect)));
}

bool DDesktopServices::playSystemSoundEffect(const QString &name)
{
    if (!systemSoundEffectEnabled(name)) {
        return false;
    }

    return previewSystemSoundEffect(name);
}

bool DDesktopServices::previewSystemSoundEffect(const DDesktopServices::SystemSoundEffect &effect)
{
    return previewSystemSoundEffect(SOUND_EFFECT_LIST.at(static_cast<int>(effect)));
}

bool DDesktopServices::previewSystemSoundEffect(const QString &name)
{
    const QString path = soundEffectFilePath(name);

    if (path.isEmpty()) {
        return false;
    }

    if (path.endsWith("wav")) {
        QSound::play(path);
    } else {
        QMediaPlayer *player = soundEffectPlayer();
        player->setMedia(QUrl::fromLocalFile(path));
        player->play();
    }

    return true;
}

QString DDesktopServices::getNameByEffectType(const DDesktopServices::SystemSoundEffect &effect)
{
    return SOUND_EFFECT_LIST.at(static_cast<int>(effect));
}

QString DDesktopServices::errorMessage()
{
    return fileManager1DBusInterface()->lastError().message();
}

DWIDGET_END_NAMESPACE
