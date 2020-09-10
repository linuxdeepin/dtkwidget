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
#include <QComboBox>

#include <DFrame>
#include <DBackgroundGroup>
#include <DListView>
#include <DGroupBox>
#include <QToolButton>
#include <DTreeView>
#include <DTableView>
#include <DHeaderView>
#include <DColumnView>

#include "listviewexample.h"

DWIDGET_USE_NAMESPACE

ListViewExampleWindow::ListViewExampleWindow(QWidget *parent)
    : PageWindowInterface(parent)
{
    addExampleWindow(new DBackgroundGroupExample(this));
    addExampleWindow(new DListViewExample(this));
    addExampleWindow(new DGroupBoxExample(this));
    addExampleWindow(new DTreeViewExample(this));
    addExampleWindow(new DHeaderViewExample(this));
    addExampleWindow(new DColumnViewExample(this));
}

DBackgroundGroupExample::DBackgroundGroupExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    this->setFixedHeight(636);
    QVBoxLayout *layout = new QVBoxLayout(this);
    QWidget *bgWidget = new QWidget(this);
    QWidget *bgPicWidget = new QWidget(this);
    QHBoxLayout *bgwLayout = new QHBoxLayout(bgWidget);
    QVBoxLayout *bgGLayout = new QVBoxLayout;
    QHBoxLayout *bgpicLayout = new QHBoxLayout(bgPicWidget);
    DBackgroundGroup *bgGroup = new DBackgroundGroup(bgGLayout, bgWidget);
    QLabel *bgPicLabel = new QLabel(bgPicWidget);

    bgPicLabel->setAlignment(Qt::AlignCenter);
    bgPicLabel->setPixmap(QPixmap(":/images/example/DBackgroundGroup.png").scaled(550, 356));
    bgpicLayout->addWidget(bgPicLabel);

    bgGroup->setItemSpacing(1);
    bgGroup->setItemMargins(QMargins(0, 0, 0, 0));
    bgGroup->setUseWidgetBackground(true);

    QFrame *frame1 = new QFrame;
    QFrame *frame2 = new QFrame;
    QFrame *frame3 = new QFrame;

    frame1->setMinimumHeight(36);
    frame2->setMinimumHeight(36);
    frame3->setMinimumHeight(36);
    frame1->setBackgroundRole(DPalette::AlternateBase);
    frame2->setBackgroundRole(DPalette::Shadow);
    frame3->setBackgroundRole(DPalette::AlternateBase);

    bgGLayout->addWidget(frame1);
    bgGLayout->addWidget(frame2);
    bgGLayout->addWidget(frame3);
    bgwLayout->addWidget(bgGroup);
    bgwLayout->setContentsMargins(105, 0, 105, 0);
    bgGLayout->setContentsMargins(0, 0, 0, 0);
    bgpicLayout->setContentsMargins(0, 0, 0, 0);

    layout->setContentsMargins(10, 0, 10, 0);
    layout->addSpacing(30);
    layout->setSpacing(70);
    layout->addWidget(bgWidget);
    layout->addWidget(bgPicWidget);
    layout->addStretch();
}

QString DBackgroundGroupExample::getTitleName() const
{
    return "DBackgroundGroup";
}

QString DBackgroundGroupExample::getDescriptionInfo() const
{
    return "在设置选项里作为一个组合选项的背景\n使用。";
}

int DBackgroundGroupExample::getFixedHeight() const
{
    return 636;
}

DListViewExample::DListViewExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    auto listViewInit = [](DListView *lv, int fixHeight, int space, QStandardItemModel *model) {
        lv->setModel(model);
        lv->setItemSpacing(space);
        lv->setSpacing(0);
        lv->setIconSize(QSize(32, 32));
        lv->setFixedHeight(fixHeight);
        lv->setContentsMargins(0, 0, 0, 0);
    };

    QVBoxLayout *layout = new QVBoxLayout(this);
    QWidget *listViewWidget = new QWidget(this);
    QWidget *listviewPicWidget = new QWidget(this);
    QVBoxLayout *listViewWLayout = new QVBoxLayout(listViewWidget);
    QVBoxLayout *listviewPicLayout = new QVBoxLayout(listviewPicWidget);
    DListView *fingerPrintLV = new DListView(listViewWidget);
    DListView *browserLV = new DListView(listViewWidget);
    DListView *screenLV = new DListView(listViewWidget);
    QStandardItemModel *fingerPrintModel = new QStandardItemModel;
    QStandardItemModel *browserModel = new QStandardItemModel;
    QStandardItemModel *screenModel = new QStandardItemModel;
    QLabel *picLabel1 = new QLabel(listviewPicWidget);
    QLabel *picLabel2 = new QLabel(listviewPicWidget);
    QLabel *picLabel3 = new QLabel(listviewPicWidget);

    listViewInit(fingerPrintLV, 111, 1, fingerPrintModel);
    listViewInit(browserLV, 232, 10, browserModel);
    listViewInit(screenLV, 326, 10, screenModel);

    picLabel1->setAlignment(Qt::AlignCenter);
    picLabel2->setAlignment(Qt::AlignCenter);
    picLabel3->setAlignment(Qt::AlignCenter);
    picLabel1->setPixmap(QPixmap(":/images/example/DListView_1.png").scaled(550, 426));
    picLabel2->setPixmap(QPixmap(":/images/example/DListView_2.png").scaled(550, 426));
    picLabel3->setPixmap(QPixmap(":/images/example/DListView_3.png").scaled(550, 426));

    DStandardItem *fingerPrintItem1 = new DStandardItem("右手大拇指");
    DStandardItem *fingerPrintItem2 = new DStandardItem("手指2");
    DStandardItem *fingerPrintItem3 = new DStandardItem("添加指纹");

    DStandardItem *browserItem1 = new DStandardItem(QIcon("://images/example/DListViewBrowser_1.svg"), "谷歌浏览器");
    DStandardItem *browserItem2 = new DStandardItem(QIcon("://images/example/DListViewBrowser_2.svg"), "火狐浏览器");
    DStandardItem *browserItem3 = new DStandardItem(QIcon("://images/example/DListViewBrowser_3.svg"), "遨游浏览器");
    DStandardItem *browserItem4 = new DStandardItem(QIcon("://images/example/DListViewBrowser_4.svg"), "Opera");

    DStandardItem *screenItem1 = new DStandardItem(QIcon(":/images/example/DListViewScreen_1.svg"), "复制");
    DViewItemAction *screenItemAction1 = new DViewItemAction;

    screenItemAction1->setText("把您的一个屏幕的内容复制到另外一个或多个屏幕");
    screenItemAction1->setFontSize(DFontSizeManager::T8);
    screenItemAction1->setTextColorRole(DPalette::TextTips);
    screenItem1->setTextActionList({screenItemAction1});

    DStandardItem *screenItem2 = new DStandardItem(QIcon(":/images/example/DListViewScreen_2.svg"), "拓展");
    DViewItemAction *screenItemAction2 = new DViewItemAction;

    screenItemAction2->setText("将您的屏幕内容扩展，在不同屏幕上显示不同内容");
    screenItemAction2->setFontSize(DFontSizeManager::T8);
    screenItemAction2->setTextColorRole(DPalette::TextTips);
    screenItem2->setTextActionList({screenItemAction2});

    DStandardItem *screenItem3 = new DStandardItem(QIcon(":/images/example/DListViewScreen_3.svg"), "只在 VGA1显示");
    DViewItemAction *screenItemAction3 = new DViewItemAction;

    screenItemAction3->setText("只在 VGA1上显示屏幕内容，其他屏幕不显示");
    screenItemAction3->setFontSize(DFontSizeManager::T8);
    screenItemAction3->setTextColorRole(DPalette::TextTips);
    screenItem3->setTextActionList({screenItemAction3});

    DStandardItem *screenItem4 = new DStandardItem(QIcon(":/images/example/DListViewScreen_4.svg"), "只在 LVDS1显示");
    DViewItemAction *screenItemAction4 = new DViewItemAction;

    screenItemAction4->setText("只在 LVDS1上显示屏幕内容，其他屏幕不显示");
    screenItemAction4->setFontSize(DFontSizeManager::T8);
    screenItemAction4->setTextColorRole(DPalette::TextTips);
    screenItem4->setTextActionList({screenItemAction4});

    fingerPrintItem3->setFontSize(DFontSizeManager::T8);
    fingerPrintItem3->setTextColorRole(DPalette::Link);
    fingerPrintItem3->setSelectable(false);

    fingerPrintModel->appendRow(fingerPrintItem1);
    fingerPrintModel->appendRow(fingerPrintItem2);
    fingerPrintModel->appendRow(fingerPrintItem3);
    browserModel->appendRow(browserItem1);
    browserModel->appendRow(browserItem2);
    browserModel->appendRow(browserItem3);
    browserModel->appendRow(browserItem4);
    screenModel->appendRow(screenItem1);
    screenModel->appendRow(screenItem2);
    screenModel->appendRow(screenItem3);
    screenModel->appendRow(screenItem4);

    listViewWLayout->setContentsMargins(85, 0, 85, 0);
    listViewWLayout->setSpacing(60);
    listViewWLayout->addWidget(fingerPrintLV, 2);
    listViewWLayout->addWidget(browserLV, 3);
    listViewWLayout->addWidget(screenLV, 4);
    listviewPicLayout->setSpacing(30);
    listviewPicLayout->addWidget(picLabel1);
    listviewPicLayout->addWidget(picLabel2);
    listviewPicLayout->addWidget(picLabel3);
    layout->addSpacing(30);
    layout->setSpacing(70);
    layout->setContentsMargins(10, 0, 10, 0);
    layout->addWidget(listViewWidget);
    layout->addWidget(listviewPicWidget);
    layout->addStretch();
}

QString DListViewExample::getTitleName() const
{
    return "DListView";
}

QString DListViewExample::getDescriptionInfo() const
{
    return "标准的单行列表。";
}

int DListViewExample::getFixedHeight() const
{
    return 2286;
}

DGroupBoxExample::DGroupBoxExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    this->setFixedHeight(644);

    QWidget *groupBoxWidget = new QWidget(this);
    QWidget *gbPicWidget = new QWidget(this);
    QHBoxLayout *gbPicLayout = new QHBoxLayout(gbPicWidget);
    QVBoxLayout *layout = new QVBoxLayout(this);
    QVBoxLayout *groupBoxWLayout = new QVBoxLayout(groupBoxWidget);
    DGroupBox *groupBox = new DGroupBox(groupBoxWidget);
    QVBoxLayout *groupBoxLayout = new QVBoxLayout(groupBox);
    QWidget *contentWidget = new QWidget(groupBox);
    QHBoxLayout *contentLayout = new QHBoxLayout(contentWidget);
    QLabel *contentTextLabel = new QLabel("代理方式");
    QComboBox *contentComboBox = new QComboBox;
    QLabel *gbPicLabel = new QLabel(gbPicWidget);

    gbPicLabel->setAlignment(Qt::AlignCenter);
    gbPicLabel->setPixmap(QPixmap(":/images/example/DGroupBox.png").scaled(568, 444));
    contentComboBox->addItems({"自动"});
    contentComboBox->setMinimumWidth(213);
    groupBoxWidget->setFixedHeight(48);

    contentLayout->setContentsMargins(10, 6, 10, 6);
    contentLayout->addWidget(contentTextLabel);
    contentLayout->addStretch();
    contentLayout->addWidget(contentComboBox);

    groupBoxLayout->setContentsMargins(0, 0, 0, 0);
    groupBoxLayout->addWidget(contentWidget);
    groupBoxWLayout->setContentsMargins(105, 0, 105, 0);
    groupBoxWLayout->addWidget(groupBox);

    gbPicLayout->setContentsMargins(0, 0, 0, 0);
    gbPicLayout->addWidget(gbPicLabel);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addSpacing(30);
    layout->addWidget(groupBoxWidget, 0);
    layout->addSpacing(70);
    layout->addWidget(gbPicWidget, 1);
    layout->addSpacing(70);
}

QString DGroupBoxExample::getTitleName() const
{
    return "DGroupBox";
}

QString DGroupBoxExample::getDescriptionInfo() const
{
    return "带图标的单行列表\n带图标的多行列表";
}

int DGroupBoxExample::getFixedHeight() const
{
    return 644;
}

DTreeViewExample::DTreeViewExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    this->setFixedHeight(908);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QWidget *treeViewWidget = new QWidget;
    QWidget *tvPicWidget = new QWidget;
    QVBoxLayout *tvLayout = new QVBoxLayout(treeViewWidget);
    QVBoxLayout *tvPLayout = new QVBoxLayout(tvPicWidget);
    DTreeView *treeView = new DTreeView;
    QStandardItemModel *model = new QStandardItemModel(treeView);
    QStyledItemDelegate *delegate = new QStyledItemDelegate(treeView);
    QLabel *picLabel = new QLabel;

    picLabel->setAlignment(Qt::AlignCenter);
    picLabel->setPixmap(QPixmap(":/images/example/DTreeView.png").scaled(550, 414));

    treeView->setItemDelegate(delegate);
    treeView->setModel(model);
    treeView->setHeaderHidden(true);
    treeView->setFrameShape(QFrame::NoFrame);
    treeView->expandAll();
    treeViewWidget->setFixedHeight(324);

    DStandardItem *groupItem = new DStandardItem("群组");
    DStandardItem *friendItem = new DStandardItem("我的好友");
    DStandardItem *classmateItem = new DStandardItem("同学");
    DStandardItem *relativeItem = new DStandardItem("亲人");

    DStandardItem *friend1 = new DStandardItem(QIcon(":/images/example/DTreeViewIcon_1.svg"), "张三");
    DStandardItem *friend2 = new DStandardItem(QIcon(":/images/example/DTreeViewIcon_2.svg"), "老吴");
    DStandardItem *friend3 = new DStandardItem(QIcon(":/images/example/DTreeViewIcon_3.svg"), "李四");
    DStandardItem *friend4 = new DStandardItem(QIcon(":/images/example/DTreeViewIcon_4.svg"), "安吉");
    DStandardItem *friend5 = new DStandardItem(QIcon(":/images/example/DTreeViewIcon_5.svg"), "陈永斌");

    groupItem->setFontSize(DFontSizeManager::T6);
    groupItem->setSizeHint(QSize(groupItem->sizeHint().width(), 36));
    friendItem->setFontSize(DFontSizeManager::T6);
    friendItem->setSizeHint(QSize(friendItem->sizeHint().width(), 36));
    classmateItem->setFontSize(DFontSizeManager::T6);
    classmateItem->setSizeHint(QSize(classmateItem->sizeHint().width(), 36));
    relativeItem->setFontSize(DFontSizeManager::T6);
    relativeItem->setSizeHint(QSize(relativeItem->sizeHint().width(), 36));

    friend1->setSizeHint(QSize(friend1->sizeHint().width(), 36));
    friend1->setFontSize(DFontSizeManager::T7);
    friend1->setBackgroundRole(DPalette::AlternateBase);
    friend2->setSizeHint(QSize(friend2->sizeHint().width(), 36));
    friend2->setFontSize(DFontSizeManager::T7);
    friend2->setBackgroundRole(DPalette::AlternateBase);
    friend3->setSizeHint(QSize(friend3->sizeHint().width(), 36));
    friend3->setFontSize(DFontSizeManager::T7);
    friend3->setBackgroundRole(DPalette::AlternateBase);
    friend4->setSizeHint(QSize(friend4->sizeHint().width(), 36));
    friend4->setFontSize(DFontSizeManager::T7);
    friend4->setBackgroundRole(DPalette::AlternateBase);
    friend5->setSizeHint(QSize(friend5->sizeHint().width(), 36));
    friend5->setFontSize(DFontSizeManager::T7);
    friend5->setBackgroundRole(DPalette::AlternateBase);

    model->appendRow(groupItem);
    model->appendRow(friendItem);
    model->appendRow(classmateItem);
    model->appendRow(relativeItem);
    treeView->setExpanded(model->index(1, 0), true);
    // 此处统一设置了iconsize  但仅有第一个生效   原因不明
    treeView->setIconSize(QSize(24, 24));

    friendItem->appendRows({friend1, friend2, friend3, friend4, friend5});
    treeView->setCurrentIndex(model->indexFromItem(friend4));

    tvLayout->setContentsMargins(160, 0, 160, 0);
    tvPLayout->setContentsMargins(0, 0, 0, 0);
    tvLayout->addWidget(treeView);
    tvPLayout->addWidget(picLabel);
    layout->setContentsMargins(10, 0, 10, 0);
    layout->addSpacing(30);
    layout->setSpacing(70);
    layout->addWidget(treeViewWidget);
    layout->addWidget(tvPicWidget);
    layout->addSpacing(70);
}

QString DTreeViewExample::getTitleName() const
{
    return "DTreeView";
}

QString DTreeViewExample::getDescriptionInfo() const
{
    return "需要使用树状结构的地方。";
}

int DTreeViewExample::getFixedHeight() const
{
    return 908;
}

DHeaderViewExample::DHeaderViewExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    this->setFixedHeight(805);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QWidget *headerviewWidget = new QWidget(this);
    QWidget *hvPicWidget = new QWidget(this);
    QVBoxLayout *hvLayout = new QVBoxLayout(headerviewWidget);
    QVBoxLayout *hvpicLayout = new QVBoxLayout(hvPicWidget);
    DListView *tv = new DListView;
    QLabel *picLabel = new QLabel;
    QStandardItemModel *model = new QStandardItemModel;
    DHeaderView *headerview = new DHeaderView(Qt::Horizontal);
    QStandardItemModel *hmodel = new QStandardItemModel;

    headerview->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    headerview->setModel(hmodel);
    picLabel->setAlignment(Qt::AlignCenter);
    picLabel->setPixmap(QPixmap(":/images/example/DHeaderView.png").scaled(560, 373));
    headerview->setMaximumHeight(36);
    headerview->setSectionResizeMode(DHeaderView::Stretch);
    headerview->setSortIndicator(0, Qt::SortOrder::DescendingOrder);
    headerview->setSectionsClickable(true);
    headerview->setSortIndicatorShown(true);

    tv->setModel(model);
    tv->setItemSpacing(0);
    tv->setSpacing(0);
    tv->addHeaderWidget(headerview);
    headerviewWidget->setFixedHeight(278);

    hmodel->setHorizontalHeaderLabels({"名称", "修改时间", "类型", "大小"});

    model->appendRow(new DStandardItem(QIcon::fromTheme("folder-videos"), "视频"));
    model->appendRow(new DStandardItem(QIcon::fromTheme("folder-pictures"), "图片"));
    model->appendRow(new DStandardItem(QIcon::fromTheme("folder-documents"), "文档"));
    model->appendRow(new DStandardItem(QIcon::fromTheme("folder-downloads"), "下载"));
    model->appendRow(new DStandardItem(QIcon::fromTheme("folder-music"), "音乐"));
    model->appendRow(new DStandardItem(QIcon::fromTheme("user-desktop"), "桌面"));

    hvpicLayout->addWidget(picLabel);
    hvpicLayout->setContentsMargins(0, 0, 0, 0);
    hvLayout->addWidget(tv);
    hvLayout->setContentsMargins(0, 0, 0, 0);
    layout->addSpacing(30);
    layout->setSpacing(70);
    layout->setContentsMargins(10, 0, 10, 0);
    layout->addWidget(headerviewWidget);
    layout->addWidget(hvPicWidget);
    layout->addSpacing(70);
}

QString DHeaderViewExample::getTitleName() const
{
    return "DHeaderView";
}

QString DHeaderViewExample::getDescriptionInfo() const
{
    return "列表视图的头部，方便用户进行排序及\n正序倒序。";
}

int DHeaderViewExample::getFixedHeight() const
{
    return 805;
}

DColumnViewExample::DColumnViewExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    DFrame *frame = new DFrame(this);
    QHBoxLayout *frameLayout = new QHBoxLayout(frame);

    DColumnView *cv = new DColumnView;
    QStandardItemModel *model = new QStandardItemModel;
    QStyledItemDelegate *itemDelegate = new QStyledItemDelegate(cv);

    auto insertItem = [](const QString &name, const QIcon &icon, int height, QStandardItemModel *model = nullptr, DStandardItem *parentItem = nullptr)
        -> DStandardItem * {
        DStandardItem *item = new DStandardItem(icon, name);
        item->setSizeHint(QSize(item->sizeHint().width(), height));
        item->setEditable(false);
        item->setFontSize(DFontSizeManager::T8);

        if (model)
            model->appendRow(item);

        if (parentItem)
            parentItem->appendRow(item);

        return item;
    };

    frame->setFixedHeight(336);
    cv->setFrameShape(QFrame::NoFrame);
    cv->setColumnWidths({121, 162});
    cv->setItemDelegate(itemDelegate);
    cv->setIconSize(QSize(24, 24));

    insertItem("视频", style()->standardIcon(QStyle::SP_DirIcon), 36, model);
    DStandardItem *picItem = insertItem("图片", style()->standardIcon(QStyle::SP_DirIcon), 36, model);
    insertItem("文档", style()->standardIcon(QStyle::SP_DirIcon), 36, model);
    insertItem("下载", style()->standardIcon(QStyle::SP_DirIcon), 36, model);
    insertItem("音乐", style()->standardIcon(QStyle::SP_DirIcon), 36, model);
    insertItem("桌面", style()->standardIcon(QStyle::SP_DirIcon), 36, model);

    insertItem("我的图片", style()->standardIcon(QStyle::SP_DirIcon), 36, nullptr, picItem);
    DStandardItem *myPicItem = insertItem("我的壁纸", style()->standardIcon(QStyle::SP_DirIcon), 36, nullptr, picItem);
    insertItem("Snapshot", style()->standardIcon(QStyle::SP_DirIcon), 36, nullptr, picItem);
    insertItem("深度截图", style()->standardIcon(QStyle::SP_DirIcon), 36, nullptr, picItem);
    insertItem("iphone相册", style()->standardIcon(QStyle::SP_DirIcon), 36, nullptr, picItem);

    insertItem("[WP] Mosaic [textless].jpg", QIcon(":/images/example/DColumViewPicIcon_1.png"), 36, nullptr, myPicItem);
    insertItem("2.jpg", QIcon(":/images/example/DColumViewPicIcon_2.png"), 36, nullptr, myPicItem);
    insertItem("underwater_16_10.jpg", QIcon(":/images/example/DColumViewPicIcon_3.png"), 36, nullptr, myPicItem);
    insertItem("inthe sky.jpg", QIcon(":/images/example/DColumViewPicIcon_4.png"), 36, nullptr, myPicItem);
    insertItem("25_III_2560_1600.jpg", QIcon(":/images/example/DColumViewPicIcon_5.png"), 36, nullptr, myPicItem);
    insertItem("164_scaled.jpg", QIcon(":/images/example/DColumViewPicIcon_6.png"), 36, nullptr, myPicItem);
    insertItem("[WP] Mosaic [textless].jpg", QIcon(":/images/example/DColumViewPicIcon_7.png"), 36, nullptr, myPicItem);
    insertItem("03345_tyrrhenum_3840x2400.jpg", QIcon(":/images/example/DColumViewPicIcon_8.png"), 36, nullptr, myPicItem);
    insertItem("03215_goodmorningyosemite_38..", QIcon(":/images/example/DColumViewPicIcon_9.png"), 36, nullptr, myPicItem);

    cv->setModel(model);
    cv->setCurrentIndex(model->indexFromItem(picItem));
    cv->setCurrentIndex(model->indexFromItem(myPicItem));

    frameLayout->addWidget(cv);
    frameLayout->setContentsMargins(5, 5, 5, 5);
    layout->setContentsMargins(10, 0, 10, 0);
    layout->addSpacing(30);
    layout->addWidget(frame);
    layout->addSpacing(70);
}

QString DColumnViewExample::getTitleName() const
{
    return "DColumnView";
}

QString DColumnViewExample::getDescriptionInfo() const
{
    return "列视图，列数不是固定的，根据显示的\n空间和实际的层级决定。";
}

int DColumnViewExample::getFixedHeight() const
{
    return 425;
}
