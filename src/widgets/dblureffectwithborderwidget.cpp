// SPDX-FileCopyrightText: 2017 - 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
#include "dblureffectwithborderwidget.h"
#include "private/dblureffectwithborderwidget_p.h"

#include <DPlatformWindowHandle>

#include <QPainter>

const QColor outerBorderColor = QColor(0, 0, 0, static_cast<int>(0.15 * 255));
const QColor innerBorderColor = QColor(255, 255, 255, static_cast<int>(0.2 * 255));

#define BORDER_WIDTH 1
#define ALPHA_OFFSET 10

DWIDGET_BEGIN_NAMESPACE

DBlurEffectWithBorderWidget::DBlurEffectWithBorderWidget(QWidget *parent)
    : DBlurEffectWidget(parent)
{
    D_D(DBlurEffectWithBorderWidget);
    DPlatformWindowHandle *handle = new DPlatformWindowHandle(this, this);

    d->cornerRadius = handle->windowRadius();
    d->themeType= DGuiApplicationHelper::instance()->themeType();

    handle->setBorderWidth(BORDER_WIDTH);

    auto setOuterBorderColor = [this, d, handle]() {
        auto outerBorderNewColor = outerBorderColor;
        if (d->themeType == DGuiApplicationHelper::ColorType::DarkType) {
            outerBorderNewColor.setAlpha(maskAlpha() + ALPHA_OFFSET * 2);
        }

        handle->setBorderColor(outerBorderNewColor);
    };

    connect(this, &DBlurEffectWidget::maskAlphaChanged, [this, setOuterBorderColor]() {
        setOuterBorderColor();
        update();
    });

    connect(handle, &DPlatformWindowHandle::windowRadiusChanged, this, [this, d, handle](){
        if (d->cornerRadius == handle->windowRadius())
            return;

        d->cornerRadius = handle->windowRadius();
        update();
    });

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, [this, d, handle, setOuterBorderColor](DGuiApplicationHelper::ColorType type){
        if (!handle || type == d->themeType)
            return;

        d->themeType = type;
        setOuterBorderColor();
        update();
    });

    setOuterBorderColor();
}

void DBlurEffectWithBorderWidget::paintEvent(QPaintEvent *e)
{
    D_D(DBlurEffectWithBorderWidget);
    DBlurEffectWidget::paintEvent(e);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    QPen pen;
    pen.setWidth(BORDER_WIDTH);

    auto innerBorderNewColor = innerBorderColor;
    if (d->themeType != DGuiApplicationHelper::DarkType) {
        innerBorderNewColor.setAlpha(maskAlpha() + ALPHA_OFFSET);
    }

    pen.setColor(innerBorderNewColor);
    p.setPen(pen);
    p.drawRoundedRect(rect(), d->cornerRadius, d->cornerRadius);
}

DBlurEffectWithBorderWidgetPrivate::DBlurEffectWithBorderWidgetPrivate(DBlurEffectWithBorderWidget *qq)
    : DObjectPrivate(qq)
{

}

DWIDGET_END_NAMESPACE
