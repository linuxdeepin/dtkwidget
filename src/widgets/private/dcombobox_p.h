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

#ifndef DCOMBOBOX_P_H
#define DCOMBOBOX_P_H

#include "dcombobox.h"
#include <DObjectPrivate>

DWIDGET_BEGIN_NAMESPACE

class DComboBoxPrivate : public DCORE_NAMESPACE::DObjectPrivate
{
    Q_DECLARE_PUBLIC(DComboBox)
public:
    explicit DComboBoxPrivate(DComboBox* q);

    void init();

    // 重写 QComboBoxPrivate类的popupGeometry
    QRect popupGeometry();

    // 重写 QComboBoxPrivate类的computeWidthHint
    int computeWidthHint() const;

    // 最大显示项数
    static const int MaxVisibleItems = 16;
};

DWIDGET_END_NAMESPACE

#endif // DCOMBOBOX_P_H

