/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef DABSTRACTDIALOG_P_H
#define DABSTRACTDIALOG_P_H

#include "dabstractdialog.h"

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

