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

#include <QVBoxLayout>
#include <DWindowMaxButton>

#include <DStyle>
#include <DTitlebar>
#include <DFrame>
#include <DMainWindow>
#include <DStyle>
#include <DApplicationHelper>
#include <DPaletteHelper>
#include <QPen>
#include <DStatusBar>
#include <DTabBar>
#include <DSlider>

#include "windowexample.h"

DWIDGET_USE_NAMESPACE

class ExampTitlebar : public DTitlebar
{
public:
   ExampTitlebar(QIcon icon)
   {
       setIcon(icon);
       setFixedSize(530, 50);
   }

private:
   void paintEvent(QPaintEvent *e) override
   {
       DTitlebar::paintEvent(e);
       QPainter p(this);
       const DPalette &dp = DPaletteHelper::instance()->palette(this);

       p.save();
       p.setPen(QPen(dp.frameBorder(), 2));
       DDrawUtils::drawRoundedRect(&p, rect().adjusted(0 , 0, -1 , -1), 16, 16,
                                   DDrawUtils::Corner::TopLeftCorner | DDrawUtils::Corner::TopRightCorner);
       p.restore();
   }
};

class ExampWindow : public DMainWindow
{
public:
   ExampWindow(QIcon icon)
   {
       titlebar()->hide();
       title = new ExampTitlebar(icon);
       title->setParent(this);
       title->move(0, 0);
   }

   ExampTitlebar* eTitlebar()
   {
       return title;
   }
private:
   void paintEvent(QPaintEvent *e) override
   {
       DMainWindow::paintEvent(e);
       QPainter p(this);
       const DPalette &dp = DPaletteHelper::instance()->palette(this);

       p.save();
       p.setPen(QPen(dp.frameBorder(), 2));
       p.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 16, 16);
       p.restore();
   }
private:
   ExampTitlebar *title = nullptr;
};

class ExampStatusBar : public DStatusBar
{
public:
   ExampStatusBar()
   {
       QWidget *central = new QWidget;
       QHBoxLayout *layout = new QHBoxLayout;
       DSlider *slider = new DSlider(Qt::Horizontal);
       slider->setFixedWidth(131);

       layout->setSpacing(0);
       layout->setMargin(0);
       layout->addSpacing(254);
       layout->addWidget(new QLabel("7项"));
       layout->addWidget(slider, 0, Qt::AlignRight);
       central->setLayout(layout);

       setFixedSize(530, 30);
       central->setFixedWidth(530);
       addWidget(central, 10);
   }
private:
   void paintEvent(QPaintEvent *e) override
   {
       QPainter p(this);
       const DPalette &dp = DPaletteHelper::instance()->palette(this);

       p.setPen(QPen(dp.frameBorder(), 2));
       DDrawUtils::drawRoundedRect(&p, rect().adjusted(0 , 0, -1 , -1), 16, 16,
                                   DDrawUtils::Corner::BottomLeftCorner | DDrawUtils::Corner::BottomRightCorner);
       DStatusBar::paintEvent(e);
   }
};

WindowExampleWindow::WindowExampleWindow(QWidget *parent)
   : PageWindowInterface(parent)
{
   addExampleWindow(new DTitleBarExample(this));
   addExampleWindow(new DMainWindowExample(this));
   addExampleWindow(new DStatusBarExample(this));
   addExampleWindow(new DSizegripExample(this));
   addExampleWindow(new DTabBarExample(this));
}

DTitleBarExample::DTitleBarExample(QWidget *parent)
   : ExampleWindowInterface(parent)
{
   setProperty("DTitleBarExample", true);
   QVBoxLayout *mainLayout = new QVBoxLayout;
   ExampTitlebar *titlebar1 = new ExampTitlebar(QIcon::fromTheme("preferences-system"));
   ExampTitlebar *titlebar2 = new ExampTitlebar(QIcon(":/images/example/movie-logo.svg"));
   ExampTitlebar *titlebar3 = new ExampTitlebar(QIcon::fromTheme("preferences-system"));
   ExampTitlebar *titlebar4 = new ExampTitlebar(QIcon::fromTheme("preferences-system"));

   titlebar2->setBackgroundTransparent(true);


   QLabel *label1 = new QLabel;
   QLabel *label2 = new QLabel;
   QLabel *label3 = new QLabel;
   QLabel *label4 = new QLabel;
   QLabel *background = new QLabel(titlebar2);

   background->setObjectName("background");
   titlebar2->setObjectName("title");

   background->setFixedSize(550, 70);
   background->setPixmap(QPixmap(":/images/example/background.png"));
   background->setScaledContents(true);
   background->move(-10, -5);
   background->lower();

   mainLayout->setSpacing(20);

   titlebar2->addWidget(new QLabel("avatar(2009)108...-ndi[Team DRSD].mkv"), Qt::AlignLeft);
   //当前不能通过setWindowFlags()函数设置标志位隐藏,setVisible()也不可
   titlebar3->findChild<DWindowMaxButton *>()->setFixedSize(0, 0);
   titlebar4->findChild<DWindowMaxButton *>()->setFixedSize(0, 0);

   label1->setScaledContents(true);
   label1->setFixedSize(550, 372);
   label1->setPixmap(QPixmap(":/images/example/DTitlebar_1.png"));

   label2->setScaledContents(true);
   label2->setFixedSize(550, 372);
   label2->setPixmap(QPixmap(":/images/example/DTitlebar_2.png"));

   label3->setScaledContents(true);
   label3->setFixedSize(550, 372);
   label3->setPixmap(QPixmap(":/images/example/DTitlebar_3.png"));

   label4->setScaledContents(true);
   label4->setFixedSize(550, 372);
   label4->setPixmap(QPixmap(":/images/example/DTitlebar_4.png"));


   mainLayout->addWidget(titlebar1, 0, Qt::AlignCenter);
   mainLayout->addWidget(titlebar2, 0, Qt::AlignCenter);
   mainLayout->addWidget(titlebar3, 0, Qt::AlignCenter);
   mainLayout->addWidget(titlebar4, 0, Qt::AlignCenter);

   mainLayout->addWidget(label1, 0, Qt::AlignCenter);
   mainLayout->addWidget(label2, 0, Qt::AlignCenter);
   mainLayout->addWidget(label3, 0, Qt::AlignCenter);
   mainLayout->addWidget(label4, 0, Qt::AlignCenter);

   setLayout(mainLayout);
}

QString DTitleBarExample::getTitleName() const
{
   return "DTitleBar";
}

QString DTitleBarExample::getDescriptionInfo() const
{
   return "DTitleBar有几种样式:\n"
          "一,可以最大化且带不透明背景\n"
          "二,可以最大化带透明背景\n"
          "三,不可以最大化带不透明背景\n"
          "四,不可以最大化带透明背景\n";
}

int DTitleBarExample::getFixedHeight() const
{
    return 1942;
}

DMainWindowExample::DMainWindowExample(QWidget *parent)
   : ExampleWindowInterface(parent)
{
   QVBoxLayout *mainLayout = new QVBoxLayout(this);
   QLabel *label = new QLabel;


   label->setScaledContents(true);
   label->setFixedSize(550, 372);
   label->setPixmap(QPixmap(":/images/example/DTitlebar_1.png"));

   mainLayout->addWidget(label, 0, Qt::AlignCenter);
}

QString DMainWindowExample::getTitleName() const
{
   return "DMainWindow";
}

QString DMainWindowExample::getDescriptionInfo() const
{
   return "主窗口";
}

int DMainWindowExample::getFixedHeight() const
{
   return 662;
}

DStatusBarExample::DStatusBarExample(QWidget *parent)
   : ExampleWindowInterface(parent)
{
   QVBoxLayout *mainLayout = new QVBoxLayout;
   ExampStatusBar *bar = new ExampStatusBar;
   QLabel *label = new QLabel;

   setLayout(mainLayout);
   label->setScaledContents(true);
   label->setFixedSize(550, 372);
   label->setPixmap(QPixmap(":/images/example/DTitlebar_1.png"));


   mainLayout->addWidget(bar, 0, Qt::AlignCenter);
   mainLayout->addSpacing(80);
   mainLayout->addWidget(label, 0, Qt::AlignCenter);
}

QString DStatusBarExample::getTitleName() const
{
   return "DStatusBar";
}

QString DStatusBarExample::getDescriptionInfo() const
{
   return "状态栏";
}

int DStatusBarExample::getFixedHeight() const
{
   return 572;
}

DTabBarExample::DTabBarExample(QWidget *parent)
   : ExampleWindowInterface(parent)
{
   DTabBar *tabbar1 = new DTabBar;
   DTabBar *tabbar2 = new DTabBar;
   QLabel *label1 = new QLabel;
   QLabel *label2 = new QLabel;
   ExampWindow *window = new ExampWindow(QIcon::fromTheme("preferences-system"));
   QVBoxLayout *layout = new QVBoxLayout(this);

   tabbar1->setEnabledEmbedStyle(true);
   tabbar1->addTab("标签一");
   tabbar1->addTab("标签二");
   tabbar1->addTab("标签三");
   tabbar1->addTab("标签四");
   tabbar1->addTab("标签五");
   tabbar1->setExpanding(true);
   tabbar1->setFixedWidth(550);

   tabbar2->addTab("/etc/.co.");
   tabbar2->addTab("gtk-wid...");

   window->eTitlebar()->addWidget(tabbar2, Qt::AlignLeft);

   window->setFixedSize(550, 120);
   window->eTitlebar()->setFixedWidth(550);

   label1->setScaledContents(true);
   label2->setScaledContents(true);
   label1->setFixedSize(550, 356);
   label2->setFixedSize(550, 328);
   label1->setPixmap(QPixmap(":/images/example/DTabBar_1.png"));
   label2->setPixmap(QPixmap(":/images/example/DTabBar_2.png"));

   layout->addWidget(tabbar1, 0, Qt::AlignCenter);
   layout->addSpacing(40);
   layout->addWidget(window, 0, Qt::AlignCenter);
   layout->addSpacing(70);
   layout->addWidget(label1, 0, Qt::AlignCenter);
   layout->addWidget(label2, 0, Qt::AlignCenter);
}

QString DTabBarExample::getTitleName() const
{
   return "DTabBar";
}

QString DTabBarExample::getDescriptionInfo() const
{
   return "类型1"
          "这类标签用在应用主窗口内在DTitlebar\n"
          "底下用作多视图的切换，DTitlebar是在\n"
          "用户新建标签的时候才会出现，比如文\n"
          "件管理器。\n"
          "类型2\n"
          "这类标签用在DTitlebar内，也是用作\n"
          "多视图的切换，但和类型1性质一样，但\n"
          "是使用场景不一样，之所以合并在\n"
          "DTitlebar内是因为该应用没有别的功能\n"
          "需要放在DTitlebar上，比如文本编辑器\n"
          "和终端，除了文字部分，标签就是它们\n"
          "最常用的功能。\n";
}

int DTabBarExample::getFixedHeight() const
{
   return 1080;
}

DSizegripExample::DSizegripExample(QWidget *parent)
   : ExampleWindowInterface(parent)
{
   QVBoxLayout *layout = new QVBoxLayout;
   QLabel *label = new QLabel;

   label->setScaledContents(true);
   label->setFixedSize(550, 372);
   label->setPixmap(QPixmap(":/images/example/DSizegrip.png"));
   layout->addWidget(label, 0, Qt::AlignCenter);

   setLayout(layout);
}

QString DSizegripExample::getTitleName() const
{
   return "DSizegrip";
}

QString DSizegripExample::getDescriptionInfo() const
{
   return "尺寸控制";
}

int DSizegripExample::getFixedHeight() const
{
   return 662;
}
