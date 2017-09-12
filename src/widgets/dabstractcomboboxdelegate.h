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

#ifndef DABSTRACTCOMBOBOXDELEGATE_H
#define DABSTRACTCOMBOBOXDELEGATE_H

#include <QObject>
#include <QPainter>
#include <QStyledItemDelegate>

#include "dcombobox.h"

DWIDGET_BEGIN_NAMESPACE

class DAbstractComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit DAbstractComboBoxDelegate(QObject *parent = 0);
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter * painter,const QStyleOptionViewItem & option,const QModelIndex & index) const;
//    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

};

DWIDGET_END_NAMESPACE

#endif // DABSTRACTCOMBOBOXDELEGATE_H
