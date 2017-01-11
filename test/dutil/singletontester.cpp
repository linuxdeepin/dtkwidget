/**
 * Copyright (C) 2016 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "singletontester.h"

#include <QDebug>
#include <QThread>

Singleton::Singleton(QObject *parent) : QObject(parent)
{
    qDebug() << "Singleton Init Begin" << this;
    QThread::sleep(3);
    qDebug() << "Singleton Init End" << this;
}

void Singleton::test()
{
    qDebug() << "test" << this;
}

MultiSingletonTester::MultiSingletonTester(QObject *parent) : QObject(parent)
{
}

void MultiSingletonTester::run()
{
    Singleton::instance()->test();
}
