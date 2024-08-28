// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DBLUREFFECTWIDGET_P_H
#define DBLUREFFECTWIDGET_P_H

#include <QPainterPath>
#include <DObjectPrivate>
#include <DBlurEffectWidget>

DWIDGET_BEGIN_NAMESPACE

class DBlurEffectWidgetPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    DBlurEffectWidgetPrivate(DBlurEffectWidget *qq);

    DBlurEffectWidget::BlurMode mode = DBlurEffectWidget::GaussianBlur;
    QImage sourceImage;
    bool customSourceImage = false;
    bool autoScaleSourceImage = false;
    DBlurEffectWidget::BlendMode blendMode = DBlurEffectWidget::InWindowBlend;

    int radius = 35;
    int blurRectXRadius = 0;
    int blurRectYRadius = 0;
    qint16 maskAlpha = -1;

    bool full = false;
    bool blurEnabled = true;

    QColor maskColor = Qt::transparent;
    DBlurEffectWidget::MaskColorType maskColorType = DBlurEffectWidget::AutoColor;
    QPainterPath maskPath;

    // group
    DBlurEffectGroup *group = nullptr;

    bool isBehindWindowBlendMode() const;
    bool isFull() const;

    void addToBlurEffectWidgetHash();
    void removeFromBlurEffectWidgetHash();

    bool updateWindowBlurArea();
    void setMaskColor(const QColor &color);
    void setMaskAlpha(const quint8 alpha);
    quint8 getMaskColorAlpha() const;
    QColor getMaskColor(const QColor &baseColor) const;

    void resetSourceImage();

    static QMultiHash<QWidget*, const DBlurEffectWidget*> blurEffectWidgetHash;
    static QHash<const DBlurEffectWidget*, QWidget*> windowOfBlurEffectHash;
    static bool updateWindowBlurArea(QWidget *topLevelWidget);

private:
    D_DECLARE_PUBLIC(DBlurEffectWidget)
};

DWIDGET_END_NAMESPACE

#endif // DBLUREFFECTWIDGET_P_H
