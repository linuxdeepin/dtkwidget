/*
 * Copyright (C) 2016 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <QScopedPointer>
#include <QPointer>
#include <QFrame>

#include "dtkwidget_global.h"

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
