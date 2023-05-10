/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *

 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
    a->setApplicationDescription(QApplication::translate("main", "Collections provides the examples for dtk applications."));
    a->setApplicationCreditsFile(":/resources/data/example-license.json");
    a->setLicensePath(":/resources/data");

    DApplicationSettings as;
    Q_UNUSED(as)

    MainWindow w;
    w.show();

    Dtk::Widget::moveToCenter(&w);

    return a->exec();
}
