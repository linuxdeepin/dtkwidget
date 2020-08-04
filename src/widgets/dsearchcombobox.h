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
#ifndef DSEARCHCOMBOBOX_H
#define DSEARCHCOMBOBOX_H

#include <dobject.h>
#include <dtkwidget_global.h>

#include <DComboBox>

DWIDGET_BEGIN_NAMESPACE

class DSearchComboBoxPrivate;
class LIBDTKWIDGETSHARED_EXPORT DSearchComboBox : public DComboBox, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DSearchComboBox)
    D_DECLARE_PRIVATE(DSearchComboBox)
public:
    explicit DSearchComboBox(QWidget *parent = nullptr);
    void setEditable(bool editable);

protected:
    void showPopup() override;
};

DWIDGET_END_NAMESPACE

#endif // DSEARCHCOMBOBOX_H
