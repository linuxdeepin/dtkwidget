// SPDX-FileCopyrightText: 2019 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later


#include "dapplicationsettings.h"

#include <DConfig>
#include <DGuiApplicationHelper>
#include <DObjectPrivate>

#ifdef Q_OS_LINUX
#include <QGSettings/QGSettings>
#endif
#include <QCoreApplication>

#define PALETTE_TYPE_KEY "paletteType"

DGUI_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

class DApplicationSettingsPrivate : public DCORE_NAMESPACE::DObjectPrivate
{
public:
    DApplicationSettingsPrivate(DApplicationSettings *qq);
    void init();
    void _q_onChanged(const QString &key);
    void _q_onPaletteTypeChanged();

#ifdef Q_OS_LINUX
    QGSettings *genericSettings;
#endif

    D_DECLARE_PUBLIC(DApplicationSettings)
};

DApplicationSettingsPrivate::DApplicationSettingsPrivate(DApplicationSettings *qq)
    : DObjectPrivate(qq)
{

}

void DApplicationSettingsPrivate::init()
{
#ifdef Q_OS_LINUX
    const QString &on = qApp->organizationName();
    const QString &name = qApp->applicationName();

    bool pathValid = true;
    if (on.isEmpty() || name.isEmpty()) {
        qErrnoWarning("%s\n", "Must set organizationName & applicationName");
        pathValid = false;
    }

    // use dconfig to save application theme palette type
    DGuiApplicationHelper::instance()->setAutoSaveApplicationTheme();

    const QByteArray appId("org.deepin.dtk.helper");
    const QByteArray restoredKey("restoredGSettings");
    const QByteArray schema("com.deepin.dtk");
    DTK_CORE_NAMESPACE::DConfig config(appId);
    if (!config.isValid())
        return;

    bool hasRestored = config.value(restoredKey).toBool();
    bool validGs = pathValid && QGSettings::isSchemaInstalled(schema);
    // gsettings to dconfig
    if (!hasRestored && validGs) {
        QGSettings genericSettings(schema, QString("/dtk/%2/%3/").arg(on, name).toLocal8Bit());

        const QString &palette_type = genericSettings.get(PALETTE_TYPE_KEY).toString();
        if (palette_type == "LightType") {
            DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::LightType);
        } else if (palette_type == "DarkType") {
            DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::DarkType);
        }

        config.setValue(restoredKey, true);
    }
#endif
}

void DApplicationSettingsPrivate::_q_onChanged(const QString &key)
{

}

void DApplicationSettingsPrivate::_q_onPaletteTypeChanged()
{

}


/*!
  \class Dtk::Widget::DApplicationSettings
  \inmodule dtkwidget
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
    d_func()->init();
}

DWIDGET_END_NAMESPACE

#include "moc_dapplicationsettings.cpp"
