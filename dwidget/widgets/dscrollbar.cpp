/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dscrollbar.h"
#include "dthememanager.h"

DWIDGET_NAMESPACE_BEGIN

DScrollBar::DScrollBar(QWidget *parent) : QScrollBar(parent)
{
    D_THEME_INIT_WIDGET(DScrollBar);
}


DWIDGET_NAMESPACE_END
