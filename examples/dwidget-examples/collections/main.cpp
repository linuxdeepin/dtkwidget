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

#include <DTitlebar>
#include <DApplication>
#include <DApplicationSettings>
#include <DWidgetUtil>
#include <DLog>
#include <DDialog>

#include <QDebug>
#include <QTextBrowser>

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
#if defined(DTK_STATIC_LIB)
    DWIDGET_INIT_RESOURCE();
#endif
    DApplication *a = DApplication::globalApplication(argc, argv);
    qDebug() << a->arguments();
    DApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
    Dtk::Core::DLogManager::registerConsoleAppender();

    a->loadTranslator();
#ifdef Q_OS_UNIX
    a->setOOMScoreAdj(500);
#endif

    if (!DGuiApplicationHelper::setSingleInstance("deepin-tool-kit-examples")) {
        qDebug() << "another instance is running!!";
    }

    a->setApplicationName("dtk-example");
    a->setOrganizationName("deepin");
    DApplicationSettings as;
    Q_UNUSED(as)

    MainWindow w;
    w.show();

    Dtk::Widget::moveToCenter(&w);

    return a->exec();
}
