// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "mainwindow.h"
#include <DApplication>
#include <QHash>
#include <QPair>
#include <functional>

int main(int argc, char *argv[])
{
    DApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
