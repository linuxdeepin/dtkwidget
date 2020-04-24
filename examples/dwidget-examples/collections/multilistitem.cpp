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

#include "multilistitem.h"
#include <QColor>

DWIDGET_USE_NAMESPACE

MultiListItem::MultiListItem(QString artistName, QString songName, QString songLength)
{
    artist = artistName;
    song = songName;
    length = songLength;
}

bool MultiListItem::sameAs(DSimpleListItem *item)
{
    return artist == (static_cast<MultiListItem*>(item))->artist && song == (static_cast<MultiListItem*>(item))->song && length == (static_cast<MultiListItem*>(item))->length;
}

void MultiListItem::drawBackground(QRect rect, QPainter *painter, int index, bool isSelect, bool isHover)
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

void MultiListItem::drawForeground(QRect rect, QPainter *painter, int column, int index, bool isSelect, bool isHover)
{
    int padding = 10;
    painter->setOpacity(1);
    
    if (isSelect) {
        painter->setPen(QPen(QColor("#FFFFFF")));    
    } else {
        painter->setPen(QPen(QColor("#000000")));
    }
    
    if (column == 0) {
        painter->drawText(QRect(rect.x() + padding, rect.y(), rect.width() - padding * 2, rect.height()), Qt::AlignLeft | Qt::AlignVCenter, artist);
    } else if (column == 1) {
        painter->drawText(QRect(rect.x() + padding, rect.y(), rect.width() - padding * 2, rect.height()), Qt::AlignLeft | Qt::AlignVCenter, song);
    } else if (column == 2) {
        painter->drawText(QRect(rect.x() + padding, rect.y(), rect.width() - padding * 2, rect.height()), Qt::AlignRight | Qt::AlignVCenter, length);
    }
}

bool MultiListItem::sortByArtist(const DSimpleListItem *item1, const DSimpleListItem *item2, bool descendingSort)
{
    // Init.
    QString artist1 = (static_cast<const MultiListItem*>(item1))->artist;
    QString artist2 = (static_cast<const MultiListItem*>(item2))->artist;
    bool sortOrder = artist1 > artist2;

    return descendingSort ? sortOrder : !sortOrder;
}

bool MultiListItem::sortBySong(const DSimpleListItem *item1, const DSimpleListItem *item2, bool descendingSort)
{
    // Init.
    QString song1 = (static_cast<const MultiListItem*>(item1))->song;
    QString song2 = (static_cast<const MultiListItem*>(item2))->song;
    bool sortOrder = song1 > song2;

    return descendingSort ? sortOrder : !sortOrder;
}

bool MultiListItem::sortByLength(const DSimpleListItem *item1, const DSimpleListItem *item2, bool descendingSort)
{
    // Init.
    QString length1 = (static_cast<const MultiListItem*>(item1))->length;
    QString length2 = (static_cast<const MultiListItem*>(item2))->length;
    bool sortOrder = length1 > length2;

    return descendingSort ? sortOrder : !sortOrder;
}
