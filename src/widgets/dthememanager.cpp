/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QFile>
#include <QWidget>
#include <QStyle>
#include <QStyleFactory>

#include <QDebug>

#include "dthememanager.h"
#include "dapplication.h"
#include "private/dthemehelper.h"

DWIDGET_BEGIN_NAMESPACE

class DThemeManagerPrivate : public DThemeManager
{

};

Q_GLOBAL_STATIC(DThemeManagerPrivate, DThemeManagerStatic)

DThemeManager * DThemeManager::instance()
{
    return DThemeManagerStatic;
}

QString DThemeManager::theme() const
{
    return m_theme;
}

void DThemeManager::setTheme(const QString theme)
{
    if (m_theme != theme) {

        QStyle *style = Q_NULLPTR;

        // TODO: remove this shit in the future.
        // It's just a trick to make all DApplications use dde qt5 styles,
        // if dlight or ddark style is set to default style of dde, those
        // ugly code will no longer needed.
        if (theme == "light") {
            style = QStyleFactory::create("dlight");
            m_theme = theme;
        } else if (theme == "dark") {
            style = QStyleFactory::create("ddark");
            m_theme = theme;
        } else if (theme == "semilight") {
            style = QStyleFactory::create("dsemilight");
            m_theme = "light";
        } else if (theme == "semidark") {
            style = QStyleFactory::create("dsemidark");
            m_theme = "dark";
        }

        if (style)
            qApp->setStyle(style);

        Q_EMIT themeChanged(m_theme);
    }
}

static QString getObjectClassName(const QObject *obj)
{
    const QString &type_name = QString::fromLocal8Bit(obj->metaObject()->className());
    const QStringList &widget_type_list = type_name.split("::");

    return widget_type_list.isEmpty() ? type_name : widget_type_list.last();
}

void DThemeManager::setTheme(QWidget *widget, const QString theme)
{
    Q_ASSERT(widget);

    QStyle *style = Q_NULLPTR;

    // TODO: remove this shit in the future.
    // It's just a trick to make all DApplications use dde qt5 styles,
    // if dlight or ddark style is set to default style of dde, those
    // ugly code will no longer needed.
    if (theme == "light") {
        style = QStyleFactory::create("dlight");
        widget->setProperty("_d_dtk_theme", theme);
    } else if (theme == "dark") {
        style = QStyleFactory::create("ddark");
        widget->setProperty("_d_dtk_theme", theme);
    } else if (theme == "semilight") {
        style = QStyleFactory::create("dsemilight");
        widget->setProperty("_d_dtk_theme", "light");
    } else if (theme == "semidark") {
        style = QStyleFactory::create("dsemidark");
        widget->setProperty("_d_dtk_theme", "dark");
    }

    if (style)
        widget->setStyle(style);

    widget->setStyleSheet(getQssForWidget(getObjectClassName(widget), theme));
}

QString DThemeManager::getQssForWidget(const QString className, const QString &theme)
{
    return const_cast<const DThemeManager*>(this)->getQssForWidget(className, theme);
}

QString DThemeManager::getQssForWidget(const QString className, const QString &theme) const
{
    QString qss;

    QString themeName = theme.isEmpty() ? m_theme : theme;
    QFile themeFile(QString(":/%1/%2.theme").arg(themeName).arg(className));

    if (themeFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qss = themeFile.readAll();

        themeFile.close();
    }

    return qss;
}

QString DThemeManager::getQssForWidget(const QString className, const QWidget *widget) const
{
    Q_ASSERT(widget);

    QString theme;

    do {
        theme = widget->property("_d_dtk_theme").toString();
        if (!theme.isEmpty()) break;
        widget = widget->parentWidget();
    } while(widget);

    return getQssForWidget(className, theme);
}

QString DThemeManager::getQssForWidget(const QWidget *widget) const
{
    return getQssForWidget(getObjectClassName(widget), widget);
}

void DThemeManager::updateQss()
{
    QWidget *w = qobject_cast<QWidget*>(sender());
    if(w){
        w->setStyleSheet(w->styleSheet());
    }
}

DThemeManager::DThemeManager() :
    QObject()
{
    this->setTheme("dark");
}


DWIDGET_END_NAMESPACE
