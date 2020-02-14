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

#include "multilistview.h"
#include "multilistitem.h"
#include <QMenu>
#include <QAction>

DWIDGET_USE_NAMESPACE

MultiListView::MultiListView(DSimpleListView *parent) : DSimpleListView(parent)
{
    QList<DSimpleListItem*> items;
    MultiListItem *item1 = new MultiListItem("Bob Dylan", "Like A Rolling Stone", "5:56");
    MultiListItem *item2 = new MultiListItem("Neil Young", "Old Man", "4:08");
    MultiListItem *item3 = new MultiListItem("Eric Clapton", "Tears In Heaven", "4:34");
    MultiListItem *item4 = new MultiListItem("John Lennon", "Imagine", "3:56");

    setColumnTitleInfo({"artistName", "songName", "songLength"}, {30, 80, 30}, 30);
    setColumnHideFlags(QList<bool>() << true << true << true, 0);
    items << item1;
    items << item2;
    items << item3;
    items << item4;

    QList<QString> titles;
    titles << "Artist" << "Song" << "Length";
    QList<int> widths;
    widths << 100 << -1 << 20;
    setColumnTitleInfo(titles, widths, 36);

    addItems(items);
    
    QList<SortAlgorithm> *alorithms = new QList<SortAlgorithm>();
    alorithms->append(&MultiListItem::sortByArtist);
    alorithms->append(&MultiListItem::sortBySong);
    alorithms->append(&MultiListItem::sortByLength);
    setColumnSortingAlgorithms(alorithms, 0, true);
    
    connect(this, &MultiListView::rightClickItems, this, &MultiListView::popupMenu, Qt::QueuedConnection);
}

void MultiListView::popupMenu(QPoint pos, QList<DSimpleListItem*> items)
{
    QMenu *menu = new QMenu();
    MultiListItem *item = static_cast<MultiListItem*>(items[0]);
    QAction *artistAction = new QAction(item->artist, this);
    QAction *songAction = new QAction(item->song, this);
    QAction *lengthAction = new QAction(item->length, this);
    
    menu->addAction(artistAction);
    menu->addAction(songAction);
    menu->addAction(lengthAction);
    
    menu->exec(pos);
}
