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

#include "widgetexample.h"

#include <DCalendarWidget>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QTableView>
#include <QDate>
#include <QDebug>

WidgetExampleWindow::WidgetExampleWindow(QWidget *parent)
    : PageWindowInterface(parent)
{
    addExampleWindow(new DCalendarWidgetExample(this));
    addExampleWindow(new DTableWidgetExample(this));
}

DCalendarWidgetExample::DCalendarWidgetExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    DCalendarWidget *calender = new DCalendarWidget(this);
    QLabel *label = new QLabel(this);
    label->setPixmap(QPixmap("://images/example/DCalendarWidget.png"));
    label->setScaledContents(true);
    label->setFixedSize(550, 406);

    mainLayout->addWidget(calender, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(50);
    mainLayout->addWidget(label, 0, Qt::AlignHCenter);
}

QString DCalendarWidgetExample::getTitleName() const
{
    return "DCalendarWidget";
}

QString DCalendarWidgetExample::getDescriptionInfo() const
{
    return "所涉及到日期操作的地方。";
}

int DCalendarWidgetExample::getFixedHeight() const
{
    return 816;
}

DTableWidgetExample::DTableWidgetExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    tableView = new QTableView(this);
    CalendarModel *model = new CalendarModel(this);
    tableView->setModel(model);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setShowGrid(false);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->verticalHeader()->hide();
    tableView->horizontalHeader()->setSectionsClickable(false);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    tableView->setFixedSize(504, 252);
    mainLayout->addWidget(tableView, 0, Qt::AlignHCenter);
}

QString DTableWidgetExample::getTitleName() const
{
    return "DTableWidget";
}

QString DTableWidgetExample::getDescriptionInfo() const
{
    return "标准的表格控件。";
}

int DTableWidgetExample::getFixedHeight() const
{
    return 352;
}

CalendarModel::CalendarModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    header << "周日"
           << "周一"
           << "周二"
           << "周三"
           << "周四"
           << "周五"
           << "周六";
    QDate date = QDate::currentDate();
    days = date.daysInMonth();
    month = date.month();
    day = date.day();
    firstDayOfWeek = date.addDays(1 - day).dayOfWeek() % 7;
    lastDayOfWeek = date.addDays(days - day).dayOfWeek() % 7;
    preDays = date.addMonths(-1).daysInMonth();

    if (firstDayOfWeek == 0) {
        firstDayRow = 1;
        if (lastDayOfWeek == 6)
            lastDayRow = days / 7;
        else
            lastDayRow = days / 7 + 1;
    } else {
        firstDayRow = 0;
        if (lastDayOfWeek == 6)
            lastDayRow = (days + firstDayOfWeek) / 7 - 1;
        else
            lastDayRow = (days + firstDayOfWeek) / 7;
    }
}

int CalendarModel::rowCount(const QModelIndex &parent) const
{
    return 6;
}

int CalendarModel::columnCount(const QModelIndex &parent) const
{
    return header.size();
}

QVariant CalendarModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
    DPalette palette = DGuiApplicationHelper::instance()->applicationPalette();

    switch (role) {
    case Qt::DisplayRole: {
        // 设置文字
        if (firstDayOfWeek == 0) {
            if (row == 0)
                return preDays + col - 6;
            if (row == firstDayRow && col == 0)
                return QString("%1/1").arg(month);
            if (row < lastDayRow)
                return col + 1 + (row - firstDayRow) * 7;
            else if (row == lastDayRow) {
                if (col <= lastDayOfWeek) {
                    return col + 1 + (row - firstDayRow) * 7;
                } else if (col == lastDayOfWeek + 1) {
                    return QString("%1/1").arg(month + 1);
                } else {
                    return col - lastDayOfWeek + (row - lastDayRow) * 7;
                }
            } else {
                return col - lastDayOfWeek + (row + 1 - lastDayRow) * 7;
            }
        } else {
            if (row == 0) {
                if (col < firstDayOfWeek)
                    return (preDays + col + 1 - firstDayOfWeek);
                else if (col == firstDayOfWeek)
                    return QString("%1/1").arg(month);
                else
                    return col - firstDayOfWeek + 1;
            } else {
                if (lastDayOfWeek != 6) {
                    if (row < lastDayRow) {
                        return col - firstDayOfWeek + 1 + row * 7;
                    } else if (row == lastDayRow) {
                        if (col <= lastDayOfWeek)
                            return col - firstDayOfWeek + 1 + row * 7;
                        else if (col == lastDayOfWeek + 1)
                            return QString("%1/1").arg(month + 1);
                        else
                            return col - lastDayOfWeek + (row - lastDayRow) * 7;
                    } else {
                        return col - lastDayOfWeek + (row - lastDayRow) * 7;
                    }
                } else {
                    if (row <= lastDayRow) {
                        return col - firstDayOfWeek + 1 + row * 7;
                    } else if (row == lastDayRow + 1) {
                        if (col == 0)
                            return QString("%1/1").arg(month + 1);
                        else
                            return col - lastDayOfWeek + (row - lastDayRow) * 7;
                    } else {
                        return col - lastDayOfWeek + (row - lastDayRow) * 7;
                    }
                }
            }
        }
    } break;
    case Qt::TextColorRole: {
        // 设置文字颜色
        if (row < firstDayRow) {
            return palette.color(DPalette::PlaceholderText);
        } else if (row == firstDayRow) {
            if (col < firstDayOfWeek)
                return palette.color(DPalette::PlaceholderText);
            else
                return palette.color(DPalette::TextTitle);
        } else if (row < lastDayRow) {
            return palette.color(DPalette::TextTitle);
        } else if (row == lastDayRow) {
            if (col <= lastDayOfWeek)
                return palette.color(DPalette::TextTitle);
            else
                palette.color(DPalette::PlaceholderText);
        } else {
            return palette.color(DPalette::PlaceholderText);
        }
    } break;
    case Qt::BackgroundRole:
        // 设置单元格背景色
        break;
    }
    return QVariant();
}

QVariant CalendarModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::BackgroundColorRole || role == Qt::BackgroundRole)
        return DGuiApplicationHelper::instance()->applicationPalette().brush(DPalette::ItemBackground);
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return header.value(section);
    } else {
        return QVariant();
    }
}
