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

#include <QMessageBox>
#include <QMenu>
#include <QTemporaryFile>
#include <QScreen>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextCodec>
#include <QDebug>

#include <DStandardItem>
#include <DTitlebar>
#include <DButtonBox>
#include <DSettings>
#include <DPlatformWindowHandle>
#include <DApplication>
#include <DPrintPreviewDialog>
#include <DSettingsDialog>
#include <DSettingsWidgetFactory>
#include <QFontDatabase>

#include "buttonexample.h"
#include "editexample.h"
#include "sliderexample.h"
#include "menuexample.h"
#include "listviewexample.h"
#include "windowexample.h"
#include "tooltipexample.h"
#include "spinnerexample.h"
#include "dialogexample.h"
#include "progressbarexample.h"
#include "layoutexample.h"
#include "scrollbarexample.h"
#include "rubberbandexample.h"
#include "widgetexample.h"
#include "lcdnumberexample.h"
#include "mainwindow.h"
#include "dsettingsbackend.h"
#include "qsettingbackend.h"
#include "dsettingsdialog.h"
#include "dsettingsoption.h"
#include "dsettings.h"

DCORE_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent)
    : DMainWindow(parent)
{
    setWindowIcon(QIcon(":/images/logo_icon.svg"));
    setMinimumSize(qApp->primaryScreen()->availableSize() / 5 * 3);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(5);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    m_pStackedWidget = new QStackedWidget;
    m_pListViewModel = new QStandardItemModel(this);

    m_pListView = new DListView(this);
    m_pListView->setFixedWidth(200);
    m_pListView->setItemSpacing(0);
    m_pListView->setItemSize(QSize(200, 50));
    m_pListView->setModel(m_pListViewModel);

    mainLayout->addWidget(m_pListView);

    mainLayout->addWidget(m_pStackedWidget);

    initModel();

    connect(m_pListView, SIGNAL(currentChanged(const QModelIndex &)), this, SLOT(onCurrentIndexChanged(const QModelIndex &)));

    DTitlebar *titlebar = this->titlebar();
    titlebar->setIcon(QIcon(":/images/logo_icon.svg"));

    if (titlebar) {
        titlebar->setMenu(new QMenu(titlebar));
        titlebar->setSeparatorVisible(true);
        titlebar->menu()->addAction("dfm-settings");
        titlebar->menu()->addAction("dt-settings");
        titlebar->menu()->addAction("testPrinter");
        QMenu *menu = titlebar->menu()->addMenu("sub-menu");
        connect(menu->addAction("show full screen"), &QAction::triggered, this, [this]() {
            this->isFullScreen() ? this->showNormal() : this->showFullScreen();
            if (QAction *action = qobject_cast<QAction *>(sender())) {
                action->setText(this->isFullScreen() ? "show normal window" : "show full screen");
            }
        });
        connect(menu->addAction("ddialog"), &QAction::triggered, this, []() {
            DDialog dlg("this is title", "this is message text......");
            dlg.addButton("ok", true, DDialog::ButtonWarning);
            dlg.setIcon(QIcon::fromTheme("dialog-information"));
            dlg.exec();
        });
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
    m_pListView->setCurrentIndex(m_pListViewModel->index(0, 0));
}

void MainWindow::menuItemInvoked(QAction *action)
{
    if (action->text() == "testPrinter") {
        DPrintPreviewDialog dialog(this);
        //测试保存PDF文件名称接口
        dialog.setDocName("test");
        dialog.setAsynPreview(8000);
        connect(&dialog, QOverload<DPrinter *, const QVector<int> &>::of(&DPrintPreviewDialog::paintRequested),
                this, [=](DPrinter *_printer, const QVector<int> &v) {
                    // 此函数内代码为调试打印内容代码，调整较随意！
                    _printer->setFromTo(1, 8000);
                    QPainter painter(_printer);
                    bool firstPage = true;
                    for (int page = _printer->fromPage(); page <= _printer->toPage(); ++page) {
                        if (!v.contains(page))
                            continue;

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

    qDebug() << "click" << action << action->isChecked();
}

MainWindow::~MainWindow()
{
}


void MainWindow::initModel()
{
    registerPage("Button", new ButtonExampleWindow(this), QIcon::fromTheme("icon_button"));
    registerPage("Edit", new EditExampleWindow(this), QIcon::fromTheme("icon_edit"));
    registerPage("Slider", new SliderExampleWindow(this), QIcon::fromTheme("icon_slider"));
    registerPage("Menu", new MenuExampleWindow(this), QIcon::fromTheme("icon_menu"));
    registerPage("ListView", new ListViewExampleWindow(this), QIcon::fromTheme("icon_ListView"));
    registerPage("Window", new WindowExampleWindow(this), QIcon::fromTheme("icon_Window"));
    registerPage("ToolTip", new ToolTipExampleWindow(this), QIcon::fromTheme("icon_Tooltip"));
    registerPage("Spinner", new SpinnerExampleWindow(this), QIcon::fromTheme("icon_Spinner"));
    registerPage("Dialog", new DialogExampleWindow(this), QIcon::fromTheme("icon_Dialog"));
    registerPage("ProgressBar", new ProgressBarExampleWindow(this), QIcon::fromTheme("icon_ProgressBar"));
    registerPage("Layout", new LayoutExampleWindow(this), QIcon::fromTheme("icon_Layout"));
    registerPage("ScrollBar", new ScrollBarExampleWindow(this), QIcon::fromTheme("icon_ScrollBar"));
    registerPage("RubberBand", new RubberBandExampleWindow(this), QIcon::fromTheme("icon_RubberBand"));
    registerPage("Widget", new WidgetExampleWindow(this), QIcon::fromTheme("icon_Widget"));
    registerPage("LCDNumber", new LCDNumberExampleWindow(this), QIcon::fromTheme("icon_LCDNumber"));
}

void MainWindow::registerPage(const QString &pageName, PageWindowInterface *pPageWindow, const QIcon &icon)
{
    auto pItem = new DStandardItem(pageName);
    pItem->setIcon(icon);
    pItem->setEditable(false);
    m_pListViewModel->appendRow(pItem);
    m_pStackedWidget->addWidget(pPageWindow);
    pPageWindow->initPageWindow();
}

void MainWindow::onCurrentIndexChanged(const QModelIndex &)
{
    m_pStackedWidget->setCurrentIndex(m_pListView->currentIndex().row());
}
