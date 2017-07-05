#include "dclipeffectwidget.h"
#include "private/dobject_p.h"

#include <QEvent>
#include <QBackingStore>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

#include <qpa/qplatformbackingstore.h>

DWIDGET_BEGIN_NAMESPACE

class DClipEffectWidgetPrivate : public DObjectPrivate
{
public:
    DClipEffectWidgetPrivate(DClipEffectWidget *qq);

    void updateImage();

    QImage image;
    QRect imageGeometry;
    QPainterPath path;
    QMargins margins;

    QWidgetList parentList;

    Q_DECLARE_PUBLIC(DClipEffectWidget)
};

DClipEffectWidgetPrivate::DClipEffectWidgetPrivate(DClipEffectWidget *qq)
    : DObjectPrivate(qq)
{

}

DClipEffectWidget::DClipEffectWidget(QWidget *parent)
    : QWidget(parent)
    , DObject(*new DClipEffectWidgetPrivate(this))
{
    Q_ASSERT(parent);

    setAttribute(Qt::WA_TransparentForMouseEvents);
    setFocusPolicy(Qt::NoFocus);
}

QMargins DClipEffectWidget::margins() const
{
    D_DC(DClipEffectWidget);

    return d->margins;
}

QPainterPath DClipEffectWidget::clipPath() const
{
    D_DC(DClipEffectWidget);

    return d->path;
}

void DClipEffectWidget::setMargins(QMargins margins)
{
    D_D(DClipEffectWidget);

    if (d->margins == margins)
        return;

    d->margins = margins;
    update();

    Q_EMIT marginsChanged(margins);
}

void DClipEffectWidget::setClipPath(const QPainterPath &path)
{
    D_D(DClipEffectWidget);

    if (d->path == path)
        return;

    d->path = path;
    d->image = QImage();

    Q_EMIT clipPathChanged(d->path);

    update();
}

bool DClipEffectWidget::eventFilter(QObject *watched, QEvent *event)
{
    D_D(DClipEffectWidget);

    if (event->type() == QEvent::Move) {
        d->image = QImage();
    }

    if (watched != parent())
        return false;

    if (event->type() == QEvent::Paint) {
        const QPoint &offset = mapTo(window(), QPoint(0, 0));
        const QImage &image = window()->backingStore()->handle()->toImage();

        if (d->image.isNull()) {
            d->imageGeometry = image.rect() & parentWidget()->rect().translated(offset);
            d->image = image.copy(d->imageGeometry);
        } else {
            QPaintEvent *e = static_cast<QPaintEvent*>(event);
            QPainter p;
            const QRect &rect = image.rect() & e->rect().translated(offset);

            p.begin(&d->image);
            p.setCompositionMode(QPainter::CompositionMode_Source);
            p.drawImage(rect.topLeft() - d->imageGeometry.topLeft(), image.copy(rect));
            p.end();
        }
    } else if (event->type() == QEvent::Resize) {
        resize(parentWidget()->size());
    }

    return false;
}

void DClipEffectWidget::paintEvent(QPaintEvent *event)
{
    D_DC(DClipEffectWidget);

    if (d->image.isNull())
        return;

    const QRect &rect = event->rect() & this->rect().marginsRemoved(d->margins);
    const QRect &imageRect = rect.translated(mapTo(window(), QPoint(0, 0)) - d->imageGeometry.topLeft()) & d->imageGeometry;

    if (!imageRect.isValid())
        return;

    QPainter p(this);
    QPainterPath newPath;

    newPath.addRect(this->rect());
    newPath -= d->path;

    p.setRenderHint(QPainter::Antialiasing);
    p.setClipPath(newPath);
    p.setCompositionMode(QPainter::CompositionMode_Source);
    p.drawImage(rect, d->image, imageRect);
}

void DClipEffectWidget::resizeEvent(QResizeEvent *event)
{
    D_D(DClipEffectWidget);

    d->image = QImage();

    QWidget::resizeEvent(event);
}

void DClipEffectWidget::showEvent(QShowEvent *event)
{
    D_D(DClipEffectWidget);

    d->parentList.clear();

    QWidget *pw = parentWidget();

    while (pw && !pw->isTopLevel()) {
        d->parentList << pw;

        pw->installEventFilter(this);
        pw = pw->parentWidget();
    }

    resize(parentWidget()->size());

    QWidget::showEvent(event);
}

void DClipEffectWidget::hideEvent(QHideEvent *event)
{
    D_D(DClipEffectWidget);

    for (QWidget *w : d->parentList)
        w->removeEventFilter(this);

    d->parentList.clear();

    QWidget::hideEvent(event);
}

DWIDGET_END_NAMESPACE
