// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <dtkwidget_global.h>
#include <DObject>

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DTitlebarToolBaseInterface : public QObject
{
    Q_OBJECT
public:
    explicit DTitlebarToolBaseInterface(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~DTitlebarToolBaseInterface(){}

    virtual QString id() const  = 0;
    virtual QString description() = 0;
    virtual QString iconName() = 0;
};

class LIBDTKWIDGETSHARED_EXPORT DTitleBarToolInterface : public DTitlebarToolBaseInterface {
    Q_OBJECT
public:
    explicit DTitleBarToolInterface(QObject *parent = nullptr) : DTitlebarToolBaseInterface(parent) {}
    virtual ~DTitleBarToolInterface(){}

    virtual QWidget *createView() = 0;
Q_SIGNALS:
    void triggered();
};

class LIBDTKWIDGETSHARED_EXPORT DTitleBarSpacerInterface : public DTitlebarToolBaseInterface {
    Q_OBJECT
public:
    explicit DTitleBarSpacerInterface(QObject *parent = nullptr) : DTitlebarToolBaseInterface(parent) {}
    virtual ~DTitleBarSpacerInterface(){}

    virtual QWidget *createPlaceholderView() = 0;
    virtual int size() const = 0;
};

class DTitlebarSettingsPrivate;
class DTitlebarSettingsImpl;
class DTitlebar;
class LIBDTKWIDGETSHARED_EXPORT DTitlebarSettings : public DCORE_NAMESPACE::DObject
{
public:
    explicit DTitlebarSettings(DTitlebar *titlebar);
    bool initilize(QList<DTitlebarToolBaseInterface *> &tools, const QString &path);

    QWidget *toolsEditPanel() const;

private:
    D_DECLARE_PRIVATE(DTitlebarSettings)
    DTitlebarSettingsImpl *impl();
    friend class DTitlebar;
};

DWIDGET_END_NAMESPACE
