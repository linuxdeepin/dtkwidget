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

#include "segmentedcontrol.h"
#include <QLabel>

Segmentedcontrol::Segmentedcontrol(QWidget *parent) :
    QFrame(parent),
    segmentedControl(new DSegmentedControl(this))
{
    setStyleSheet("Segmentedcontrol{background: #1e1e1e;}");

    segmentedControl->addSegmented("page1");
    segmentedControl->addSegmented("page2");
    segmentedControl->addSegmented("page3");
    segmentedControl->move(100, 50);

    QLabel *label = new QLabel(this);
    QPalette pa = label->palette();
    pa.setColor(QPalette::Foreground, Qt::white);
    label->setPalette(pa);
    label->move(100, 150);
    label->setText(QString("current paga %1").arg(segmentedControl->currentIndex()+1));

    connect(segmentedControl, &DSegmentedControl::currentChanged, [=](){
        label->setText(QString("current paga %1").arg(segmentedControl->currentIndex()+1));
    });
}

