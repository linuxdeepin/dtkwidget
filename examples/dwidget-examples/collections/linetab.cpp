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

#include "linetab.h"

#include <DHorizontalLine>
#include <DVerticalLine>

LineTab::LineTab(QWidget *parent) : QLabel(parent)
{
    setStyleSheet("LineTab{background-color: #252627;}");
    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout *drawerLayout = new QHBoxLayout;
    //////////////////////////////////////////////////////////////--DSeparator
    DHorizontalLine *dsHorizontal = new DHorizontalLine(this);
    dsHorizontal->setFixedSize(200, 2);
    layout->addWidget(dsHorizontal, 0, Qt::AlignLeft | Qt::AlignVCenter);
    DVerticalLine *dsVertical = new DVerticalLine(this);
    dsVertical->setFixedSize(2, 200);
    drawerLayout->addWidget(dsVertical, 0, Qt::AlignTop | Qt::AlignHCenter);

    //////////////////////////////////////////////////////////////--DHeaderLine
    DHeaderLine *headerLine = new DHeaderLine(this);
    headerLine->setFixedSize(400, 30);
    layout->addWidget(headerLine);
    headerLine->setTitle("Control Library");
    layout->addLayout(drawerLayout);

    //////////////////////////////////////////////////////////////--DBaseLine
    DBaseLine *baseLine = new DBaseLine(this);
    baseLine->setLeftContent(new QLabel("DBaseLine"));
    QPushButton *bbb = new QPushButton("Test button", this);
    bbb->setStyleSheet("background-color: red");
    baseLine->setRightContent(bbb);

    //////////////////////////////////////////////////////////////--DDrawer
    DDrawer *baseExpand = new DDrawer(this);
    baseExpand->setExpand(true);
    baseExpand->setFixedWidth(300);
    baseExpand->setHeader(baseLine);
    connect(bbb, &QPushButton::clicked, [=] {
        baseExpand->setExpand(!baseExpand->expand());
    });
    DHorizontalLine *dsHorizontal2 = new DHorizontalLine(this);
    dsHorizontal2->setFixedWidth(baseExpand->width());
    QLabel *expandLabel = new QLabel("Expand content");
    expandLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    expandLabel->setFixedSize(200, 50);
    baseExpand->setContent(expandLabel);
    drawerLayout->addWidget(baseExpand, 0, Qt::AlignTop | Qt::AlignHCenter);
    dsHorizontal2->move(baseExpand->x(), baseExpand->y() - 2);

    /////////////////////////////////////////////////////////////--Right content
    QLabel *rightWidget = new QLabel(this);
    rightWidget->setFixedSize(300, 500);
    drawerLayout->addWidget(rightWidget);
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(0);
    rightLayout->setMargin(0);
    rightWidget->setLayout(rightLayout);

    /////////////////////////////////////////////////////////////--DArrowLineDrawer
    DArrowLineDrawer *arrowLineExpand = new DArrowLineDrawer();
    arrowLineExpand->setFixedWidth(300);
    arrowLineExpand->setTitle("Arrow Line Drawer");
    QLabel *expandLabel2 = new QLabel("Arrow Drawer content");
    expandLabel2->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    expandLabel2->setFixedSize(200, 200);
    arrowLineExpand->setContent(expandLabel2);
    connect(arrowLineExpand, &DArrowLineDrawer::expandChange, [=] {
        expandLabel2->setFixedSize(expandLabel2->width(), expandLabel2->height() / 3 * 2);
    });
    /////////////////////////////////////////////////////////////--DArrowLineExpand2
    DArrowLineDrawer *arrowLineExpand2 = new DArrowLineDrawer();
    arrowLineExpand2->setHeaderHeight(50);
    arrowLineExpand2->setFixedWidth(300);
    arrowLineExpand2->setTitle("Arrow Line Drawer");
    QLabel *expandLabel4 = new QLabel("Arrow Drawer content");
    expandLabel4->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    expandLabel4->setFixedSize(200, 200);
    arrowLineExpand2->setContent(expandLabel4);

    /////////////////////////////////////////////////////////////--DSwitchLineExpand
    DSwitchLineExpand *switchLineExpand = new DSwitchLineExpand();
    switchLineExpand->setFixedWidth(300);
    switchLineExpand->setTitle("Switch Line Expand");
    QLabel *expandLabel3 = new QLabel("Switch Expand content");
    expandLabel3->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    expandLabel3->setFixedSize(200, 50);
    switchLineExpand->setContent(expandLabel3);

    DDrawerGroup *group = new DDrawerGroup(this);
    group->addExpand(arrowLineExpand);
    group->addExpand(arrowLineExpand2);

    rightLayout->addWidget(arrowLineExpand2);
    rightLayout->addWidget(arrowLineExpand);
    rightLayout->addWidget(switchLineExpand);
    rightLayout->addStretch(1);
}
