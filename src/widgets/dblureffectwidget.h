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

#ifndef DBLUREFFECTWIDGET_H
#define DBLUREFFECTWIDGET_H

#include "dtkwidget_global.h"
#include "dobject.h"

#include <QWidget>

DWIDGET_BEGIN_NAMESPACE

class DBlurEffectWidgetPrivate;

class DBlurEffectWidget : public QWidget, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    // The "radius" property is only support for InWindowBlend. See property "blendMode"
    Q_PROPERTY(int radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(BlurMode mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(BlendMode blendMode READ blendMode WRITE setBlendMode NOTIFY blendModeChanged)
    Q_PROPERTY(int blurRectXRadius READ blurRectXRadius WRITE setBlurRectXRadius NOTIFY blurRectXRadiusChanged)
    Q_PROPERTY(int blurRectYRadius READ blurRectYRadius WRITE setBlurRectYRadius NOTIFY blurRectYRadiusChanged)
    // ###(zccrs): The alpha channel of the color is fixed.
    // The alpha channel is 102 if the DPlatformWindowHandle::hasBlurWindow() is true, otherwise is 204).
    Q_PROPERTY(QColor maskColor READ maskColor WRITE setMaskColor NOTIFY maskColorChanged)

public:
    // TODO: To support MeanBlur, MedianBlur, BilateralFilter
    /*!
     * \brief The BlurMode enum determines which blur algorithm will be used.
     */
    enum BlurMode {
        GaussianBlur /*!< gaussian blur */
    };

    Q_ENUMS(BlurMode)

    /*!
     * \brief The BlendMode enum controls with which part the widget will be blended with.
     */
    enum BlendMode {
        InWindowBlend, /*!< blend with the widgets between the window and this widget */
        BehindWindowBlend /*!< blend with the pixels behind the window */
    };

    Q_ENUMS(BlendMode)

    /*!
     * \brief The MaskColorType enum contains the possible color used as the background.
     */
    enum MaskColorType {
        DarkColor, /*!< dark style, predefined dark color is used */
        LightColor, /*!< light style, predefined light color is used */
        CustomColor /*!< custom color, custom color is used */
    };

    Q_ENUMS(MaskColorType)

    explicit DBlurEffectWidget(QWidget *parent = 0);
    ~DBlurEffectWidget();

    int radius() const;
    BlurMode mode() const;

    BlendMode blendMode() const;
    int blurRectXRadius() const;
    int blurRectYRadius() const;

    QColor maskColor() const;

    void setMaskPath(const QPainterPath &path);

public Q_SLOTS:
    void setRadius(int radius);
    void setMode(BlurMode mode);

    void setBlendMode(BlendMode blendMode);
    void setBlurRectXRadius(int blurRectXRadius);
    void setBlurRectYRadius(int blurRectYRadius);

    void setMaskColor(QColor maskColor);
    void setMaskColor(MaskColorType type);

Q_SIGNALS:
    void radiusChanged(int radius);
    void modeChanged(BlurMode mode);

    void blendModeChanged(BlendMode blendMode);
    void blurRectXRadiusChanged(int blurRectXRadius);
    void blurRectYRadiusChanged(int blurRectYRadius);

    void maskColorChanged(QColor maskColor);

protected:
    DBlurEffectWidget(DBlurEffectWidgetPrivate &dd, QWidget *parent = 0);

    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void moveEvent(QMoveEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;
    void changeEvent(QEvent *event) Q_DECL_OVERRIDE;

private:
    D_DECLARE_PRIVATE(DBlurEffectWidget)
};

DWIDGET_END_NAMESPACE

#endif // DBLUREFFECTWIDGET_H
