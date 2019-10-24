/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#include "dsettingsdialog.h"

#include <QDebug>
#include <QHBoxLayout>

#include <DSettingsOption>
#include <DSettings>
#include <DTitlebar>
#include <DFrame>

#include "dsettingswidgetfactory.h"

#include "private/settings/content.h"
#include "private/settings/navigation.h"

#include "dspinbox.h"
#include "dwindowclosebutton.h"

DWIDGET_BEGIN_NAMESPACE

// see also: dtitlebar.cpp
const int DefaultTitlebarHeight = 40;

/*!
 * \~english \class DSettingsDialog
 * \brief DSettingsDialog provide an common setting ui for deepin style application.
 * It's depend Dtk::Widget::DSettingsWidgetFactory to auot build ui compent from json file.
 * \sa Dtk::Widget::DSettingsWidgetFactory
 * \sa Dtk::Core::DSettings
 */

/*!
 * \~chinese \class DSettingsDialog
 * \brief 为使用DSettings的Dtk程序提供一个通用的设置对话框，这个对话框可以通过json配置文件来自动生成。
 * \sa Dtk::Widget::DSettingsWidgetFactory
 * \sa Dtk::Core::DSettings
 */

class DSettingsDialogPrivate
{
public:
    DSettingsDialogPrivate(DSettingsDialog *parent) : q_ptr(parent) {}

    Navigation      *leftFrame;
    Content         *content;

    DSettingsDialog *q_ptr;
    Q_DECLARE_PUBLIC(DSettingsDialog)
};

DSettingsDialog::DSettingsDialog(QWidget *parent) :
    DAbstractDialog(false, parent), dd_ptr(new DSettingsDialogPrivate(this))
{
    Q_D(DSettingsDialog);

    setObjectName("DSettingsDialog");

    auto mainlayout = new QVBoxLayout(this);
    mainlayout->setContentsMargins(0, 0, 0, 0);
    mainlayout->setSpacing(0);

    d->leftFrame = new Navigation;
    d->content = new Content;

    DFrame *rightFrame = new DFrame;
    rightFrame->setContentsMargins(10, 10, 10, 0);

    QVBoxLayout *rightlayout = new QVBoxLayout(rightFrame);
    DTitlebar *frameBar = new DTitlebar;
    frameBar->setMenuVisible(false);
    frameBar->setTitle(QString());

    d->leftFrame->setObjectName("LeftFrame");
    d->content->setObjectName("RightFrame");

    rightlayout->setMargin(0);
    rightlayout->addWidget(d->content);

    QHBoxLayout *bottomlayout = new QHBoxLayout;
    bottomlayout->addWidget(d->leftFrame);
    bottomlayout->addWidget(rightFrame);
    bottomlayout->setContentsMargins(0, 0, 0, 0);

    mainlayout->addWidget(frameBar);
    mainlayout->addLayout(bottomlayout);

    setFixedWidth(680);

    connect(d->leftFrame, &Navigation::selectedGroup, d->content, &Content::onScrollToGroup);
    connect(d->content, &Content::scrollToGroup, d->leftFrame, [ = ](const QString & key) {
        d->leftFrame->blockSignals(true);
        d->leftFrame->onSelectGroup(key);
        d->leftFrame->blockSignals(false);
    });
}

DSettingsDialog::~DSettingsDialog()
{

}

/*!
 * \~english \brief Return the widget build factory of this dialog.
 * Every instance of DSettingDialog has it own widgetfactory.
 * \return
 */
/*!
 * \~chinese \brief 获取当前对话框使用的控件构造工厂。
 * 每一个设置对话框都有自己的构造工厂实例，这些实例并不会共享数据。
 * \return
 */
DSettingsWidgetFactory *DSettingsDialog::widgetFactory() const
{
    Q_D(const DSettingsDialog);
    return  d->content->widgetFactory();
}

/*!
 * \~english \brief Create all widget for settings options.
 * Warnning that you can only call the once.
 * \param settings Dtk::Core::DSettings object from json
 */
/*!
 * \~chinese \brief 根据settings数据来创建控件，该方法只能调用一次。
 * \param settings 配置文件实例。
 */
void DSettingsDialog::updateSettings(Dtk::Core::DSettings *settings)
{
    // TODO: limit to call once
    updateSettings(QByteArray(), settings);
}

/*!
 * \~english \brief Create all widget for settings options with translate context
 * \param translateContext custom translate data for i18n.
 * \param settings Dtk::Core::DSettings object from json
 * \sa void DSettingsDialog::updateSettings(Dtk::Core::DSettings *settings)
 */
/*!
 * \~chinese \brief 根据settings数据来创建控件，并使用translateContext来进行国际化，该方法只能调用一次。
 * \param translateContext 国际化使用的上下文。
 * \param settings 配置文件实例。
 * \sa void DSettingsDialog::updateSettings(Dtk::Core::DSettings *settings)
 */
void DSettingsDialog::updateSettings(const QByteArray &translateContext, Core::DSettings *settings)
{
    Q_D(DSettingsDialog);
    d->leftFrame->updateSettings(translateContext, settings);
    d->content->updateSettings(translateContext, settings);
    adjustSize();
}

DWIDGET_END_NAMESPACE
