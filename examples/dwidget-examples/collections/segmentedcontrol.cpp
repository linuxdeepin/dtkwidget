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

#include "segmentedcontrol.h"
#include <QLabel>

Segmentedcontrol::Segmentedcontrol(QWidget *parent) :
    QFrame(parent),
    buttonBox(new DButtonBox(this))
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    buttonBox->setButtonList({new DButtonBoxButton("page 1"), new DButtonBoxButton("page 2"), new DButtonBoxButton("page 3")}, true);

    for (int i = 0; i < buttonBox->buttonList().count(); ++i)
        buttonBox->setId(buttonBox->buttonList().at(i), i+1);

    layout->addWidget(buttonBox, 1, Qt::AlignCenter);

    QLabel *label = new QLabel(this);
    QPalette pa = label->palette();
    pa.setColor(QPalette::Foreground, Qt::blue);
    label->setPalette(pa);
    layout->addWidget(label, 5, Qt::AlignCenter);

    connect(buttonBox, &DButtonBox::buttonClicked, [=](QAbstractButton *button){
        label->setText(QString("current paga %1").arg(buttonBox->id(button)));
    });
}

