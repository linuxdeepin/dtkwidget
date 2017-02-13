#ifndef DBLUREFFECTWIDGET_H
#define DBLUREFFECTWIDGET_H

#include "dwidget_global.h"
#include "dobject.h"

#include <QWidget>

DWIDGET_BEGIN_NAMESPACE

class DBlurEffectWidgetPrivate;

class DBlurEffectWidget : public QWidget, public DObject
{
    Q_OBJECT

    // The "radius" property is only support for InWindowBlend. See property "blendMode"
    Q_PROPERTY(int radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(BlurMode mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(BlendMode blendMode READ blendMode WRITE setBlendMode NOTIFY blendModeChanged)
    Q_PROPERTY(int blurRectXRadius READ blurRectXRadius WRITE setBlurRectXRadius NOTIFY blurRectXRadiusChanged)
    Q_PROPERTY(int blurRectYRadius READ blurRectYRadius WRITE setBlurRectYRadius NOTIFY blurRectYRadiusChanged)

public:
    /// TODO: To support MeanBlur, MedianBlur, BilateralFilter
    enum BlurMode {
        GaussianBlur
    };

    Q_ENUMS(BlurMode)

    enum BlendMode {
        InWindowBlend,
        BehindWindowBlend
    };

    Q_ENUMS(BlendMode)

    explicit DBlurEffectWidget(QWidget *parent = 0);
    ~DBlurEffectWidget();

    int radius() const;
    BlurMode mode() const;

    BlendMode blendMode() const;
    int blurRectXRadius() const;
    int blurRectYRadius() const;

public slots:
    void setRadius(int radius);
    void setMode(BlurMode mode);

    void setBlendMode(BlendMode blendMode);
    void setBlurRectXRadius(int blurRectXRadius);
    void setBlurRectYRadius(int blurRectYRadius);

signals:
    void radiusChanged(int radius);
    void modeChanged(BlurMode mode);

    void blendModeChanged(BlendMode blendMode);
    void blurRectXRadiusChanged(int blurRectXRadius);
    void blurRectYRadiusChanged(int blurRectYRadius);

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
