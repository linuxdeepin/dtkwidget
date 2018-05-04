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

#ifndef DSIMPLECOMBOBOX_H
#define DSIMPLECOMBOBOX_H

#include <QWidget>

#include "dtkwidget_global.h"
#include "dcomboboxmodel.h"
#include "dabstractcomboboxdelegate.h"
#include "dcombobox.h"

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT  D_DECL_DEPRECATED_X("Use QComboBox directly.") DSimpleComboBox : public DComboBox
{
    Q_OBJECT
public:
    explicit DSimpleComboBox(QWidget *parent = 0);

    void addItem(const QString &text);
    void addItems(const QStringList &texts);

Q_SIGNALS:
    void currentTextChanged(const QString &);

private:
    Q_SLOT void onCurrentIndexChange(int index);
};

DWIDGET_END_NAMESPACE

#endif // DSIMPLECOMBOBOX_H
