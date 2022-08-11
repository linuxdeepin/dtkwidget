// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
