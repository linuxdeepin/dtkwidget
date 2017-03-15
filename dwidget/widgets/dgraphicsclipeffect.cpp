#include "dgraphicsclipeffect.h"
#include "private/dobject_p.h"

#include <QPainter>
#include <QDebug>

DWIDGET_BEGIN_NAMESPACE

class DGraphicsClipEffectPrivate : public DObjectPrivate
{
public:
    DGraphicsClipEffectPrivate(DGraphicsClipEffect *qq);

    QPainterPath clipPath;
    QMargins margins;

    Q_DECLARE_PUBLIC(DGraphicsClipEffect)
};

DGraphicsClipEffectPrivate::DGraphicsClipEffectPrivate(DGraphicsClipEffect *qq)
    : DObjectPrivate(qq)
{

}

DGraphicsClipEffect::DGraphicsClipEffect(QObject *parent)
    : QGraphicsEffect(parent)
    , DObject(*new DGraphicsClipEffectPrivate(this))
{

}

QMargins DGraphicsClipEffect::margins() const
{
    D_DC(DGraphicsClipEffect);

    return d->margins;
}

QPainterPath DGraphicsClipEffect::clipPath() const
{
    D_DC(DGraphicsClipEffect);

    return d->clipPath;
}

void DGraphicsClipEffect::setMargins(const QMargins &margins)
{
    D_D(DGraphicsClipEffect);

    if (d->margins == margins)
        return;

    d->margins = margins;
    emit marginsChanged(margins);
}

void DGraphicsClipEffect::setClipPath(const QPainterPath &clipPath)
{
    D_D(DGraphicsClipEffect);

    if (d->clipPath == clipPath)
        return;

    d->clipPath = clipPath;
    emit clipPathChanged(clipPath);
}

void DGraphicsClipEffect::draw(QPainter *painter)
{
    if (!painter->isActive()) {
        qWarning() << "DGraphicsClipEffect::draw: The painter is not active!";

        drawSource(painter);
        return;
    }

    D_D(DGraphicsClipEffect);

    QPoint offset;
    Qt::CoordinateSystem system = sourceIsPixmap() ? Qt::LogicalCoordinates : Qt::DeviceCoordinates;
    QPixmap pixmap = sourcePixmap(system, &offset, QGraphicsEffect::PadToEffectiveBoundingRect);

    if (pixmap.isNull())
        return;

    QPainter pixmapPainter(&pixmap);
    QPainterPath newPath;

    newPath.addRect(pixmap.rect().marginsRemoved(d->margins));
    newPath -= d->clipPath;
    pixmapPainter.setRenderHints(painter->renderHints() | QPainter::Antialiasing);
    pixmapPainter.setCompositionMode(QPainter::CompositionMode_Clear);
    pixmapPainter.fillPath(newPath, Qt::transparent);

    painter->save();

    if (system == Qt::DeviceCoordinates)
        painter->setWorldTransform(QTransform());

    painter->drawPixmap(offset, pixmap);
    painter->restore();
}

DWIDGET_END_NAMESPACE
