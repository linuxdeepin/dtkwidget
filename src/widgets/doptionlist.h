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

#ifndef DOPTIONLIST_H
#define DOPTIONLIST_H

#include "dtkwidget_global.h"
#include "dobject.h"
#include "doption.h"

#include <QListWidget>

DWIDGET_BEGIN_NAMESPACE

class DOptionListPrivate;
class D_DECL_DEPRECATED_X("Remove useless old widget") LIBDTKWIDGETSHARED_EXPORT DOptionList : public QListWidget, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DOptionList)
    D_DECLARE_PRIVATE(DOptionList)

public:
    explicit DOptionList(QWidget *parent = nullptr);

    void addOption(DOption *option);
    void insertOption(int pos, DOption *option);

    void setCurrentSelected(int pos);
    void setCurrentSelected(const QString &value);

protected:
    D_PRIVATE_SLOT(void _q_currentItemChanged(QListWidgetItem*,QListWidgetItem*))
};

DWIDGET_END_NAMESPACE

#endif // DOPTIONLIST_H
