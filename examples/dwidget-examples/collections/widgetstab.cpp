/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#include "widgetstab.h"
#include "darrowrectangle.h"
#include "dtextedit.h"
#include "dlineedit.h"
#ifdef Q_OS_LINUX
#include "dmpriscontrol.h"
#include <DRegionMonitor>
#endif

#include <QCheckBox>
#include <QPushButton>

DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE

WidgetsTab::WidgetsTab(QWidget *parent) : QLabel(parent)
{
    setStyleSheet("WidgetsTab {background-color:#252627;} QCheckBox {color:#666;}");
    QHBoxLayout *mLayout = new QHBoxLayout(this);

#ifdef Q_OS_LINUX
    DRegionMonitor *rm = new DRegionMonitor(this);
    rm->registerRegion(QRegion(0, 0, 500, 500));
    Q_ASSERT(rm->registered());

    connect(rm, &DRegionMonitor::buttonPress, [=](const QPoint &p, const int flag) { qDebug() << "btn press:" << p << flag; });
    connect(rm, &DRegionMonitor::buttonRelease, [=](const QPoint &p, const int flag) { qDebug() << "btn release:" << p << flag; });
#endif
    QWidget *checkBoxWidget = new QWidget(this);
    QVBoxLayout *checkBoxLayout = new QVBoxLayout(checkBoxWidget);
    QCheckBox *lunarVisible = new QCheckBox(this);
    lunarVisible->setText("lunar visible");
    lunarVisible->setChecked(true);
    checkBoxLayout->addWidget(lunarVisible);
    QCheckBox *festivalHighlight = new QCheckBox(this);
    festivalHighlight->setText("lunar festival highlight");
    festivalHighlight->setChecked(true);
    checkBoxLayout->addWidget(festivalHighlight);
    QCheckBox *cellSelectable = new QCheckBox(this);
    cellSelectable->setText("cell selectable");
    cellSelectable->setChecked(true);
    checkBoxLayout->addWidget(cellSelectable);
    QCheckBox *controlPanelVisible = new QCheckBox(this);
    controlPanelVisible->setText("control panel visible");
    checkBoxLayout->addWidget(controlPanelVisible);
    controlPanelVisible->setChecked(true);
    QCheckBox *dateInfoVisible = new QCheckBox(this);
    dateInfoVisible->setText("date info visible");
    dateInfoVisible->setChecked(false);
    checkBoxLayout->addWidget(dateInfoVisible);
    QCheckBox *togglePic = new QCheckBox(this);
    togglePic->setText("toggle music picture");
    togglePic->setChecked(false);
    checkBoxLayout->addWidget(togglePic);
    mLayout->addWidget(checkBoxWidget);

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
    rectangle->setBackgroundColor(QColor(255, 255, 255, 127));

#ifdef Q_OS_LINUX
    // mpris
    DMPRISControl *control = new DMPRISControl(this);
    control->setFixedSize(300, 300);
    mLayout->addWidget(control);
    control->setStyleSheet("background-color:red;");

    connect(togglePic, SIGNAL(toggled(bool)), control, SLOT(setPictureVisible(bool)));
#endif
    QWidget *editWidget = new QWidget(this);
    QVBoxLayout *editLayout = new QVBoxLayout(editWidget);
    DLineEdit *pLineEdit = new DLineEdit(this);
    editLayout->addWidget(pLineEdit);

    DTextEdit *pTextEdit = new DTextEdit(this);
    editLayout->addWidget(pTextEdit);
    mLayout->addWidget(editWidget);
}

