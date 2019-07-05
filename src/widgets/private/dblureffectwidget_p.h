/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef DBLUREFFECTWIDGET_P_H
#define DBLUREFFECTWIDGET_P_H

#include <DObjectPrivate>
#include "dblureffectwidget.h"

DWIDGET_BEGIN_NAMESPACE

class DBlurEffectWidgetPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    DBlurEffectWidgetPrivate(DBlurEffectWidget *qq);

    DBlurEffectWidget::BlurMode mode = DBlurEffectWidget::GaussianBlur;
    QImage sourceImage;
    DBlurEffectWidget::BlendMode blendMode = DBlurEffectWidget::InWindowBlend;

    int radius = 10;
    int blurRectXRadius = 0;
    int blurRectYRadius = 0;
    quint8 maskAlpha = 102;

    bool full = false;
    bool blurEnabled = true;

    QColor maskColor = Qt::transparent;
    DBlurEffectWidget::MaskColorType maskColorType = DBlurEffectWidget::CustomColor;
    QPainterPath maskPath;

    bool isBehindWindowBlendMode() const;
    bool isFull() const;

    void addToBlurEffectWidgetHash();
    void removeFromBlurEffectWidgetHash();

    bool updateWindowBlurArea();
    void setMaskColor(const QColor &color);
    void setMaskAlpha(const quint8 alpha);

    static QMultiHash<QWidget*, const DBlurEffectWidget*> blurEffectWidgetHash;
    static QHash<const DBlurEffectWidget*, QWidget*> windowOfBlurEffectHash;
    static bool updateWindowBlurArea(QWidget *topLevelWidget);

private:
    D_DECLARE_PUBLIC(DBlurEffectWidget)
};

DWIDGET_END_NAMESPACE

#endif // DBLUREFFECTWIDGET_P_H
