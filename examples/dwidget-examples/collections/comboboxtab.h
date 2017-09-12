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

#ifndef COMBOBOXTAB_H
#define COMBOBOXTAB_H

#include <QWidget>
#include <QLabel>
#include <QJsonObject>
#include <QAbstractItemView>
#include "dcombobox.h"
#include "dcomboboxmodel.h"
#include "dfontcombobox.h"
#include "dcolorcombobox.h"
#include "dsimplecombobox.h"

DWIDGET_USE_NAMESPACE

class ComboBoxTab : public QLabel
{
    Q_OBJECT
public:
    explicit ComboBoxTab(QWidget *parent = 0);

Q_SIGNALS:

private:
    DComboBox *colorCB;
    DComboBoxModel * colorModel;
    DComboBox *cb;
    DComboBoxModel * model;
    DComboBox *simpleBox;
};

#endif // COMBOBOXTAB_H
