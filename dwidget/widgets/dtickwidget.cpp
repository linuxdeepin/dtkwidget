#include "dtickwidget.h"
#include "private/dtickwidget_p.h"

#include <QHBoxLayout>
#include <QPixmap>
#include <QWidget>
#include <QPainter>

DWIDGET_BEGIN_NAMESPACE

DTickWidget::DTickWidget(QWidget *widget, QWidget *parent)
    : QGraphicsEffect(parent)
    , DObject(*new DTickWidgetPrivate(this))
{
    D_D(DTickWidget);

    widget->setGraphicsEffect(this);
    d->content = widget;
    d->init();
    d->initDirection();

    setDuration(1000);

    connect(d->runAnimation, &QVariantAnimation::valueChanged, this, &DTickWidget::update);
    connect(d->runAnimation, &QVariantAnimation::finished, this, &DTickWidget::finished);
}

void DTickWidget::draw(QPainter *painter)
{
    D_D(DTickWidget);

    painter->eraseRect(d->content->rect());

    QPoint offset;
    QPixmap pixmap;

    if (sourceIsPixmap()) {
        pixmap = sourcePixmap(Qt::LogicalCoordinates, &offset);
    } else {
        pixmap = sourcePixmap(Qt::DeviceCoordinates, &offset);
        painter->setWorldTransform(QTransform());
    }

    QPoint p = d->runAnimation->currentValue().toPoint();

    switch (d->direction) {
    case LeftToRight:
        offset = QPoint(pixmap.width() + p.x(), p.y());
        break;
    case RightToLeft:
        offset = QPoint(-pixmap.width() + p.x(), p.y());
        break;
    case TopToBottom:
        offset = QPoint(p.x(), pixmap.height() + p.y());
        break;
    case BottomToTop:
        offset = QPoint(p.x(), -pixmap.height() + p.y());
        break;
    default:
        break;
    }

    painter->drawPixmap(d->runAnimation->currentValue().toPoint(), pixmap);
    painter->drawPixmap(offset, pixmap);
}

void DTickWidget::play()
{
    D_D(DTickWidget);

    d->runAnimation->start();

    Q_EMIT stateChanged();
}

void DTickWidget::stop()
{
    D_D(DTickWidget);

    d->runAnimation->stop();

    Q_EMIT stateChanged();
}

void DTickWidget::pause()
{
    D_D(DTickWidget);

    d->runAnimation->pause();

    Q_EMIT stateChanged();
}

void DTickWidget::resume()
{
    D_D(DTickWidget);

    d->runAnimation->resume();

    Q_EMIT stateChanged();
}

void DTickWidget::setDirection(DTickWidget::Direction direction)
{
    D_D(DTickWidget);

    if (d->direction == direction)
        return;

    d->direction = direction;

    d->initDirection();
}

void DTickWidget::setDuration(const int duration)
{
    D_D(DTickWidget);

    if (d->duration == duration)
        return;

    d->duration = duration;

    d->runAnimation->setDuration(duration);
}

DTickWidgetPrivate::DTickWidgetPrivate(DTickWidget *qq)
    : DObjectPrivate(qq)
{
}

DTickWidgetPrivate::~DTickWidgetPrivate()
{
    runAnimation->deleteLater();
}

void DTickWidgetPrivate::init()
{
    runAnimation = new QVariantAnimation;
    runAnimation->setLoopCount(-1);
}

void DTickWidgetPrivate::initDirection()
{
    D_Q(DTickWidget);

    switch (direction) {
    case DTickWidget::LeftToRight:
        runAnimation->setStartValue(QPoint(content->x(), content->y()));
        runAnimation->setEndValue(QPoint(content->width(), content->y()));
        break;
    case DTickWidget::RightToLeft:
        runAnimation->setStartValue(QPoint(content->x(), content->y()));
        runAnimation->setEndValue(QPoint(-content->width(), content->y()));
        break;
    case DTickWidget::TopToBottom:
        runAnimation->setStartValue(QPoint(content->x(), content->y()));
        runAnimation->setEndValue(QPoint(content->x(), content->height()));
        break;
    case DTickWidget::BottomToTop:
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
