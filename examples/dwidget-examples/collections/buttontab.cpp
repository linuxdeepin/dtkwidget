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

#include "buttontab.h"

#include <QDebug>

DWIDGET_USE_NAMESPACE

ButtonTab::ButtonTab(QWidget *parent) : QLabel(parent)
{
    setStyleSheet("ButtonTab{background-color: #252627;}");
    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout *winButtonLayout = new QHBoxLayout;
    QHBoxLayout *imageButtonLayout = new QHBoxLayout;

    winButtonLayout->setSpacing(20);
    imageButtonLayout->setSpacing(20);
    layout->addLayout(winButtonLayout, 1);
    layout->addLayout(imageButtonLayout, 5);

    DArrowButton * arrowButton = new DArrowButton(this);
    arrowButton->setArrowDirection(DArrowButton::ArrowDown);
    winButtonLayout->addWidget(arrowButton);

    DWindowMinButton * minButton = new DWindowMinButton(this);
    winButtonLayout->addWidget(minButton);

    DWindowMaxButton * maxButton = new DWindowMaxButton(this);
    winButtonLayout->addWidget(maxButton);

    DWindowCloseButton * closeButton = new DWindowCloseButton(this);
    winButtonLayout->addWidget(closeButton);

    DWindowOptionButton * optionButton = new DWindowOptionButton(this);
    winButtonLayout->addWidget(optionButton);
    winButtonLayout->addStretch();

    //////////////////////////////////////////////////////////////--DTextButton

    DIconButton *imageButton = new DIconButton(this);
    imageButton->setIcon(QIcon(":/images/button.png"));
    imageButtonLayout->addWidget(imageButton);
    imageButton->setChecked(true);

    DIconButton *imageButton2 = new DIconButton(this);
    imageButton2->setIcon(QIcon(":/images/buttonHover.png"));
    imageButtonLayout->addWidget(imageButton2);
    // 设置button为圆形
    imageButton2->setEnabledCircle(true);

    imageButtonLayout->addStretch();

    connect(imageButton2, SIGNAL(clicked()), this, SLOT(buttonClickTest()));

    DSwitchButton *switchButton = new DSwitchButton(this);
    layout->addWidget(switchButton, 1);
}

void ButtonTab::buttonClickTest()
{
    qDebug() << "clicked";
}
