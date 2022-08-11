// SPDX-FileCopyrightText: 2011 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dsimplelistitem.h"

DWIDGET_BEGIN_NAMESPACE

/*!
  \class Dtk::Widget::DSimpleListItem
  \inmodule dtkwidget
  \brief DSimpleListItem 是 DSimpleListView 的接口，得到 DSimpleListView 传递过来的 QPainter、列信息、表格矩形数据后，由开发者完全控制行内容的绘制.
 */

/*!
  \fn virtual bool DSimpleListItem::sameAs(DSimpleListItem *item) = 0
  \brief 此接口是用来比较两个 item 的函数.

  \a item 用于比较的 item 指针.
  \return 相同返回true，否则返回false。
 */

/*!
  \fn virtual bool DSimpleListItem::drawBackground(QRect rect, QPainter *painter, int index, bool isSelect, bool isHover) = 0
  \brief drawBackground 用于绘制背景的接口函数.

  \a rect 对应矩形区域行
  \a painter 可以绘画出你想画的内容
  \a index 索引值，可以根据索引绘制不同的行效果，例如斑马线
  \a isSelect 当前项是否被选择
  \a isHover 鼠标是否悬停在当前项
 */

/*!
  \fn virtual void DSimpleListItem::drawForeground(QRect rect, QPainter *painter, int column, int index, bool isSelect, bool isHover) = 0
  \brief drawForeground 用于绘制内容的接口函数.

  \a rect 对应矩形区域行
  \a painter 可以绘画出你想画的内容
  \a column 可以根据列索引绘制不同的列内容
  \a index 索引值，可以根据索引绘制不同的效果
  \a isSelect 当前项是否被选择
  \a isHover 鼠标是否悬停在当前项
 */

DSimpleListItem::DSimpleListItem()
{

}

DWIDGET_END_NAMESPACE
