// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QDebug>

#include "rubberbandexample.h"

DWIDGET_USE_NAMESPACE

RubberBandExampleWindow::RubberBandExampleWindow(QWidget *parent)
    : PageWindowInterface(parent)
{
    addExampleWindow(new DRubberBandExample(this));
}

DRubberBandExample::DRubberBandExample(QWidget *parent)
    : ExampleWindowInterface(parent)
    , m_pRubberBand(nullptr)
{
    QVBoxLayout *pVBoxLayout = new QVBoxLayout;
    pVBoxLayout->setContentsMargins(0, 0, 0, 0);
    pVBoxLayout->setSpacing(0);

    setLayout(pVBoxLayout);

    QLabel *pLabel_1 = new QLabel;
    QPixmap pix_1(":/images/example/DRubberBand.png");
    pLabel_1->setFixedSize(550, 356);
    pLabel_1->setPixmap(pix_1);
    pLabel_1->setScaledContents(true);

    pVBoxLayout->addStretch();
    pVBoxLayout->addWidget(pLabel_1, 0, Qt::AlignCenter);
    pVBoxLayout->addSpacing(30);
}

void DRubberBandExample::mousePressEvent(QMouseEvent *event)
{
    m_origin = event->pos();
    if (!m_pRubberBand)
        m_pRubberBand = new DRubberBand(DRubberBand::Rectangle, this);
    m_pRubberBand->setGeometry(QRect(m_origin, QSize()));
    m_pRubberBand->show();
}

void DRubberBandExample::mouseMoveEvent(QMouseEvent *event)
{
    m_pRubberBand->setGeometry(QRect(m_origin, event->pos()).normalized());
}

void DRubberBandExample::mouseReleaseEvent(QMouseEvent * /*event*/)
{
    m_pRubberBand->hide();
}

QString DRubberBandExample::getTitleName() const
{
    return "DRubberBand";
}

QString DRubberBandExample::getDescriptionInfo() const
{
    return "所有用户可以用鼠标拖拽矩形区域进行\n"
           "框选的地方,比如文件管理器,桌面.";
}

int DRubberBandExample::getFixedHeight() const
{
    return 810;
}
