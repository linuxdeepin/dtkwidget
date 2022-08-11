// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
