/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "linetab.h"

LineTab::LineTab(QWidget *parent) : QLabel(parent)
{
//        setStyleSheet("LineTab{background-color: #252627;}");
        //////////////////////////////////////////////////////////////--DSeparator
        DSeparatorHorizontal *dsHorizontal = new DSeparatorHorizontal(this);
        dsHorizontal->setFixedSize(200,2);
        dsHorizontal->move(1,5);
        DSeparatorVertical *dsVertical = new DSeparatorVertical(this);
        dsVertical->setFixedSize(2,200);
        dsVertical->move(10,5);
}

