/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QDebug>
#include <QDir>
#include <QLocalSocket>
#include <QLibraryInfo>
#include <QTranslator>
#include <QLocalServer>
#include <QProcess>
#include <QMenu>
#include <QStyleFactory>
#include <QSystemSemaphore>
#include <QtConcurrent/QtConcurrent>

#include <qpa/qplatformintegrationfactory_p.h>

#include <DStandardPaths>

#include "dapplication.h"
#include "dthememanager.h"
#include "private/dthemehelper.h"
#include "private/dapplication_p.h"
#include "daboutdialog.h"

#ifdef Q_OS_LINUX
#include "startupnotificationmonitor.h"
#endif

#define DXCB_PLUGIN_KEY "dxcb"

DCORE_USE_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

DApplicationPrivate::DApplicationPrivate(DApplication *q) :
    DObjectPrivate(q)
{
#ifdef Q_OS_LINUX
    StartupNotificationMonitor *monitor = StartupNotificationMonitor::instance();
    QObject::connect(monitor, &StartupNotificationMonitor::appStartup, [this, q](const QString id) {
        m_monitoredStartupApps.append(id);
        q->setOverrideCursor(Qt::WaitCursor);
    });
    QObject::connect(monitor, &StartupNotificationMonitor::appStartupCompleted, [this, q](const QString id) {
        m_monitoredStartupApps.removeAll(id);
        if (m_monitoredStartupApps.isEmpty()) {
            q->setOverrideCursor(Qt::ArrowCursor);
        }
    });
#endif
}

DApplicationPrivate::~DApplicationPrivate()
{
    if (m_localServer) {
        m_localServer->close();
    }
}

QString DApplicationPrivate::theme() const
{
    return DThemeManager::instance()->theme();
}

void DApplicationPrivate::setTheme(const QString &theme)
{
    DThemeManager *themeManager = DThemeManager::instance();
    themeManager->setTheme(theme);
}

bool DApplicationPrivate::setSingleInstance(const QString &key)
{
    D_Q(DApplication);

    if (m_localServer) {
        return m_localServer->isListening();
    }

    QLocalSocket *localSocket = new QLocalSocket;
    localSocket->connectToServer(key);

    // if connect success, another instance is running.
    bool result = localSocket->waitForConnected(1000);
    localSocket->deleteLater();

    if (result) {
        return false;
    }

    // create local server
    m_localServer = new QLocalServer(q);
    m_localServer->removeServer(key);

    QObject::connect(m_localServer, &QLocalServer::newConnection, q, &DApplication::newInstanceStarted);

    return m_localServer->listen(key);
}

static bool tryAcquireSystemSemaphore(QSystemSemaphore *ss, qint64 timeout = 10)
{
    if (ss->error() != QSystemSemaphore::NoError) {
        return false;
    }

    QSystemSemaphore _tmp_ss(QString("%1-%2").arg("DTK::tryAcquireSystemSemaphore").arg(ss->key()), 1, QSystemSemaphore::Open);

    _tmp_ss.acquire();

    QElapsedTimer t;
    QFuture<bool> request = QtConcurrent::run(ss, &QSystemSemaphore::acquire);

    t.start();

    while (Q_LIKELY(t.elapsed() < timeout && !request.isFinished()));

    if (request.isFinished()) {
        return true;
    }

    if (Q_LIKELY(request.isRunning())) {
        if (Q_LIKELY(ss->release(1))) {
            request.waitForFinished();
        }
    }

    return false;
}

bool DApplicationPrivate::setSingleInstanceBySemaphore(const QString &key)
{
    static QSystemSemaphore ss(key, 1, QSystemSemaphore::Open);
    static bool singleInstance = false;

    if (singleInstance)
        return true;

    Q_ASSERT_X(ss.error() == QSystemSemaphore::NoError, "DApplicationPrivate::setSingleInstanceBySemaphore:", ss.errorString().toLocal8Bit().constData());

    singleInstance = tryAcquireSystemSemaphore(&ss);

    if (singleInstance) {
        QtConcurrent::run([] {
            while (ss.acquire()) {
                if (qApp->startingUp())
                    break;

                ss.release(1);

                Q_EMIT qApp->newInstanceStarted();
            }
        });

        qAddPostRoutine([] {
            ss.release(1);
        });
    }

    return singleInstance;
}

bool DApplicationPrivate::loadDtkTranslator(QList<QLocale> localeFallback)
{
    D_Q(DApplication);

    auto qtTranslator = new QTranslator(q);
    qtTranslator->load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    q->installTranslator(qtTranslator);

    auto qtbaseTranslator = new QTranslator(q);
    qtTranslator->load("qtbase_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    q->installTranslator(qtbaseTranslator);

    QList<DPathBuf> translateDirs;
    auto dtkwidgetDir = "dtkwidget";
    auto dtkwidgetName = "dtkwidget2";

    //("/home/user/.local/share", "/usr/local/share", "/usr/share")
    auto dataDirs = DStandardPaths::standardLocations(QStandardPaths::GenericDataLocation);
    for (const auto &path : dataDirs) {
        DPathBuf DPathBuf(path);
        translateDirs << DPathBuf / dtkwidgetDir / "translations";
    }

    DPathBuf runDir(q->applicationDirPath());
    translateDirs << runDir.join("translations");

    DPathBuf currentDir(QDir::currentPath());
    translateDirs << currentDir.join("translations");

    return loadTranslator(translateDirs, dtkwidgetName, localeFallback);
}

bool DApplicationPrivate::loadTranslator(QList<DPathBuf> translateDirs, const QString &name, QList<QLocale> localeFallback)
{
    D_Q(DApplication);

    for (auto &locale : localeFallback) {
        QString translateFilename = QString("%1_%2").arg(name).arg(locale.name());
        for (auto &path : translateDirs) {
            QString translatePath = (path / translateFilename).toString();
            if (QFile::exists(translatePath + ".qm")) {
                qDebug() << "load translate" << translatePath;
                auto translator = new QTranslator(q);
                translator->load(translatePath);
                q->installTranslator(translator);
                return true;
            }
        }

        QStringList parseLocalNameList = locale.name().split("_", QString::SkipEmptyParts);
        if (parseLocalNameList.length() > 0) {
            translateFilename = QString("%1_%2").arg(name)
                                .arg(parseLocalNameList.at(0));
            for (auto &path : translateDirs) {
                QString translatePath = (path / translateFilename).toString();
                if (QFile::exists(translatePath + ".qm")) {
                    qDebug() << "translatePath after feedback:" << translatePath;
                    auto translator = new QTranslator(q);
                    translator->load(translatePath);
                    q->installTranslator(translator);
                    return true;
                }
            }
        }
    }

    qWarning() << name << "can not find qm files" ;
    return false;
}

DApplication::DApplication(int &argc, char **argv) :
    QApplication(argc, argv),
    DObject(*new DApplicationPrivate(this))
{
    qputenv("QT_QPA_PLATFORM", QByteArray());
}

QString DApplication::theme() const
{
    D_DC(DApplication);

    return d->theme();
}

void DApplication::setTheme(const QString &theme)
{
    D_D(DApplication);

    d->setTheme(theme);
}

bool DApplication::setSingleInstance(const QString &key)
{
    D_D(DApplication);

    return d->setSingleInstanceBySemaphore(key);
}

//! load translate file form system or application data path;
/*!
  \param localeFallback, a list of fallback locale you want load.
  \return load success
*/
bool DApplication::loadTranslator(QList<QLocale> localeFallback)
{
    D_D(DApplication);

    d->loadDtkTranslator(localeFallback);

    QList<DPathBuf> translateDirs;
    auto appName = applicationName();
    //("/home/user/.local/share", "/usr/local/share", "/usr/share")
    auto dataDirs = DStandardPaths::standardLocations(QStandardPaths::GenericDataLocation);
    for (const auto &path : dataDirs) {
        DPathBuf DPathBuf(path);
        translateDirs << DPathBuf / appName / "translations";
    }
    DPathBuf runDir(this->applicationDirPath());
    translateDirs << runDir.join("translations");
    DPathBuf currentDir(QDir::currentPath());
    translateDirs << currentDir.join("translations");

    return d->loadTranslator(translateDirs, appName, localeFallback);
}

bool DApplication::loadDXcbPlugin()
{
    Q_ASSERT_X(!qApp, "DApplication::loadDxcbPlugin", "Must call before QGuiApplication defined object");

    if (!QPlatformIntegrationFactory::keys().contains(DXCB_PLUGIN_KEY)) {
        return false;
    }

    return qputenv("QT_QPA_PLATFORM", DXCB_PLUGIN_KEY);
}

bool DApplication::isDXcbPlatform()
{
    return qApp && qApp->platformName() == "dxcb";
}

QString DApplication::productName() const
{
    D_DC(DApplication);

    return d->productName.isEmpty() ? applicationDisplayName() : d->productName;
}

void DApplication::setProductName(const QString &productName)
{
    D_D(DApplication);

    d->productName = productName;
}

const QPixmap &DApplication::productIcon() const
{
    D_DC(DApplication);

    return d->productIcon;
}

void DApplication::setProductIcon(const QPixmap &productIcon)
{
    D_D(DApplication);

    d->productIcon = productIcon;
}

QString DApplication::applicationLicense() const
{
    D_DC(DApplication);

    return d->appLicense;
}

void DApplication::setApplicationLicense(const QString &license)
{
    D_D(DApplication);

    d->appLicense = license;
}

QString DApplication::applicationDescription() const
{
    D_DC(DApplication);

    return d->appDescription;
}

void DApplication::setApplicationDescription(const QString &description)
{
    D_D(DApplication);

    d->appDescription = description;
}

QString DApplication::applicationHomePage() const
{
    D_DC(DApplication);

    return d->homePage;
}

void DApplication::setApplicationHomePage(const QString &link)
{
    D_D(DApplication);

    d->homePage = link;
}

QString DApplication::applicationAcknowledgementPage() const
{
    D_DC(DApplication);

    return d->acknowledgementPage;
}

void DApplication::setApplicationAcknowledgementPage(const QString &link)
{
    D_D(DApplication);

    d->acknowledgementPage = link;
}

DAboutDialog *DApplication::aboutDialog()
{
    D_D(DApplication);

    return d->aboutDialog;
}

void DApplication::setAboutDialog(DAboutDialog *aboutDialog)
{
    D_D(DApplication);

    if (d->aboutDialog && d->aboutDialog != aboutDialog) {
        d->aboutDialog->deleteLater();
    }

    d->aboutDialog = aboutDialog;
}

/**
 * @brief DApplication::helpActionHandler
 *
 * Triggered when user clicked the help menu item of this window's titlebar,
 * default action is to open the user manual of this program, override this
 * method if you want to change the default action.
 *
 */
void DApplication::handleHelpAction()
{
    const QString appName = applicationName();
    QProcess::startDetached("dman", QStringList() << appName);
}

/**
 * @brief DApplication::aboutActionHandler
 *
 * Triggered when user clicked the about menu item of this window's titlebar,
 * default action is to show the about dialog of this window(if there is one),
 * override this method if you want to change the default action.
 *
 */
void DApplication::handleAboutAction()
{
    D_D(DApplication);

    if (d->aboutDialog) {
        d->aboutDialog->show();
        return;
    }

    DAboutDialog *aboutDialog = new DAboutDialog(activeWindow());
    aboutDialog->setProductName(productName());
    aboutDialog->setProductIcon(productIcon());
    aboutDialog->setVersion(translate("DAboutDialog", "Version: %1").arg(applicationVersion()));
    aboutDialog->setDescription(applicationDescription());

    if (!applicationLicense().isEmpty()) {
        aboutDialog->setLicense(translate("DAboutDialog", "%1 is released under %2").arg(productName()).arg(applicationLicense()));
    }
    if (!applicationAcknowledgementPage().isEmpty()) {
        aboutDialog->setAcknowledgementLink(applicationAcknowledgementPage());
    }

    aboutDialog->exec();
    aboutDialog->deleteLater();
}

/**
 * @brief DApplication::quitActionHandler
 *
 * Triggered when user clicked the exit menu item of this window's titlebar,
 * default action is to quit this program, you can try to save your data before
 * the program quitting by connecting to the aboutToQuit signal of this application.
 * override this method if you want to change the default action.
 *
 */
void DApplication::handleQuitAction()
{
    quit();
}

bool DApplication::notify(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::PolishRequest) {
        // Fixed the style for the menu widget to dlight
        // ugly code will no longer needed.
        if (QMenu *menu = qobject_cast<QMenu *>(obj)) {
            if (!menu->testAttribute(Qt::WA_SetStyle))
                if (QStyle *style = QStyleFactory::create("dlight")) {
                    menu->setStyle(style);
                }
        }
    }

    return QApplication::notify(obj, event);
}

DWIDGET_END_NAMESPACE
