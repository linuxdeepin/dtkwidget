// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dexpandgroup.h"
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
DWIDGET_USE_NAMESPACE

/*!
  \class Dtk::Widget::DExpandGroup
  \inmodule dtkwidget
  \brief 封装了一组 DBaseExpand 控件(当然也可以是其子类), 并保证同一时间只有一个 DBaseExpand 处于展开状态.

  DExpandGroup 顾名思义是用来管理一组 DBaseExpand 控件的，提供了简单的新增，获取，移除的方法，用于统一管理多个 DBaseExpand 对象的展开和收起状态,
  DExpandGroup 会保证在同一时间只有一个 DBaseExpand 控件处于展开状态, 也就是说如果点击展开了组里的一个控件, DExpandGroup 会将组里其他控件设置为收起状态。
  \note 请注意，DExpandGroup 并不是控件, 只需要将需要管理的 DBaseExpand 添加进来即可
 */

/*!
  \brief 获取类实例
  
  \a parent 作为实例的父对象
 */
DExpandGroup::DExpandGroup(QObject *parent) : QObject(parent)
{

}

/*!
  \brief 获取所有正在管理的控件
  
  \return 控件组成的列表
 */
QList<DBaseExpand *> DExpandGroup::expands() const
{
    return m_expandMap.values();
}

/*!
  \brief 获取处于已展开状态的 DBaseExpand 对象
  
  \return 已展开状态的控件, 若没有已展开的控件则返回 NULL
 */
DBaseExpand *DExpandGroup::checkedExpand() const
{
    if (m_checkedMap.isEmpty())
        return NULL;
    else
        return m_checkedMap.first();
}

/*!
  \brief 获取指定 id 对应的控件
  
  \a id 增加控件时指定的的id
  
  \return 根据 id 找到的控件
 */
DBaseExpand *DExpandGroup::expand(int id) const
{
    return m_expandMap.value(id);
}

/*!
  \brief 增加一个控件
  
  \a expand 要新增的控件对象
  \a id 为要新增的控件对象指定id，id 应该是唯一值，如果没有给定 id ，则使用已有列表中最大的id自加1后的值
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

/*!
  \brief 设置指定控件的id
  
  \a expand 要设置id的控件
  \a id 要设置的id
 */
void DExpandGroup::setId(DBaseExpand *expand, int id)
{
    int index = m_expandMap.values().indexOf(expand);
    if (index != -1){
        m_expandMap.remove(m_expandMap.keys().at(index));
        addExpand(expand, id);
    }
}

/*!
  \brief 移除一个控件
  
  \a expand 要移除的控件对象
 */
void DExpandGroup::removeExpand(DBaseExpand *expand)
{
    int index = m_expandMap.values().indexOf(expand);
    m_expandMap.remove(m_expandMap.keys().at(index));
}

/*!
  \brief 返回处于已展开状态控件的 id
  
  \return 如果当前没有处于展开状态的控件则返回 -1
 */
int DExpandGroup::checkedId() const
{
    if (m_checkedMap.isEmpty())
        return -1;
    else
        return m_checkedMap.firstKey();
}

/*!
  \brief 获取指定控件的 id
  
  \a expand 指定控件对象
  
  \return 指定控件对象的 id
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

#endif
