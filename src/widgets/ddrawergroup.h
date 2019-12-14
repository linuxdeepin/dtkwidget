/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
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
#ifndef DDRAWERGROUP_H
#define DDRAWERGROUP_H

#include <dtkwidget_global.h>
#include <DObject>

DWIDGET_BEGIN_NAMESPACE

class DDrawer;
class DDrawerGroupPrivate;
class LIBDTKWIDGETSHARED_EXPORT DDrawerGroup : public QObject, public DCORE_NAMESPACE::DObject
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DDrawerGroup)

public:
    explicit DDrawerGroup(QObject *parent = 0);

    QList<DDrawer *> expands() const;
    DDrawer * checkedExpand() const;
    DDrawer * expand(int id) const;
    void addExpand(DDrawer *expand, int id = -1);
    void setId(DDrawer *expand, int id);
    void removeExpand(DDrawer *expand);
    int checkedId() const;
    int id(DDrawer *expand) const;

private:
    void onExpandChanged(bool v);
};

DWIDGET_END_NAMESPACE

#endif // DDRAWERGROUP_H
