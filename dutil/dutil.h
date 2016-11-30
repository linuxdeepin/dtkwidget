/**
 * Copyright (C) 2016 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#pragma once

#include <QTimer>
#include <QThread>
#include <QMetaObject>
#include <QCoreApplication>

namespace DUtil
{

template <typename Func1>
inline void TimerSingleShot(int msec,  Func1 slot)
{
#if QT_VERSION >= 0x050500
    QTimer::singleShot(msec, slot);
#else
    QTimer *timer = new QTimer;
    timer->setSingleShot(true);
    timer->setInterval(msec);
    timer->moveToThread(qApp->thread());
    QObject::connect(timer, &QTimer::timeout, slot);
    QObject::connect(timer, &QTimer::timeout, timer, &QTimer::deleteLater);
    if (QThread::currentThread() == qApp->thread()) { timer->start(); }
    else { QMetaObject::invokeMethod(timer, "start", Qt::QueuedConnection); }
#endif
}


}
