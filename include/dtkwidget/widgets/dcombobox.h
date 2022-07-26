/*
* Copyright (C) 2021 ~ 2021 Uniontech Software Technology Co.,Ltd.
*
* Author:     Ye ShanShan <yeshanshan@uniontech.com>
*
* Maintainer: Ye ShanShan <yeshanshan@uniontech.com>>
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
#ifndef DCOMBOBOX_H
#define DCOMBOBOX_H

#include <dtkwidget_global.h>
#include <DObject>
#include <QComboBox>

DWIDGET_BEGIN_NAMESPACE

class DComboBoxPrivate;
class LIBDTKWIDGETSHARED_EXPORT DComboBox : public QComboBox, public DCORE_NAMESPACE::DObject
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DComboBox)

public:
    explicit DComboBox(QWidget *parent = nullptr);

protected:
    DComboBox(DComboBoxPrivate &dd, QWidget *parent);

    // QComboBox interface
public:
    virtual void showPopup() override;
};

DWIDGET_END_NAMESPACE

#endif // DCOMBOBOX_H
