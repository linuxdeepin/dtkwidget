#include "dtickeffect.h"
#include "private/dtickeffect_p.h"

#include <QHBoxLayout>
#include <QPixmap>
#include <QWidget>
#include <QPainter>

DWIDGET_BEGIN_NAMESPACE

DTickEffect::DTickEffect(QWidget *widget, QWidget *parent)
    : QGraphicsEffect(parent)
    , DObject(*new DTickEffectPrivate(this))
{
    D_D(DTickEffect);

    widget->setGraphicsEffect(this);

    d->content = widget;
    d->init();
    setDirection(DTickEffect::LeftToRight);

    setDuration(1000);

    connect(d->runAnimation, &QVariantAnimation::valueChanged, this, &DTickEffect::update);
    connect(d->runAnimation, &QVariantAnimation::finished, this, &DTickEffect::finished);
}

void DTickEffect::draw(QPainter *painter)
{
    D_D(DTickEffect);

    QPoint offset;
    QPixmap pixmap;

    if (sourceIsPixmap())
        pixmap = sourcePixmap(Qt::LogicalCoordinates, &offset, QGraphicsEffect::NoPad);
    else
        pixmap = sourcePixmap(Qt::DeviceCoordinates, &offset, QGraphicsEffect::NoPad);

    QPoint p = d->runAnimation->currentValue().toPoint();

    switch (d->direction) {
    case LeftToRight:
        offset = QPoint(-pixmap.width() + p.x(), p.y());
        break;
    case RightToLeft:
        offset = QPoint(pixmap.width() + p.x(), p.y());
        break;
    case TopToBottom:
        offset = QPoint(p.x(), -pixmap.height() + p.y());
        break;
    case BottomToTop:
        offset = QPoint(p.x(), pixmap.height() + p.y());
        break;
    default:
        break;
    }

    painter->drawPixmap(d->runAnimation->currentValue().toPoint(), pixmap);
    painter->drawPixmap(offset, pixmap);
}

void DTickEffect::play()
{
    D_D(DTickEffect);

    d->runAnimation->start();

    Q_EMIT stateChanged();
}

void DTickEffect::stop()
{
    D_D(DTickEffect);

    d->runAnimation->stop();

    Q_EMIT stateChanged();
}

void DTickEffect::pause()
{
    D_D(DTickEffect);

    d->runAnimation->pause();

    Q_EMIT stateChanged();
}

void DTickEffect::resume()
{
    D_D(DTickEffect);

    d->runAnimation->resume();

    Q_EMIT stateChanged();
}

void DTickEffect::setDirection(DTickEffect::Direction direction)
{
    D_D(DTickEffect);

    if (d->direction == direction)
        return;

    d->direction = direction;

    d->initDirection();
}

void DTickEffect::setDuration(const int duration)
{
    D_D(DTickEffect);

    if (d->duration == duration)
        return;

    d->duration = duration;

    d->runAnimation->setDuration(duration);
}

DTickEffectPrivate::DTickEffectPrivate(DTickEffect *qq)
    : DObjectPrivate(qq)
{
}

DTickEffectPrivate::~DTickEffectPrivate()
{
    runAnimation->deleteLater();
}

void DTickEffectPrivate::init()
{
    runAnimation = new QVariantAnimation;
    runAnimation->setLoopCount(-1);
}

void DTickEffectPrivate::initDirection()
{
    D_Q(DTickEffect);

    switch (direction) {
    case DTickEffect::LeftToRight:
        runAnimation->setStartValue(QPoint(content->x(), content->y()));
        runAnimation->setEndValue(QPoint(content->width(), content->y()));
        break;
    case DTickEffect::RightToLeft:
        runAnimation->setStartValue(QPoint(content->x(), content->y()));
        runAnimation->setEndValue(QPoint(-content->width(), content->y()));
        break;
    case DTickEffect::TopToBottom:
        runAnimation->setStartValue(QPoint(content->x(), content->y()));
        runAnimation->setEndValue(QPoint(content->x(), content->height()));
        break;
    case DTickEffect::BottomToTop:
        runAnimation->setStartValue(QPoint(content->x(), content->y()));
        runAnimation->setEndValue(QPoint(content->x(), -content->height()));
        break;
    default:
        break;
    }

    q->stop();
    q->play();
}

DWIDGET_END_NAMESPACE
