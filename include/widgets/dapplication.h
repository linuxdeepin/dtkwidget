// SPDX-FileCopyrightText: 2015 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DAPPLICATION_H
#define DAPPLICATION_H

#include <dtkwidget_global.h>
#include <DObject>
#include <DPalette>

#include <QApplication>

DGUI_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

#define DAPPLICATION_XSTRING(s) DAPPLICATION_STRING(s)
#define DAPPLICATION_STRING(s) #s

class DApplication;
class DApplicationPrivate;
class DAboutDialog;
class DFeatureDisplayDialog;
class DAppHandler;

#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<DApplication *>(QCoreApplication::instance()))

class LIBDTKWIDGETSHARED_EXPORT DApplication : public QApplication, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DApplication)
    Q_PROPERTY(bool visibleMenuShortcutText READ visibleMenuShortcutText WRITE setVisibleMenuShortcutText)
    Q_PROPERTY(bool visibleMenuCheckboxWidget READ visibleMenuCheckboxWidget WRITE setVisibleMenuCheckboxWidget)
    Q_PROPERTY(bool visibleMenuIcon READ visibleMenuIcon WRITE setVisibleMenuIcon)
    Q_PROPERTY(bool autoActivateWindows READ autoActivateWindows WRITE setAutoActivateWindows)

public:
    static DApplication *globalApplication(int &argc, char **argv);

    DApplication(int &argc, char **argv);

    enum SingleScope {
        UserScope,
        SystemScope
    };

    D_DECL_DEPRECATED QString theme() const;
    D_DECL_DEPRECATED void setTheme(const QString &theme);

#ifdef Q_OS_UNIX
    void setOOMScoreAdj(const int score);
#endif

    bool setSingleInstance(const QString &key);
    bool setSingleInstance(const QString &key, SingleScope singleScope);

    bool loadTranslator(QList<QLocale> localeFallback = QList<QLocale>() << QLocale::system());

    //! warning: Must call before QGuiApplication defined object
    D_DECL_DEPRECATED static bool loadDXcbPlugin();
    static bool isDXcbPlatform();

    // return the libdtkwidget version of build application
    static int buildDtkVersion();
    // return the libdtkwidget version of runing application
    static int runtimeDtkVersion();

    // let startdde know that we've already started.
    static void registerDDESession();

    static void customQtThemeConfigPathByUserHome(const QString &home);
    static void customQtThemeConfigPath(const QString &path);
    static QString customizedQtThemeConfigPath();

    // meta information that necessary to create a about dialog for the application.
    QString productName() const;
    void setProductName(const QString &productName);

    const QIcon &productIcon() const;
    void setProductIcon(const QIcon &productIcon);

    QString applicationLicense() const;
    void setApplicationLicense(const QString &license);

    QString applicationDescription() const;
    void setApplicationDescription(const QString &description);

    QString applicationHomePage() const;
    void setApplicationHomePage(const QString &link);

    QString applicationAcknowledgementPage() const;
    void setApplicationAcknowledgementPage(const QString &link);

    bool applicationAcknowledgementVisible() const;
    void setApplicationAcknowledgementVisible(bool visible);

    DAboutDialog *aboutDialog();
    void setAboutDialog(DAboutDialog *aboutDialog);

    DFeatureDisplayDialog *featureDisplayDialog();
    void setFeatureDisplayDialog(DFeatureDisplayDialog *featureDisplayDialog);

    bool visibleMenuShortcutText() const;
    void setVisibleMenuShortcutText(bool value);

    bool visibleMenuCheckboxWidget() const;
    void setVisibleMenuCheckboxWidget(bool value);

    bool visibleMenuIcon() const;
    void setVisibleMenuIcon(bool value);

    bool autoActivateWindows() const;
    void setAutoActivateWindows(bool autoActivateWindows);

    // 使窗口内的输入框自动适应虚拟键盘
    void acclimatizeVirtualKeyboard(QWidget *window);
    void ignoreVirtualKeyboard(QWidget *window);
    bool isAcclimatizedVirtualKeyboard(QWidget *window) const;

#ifdef VERSION
    static inline QString buildVersion(const QString &fallbackVersion)
    {
        QString autoVersion = DAPPLICATION_XSTRING(VERSION);
        if (autoVersion.isEmpty()) {
            autoVersion = fallbackVersion;
        }
        return autoVersion;
    }
#else
    static inline QString buildVersion(const QString &fallbackVersion)
    {
        return fallbackVersion;
    }
#endif

Q_SIGNALS:
    void newInstanceStarted();

    //###(zccrs): Depend the Qt platform theme plugin(from the package: dde-qt5integration)
    void iconThemeChanged();
    //###(zccrs): Emit form the Qt platform theme plugin(from the package: dde-qt5integration)
    void screenDevicePixelRatioChanged(QScreen *screen);

public:
    void setCustomHandler(DAppHandler *handler);
    DAppHandler *customHandler();

protected:
    virtual void handleHelpAction();
    virtual void handleAboutAction();
    virtual void handleQuitAction();

public:
    bool notify(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;

private:
    friend class DTitlebarPrivate;
    friend class DMainWindowPrivate;

    D_PRIVATE_SLOT(void _q_onNewInstanceStarted())
    D_PRIVATE_SLOT(void _q_panWindowContentsForVirtualKeyboard())
    D_PRIVATE_SLOT(void _q_resizeWindowContentsForVirtualKeyboard())
    D_PRIVATE_SLOT(void _q_sizeModeChanged())
};

class LIBDTKWIDGETSHARED_EXPORT DAppHandler {
public:
    inline virtual ~DAppHandler() = default;

    virtual void handleHelpAction() = 0;
    virtual void handleAboutAction() = 0;
    virtual void handleQuitAction() = 0;
};

class DtkBuildVersion {
public:
    static int value;
};

#ifndef LIBDTKWIDGET_LIBRARY
class Q_DECL_HIDDEN _DtkBuildVersion {
public:
    _DtkBuildVersion() {
        DtkBuildVersion::value = DTK_VERSION;
    }
};

static _DtkBuildVersion _dtk_build_version;
#endif

DWIDGET_END_NAMESPACE

#endif // DAPPLICATION_H
