/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dlinkbutton.h"
#include "dthememanager.h"

DWIDGET_NAMESPACE_BEGIN

DLinkButton::DLinkButton(const QString &text, QWidget *parent) :
    QPushButton(text, parent)
{
    D_THEME_INIT_WIDGET(DLinkButton);
}

DWIDGET_NAMESPACE_END
