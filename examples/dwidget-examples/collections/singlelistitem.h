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

#ifndef SINGLELISTITEM_H
#define SINGLELISTITEM_H

#include <DSimpleListItem>

DWIDGET_USE_NAMESPACE

class SingleListItem : public DSimpleListItem
{
    Q_OBJECT
    
public:
    SingleListItem(QString itemName);
    
    bool sameAs(DSimpleListItem *item);
    void drawBackground(QRect rect, QPainter *painter, int index, bool isSelect, bool isHover);
    void drawForeground(QRect rect, QPainter *painter, int column, int index, bool isSelect, bool isHover);
    
    QString name;
};	

#endif
