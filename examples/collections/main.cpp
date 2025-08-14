// SPDX-FileCopyrightText: 2015 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "mainwindow.h"

#include <DApplication>
#include <DWidgetUtil>
#include <DLog>

#include <QDebug>

DCORE_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
#if defined(DTK_STATIC_LIB)
    DWIDGET_INIT_RESOURCE();
#endif
    DApplication *a = DApplication::globalApplication(argc, argv);

#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    DApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
#endif
    DLogManager::registerConsoleAppender();

    a->loadTranslator();
#ifdef Q_OS_UNIX
    a->setOOMScoreAdj(500);
#endif

    a->setAutoActivateWindows(true);
    if (!a->setSingleInstance("deepin-tool-kit-examples")) {
        qDebug() << "another instance is running!!";
        return 0;
    }

    a->setApplicationName("dtk-example");
    a->setOrganizationName("deepin");
    a->setApplicationVersion("1.0");
    a->setProductIcon(QIcon(":/images/logo_icon.svg"));
    a->setWindowIcon(QIcon(":/images/logo_icon.svg"));
    a->setApplicationDescription(QApplication::translate("main", "Collections provides the examples for dtk applications."));
    a->setApplicationDisplayName(QObject::tr("Collections"));
    a->setApplicationLicense(QObject::tr("2023 UnionTech Software Technology Co., Ltd."));
    a->setProperty("applicationCreditsFile", ":/resources/data/example-license.json");
    a->setProperty("licensePath", ":/resources/data");

    MainWindow w;
    w.show();

    Dtk::Widget::moveToCenter(&w);

    return a->exec();
}
