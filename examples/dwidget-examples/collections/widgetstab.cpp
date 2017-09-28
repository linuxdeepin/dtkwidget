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

#include "widgetstab.h"
#include "darrowrectangle.h"
#ifdef Q_OS_LINUX
#include "dmpriscontrol.h"
#include "dregionmonitor.h"
using Dtk::Widget::DRegionMonitor;
#endif


#include <QCheckBox>


WidgetsTab::WidgetsTab(QWidget *parent) : QLabel(parent)
{
    setStyleSheet("WidgetsTab {background-color:#252627;} QCheckBox {color:#666;}");

#ifdef Q_OS_LINUX
    DRegionMonitor *rm = new DRegionMonitor(this);
    rm->registerRegion(QRegion(0, 0, 500, 500));
    Q_ASSERT(rm->registered());

    connect(rm, &DRegionMonitor::buttonPress, [=](const QPoint &p, const int flag) { qDebug() << "btn press:" << p << flag; });
    connect(rm, &DRegionMonitor::buttonRelease, [=](const QPoint &p, const int flag) { qDebug() << "btn release:" << p << flag; });
#endif

    QCheckBox *lunarVisible = new QCheckBox(this);
    lunarVisible->setText("lunar visible");
    lunarVisible->move(320, 50);
    lunarVisible->setChecked(true);
    QCheckBox *festivalHighlight = new QCheckBox(this);
    festivalHighlight->setText("lunar festival highlight");
    festivalHighlight->setChecked(true);
    festivalHighlight->move(320, 80);
    QCheckBox *cellSelectable = new QCheckBox(this);
    cellSelectable->setText("cell selectable");
    cellSelectable->setChecked(true);
    cellSelectable->move(320, 110);
    QCheckBox *controlPanelVisible = new QCheckBox(this);
    controlPanelVisible->setText("control panel visible");
    controlPanelVisible->move(320, 140);
    controlPanelVisible->setChecked(true);
    QCheckBox *dateInfoVisible = new QCheckBox(this);
    dateInfoVisible->setText("date info visible");
    dateInfoVisible->setChecked(false);
    dateInfoVisible->move(320, 170);

    ////////////////////ArrowRectangle//////////////////
    DArrowRectangle *rectangle = new DArrowRectangle(DArrowRectangle::ArrowLeft);
//    rectangle->setArrowY(15);
    rectangle->setArrowX(25);
    rectangle->setArrowWidth(30);
    rectangle->setArrowHeight(10);
    QPushButton *button = new QPushButton("DArrowRectangle Test");
    button->setFixedSize(200, 30);
    rectangle->setContent(button);
    rectangle->show(200, 200);
    rectangle->setBackgroundColor(QColor(255, 255, 255, 0.5));

#ifdef Q_OS_LINUX
    // mpris
    DMPRISControl *control = new DMPRISControl(this);
    control->setFixedSize(300, 300);
    control->move(500, 0);
    control->setStyleSheet("background-color:red;");
#endif
}

