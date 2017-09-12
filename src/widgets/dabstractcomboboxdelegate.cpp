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

#include "dabstractcomboboxdelegate.h"

DWIDGET_BEGIN_NAMESPACE

DAbstractComboBoxDelegate::DAbstractComboBoxDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

void DAbstractComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);

    //TODO, need to find anther way to get current index
    DComboBox *cb = static_cast<DComboBox *>(this->parent());
    if (cb){
        QModelIndex currentIndex = index.model()->index(cb->currentIndex(),0,QModelIndex());
        if (index == currentIndex){
            QPixmap pixmap((cb->hoverIndex()  != currentIndex) ? cb->normalTickImg() : cb->hoverTickImg());
            QRect rect = option.rect;
            rect.setX(TEXT_LEFT_MARGIN);
            rect.setY(rect.y() + (rect.height() - pixmap.height()) / 2);//draw in vertical center
            rect.setSize(pixmap.size());
            painter->drawPixmap(rect, pixmap);
        }
    }
}

void DAbstractComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)

    editor->setGeometry(option.rect);
}

DWIDGET_END_NAMESPACE
