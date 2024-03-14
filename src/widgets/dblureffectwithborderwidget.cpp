// SPDX-FileCopyrightText: 2017 - 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
#include "dblureffectwithborderwidget.h"
#include "private/dblureffectwithborderwidget_p.h"

#include <DPlatformWindowHandle>

#include <QPainter>

static QColor outerBorderColor = QColor(0, 0, 0, static_cast<int>(0.15 * 255));
static QColor innerBorderColor = QColor(255, 255, 255, static_cast<int>(0.2 * 255));

#define BORDER_WIDTH 1
#define ALPHA_OFFSET 10

DWIDGET_BEGIN_NAMESPACE

DBlurEffectWithBorderWidget::DBlurEffectWithBorderWidget(QWidget *parent)
    : DBlurEffectWidget(parent)
{
    D_D(DBlurEffectWithBorderWidget);
    DPlatformWindowHandle *handle = new DPlatformWindowHandle(this, this);

    if (d->themeType == DGuiApplicationHelper::ColorType::DarkType)
        outerBorderColor.setAlpha(maskAlpha() + ALPHA_OFFSET * 2);

    handle->setBorderWidth(BORDER_WIDTH);
    handle->setBorderColor(outerBorderColor);
    d->cornerRadius = handle->windowRadius();
    d->themeType= DGuiApplicationHelper::instance()->themeType();

    connect(handle, &DPlatformWindowHandle::windowRadiusChanged, this, [this, d, handle](){
        if (d->cornerRadius == handle->windowRadius())
            return;

        d->cornerRadius = handle->windowRadius();
        update();
    });

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, [ this, d, handle ](DGuiApplicationHelper::ColorType type){
        if (!handle || type == d->themeType)
            return;

        if (type == DGuiApplicationHelper::ColorType::DarkType)
            outerBorderColor.setAlpha(maskAlpha() + ALPHA_OFFSET * 2);

        handle->setBorderColor(outerBorderColor);

        d->themeType = type;
        update();
    });

}

void DBlurEffectWithBorderWidget::paintEvent(QPaintEvent *e)
{
    D_D(DBlurEffectWithBorderWidget);
    DBlurEffectWidget::paintEvent(e);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    QPen pen;
    pen.setWidth(BORDER_WIDTH);

    if (d->themeType != DGuiApplicationHelper::DarkType)
        innerBorderColor.setAlpha(maskAlpha() + ALPHA_OFFSET);

    pen.setColor(innerBorderColor);
    p.setPen(pen);
    p.drawRoundedRect(rect(), d->cornerRadius, d->cornerRadius);
}

DBlurEffectWithBorderWidgetPrivate::DBlurEffectWithBorderWidgetPrivate(DBlurEffectWithBorderWidget *qq)
    : DObjectPrivate(qq)
{

}

DWIDGET_END_NAMESPACE
