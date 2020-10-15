/*
 * Copyright (C) 2020 ~ 2020 Deepin Technology Co., Ltd.
 *
 * Author:     sunkang <sunkang@uniontech.com>
 *
 * Maintainer: sunkang <sunkang@uniontech.com>
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

#ifndef WIDGETEXAMPLE_H
#define WIDGETEXAMPLE_H

#include "dtkwidget_global.h"
#include "examplewindowinterface.h"
#include "pagewindowinterface.h"

#include <DGuiApplicationHelper>

#include <QWidget>
#include <QLabel>
#include <QAbstractTableModel>
#include <QDate>

DGUI_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

class QTableView;

class WidgetExampleWindow : public PageWindowInterface
{
    Q_OBJECT

public:
    explicit WidgetExampleWindow(QWidget *parent = nullptr);
};

class DCalendarWidgetExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DCalendarWidgetExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DTableWidgetExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DTableWidgetExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;

private:
    QTableView *tableView;
};

class CalendarModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    CalendarModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

private:
    QStringList header;
    QVector<QVector<QDate>> m_tableData; // row = 6, col = 7
};

#endif // WIDGETEXAMPLE_H
