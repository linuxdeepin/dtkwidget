// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>

#include <DStyle>
#include <DSpinner>

#include "spinnerexample.h"

DWIDGET_USE_NAMESPACE

SpinnerExampleWindow::SpinnerExampleWindow(QWidget *parent)
    : PageWindowInterface(parent)
{
    addExampleWindow(new DSpinnerExample(this));
}

DSpinnerExample::DSpinnerExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *pVBoxLayout = new QVBoxLayout;
    pVBoxLayout->setContentsMargins(0, 0, 0, 0);
    pVBoxLayout->setSpacing(0);
    setLayout(pVBoxLayout);

    QHBoxLayout *pHBoxLayout_1 = new QHBoxLayout;
    pHBoxLayout_1->setContentsMargins(0, 0, 0, 0);
    pHBoxLayout_1->setSpacing(0);

    auto getSpinnerWidget = [](const QSize &size) {
        DSpinner *pSpinner = new DSpinner;
        pSpinner->setFixedSize(size);
        pSpinner->start();
        return pSpinner;
    };

    pHBoxLayout_1->addStretch(1);
    pHBoxLayout_1->addWidget(getSpinnerWidget(QSize(16, 16)));
    pHBoxLayout_1->addStretch(1);
    pHBoxLayout_1->addWidget(getSpinnerWidget(QSize(24, 24)));
    pHBoxLayout_1->addStretch(1);
    pHBoxLayout_1->addWidget(getSpinnerWidget(QSize(32, 32)));
    pHBoxLayout_1->addStretch(1);
    pHBoxLayout_1->addWidget(getSpinnerWidget(QSize(54, 54)));
    pHBoxLayout_1->addStretch(1);
    pHBoxLayout_1->addWidget(getSpinnerWidget(QSize(96, 96)));
    pHBoxLayout_1->addStretch(1);

    pVBoxLayout->addLayout(pHBoxLayout_1);

    QLabel *pLabel_1 = new QLabel;
    QPixmap pix_1(":/images/example/DSpinner.png");
    pLabel_1->setFixedSize(570, 326);
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

QString DSpinnerExample::getTitleName() const
{
    return "DSpinner";
}

QString DSpinnerExample::getDescriptionInfo() const
{
    return "所有需要用户等待的地方,且没有具体\n"
           "的等待时间,不知道进度,可能很快也\n"
           "可能需要比较久.";
}

int DSpinnerExample::getFixedHeight() const
{
    return 800;
}
