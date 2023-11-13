// SPDX-FileCopyrightText: 2017 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "ddesktopservices.h"
#include <QtDBus/QtDBus>
#include <QDebug>
#include <QFile>

#include <DDesktopServices>
using DGuiDesktopServices = DTK_GUI_NAMESPACE::DDesktopServices;
using DGuiSystemSoundEffect = DGuiDesktopServices::SystemSoundEffect;

DWIDGET_BEGIN_NAMESPACE

bool DDesktopServices::showFolder(QString localFilePath, const QString &startupId)
{
    return DGuiDesktopServices::showFolder(localFilePath, startupId);
}

bool DDesktopServices::showFolders(const QList<QString> localFilePaths, const QString &startupId)
{
    return DGuiDesktopServices::showFolders(localFilePaths, startupId);
}

bool DDesktopServices::showFolder(QUrl url, const QString &startupId)
{
    return DGuiDesktopServices::showFolder(url, startupId);
}

bool DDesktopServices::showFolders(const QList<QUrl> urls, const QString &startupId)
{
    return DGuiDesktopServices::showFolders(urls, startupId);
}

bool DDesktopServices::showFileItemPropertie(QString localFilePath, const QString &startupId)
{
    return DGuiDesktopServices::showFileItemProperty(localFilePath , startupId);
}

bool DDesktopServices::showFileItemProperties(const QList<QString> localFilePaths, const QString &startupId)
{
    return DGuiDesktopServices::showFileItemProperties(localFilePaths, startupId);
}

bool DDesktopServices::showFileItemPropertie(QUrl url, const QString &startupId)
{
    return DGuiDesktopServices::showFileItemProperty(url, startupId);
}

bool DDesktopServices::showFileItemProperties(const QList<QUrl> urls, const QString &startupId)
{
    return DGuiDesktopServices::showFileItemProperties(urls, startupId);
}

bool DDesktopServices::showFileItem(QString localFilePath, const QString &startupId)
{
    return DGuiDesktopServices::showFileItem(localFilePath, startupId);
}

bool DDesktopServices::showFileItems(const QList<QString> localFilePaths, const QString &startupId)
{
    return DGuiDesktopServices::showFileItems(localFilePaths, startupId);
}

bool DDesktopServices::showFileItem(QUrl url, const QString &startupId)
{
    return DGuiDesktopServices::showFileItem(url, startupId);
}

bool DDesktopServices::showFileItems(const QList<QUrl> urls, const QString &startupId)
{
    return DGuiDesktopServices::showFileItems(urls, startupId);
}

bool DDesktopServices::trash(QString localFilePath)
{
    return DGuiDesktopServices::trash(localFilePath);
}

bool DDesktopServices::trash(const QList<QString> localFilePaths)
{
    return DGuiDesktopServices::trash(localFilePaths);
}

bool DDesktopServices::trash(QUrl url)
{
    return DGuiDesktopServices::trash(url);
}

bool DDesktopServices::trash(const QList<QUrl> urls)
{
    return DGuiDesktopServices::trash(urls);
}

bool DDesktopServices::playSystemSoundEffect(const DDesktopServices::SystemSoundEffect &effect)
{
    return DGuiDesktopServices::playSystemSoundEffect(DGuiSystemSoundEffect(effect));
}

bool DDesktopServices::playSystemSoundEffect(const QString &name)
{
    return DGuiDesktopServices::playSystemSoundEffect(name);
}

bool DDesktopServices::previewSystemSoundEffect(const DDesktopServices::SystemSoundEffect &effect)
{
    return DGuiDesktopServices::previewSystemSoundEffect(DGuiSystemSoundEffect(effect));
}

bool DDesktopServices::previewSystemSoundEffect(const QString &name)
{
    return DGuiDesktopServices::previewSystemSoundEffect(name);
}

QString DDesktopServices::getNameByEffectType(const DDesktopServices::SystemSoundEffect &effect)
{
    return DGuiDesktopServices::getNameByEffectType(DGuiSystemSoundEffect(effect));
}

QString DDesktopServices::errorMessage()
{
    return DGuiDesktopServices::errorMessage();
}

DWIDGET_END_NAMESPACE
