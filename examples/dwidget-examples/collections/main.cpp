/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "mainwindow.h"

#include "dapplication.h"

#include <QDebug>

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication a(argc, argv);
    a.setTheme("dark");
    if (!a.setSingleInstance("libdui-examples"))
        qDebug() << "another instance is running!!";

    MainWindow w;

//    w.resize(w.size());
    w.show();

    return a.exec();
}
