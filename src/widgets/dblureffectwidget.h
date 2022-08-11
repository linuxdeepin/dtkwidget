// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DBLUREFFECTWIDGET_H
#define DBLUREFFECTWIDGET_H

#include <dtkwidget_global.h>
#include <DObject>

#include <QWidget>

DWIDGET_BEGIN_NAMESPACE

class DBlurEffectWidgetPrivate;
class LIBDTKWIDGETSHARED_EXPORT DBlurEffectWidget : public QWidget, public DTK_CORE_NAMESPACE::DObject
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
    Q_PROPERTY(quint8 maskAlpha READ maskAlpha WRITE setMaskAlpha NOTIFY maskAlphaChanged)
    Q_PROPERTY(bool full READ isFull WRITE setFull NOTIFY fullChanged)
    Q_PROPERTY(bool blurEnabled READ blurEnabled WRITE setBlurEnabled NOTIFY blurEnabledChanged)

public:
    // TODO: To support MeanBlur, MedianBlur, BilateralFilter
    enum BlurMode {
        GaussianBlur
    };

    Q_ENUMS(BlurMode)

    enum BlendMode {
        InWindowBlend,
        BehindWindowBlend,
        InWidgetBlend
    };

    Q_ENUMS(BlendMode)

    enum MaskColorType {
        DarkColor,
        LightColor,
        AutoColor,
        CustomColor
    };

    Q_ENUMS(MaskColorType)

    explicit DBlurEffectWidget(QWidget *parent = 0);
    ~DBlurEffectWidget();

    int radius() const;
    BlurMode mode() const;

    BlendMode blendMode() const;
    int blurRectXRadius() const;
    int blurRectYRadius() const;

    bool isFull() const;
    bool blurEnabled() const;

    QColor maskColor() const;

    quint8 maskAlpha() const;

    void setMaskPath(const QPainterPath &path);
    void setSourceImage(const QImage &image, bool autoScale = true);

public Q_SLOTS:
    void setRadius(int radius);
    void setMode(BlurMode mode);

    void setBlendMode(BlendMode blendMode);
    void setBlurRectXRadius(int blurRectXRadius);
    void setBlurRectYRadius(int blurRectYRadius);
    void setMaskAlpha(quint8 alpha);
    void setMaskColor(QColor maskColor);
    void setMaskColor(MaskColorType type);
    void setFull(bool full);
    void setBlurEnabled(bool blurEnabled);

    void updateBlurSourceImage(const QRegion &ren);

Q_SIGNALS:
    void radiusChanged(int radius);
    void modeChanged(BlurMode mode);

    void blendModeChanged(BlendMode blendMode);
    void blurRectXRadiusChanged(int blurRectXRadius);
    void blurRectYRadiusChanged(int blurRectYRadius);
    void maskAlphaChanged(quint8 alpha);
    void maskColorChanged(QColor maskColor);
    void fullChanged(bool full);
    void blurEnabledChanged(bool blurEnabled);

    void blurSourceImageDirtied();

protected:
    DBlurEffectWidget(DBlurEffectWidgetPrivate &dd, QWidget *parent = 0);

    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void moveEvent(QMoveEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;
    void changeEvent(QEvent *event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    D_DECLARE_PRIVATE(DBlurEffectWidget)
    friend class DBlurEffectGroup;
};

class DBlurEffectGroupPrivate;
class DBlurEffectGroup : public DTK_CORE_NAMESPACE::DObject
{
    D_DECLARE_PRIVATE(DBlurEffectGroup)
public:
    explicit DBlurEffectGroup();
    ~DBlurEffectGroup();

    void setSourceImage(QImage image, int blurRadius = 35);
    void addWidget(DBlurEffectWidget *widget, const QPoint &offset = QPoint(0, 0));
    void removeWidget(DBlurEffectWidget *widget);

    void paint(QPainter *pa, DBlurEffectWidget *widget) const;
};

DWIDGET_END_NAMESPACE

#endif // DBLUREFFECTWIDGET_H
