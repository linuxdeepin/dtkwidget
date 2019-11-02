/*
 * Copyright (C) 2016 ~ 2017 Deepin Technology Co., Ltd.
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

#pragma once

#include <QScopedPointer>

#include <DStyle>
#include <DStyledItemDelegate>

DWIDGET_BEGIN_NAMESPACE

class NavigationDelegatePrivate;
class NavigationDelegate: public DStyledItemDelegate
{
    Q_OBJECT
public:
    NavigationDelegate(QAbstractItemView *parent);
    ~NavigationDelegate();


    enum DataRole {
        NavLevelRole = Dtk::UserRole + 100,
        NavKeyRole,
    };

    enum NavLevel {
        Split = 1001,
        Level1,
        Level2,
        Level3
    };

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const override;
};

DWIDGET_END_NAMESPACE
