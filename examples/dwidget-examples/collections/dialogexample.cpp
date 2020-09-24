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

#include <DLabel>
#include <DDialog>
#include <DPushButton>
#include <DPasswordEdit>
#include <DMessageManager>
#include <DFileChooserEdit>
#include <DFloatingMessage>

#include "dialogexample.h"

DWIDGET_USE_NAMESPACE

DialogExampleWindow::DialogExampleWindow(QWidget *parent)
    : PageWindowInterface(parent)
{
    addExampleWindow(new DDialogExample(this));
    addExampleWindow(new DFileDialogExample(this));
    addExampleWindow(new DMessageManagerExample(this));
}

DDialogExample::DDialogExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QLabel *label1 = new QLabel;
    QLabel *label2 = new QLabel;

    label1->setFixedSize(381, 181);
    label1->setScaledContents(true);
    label2->setFixedSize(381, 160);
    label2->setScaledContents(true);
    label1->setPixmap(QPixmap(":/images/example/DDialog_1.png"));
    label2->setPixmap(QPixmap(":/images/example/DDialog_2.png"));

    DLabel *label = new DLabel;
    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    DPushButton *btn = new DPushButton("开始还原");
    mainlayout->setMargin(0);
    mainlayout->setSpacing(0);

    label->setPixmap(QPixmap(":/images/example/DDialog.png"));
    label->setFixedSize(550, 426);
    label->setScaledContents(true);

    mainlayout->addWidget(btn, 0, Qt::AlignCenter);
    mainlayout->addWidget(label1, 0, Qt::AlignCenter);
    mainlayout->addWidget(label2, 0, Qt::AlignCenter);
    mainlayout->addSpacing(60);
    mainlayout->addWidget(label, 0, Qt::AlignCenter);
    mainlayout->addSpacing(70);

    connect(btn, &DPushButton::clicked, this, [ = ] {
        DDialog dialog;
        dialog.setIcon(style()->standardIcon(QStyle::SP_MessageBoxWarning));
        dialog.setTitle("还原当前系统需要管理员权限");
        dialog.addContent(new DPasswordEdit);
        dialog.addButton("取消");
        dialog.addButton("授权", false, DDialog::ButtonRecommend);
        dialog.exec();
    });

}

QString DDialogExample::getTitleName() const
{
    return "DDialog";
}

QString DDialogExample::getDescriptionInfo() const
{
    return "用于需要用户处理事务，又不希望跳转\n"
           "页面以致打断工作流程时。";
}

int DDialogExample::getFixedHeight() const
{
    return 967;
}

DFileDialogExample::DFileDialogExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QLabel *label1 = new QLabel;
    QLabel *label2 = new QLabel;
    DLabel *label3 = new DLabel;
    DFileChooserEdit *dialog = new DFileChooserEdit;
    QVBoxLayout *mainLayout= new QVBoxLayout(this);

    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    label1->setFixedSize(550, 334);
    label2->setFixedSize(550, 334);
    label3->setFixedSize(550, 387);
    label1->setScaledContents(true);
    label2->setScaledContents(true);
    label3->setScaledContents(true);
    label1->setPixmap(QPixmap(":/images/example/DFileDialog_1.png"));
    label2->setPixmap(QPixmap(":/images/example/DFileDialog_2.png"));
    label3->setPixmap(QPixmap(":/images/example/DFileDialog.png"));

    mainLayout->addWidget(dialog, 0, Qt::AlignCenter);
    mainLayout->addWidget(label1, 0, Qt::AlignCenter);
    mainLayout->addWidget(label2, 0, Qt::AlignCenter);
    mainLayout->addWidget(label3, 0, Qt::AlignCenter);
}

QString DFileDialogExample::getTitleName() const
{
    return "DFileDialog";
}

QString DFileDialogExample::getDescriptionInfo() const
{
    return "有需要调用打开文件，保存文件的地\n"
           "方。底部工具栏上面的选项和内容多少\n"
           "会根据应用自身的需要显示不同的内容。\n";
}

int DFileDialogExample::getFixedHeight() const
{
    return 1256;
}

DMessageManagerExample::DMessageManagerExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    QLabel *label = new QLabel;
    QVBoxLayout *labelLayout = new QVBoxLayout(label);
    DPushButton *btn1 = new DPushButton("点击按钮呼出自动消失的提示信息");
    DPushButton *btn2 = new DPushButton("点击按钮呼出不会自动消失的提示信息");

    connect(btn1, &DPushButton::clicked, this, [ = ] {
        DMessageManager::instance()->sendMessage(this, style()->standardIcon(QStyle::SP_MessageBoxWarning),
                                                 "成功添加到\"校园民谣\"");
    });
    connect(btn2, &DPushButton::clicked, this, [ = ] {
        DFloatingMessage *message = new DFloatingMessage(DFloatingMessage::ResidentType);
        message->setIcon(style()->standardIcon(QStyle::SP_MessageBoxWarning));
        message->setMessage("磁盘中的原文件已被修改，是否重新输入？");
        message->setWidget(new DPushButton("重新载入"));
        labelLayout->addWidget(message, 0, Qt::AlignCenter | Qt::AlignBottom);
        DMessageManager::instance()->sendMessage(this, message);
    });

    label->setScaledContents(true);
    label->setFixedSize(550, 309);
    label->setPixmap(QPixmap(":/images/example/dock_notice.png"));
    labelLayout->addStretch(10);

    mainlayout->setMargin(0);
    mainlayout->setSpacing(0);
    mainlayout->addWidget(btn1, 0, Qt::AlignCenter);
    mainlayout->addWidget(btn2, 0, Qt::AlignCenter);
    mainlayout->addWidget(label, 0, Qt::AlignCenter);
}

QString DMessageManagerExample::getTitleName() const
{
    return "DMessageManager";
}

QString DMessageManagerExample::getDescriptionInfo() const
{
    return "类型1\n"
           "这类应用内提醒是不需要用户进行操作\n"
           "的，过几秒钟后会自主消失，仅仅是向\n"
           "用户告知一些信息，比如什么成功或失\n"
           "败了，有什么需要注意的之类的。\n"
           "类型2\n"
           "这类应用内提醒需要用户的操作，在用\n"
           "户操作之前不会自主消失\n";
}

int DMessageManagerExample::getFixedHeight() const
{
    return 588;
}
