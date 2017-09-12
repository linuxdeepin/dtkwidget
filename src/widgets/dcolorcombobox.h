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

#ifndef DCOLORCOMBOBOX_H
#define DCOLORCOMBOBOX_H

#include <QLabel>
#include <QObject>
#include <QPainter>
#include <QPaintEvent>
#include <QJsonObject>
#include <QStyleOptionViewItem>
#include <QStyledItemDelegate>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QColor>

#include "dtkwidget_global.h"
#include "dcomboboxmodel.h"
#include "dcombobox.h"
#include "dabstractcomboboxdelegate.h"

DWIDGET_BEGIN_NAMESPACE

class DColorComboBoxPrivate;
class LIBDTKWIDGETSHARED_EXPORT DColorComboBox : public DComboBox
{
    Q_OBJECT
public:
    explicit DColorComboBox(QWidget *parent = 0);
    void addData(const QColor &color, const QString &title);
    void addData(const QString &color, const QString &title);

Q_SIGNALS:
    void currentColorChange(QColor color);

public Q_SLOTS:
    void onCurrentIndexChange(int index);

private:
    D_DECLARE_PRIVATE(DColorComboBox)
};

DWIDGET_END_NAMESPACE

#endif // DCOLORCOMBOBOX_H
