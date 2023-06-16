// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dsettingsdialog.h"

#include <QDebug>
#include <QHBoxLayout>

#include <DSettingsOption>
#include <DPushButton>
#include <DSettings>
#include <DTitlebar>
#include <DFrame>

#include "dsettingswidgetfactory.h"

#include "private/settings/content.h"
#include "private/settings/navigation.h"

#include "dapplication.h"
#include "dspinbox.h"
#include "dwindowclosebutton.h"

DWIDGET_BEGIN_NAMESPACE

// see also: dtitlebar.cpp
const int DefaultTitlebarHeight = 40;

/*!
  @~english
  \class Dtk::Widget::DSettingsDialog
  \inmodule dtkwidget
  \brief DSettingsDialog provide an common setting ui for deepin style application.
  \details It's depend Dtk::Widget::DSettingsWidgetFactory to auot build ui compent from json file.
  \sa Dtk::Widget::DSettingsWidgetFactory
  \sa Dtk::Core::DSettings
 */

class DSettingsDialogPrivate
{
public:
    DSettingsDialogPrivate(DSettingsDialog *parent) : q_ptr(parent) {}

    Navigation      *leftFrame;
    Content         *content;
    DTitlebar       *frameBar;

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
    rightFrame->setLineWidth(0);
    rightFrame->setContentsMargins(10, 10, 10, 10);
    rightFrame->setAccessibleName("DSettingDialogRightFrame");

    QVBoxLayout *rightlayout = new QVBoxLayout(rightFrame);
    d->frameBar = new DTitlebar;
    d->frameBar->setMenuVisible(false);
    d->frameBar->setTitle(QString());
    d->frameBar->setAccessibleName("DSettingTitleBar");

    d->leftFrame->setObjectName("LeftFrame");
    d->leftFrame->setAccessibleName("DSettingDialogLeftFrame");
    d->content->setObjectName("RightFrame");
    d->content->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    d->content->setAccessibleName("DSettingDialogContentWidget");

    rightlayout->setContentsMargins(0, 0, 0, 0);
    rightlayout->addWidget(d->content);

    QHBoxLayout *bottomlayout = new QHBoxLayout;
    bottomlayout->addWidget(d->leftFrame, 0, Qt::AlignLeft);
    bottomlayout->addWidget(rightFrame);
    bottomlayout->setContentsMargins(0, 0, 0, 0);

    mainlayout->addWidget(d->frameBar);
    mainlayout->addLayout(bottomlayout);

    setMinimumWidth(680);

    connect(d->leftFrame, &Navigation::selectedGroup, d->content, &Content::onScrollToGroup);
    connect(d->content, &Content::scrollToGroup, d->leftFrame, [ = ](const QString & key) {
        d->leftFrame->blockSignals(true);
        d->leftFrame->onSelectGroup(key);
        d->leftFrame->blockSignals(false);
    });

    DApplication *dapp = qobject_cast<DApplication*>(qApp);

    if (dapp) {
        d->content->setAttribute(Qt::WA_ContentsMarginsRespectsSafeArea, false);
        d->content->setProperty("_dtk_NoTopLevelEnabled", true);
        qApp->acclimatizeVirtualKeyboard(d->content);
    }
    // 修复 wayland 下使用 setwindowicon 程序出现死循环
    if (DApplication::isDXcbPlatform()) {
        connect(this, &DSettingsDialog::windowIconChanged, d->frameBar, &DTitlebar::setIcon);
        connect(this, &DSettingsDialog::windowTitleChanged, d->frameBar, &DTitlebar::setTitle);
    }
}

DSettingsDialog::~DSettingsDialog()
{

}

/*!
  @~english
  \brief get the factory of this dialog.
  \details Every instance of DSettingDialog has it own widgetfactory.
  \return Return the factory of this dialog.

 */
DSettingsWidgetFactory *DSettingsDialog::widgetFactory() const
{
    Q_D(const DSettingsDialog);
    return  d->content->widgetFactory();
}

bool DSettingsDialog::groupIsVisible(const QString &groupKey) const
{
    Q_D(const DSettingsDialog);
    return d->content->groupIsVisible(groupKey);
}

/*!
  @~english
  \brief DSettingsDialog::setResetVisible Set the default setting button to display
  \param[in] visible true: show, false: hide
  \note Please call after updateSettings ()
 */
void DSettingsDialog::setResetVisible(bool visible)
{
    D_D(DSettingsDialog);
    DPushButton *btn = d->content->findChild<DPushButton *>("SettingsContentReset");
    if (btn == nullptr)
        return;
    btn->setVisible(visible);

}

/*!
  @~english
  \brief DSettingsDialog::scrollToGroup Turn the dialog to the specified group
  \param[in] groupKey GroupKeys in DSettings and Childgroups
  \note Please call after the dialog show()
 */
void DSettingsDialog::scrollToGroup(const QString &groupKey)
{
    D_D(DSettingsDialog);

    d->leftFrame->onSelectGroup(groupKey);
    d->content->onScrollToGroup(groupKey);
}

/*!
  @~english
  \brief DSettingsDialog::setIcon Set the icon of the title bar
  \param[in] icon the Icon Set
 */
void DSettingsDialog::setIcon(const QIcon &icon)
{
    D_D(DSettingsDialog);

    d->frameBar->setIcon(icon);
}

/*!
  @~english
  \brief Create all widget for settings options, that you can only call the once.
  \param[in] settings Dtk::Core::DSettings object from json
 */
void DSettingsDialog::updateSettings(Dtk::Core::DSettings *settings)
{
    // TODO: limit to call once
    updateSettings(QByteArray(), settings);
}

/*!
  @~english
  \brief Create all widget for settings options with translate context.
  \param[in] translateContext custom translate data for i18n.
  \param[in] settings Dtk::Core::DSettings object from json
  \sa DSettingsDialog::updateSettings(Dtk::Core::DSettings *settings)
 */
void DSettingsDialog::updateSettings(const QByteArray &translateContext, Core::DSettings *settings)
{
    Q_D(DSettingsDialog);
    d->leftFrame->updateSettings(translateContext, settings);
    d->content->updateSettings(translateContext, settings);
    adjustSize();
}

void DSettingsDialog::setGroupVisible(const QString &groupKey, bool visible)
{
    Q_D(DSettingsDialog);
    d->leftFrame->setGroupVisible(groupKey, visible);
    d->content->setGroupVisible(groupKey, visible);
}

DWIDGET_END_NAMESPACE
