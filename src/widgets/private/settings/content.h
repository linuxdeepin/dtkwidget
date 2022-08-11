// SPDX-FileCopyrightText: 2016 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <QScopedPointer>
#include <QPointer>
#include <QFrame>

#include <dtkwidget_global.h>

DCORE_BEGIN_NAMESPACE
class DSettings;
DCORE_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

class DSettingsWidgetFactory;
class ContentPrivate;
class LIBDTKWIDGETSHARED_EXPORT Content : public QWidget
{
    Q_OBJECT
public:
    explicit Content(QWidget *parent = 0);
    ~Content();

    DSettingsWidgetFactory* widgetFactory() const;
    bool groupIsVisible(const QString &key) const;
    void setGroupVisible(const QString &key, bool visible);

Q_SIGNALS:
    void scrollToGroup(const QString &key);

public Q_SLOTS:
    void onScrollToGroup(const QString &key);
    void updateSettings(const QByteArray &translateContext, QPointer<DTK_CORE_NAMESPACE::DSettings> settings);

private:
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    QScopedPointer<ContentPrivate> d_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(d_ptr), Content)
};

DWIDGET_END_NAMESPACE
