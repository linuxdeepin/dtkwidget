// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <dtkcore_global.h>
#include <dtkwidget_config.h>

#include <QtCore/QMetaMethod>

#define DWIDGET_NAMESPACE Widget
#define DTK_WIDGET_NAMESPACE DTK_NAMESPACE::Widget

#define DWIDGET_BEGIN_NAMESPACE namespace DTK_NAMESPACE { namespace DWIDGET_NAMESPACE {
#define DWIDGET_END_NAMESPACE }}
#define DWIDGET_USE_NAMESPACE using namespace DTK_WIDGET_NAMESPACE;

#if defined(DTK_STATIC_LIB)
void inline dtk_windget_init_resource()
{
    Q_INIT_RESOURCE(icons);
    Q_INIT_RESOURCE(dui_theme_dark);
    Q_INIT_RESOURCE(dui_theme_light);
    // TODO: use marco create by dtk_build
#if defined(DTK_STATIC_TRANSLATION)
    Q_INIT_RESOURCE(dtkwidget_translations);
#endif
}
#endif

namespace Dtk
{
namespace Widget
{

#if defined(DTK_STATIC_LIB)
#define DWIDGET_INIT_RESOURCE() \
    do { \
        dtk_windget_init_resource(); \
    } while (0)
#endif

}
}

#if defined(DTK_STATIC_LIB)
#  define LIBDTKWIDGETSHARED_EXPORT
#else
#if defined(LIBDTKWIDGET_LIBRARY)
#  define LIBDTKWIDGETSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBDTKWIDGETSHARED_EXPORT Q_DECL_IMPORT
#endif
#endif

#define DTKWIDGET_DECL_DEPRECATED D_DECL_DEPRECATED

#define D_THEME_INIT_WIDGET(className, ...) do{\
        DThemeManager * manager = DThemeManager::instance(); \
        {const QString &sheet = this->styleSheet() + manager->getQssForWidget(#className, this); \
            if (!sheet.isEmpty()) this->setStyleSheet(sheet); \
        } \
        connect(manager, &DThemeManager::themeChanged, this, [this, manager] (QString) { \
            const QString &sheet = manager->getQssForWidget(#className, this); \
            if (!sheet.isEmpty()) this->setStyleSheet(sheet); \
        });\
        connect(manager, &DThemeManager::widgetThemeChanged, this, [this, manager] (QWidget *w, QString) { \
            if (w == this) this->setStyleSheet(manager->getQssForWidget(#className, this)); \
        }); \
        QStringList list = QString(#__VA_ARGS__).replace(" ", "").split(",");\
        const QMetaObject *self = metaObject();\
        Q_FOREACH (const QString &str, list) {\
            if(str.isEmpty())\
                continue;\
            connect(this, self->property(self->indexOfProperty(str.toLatin1().data())).notifySignal(),\
                    manager, manager->metaObject()->method(manager->metaObject()->indexOfMethod("updateQss()")));\
        } \
    } while (0);
