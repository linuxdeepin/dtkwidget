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

#include "singlelistitem.h"
#include <QColor>

DWIDGET_USE_NAMESPACE

SingleListItem::SingleListItem(QString itemName)
{
    name = itemName;
}

bool SingleListItem::sameAs(DSimpleListItem *item)
{
    return name == (static_cast<SingleListItem*>(item))->name;
}

void SingleListItem::drawBackground(QRect rect, QPainter *painter, int index, bool isSelect)
{
    QPainterPath path;
    path.addRect(QRectF(rect));
    
    painter->setOpacity(1);
    if (isSelect) {
        painter->fillPath(path, QColor("#2CA7F8"));
    } else if (index % 2 == 1) {
        painter->fillPath(path, QColor("#D8D8D8"));
    }
}

void SingleListItem::drawForeground(QRect rect, QPainter *painter, int column, int index, bool isSelect)
{
    painter->setOpacity(1);
    if (isSelect) {
        painter->setPen(QPen(QColor("#FFFFFF")));    
    } else {
        painter->setPen(QPen(QColor("#000000")));
    }
    
    int padding = 10;
    painter->drawText(QRect(rect.x() + padding, rect.y(), rect.width() - padding * 2, rect.height()), Qt::AlignLeft | Qt::AlignVCenter, name);
}
