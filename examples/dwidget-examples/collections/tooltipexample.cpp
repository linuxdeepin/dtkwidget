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
#include <QStandardItemModel>
#include <QDebug>

#include <DStyle>
#include <DPushButton>
#include <DArrowRectangle>
#include <DListView>
#include <DLabel>
#include <DHorizontalLine>
#include <DProgressBar>

#include "tooltipexample.h"

DWIDGET_USE_NAMESPACE

ToolTipExampleWindow::ToolTipExampleWindow(QWidget *parent)
    : PageWindowInterface(parent)
{
    addExampleWindow(new DToolTipExample(this));
    addExampleWindow(new DArrowRectangleExample(this));
}

DToolTipExample::DToolTipExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *pVBoxLayout = new QVBoxLayout;
    pVBoxLayout->setMargin(0);
    pVBoxLayout->setSpacing(0);
    setLayout(pVBoxLayout);

    QHBoxLayout *pHBoxLayout_1 = new QHBoxLayout;
    pHBoxLayout_1->setMargin(0);
    pHBoxLayout_1->setSpacing(0);

    DPushButton *pButton_1 = new DPushButton("悬停显示ToolTip");
    pButton_1->setToolTip("返回上一级");

    DPushButton *pButton_2 = new DPushButton("悬停显示ToolTip");
    pButton_2->setToolTip("点击搜索或输入地址");

    pHBoxLayout_1->addStretch(1);
    pHBoxLayout_1->addWidget(pButton_1);
    pHBoxLayout_1->addStretch(1);
    pHBoxLayout_1->addWidget(pButton_2);
    pHBoxLayout_1->addStretch(1);

    pVBoxLayout->addLayout(pHBoxLayout_1);

    QLabel *pLabel_1 = new QLabel;
    QPixmap pix_1(":/images/example/DToolTip.png");
    pLabel_1->setFixedSize(550, 356);
    pLabel_1->setPixmap(pix_1);
    pLabel_1->setScaledContents(true);

    QHBoxLayout *pHBoxLayout_pic_1 = new QHBoxLayout;
    pHBoxLayout_pic_1->setMargin(0);
    pHBoxLayout_pic_1->setSpacing(0);
    pHBoxLayout_pic_1->addWidget(pLabel_1);

    pVBoxLayout->addSpacing(30);
    pVBoxLayout->addLayout(pHBoxLayout_pic_1);
    pVBoxLayout->addSpacing(20);
}

QString DToolTipExample::getTitleName() const
{
    return "DToolTip";
}

QString DToolTipExample::getDescriptionInfo() const
{
    return "所有需要用到提示的地方.\n"
           "出现需要有延迟,鼠标是悬停2妙左右\n"
           "出现,触屏是按住就出现.";
}

int DToolTipExample::getFixedHeight() const
{
    return 600;
}

DArrowRectangleExample::DArrowRectangleExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *pVBoxLayout = new QVBoxLayout;
    pVBoxLayout->setMargin(0);
    pVBoxLayout->setSpacing(0);
    setLayout(pVBoxLayout);

    QHBoxLayout *pHBoxLayout_1 = new QHBoxLayout;
    pHBoxLayout_1->setMargin(0);
    pHBoxLayout_1->setSpacing(0);

    //DArrowRectangle的FloatWidget模式必须要有父窗口
    DArrowRectangle *pRectangle_1 = new DArrowRectangle(DArrowRectangle::ArrowBottom, DArrowRectangle::FloatWidget, this);
    pRectangle_1->setRadiusArrowStyleEnable(true);
    pRectangle_1->setRadius(8);
    QWidget *pContentWidget = new QWidget;
    pContentWidget->setFixedSize(300, 300);
    pRectangle_1->setContent(pContentWidget);
    QVBoxLayout *pVBoxLayout_content = new QVBoxLayout;
    pVBoxLayout_content->setMargin(0);
    pVBoxLayout_content->setSpacing(0);
    pContentWidget->setLayout(pVBoxLayout_content);

    DLabel *pTitle = new DLabel("图标列表");
    pTitle->setFixedHeight(46);
    QFont font;
    font.setPixelSize(20);
    pTitle->setFont(font);

    auto getListItem = [](const QPixmap &pixIcon, const QString &str) {
        QLabel *pLeftIcon = new QLabel;
        pLeftIcon->setPixmap(pixIcon);

        QLabel *rightIcon = new QLabel;
        rightIcon->setPixmap(QPixmap(":/images/example/Oval_186.svg"));

        QLabel *middleLabel = new QLabel;
        middleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        QLabel *pTitle_1 = new QLabel;
        pTitle_1->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        pTitle_1->setText(str);
        QFont font;
        font.setPixelSize(14);
        pTitle_1->setFont(font);

        QLabel *pTitle_2 = new QLabel;
        pTitle_2->setText("23/48G");
        font.setPixelSize(12);
        pTitle_2->setFont(font);

        QVBoxLayout *pVBoxLayout = new QVBoxLayout;
        pVBoxLayout->setMargin(0);
        pVBoxLayout->setSpacing(0);
        middleLabel->setLayout(pVBoxLayout);

        pVBoxLayout->addSpacing(11);
        pVBoxLayout->addWidget(pTitle_1);
        pVBoxLayout->addWidget(pTitle_2);

        QProgressBar *pBar = new QProgressBar;
        pBar->setFixedHeight(3);
        pBar->setRange(0, 100);
        pBar->setValue(60);

        pVBoxLayout->addWidget(pBar);
        pVBoxLayout->addSpacing(10);

        QHBoxLayout *pHBoxLayout = new QHBoxLayout;
        pHBoxLayout->setMargin(0);
        pHBoxLayout->setSpacing(0);
        pHBoxLayout->addSpacing(10);
        pHBoxLayout->addWidget(pLeftIcon);
        pHBoxLayout->addSpacing(10);
        pHBoxLayout->addWidget(middleLabel);
        pHBoxLayout->addSpacing(10);
        pHBoxLayout->addWidget(rightIcon);
        pHBoxLayout->addSpacing(12);

        QWidget *pItem = new QWidget;
        pItem->setFixedHeight(64);
        pItem->setLayout(pHBoxLayout);
        return pItem;
    };

    pVBoxLayout_content->addWidget(pTitle);
    pVBoxLayout_content->addWidget(new DHorizontalLine);
    pVBoxLayout_content->addWidget(getListItem(QPixmap(":/images/example/drive-harddisk-48px.svg"), "我的磁盘"));
    pVBoxLayout_content->addWidget(getListItem(QPixmap(":/images/example/drive-harddisk-48px_1.svg"), "文档"));
    pVBoxLayout_content->addWidget(getListItem(QPixmap(":/images/example/drive-harddisk-48px_2.svg"), "可移动磁盘"));
    pVBoxLayout_content->addWidget(getListItem(QPixmap(":/images/example/drive-harddisk-48px_3.svg"), "SD卡"));
    pVBoxLayout_content->addStretch();

    DArrowRectangle *pRectangle_2 = new DArrowRectangle(DArrowRectangle::ArrowBottom, DArrowRectangle::FloatWidget, this);
    pRectangle_2->setRadius(8);
    pRectangle_2->setRadiusArrowStyleEnable(true);
    QLabel *pContentWidget_2 = new QLabel("深度音乐");
    pContentWidget_2->setAlignment(Qt::AlignCenter);
    pContentWidget_2->setFixedSize(90, 40);
    pRectangle_2->setContent(pContentWidget_2);

    QVBoxLayout *pVBoxLayout_1 = new QVBoxLayout;
    pVBoxLayout_1->setMargin(0);
    pVBoxLayout_1->setSpacing(0);
    pVBoxLayout_1->addWidget(pRectangle_2);
    pVBoxLayout_1->addStretch();

    pHBoxLayout_1->addStretch();
    pHBoxLayout_1->addWidget(pRectangle_1);
    pHBoxLayout_1->addStretch();
    pHBoxLayout_1->addLayout(pVBoxLayout_1);
    pHBoxLayout_1->addStretch();

    pVBoxLayout->addLayout(pHBoxLayout_1);

    QLabel *pLabel_1 = new QLabel;
    QPixmap pix_1(":/images/example/DArrowRectangle.png");
    pLabel_1->setFixedSize(570, 330);
    pLabel_1->setPixmap(pix_1);
    pLabel_1->setScaledContents(true);

    QHBoxLayout *pHBoxLayout_pic_1 = new QHBoxLayout;
    pHBoxLayout_pic_1->setMargin(0);
    pHBoxLayout_pic_1->setSpacing(0);
    pHBoxLayout_pic_1->addWidget(pLabel_1);

    pVBoxLayout->addSpacing(30);
    pVBoxLayout->addLayout(pHBoxLayout_pic_1);
    pVBoxLayout->addSpacing(20);
}

QString DArrowRectangleExample::getTitleName() const
{
    return "DArrowRectangle";
}

QString DArrowRectangleExample::getDescriptionInfo() const
{
    return "带尖角的popup窗口,内容部分不是固\n"
           "定的,可以做多种定制.";
}

int DArrowRectangleExample::getFixedHeight() const
{
    return 818;
}
