/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#ifdef DTK_DBUS_SINGLEINSTANCE
#include <QDBusError>
#include <QDBusConnection>
#endif

#include <qpa/qplatformintegrationfactory_p.h>

#include <DStandardPaths>

#include "dapplication.h"
#include "dthememanager.h"
#include "private/dapplication_p.h"
#include "daboutdialog.h"

#ifdef Q_OS_LINUX
#include "startupnotificationmonitor.h"
#endif

#define DXCB_PLUGIN_KEY "dxcb"
#define DXCB_PLUGIN_SYMBOLIC_PROPERTY "_d_isDxcb"

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

    if (singleInstance) {
        return true;
    }

    Q_ASSERT_X(ss.error() == QSystemSemaphore::NoError, "DApplicationPrivate::setSingleInstanceBySemaphore:", ss.errorString().toLocal8Bit().constData());

    singleInstance = tryAcquireSystemSemaphore(&ss);

    if (singleInstance) {
        QtConcurrent::run([] {
            while (ss.acquire() && singleInstance)
            {
                if (qApp->startingUp() || qApp->closingDown()) {
                    break;
                }

                ss.release(1);

                Q_EMIT qApp->newInstanceStarted();
            }
        });

        auto clean_fun = [] {
            ss.release(1);
            singleInstance = false;
        };

        qAddPostRoutine(clean_fun);
        atexit(clean_fun);
    }

    return singleInstance;
}

#ifdef DTK_DBUS_SINGLEINSTANCE
/**
* \brief DApplicationPrivate::setSingleInstanceByDbus will check singleinstance by
* register dbus service
* \param key is the last of dbus service name, like "com.deepin.SingleInstance.key"
* \return
*/
bool DApplicationPrivate::setSingleInstanceByDbus(const QString &key)
{
    auto basename = "com.deepin.SingleInstance.";
    QString name = basename + key;
    auto sessionBus = QDBusConnection::sessionBus();
    if (!sessionBus.registerService(name)) {
        qDebug() << "register service failed:" << sessionBus.lastError();
        return  false;
    }
    return true;
}
#endif

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

/**
 * @brief DApplication::DApplication constructs an instance of DApplication.
 * @param argc is the same as in the main function.
 * @param argv is the same as in the main function.
 */
DApplication::DApplication(int &argc, char **argv) :
    QApplication(argc, argv),
    DObject(*new DApplicationPrivate(this))
{
    qputenv("QT_QPA_PLATFORM", QByteArray());
}

/**
 * @brief DApplication::theme returns name of the theme that the application is currently using.
 *
 * theme name can be one of light, dark, semidark or semilight.
 *
 * @return the theme name.
 */
QString DApplication::theme() const
{
    D_DC(DApplication);

    return d->theme();
}

/**
 * @brief DApplication::setTheme for the application to use the theme we provide.
 * @param theme is the name of the theme we want to set.
 */
void DApplication::setTheme(const QString &theme)
{
    D_D(DApplication);

    d->setTheme(theme);
}

/**
 * @brief DApplication::setSingleInstance marks this application to be single instanced.
 * @param key is used as the unique ID of every application.
 *
 * It should be in form of dde-dock, dde-desktop or dde-control-center etc.
 *
 * You can use dbus implement if you use an sandbox like flatpak and so on, just
 * build with DTK_DBUS_SINGLEINSTANCE
 *
 * @return true if succeed, otherwise false.
 */
bool DApplication::setSingleInstance(const QString &key)
{
    D_D(DApplication);
#ifdef DTK_DBUS_SINGLEINSTANCE
    return d->setSingleInstanceByDbus(key);
#else
    return d->setSingleInstanceBySemaphore(key);
#endif
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

    // fix QGuiApplication::platformName() to xcb
    qputenv("DXCB_FAKE_PLATFORM_NAME_XCB", "true");

    return qputenv("QT_QPA_PLATFORM", DXCB_PLUGIN_KEY);
}

bool DApplication::isDXcbPlatform()
{
    return qApp && (qApp->platformName() == DXCB_PLUGIN_KEY || qApp->property(DXCB_PLUGIN_SYMBOLIC_PROPERTY).toBool());
}

/**
 * @brief DApplication::productName returns the product name of this application.
 *
 * It's mainly used to construct an about dialog of the application.
 *
 * @return the product name of this application if set, otherwise the applicationDisplayName.
 */
QString DApplication::productName() const
{
    D_DC(DApplication);

    return d->productName.isEmpty() ? applicationDisplayName() : d->productName;
}

/**
 * @brief DApplication::setProductName sets the product name of this application.
 * @param productName is the product name to be set.
 */
void DApplication::setProductName(const QString &productName)
{
    D_D(DApplication);

    d->productName = productName;
}

/**
 * @brief DApplication::productIcon returns the product icon of this application.
 *
 * It's mainly used to construct an about dialog of the application.
 *
 * @return the product icon of this application if set, otherwise empty.
 */
const QIcon &DApplication::productIcon() const
{
    D_DC(DApplication);

    return d->productIcon;
}

/**
 * @brief DApplication::setProductIcon sets the product icon of this application.
 * @param productIcon is the product icon to be set.
 */
void DApplication::setProductIcon(const QPixmap &productIconPixmap)
{
    D_D(DApplication);

    d->productIcon = productIconPixmap;
}


/**
 * @brief DApplication::setProductIcon sets the product icon of this application.
 * @param productIcon is the product icon to be set.
 */
void DApplication::setProductIcon(const QIcon &productIcon)
{
    D_D(DApplication);

    d->productIcon = productIcon;
}

/**
 * @brief DApplication::applicationLicense returns the license used by this application.
 *
 * It's mainly used to construct an about dialog of the application.
 *
 * @return the license used by this application.
 */
QString DApplication::applicationLicense() const
{
    D_DC(DApplication);

    return d->appLicense;
}

/**
 * @brief DApplication::setApplicationLicense sets the license of this application.
 * @param license is the license to be set.
 */
void DApplication::setApplicationLicense(const QString &license)
{
    D_D(DApplication);

    d->appLicense = license;
}

/**
 * @brief DApplication::applicationDescription returns the long description of the application.
 *
 * It's mainly used to construct an about dialog of the application.
 *
 * @return the description of the application if set, otherwise empty.
 */
QString DApplication::applicationDescription() const
{
    D_DC(DApplication);

    return d->appDescription;
}

/**
 * @brief DApplication::setApplicationDescription sets the description of the application.
 * @param description is description to be set.
 */
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

/**
 * @brief DApplication::applicationAcknowledgementPage returns the acknowlegement page of the application.
 *
 * It's mainly used to construct an about dialog of the application.
 *
 * @return the acknowlegement page of the application if set, otherwise empty.
 */
QString DApplication::applicationAcknowledgementPage() const
{
    D_DC(DApplication);

    return d->acknowledgementPage;
}

/**
 * @brief DApplication::setApplicationAcknowledgementPage sets the acknowlegement page of the application.
 * @param link is the acknowlegement page link to be shown in the about dialog.
 */
void DApplication::setApplicationAcknowledgementPage(const QString &link)
{
    D_D(DApplication);

    d->acknowledgementPage = link;
}

/**
 * @brief DApplication::aboutDialog returns the about dialog of this application.
 *
 * If the about dialog is not set, it will automatically construct one.
 *
 * @return the about dialog instance.
 */
DAboutDialog *DApplication::aboutDialog()
{
    D_D(DApplication);

    return d->aboutDialog;
}

/**
 * @brief DApplication::setAboutDialog sets the about dialog of this application.
 *
 * It's mainly used to override the auto-constructed about dialog which is not
 * a common case, so please do double check before using this method.
 *
 * @param aboutDialog
 */
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
