// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dwaterprogress.h"

#include <QtMath>
#include <QTimer>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsDropShadowEffect>
#include <QEvent>
#include <QIcon>
#include <QLinearGradient>

#include <DIconTheme>
#include <DObjectPrivate>
#include <DSvgRenderer>

DCORE_USE_NAMESPACE
DGUI_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

struct Pop {
    Pop(double s, double xs, double ys):
        size(s), xSpeed(xs), ySpeed(ys) {}

    double size;
    double xSpeed;
    double ySpeed;
    double xOffset = 0.0;
    double yOffset = 0.0;
};

class DWaterProgressPrivate: public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    DWaterProgressPrivate(DWaterProgress *parent): DObjectPrivate(parent)
    {
        pops.append(Pop(7, -1.8, 0.6));
        pops.append(Pop(8, 1.2, 1.0));
        pops.append(Pop(11, 0.8, 1.6));
    }

    void resizePixmap(QSize sz);
    void initUI();
    void setValue(int v);
    void paint(QPainter *p);

    QImage waterFrontImage;
    QImage waterBackImage;
    QString progressText;
    QTimer *timer = Q_NULLPTR;
    QList<Pop> pops;

    int     interval = 33;
    int     value = -1;

    double  frontXOffset = 0;
    double  backXOffset = 0;

    bool    textVisible = true;

    D_DECLARE_PUBLIC(DWaterProgress)
};

/*!
@~english
    @class Dtk::Widget::DWaterProgress
    @ingroup progressbar
    @brief Use DWaterProgress to create a new water animation progress.
    @image html dwaterprogress.png
    
    Use DWaterProgress to create an new water animation progress. There is also an optional progress
    text can be set as visible via setTextVisible().
*/

DWaterProgress::DWaterProgress(QWidget *parent) :
    QWidget(parent), DObject(*new DWaterProgressPrivate(this))
{
    D_D(DWaterProgress);
    d->initUI();

    // apply effect
    auto effect = new QGraphicsDropShadowEffect(this);
    effect->setOffset(0, 6);
    effect->setColor(QColor(1, 153, 248, 255 * 5 / 20));
    effect->setBlurRadius(12);
    this->setGraphicsEffect(effect);
}

DWaterProgress::~DWaterProgress()
{

}

/*!
@~english
  @brief get the current progress value
 */
int DWaterProgress::value() const
{
    D_DC(DWaterProgress);
    return d->value;
}

/*!
@~english
  @brief star water animation.
 */
void DWaterProgress::start()
{
    D_DC(DWaterProgress);
    d->timer->start();
}

/*!
@~english
  @brief stop water animation.
 */
void DWaterProgress::stop()
{
    D_DC(DWaterProgress);
    d->timer->stop();
}

/*!
@~english
  @brief set the current progress 
  @param[in] value (range: 0~100)
 */
void DWaterProgress::setValue(int value)
{
    D_D(DWaterProgress);
    if (d->value == value) {
        return;
    }
    d->setValue(value);
    Q_EMIT valueChanged();
}

/*!
@~english
  @brief set progress text is visible or not
  @param[in] visible set the progress text value is visible or not(like 50% when value is 50) 
 */
void DWaterProgress::setTextVisible(bool visible)
{
    D_D(DWaterProgress);
    d->textVisible = visible;
}

void DWaterProgress::paintEvent(QPaintEvent *)
{
    D_D(DWaterProgress);
    QPainter p(this);
    d->paint(&p);
}

void DWaterProgress::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::PaletteChange) {
        D_D(DWaterProgress);
        d->waterBackImage = QImage();
        d->waterFrontImage = QImage();
    }

    return QWidget::changeEvent(e);
}

void DWaterProgressPrivate::resizePixmap(QSize sz)
{
    // resize water;
    auto waterWidth = 500  * sz.width() / 100;
    auto waterHeight = 110  * sz.height() / 100;
    auto waterSize = QSizeF(waterWidth, waterHeight).toSize();

    if (waterFrontImage.size() != waterSize) {
        QIcon renderer = DIconTheme::findQIcon("water_front");
        QImage image(waterWidth, waterHeight, QImage::Format_ARGB32);
        image.fill(Qt::transparent);  // partly transparent red-ish background
        QPainter waterPainter(&image);
        renderer.paint(&waterPainter, image.rect());
        waterFrontImage = image;
    }
    if (waterBackImage.size() != waterSize) {
        QIcon renderer = DIconTheme::findQIcon("water_back");
        QImage image(waterWidth, waterHeight, QImage::Format_ARGB32);
        image.fill(Qt::transparent);  // partly transparent red-ish background
        QPainter waterPainter(&image);
        renderer.paint(&waterPainter, image.rect());
        waterBackImage = image;
    }
}

void DWaterProgressPrivate::initUI()
{
    D_Q(DWaterProgress);
    q->setMinimumSize(100, 100);

    timer = new QTimer(q);
    timer->setInterval(interval);
    resizePixmap(q->size());
    frontXOffset = q->width();
    backXOffset = 0;

    q->connect(timer, &QTimer::timeout, q, [ = ] {
        // interval can not be zero, and limit to 1
        interval = (interval < 1) ? 1 : interval;

        // move 60% per second
        double frontXDeta = 40.0 / (1000.0 / interval);
        // move 90% per second
        double backXDeta = 60.0 / (1000.0 / interval);

        int canvasWidth = static_cast<int>(q->width() * q->devicePixelRatioF());
        frontXOffset -= frontXDeta *canvasWidth / 100;
        backXOffset += backXDeta *canvasWidth / 100;

        if (frontXOffset > canvasWidth)
        {
            frontXOffset = canvasWidth;
        }
        if (frontXOffset < - (waterFrontImage.width() - canvasWidth))
        {
            frontXOffset = canvasWidth;
        }

        if (backXOffset > waterBackImage.width())
        {
            backXOffset = 0;
        }

        // update pop
        // move 25% per second default
        double speed = 25 / (1000.0 / interval) /** 100 / q->height()*/;
        for (auto &pop : pops)
        {
            // yOffset 0 ~ 100;
            pop.yOffset += speed * pop.ySpeed;
            if (pop.yOffset < 0) {
            }
            if (pop.yOffset > value) {
                pop.yOffset = 0;
            }
            pop.xOffset = qSin((pop.yOffset / 100) * 2 * 3.14) * 18 * pop.xSpeed + 50;
        }
        q->update();
    });
}

void DWaterProgressPrivate::setValue(int v)
{
    value = v;
    progressText = QString("%1").arg(v);
}

void DWaterProgressPrivate::paint(QPainter *p)
{
    D_Q(DWaterProgress);
    p->setRenderHint(QPainter::Antialiasing);

    qreal pixelRatio = q->devicePixelRatioF();
    QRectF rect = QRectF(0, 0, q->width() * pixelRatio, q->height() * pixelRatio);
    QSize sz = QSizeF(q->width() * pixelRatio, q->height() * pixelRatio).toSize();

    resizePixmap(sz);

    int yOffset = rect.toRect().topLeft().y() + (100 - value - 10)  * sz.height() / 100;

    // draw water
    QImage waterImage = QImage(sz, QImage::Format_ARGB32_Premultiplied);
    QPainter waterPinter(&waterImage);
    waterPinter.setRenderHint(QPainter::Antialiasing);
    waterPinter.setCompositionMode(QPainter::CompositionMode_Source);

    QPointF pointStart(sz.width() / 2, 0);
    QPointF pointEnd(sz.width() / 2, sz.height());
    QLinearGradient linear(pointStart, pointEnd);
    QColor startColor("#1F08FF");
    startColor.setAlphaF(1);
    QColor endColor("#50FFF7");
    endColor.setAlphaF(0.28);
    linear.setColorAt(0, startColor);
    linear.setColorAt(1, endColor);
    linear.setSpread(QGradient::PadSpread);
    waterPinter.setPen(Qt::NoPen);
    waterPinter.setBrush(linear);
    waterPinter.drawEllipse(waterImage.rect().center(), sz.width() / 2 + 1, sz.height() / 2  + 1);

    waterPinter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    waterPinter.drawImage(static_cast<int>(backXOffset), yOffset, waterBackImage);
    waterPinter.drawImage(static_cast<int>(backXOffset) - waterBackImage.width(), yOffset, waterBackImage);
    waterPinter.drawImage(static_cast<int>(frontXOffset), yOffset, waterFrontImage);
    waterPinter.drawImage(static_cast<int>(frontXOffset) - waterFrontImage.width(), yOffset, waterFrontImage);

    //drwa pop
    if (value > 30) {
        for (auto &pop : pops) {
            QPainterPath popPath;
            popPath.addEllipse(pop.xOffset * sz.width() / 100, (100 - pop.yOffset) * sz.height() / 100,
                               pop.size * sz.width() / 100, pop.size * sz.height() / 100);
            QColor color(255, 255, 255, 255 * 0.3);
            waterPinter.fillPath(popPath, color);
        }
    }

    if (textVisible) {
        auto font = waterPinter.font();

        QRect rectValue;
        QSize fontTextSize;
        int actual_width;
        int actual_height;
        if (progressText == "100") {
            font.setPixelSize(sz.height() * 35 / 100);
            waterPinter.setFont(font);

            fontTextSize = QFontMetrics(font).size(Qt::TextSingleLine | Qt::AlignCenter, progressText);
            int design_width = sz.width() * 60 / 100;
            int design_height = sz.height() * 35 / 100;
            actual_width = qMax(fontTextSize.width(), design_width);
            actual_height = qMax(fontTextSize.height(), design_height);

            rectValue.setWidth(actual_width);
            rectValue.setHeight(actual_height);
            rectValue.moveCenter(rect.center().toPoint());
            waterPinter.setPen(Qt::white);
            waterPinter.drawText(rectValue, Qt::AlignCenter, progressText);

        } else {
            font.setPixelSize(sz.height() * 40 / 100);
            waterPinter.setFont(font);

            QFontMetrics numberFontMetrics(font);
            fontTextSize = numberFontMetrics.size(Qt::TextSingleLine | Qt::AlignCenter, progressText);
            int design_width = sz.width() * 45 / 100;
            int design_height = sz.height() * 40 / 100;
            actual_width = qMax(fontTextSize.width(), design_width);
            actual_height = qMax(fontTextSize.height(), design_height);

            rectValue.setWidth(actual_width);
            rectValue.setHeight(actual_height);
            rectValue.moveCenter(rect.center().toPoint());
            rectValue.moveLeft(rect.left() + rect.width() * 0.45 * 0.5);

            waterPinter.setPen(Qt::white);
            waterPinter.drawText(rectValue, Qt::AlignCenter, progressText);
            font.setPixelSize(font.pixelSize() / 2);
            waterPinter.setFont(font);

            QFontMetrics ratioFontMetrics(font);
            design_height = rect.height() * 20 / 100;
            actual_height = qMax(ratioFontMetrics.height(), design_height);
            int descent_diff = numberFontMetrics.descent() - ratioFontMetrics.descent();

            QRect rectPerent(QPoint(rectValue.right(), rectValue.bottom() - descent_diff - actual_height),
                             QPoint(rectValue.right() + rect.width() * 20 / 100, rectValue.bottom() - descent_diff));

            waterPinter.drawText(rectPerent, Qt::AlignCenter, "%");
        }
    }
    waterPinter.end();

    QPixmap maskPixmap(sz);
    maskPixmap.fill(Qt::transparent);
    QPainterPath path;
    path.addEllipse(QRectF(0, 0, sz.width(), sz.height()));
    QPainter maskPainter(&maskPixmap);
    maskPainter.setRenderHint(QPainter::Antialiasing);
    maskPainter.setPen(QPen(Qt::white, 1));
    maskPainter.fillPath(path, QBrush(Qt::white));

    QPainter::CompositionMode mode = QPainter::CompositionMode_SourceIn;
    QImage contentImage = QImage(sz, QImage::Format_ARGB32_Premultiplied);
    QPainter contentPainter(&contentImage);
    contentPainter.setCompositionMode(QPainter::CompositionMode_Source);
    contentPainter.fillRect(contentImage.rect(), Qt::transparent);
    contentPainter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    contentPainter.drawImage(0, 0, maskPixmap.toImage());
    contentPainter.setCompositionMode(mode);
    contentPainter.drawImage(0, 0, waterImage);
    contentPainter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    contentPainter.end();

    contentImage.setDevicePixelRatio(pixelRatio);
    p->drawImage(q->rect(), contentImage);
}

DWIDGET_END_NAMESPACE
