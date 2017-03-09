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
    QPainterPath path;
    QMargins margins;

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

    parent->installEventFilter(this);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setFocusPolicy(Qt::NoFocus);
}

void DClipEffectWidget::setClipPath(const QPainterPath &path)
{
    D_D(DClipEffectWidget);

    if (d->path == path)
        return;

    d->path = path;
    d->image = QImage();

    update();
}

QMargins DClipEffectWidget::margins() const
{
    D_DC(DClipEffectWidget);

    return d->margins;
}

void DClipEffectWidget::setMargins(QMargins margins)
{
    D_D(DClipEffectWidget);

    if (d->margins == margins)
        return;

    d->margins = margins;
    d->image = QImage();

    emit marginsChanged(margins);
}

bool DClipEffectWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched != parent())
        return false;

    D_D(DClipEffectWidget);

    if (event->type() == QEvent::Paint) {
        const QPoint &offset = mapTo(window(), QPoint(0, 0));
        QPainter p;

        if (d->image.isNull()) {
            d->image = window()->backingStore()->handle()->toImage().copy(rect().marginsRemoved(d->margins).translated(offset));
            p.begin(&d->image);
        } else {
            p.begin(&d->image);
            p.drawImage(0, 0, window()->backingStore()->handle()->toImage().copy(static_cast<QPaintEvent*>(event)->rect().translated(offset + QPoint(d->margins.left(), d->margins.top()))));
        }

        p.setCompositionMode(QPainter::CompositionMode_Clear);
        p.fillPath(d->path, Qt::transparent);
    } else if (event->type() == QEvent::Resize) {
        resize(parentWidget()->size());
    }

    return false;
}

void DClipEffectWidget::paintEvent(QPaintEvent *event)
{
    D_DC(DClipEffectWidget);

    QPainter p(this);
    const QRect &rect = event->rect();

    p.drawImage(rect, d->image, rect.translated(-d->margins.left(), -d->margins.top()));
}

void DClipEffectWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    D_D(DClipEffectWidget);

    d->image = QImage();
}

DWIDGET_END_NAMESPACE
