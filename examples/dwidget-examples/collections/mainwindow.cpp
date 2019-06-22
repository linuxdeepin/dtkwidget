/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
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

#include <DApplication>

#include "mainwindow.h"
#include "buttonlisttab.h"
#include "graphicseffecttab.h"
#include "simplelistviewtab.h"
#include "dtoast.h"

#ifndef DTK_NO_MULTIMEDIA
#include "cameraform.h"
#endif

DCORE_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent)
    : DMainWindow(parent)
{
    auto flags = windowFlags() & ~Qt::WindowMaximizeButtonHint;
    flags = flags & ~Qt::WindowMinimizeButtonHint;
    setWindowFlags(flags);
    setAttribute(Qt::WA_TranslucentBackground);

    DThemeManager *themeManager = DThemeManager::instance();

    initTabWidget();

    QVBoxLayout *mainLayout = new QVBoxLayout();

    mainLayout->setMargin(5);
    mainLayout->addWidget(m_mainTab);

    QHBoxLayout *styleLayout = new QHBoxLayout();
    QPushButton *darkButton = new QPushButton("Dark", this);
    QPushButton *lightBUtton = new QPushButton("Light", this);
    QPushButton *enableButtons = new QPushButton("Enable Titlebar ", this);
    QPushButton *disableButtons = new QPushButton("Disable Titlebar", this);
    QPushButton *toggleMinMaxButtons = new QPushButton("Toggle MinMax", this);
    QPushButton *fullscreenButtons = new QPushButton("Fullscreen", this);

    themeManager->setTheme(lightBUtton, "light");

    connect(darkButton, &QPushButton::clicked, [ = ] {
        themeManager->setTheme("dark");
    });
    connect(lightBUtton, &QPushButton::clicked, [ = ] {
        themeManager->setTheme("light");
    });
    connect(enableButtons, &QPushButton::clicked, [ = ] {
        titlebar()->setDisableFlags(Qt::Widget);
    });
    connect(disableButtons, &QPushButton::clicked, [ = ] {
        titlebar()->setDisableFlags(Qt::WindowMinimizeButtonHint
        | Qt::WindowCloseButtonHint
        | Qt::WindowMaximizeButtonHint
        | Qt::WindowSystemMenuHint);
    });
    connect(fullscreenButtons, &QPushButton::clicked, [ = ] {
        if (!isFullScreen())
        {
            showFullScreen();
        } else
        {
            showNormal();
        }
    });

    connect(toggleMinMaxButtons, &QPushButton::clicked, [ = ] {
        auto flags = windowFlags();
        if (flags.testFlag(Qt::WindowMinimizeButtonHint))
        {
            flags  &= ~Qt::WindowMaximizeButtonHint;
            flags  &= ~Qt::WindowMinimizeButtonHint;
        } else
        {
            flags |= Qt::WindowMaximizeButtonHint;
            flags |= Qt::WindowMinimizeButtonHint;
        }
        setWindowFlags(flags);
        show();
    });

    styleLayout->addWidget(darkButton);
    styleLayout->addWidget(lightBUtton);
    styleLayout->addWidget(enableButtons);
    styleLayout->addWidget(disableButtons);
    styleLayout->addWidget(toggleMinMaxButtons);
    styleLayout->addWidget(fullscreenButtons);
    styleLayout->addStretch();

    mainLayout->addLayout(styleLayout);

    QWidget *centralWidget = new QWidget(this);

    centralWidget->setLayout(mainLayout);

    setCentralWidget(centralWidget);

    DTitlebar *titlebar = this->titlebar();

    if (titlebar) {
        titlebar->setMenu(new QMenu(titlebar));
        titlebar->setSeparatorVisible(true);
        titlebar->menu()->addAction("dfm-settings");
        titlebar->menu()->addAction("dt-settings");
        titlebar->menu()->addAction("testmenu1");
        titlebar->menu()->addAction("testmenu2");
        QMenu *menu = titlebar->menu()->addMenu("menu1");
        menu->addAction("menu1->action1");
        menu->addAction("menu1->action2");
        connect(titlebar->menu(), &QMenu::triggered, this, &MainWindow::menuItemInvoked);

        titlebar->setDisableFlags(Qt::WindowMinimizeButtonHint
                                  | Qt::WindowMaximizeButtonHint
                                  | Qt::WindowSystemMenuHint);
        titlebar->setAutoHideOnFullscreen(true);
    }

    auto toast = new DToast(this);
    toast->setText("Successfully close window");
    toast->setIcon(":/images/light/images/window/close_press.svg");
    QTimer::singleShot(1000, [ = ]() {
        toast->pop();
        toast->move((width() - toast->width()) / 2,
                    (height() - toast->height()) / 2);
    });
    QTimer::singleShot(4000, [ = ]() {
        toast->pop();
    });
}

void MainWindow::menuItemInvoked(QAction *action)
{
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
        dsd.updateSettings(settings);
        dsd.exec();
        return;
    }

    QMessageBox::warning(this, "menu clieck",  action->text() + ", was cliecked");
    qDebug() << "click" << action << action->isChecked();
}

void MainWindow::initTabWidget()
{
    m_mainTab = new QTabWidget(this);

    LineTab *lineTab = new LineTab(this);

    BarTab *barTab = new BarTab(this);

    ButtonTab *buttonTab = new ButtonTab(this);

    InputTab *inputTab = new InputTab(this);

    SliderTab *sliderTab = new SliderTab(this);

    IndicatorTab *indicatorTab = new IndicatorTab(this);

    ButtonListTab *buttonListGroupTab = new ButtonListTab(this);

    Segmentedcontrol *segmentedControl = new Segmentedcontrol(this);

    WidgetsTab *widgetsTab = new WidgetsTab(this);

#ifndef DTK_NO_MULTIMEDIA
    CameraForm *cameraform = new CameraForm(this);
#endif

    GraphicsEffectTab *effectTab = new GraphicsEffectTab(this);

    SimpleListViewTab *simplelistviewTab = new SimpleListViewTab(this);

    m_mainTab->addTab(widgetsTab, "Widgets");
    m_mainTab->addTab(effectTab, "GraphicsEffect");
    m_mainTab->addTab(indicatorTab, "Indicator");
    m_mainTab->addTab(lineTab, "Line");
    m_mainTab->addTab(barTab, "Bar");
    m_mainTab->addTab(buttonTab, "Button");
    m_mainTab->addTab(inputTab, "Input");
    m_mainTab->addTab(sliderTab, "Slider");

    m_mainTab->addTab(buttonListGroupTab, "ButtonList");
    m_mainTab->addTab(segmentedControl, "Segmented Control");
#ifndef DTK_NO_MULTIMEDIA
    m_mainTab->addTab(cameraform, "Camera View");
#endif
    m_mainTab->addTab(simplelistviewTab, "SimpleListView");

    m_mainTab->setCurrentIndex(0);

}

MainWindow::~MainWindow()
{

}
