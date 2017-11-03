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

#ifndef MULTILISTITEM_H
#define MULTILISTITEM_H

#include <DSimpleListItem>

DWIDGET_USE_NAMESPACE

class MultiListItem : public DSimpleListItem
{
    Q_OBJECT
    
public:
    MultiListItem(QString artistName, QString songName, QString songLength);
    
    bool sameAs(DSimpleListItem *item);
    void drawBackground(QRect rect, QPainter *painter, int index, bool isSelect);
    void drawForeground(QRect rect, QPainter *painter, int column, int index, bool isSelect);
    
    static bool sortByArtist(const DSimpleListItem *item1, const DSimpleListItem *item2, bool descendingSort);
    static bool sortBySong(const DSimpleListItem *item1, const DSimpleListItem *item2, bool descendingSort);
    static bool sortByLength(const DSimpleListItem *item1, const DSimpleListItem *item2, bool descendingSort);
    
    QString artist;
    QString song;
    QString length;
};	

#endif
