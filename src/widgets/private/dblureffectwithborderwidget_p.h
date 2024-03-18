// SPDX-FileCopyrightText: 2017 - 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
#ifndef DBLUREFFECTWITHBORDERWIDGET_P_H
#define DBLUREFFECTWITHBORDERWIDGET_P_H

#include <DObjectPrivate>
#include <DBlurEffectWithBorderWidget>
#include <DGuiApplicationHelper>

DWIDGET_BEGIN_NAMESPACE

class DBlurEffectWithBorderWidgetPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    DBlurEffectWithBorderWidgetPrivate(DBlurEffectWithBorderWidget *qq);

    int cornerRadius = 0;
    DGUI_NAMESPACE::DGuiApplicationHelper::ColorType themeType = DGUI_NAMESPACE::DGuiApplicationHelper::LightType;

private:
    D_DECLARE_PUBLIC(DBlurEffectWithBorderWidget)
};

DWIDGET_END_NAMESPACE

#endif // DBLUREFFECTWITHBORDERWIDGET_P_H
