// SPDX-FileCopyrightText: 2019 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later


#include "dapplicationsettings.h"
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
#include <DObjectPrivate>

DWIDGET_BEGIN_NAMESPACE

class DApplicationSettingsPrivate : public DCORE_NAMESPACE::DObjectPrivate
{
public:
    DApplicationSettingsPrivate(DApplicationSettings *qq);
    void _q_onChanged(const QString &key);
    void _q_onPaletteTypeChanged();

    D_DECLARE_PUBLIC(DApplicationSettings)
};

DApplicationSettingsPrivate::DApplicationSettingsPrivate(DApplicationSettings *qq)
    : DObjectPrivate(qq)
{

}

void DApplicationSettingsPrivate::_q_onChanged(const QString &)
{
}

void DApplicationSettingsPrivate::_q_onPaletteTypeChanged()
{
}


/*!
  \class Dtk::Widget::DApplicationSettings
  \inmodule dtkwidget
  \deprecated The feature has been moved to DGuiApplicationHelper,
    We can disable it by setting DGuiApplicationHelper::DontSaveApplicationTheme enum with setAttribute.
  \brief DApplicationSettings保存应用程序的设置.

  DApplicationSettings存储程序的通用性设置的信息，如当前选择的主题
 */

/*!
  \brief DApplicationSettings::DApplicationSettings构造函数.

  \a parent 参数被发送到 QObject 构造函数
 */
DApplicationSettings::DApplicationSettings(QObject *parent)
    : QObject(parent)
    , DObject(*new DApplicationSettingsPrivate(this))
{
}

DWIDGET_END_NAMESPACE

#include "moc_dapplicationsettings.cpp"
#endif
