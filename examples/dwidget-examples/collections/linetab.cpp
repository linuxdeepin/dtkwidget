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
//        setStyleSheet("LineTab{background-color: #252627;}");
        //////////////////////////////////////////////////////////////--DSeparator
        DHorizontalLine *dsHorizontal = new DHorizontalLine(this);
        dsHorizontal->setFixedSize(200,2);
        dsHorizontal->move(1,5);
        DVerticalLine *dsVertical = new DVerticalLine(this);
        dsVertical->setFixedSize(2,200);
        dsVertical->move(10,5);

        //////////////////////////////////////////////////////////////--DHeaderLine
        DHeaderLine *headerLine = new DHeaderLine(this);
        headerLine->setFixedSize(400, 30);
        headerLine->move(20,30);
        headerLine->setTitle("Control Library");

        //////////////////////////////////////////////////////////////--DBaseLine
        DBaseLine * baseLine = new DBaseLine(this);
        baseLine->setLeftContent(new QLabel("DBaseLine"));
        QPushButton *bbb = new QPushButton("Test button",this);
        bbb->setStyleSheet("background-color: red");
        baseLine->setRightContent(bbb);

        //////////////////////////////////////////////////////////////--DBaseExpand
        DBaseExpand *baseExpand = new DBaseExpand(this);
        baseExpand->setExpand(true);
        baseExpand->setFixedWidth(300);
        baseExpand->setHeader(baseLine);
        connect(bbb, &QPushButton::clicked, [=]{
            baseExpand->setExpand(!baseExpand->expand());
        });
        DHorizontalLine *dsHorizontal2 = new DHorizontalLine(this);
        dsHorizontal2->setFixedWidth(baseExpand->width());
        QLabel *expandLabel = new QLabel("Expand content");
        expandLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        expandLabel->setFixedSize(200,50);
        baseExpand->setContent(expandLabel);
        baseExpand->move(20,70);
        dsHorizontal2->move(baseExpand->x(),baseExpand->y() - 2);

        /////////////////////////////////////////////////////////////--Right content
        QLabel *rightWidget = new QLabel(this);
        rightWidget->setFixedSize(300, 500);
        rightWidget->move(400, 0);
        QVBoxLayout *rightLayout = new QVBoxLayout();
        rightLayout->setSpacing(0);
        rightLayout->setMargin(0);
        rightWidget->setLayout(rightLayout);

        /////////////////////////////////////////////////////////////--DArrowLineExpand
        DArrowLineExpand *arrowLineExpand = new DArrowLineExpand();
        arrowLineExpand->setFixedWidth(300);
        arrowLineExpand->setTitle("Arrow Line Expand");
        QLabel *expandLabel2 = new QLabel("Arrow Expand content");
        expandLabel2->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        expandLabel2->setFixedSize(200,200);
        arrowLineExpand->setContent(expandLabel2);
        connect(arrowLineExpand, &DArrowLineExpand::expandChange, [=]{
            expandLabel2->setFixedSize(expandLabel2->width(), expandLabel2->height() / 3 * 2);
        });
        /////////////////////////////////////////////////////////////--DArrowLineExpand2
        DArrowLineExpand *arrowLineExpand2 = new DArrowLineExpand();
        arrowLineExpand2->setHeaderHeight(50);
        arrowLineExpand2->setFixedWidth(300);
        arrowLineExpand2->setTitle("Arrow Line Expand");
        QLabel *expandLabel4 = new QLabel("Arrow Expand content");
        expandLabel4->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        expandLabel4->setFixedSize(200,200);
        arrowLineExpand2->setContent(expandLabel4);

        /////////////////////////////////////////////////////////////--DSwitchLineExpand
        DSwitchLineExpand *switchLineExpand = new DSwitchLineExpand();
        switchLineExpand->setFixedWidth(300);
        switchLineExpand->setTitle("Switch Line Expand");
        QLabel *expandLabel3 = new QLabel("Switch Expand content");
        expandLabel3->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        expandLabel3->setFixedSize(200,50);
        switchLineExpand->setContent(expandLabel3);

        DExpandGroup *group = new DExpandGroup(this);
        group->addExpand(arrowLineExpand);
        group->addExpand(arrowLineExpand2);

        rightLayout->addWidget(arrowLineExpand2);
        rightLayout->addWidget(arrowLineExpand);
        rightLayout->addWidget(switchLineExpand);
        rightLayout->addStretch(1);
}

