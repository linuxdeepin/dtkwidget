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

#ifndef DOPTIONLIST_P_H
#define DOPTIONLIST_P_H

#include "doptionlist.h"
#include "doption.h"

#include <DObjectPrivate>

#include <QList>
#include <QListWidgetItem>

DWIDGET_BEGIN_NAMESPACE

class DOptionListPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    D_DECLARE_PUBLIC(DOptionList)

public:
    DOptionListPrivate(DOptionList *q);

    void init();
    void addOption(DOption *option);
    void insertOption(int pos, DOption *option);
    void setCurrentSelected(int pos);
    void setCurrentSelected(const QString &value);

public Q_SLOTS:
    void _q_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

protected:
    QMap<QListWidgetItem *, DOption *> m_optionList;
};

DWIDGET_END_NAMESPACE

#endif // DOPTIONLIST_P_H
