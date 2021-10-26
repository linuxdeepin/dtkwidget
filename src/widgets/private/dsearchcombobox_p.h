/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     wangpeng <wangpeng@uniontech.com>
*
* Maintainer: wangpeng <wangpeng@uniontech.com>
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
#ifndef DSEARCHCOMBOBOX_P_H
#define DSEARCHCOMBOBOX_P_H

#include "dsearchcombobox.h"
#include "private/dcombobox_p.h"
#include <DObjectPrivate>
#include <DSearchEdit>
#include <QSortFilterProxyModel>


DWIDGET_BEGIN_NAMESPACE

class DSearchComboBox;
class DSearchComboBoxPrivate : public DComboBoxPrivate
{
    Q_DECLARE_PUBLIC(DSearchComboBox)
public:
    explicit DSearchComboBoxPrivate(DSearchComboBox *q);

private:
    DSearchEdit *searchEdit;
    QCompleter *completer;
    QSortFilterProxyModel *proxyModel;
    int height;
};

DWIDGET_END_NAMESPACE

#endif // DSEARCHCOMBOBOX_P_H
