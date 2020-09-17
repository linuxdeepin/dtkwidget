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

#include "menuexample.h"

#include <QMenu>

#include <QLabel>
#include <QPainter>
#include <QVBoxLayout>
#include <QPropertyAnimation>

DWIDGET_USE_NAMESPACE

MenuExampleWindow::MenuExampleWindow(QWidget *parent)
    : PageWindowInterface(parent)
{
    addExampleWindow(new DMenuExample(this));
}

DMenuExample::DMenuExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    animation = new QPropertyAnimation(this, "aColor", this);

    restoreAnimation();
    connect(animation, &QPropertyAnimation::finished, this, [=]() {
        animation->setStartValue(animation->endValue());
        QColor startColor = animation->startValue().value<QColor>();
        animation->setEndValue(QColor(startColor.red(), startColor.green(), startColor.blue(), (255 - startColor.alpha())));
        animation->start();
    });

    QVBoxLayout *layout = new QVBoxLayout(this);
    QWidget *menuWidget = new QWidget(this);
    QWidget *menuPicWidget = new QWidget(this);
    QWidget *menuTopWidget = new QWidget(menuWidget);
    QHBoxLayout *menuTopLayout = new QHBoxLayout(menuTopWidget);
    QVBoxLayout *menuLayout = new QVBoxLayout(menuWidget);
    QHBoxLayout *picLayout = new QHBoxLayout(menuPicWidget);
    QLabel *label = new QLabel(menuPicWidget);
    QLabel *topLeftMenuLabel = new QLabel(menuTopWidget);
    QLabel *topMidMenuLabel = new QLabel(menuTopWidget);
    QLabel *topRightMenuLabel = new QLabel(menuTopWidget);
    QLabel *topBottomMenuLabel = new QLabel(menuTopWidget);

    topLeftMenuLabel->setFixedSize(182, 400);
    topLeftMenuLabel->setPixmap(QPixmap(":/images/example/DMenuPicture_1.png"));
    topLeftMenuLabel->setScaledContents(true);

    topMidMenuLabel->setFixedSize(182, 391);
    topMidMenuLabel->setPixmap(QPixmap(":/images/example/DMenuPicture_2.png"));
    topMidMenuLabel->setScaledContents(true);

    topRightMenuLabel->setFixedSize(162, 211);
    topRightMenuLabel->setPixmap(QPixmap(":/images/example/DMenuPicture_3.png"));
    topRightMenuLabel->setScaledContents(true);

    topBottomMenuLabel->setFixedSize(350, 113);
    topBottomMenuLabel->setPixmap(QPixmap(":/images/example/DMenuPicture_4.png"));
    topBottomMenuLabel->setScaledContents(true);

    pixmap = QPixmap(":/images/example/DMenu.png").scaled(550, 373, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    label->setFixedSize(550, 373);
    label->setPixmap(pixmap);
    label->setScaledContents(true);
    label->setObjectName("menuPicLabel");
    label->installEventFilter(this);

    leftMenu = new QMenu(menuTopWidget);

    picLayout->setMargin(0);
    picLayout->setSpacing(0);
    picLayout->addWidget(label);

    connect(leftMenu, &QMenu::aboutToShow, [=]() {
        animation->stop();
        restoreAnimation();
        acolor = QColor(15, 207, 255, 0);
        label->update();
    });

    connect(leftMenu, &QMenu::aboutToHide, [=]() {
        if (label->underMouse()) {
            restoreAnimation();
            animation->start();
        } else {
            restoreAnimation();
            label->update();
        }
    });

    QMenu *leftDocumentMenu = new QMenu("新建文档");
    leftDocumentMenu->addAction("办公文档");
    leftDocumentMenu->addAction("电子表格");
    leftDocumentMenu->addAction("演示文档");
    leftDocumentMenu->addAction("文本文档");

    QMenu *leftSortMenu = new QMenu("排序方式");
    leftSortMenu->addAction("名称");
    leftSortMenu->addAction("修改时间");
    leftSortMenu->addAction("大小");
    leftSortMenu->addAction("类型");

    QMenu *leftShowMenu = new QMenu("显示方式");
    leftShowMenu->addAction("图标");
    leftShowMenu->addAction("列表");
    leftShowMenu->addAction("分栏");

    leftMenu->addAction("新建文件夹");
    leftMenu->addMenu(leftDocumentMenu);
    leftMenu->addMenu(leftShowMenu);
    leftMenu->addMenu(leftSortMenu);
    leftMenu->addAction("以管理员身份打开");
    leftMenu->addAction("在终端中打开");
    leftMenu->addSeparator();
    leftMenu->addAction("粘贴");
    leftMenu->addAction("全选");
    leftMenu->addSeparator();
    leftMenu->addAction("属性");

    menuTopLayout->setSpacing(10);
    menuTopLayout->setMargin(0);
    menuTopLayout->addWidget(topLeftMenuLabel, 0, Qt::AlignBottom);
    menuTopLayout->addWidget(topMidMenuLabel, 0, Qt::AlignBottom);
    menuTopLayout->addWidget(topRightMenuLabel, 0, Qt::AlignBottom);

    menuLayout->setSpacing(30);
    menuLayout->setMargin(0);
    menuLayout->addWidget(menuTopWidget);
    menuLayout->addWidget(topBottomMenuLabel, 0, Qt::AlignCenter);

    layout->setContentsMargins(10, 0, 10, 0);
    layout->addSpacing(30);
    layout->addWidget(menuWidget);
    layout->addSpacing(70);
    layout->addWidget(menuPicWidget);
    layout->addSpacing(30);
}

QString DMenuExample::getTitleName() const
{
    return "DMenu";
}

QString DMenuExample::getDescriptionInfo() const
{
    return QString("DTK上经常用到的控件，主要出现在右\n"
                   "键，DCombobox弹出，主菜单，搜索\n"
                   "框的补全等一些地方。带尖角的菜单有\n"
                   "明确的指向，告诉用户这个菜单对应的\n"
                   "是哪个地方的。");
}

int DMenuExample::getFixedHeight() const
{
    return 1089;
}

QColor DMenuExample::getAColor()
{
    return acolor;
}

void DMenuExample::setAColor(const QColor &color)
{
    acolor = color;
    this->findChild<QLabel *>("menuPicLabel")->update();
}

bool DMenuExample::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this->findChild<QLabel *>("menuPicLabel")) {
        if (event->type() == QEvent::Paint) {
            QLabel *menuPicLabel = this->findChild<QLabel *>("menuPicLabel");
            QPixmap tPixmap = this->pixmap;
            QPainter p(&tPixmap);
            QPainterPath path;

            path.moveTo(QPoint(76, 236));
            path.lineTo(QPoint(278, 236));
            path.lineTo(QPoint(278, 136));
            path.lineTo(QPoint(537, 136));
            path.lineTo(QPoint(537, 345));
            path.lineTo(QPoint(76, 345));
            path.lineTo(QPoint(76, 236));

            p.setPen(this->acolor);
            p.setBrush(Qt::NoBrush);

            p.drawPath(path);

            p.drawText(QPoint(370, 218), "右键点击空白区域");
            menuPicLabel->setPixmap(tPixmap);
        } else if (event->type() == QEvent::Enter) {
            animation->start();
        } else if (event->type() == QEvent::Leave && !leftMenu->isVisible()) {
            animation->stop();
            restoreAnimation();

            QLabel *menuPicLabel = this->findChild<QLabel *>("menuPicLabel");
            menuPicLabel->update();
        } else if (event->type() == QEvent::MouseButtonRelease) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            QLabel *menuPicLabel = this->findChild<QLabel *>("menuPicLabel");

            if (mouseEvent->button() & Qt::RightButton) {
                QPoint mousePos = menuPicLabel->mapTo(menuPicLabel, mouseEvent->pos());
                QRegion region;
                region = region.united(QRect(278, 136, 259, 100));
                region = region.united(QRect(76, 236, 461, 109));

                if (region.contains(mousePos)) {
                    leftMenu->popup(menuPicLabel->mapToGlobal(mouseEvent->pos()));
                }
            }
        }
    }

    return false;
}

void DMenuExample::restoreAnimation()
{
    acolor = QColor(15, 207, 255, 255);
    animation->setStartValue(QVariant::fromValue<QColor>(QColor(15, 207, 255, 255)));
    animation->setEndValue(QVariant::fromValue<QColor>(QColor(15, 207, 255, 0)));
    animation->setDuration(1000);
}
