/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QDebug>

#include "dapplication.h"
#include "dthememanager.h"
#include "private/dthemehelper.h"

DWIDGET_BEGIN_NAMESPACE

DApplication::DApplication(int &argc, char **argv) :
    QApplication(argc, argv)
{

}

QString DApplication::theme() const
{
    return DThemeManager::instance()->theme();
}

void DApplication::setTheme(const QString & theme)
{
    DThemeManager * themeManager = DThemeManager::instance();
    themeManager->setTheme(theme);
}

DWIDGET_END_NAMESPACE
