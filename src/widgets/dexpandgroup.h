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

#ifndef EXPANDGROUP_H
#define EXPANDGROUP_H

#include <QObject>
#include <QList>
#include <QMap>

#include "dtkwidget_global.h"
#include "dbaseexpand.h"

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT D_DECL_DEPRECATED_X("Use DDrawerGroup") DExpandGroup : public QObject
{
    Q_OBJECT
public:
    explicit DExpandGroup(QObject *parent = 0);

    QList<DBaseExpand *> expands() const;
    DBaseExpand * checkedExpand() const;
    DBaseExpand * expand(int id) const;
    void addExpand(DBaseExpand *expand, int id = -1);
    void setId(DBaseExpand *expand, int id);
    void removeExpand(DBaseExpand *expand);
    int checkedId() const;
    int id(DBaseExpand *expand) const;

private:
    void onExpandChanged(bool v);

private:
    QMap<int, DBaseExpand *> m_expandMap;
    QMap<int, DBaseExpand *> m_checkedMap;
};

DWIDGET_END_NAMESPACE

#endif // EXPANDGROUP_H
