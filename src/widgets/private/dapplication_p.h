// SPDX-FileCopyrightText: 2015 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DAPPLICATION_P_H
#define DAPPLICATION_P_H

#include "dsizemode.h"
#include <DObjectPrivate>
#include <DApplication>
#include <DPathBuf>

#include <QIcon>
#include <QPointer>

class QLocalServer;
class QTranslator;

DWIDGET_BEGIN_NAMESPACE
DCORE_USE_NAMESPACE

class DAboutDialog;
class DFeatureDisplayDialog;

class DApplicationPrivate : public DObjectPrivate
{

public:

    D_DECLARE_PUBLIC(DApplication)

    explicit DApplicationPrivate(DApplication *q);
    ~DApplicationPrivate();
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
    D_DECL_DEPRECATED QString theme() const;
    D_DECL_DEPRECATED void setTheme(const QString &theme);
#endif    

    bool setSingleInstanceBySemaphore(const QString &key);
#ifdef Q_OS_UNIX
    bool setSingleInstanceByDbus(const QString &key);
#endif

    bool loadDtkTranslator(QList<QLocale> localeFallback);
    void _q_onNewInstanceStarted();

    // 为控件适应当前虚拟键盘的位置
    void doAcclimatizeVirtualKeyboard(QWidget *window, QWidget *widget, bool allowResizeContentsMargins);
    void acclimatizeVirtualKeyboardForFocusWidget(bool allowResizeContentsMargins);
    void _q_panWindowContentsForVirtualKeyboard();
    void _q_resizeWindowContentsForVirtualKeyboard();
    void _q_sizeModeChanged();
    void handleSizeModeChangeEvent(QWidget *widget, QEvent *event);

    static bool isUserManualExists();
public:
// int m_pidLockFD = 0;
    QLocalServer *m_localServer = nullptr;
#ifdef Q_OS_UNIX
    QStringList m_monitoredStartupApps;
#endif

    QString productName;
    QIcon   productIcon;
    QString appLicense;
    QString appDescription;
    QString homePage;
    QString acknowledgementPage;
    QString applicationCreditsFile;
    QByteArray applicationCreditsContent;
    QString licensePath;

    bool acknowledgementPageVisible = true;

    bool visibleMenuShortcutText   = false;
    bool visibleMenuCheckboxWidget = false;
    bool visibleMenuIcon           = false;
    bool autoActivateWindows       = false;

    DAppHandler *appHandler = Q_NULLPTR;
    DAboutDialog *aboutDialog = Q_NULLPTR;
    DFeatureDisplayDialog *featureDisplayDialog = Q_NULLPTR;
    DLicenseDialog *licenseDialog = Q_NULLPTR;

    // 需要自适应虚拟键盘环境的窗口
    QPointer<QWidget> activeInputWindow;
    // 上一次为适配虚拟键盘所设置的值
    QPair<int, int> lastContentsMargins;
    QMargins activeInputWindowContentsMargins;
    QList<QWidget*> acclimatizeVirtualKeyboardWindows;
};

DWIDGET_END_NAMESPACE

#endif // DAPPLICATION_P_H
