// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "pagewindowinterface.h"
#include "examplewindowinterface.h"

#include <DFrame>

#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>
#include <QScrollArea>

DWIDGET_USE_NAMESPACE

PageWindowInterface::PageWindowInterface(QWidget *parent)
    : QWidget(parent)
{
}

PageWindowInterface::~PageWindowInterface()
{
}

QWidget *PageWindowInterface::doLayout(ExampleWindowInterface *pExample)
{
    Q_ASSERT(pExample != nullptr);

    DFrame *pWidget = new DFrame;
    pWidget->setFrameRounded(true);

    QLabel *pDescriptionLabel = new QLabel;
    pDescriptionLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    pDescriptionLabel->setFixedWidth(292);
    pDescriptionLabel->setFixedHeight(pExample->getFixedHeight());

    QLabel *pLabel_1 = new QLabel;
    pLabel_1->setTextInteractionFlags(Qt::TextBrowserInteraction);
    pLabel_1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QFont font_1;
    font_1.setPixelSize(24);
    pLabel_1->setFont(font_1);
    pLabel_1->setText(pExample->getTitleName());

    QLabel *pLabel_2 = new QLabel;
    pLabel_2->setTextInteractionFlags(Qt::TextBrowserInteraction);
    pLabel_2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QFont font_2;
    font_2.setPixelSize(12);
    pLabel_2->setFont(font_2);
    pLabel_2->setText(pExample->getDescriptionInfo());

    QVBoxLayout *pVBoxLayout_label = new QVBoxLayout;
    pVBoxLayout_label->setMargin(10);
    pVBoxLayout_label->setSpacing(0);
    pDescriptionLabel->setLayout(pVBoxLayout_label);

    pVBoxLayout_label->addWidget(pLabel_1);
    pVBoxLayout_label->setSpacing(10);
    pVBoxLayout_label->addWidget(pLabel_2);
    pVBoxLayout_label->addStretch();

    QHBoxLayout *pHBoxLayout = new QHBoxLayout;
    pHBoxLayout->setMargin(0);
    pHBoxLayout->setSpacing(0);

    pWidget->setLayout(pHBoxLayout);

    pExample->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pHBoxLayout->addWidget(pDescriptionLabel);
    pHBoxLayout->addWidget(new DVerticalLine);
    pHBoxLayout->addWidget(pExample);

    pWidget->setFixedHeight(pExample->getFixedHeight());

    return pWidget;
}

void PageWindowInterface::initPageWindow()
{
    QScrollArea *pArea = new QScrollArea(this);

    QWidget *pWidget = new QWidget(this);

    QVBoxLayout *pVBoxLayout = new QVBoxLayout;
    pVBoxLayout->setMargin(10);
    pVBoxLayout->setSpacing(10);
    pWidget->setLayout(pVBoxLayout);

    for (auto pExample : m_exampleList) {
        pVBoxLayout->addWidget(doLayout(pExample));
    }

    pVBoxLayout->addStretch();

    pArea->setWidget(pWidget);
    pArea->setWidgetResizable(true);

    QHBoxLayout *pHBoxLayout = new QHBoxLayout;
    pHBoxLayout->setMargin(0);
    pHBoxLayout->setSpacing(0);
    setLayout(pHBoxLayout);
    pHBoxLayout->addWidget(pArea);
}

void PageWindowInterface::mouseMoveEvent(QMouseEvent *event)
{
    //屏蔽掉鼠标移动事件
    event->accept();
}

void PageWindowInterface::addExampleWindow(ExampleWindowInterface *pExample)
{
    m_exampleList << pExample;
}
