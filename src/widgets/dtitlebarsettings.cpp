// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dtitlebarsettings.h"
#include "dtitlebar.h"
#include "private/dtitlebarsettingsimpl.h"
#include <DObjectPrivate>

DWIDGET_BEGIN_NAMESPACE

class DTitlebarSettingsPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    Q_DECLARE_PUBLIC(DTitlebarSettings)

    DTitlebarSettingsPrivate(DTitlebarSettings *qq);

    DTitlebarSettingsImpl* impl;
    DTitlebar *titlebar;
};

DTitlebarSettingsPrivate::DTitlebarSettingsPrivate(DTitlebarSettings *qq)
    : DObjectPrivate(qq)
    , impl(new DTitlebarSettingsImpl())
{
}

DTitlebarSettings::DTitlebarSettings(DTitlebar *titlebar)
    : DObject( *new DTitlebarSettingsPrivate(this))
{
    D_D(DTitlebarSettings);
    d->titlebar = titlebar;
}

bool DTitlebarSettings::initilize(QList<DTitlebarToolBaseInterface *> &tools, const QString &path)
{
    D_D(DTitlebarSettings);
    d->impl->setTools(tools);
    if (!d->impl->load(path)) {
        return false;
    }

    auto titleBarEditPanel = d->impl->toolsView();
    titleBarEditPanel->setParent(d->titlebar->parentWidget());
    titleBarEditPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    d->titlebar->setCustomWidget(titleBarEditPanel);
    return true;
}

QWidget *DTitlebarSettings::toolsEditPanel() const
{
    D_DC(DTitlebarSettings);
    return d->impl->toolsEditPanel();
}

DTitlebarSettingsImpl *DTitlebarSettings::impl()
{
    D_D(DTitlebarSettings);
    return d->impl;
}

DWIDGET_END_NAMESPACE

#include "moc_dtitlebarsettings.cpp"
