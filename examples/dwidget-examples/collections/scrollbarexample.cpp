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

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <DScrollArea>
#include <QImageReader>
#include <DFrame>
#include "scrollbarexample.h"

DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE

ScrollBarExampleWindow::ScrollBarExampleWindow(QWidget *parent)
    : PageWindowInterface(parent)
{
    addExampleWindow(new DScrollBarExample(this));
}

DScrollBarExample::DScrollBarExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *mLayout = new QVBoxLayout(this);
    QImageReader reader;
    reader.setFileName(":/images/example/DScrollBar_1.png");
    reader.setScaledSize(reader.size().scaled(480, 120, Qt::KeepAspectRatio));

    QLabel *sbimg = new QLabel;
    sbimg->setAlignment(Qt::AlignHCenter);
    sbimg->setPixmap(QPixmap::fromImageReader(&reader));

    DScrollArea *sa = new DScrollArea;
    QLabel *image = new QLabel;
    image->setPixmap(QPixmap(":/images/example/DScrollBar.png"));
    sa->setWidget(image);

    mLayout->addWidget(sbimg, 0, Qt::AlignTop | Qt::AlignHCenter);
    mLayout->addSpacing(5);
    mLayout->addWidget(sa);
}

QString DScrollBarExample::getTitleName() const
{
    return "DScrollBar";
}

QString DScrollBarExample::getDescriptionInfo() const
{
    return QStringLiteral("所有产生滚动的地方");
}

int DScrollBarExample::getFixedHeight() const
{
    return 500;
}
