/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "dwaterprogress.h"

#include <QtMath>
#include <QTimer>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QEvent>
#include <QIcon>
#include <QLinearGradient>

#include <DObjectPrivate>
#include <DSvgRenderer>

DCORE_USE_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

struct Pop {
    Pop(double s, double xs, double ys):
        size(s), xSpeed(xs), ySpeed(ys) {}

    double size;
    double xSpeed;
    double ySpeed;
    double xOffset;
    double yOffset;
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
    QTimer *timer               = Q_NULLPTR;
    QList<Pop> pops;

    int     interval            = 33;
    int     value;

    double  frontXOffset        = 0;
    double  backXOffset         = 0;

    bool    textVisible         = true;

    D_DECLARE_PUBLIC(DWaterProgress)
};

/*!
    \~chinese \class DWaterProgress
    \~chinese \brief 可以使用 DWaterProgress 类快速创建圆形容器内水位高低指示进度的动画控件。

    \~chinese \image html dwaterprogress.png

    \~chinese 可以使用 DWaterProgress 类快速创建圆形容器内水位高低指示进度的动画控件。并提供了可选的是否显示进度文字的选项。
*/

/*!
    \~english \class DWaterProgress
    \~english \brief Use DWaterProgress to create an new water animation progress.

    \~english \image html dwaterprogress.png

    \~english Use DWaterProgress to create an new water animation progress. There is also an optional progress
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
 * \~english \brief get the current progress value
 * \~chinese \brief 当前进度值
 */
int DWaterProgress::value() const
{
    D_DC(DWaterProgress);
    return d->value;
}

/*!
 * \~english \brief star water animation.
 * \~chinese \brief 开始水位动画
 */
void DWaterProgress::start()
{
    D_DC(DWaterProgress);
    d->timer->start();
}

/*!
 * \~english \brief stop water animation.
 * \~chinese \brief 停止水位动画
 */
void DWaterProgress::stop()
{
    D_DC(DWaterProgress);
    d->timer->stop();
}

/*!
 * \~english \brief set the current progress value (range: 0~100)
 * \~chinese \brief 设置当前进度值，（范围 0~100）
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
 * \~english \brief set progress text visible or not
 * \~chinese \brief 设置是否显示进度
 *
 * \~english set the progress text value(like 50% when value is 50) visible.
 * \~chinese 设置进度文字是否显示，如值为 50 时显示 50% 。
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
        QIcon renderer = QIcon::fromTheme("water_front");
        QImage image(waterWidth, waterHeight, QImage::Format_ARGB32);
        image.fill(Qt::transparent);  // partly transparent red-ish background
        QPainter waterPainter(&image);
        renderer.paint(&waterPainter, image.rect());
        waterFrontImage = image;
    }
    if (waterBackImage.size() != waterSize) {
        QIcon renderer = QIcon::fromTheme("water_back");
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

    value = 0;

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
    waterPinter.drawEllipse(waterImage.rect().center(), sz.width() / 2, sz.height() / 2);

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
        if (progressText == "100") {
            font.setPixelSize(sz.height() * 35 / 100);
            waterPinter.setFont(font);

            rectValue.setWidth(sz.width() * 60 / 100);
            rectValue.setHeight(sz.height() * 35 / 100);
            rectValue.moveCenter(rect.center().toPoint());
            waterPinter.setPen(Qt::white);
            waterPinter.drawText(rectValue, Qt::AlignCenter, progressText);

        } else {
            font.setPixelSize(sz.height() * 40 / 100);
            waterPinter.setFont(font);

            rectValue.setWidth(sz.width() * 45 / 100);
            rectValue.setHeight(sz.height() * 40 / 100);
            rectValue.moveCenter(rect.center().toPoint());
            rectValue.moveLeft(rect.left() + rect.width() * 0.45 * 0.5);

            waterPinter.setPen(Qt::white);
            waterPinter.drawText(rectValue, Qt::AlignCenter, progressText);
            font.setPixelSize(font.pixelSize() / 2);
            waterPinter.setFont(font);
            QRect rectPerent(QPoint(rectValue.right(), rectValue.bottom()  - rect.height() * 20 / 100),
                             QPoint(rectValue.right() + rect.width() * 20 / 100, rectValue.bottom()));

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
