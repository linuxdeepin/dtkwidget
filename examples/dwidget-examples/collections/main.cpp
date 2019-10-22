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

#include "mainwindow.h"
#include "dapplication.h"
#include "dplatformwindowhandle.h"

#include <QDebug>
#include <DLog>
#include <DTitlebar>
#include <DApplicationSettings>
#include <DListView>
#include <DStandardItem>

#include "util/dwidgetutil.h"

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
#if defined(DTK_STATIC_LIB)
    DWIDGET_INIT_RESOURCE();
#endif

    DApplication::loadDXcbPlugin();

    DApplication a(argc, argv);
    Dtk::Core::DLogManager::registerConsoleAppender();

    a.loadTranslator();
    a.setAttribute(Qt::AA_EnableHighDpiScaling, true);
#ifdef Q_OS_UNIX
    a.setOOMScoreAdj(500);
#endif

    if (!a.setSingleInstance("deepin-tool-kit-examples")) {
        qDebug() << "another instance is running!!";
    }

    a.setApplicationName("dtk-example");
    a.setOrganizationName("deepin");
    DApplicationSettings as;
    Q_UNUSED(as)

    DMainWindow w;
    DListView view;
    QStandardItemModel *model = new QStandardItemModel(&view);
    DStandardItem *item = new DStandardItem();

    item->setText("test");
    item->setIcon(QIcon::fromTheme("deepin-clone"));
    auto action1 = new DViewItemAction();
    action1->setText("action1");
    auto action2 = new DViewItemAction();
    action2->setIcon(QIcon::fromTheme("dde-file-manager"));
    action2->setText("action2");
    item->setActionList(Qt::RightEdge, {action1, action2});
    model->appendRow(item);
    view.setModel(model);

    w.setCentralWidget(&view);
    w.show();
    Dtk::Widget::moveToCenter(&w);

    return a.exec();
}
