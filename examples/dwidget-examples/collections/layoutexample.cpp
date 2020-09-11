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

#include <DFrame>
#include <DSplitter>

#include "layoutexample.h"

DWIDGET_USE_NAMESPACE

LayoutExampleWindow::LayoutExampleWindow(QWidget *parent)
    : PageWindowInterface(parent)
{
    addExampleWindow(new DFrameExample(this));
    addExampleWindow(new DSplitterExample(this));
    addExampleWindow(new DVerticalLineExample(this));
    addExampleWindow(new DHorizontalLineExample(this));
}

DFrameExample::DFrameExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QWidget *frameWidget = new QWidget(this);
    QWidget *framePicWidget = new QWidget(this);

    DFrame *frame = new DFrame(frameWidget);
    QLabel *picLabel = new QLabel(framePicWidget);

    QHBoxLayout *frameLayout = new QHBoxLayout(frameWidget);
    QHBoxLayout *picLayout = new QHBoxLayout(framePicWidget);

    QPixmap picPixmap(":/images/example/DFrame.png");

    frame->setFixedHeight(240);

    picLabel->setFixedSize(550, 426);
    picLabel->setScaledContents(true);
    picLabel->setPixmap(picPixmap);

    frameLayout->setContentsMargins(23, 0, 23, 0);
    frameLayout->addWidget(frame);
    picLayout->setMargin(0);
    picLayout->addWidget(picLabel);

    layout->setSpacing(0);
    layout->addSpacing(30);
    layout->addWidget(frameWidget);
    layout->addSpacing(70);
    layout->addWidget(framePicWidget);
    layout->addSpacing(70);
    layout->setContentsMargins(10, 0, 10, 0);
}

QString DFrameExample::getTitleName() const
{
    return "DFrame";
}

QString DFrameExample::getDescriptionInfo() const
{
    return QString("用于框选某一部分选项，让这些框选的\n部分作为一个整体。");
}

int DFrameExample::getFixedHeight() const
{
    return 836;
}

DSplitterExample::DSplitterExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QWidget *splitterWidget = new QWidget(this);
    QWidget *splitterPicWidget = new QWidget(this);

    DSplitter *hSplitter = new DSplitter(Qt::Horizontal, splitterWidget);
    QLabel *picLabel = new QLabel(splitterPicWidget);

    QHBoxLayout *splitterLayout = new QHBoxLayout(splitterWidget);
    QHBoxLayout *picLayout = new QHBoxLayout(splitterPicWidget);

    QPixmap picPixmap(":/images/example/DSplitter.png");

    QWidget *spRightWidget = new QWidget;

    spRightWidget->setBackgroundRole(QPalette::Window);
    spRightWidget->setAutoFillBackground(true);

    hSplitter->setFrameShape(QFrame::Panel);
    hSplitter->setFrameShadow(QFrame::Raised);
    hSplitter->setHandleWidth(2);
    hSplitter->setFixedHeight(126);
    hSplitter->addWidget(new QWidget);
    hSplitter->addWidget(spRightWidget);
    hSplitter->setSizes({1, 6});

    picLabel->setFixedSize(550, 372);
    picLabel->setScaledContents(true);
    picLabel->setPixmap(picPixmap);

    splitterLayout->setSpacing(100);
    splitterLayout->setContentsMargins(169, 0, 169, 0);
    splitterLayout->addWidget(hSplitter);
    picLayout->setMargin(0);
    picLayout->addWidget(picLabel);

    layout->setSpacing(0);
    layout->addSpacing(30);
    layout->addWidget(splitterWidget);
    layout->addSpacing(70);
    layout->addWidget(splitterPicWidget);
    layout->addSpacing(70);
    layout->setContentsMargins(10, 0, 10, 0);
}

QString DSplitterExample::getTitleName() const
{
    return "DSplitter";
}

QString DSplitterExample::getDescriptionInfo() const
{
    return QString("所有需要左右隔开的地方，进行区域分\n隔。");
}

int DSplitterExample::getFixedHeight() const
{
    return 668;
}

DVerticalLineExample::DVerticalLineExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QWidget *verticalLineWidget = new QWidget(this);
    QWidget *verticalLinePicWidget = new QWidget(this);

    DVerticalLine *verticalLine = new DVerticalLine(verticalLineWidget);
    QLabel *picLabel = new QLabel(verticalLinePicWidget);

    QHBoxLayout *verticalLineLayout = new QHBoxLayout(verticalLineWidget);
    QHBoxLayout *picLayout = new QHBoxLayout(verticalLinePicWidget);

    QPixmap picPixmap(":/images/example/DVerticalline.png");

    verticalLine->setFixedHeight(28);

    picLabel->setFixedSize(550, 356);
    picLabel->setScaledContents(true);
    picLabel->setPixmap(picPixmap);

    verticalLineLayout->setMargin(0);
    verticalLineLayout->addWidget(verticalLine);
    picLayout->setMargin(0);
    picLayout->addWidget(picLabel);

    layout->setSpacing(0);
    layout->addSpacing(30);
    layout->addWidget(verticalLineWidget);
    layout->addSpacing(70);
    layout->addWidget(verticalLinePicWidget);
    layout->addSpacing(70);
    layout->setContentsMargins(10, 0, 10, 0);
}

QString DVerticalLineExample::getTitleName() const
{
    return "DVerticalLine";
}

QString DVerticalLineExample::getDescriptionInfo() const
{
    return QString("垂直分割线，用在左右需要分割的地\n方，比如DHeaderView里的列分割线。");
}

int DVerticalLineExample::getFixedHeight() const
{
    return 554;
}

DHorizontalLineExample::DHorizontalLineExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);

    DHorizontalLine *line = new DHorizontalLine(this);
    layout->addWidget(line);

    layout->setContentsMargins(35, 0, 35, 0);
}

QString DHorizontalLineExample::getTitleName() const
{
    return "DHorizontalLine";
}

QString DHorizontalLineExample::getDescriptionInfo() const
{
    return QString("水平分割线，用在上下需要分割的地\n方。");
}

int DHorizontalLineExample::getFixedHeight() const
{
    return 165;
}
