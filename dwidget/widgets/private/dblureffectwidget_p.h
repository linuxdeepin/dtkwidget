#ifndef DBLUREFFECTWIDGET_P_H
#define DBLUREFFECTWIDGET_P_H

#include "private/dobject_p.h"
#include "dblureffectwidget.h"

DWIDGET_BEGIN_NAMESPACE

class DBlurEffectWidgetPrivate : public DObjectPrivate
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
    QPainterPath maskPath;

    bool isBehindWindowBlendMode() const;

    void addToBlurEffectWidgetHash();
    void removeFromBlurEffectWidgetHash();

    bool updateWindowBlurArea();

    static QMultiHash<QWidget*, const DBlurEffectWidget*> blurEffectWidgetHash;
    static QHash<const DBlurEffectWidget*, QWidget*> windowOfBlurEffectHash;
    static bool updateWindowBlurArea(QWidget *topLevelWidget);

private:
    D_DECLARE_PUBLIC(DBlurEffectWidget)
};

DWIDGET_END_NAMESPACE

#endif // DBLUREFFECTWIDGET_P_H
