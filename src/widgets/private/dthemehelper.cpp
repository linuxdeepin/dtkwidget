// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dthemehelper.h"

DThemeHelper::DThemeHelper(QObject *parent) :
    QObject(parent)
{

}

QString DThemeHelper::themeToQss(QString theme)
{
    return theme;
}

QString DThemeHelper::trimThemeContents(QString qss)
{
    return qss;
}
