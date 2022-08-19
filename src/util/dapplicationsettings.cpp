// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later


#include "dtkwidget/util/dapplicationsettings.h"

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
    D_Q(DApplicationSettings);

    const QString &on = qApp->organizationName();
    const QString &name = qApp->applicationName();

    if (on.isEmpty() || name.isEmpty()) {
        qFatal("%s\n", "Must set organizationName & applicationName");
        std::abort();
    }

    if (!QGSettings::isSchemaInstalled("com.deepin.dtk"))
        return;

    genericSettings = new QGSettings("com.deepin.dtk", QString("/dtk/%2/%3/").arg(on, name).toLocal8Bit(), q);
    // 初始化设置
    _q_onChanged(PALETTE_TYPE_KEY);

    q->connect(genericSettings, SIGNAL(changed(const QString &)), q, SLOT(_q_onChanged(const QString &)));
    q->connect(DGuiApplicationHelper::instance(), SIGNAL(paletteTypeChanged(ColorType)),
               q, SLOT(_q_onPaletteTypeChanged()));
#endif
}

void DApplicationSettingsPrivate::_q_onChanged(const QString &key)
{
#ifdef Q_OS_LINUX
    if (key != PALETTE_TYPE_KEY)
        return;

    const QString &palette_type = genericSettings->get(PALETTE_TYPE_KEY).toString();

    if (palette_type == "LightType") {
        DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::LightType);
    } else if (palette_type == "DarkType") {
        DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::DarkType);
    } else if (palette_type == "UnknownType") {
        DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::UnknownType);
    }
#endif
}

void DApplicationSettingsPrivate::_q_onPaletteTypeChanged()
{
#ifdef Q_OS_LINUX
    switch (DGuiApplicationHelper::instance()->paletteType()) {
    case DGuiApplicationHelper::LightType:
        genericSettings->set(PALETTE_TYPE_KEY, "LightType");
        break;
    case DGuiApplicationHelper::DarkType:
        genericSettings->set(PALETTE_TYPE_KEY, "DarkType");
        break;
    default:
        genericSettings->set(PALETTE_TYPE_KEY, "UnknownType");
        break;
    }
#endif
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
