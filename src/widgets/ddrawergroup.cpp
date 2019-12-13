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
#include "ddrawergroup.h"
#include "ddrawer.h"

#include <DObjectPrivate>

DWIDGET_BEGIN_NAMESPACE

class DDrawerGroupPrivate : public DCORE_NAMESPACE::DObjectPrivate
{
public:
    DDrawerGroupPrivate(DDrawerGroup *qq)
        : DObjectPrivate(qq)
    {

    }

    QMap<int, DDrawer *> expandMap;
    QMap<int, DDrawer *> checkedMap;

    D_DECLARE_PUBLIC(DDrawerGroup)
};

/**
 * \~chinese \class DDrawerGroup
 * \~chinese \brief 封装了一组 DDrawer 控件(当然也可以是其子类), 并保证同一时间只有一个 DDrawer 处于展开状态

 * \~chinese DDrawerGroup 顾名思义是用来管理一组 DDrawer 控件的，提供了简单的新增，获取，移除的方法，用于统一管理多个 DDrawer 对象的展开和收起状态,
 * \~chinese DDrawerGroup 会保证在同一时间只有一个 DDrawer 控件处于展开状态, 也就是说如果点击展开了组里的一个控件, DDrawerGroup 会将组里其他控件设置为收起状态。
 * \note 请注意，DDrawerGroup 并不是控件, 只需要将需要管理的 DDrawer 添加进来即可
 */

/**
 * \~chinese \brief 获取类实例
 * \~chinese
 * \~chinese \param parent 作为实例的父对象
 */
DDrawerGroup::DDrawerGroup(QObject *parent)
    : QObject(parent)
    , DCORE_NAMESPACE::DObject(*new DDrawerGroupPrivate(this))
{

}

/**
 * \~chinese \brief 获取所有正在管理的控件
 * \~chinese
 * \~chinese \return 控件组成的列表
 */
QList<DDrawer *> DDrawerGroup::expands() const
{
    D_DC(DDrawerGroup);
    return d->expandMap.values();
}

/**
 * \~chinese \brief 获取处于已展开状态的 DDrawer 对象
 * \~chinese
 * \~chinese \return 已展开状态的控件, 若没有已展开的控件则返回 NULL
 */
DDrawer *DDrawerGroup::checkedExpand() const
{
    D_DC(DDrawerGroup);
    if (d->checkedMap.isEmpty())
        return NULL;
    else
        return d->checkedMap.first();
}

/**
 * \~chinese \brief 获取指定 id 对应的控件
 * \~chinese
 * \~chinese \param id 增加控件时指定的的id
 * \~chinese
 * \~chinese \return 根据 id 找到的控件
 */
DDrawer *DDrawerGroup::expand(int id) const
{
    D_DC(DDrawerGroup);
    return d->expandMap.value(id);
}

/**
 * \~chinese \brief 增加一个控件
 * \~chinese
 * \~chinese \param expand 要新增的控件对象
 * \~chinese \param id 为要新增的控件对象指定id，id 应该是唯一值，如果没有给定 id ，则使用已有列表中最大的id自加1后的值
 */
void DDrawerGroup::addExpand(DDrawer *expand, int id)
{
    D_D(DDrawerGroup);
    if (d->expandMap.values().indexOf(expand) == -1){
        if (id == -1){
            int maxId = -1;
            Q_FOREACH (int tmp, d->expandMap.keys()) {
                maxId = qMax(tmp, maxId);
            }

            id = maxId + 1;
        }
        d->expandMap.insert(id, expand);

        connect(expand, &DDrawer::expandChange, this, &DDrawerGroup::onExpandChanged);
    }
}

/**
 * \~chinese \brief 设置指定控件的id
 * \~chinese
 * \~chinese \param expand 要设置id的控件
 * \~chinese \param id 要设置的id
 */
void DDrawerGroup::setId(DDrawer *expand, int id)
{
    D_D(DDrawerGroup);
    int index = d->expandMap.values().indexOf(expand);
    if (index != -1){
        d->expandMap.remove(d->expandMap.keys().at(index));
        addExpand(expand, id);
    }
}

/**
 * \~chinese \brief 移除一个控件
 * \~chinese
 * \~chinese \param expand 要移除的控件对象
 */
void DDrawerGroup::removeExpand(DDrawer *expand)
{
    D_D(DDrawerGroup);
    int index = d->expandMap.values().indexOf(expand);
    d->expandMap.remove(d->expandMap.keys().at(index));
}

/**
 * \~chinese \brief 返回处于已展开状态控件的 id
 * \~chinese
 * \~chinese \return 如果当前没有处于展开状态的控件则返回 -1
 */
int DDrawerGroup::checkedId() const
{
    D_DC(DDrawerGroup);
    if (d->checkedMap.isEmpty())
        return -1;
    else
        return d->checkedMap.firstKey();
}

/**
 * \~chinese \brief 获取指定控件的 id
 * \~chinese
 * \~chinese \param expand 指定控件对象
 * \~chinese
 * \~chinese \return 指定控件对象的 id
 */
int DDrawerGroup::id(DDrawer *expand) const
{
    D_DC(DDrawerGroup);
    return d->expandMap.keys().at(d->expandMap.values().indexOf(expand));
}

void DDrawerGroup::onExpandChanged(bool v)
{
    D_D(DDrawerGroup);
    DDrawer *dSender = qobject_cast<DDrawer *>(sender());
    if (!dSender || dSender == checkedExpand() || !v)
        return;

    d->checkedMap.clear();
    int checkedIndex = d->expandMap.values().indexOf(dSender);
    d->checkedMap.insert(d->expandMap.keys().at(checkedIndex), dSender);

    QList<DDrawer *> expandList = d->expandMap.values();
    for (int i = 0; i < expandList.count(); i ++){
        DDrawer *target = expandList.at(i);
        if (target != dSender && target->expand())
            target->setExpand(false);
    }
}

DWIDGET_END_NAMESPACE
