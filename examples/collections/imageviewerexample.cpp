// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "imageviewerexample.h"

#include <DImageViewer>
#include <DPushButton>

#include <QBoxLayout>

ImageViewerExampleWindow::ImageViewerExampleWindow(QWidget *parent)
    : PageWindowInterface(parent)
{
    addExampleWindow(new ImageViewerExample(this));
}

ImageViewerExample::ImageViewerExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    setLayout(mainLayout);

    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setSpacing(0);

    DPushButton *fitToWidget = new DPushButton("适应窗口", this);
    DPushButton *fitNormalSize = new DPushButton("1:1视图", this);
    DPushButton *rotate = new DPushButton("顺时针旋转", this);
    DPushButton *counterRotate = new DPushButton("顺时针旋转", this);
    DPushButton *beginCrop = new DPushButton("开始裁剪", this);
    DPushButton *endCrop = new DPushButton("结束裁剪", this);
    DPushButton *reset = new DPushButton("复位", this);

    buttonLayout->addWidget(fitToWidget);
    buttonLayout->addWidget(fitNormalSize);
    buttonLayout->addWidget(rotate);
    buttonLayout->addWidget(counterRotate);
    buttonLayout->addWidget(beginCrop);
    buttonLayout->addWidget(endCrop);
    buttonLayout->addWidget(reset);

    DImageViewer *imageViewer = new DImageViewer(this);
    imageViewer->setFixedSize(550, 350);
    imageViewer->setImage(QImage(":/images/example/DArrowRectangle.png"));
    imageViewer->fitToWidget();

    mainLayout->addSpacing(20);
    mainLayout->addLayout(buttonLayout, 0);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(imageViewer, 1, Qt::AlignCenter);

    connect(fitToWidget, &DPushButton::clicked, this, [=]() { imageViewer->fitToWidget(); });
    connect(fitNormalSize, &DPushButton::clicked, this, [=]() { imageViewer->fitNormalSize(); });
    connect(rotate, &DPushButton::clicked, this, [=]() { imageViewer->rotateClockwise(); });
    connect(counterRotate, &DPushButton::clicked, this, [=]() { imageViewer->rotateCounterclockwise(); });
    connect(beginCrop, &DPushButton::clicked, this, [=]() { imageViewer->beginCropImage(); });
    connect(endCrop, &DPushButton::clicked, this, [=]() { imageViewer->endCropImage(); });
    connect(reset, &DPushButton::clicked, this, [=]() {
        imageViewer->resetRotateAngle();
        imageViewer->resetCropImage();
    });
}

QString ImageViewerExample::getTitleName() const
{
    return "DImageViewer";
}

QString ImageViewerExample::getDescriptionInfo() const
{
    return "图片浏览控件，提供图片展示、旋转及裁剪功能";
}

int ImageViewerExample::getFixedHeight() const
{
    return 400;
}
