/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DBLUREFFECTWIDGET_P_H
#define DBLUREFFECTWIDGET_P_H

#include <DObjectPrivate>
#include "dblureffectwidget.h"

DWIDGET_BEGIN_NAMESPACE

class DBlurEffectWidgetPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    DBlurEffectWidgetPrivate(DBlurEffectWidget *qq);

    int radius = 10;
    DBlurEffectWidget::BlurMode mode = DBlurEffectWidget::GaussianBlur;
    QImage sourceImage;
    DBlurEffectWidget::BlendMode blendMode = DBlurEffectWidget::InWindowBlend;
    int blurRectXRadius = 0;
    int blurRectYRadius = 0;

    QColor maskColor = Qt::transparent;
    DBlurEffectWidget::MaskColorType maskColorType = DBlurEffectWidget::CustomColor;
    QPainterPath maskPath;

    bool isBehindWindowBlendMode() const;

    void addToBlurEffectWidgetHash();
    void removeFromBlurEffectWidgetHash();

    bool updateWindowBlurArea();
    void setMaskColor(const QColor &color);

    static QMultiHash<QWidget*, const DBlurEffectWidget*> blurEffectWidgetHash;
    static QHash<const DBlurEffectWidget*, QWidget*> windowOfBlurEffectHash;
    static bool updateWindowBlurArea(QWidget *topLevelWidget);

private:
    D_DECLARE_PUBLIC(DBlurEffectWidget)
};

DWIDGET_END_NAMESPACE

#endif // DBLUREFFECTWIDGET_P_H
