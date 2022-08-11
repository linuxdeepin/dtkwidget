// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DABSTRACTDIALOG_P_H
#define DABSTRACTDIALOG_P_H

#include <DAbstractDialog>

#include <DObjectPrivate>

class QLabel;
class QBoxLayout;

DWIDGET_BEGIN_NAMESPACE

class DPlatformWindowHandle;
class DBlurEffectWidget;

class DAbstractDialogPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
protected:
    DAbstractDialogPrivate(DAbstractDialog *qq);

    QRect getParentGeometry() const;

private:
    void init(bool blurIfPossible);

    bool mouseMoved = false;
    /// 记录是否收到过鼠标移动事件.
    bool mousePressed = false;

    QPoint dragPosition;
    DAbstractDialog::DisplayPosition displayPosition = DAbstractDialog::Center;

    DPlatformWindowHandle *handle = Q_NULLPTR;
    DBlurEffectWidget *bgBlurWidget = Q_NULLPTR;

    Q_DECLARE_PUBLIC(DAbstractDialog)
};

DWIDGET_END_NAMESPACE

#endif // DABSTRACTDIALOG_P_H

