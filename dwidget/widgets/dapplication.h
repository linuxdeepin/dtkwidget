/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DAPPLICATION_H
#define DAPPLICATION_H

#include <QApplication>

#include "libdui_global.h"

DWIDGET_NAMESPACE_BEGIN

class LIBDUISHARED_EXPORT DApplication : public QApplication
{
    Q_OBJECT
public:
    DApplication(int &argc, char **argv);

    QString theme() const;
    void setTheme(const QString & theme);
};

DWIDGET_NAMESPACE_END

#endif // DAPPLICATION_H
