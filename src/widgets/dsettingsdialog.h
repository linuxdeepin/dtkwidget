// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <QPointer>
#include <QScopedPointer>

#include <DSettings>

#include <DAbstractDialog>

DWIDGET_BEGIN_NAMESPACE

class DSettingsWidgetFactory;
class DSettingsDialogPrivate;
class LIBDTKWIDGETSHARED_EXPORT DSettingsDialog : public DAbstractDialog
{
    Q_OBJECT
public:
    DSettingsDialog(QWidget *parent = nullptr);
    ~DSettingsDialog();

    DSettingsWidgetFactory* widgetFactory() const;
    bool groupIsVisible(const QString &groupKey) const;
    void setResetVisible(bool visible);
    void scrollToGroup(const QString &groupKey); //需要在对话框 show 以后使用
    void setIcon(const QIcon &icon);

public Q_SLOTS:
    void updateSettings(DTK_CORE_NAMESPACE::DSettings *settings);
    void updateSettings(const QByteArray &translateContext, DTK_CORE_NAMESPACE::DSettings *settings);
    void setGroupVisible(const QString &groupKey, bool visible);

private:
    QScopedPointer<DSettingsDialogPrivate> dd_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(dd_ptr), DSettingsDialog)
};

DWIDGET_END_NAMESPACE
