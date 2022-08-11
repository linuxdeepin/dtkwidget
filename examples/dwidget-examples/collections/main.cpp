// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "mainwindow.h"

#include <DApplication>
#include <DApplicationSettings>
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

    DApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
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

    DApplicationSettings as;
    Q_UNUSED(as)

    MainWindow w;
    w.show();

    Dtk::Widget::moveToCenter(&w);

    return a->exec();
}
