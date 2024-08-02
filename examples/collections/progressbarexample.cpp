// SPDX-FileCopyrightText: 2020 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QDebug>
#include <DLabel>
#include <QVBoxLayout>
#include <DProgressBar>
#include <DWaterProgress>
#include <QPropertyAnimation>
#include <DColoredProgressBar>
#include <DIndeterminateProgressbar>
#include "progressbarexample.h"

DWIDGET_USE_NAMESPACE

static auto pBarRun = [](QWidget *pBar){
    auto animation = new QPropertyAnimation(pBar, "value");
    animation->setDuration(10000);
    animation->setLoopCount(-1);
    animation->setStartValue(0);
    animation->setEndValue(100);
    animation->start();
};

ProgressBarExampleWindow::ProgressBarExampleWindow(QWidget *parent)
    : PageWindowInterface(parent)
{
    addExampleWindow(new DProgressBarExample(this));
    addExampleWindow(new DWaterProgressExample(this));
    addExampleWindow(new DColoredProgressBarExample(this));
    addExampleWindow(new DIndeterminateProgressBarExample(this));
}

DProgressBarExample::DProgressBarExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    auto mainLayout = new QVBoxLayout(this);
    auto pTextBar = new DProgressBar();
    auto pNoTextBar = new DProgressBar();
    auto image1 = new DLabel();
    auto image2 = new DLabel();

    pTextBar->setTextVisible(true);
    pTextBar->setFixedWidth(500);
    pTextBar->setValue(45);
    pTextBar->setAlignment(Qt::AlignCenter);

    pNoTextBar->setFixedWidth(500);
    pNoTextBar->setValue(62);

    image1->setFixedSize(550, 426);
    image1->setScaledContents(true);
    image1->setPixmap(QPixmap(":/images/example/DProgressBar_1.png"));

    image2->setFixedSize(550, 426);
    image2->setScaledContents(true);
    image2->setPixmap(QPixmap(":/images/example/DProgressBar_2.png"));

    mainLayout->addWidget(pTextBar, 0, Qt::AlignCenter);
    mainLayout->addSpacing(49);
    mainLayout->addWidget(pNoTextBar, 0, Qt::AlignCenter);
    mainLayout->addSpacing(69);
    mainLayout->addWidget(image1, 0, Qt::AlignCenter);
    mainLayout->addSpacing(30);
    mainLayout->addWidget(image2, 0, Qt::AlignCenter);

    setLayout(mainLayout);

    connect(pTextBar, &DProgressBar::valueChanged, this, [=](int value){
        pTextBar->setFormat(QString("已下载%1%").arg(value));
    });

    pBarRun(pTextBar);
    pBarRun(pNoTextBar);
}

QString DProgressBarExample::getTitleName() const
{
    return "DProgressBar";
}

QString DProgressBarExample::getDescriptionInfo() const
{
    return QString("类型1\n"
                   "可以操作的进度条，点击可以进行暂\n"
                   "停，进度条内有文字。目前只有控制中\n"
                   "心更新部分用了。\n"
                   "类型2\n"
                   "所有需要用到进度条的地方，这种进度\n"
                   "条不可以操作，而是一种状态的指示，\n"
                   "告诉用户当前完成了多少或者使用了多\n"
                   "少的一个比例。");
}

int DProgressBarExample::getFixedHeight() const
{
    return 1143;
}

DWaterProgressExample::DWaterProgressExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    auto mainLayout = new QVBoxLayout(this);
    auto waterPBar = new DWaterProgress();
    auto image = new DLabel();

    waterPBar->setFixedSize(98, 98);
    waterPBar->setValue(99);
    waterPBar->start();

    image->setFixedSize(320, 410);
    image->setScaledContents(true);
    image->setPixmap(QPixmap(":/images/example/DWaterProgress.png"));

    mainLayout->addWidget(waterPBar, 0, Qt::AlignCenter);
    mainLayout->addSpacing(85);
    mainLayout->addWidget(image, 0, Qt::AlignCenter);

    setLayout(mainLayout);

    pBarRun(waterPBar);
}

QString DWaterProgressExample::getTitleName() const
{
    return "DWaterProgress";
}

QString DWaterProgressExample::getDescriptionInfo() const
{
    return QString("进度条另外一种带趣味的展示形式，作\n"
                   "用是减少用户枯燥的等待。主要用在小\n"
                   "工具主窗口内部，作为一个中间状态展\n"
                   "示给用户，最终的结果往往会跟随成功\n"
                   "或者失败的图标。");
}

int DWaterProgressExample::getFixedHeight() const
{
    return 708;
}

DColoredProgressBarExample::DColoredProgressBarExample(QWidget *parent)
    : ExampleWindowInterface(parent)

{
    auto mainLayout = new QVBoxLayout(this);
    auto clrPBar = new DColoredProgressBar();

    clrPBar->addThreshold(10, QBrush(QColor(Qt::black)));
    clrPBar->addThreshold(20, QBrush(QColor(Qt::red)));
    clrPBar->addThreshold(30, QBrush(QColor(Qt::green)));
    clrPBar->addThreshold(40, QBrush(QColor(Qt::blue)));
    clrPBar->addThreshold(50, QBrush(QColor(Qt::cyan)));
    clrPBar->addThreshold(60, QBrush(QColor(Qt::darkGray)));
    clrPBar->addThreshold(70, QBrush(QColor(Qt::black)));
    clrPBar->addThreshold(80, QBrush(QColor(Qt::green)));
    clrPBar->addThreshold(90, QBrush(QColor(Qt::magenta)));

    clrPBar->setFixedSize(500, 35);
    mainLayout->addWidget(clrPBar, 0, Qt::AlignCenter);
    setLayout(mainLayout);

    pBarRun(clrPBar);
}

QString DColoredProgressBarExample::getTitleName() const
{
    return "DColoredProgressBar";
}

QString DColoredProgressBarExample::getDescriptionInfo() const
{
    return QString("进度条另外一种带趣味的展示形式，作\n"
                   "用是减少用户枯燥的等待。主要用在小\n"
                   "工具主窗口内部，作为一个中间状态展\n"
                   "示给用户，最终的结果往往会跟随成功\n"
                   "或者失败的图标。");
}

int DColoredProgressBarExample::getFixedHeight() const
{
    return 200;
}

DIndeterminateProgressBarExample::DIndeterminateProgressBarExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    auto mainLayout = new QVBoxLayout(this);
    auto indeterBar = new DIndeterminateProgressbar();
    indeterBar->setFixedSize(500, 35);
    mainLayout->addWidget(indeterBar, 0, Qt::AlignCenter);
    setLayout(mainLayout);
}

QString DIndeterminateProgressBarExample::getTitleName() const
{
    return "DIndeterminateProgressbar";
}

QString DIndeterminateProgressBarExample::getDescriptionInfo() const
{
    return QString("一个模糊进度条，不展示具体进度值，\n"
                   "用于等待时间不确定的情况。主要用\n"
                   "在小工具主窗口内部，作为一个中间状态\n"
                   "展示给用户，最终的结果往往会跟随成功\n"
                   "或者失败的图标。");
}

int DIndeterminateProgressBarExample::getFixedHeight() const
{
    return 200;
}
