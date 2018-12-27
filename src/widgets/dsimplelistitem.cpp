/* -*- Mode: C++; indent-tabs-mode: nil; tab-width: 4 -*-
 * -*- coding: utf-8 -*-
 *
 * Copyright (C) 2011 ~ 2017 Deepin, Inc.
 *               2011 ~ 2017 Wang Yong
 *
 * Author:     Wang Yong <wangyong@deepin.com>
 * Maintainer: Wang Yong <wangyong@deepin.com>
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

#include "dsimplelistitem.h"

DWIDGET_BEGIN_NAMESPACE

/*!
 * \~chinese \class DSimpleListItem
 * \~chinese \brief DSimpleListItem 是 DSimpleListView 的接口，得到 DSimpleListView 传递过来的 QPainter、列信息、表格矩形数据后，由开发者完全控制行内容的绘制。
 */

/*!
 * \~chinese \property DSimpleListItem::sameAs
 * \~chinese \brief 此接口是用来比较两个 item 的函数
 */

/*!
 * \~chinese \property DSimpleListItem::drawBackground
 * \~chinese \brief drawBackground 用于绘制背景的接口函数。
 * \~chinese \param rect 对应矩形区域行
 * \~chinese \param painter 可以绘画出你想画的内容
 * \~chinese \param index 索引值，可以根据索引绘制不同的行效果，例如斑马线
 * \~chinese \param isSelect 当前项是否被选择
 * \~chinese \param isHover 鼠标是否悬停在当前项
 */

/*!
 * \~chinese \property DSimpleListItem::drawForeground
 * \~chinese \brief drawForeground 用于绘制内容的接口函数。
 * \~chinese \param rect 对应矩形区域行
 * \~chinese \param painter 可以绘画出你想画的内容
 * \~chinese \param column 可以根据列索引绘制不同的列内容
 * \~chinese \param index 索引值，可以根据索引绘制不同的效果
 * \~chinese \param isSelect 当前项是否被选择
 * \~chinese \param isHover 鼠标是否悬停在当前项
 */

DSimpleListItem::DSimpleListItem()
{

}

DWIDGET_END_NAMESPACE
