/**
 * Copyright (C) 2016 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#pragma once

#include <QObject>
#include "dsingleton.h"

class Singleton : public QObject, public Dtk::DSingleton<Singleton>
{
    Q_OBJECT
    friend class Dtk::DSingleton<Singleton>;
public:
    explicit Singleton(QObject *parent = 0);

    void test();
signals:

public slots:

};

class MultiSingletonTester : public QObject
{
    Q_OBJECT
public:
    explicit MultiSingletonTester(QObject *parent = 0);

    void run();
signals:

public slots:

};


