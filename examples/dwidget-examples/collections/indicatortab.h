/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INDICATORTAB_H
#define INDICATORTAB_H

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "dloadingindicator.h"

DWIDGET_USE_NAMESPACE

class IndicatorTab : public QLabel
{
    Q_OBJECT
public:
    explicit IndicatorTab(QWidget *parent = 0);
};

#endif // INDICATORTAB_H
