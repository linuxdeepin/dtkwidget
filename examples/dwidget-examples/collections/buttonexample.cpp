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
#include <QMenu>
#include <QScrollArea>
#include <QDebug>

#include <DDialog>
#include <DStyle>
#include <DPushButton>
#include <DMessageBox>
#include <DWarningButton>
#include <DSuggestButton>
#include <DFrame>

#include "buttonexample.h"

DWIDGET_USE_NAMESPACE

ButtonExampleWindow::ButtonExampleWindow(QWidget *parent)
    : PageWindowInterface(parent)
{
    addExampleWindow(new DPushButtonExample(this));
    addExampleWindow(new DWarningButtonExample(this));
    addExampleWindow(new DSuggestButtonExample(this));
    addExampleWindow(new DToolButtonExample(this));
    addExampleWindow(new DIconButtonExample(this));
    addExampleWindow(new DButtonBoxExample(this));
    addExampleWindow(new DFloatingButtonExample(this));
    addExampleWindow(new DSwitchButtonExample(this));
    addExampleWindow(new DCheckButtonExample(this));
}

DPushButtonExample::DPushButtonExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QHBoxLayout *pHBoxLayout_1 = new QHBoxLayout;
    QHBoxLayout *pHBoxLayout_2 = new QHBoxLayout;
    QVBoxLayout *pVBoxLayout = new QVBoxLayout;

    setLayout(pVBoxLayout);
    pVBoxLayout->addStretch();
    pVBoxLayout->addLayout(pHBoxLayout_1);
    pVBoxLayout->addSpacing(20);
    pVBoxLayout->addLayout(pHBoxLayout_2);
    pVBoxLayout->addStretch();

    DPushButton *pButtonNormal = new DPushButton("button normal", this);
    pButtonNormal->setFixedSize(200, 36);

    DPushButton *pButtonDisabled = new DPushButton("button disabled", this);
    pButtonDisabled->setFixedSize(200, 36);
    pButtonDisabled->setEnabled(false);

    pHBoxLayout_1->addStretch();
    pHBoxLayout_1->addWidget(pButtonNormal);
    pHBoxLayout_1->addSpacing(20);
    pHBoxLayout_1->addWidget(pButtonDisabled);
    pHBoxLayout_1->addStretch();

    DPushButton *pPushButton = new DPushButton("push button", this);
    pPushButton->setFixedSize(200, 36);
    pPushButton->setCheckable(true);

    QMenu *pMenu = new QMenu(this);
    pMenu->addAction("item_1");
    pMenu->addAction("item_2");
    pMenu->addAction("item_3");

    pPushButton->setMenu(pMenu);

    DPushButton *pPushButtonDisabled = new DPushButton("push button", this);
    pPushButtonDisabled->setFixedSize(200, 36);
    pPushButtonDisabled->setEnabled(false);
    pPushButtonDisabled->setMenu(pMenu);

    pHBoxLayout_2->addStretch();
    pHBoxLayout_2->addWidget(pPushButton);
    pHBoxLayout_2->addSpacing(20);
    pHBoxLayout_2->addWidget(pPushButtonDisabled);
    pHBoxLayout_2->addStretch();

    connect(pButtonNormal, &DPushButton::clicked, this, [] {
        DDialog dialog("", "名称push button已经被占用,请使用其他名称", nullptr);
        dialog.setIcon(DStyle().standardIcon(DStyle::SP_MessageBoxWarning));
        dialog.addButton("确定");
        dialog.exec();
    });
}

QString DPushButtonExample::getTitleName() const
{
    return "DPushButton";
}

QString DPushButtonExample::getDescriptionInfo() const
{
    return "普通的文字按钮和带菜单的文字按钮\n"
           "按钮的文字随着菜单的选择而改变";
}

int DPushButtonExample::getFixedHeight() const
{
    return 402;
}

DWarningButtonExample::DWarningButtonExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QHBoxLayout *pHBoxLayout_1 = new QHBoxLayout;
    QVBoxLayout *pVBoxLayout = new QVBoxLayout;

    setLayout(pVBoxLayout);
    pVBoxLayout->addSpacing(40);
    pVBoxLayout->addLayout(pHBoxLayout_1);
    pVBoxLayout->addStretch();

    DWarningButton *pWarningButton = new DWarningButton(this);
    pWarningButton->setText("warning");
    pWarningButton->setFixedSize(200, 36);

    DWarningButton *pWarningButtonDisabled = new DWarningButton(this);
    pWarningButtonDisabled->setText("warning disabled");
    pWarningButtonDisabled->setEnabled(false);
    pWarningButtonDisabled->setFixedSize(200, 36);

    pHBoxLayout_1->addStretch();
    pHBoxLayout_1->addWidget(pWarningButton);
    pHBoxLayout_1->addSpacing(20);
    pHBoxLayout_1->addWidget(pWarningButtonDisabled);
    pHBoxLayout_1->addStretch();

    connect(pWarningButton, &DWarningButton::clicked, this, [] {
        DDialog dialog("", "格式化操作会清空该磁盘数据,您需要继续吗?\n此操作不可以恢复", nullptr);
        dialog.setIcon(DStyle().standardIcon(DStyle::SP_DriveNetIcon));
        dialog.addButton("取消");
        dialog.addButton("格式化");
        dialog.exec();
    });
}

QString DWarningButtonExample::getTitleName() const
{
    return "DWarningButton";
}

QString DWarningButtonExample::getDescriptionInfo() const
{
    return "功能和普通的文字按钮一致,不过用在\n"
           "警告语上,告诉用户有危险操作.";
}

int DWarningButtonExample::getFixedHeight() const
{
    return 368;
}

DSuggestButtonExample::DSuggestButtonExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QHBoxLayout *pHBoxLayout_1 = new QHBoxLayout;
    QHBoxLayout *pHBoxLayout_2 = new QHBoxLayout;
    QVBoxLayout *pVBoxLayout = new QVBoxLayout;

    setLayout(pVBoxLayout);
    pVBoxLayout->addStretch();
    pVBoxLayout->addLayout(pHBoxLayout_1);
    pVBoxLayout->addSpacing(20);
    pVBoxLayout->addLayout(pHBoxLayout_2);
    pVBoxLayout->addStretch();

    DSuggestButton *pRecommend = new DSuggestButton("recommend", this);
    pRecommend->setFixedSize(200, 36);

    DPushButton *pRecommendDisabled = new DPushButton("recommend disabled", this);
    pRecommendDisabled->setFixedSize(200, 36);
    pRecommendDisabled->setEnabled(false);

    pHBoxLayout_1->addStretch();
    pHBoxLayout_1->addWidget(pRecommend);
    pHBoxLayout_1->addSpacing(20);
    pHBoxLayout_1->addWidget(pRecommendDisabled);
    pHBoxLayout_1->addStretch();

    DSuggestButton *pHighlight = new DSuggestButton("highlight", this);
    pHighlight->setFixedSize(200, 36);
    pHighlight->setCheckable(true);
    pHighlight->setFocus();

    DPushButton *pHighlightDisabled = new DPushButton("highlight disabled", this);
    pHighlightDisabled->setFixedSize(200, 36);
    pHighlightDisabled->setEnabled(false);

    pHBoxLayout_2->addStretch();
    pHBoxLayout_2->addWidget(pHighlight);
    pHBoxLayout_2->addSpacing(20);
    pHBoxLayout_2->addWidget(pHighlightDisabled);
    pHBoxLayout_2->addStretch();

    connect(pRecommend, &DPushButton::clicked, this, [] {
        DDialog dialog("", "这里现实简要出错信息XXXXXXXX", nullptr);
        dialog.setIcon(DStyle().standardIcon(DStyle::SP_MessageBoxWarning));
        dialog.addButton("显示详情");
        dialog.addButton("确定");
        dialog.exec();
    });
}

QString DSuggestButtonExample::getTitleName() const
{
    return "DSuggestButton";
}

QString DSuggestButtonExample::getDescriptionInfo() const
{
    return "往往不会单独出现,绝大多数情况都是\n"
           "和DPushButton一起出现,显示在\n"
           "DPushButton的右边,主要是起引导用\n"
           "户点击的作用.";
}

int DSuggestButtonExample::getFixedHeight() const
{
    return 400;
}

DToolButtonExample::DToolButtonExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DToolButtonExample::getTitleName() const
{
    return "DToolButton";
}

QString DToolButtonExample::getDescriptionInfo() const
{
    return QString();
}

int DToolButtonExample::getFixedHeight() const
{
    return 300;
}

DIconButtonExample::DIconButtonExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DIconButtonExample::getTitleName() const
{
    return "DIconButton";
}

QString DIconButtonExample::getDescriptionInfo() const
{
    return QString();
}

int DIconButtonExample::getFixedHeight() const
{
    return 300;
}

DButtonBoxExample::DButtonBoxExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DButtonBoxExample::getTitleName() const
{
    return "DButtonBox";
}

QString DButtonBoxExample::getDescriptionInfo() const
{
    return QString();
}

int DButtonBoxExample::getFixedHeight() const
{
    return 300;
}

DFloatingButtonExample::DFloatingButtonExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DFloatingButtonExample::getTitleName() const
{
    return "DFloatingButton";
}

QString DFloatingButtonExample::getDescriptionInfo() const
{
    return QString();
}

int DFloatingButtonExample::getFixedHeight() const
{
    return 300;
}

DSwitchButtonExample::DSwitchButtonExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DSwitchButtonExample::getTitleName() const
{
    return "DSwitchButton";
}

QString DSwitchButtonExample::getDescriptionInfo() const
{
    return QString();
}

int DSwitchButtonExample::getFixedHeight() const
{
    return 300;
}

DCheckButtonExample::DCheckButtonExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DCheckButtonExample::getTitleName() const
{
    return "DCheckButton";
}

QString DCheckButtonExample::getDescriptionInfo() const
{
    return QString();
}

int DCheckButtonExample::getFixedHeight() const
{
    return 300;
}

DComboBoxExample::DComboBoxExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
}

QString DComboBoxExample::getTitleName() const
{
    return "DComboBox";
}

QString DComboBoxExample::getDescriptionInfo() const
{
    return QString();
}

int DComboBoxExample::getFixedHeight() const
{
    return 300;
}
