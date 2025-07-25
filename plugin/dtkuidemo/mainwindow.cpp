// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <DTitlebar>
#include <DSearchEdit>
#include <denhancedwidget.h>
#include <DStackedWidget>

#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    DMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());

    // 给 title bar 添加搜索框
    DSearchEdit *search = new DSearchEdit;
    search->setPlaceHolder("探索未知领域～");
    this->titlebar()->addWidget(search);
    this->titlebar()->setIcon(QIcon(":/resources/logo_icon.svg"));

    // 设置 DButtonBox 里面的 button
    DButtonBox *buttonBox = ui->Dtk__Widget__DButtonBox;
    buttonBox->setButtonList({new DButtonBoxButton("浅色模式"), new DButtonBoxButton("深色模式")}, true);
    buttonBox->setId(buttonBox->buttonList().at(0), 0);
    buttonBox->setId(buttonBox->buttonList().at(1), 1);
    if (DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::LightType) {
        buttonBox->buttonList().at(0)->click();
    } else {
        buttonBox->buttonList().at(1)->click();
    }

    connect(buttonBox, &DButtonBox::buttonClicked, this, [buttonBox](QAbstractButton *button) {
        if (buttonBox->id(button) == 0) {
            DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::LightType);
        } else {
            DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::DarkType);
        }
    });
    titlebar()->addWidget(buttonBox);


    // 设置 LineDrawer 里面的内容
    DArrowLineDrawer *expand = ui->Dtk__Widget__DArrowLineDrawer;
    QWidget *content = new QWidget(this);
    QLabel *lab = new QLabel("  在这里可以显示你想要显示的绝大部分内容", content);
    QFont f(lab->font());
    f.setPixelSize(28);
    f.setWeight(QFont::Medium);
    f.setBold(false);
    lab->setFont(f);
    lab->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter );

    content->setMinimumHeight(content->height() + expand->height());

    expand->setTitle("可折叠的提示信息");
    expand->setContent(content);
    expand->setExpand(false);

    expand->setExpandedSeparatorVisible(false);
    expand->setSeparatorVisible(false);

    connect(expand, &DArrowLineDrawer::expandChange, [](bool expanded) {
        qDebug() << "DArrow state: " << (expanded ? "expanded" : "closed");
    });

    DEnhancedWidget *hanceedWidget = new DEnhancedWidget(expand, expand);
    connect(hanceedWidget, &DEnhancedWidget::heightChanged, hanceedWidget, [](int height) {
        qDebug() << "height changed: " << height;
    });

    // 设置带颜色的进度条的进度
    DColoredProgressBar *clrPBar = ui->Dtk__Widget__DColoredProgressBar;
    clrPBar->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    clrPBar->addThreshold(10, QBrush(QColor(Qt::black)));
    clrPBar->addThreshold(20, QBrush(QColor(Qt::red)));
    clrPBar->addThreshold(30, QBrush(QColor(Qt::green)));
    clrPBar->addThreshold(40, QBrush(QColor(Qt::blue)));
    clrPBar->addThreshold(50, QBrush(QColor(Qt::cyan)));
    clrPBar->addThreshold(60, QBrush(QColor(Qt::darkGray)));
    clrPBar->addThreshold(70, QBrush(QColor(Qt::black)));
    clrPBar->addThreshold(80, QBrush(QColor(Qt::green)));
    clrPBar->addThreshold(90, QBrush(QColor(Qt::magenta)));

    QTimer *timer = new QTimer(this->centralWidget());
    connect(timer, &QTimer::timeout, [clrPBar](){
        static int i = 0;
        i++;
        i = i%101;
        clrPBar->setValue(i);
    });
    timer->setInterval(100);
    timer->start();


    // 设置不带颜色的进度条的进度
    ui->Dtk__Widget__DProgressBar->setAlignment(Qt::AlignCenter);
    QTimer *timer2 = new QTimer(this->centralWidget());
    connect(timer2, &QTimer::timeout, [this](){
        static int i = 0;
        i++;
        i = i%101;
        ui->Dtk__Widget__DProgressBar->setValue(i);

        ui->Dtk__Widget__DWaterProgress->setValue(i);
        ui->Dtk__Widget__DWaterProgress->start();
    });
    timer2->setInterval(100);
    timer2->start();

    ui->Dtk__Widget__DLineEdit->setText("请输入内容");

    // 设置 floating buttong 的信号槽
    // 要设置默认的 icon、text
    DFloatingButton *fltButton = ui->Dtk__Widget__DFloatingButton;
    fltButton->setFixedSize(40, 40);                                    // TODO: dtk的代码里面设置了 setFixedSize，这里没办法只能这样重新设置
    fltButton->setIcon(QIcon("/usr/share/icons/bloom/actions/24/lock.svg"));
    connect(fltButton, &DFloatingButton::clicked, []{
        qDebug() << "floating button clicked.";
    });

    // 设置 key seq
    DKeySequenceEdit *keyEditer = ui->Dtk__Widget__DKeySequenceEdit;
    keyEditer->setAlignment(Qt::AlignCenter);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    keyEditer->setKeySequence(QKeySequence(Qt::CTRL | Qt::Key_A));      // 用|号，不要用+号
#else
    keyEditer->setKeySequence(QKeySequence(Qt::CTRL + Qt::Key_A));      // 用+号不要用逗号
#endif
    connect(ui->Dtk__Widget__DKeySequenceEdit, &DKeySequenceEdit::keySequenceChanged, [] (const QKeySequence &seq) {
        qDebug() << "Key sequence: " << seq;
    });

    // 设置 CrubEdit
    DCrumbEdit *crumb = ui->Dtk__Widget__DCrumbEdit;
    crumb->appendCrumb("世界");
    crumb->appendCrumb("个性");
    crumb->appendCrumb("色彩");

    // 设置 DLabel
    // ui->Dtk__Widget__DLabel_2->setText("请输入内容");
    // ui->Dtk__Widget__DLabel_2->setWordWrap(true);

    // 添加 stackwidget
    DStackedWidget *stackWidget = ui->stackedWidget;

    // StackWidget 自带了两个 QWidget，自己再加 4 个 QLabel
    stackWidget->widget(0)->setBackgroundRole(QPalette::Midlight);
    stackWidget->widget(0)->setAutoFillBackground(true);

    stackWidget->widget(1)->setBackgroundRole(QPalette::Shadow);
    stackWidget->widget(1)->setAutoFillBackground(true);

    int n = 4;
    for (int i = 0; i < n; i++) {
        QLabel *label = new QLabel();
        stackWidget->addWidget(label);

        label->setAutoFillBackground(true);
        label->setPalette(QPalette(QPalette::Window, QColor(255, 255 * i / n, 255 - 255 * i / n)));
    }

    // stackWidget->setFocusPolicy(Qt::NoFocus);
    QTimer *timer3 = new QTimer(this->centralWidget());
    connect(timer3, &QTimer::timeout, [this, stackWidget](){
        static int i = 0;
        // stackWidget->widget(i)->setFocusPolicy(Qt::NoFocus);
        stackWidget->setCurrentWidget(stackWidget->widget(i));
        ui->Dtk__Widget__DPageIndicator->setPageCount(stackWidget->count());
        ui->Dtk__Widget__DPageIndicator->setCurrentPage(stackWidget->currentIndex());
        // ui->Dtk__Widget__DPageIndicator->setFocusPolicy(Qt::NoFocus);
        i++;
        i = i % stackWidget->count();
    });
    timer3->setInterval(300);
    // bug: 开启定时器后DCrumbEdit不能输入中文。
    timer3->start();

    ui->Dtk__Widget__DToolButton_2->setIcon(QIcon("/usr/share/icons/bloom/actions/24/unlock.svg"));
    ui->Dtk__Widget__DSpinner->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}
