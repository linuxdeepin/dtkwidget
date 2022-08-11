// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EXPANDGROUP_H
#define EXPANDGROUP_H

#include <QObject>
#include <QList>
#include <QMap>

#include <dtkwidget_global.h>
#include <dbaseexpand.h>

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
