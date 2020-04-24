/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#include "dexpandgroup.h"

DWIDGET_USE_NAMESPACE

/**
 * \~chinese \class DExpandGroup
 * \~chinese \brief 封装了一组 DBaseExpand 控件(当然也可以是其子类), 并保证同一时间只有一个 DBaseExpand 处于展开状态

 * \~chinese DExpandGroup 顾名思义是用来管理一组 DBaseExpand 控件的，提供了简单的新增，获取，移除的方法，用于统一管理多个 DBaseExpand 对象的展开和收起状态,
 * \~chinese DExpandGroup 会保证在同一时间只有一个 DBaseExpand 控件处于展开状态, 也就是说如果点击展开了组里的一个控件, DExpandGroup 会将组里其他控件设置为收起状态。
 * \note 请注意，DExpandGroup 并不是控件, 只需要将需要管理的 DBaseExpand 添加进来即可
 */

/**
 * \~chinese \brief 获取类实例
 * \~chinese
 * \~chinese \param parent 作为实例的父对象
 */
DExpandGroup::DExpandGroup(QObject *parent) : QObject(parent)
{

}

/**
 * \~chinese \brief 获取所有正在管理的控件
 * \~chinese
 * \~chinese \return 控件组成的列表
 */
QList<DBaseExpand *> DExpandGroup::expands() const
{
    return m_expandMap.values();
}

/**
 * \~chinese \brief 获取处于已展开状态的 DBaseExpand 对象
 * \~chinese
 * \~chinese \return 已展开状态的控件, 若没有已展开的控件则返回 NULL
 */
DBaseExpand *DExpandGroup::checkedExpand() const
{
    if (m_checkedMap.isEmpty())
        return NULL;
    else
        return m_checkedMap.first();
}

/**
 * \~chinese \brief 获取指定 id 对应的控件
 * \~chinese
 * \~chinese \param id 增加控件时指定的的id
 * \~chinese
 * \~chinese \return 根据 id 找到的控件
 */
DBaseExpand *DExpandGroup::expand(int id) const
{
    return m_expandMap.value(id);
}

/**
 * \~chinese \brief 增加一个控件
 * \~chinese
 * \~chinese \param expand 要新增的控件对象
 * \~chinese \param id 为要新增的控件对象指定id，id 应该是唯一值，如果没有给定 id ，则使用已有列表中最大的id自加1后的值
 */
void DExpandGroup::addExpand(DBaseExpand *expand, int id)
{
    if (m_expandMap.values().indexOf(expand) == -1){
        if (id == -1){
            int maxId = -1;
            Q_FOREACH (int tmp, m_expandMap.keys()) {
                maxId = qMax(tmp, maxId);
            }

            id = maxId + 1;
        }
        m_expandMap.insert(id, expand);

        connect(expand, &DBaseExpand::expandChange, this, &DExpandGroup::onExpandChanged);
    }
}

/**
 * \~chinese \brief 设置指定控件的id
 * \~chinese
 * \~chinese \param expand 要设置id的控件
 * \~chinese \param id 要设置的id
 */
void DExpandGroup::setId(DBaseExpand *expand, int id)
{
    int index = m_expandMap.values().indexOf(expand);
    if (index != -1){
        m_expandMap.remove(m_expandMap.keys().at(index));
        addExpand(expand, id);
    }
}

/**
 * \~chinese \brief 移除一个控件
 * \~chinese
 * \~chinese \param expand 要移除的控件对象
 */
void DExpandGroup::removeExpand(DBaseExpand *expand)
{
    int index = m_expandMap.values().indexOf(expand);
    m_expandMap.remove(m_expandMap.keys().at(index));
}

/**
 * \~chinese \brief 返回处于已展开状态控件的 id
 * \~chinese
 * \~chinese \return 如果当前没有处于展开状态的控件则返回 -1
 */
int DExpandGroup::checkedId() const
{
    if (m_checkedMap.isEmpty())
        return -1;
    else
        return m_checkedMap.firstKey();
}

/**
 * \~chinese \brief 获取指定控件的 id
 * \~chinese
 * \~chinese \param expand 指定控件对象
 * \~chinese
 * \~chinese \return 指定控件对象的 id
 */
int DExpandGroup::id(DBaseExpand *expand) const
{
    return m_expandMap.keys().at(m_expandMap.values().indexOf(expand));
}

void DExpandGroup::onExpandChanged(bool v)
{
    DBaseExpand *dSender = qobject_cast<DBaseExpand *>(sender());
    if (!dSender || dSender == checkedExpand() || !v)
        return;

    m_checkedMap.clear();
    int checkedIndex = m_expandMap.values().indexOf(dSender);
    m_checkedMap.insert(m_expandMap.keys().at(checkedIndex), dSender);

    QList<DBaseExpand *> expandList = m_expandMap.values();
    for (int i = 0; i < expandList.count(); i ++){
        DBaseExpand *target = expandList.at(i);
        if (target != dSender && target->expand())
            target->setExpand(false);
    }
}


