/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dtextbutton.h"
#include "dthememanager.h"

DWIDGET_NAMESPACE_BEGIN

DTextButton::DTextButton(const QString & text, QWidget * parent) :
    QPushButton(text, parent)
{
    D_THEME_INIT_WIDGET(DTextButton);
}

DTextButton::~DTextButton()
{

}

DWIDGET_NAMESPACE_END
