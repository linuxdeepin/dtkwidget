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

#include "dsettingswidgetfactory.h"

#include "private/settings/content.h"
#include "private/settings/navigation.h"

#include "dthememanager.h"
#include "dwindowclosebutton.h"
#include "dspinbox.h"
#include "dbuttonlist.h"
#include "doptionlist.h"

DWIDGET_BEGIN_NAMESPACE

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

    QFrame          *rightFrame;
    QVBoxLayout     *rightLayout;
    Content         *content;

    DSettingsDialog *q_ptr;
    Q_DECLARE_PUBLIC(DSettingsDialog)
};

DSettingsDialog::DSettingsDialog(QWidget *parent) :
    DAbstractDialog(parent), dd_ptr(new DSettingsDialogPrivate(this))
{
    Q_D(DSettingsDialog);

    DThemeManager::registerWidget(this);

    setObjectName("DSettingsDialog");

    auto layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    d->leftFrame = new Navigation;
    d->leftFrame->layout()->setContentsMargins(0, 30, 0, 0);

    layout->addWidget(d->leftFrame);

    d->rightFrame = new QFrame;
    d->rightFrame->setObjectName("RightFrame");
    d->rightLayout = new QVBoxLayout(d->rightFrame);
    d->rightLayout->setContentsMargins(1, 0, 0, 0);
    d->rightLayout->setSpacing(0);

    d->content = new Content;

    auto closeBt = new DImageButton;
    closeBt->setObjectName("DSettingsDialogClose");
    d->rightLayout->addWidget(closeBt, 0, Qt::AlignVCenter | Qt::AlignRight);
    d->rightLayout->addWidget(d->content);

    layout->addWidget(d->rightFrame);
    setFixedWidth(680);

    connect(d->leftFrame, &Navigation::selectedGroup, d->content, &Content::onScrollToGroup);
    connect(d->content, &Content::scrollToGroup, d->leftFrame, [ = ](const QString & key) {
        d->leftFrame->blockSignals(true);
        d->leftFrame->onSelectGroup(key);
        d->leftFrame->blockSignals(false);
    });
    connect(closeBt, &DWindowCloseButton::clicked, this, &DSettingsDialog::close);
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
 * \~chinese \brief 获取当前对话框使用的空间构造工厂。
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
