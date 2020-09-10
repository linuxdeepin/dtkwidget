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
#include <DToolButton>
#include <DToolBar>
#include <DIconButton>
#include <DButtonBox>
#include <DFloatingButton>
#include <DSwitchButton>
#include <DRadioButton>
#include <DCheckBox>
#include <DComboBox>
#include <DFontComboBox>
#include <DListView>
#include <DSearchComboBox>

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
    addExampleWindow(new DRadioButtonExample(this));
    addExampleWindow(new DCheckButtonExample(this));
    addExampleWindow(new DComboBoxExample(this));
    addExampleWindow(new DFontComboBoxExample(this));
    addExampleWindow(new DSearchComboBoxExample(this));
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
    return 300;
}

DToolButtonExample::DToolButtonExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *pVBoxLayout = new QVBoxLayout;
    pVBoxLayout->setMargin(0);
    pVBoxLayout->setSpacing(0);
    setLayout(pVBoxLayout);

    QHBoxLayout *pHBoxLayout = new QHBoxLayout;
    pHBoxLayout->setMargin(20);
    pHBoxLayout->setSpacing(0);

    auto showDialog = [](const QString &info) {
        DDialog dialog("", info, nullptr);
        dialog.setIcon(DStyle().standardIcon(DStyle::SP_MessageBoxInformation));
        dialog.addButton("确定");
        dialog.exec();
    };

    DToolBar *pToolBar = new DToolBar;
    pToolBar->addAction(QIcon::fromTheme("icon_button"), "", this, [showDialog] {
        showDialog("这是icon_button的图标");
    });

    pToolBar->addAction(QIcon::fromTheme("icon_edit"), "", this, [showDialog] {
        showDialog("这是icon_edit的图标");
    });

    pToolBar->addAction(QIcon::fromTheme("icon_slider"), "", this, [showDialog] {
        showDialog("这是icon_slider的图标");
    });

    pToolBar->addAction(QIcon::fromTheme("icon_ListView"), "", this, [showDialog] {
        showDialog("这是icon_ListView的图标");
    });

    pToolBar->addAction(QIcon::fromTheme("icon_Window"), "", this, [showDialog] {
        showDialog("这是icon_Window的图标");
    });

    pToolBar->addAction(QIcon::fromTheme("icon_Tooltip"), "", this, [showDialog] {
        showDialog("这是icon_Tooltip的图标");
    });

    pHBoxLayout->addWidget(pToolBar);

    pVBoxLayout->addSpacing(20);
    pVBoxLayout->addLayout(pHBoxLayout);
    pHBoxLayout->addSpacing(30);

    QLabel *pLabel = new QLabel;
    QPixmap pix(":/images/example/DToolButton.png");
    pLabel->setFixedSize(570, 348);
    pLabel->setPixmap(pix);
    pLabel->setScaledContents(true);

    QHBoxLayout *pHBoxLayout_pic = new QHBoxLayout;
    pHBoxLayout_pic->setMargin(0);
    pHBoxLayout_pic->setSpacing(0);
    pHBoxLayout_pic->addWidget(pLabel);

    pVBoxLayout->addLayout(pHBoxLayout_pic);
}

QString DToolButtonExample::getTitleName() const
{
    return "DToolButton";
}

QString DToolButtonExample::getDescriptionInfo() const
{
    return "主要用在工具栏上作为应用的功能按\n"
           "钮,比如画板左侧的工具栏图标.";
}

int DToolButtonExample::getFixedHeight() const
{
    return 600;
}

DIconButtonExample::DIconButtonExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *pVBoxLayout = new QVBoxLayout;
    pVBoxLayout->setMargin(0);
    pVBoxLayout->setSpacing(0);
    setLayout(pVBoxLayout);

    QHBoxLayout *pHBoxLayout = new QHBoxLayout;
    pHBoxLayout->setMargin(10);
    pHBoxLayout->setSpacing(0);

    DIconButton *pButton_1 = new DIconButton(DStyle::SP_IncreaseElement, this);
    pButton_1->setFixedSize(36, 36);

    DIconButton *pButton_2 = new DIconButton(DStyle::SP_ArrowEnter, this);
    pButton_2->setFixedSize(36, 36);

    DIconButton *pButton_3 = new DIconButton(DStyle::SP_IncreaseElement, this);
    pButton_3->setEnabled(false);
    pButton_3->setFixedSize(36, 36);

    DIconButton *pButton_4 = new DIconButton(DStyle::SP_ArrowEnter, this);
    pButton_4->setEnabled(false);
    pButton_4->setFixedSize(36, 36);

    DIconButton *pButton_5 = new DIconButton(DStyle::SP_DeleteButton, this);
    pButton_5->setFlat(true);
    pButton_5->setFixedSize(QSize(24, 24));
    pButton_5->setIconSize(QSize(16, 16));
    DStyle::setFocusRectVisible(pButton_5, false);

    DIconButton *pButton_6 = new DIconButton(DStyle::SP_AddButton, this);
    pButton_6->setFlat(true);
    pButton_6->setFixedSize(QSize(24, 24));
    pButton_6->setIconSize(QSize(16, 16));
    DStyle::setFocusRectVisible(pButton_6, false);

    DIconButton *pButton_7 = new DIconButton(DStyle::SP_DeleteButton, this);
    pButton_7->setEnabled(false);
    pButton_7->setFlat(true);
    pButton_7->setFixedSize(QSize(24, 24));
    pButton_7->setIconSize(QSize(16, 16));
    DStyle::setFocusRectVisible(pButton_7, false);

    DIconButton *pButton_8 = new DIconButton(DStyle::SP_AddButton, this);
    pButton_8->setEnabled(false);
    pButton_8->setFlat(true);
    pButton_8->setFixedSize(QSize(24, 24));
    pButton_8->setIconSize(QSize(16, 16));
    DStyle::setFocusRectVisible(pButton_8, false);

    pHBoxLayout->addStretch();
    pHBoxLayout->addWidget(pButton_1);
    pHBoxLayout->addSpacing(10);
    pHBoxLayout->addWidget(pButton_2);
    pHBoxLayout->addSpacing(50);
    pHBoxLayout->addWidget(pButton_3);
    pHBoxLayout->addSpacing(10);
    pHBoxLayout->addWidget(pButton_4);

    pHBoxLayout->addSpacing(100);
    pHBoxLayout->addWidget(pButton_5);
    pHBoxLayout->addSpacing(10);
    pHBoxLayout->addWidget(pButton_6);
    pHBoxLayout->addSpacing(50);
    pHBoxLayout->addWidget(pButton_7);
    pHBoxLayout->addSpacing(10);
    pHBoxLayout->addWidget(pButton_8);
    pHBoxLayout->addStretch();

    pVBoxLayout->addSpacing(20);
    pVBoxLayout->addLayout(pHBoxLayout);
    pHBoxLayout->addSpacing(20);

    QLabel *pLabel = new QLabel;
    QPixmap pix(":/images/example/DIconButton.png");
    pLabel->setFixedSize(568, 444);
    pLabel->setPixmap(pix);
    pLabel->setScaledContents(true);

    QHBoxLayout *pHBoxLayout_pic = new QHBoxLayout;
    pHBoxLayout_pic->setMargin(0);
    pHBoxLayout_pic->setSpacing(0);
    pHBoxLayout_pic->addWidget(pLabel);

    pVBoxLayout->addLayout(pHBoxLayout_pic);
}

QString DIconButtonExample::getTitleName() const
{
    return "DIconButton";
}

QString DIconButtonExample::getDescriptionInfo() const
{
    return "性质和DPushButton一致,只不过是用\n"
           "图形化代替文字,最常见的有新建,上\n"
           "一个下一个等.";
}

int DIconButtonExample::getFixedHeight() const
{
    return 600;
}

DButtonBoxExample::DButtonBoxExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *pVBoxLayout = new QVBoxLayout;
    pVBoxLayout->setMargin(0);
    pVBoxLayout->setSpacing(0);
    setLayout(pVBoxLayout);

    QHBoxLayout *pHBoxLayout = new QHBoxLayout;
    pHBoxLayout->setMargin(10);
    pHBoxLayout->setSpacing(0);

    DButtonBox *pButtonBox_1 = new DButtonBox;

    DButtonBoxButton *pButton_1 = new DButtonBoxButton(DStyle().standardIcon(DStyle::SP_ArrowLeave));
    pButton_1->setFixedSize(36, 36);

    DButtonBoxButton *pButton_2 = new DButtonBoxButton(DStyle().standardIcon(DStyle::SP_ArrowEnter));
    pButton_2->setFixedSize(36, 36);

    pButtonBox_1->setButtonList(QList<DButtonBoxButton *>() << pButton_1 << pButton_2, true);

    DButtonBox *pButtonBox_2 = new DButtonBox;
    DButtonBoxButton *pButton_3 = new DButtonBoxButton(DStyle().standardIcon(DStyle::SP_ArrowLeave));
    pButton_3->setFixedSize(36, 36);
    pButton_3->setEnabled(false);

    DButtonBoxButton *pButton_4 = new DButtonBoxButton(DStyle().standardIcon(DStyle::SP_ArrowEnter));
    pButton_4->setFixedSize(36, 36);
    pButton_4->setEnabled(false);

    pButtonBox_2->setButtonList(QList<DButtonBoxButton *>() << pButton_3 << pButton_4, true);

    DButtonBox *pButtonBox_3 = new DButtonBox;
    DButtonBoxButton *pButton_5 = new DButtonBoxButton(DStyle().standardIcon(DStyle::SP_LockElement), "锁定设置");
    pButton_3->setFixedSize(36, 36);
    pButton_3->setEnabled(false);

    DButtonBoxButton *pButton_6 = new DButtonBoxButton(DStyle().standardIcon(DStyle::SP_UnlockElement), "解锁设置");
    pButton_4->setFixedSize(36, 36);
    pButton_4->setEnabled(false);

    pButtonBox_3->setButtonList(QList<DButtonBoxButton *>() << pButton_5 << pButton_6, true);

    pHBoxLayout->addStretch();
    pHBoxLayout->addWidget(pButtonBox_1);
    pHBoxLayout->addSpacing(10);
    pHBoxLayout->addWidget(pButtonBox_2);
    pHBoxLayout->addSpacing(50);
    pHBoxLayout->addWidget(pButtonBox_3);
    pHBoxLayout->addStretch();

    pVBoxLayout->addSpacing(20);
    pVBoxLayout->addLayout(pHBoxLayout);
    pHBoxLayout->addSpacing(20);

    QLabel *pLabel = new QLabel;
    QPixmap pix(":/images/example/DButtonBox.png");
    pLabel->setFixedSize(570, 426);
    pLabel->setPixmap(pix);
    pLabel->setScaledContents(true);

    QHBoxLayout *pHBoxLayout_pic = new QHBoxLayout;
    pHBoxLayout_pic->setMargin(0);
    pHBoxLayout_pic->setSpacing(0);
    pHBoxLayout_pic->addWidget(pLabel);

    pVBoxLayout->addLayout(pHBoxLayout_pic);
}

QString DButtonBoxExample::getTitleName() const
{
    return "DButtonBox";
}

QString DButtonBoxExample::getDescriptionInfo() const
{
    return "群组按钮,几个连体按钮选项是为了一\n"
           "个共同的功能服务,比如日历的切换视\n"
           "图,还有常见的文字选项如加粗下划线\n"
           "中横线等.";
}

int DButtonBoxExample::getFixedHeight() const
{
    return 600;
}

DFloatingButtonExample::DFloatingButtonExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *pVBoxLayout = new QVBoxLayout;
    pVBoxLayout->setMargin(0);
    pVBoxLayout->setSpacing(0);
    setLayout(pVBoxLayout);

    QHBoxLayout *pHBoxLayout = new QHBoxLayout;
    pHBoxLayout->setMargin(10);
    pHBoxLayout->setSpacing(0);

    DFloatingButton *pFloatingButton_1 = new DFloatingButton(DStyle::SP_IncreaseElement, this);
    pFloatingButton_1->setFixedSize(48, 48);

    DFloatingButton *pFloatingButton_2 = new DFloatingButton(DStyle::SP_IncreaseElement, this);
    pFloatingButton_2->setFixedSize(48, 48);
    pFloatingButton_2->setEnabled(false);

    pHBoxLayout->addStretch();
    pHBoxLayout->addWidget(pFloatingButton_1);
    pHBoxLayout->addSpacing(50);
    pHBoxLayout->addWidget(pFloatingButton_2);
    pHBoxLayout->addStretch();

    pVBoxLayout->addSpacing(20);
    pVBoxLayout->addLayout(pHBoxLayout);

    QLabel *pLabel = new QLabel;
    QPixmap pix(":/images/example/DFloatingButton.png");
    pLabel->setFixedSize(568, 444);
    pLabel->setPixmap(pix);
    pLabel->setScaledContents(true);

    QHBoxLayout *pHBoxLayout_pic = new QHBoxLayout;
    pHBoxLayout_pic->setMargin(0);
    pHBoxLayout_pic->setSpacing(0);
    pHBoxLayout_pic->addWidget(pLabel);

    pVBoxLayout->addSpacing(30);
    pVBoxLayout->addLayout(pHBoxLayout_pic);
    pVBoxLayout->addSpacing(20);
}

QString DFloatingButtonExample::getTitleName() const
{
    return "DFloatingButton";
}

QString DFloatingButtonExample::getDescriptionInfo() const
{
    return "在一个场景中作为一个主要功能使用,\n"
           "比如控制中心账户列表中作为添加账户\n"
           "使用.这个按钮是悬浮的,不占据底下\n"
           "内容的空间.";
}

int DFloatingButtonExample::getFixedHeight() const
{
    return 600;
}

DSwitchButtonExample::DSwitchButtonExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *pVBoxLayout = new QVBoxLayout;
    pVBoxLayout->setMargin(0);
    pVBoxLayout->setSpacing(0);
    setLayout(pVBoxLayout);

    QHBoxLayout *pHBoxLayout = new QHBoxLayout;
    pHBoxLayout->setMargin(0);
    pHBoxLayout->setSpacing(0);

    DSwitchButton *pSwitchButton_1 = new DSwitchButton;

    DSwitchButton *pSwitchButton_2 = new DSwitchButton;
    pSwitchButton_2->setEnabled(false);

    pHBoxLayout->addStretch();
    pHBoxLayout->addWidget(pSwitchButton_1);
    pHBoxLayout->addSpacing(30);
    pHBoxLayout->addWidget(pSwitchButton_2);
    pHBoxLayout->addStretch();

    pVBoxLayout->addSpacing(30);
    pVBoxLayout->addLayout(pHBoxLayout);

    QLabel *pLabel = new QLabel;
    QPixmap pix(":/images/example/DSwitchButton.png");
    pLabel->setFixedSize(568, 444);
    pLabel->setPixmap(pix);
    pLabel->setScaledContents(true);

    QHBoxLayout *pHBoxLayout_pic = new QHBoxLayout;
    pHBoxLayout_pic->setMargin(0);
    pHBoxLayout_pic->setSpacing(0);
    pHBoxLayout_pic->addWidget(pLabel);

    pVBoxLayout->addSpacing(30);
    pVBoxLayout->addLayout(pHBoxLayout_pic);
    pVBoxLayout->addSpacing(20);
}

QString DSwitchButtonExample::getTitleName() const
{
    return "DSwitchButton";
}

QString DSwitchButtonExample::getDescriptionInfo() const
{
    return "普通的开关控件,等价控件为\n"
           "DCheckButton";
}

int DSwitchButtonExample::getFixedHeight() const
{
    return 600;
}

DRadioButtonExample::DRadioButtonExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *pVBoxLayout = new QVBoxLayout;
    pVBoxLayout->setMargin(0);
    pVBoxLayout->setSpacing(0);
    setLayout(pVBoxLayout);

    QHBoxLayout *pHBoxLayout = new QHBoxLayout;
    pHBoxLayout->setMargin(0);
    pHBoxLayout->setSpacing(0);

    DRadioButton *pRadioButton_1 = new DRadioButton;
    //也可以不设置,默认是设置为互斥的
    pRadioButton_1->setAutoExclusive(true);

    DRadioButton *pRadioButton_2 = new DRadioButton;
    pRadioButton_2->setAutoExclusive(true);

    pHBoxLayout->addStretch();
    pHBoxLayout->addWidget(pRadioButton_1);
    pHBoxLayout->addSpacing(30);
    pHBoxLayout->addWidget(pRadioButton_2);
    pHBoxLayout->addStretch();

    pVBoxLayout->addSpacing(30);
    pVBoxLayout->addLayout(pHBoxLayout);

    QLabel *pLabel = new QLabel;
    QPixmap pix(":/images/example/DRadioButton.png");
    pLabel->setFixedSize(568, 444);
    pLabel->setPixmap(pix);
    pLabel->setScaledContents(true);

    QHBoxLayout *pHBoxLayout_pic = new QHBoxLayout;
    pHBoxLayout_pic->setMargin(0);
    pHBoxLayout_pic->setSpacing(0);
    pHBoxLayout_pic->addWidget(pLabel);

    pVBoxLayout->addSpacing(30);
    pVBoxLayout->addLayout(pHBoxLayout_pic);
    pVBoxLayout->addSpacing(20);
}

QString DRadioButtonExample::getTitleName() const
{
    return "DRadioButton";
}

QString DRadioButtonExample::getDescriptionInfo() const
{
    return "常见于设置或者对话框中作为一个选项\n"
           "存在,至少提供2个或者多个选项,选\n"
           "项之间是互斥的.";
}

int DRadioButtonExample::getFixedHeight() const
{
    return 600;
}

DCheckButtonExample::DCheckButtonExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *pVBoxLayout = new QVBoxLayout;
    pVBoxLayout->setMargin(0);
    pVBoxLayout->setSpacing(0);
    setLayout(pVBoxLayout);

    QHBoxLayout *pHBoxLayout = new QHBoxLayout;
    pHBoxLayout->setMargin(0);
    pHBoxLayout->setSpacing(0);

    DCheckBox *pCheckBox_1 = new DCheckBox("");
    pCheckBox_1->setTristate(true);
    pCheckBox_1->setCheckState(Qt::PartiallyChecked);

    DCheckBox *pCheckBox_2 = new DCheckBox("");

    DCheckBox *pCheckBox_3 = new DCheckBox("");
    pCheckBox_3->setTristate(true);
    pCheckBox_3->setCheckState(Qt::Unchecked);
    pCheckBox_3->setEnabled(false);

    DCheckBox *pCheckBox_4 = new DCheckBox("");
    pCheckBox_4->setTristate(true);
    pCheckBox_4->setCheckState(Qt::Checked);
    pCheckBox_4->setEnabled(false);

    DCheckBox *pCheckBox_5 = new DCheckBox("");
    pCheckBox_5->setTristate(true);
    pCheckBox_5->setCheckState(Qt::PartiallyChecked);
    pCheckBox_5->setEnabled(false);

    pHBoxLayout->addStretch();
    pHBoxLayout->addWidget(pCheckBox_1);
    pHBoxLayout->addSpacing(10);
    pHBoxLayout->addWidget(pCheckBox_2);
    pHBoxLayout->addSpacing(50);
    pHBoxLayout->addWidget(pCheckBox_3);
    pHBoxLayout->addSpacing(10);
    pHBoxLayout->addWidget(pCheckBox_4);
    pHBoxLayout->addSpacing(10);
    pHBoxLayout->addWidget(pCheckBox_5);
    pHBoxLayout->addStretch();

    pVBoxLayout->addSpacing(30);
    pVBoxLayout->addLayout(pHBoxLayout);

    QLabel *pLabel = new QLabel;
    QPixmap pix(":/images/example/DCheckButton.png");
    pLabel->setFixedSize(568, 444);
    pLabel->setPixmap(pix);
    pLabel->setScaledContents(true);

    QHBoxLayout *pHBoxLayout_pic = new QHBoxLayout;
    pHBoxLayout_pic->setMargin(0);
    pHBoxLayout_pic->setSpacing(0);
    pHBoxLayout_pic->addWidget(pLabel);

    pVBoxLayout->addSpacing(30);
    pVBoxLayout->addLayout(pHBoxLayout_pic);
    pVBoxLayout->addSpacing(20);
}

QString DCheckButtonExample::getTitleName() const
{
    return "DCheckButton";
}

QString DCheckButtonExample::getDescriptionInfo() const
{
    return "和DRadioButton一致,也常用于设\n"
           "置和对话框的选项,但每个选项之间是\n"
           "独立的,不会产生冲突.每个选项的等\n"
           "价控件是DSwitchButton.";
}

int DCheckButtonExample::getFixedHeight() const
{
    return 600;
}

DComboBoxExample::DComboBoxExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *pVBoxLayout = new QVBoxLayout;
    pVBoxLayout->setMargin(0);
    pVBoxLayout->setSpacing(0);
    setLayout(pVBoxLayout);

    QHBoxLayout *pHBoxLayout_1 = new QHBoxLayout;
    pHBoxLayout_1->setMargin(0);
    pHBoxLayout_1->setSpacing(0);

    DComboBox *pComboBox_1 = new DComboBox;
    pComboBox_1->setFixedSize(240, 36);
    pComboBox_1->addItem("ComboBox button 1");
    pComboBox_1->addItem("ComboBox button 2");
    pComboBox_1->addItem("ComboBox button 3");
    pHBoxLayout_1->addWidget(pComboBox_1);

    QHBoxLayout *pHBoxLayout_2 = new QHBoxLayout;
    pHBoxLayout_2->setMargin(0);
    pHBoxLayout_2->setSpacing(0);
    DComboBox *pComboBox_2 = new DComboBox;
    pComboBox_2->setFixedSize(340, 36);
    pComboBox_2->addItem("/space");
    pHBoxLayout_2->addWidget(pComboBox_2);

    pVBoxLayout->addSpacing(30);
    pVBoxLayout->addLayout(pHBoxLayout_1);
    pVBoxLayout->addSpacing(30);
    pVBoxLayout->addLayout(pHBoxLayout_2);

    QLabel *pLabel_1 = new QLabel;
    QPixmap pix_1(":/images/example/DComboBox_1.png");
    pLabel_1->setFixedSize(568, 444);
    pLabel_1->setPixmap(pix_1);
    pLabel_1->setScaledContents(true);

    QLabel *pLabel_2 = new QLabel;
    QPixmap pix_2(":/images/example/DComboBox_2.png");
    pLabel_2->setFixedSize(568, 444);
    pLabel_2->setPixmap(pix_2);
    pLabel_2->setScaledContents(true);

    QHBoxLayout *pHBoxLayout_pic_1 = new QHBoxLayout;
    pHBoxLayout_pic_1->setMargin(0);
    pHBoxLayout_pic_1->setSpacing(0);
    pHBoxLayout_pic_1->addWidget(pLabel_1);

    QHBoxLayout *pHBoxLayout_pic_2 = new QHBoxLayout;
    pHBoxLayout_pic_2->setMargin(0);
    pHBoxLayout_pic_2->setSpacing(0);
    pHBoxLayout_pic_2->addWidget(pLabel_2);

    pVBoxLayout->addSpacing(30);
    pVBoxLayout->addLayout(pHBoxLayout_pic_1);
    pVBoxLayout->addSpacing(20);
    pVBoxLayout->addLayout(pHBoxLayout_pic_2);
    pVBoxLayout->addSpacing(20);
}

QString DComboBoxExample::getTitleName() const
{
    return "DComboBox";
}

QString DComboBoxExample::getDescriptionInfo() const
{
    return "作为一个选项存在,多数时候前面有标\n"
           "题,但也有没有标题的情况.点击是出\n"
           "现一个菜单,点击选项某一项菜单后菜\n"
           "单收起,选项框上的文字显示菜单里激\n"
           "活的那一项.";
}

int DComboBoxExample::getFixedHeight() const
{
    return 1200;
}

DFontComboBoxExample::DFontComboBoxExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *pVBoxLayout = new QVBoxLayout;
    pVBoxLayout->setMargin(0);
    pVBoxLayout->setSpacing(0);
    setLayout(pVBoxLayout);

    QHBoxLayout *pHBoxLayout_1 = new QHBoxLayout;
    pHBoxLayout_1->setMargin(0);
    pHBoxLayout_1->setSpacing(0);

    DFontComboBox *pComboBox_1 = new DFontComboBox;
    pComboBox_1->setFixedSize(240, 36);
    pHBoxLayout_1->addWidget(pComboBox_1);

    pVBoxLayout->addSpacing(30);
    pVBoxLayout->addLayout(pHBoxLayout_1);
    pVBoxLayout->addSpacing(30);

    QLabel *pLabel_1 = new QLabel;
    QPixmap pix_1(":/images/example/DFontComboBox.png");
    pLabel_1->setFixedSize(568, 444);
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

QString DFontComboBoxExample::getTitleName() const
{
    return "DFontComboBox";
}

QString DFontComboBoxExample::getDescriptionInfo() const
{
    return "和DComboBox其实是一个控件,但这\n"
           "里仅用于自体的选择.";
}

int DFontComboBoxExample::getFixedHeight() const
{
    return 700;
}

DSearchComboBoxExample::DSearchComboBoxExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *pVBoxLayout = new QVBoxLayout;
    pVBoxLayout->setMargin(0);
    pVBoxLayout->setSpacing(0);
    setLayout(pVBoxLayout);

    QHBoxLayout *pHBoxLayout_1 = new QHBoxLayout;
    pHBoxLayout_1->setMargin(0);
    pHBoxLayout_1->setSpacing(0);

    DSearchComboBox *pComboBox_1 = new DSearchComboBox;
    pComboBox_1->setEditable(true);
    pComboBox_1->addItem("手动选择驱动方案");
    pComboBox_1->addItem("手动选择驱动方案");
    pComboBox_1->addItem("手动选择驱动方案");
    pComboBox_1->setFixedSize(240, 36);
    pHBoxLayout_1->addWidget(pComboBox_1);

    pVBoxLayout->addSpacing(30);
    pVBoxLayout->addLayout(pHBoxLayout_1);
    pVBoxLayout->addSpacing(30);

    QLabel *pLabel_1 = new QLabel;
    QPixmap pix_1(":/images/example/DSearchComboBox.png");
    pLabel_1->setFixedSize(568, 444);
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

QString DSearchComboBoxExample::getTitleName() const
{
    return "DSearchComboBox";
}

QString DSearchComboBoxExample::getDescriptionInfo() const
{
    return "一个带搜索功能的ComboBox控件.";
}

int DSearchComboBoxExample::getFixedHeight() const
{
    return 700;
}
