/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef LIBDUI_GLOBAL_H
#define LIBDUI_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QtCore/QMetaMethod>

#if defined(LIBDUI_LIBRARY)
#  define LIBDUISHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBDUISHARED_EXPORT Q_DECL_IMPORT
#endif

#define DUI_DECL_DEPRECATED Q_DECL_DEPRECATED

#define DTK_NAMESPACE Dtk
#define DWIDGET_NAMESPACE Widget

#if !defined(DWIDGET_NAMESPACE)
#   define DWIDGET_NAMESPACE_BEGIN
#   define DWIDGET_NAMESPACE_END
#   define USE_NAMESPACE_DWIDGET
#else
#   define DWIDGET_NAMESPACE_BEGIN namespace DTK_NAMESPACE { namespace DWIDGET_NAMESPACE {
#   define DWIDGET_NAMESPACE_END }}
#   define USE_NAMESPACE_DWIDGET using namespace DTK_NAMESPACE::DWIDGET_NAMESPACE;
#endif

#define D_THEME_INIT_WIDGET(className, propertys...) \
    DThemeManager * manager = DThemeManager::instance(); \
    this->setStyleSheet(this->styleSheet() + manager->getQssForWidget(#className)); \
    connect(manager, &DThemeManager::themeChanged, [=](QString) { \
        this->setStyleSheet(manager->getQssForWidget(#className)); \
    });\
    QStringList list = QString(#propertys).replace(" ", "").split(",");\
    const QMetaObject *self = metaObject();\
    foreach (const QString &str, list) {\
        if(str.isEmpty())\
            continue;\
        connect(this, self->property(self->indexOfProperty(str.toLatin1().data())).notifySignal(),\
        manager, manager->metaObject()->method(manager->metaObject()->indexOfMethod("updateQss()")));\
    }
#endif // LIBDUI_GLOBAL_H
