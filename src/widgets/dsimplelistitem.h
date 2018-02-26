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

#ifndef DSIMPLELISTITEM_H
#define DSIMPLELISTITEM_H

#include <dtkwidget_global.h>
#include <QObject>
#include <QPainter>

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DSimpleListItem : public QObject
{
    Q_OBJECT
    
public:
    DSimpleListItem();
    
    /*
     * The interface function that used to compare the two DSimpleListItem
     * The DSimpleListView requires this interface to keep the selected items unchanged when refreshed
     * 
     * @item any subclass of DSimpleListItem, you can use static_cast in implementation to access any attribute to compare two items
     * @return return true if two items have same attribute, the compare method implement by subclass of DSimpleListItem
     */
    virtual bool sameAs(DSimpleListItem *item)=0;
    
    /* 
     * The interface function that used to draw background of DSimpleListItem.
     * Such as background and selected effect.
     * 
     * @rect row corresponding to the drawing of the rectangular area
     * @painter the painter used to draw anything you want
     * @index the index of DSimpleListItem, you can draw different rows effect based on the index, such as the zebra crossing
     * @isSelect current item is selected, you can draw selected effect under content when isSelect is true
     */
    virtual void drawBackground(QRect rect, QPainter *painter, int index, bool isSelect)=0;
    
    /* 
     * The interface function that used to draw foreground of DSimpleListItem.
     * 
     * @rect column corresponding to the drawing of the rectangular area
     * @painter the painter used to draw anything you want
     * @column the column of DSimpleListItem, you can draw different column content based on the column index
     * @index the index of DSimpleListItem, you can draw different rows effect based on the index, such as the zebra crossing
     * @isSelect current item is selected, you can draw selected effect under content when isSelect is true
     */
    virtual void drawForeground(QRect rect, QPainter *painter, int column, int index, bool isSelect)=0;
    
    
    /* 
     * The interface function that used to draw hover effect.
     * 
     * @rect column corresponding to the drawing of the rectangular area
     * @painter the painter used to draw anything you want
     */
    virtual void drawHover(QRect rect, QPainter *painter)=0;
};

DWIDGET_END_NAMESPACE

#endif
