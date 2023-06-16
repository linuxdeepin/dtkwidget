// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>

#include <DStyle>
#include <DLCDNumber>

#include "lcdnumberexample.h"

DWIDGET_USE_NAMESPACE

LCDNumberExampleWindow::LCDNumberExampleWindow(QWidget *parent)
    : PageWindowInterface(parent)
{
    addExampleWindow(new DLCDNumberExample(this));
}

DLCDNumberExample::DLCDNumberExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *pVBoxLayout = new QVBoxLayout;
    pVBoxLayout->setContentsMargins(0, 0, 0, 0);
    pVBoxLayout->setSpacing(0);
    setLayout(pVBoxLayout);

    QHBoxLayout *pHBoxLayout_1 = new QHBoxLayout;
    pHBoxLayout_1->setContentsMargins(0, 0, 0, 0);
    pHBoxLayout_1->setSpacing(0);

    DLCDNumber *pNumber = new DLCDNumber;
    pNumber->setFixedSize(82, 64);
    pNumber->setDecMode();
    pNumber->setDigitCount(2);
    pNumber->display("08");
    pNumber->setFrameShape(QFrame::NoFrame);
    pHBoxLayout_1->addWidget(pNumber);

    pVBoxLayout->addLayout(pHBoxLayout_1);

    QLabel *pLabel_1 = new QLabel;
    QPixmap pix_1(":/images/example/DLCDNumber.png");
    pLabel_1->setFixedSize(568, 444);
    pLabel_1->setPixmap(pix_1);
    pLabel_1->setScaledContents(true);

    QHBoxLayout *pHBoxLayout_pic_1 = new QHBoxLayout;
    pHBoxLayout_pic_1->setContentsMargins(0, 0, 0, 0);
    pHBoxLayout_pic_1->setSpacing(0);
    pHBoxLayout_pic_1->addWidget(pLabel_1);

    pVBoxLayout->addSpacing(30);
    pVBoxLayout->addLayout(pHBoxLayout_pic_1);
    pVBoxLayout->addSpacing(20);
}

QString DLCDNumberExample::getTitleName() const
{
    return "DLCDNumber";
}

QString DLCDNumberExample::getDescriptionInfo() const
{
    return "需要用到电子数字的地方";
}

int DLCDNumberExample::getFixedHeight() const
{
    return 630;
}
