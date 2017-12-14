#include "dspinner.h"

#include <QtMath>
#include <QPainter>
#include <QTimer>

#include <DObjectPrivate>

DWIDGET_BEGIN_NAMESPACE

class DSpinnerPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    explicit DSpinnerPrivate(DSpinner *qq);

    QList<QColor> createDefaultIndicatorColorList(QColor color);

    QTimer refreshTimer;

    double indicatorShadowOffset = 10;
    double currentDegree = 0.0;

    QList<QList<QColor>> indicatorColors;

    D_DECLARE_PUBLIC(DSpinner)
};

DSpinnerPrivate::DSpinnerPrivate(DSpinner *qq)
    : DObjectPrivate(qq)
{

}

DSpinner::DSpinner(QWidget *parent) :
    QWidget(parent), DObject(*new DSpinnerPrivate(this))
{
    Q_D(DSpinner);

    d->indicatorColors << d->createDefaultIndicatorColorList(QColor(200, 46, 255));
    d->indicatorColors << d->createDefaultIndicatorColorList(QColor(207, 225, 0));
    d->indicatorColors << d->createDefaultIndicatorColorList(QColor(0, 255, 241));

    d->refreshTimer.setInterval(30);

    connect(&d->refreshTimer, &QTimer::timeout,
    this, [ = ]() {
        d->currentDegree += 14;
        update();
    });
}

DSpinner::~DSpinner()
{

}

bool DSpinner::isPlaying() const
{
    Q_D(const DSpinner);
    return d->refreshTimer.isActive();
}

void DSpinner::start()
{
    Q_D(DSpinner);
    d->refreshTimer.start();
}

void DSpinner::stop()
{
    Q_D(DSpinner);
    d->refreshTimer.stop();
}

void DSpinner::setBackgroundColor(QColor color)
{
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, color);
    setPalette(pal);
}

void DSpinner::paintEvent(QPaintEvent *)
{
    Q_D(DSpinner);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    auto degreeCurrent = d->currentDegree * 1.0;

    auto center = QRectF(rect()).center();
    auto radius = qMin(rect().width(), rect().height()) / 2.0;
    auto indicatorRadius = radius / 2 / 2 * 1.1;
    auto indicatorDegreeDelta = 360 / d->indicatorColors.count();

    for (int i = 0; i <  d->indicatorColors.count(); ++i) {
        auto colors = d->indicatorColors.value(i);
        for (int j = 0; j < colors.count(); ++j) {
            degreeCurrent = d->currentDegree - j * d->indicatorShadowOffset + indicatorDegreeDelta * i;
            auto x = (radius - indicatorRadius) * qCos(qDegreesToRadians(degreeCurrent));
            auto y = (radius - indicatorRadius) * qSin(qDegreesToRadians(degreeCurrent));

            x = center.x() + x;
            y = center.y() + y;
            auto tl = QPointF(x - 1 * indicatorRadius, y - 1 * indicatorRadius);
            QRectF rf(tl.x(), tl.y(), indicatorRadius * 2, indicatorRadius * 2);

            QPainterPath path;
            path.addEllipse(rf);

            painter.fillPath(path, colors.value(j));
        }
    }
}

QList<QColor> DSpinnerPrivate::createDefaultIndicatorColorList(QColor color)
{
    QList<QColor> colors;
    QList<int> opacitys;
    opacitys << 100 << 30 << 15 << 10 << 5 << 4 << 3 << 2 << 1;
    for (int i = 0; i < opacitys.count(); ++i) {
        color.setAlpha(255 * opacitys.value(i) / 100);
        colors << color;
    }
    return colors;
}

DWIDGET_END_NAMESPACE
