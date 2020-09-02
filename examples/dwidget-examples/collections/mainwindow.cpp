/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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
#include <QPushButton>
#include <QMessageBox>
#include <QMenu>
#include <QFontDatabase>
#include <QTextCodec>
#include <QDebug>
#include <QTemporaryFile>
#include <QScreen>
#include <DDialog>
#include <DStandardItem>

#include "qsettingbackend.h"
#include "dsettingsdialog.h"
#include "dsettingsoption.h"
#include "dsettings.h"

#include "dslider.h"
#include "dthememanager.h"
#include "dtkwidget_global.h"
#include "dswitchbutton.h"
#include "segmentedcontrol.h"
#include "dplatformwindowhandle.h"
#include "dtitlebar.h"
#include <DSettingsWidgetFactory>

#include <DApplication>
#include <DPrintPreviewDialog>
#include "mainwindow.h"
#include "graphicseffecttab.h"
#include "simplelistviewtab.h"
#include "dtoast.h"
#include "buttonexample.h"

#ifndef DTK_NO_MULTIMEDIA
#include "cameraform.h"
#endif

DCORE_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent)
    : DMainWindow(parent)
{
    setMinimumSize(qApp->primaryScreen()->availableSize() / 5 * 3);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(5);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    m_pStackedWidget = new QStackedWidget;
    m_pPrimaryMenuModel = new QStandardItemModel(this);

    m_pPrimaryListView = new DListView(this);
    m_pPrimaryListView->setFixedWidth(200);
    m_pPrimaryListView->setItemSpacing(0);
    m_pPrimaryListView->setItemSize(QSize(200, 50));
    m_pPrimaryListView->setModel(m_pPrimaryMenuModel);

    m_pSubListView = new DListView(this);
    m_pSubListView->setFixedWidth(330);
    m_pSubListView->setItemSpacing(10);
    m_pSubListView->setItemSize(QSize(330, 60));

    mainLayout->addWidget(m_pPrimaryListView);
    mainLayout->addWidget(m_pSubListView);

    mainLayout->addWidget(m_pStackedWidget);

    initListView();
    initModel();

    Q_ASSERT(m_primaryMenu.size() == m_pPrimaryMenuModel->rowCount());

    connect(m_pPrimaryListView, SIGNAL(currentChanged(const QModelIndex &)), this, SLOT(onPrimaryIndexChanged(const QModelIndex &)));
    connect(m_pSubListView, SIGNAL(currentChanged(const QModelIndex &)), this, SLOT(onSubIndexChanged(const QModelIndex &)));

    DTitlebar *titlebar = this->titlebar();

    if (titlebar) {
        titlebar->setMenu(new QMenu(titlebar));
        titlebar->setSeparatorVisible(true);
        titlebar->menu()->addAction("dfm-settings");
        titlebar->menu()->addAction("dt-settings");
        titlebar->menu()->addAction("testPrinter");
        QMenu *menu = titlebar->menu()->addMenu("menu1");
        menu->addAction("menu1->action1");
        menu->addAction("menu1->action2");
        connect(titlebar->menu(), &QMenu::triggered, this, &MainWindow::menuItemInvoked);

        titlebar->setDisableFlags(Qt::WindowMinimizeButtonHint
                                  | Qt::WindowMaximizeButtonHint
                                  | Qt::WindowSystemMenuHint);
        titlebar->setAutoHideOnFullscreen(true);
    }

    DButtonBox *buttonBox = new DButtonBox(titlebar);
    buttonBox->setFixedWidth(220);
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

    titlebar->addWidget(buttonBox);

    //初始化选中主菜单第一项
    m_pPrimaryListView->setCurrentIndex(m_pPrimaryMenuModel->index(0, 0));
}

void MainWindow::menuItemInvoked(QAction *action)
{
    if (action->text() == "testPrinter") {
        DPrintPreviewDialog dialog(this);
        connect(&dialog, &DPrintPreviewDialog::paintRequested,
                this, [=](DPrinter *_printer) {
                    // 此函数内代码为调试打印内容代码，调整较随意！
                    _printer->setFromTo(1, 10);
                    QPainter painter(_printer);
                    bool firstPage = true;
                    for (int page = _printer->fromPage(); page <= _printer->toPage(); ++page) {
                        painter.resetTransform();
                        if (!firstPage)
                            _printer->newPage();
                        qApp->processEvents();

                        // 给出调用方widget界面作为打印内容
                        double xscale = _printer->pageRect().width() / double(this->width());
                        double yscale = _printer->pageRect().height() / double(this->height());
                        double scale = qMin(xscale, yscale);
                        painter.translate(_printer->pageRect().width() / 2.0, _printer->pageRect().height() / 2.0);
                        painter.scale(scale, scale);
                        painter.translate(-this->width() / 2, -this->height() / 2);
                        this->render(&painter);

                        painter.resetTransform();
                        QFont font /*("CESI仿宋-GB2312")*/;
                        font.setPixelSize(16);
                        font = QFont(font, painter.device());
                        QRectF rect = _printer->pageRect();
                        rect = QRectF(0, 0, rect.width(), rect.height());
                        painter.setFont(font);
                        // 画可用页面矩形,提供调试效果参考
                        painter.drawRect(rect);
                        QFontMetricsF fontMetrics(font);
                        QString text = QString("统信软件 第%1页").arg(page);
                        QRectF stringRect = fontMetrics.boundingRect(text);
                        //添加页脚页面信息
                        painter.drawText(QPointF(rect.bottomRight().x() - stringRect.width(), rect.bottomRight().y() - stringRect.height()),
                                         QString("统信软件 第%1页").arg(page));
                        firstPage = false;
                    }
                });
        dialog.exec();
        return;
    }
    if (action->text() == "dfm-settings") {
        QTemporaryFile tmpFile;
        tmpFile.open();
        auto backend = new Dtk::Core::QSettingBackend(tmpFile.fileName());

        auto settings = Dtk::Core::DSettings::fromJsonFile(":/resources/data/dfm-settings.json");
        settings->setBackend(backend);

        DSettingsDialog dsd(this);
        dsd.updateSettings(settings);
        dsd.exec();
        return;
    }

    if (action->text() == "dt-settings") {
        QTemporaryFile tmpFile;
        tmpFile.open();
        auto backend = new Dtk::Core::QSettingBackend(tmpFile.fileName());

        auto settings = Dtk::Core::DSettings::fromJsonFile(":/resources/data/dt-settings.json");
        settings->setBackend(backend);

        QFontDatabase fontDatabase;
        auto fontFamliy = settings->option("base.font.family");
        QMap<QString, QVariant> fontDatas;

        QStringList values = fontDatabase.families();
        QStringList keys;
        for (auto &v : values) {
            keys << v.toLower().trimmed();
        }
        fontDatas.insert("keys", keys);
        fontDatas.insert("values", values);
        fontFamliy->setData("items", fontDatas);

        // or you can set default value by json
        if (fontFamliy->value().toString().isEmpty()) {
            fontFamliy->setValue("droid serif");
        }

        connect(fontFamliy, &DSettingsOption::valueChanged,
        this, [](QVariant value) {
            qDebug() << "fontFamliy change" << value;
        });

        QStringList codings;
        for (auto coding : QTextCodec::availableCodecs()) {
            codings << coding;
        }

        auto encoding = settings->option("advance.encoding.encoding");
        encoding->setData("items", codings);
        encoding->setValue(0);

        DSettingsDialog dsd(this);
        dsd.widgetFactory()->registerWidget("custom-button", [](QObject *obj) -> QWidget * {
            if (DSettingsOption *option = qobject_cast<DSettingsOption *>(obj)) {
                qDebug() << "create custom button:" << option->value();
                QPushButton *button = new QPushButton(option->value().toString());
                return button;
            }

            return nullptr;
        });
        dsd.updateSettings(settings);
        dsd.exec();
        return;
    }

    DDialog dlg("menu clicked", action->text() + ", was clicked");
    dlg.addButton("ok", true, DDialog::ButtonWarning);
    dlg.setIcon(QIcon::fromTheme("dialog-information"));
    dlg.exec();
    qDebug() << "click" << action << action->isChecked();
}

void MainWindow::initListView()
{
    //test菜单放一些测试控件的例子
    registerPage("Test", "Widgets", new WidgetsTab(this));
    registerPage("Test", "GraphicsEffect", new GraphicsEffectTab(this));
    registerPage("Test", "Indicator", new IndicatorTab(this));
    registerPage("Test", "Line", new LineTab(this));
    registerPage("Test", "Bar", new BarTab(this));
    registerPage("Test", "Button", new ButtonTab(this));
    registerPage("Test", "Input", new InputTab(this));
    registerPage("Test", "Slider", new SliderTab(this));
    registerPage("Test", "segmentedControl", new Segmentedcontrol(this));
    registerPage("Test", "SimpleListViewTab", new SimpleListViewTab(this));

    registerPage("Button", "DPushButton", new DPushButtonExample(this));
    registerPage("Button", "DWarningButton", new DWarningButtonExample(this));
    registerPage("Button", "DSuggestButton", new DSuggestButtonExample(this));
    registerPage("Button", "DToolButton", new QLabel("DToolButton"));
    registerPage("Button", "DIconButton", new QLabel("DIconButton"));
    registerPage("Button", "DButtonBox", new QLabel("DButtonBox"));
    registerPage("Button", "DFloatingButton", new QLabel("DFloatingButton"));
    registerPage("Button", "DSwitchButton", new QLabel("DSwitchButton"));
    registerPage("Button", "DCheckButton", new QLabel("DCheckButton"));
    registerPage("Button", "DComboBox", new QLabel("DComboBox"));

    registerPage("Edit", "DSearchEdit", new QLabel("DSearchEdit"));
    registerPage("Edit", "DLineEdit", new QLabel("DLineEdit"));
    registerPage("Edit", "DIpv4LineEdit", new QLabel("DIpv4LineEdit"));
    registerPage("Edit", "DPasswordEdit", new QLabel("DPasswordEdit"));
    registerPage("Edit", "DFileChooserEdit", new QLabel("DFileChooserEdit"));
    registerPage("Edit", "DSpinBox", new QLabel("DSpinBox"));
    registerPage("Edit", "DTextEdit", new QLabel("DTextEdit"));
    registerPage("Edit", "DCrumbTextFormat", new QLabel("DCrumbTextFormat"));
    registerPage("Edit", "DKeySequenceEdit", new QLabel("DKeySequenceEdit"));

    registerPage("Slider", "DSlider", new QLabel("DSlider"));

    registerPage("ListView", "DBackgroundGroup", new QLabel("DBackgroundGroup"));
    registerPage("ListView", "DListView", new QLabel("DListView"));
    registerPage("ListView", "DGroupBox", new QLabel("DGroupBox"));
    registerPage("ListView", "DTreeView", new QLabel("DTreeView"));
    registerPage("ListView", "DColumnView", new QLabel("DColumnView"));

    registerPage("Window", "DTitleBar", new QLabel("DTitleBar"));
    registerPage("Window", "DMainWindow", new QLabel("DMainWindow"));
    registerPage("Window", "DStatusBar", new QLabel("DStatusBar"));
    registerPage("Window", "DTabBar", new QLabel("DTabBar"));

    registerPage("ToolTip", "DToolTip", new QLabel("DToolTip"));
    registerPage("ToolTip", "DArrowRectangle", new QLabel("DArrowRectangle"));
    registerPage("ToolTip", "DSpinner", new QLabel("DSpinner"));

    registerPage("Dialog", "DDialog", new QLabel("DDialog"));
    registerPage("Dialog", "DFileDialog", new QLabel("DSpinner"));
    registerPage("Dialog", "DMessageManager", new QLabel("DMessageManager"));

    registerPage("ProgressBar", "DProgressBar", new QLabel("DProgressBar"));
    registerPage("ProgressBar", "DWaterProgress", new QLabel("DWaterProgress"));

    registerPage("Layout", "DFrame", new QLabel("DFrame"));
    registerPage("Layout", "DSplitter", new QLabel("DWaterProgress"));
    registerPage("Layout", "DVerticalLine", new QLabel("DVerticalLine"));
    registerPage("Layout", "DHorizontalLine", new QLabel("DHorizontalLine"));

    registerPage("ScrollBar", "DScrollBar", new QLabel("DScrollBar"));

    registerPage("RubberBand", "DRubberBand", new QLabel("DRubberBand"));

    registerPage("Widget", "DCalendarWidget", new QLabel("DCalendarWidget"));
    registerPage("Widget", "DTableWidget", new QLabel("DTableWidget"));
}

MainWindow::~MainWindow()
{
    for (auto pItem : m_primaryMenu) {
        delete pItem;
    }
}

void MainWindow::registerPage(const QString &primaryMenuName, const QString &subMenuName, QWidget *pPageWindow)
{
    Q_ASSERT(pPageWindow != nullptr);

    int item_index = -1;

    for (int index = 0; index < m_primaryMenu.size(); ++index) {
        if (m_primaryMenu[index]->m_itemName == primaryMenuName) {
            item_index = index;
            break;
        }
    }

    if (item_index != -1) {
        int sub_item_index = -1;
        for (int index = 0; index < m_primaryMenu[item_index]->m_itemVector.size(); ++index) {
            if (m_primaryMenu[item_index]->m_itemVector[index]->m_itemName == subMenuName) {
                sub_item_index = index;
                qWarning() << "pageWindow has been registered !!!!!!!";
                Q_ASSERT(false);
                break;
            }
        }

        if (sub_item_index == -1) {
            auto pItem = new ItemInfo(subMenuName);
            m_primaryMenu[item_index]->m_itemVector.push_back(pItem);
            pItem->m_pPageWindow = pPageWindow;
        }

        return;
    }

    auto primaryMenuItem = new ItemInfo(primaryMenuName);
    m_primaryMenu.push_back(primaryMenuItem);

    auto subMenuItem = new ItemInfo(subMenuName);
    primaryMenuItem->m_itemVector.push_back(subMenuItem);

    subMenuItem->m_pPageWindow = pPageWindow;
}

void MainWindow::initModel()
{
    for (auto pItem : m_primaryMenu) {
        auto pModelItem = new DStandardItem(pItem->m_itemName);
        pModelItem->setEditable(false);
        m_pPrimaryMenuModel->appendRow(pModelItem);

        for (auto pSubItem : pItem->m_itemVector) {
            m_pStackedWidget->addWidget(pSubItem->m_pPageWindow);
        }
    }
}

void MainWindow::onPrimaryIndexChanged(const QModelIndex & /*modelIndex*/)
{
    QStandardItemModel *pSubModel = new QStandardItemModel(this);
    for (auto pSubMenuItem : m_primaryMenu[m_pPrimaryListView->currentIndex().row()]->m_itemVector) {
        auto pSubModelItem = new DStandardItem(pSubMenuItem->m_itemName);
        pSubModelItem->setEditable(false);
        pSubModel->appendRow(pSubModelItem);
    }
    //之前的model在设置新的model后会自动销毁
    m_pSubListView->setModel(pSubModel);

    //主动刷新页面
    m_pSubListView->setCurrentIndex(pSubModel->index(0, 0));
    QWidget *pPageWindow = m_primaryMenu[m_pPrimaryListView->currentIndex().row()]->m_itemVector[m_pSubListView->currentIndex().row()]->m_pPageWindow;
    Q_ASSERT(pPageWindow != nullptr);
    int currentIndex = m_pStackedWidget->indexOf(pPageWindow);
    m_pStackedWidget->setCurrentIndex(currentIndex);
}

void MainWindow::onSubIndexChanged(const QModelIndex &modelIndex)
{
    if (!modelIndex.isValid()) {
        return;
    }

    int primaryMenuIndex = m_pPrimaryListView->currentIndex().row();
    QWidget *pPageWindow = m_primaryMenu[primaryMenuIndex]->m_itemVector[m_pSubListView->currentIndex().row()]->m_pPageWindow;
    Q_ASSERT(pPageWindow != nullptr);
    int currentIndex = m_pStackedWidget->indexOf(pPageWindow);
    m_pStackedWidget->setCurrentIndex(currentIndex);
}
