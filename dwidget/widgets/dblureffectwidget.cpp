#include "dblureffectwidget.h"
#include "private/dblureffectwidget_p.h"
#include "dplatformwindowhandle.h"

#include <QPainter>
#include <QBackingStore>
#include <QPaintEvent>

#include <qpa/qplatformbackingstore.h>

QT_BEGIN_NAMESPACE
Q_WIDGETS_EXPORT void qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

QMultiHash<QWidget*, const DBlurEffectWidget*> DBlurEffectWidgetPrivate::blurEffectWidgetHash;

DBlurEffectWidgetPrivate::DBlurEffectWidgetPrivate(DBlurEffectWidget *qq)
    : DObjectPrivate(qq)
{

}

bool DBlurEffectWidgetPrivate::isBehindWindowBlendMode() const
{
    D_QC(DBlurEffectWidget);

    return blendMode == DBlurEffectWidget::BehindWindowBlend
            || q->isTopLevel();
}

void DBlurEffectWidgetPrivate::addToBlurEffectWidgetHash()
{
    D_QC(DBlurEffectWidget);

    blurEffectWidgetHash.insertMulti(q->topLevelWidget(), q);
    updateWindowBlurArea(q->topLevelWidget());
}

void DBlurEffectWidgetPrivate::removeFromBlurEffectWidgetHash()
{
    D_QC(DBlurEffectWidget);

    blurEffectWidgetHash.remove(q->topLevelWidget(), q);
    updateWindowBlurArea(q->topLevelWidget());
}

bool DBlurEffectWidgetPrivate::updateWindowBlurArea(QWidget *topLevelWidget)
{
    QList<const DBlurEffectWidget*> blurEffectWidgetList = blurEffectWidgetHash.values(topLevelWidget);
    QVector<DPlatformWindowHandle::WMBlurArea> areaList;

    areaList.reserve(blurEffectWidgetList.size());

    foreach (const DBlurEffectWidget *w, blurEffectWidgetList) {
        if (!w->isVisible())
            continue;

        QRect r = w->rect();

        r.moveTopLeft(w->mapTo(w->topLevelWidget(), r.topLeft()));

        areaList << dMakeWMBlurArea(r.x(), r.y(), r.width(), r.height(), w->blurRectXRadius(), w->blurRectYRadius());
    }

    if (blurEffectWidgetList.isEmpty()) {
        blurEffectWidgetHash.remove(topLevelWidget);
    }

    return DPlatformWindowHandle::setWindowBlurAreaByWM(topLevelWidget, areaList);
}

DBlurEffectWidget::DBlurEffectWidget(QWidget *parent)
    : QWidget(parent)
    , DObject(*new DBlurEffectWidgetPrivate(this))
{
    if (!parent) {
        D_D(DBlurEffectWidget);

        d->addToBlurEffectWidgetHash();
    }
}

DBlurEffectWidget::~DBlurEffectWidget()
{
    D_D(DBlurEffectWidget);

    if (d->isBehindWindowBlendMode())
        d->removeFromBlurEffectWidgetHash();
}

int DBlurEffectWidget::radius() const
{
    D_DC(DBlurEffectWidget);

    return d->radius;
}

DBlurEffectWidget::BlurMode DBlurEffectWidget::mode() const
{
    D_DC(DBlurEffectWidget);

    return d->mode;
}

DBlurEffectWidget::BlendMode DBlurEffectWidget::blendMode() const
{
    D_DC(DBlurEffectWidget);

    return d->blendMode;
}

int DBlurEffectWidget::blurRectXRadius() const
{
    D_DC(DBlurEffectWidget);

    return d->blurRectXRadius;
}

int DBlurEffectWidget::blurRectYRadius() const
{
    D_DC(DBlurEffectWidget);

    return d->blurRectYRadius;
}

void DBlurEffectWidget::setRadius(int radius)
{
    D_D(DBlurEffectWidget);

    if (d->radius == radius)
        return;

    d->radius = radius;

    update();

    emit radiusChanged(radius);
}

void DBlurEffectWidget::setMode(DBlurEffectWidget::BlurMode mode)
{
    D_D(DBlurEffectWidget);

    if (d->mode == mode)
        return;

    d->mode = mode;

    emit modeChanged(mode);
}

void DBlurEffectWidget::setBlendMode(DBlurEffectWidget::BlendMode blendMode)
{
    D_D(DBlurEffectWidget);

    if (d->blendMode == blendMode)
        return;

    if (blendMode == BehindWindowBlend) {
        d->addToBlurEffectWidgetHash();
    } else if (d->blendMode == BehindWindowBlend) {
        d->removeFromBlurEffectWidgetHash();
    }

    d->blendMode = blendMode;
    emit blendModeChanged(blendMode);
}

void DBlurEffectWidget::setBlurRectXRadius(int blurRectXRadius)
{
    D_D(DBlurEffectWidget);

    if (d->blurRectXRadius == blurRectXRadius)
        return;

    d->blurRectXRadius = blurRectXRadius;
    emit blurRectXRadiusChanged(blurRectXRadius);
}

void DBlurEffectWidget::setBlurRectYRadius(int blurRectYRadius)
{
    D_D(DBlurEffectWidget);

    if (d->blurRectYRadius == blurRectYRadius)
        return;

    d->blurRectYRadius = blurRectYRadius;
    emit blurRectYRadiusChanged(blurRectYRadius);
}

DBlurEffectWidget::DBlurEffectWidget(DBlurEffectWidgetPrivate &dd, QWidget *parent)
    : QWidget(parent)
    , DObject(dd)
{

}

void DBlurEffectWidget::paintEvent(QPaintEvent *event)
{
    D_D(DBlurEffectWidget);

    if (d->isBehindWindowBlendMode()) {
        QPainter pa(this);
        QPainterPath path;

        path.addRoundedRect(rect(), d->blurRectXRadius, d->blurRectYRadius);

        pa.setRenderHint(QPainter::Antialiasing);
        pa.setCompositionMode(QPainter::CompositionMode_Clear);
        pa.fillPath(path, Qt::transparent);
        pa.end();

        return;
    }

    int radius = d->radius;
    QPoint point_offset = mapTo(window(), QPoint(0, 0));
    const QRect paintRect = event->rect();

    if (d->sourceImage.isNull()) {
        const QRect &tmp_rect = rect().translated(point_offset);

        d->sourceImage = window()->backingStore()->handle()->toImage().copy(tmp_rect);
    } else {
        QPainter pa_image(&d->sourceImage);

        pa_image.setCompositionMode(QPainter::CompositionMode_Source);

        for (const QRect &rect : event->region().rects()) {
            pa_image.drawImage(rect.topLeft(),
                               window()->backingStore()->handle()->toImage().copy(rect.translated(point_offset)));
        }

        pa_image.end();
    }

    QImage image = d->sourceImage.copy(paintRect.adjusted(-radius, -radius, radius, radius));

    QPainter pa(this);
    pa.translate(paintRect.topLeft() - QPoint(radius, radius));
    qt_blurImage(&pa, image, radius, false, false);
}

void DBlurEffectWidget::moveEvent(QMoveEvent *event)
{
    D_D(DBlurEffectWidget);

    if (!d->isBehindWindowBlendMode())
        return QWidget::moveEvent(event);

    d->updateWindowBlurArea(topLevelWidget());

    QWidget::moveEvent(event);
}

void DBlurEffectWidget::resizeEvent(QResizeEvent *event)
{
    D_D(DBlurEffectWidget);

    d->sourceImage = QImage();

    if (!d->isBehindWindowBlendMode())
        return QWidget::resizeEvent(event);

    d->updateWindowBlurArea(topLevelWidget());

    QWidget::resizeEvent(event);
}

void DBlurEffectWidget::showEvent(QShowEvent *event)
{
    D_D(DBlurEffectWidget);

    if (!d->isBehindWindowBlendMode())
        return QWidget::showEvent(event);

    d->updateWindowBlurArea(topLevelWidget());

    QWidget::showEvent(event);
}

void DBlurEffectWidget::hideEvent(QHideEvent *event)
{
    D_D(DBlurEffectWidget);

    if (!d->isBehindWindowBlendMode())
        return QWidget::hideEvent(event);

    d->updateWindowBlurArea(topLevelWidget());

    QWidget::hideEvent(event);
}

void DBlurEffectWidget::changeEvent(QEvent *event)
{
    D_D(DBlurEffectWidget);

    if (!d->isBehindWindowBlendMode())
        return QWidget::changeEvent(event);

    if (event->type() == QEvent::ParentAboutToChange) {
        d->removeFromBlurEffectWidgetHash();
    } else if (event->type() == QEvent::ParentChange) {
        d->addToBlurEffectWidgetHash();
    }

    QWidget::changeEvent(event);
}

DWIDGET_END_NAMESPACE
