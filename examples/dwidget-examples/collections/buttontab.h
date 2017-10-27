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

#ifndef BUTTONTAB_H
#define BUTTONTAB_H

#include <QWidget>
#include <QLabel>

#include "dtkwidget_global.h"
#include "dtextbutton.h"
#include "dimagebutton.h"
#include "dheaderline.h"
#include "darrowbutton.h"
#include "dswitchbutton.h"
#include "dwindowmaxbutton.h"
#include "dwindowminbutton.h"
#include "dwindowclosebutton.h"
#include "dwindowoptionbutton.h"
#include "dlinkbutton.h"

class ButtonTab : public QLabel
{
    Q_OBJECT
public:
    explicit ButtonTab(QWidget *parent = 0);

public Q_SLOTS:
    void buttonClickTest();
};

#endif // BUTTONTAB_H
