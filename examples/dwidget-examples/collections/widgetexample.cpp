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

    //生成一个6行7列的数组
    for (int index = 0; index < 6; ++index) {
        m_tableData.push_back(QVector<QDate>(7));
    }

    const QDate curMonthFirstDay(QDate::currentDate().year(), QDate::currentDate().month(), 1);
    const int first_day_index = curMonthFirstDay.dayOfWeek() % 7;
    //本月第一天在数组中初始化
    m_tableData[0][first_day_index] = curMonthFirstDay;

    const int days_in_month = QDate::currentDate().daysInMonth();
    const QDate curMonthLastDay(QDate::currentDate().year(), QDate::currentDate().month(), days_in_month);

    const int last_day_index = first_day_index + days_in_month - 1;
    //本月最后一天在数组中初始化
    m_tableData[last_day_index / 7][last_day_index % 7] = curMonthLastDay;

    for (int rowIndex = 0; rowIndex < 6; ++rowIndex) {
        for (int colIndex = 0; colIndex < 7; ++colIndex) {
            if (m_tableData[rowIndex][colIndex].isValid()) {
                continue;
            }

            int index = rowIndex * 7 + colIndex;
            m_tableData[rowIndex][colIndex] = curMonthFirstDay.addDays(index - first_day_index);
        }
    }
}

int CalendarModel::rowCount(const QModelIndex &) const
{
    return m_tableData.size();
}

int CalendarModel::columnCount(const QModelIndex &) const
{
    return header.size();
}

QVariant CalendarModel::data(const QModelIndex &index, int role) const
{
    DPalette palette = DGuiApplicationHelper::instance()->applicationPalette();

    switch (role) {
    case Qt::DisplayRole: {
        int days = m_tableData[index.row()][index.column()].day();
        if (days == 1) {
            return QString("%1/1").arg(m_tableData[index.row()][index.column()].month());
        } else {
            return QString::number(days);
        }
    }
    case Qt::TextColorRole: {
        // 设置文字颜色
        if (m_tableData[index.row()][index.column()].month() == QDate::currentDate().month()) {
            return palette.color(DPalette::TextTitle);
        } else {
            return palette.color(DPalette::PlaceholderText);
        }
    }
    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;
    case Qt::BackgroundRole:
        // 设置单元格背景色
        break;
    default:
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
