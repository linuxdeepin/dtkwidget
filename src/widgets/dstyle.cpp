/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
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
#include "dstyle.h"
#include "dstyleoption.h"

#include <DGuiApplicationHelper>

#include <QStyleOption>
#include <QTextLayout>
#include <QTextLine>
#include <QPixmapCache>
#include <QGuiApplication>
#include <QAbstractItemView>

#include <qmath.h>
#include <private/qfixed_p.h>
#include <private/qtextengine_p.h>
#include <private/qicon_p.h>

#include <math.h>

QT_BEGIN_NAMESPACE
//extern Q_WIDGETS_EXPORT void qt_blurImage(QImage &blurImage, qreal radius, bool quality, int transposed = 0);
extern Q_WIDGETS_EXPORT void qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);
QT_END_NAMESPACE

DGUI_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

static Qt::TextFormat textFormat = Qt::TextFormat::AutoText;

/*!
 * \~chinese \brief DStyle::adjustColor调整颜色
 * \~chinese \sa DGuiApplicationHelper::adjustColor()
 */
QColor DStyle::adjustColor(const QColor &base,
                           qint8 hueFloat, qint8 saturationFloat, qint8 lightnessFloat,
                           qint8 redFloat, qint8 greenFloat, qint8 blueFloat, qint8 alphaFloat)
{
    return DGuiApplicationHelper::adjustColor(base, hueFloat, saturationFloat, lightnessFloat, redFloat, greenFloat, blueFloat, alphaFloat);
}

/*!
 * \~chinese \brief DStyle::blendColor混合颜色
 * \~chinese \sa DGuiApplicationHelper::blendColor()
 */
QColor DStyle::blendColor(const QColor &substrate, const QColor &superstratum)
{
    return DGuiApplicationHelper::blendColor(substrate, superstratum);
}

/*!
 * \~chinese \brief DStyle::setTooltipTextFormat 设置 tooltip 文本格式
 * \~chinese \sa Qt::TextFormat
 */
void DStyle::setTooltipTextFormat(Qt::TextFormat format)
{
    textFormat = format;
}
/*!
 * \~chinese \brief DStyle::tooltipTextFormat 获取 tooltip 文本格式
 * \~chinese \sa Qt::TextFormat
 */
Qt::TextFormat DStyle::tooltipTextFormat()
{
    return textFormat;
}

void DStyle::setFocusRectVisible(QWidget *widget, bool visible)
{
    widget->setProperty("_d_dtk_noFocusRect", !visible);
}

void DStyle::setFrameRadius(QWidget *widget, int radius)
{
    widget->setProperty("_d_dtk_frameRadius", radius);
}

namespace DDrawUtils {
static QImage dropShadow(const QPixmap &px, qreal radius, const QColor &color)
{
    if (px.isNull())
        return QImage();

    QImage tmp(px.size() + QSize(radius * 2, radius * 2), QImage::Format_ARGB32_Premultiplied);
    tmp.fill(0);
    QPainter tmpPainter(&tmp);
    tmpPainter.setCompositionMode(QPainter::CompositionMode_Source);
    tmpPainter.drawPixmap(QPoint(radius, radius), px);
    tmpPainter.end();

    // blur the alpha channel
    QImage blurred(tmp.size(), QImage::Format_ARGB32_Premultiplied);
    blurred.fill(0);
    QPainter blurPainter(&blurred);
    qt_blurImage(&blurPainter, tmp, radius, false, true);
    blurPainter.end();

    if (color == QColor(Qt::black))
        return blurred;

    tmp = blurred;

    // blacken the image...
    tmpPainter.begin(&tmp);
    tmpPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    tmpPainter.fillRect(tmp.rect(), color);
    tmpPainter.end();

    return tmp;
}

static QList<QRect> sudokuByRect(const QRect &rect, QMargins borders)
{
    QList<QRect> list;

//    qreal border_width = borders.left() + borders.right();

//    if ( border_width > rect.width()) {
//        borders.setLeft(borders.left() / border_width * rect.width());
//        borders.setRight(rect.width() - borders.left());
//    }

//    qreal border_height = borders.top() + borders.bottom();

//    if (border_height > rect.height()) {
//        borders.setTop(borders.top()/ border_height * rect.height());
//        borders.setBottom(rect.height() - borders.top());
//    }

    const QRect &contentsRect = rect - borders;

    list << QRect(0, 0, borders.left(), borders.top());
    list << QRect(list.at(0).topRight(), QSize(contentsRect.width(), borders.top())).translated(1, 0);
    list << QRect(list.at(1).topRight(), QSize(borders.right(), borders.top())).translated(1, 0);
    list << QRect(list.at(0).bottomLeft(), QSize(borders.left(), contentsRect.height())).translated(0, 1);
    list << contentsRect;
    list << QRect(contentsRect.topRight(), QSize(borders.right(), contentsRect.height())).translated(1, 0);
    list << QRect(list.at(3).bottomLeft(), QSize(borders.left(), borders.bottom())).translated(0, 1);
    list << QRect(contentsRect.bottomLeft(), QSize(contentsRect.width(), borders.bottom())).translated(0, 1);
    list << QRect(contentsRect.bottomRight(), QSize(borders.left(), borders.bottom())).translated(1, 1);

    return list;
}

static QImage borderImage(const QPixmap &px, const QMargins &borders, const QSize &size, QImage::Format format)
{
    QImage image(size, format);
    QPainter pa(&image);

    const QList<QRect> sudoku_src = sudokuByRect(px.rect(), borders);
    const QList<QRect> sudoku_tar = sudokuByRect(QRect(QPoint(0, 0), size), borders);

    pa.setCompositionMode(QPainter::CompositionMode_Source);  //设置组合模式

    for (int i = 0; i < 9; ++i) {
        pa.drawPixmap(sudoku_tar[i], px, sudoku_src[i]);
    }

    pa.end();

    return image;
}

void drawShadow(QPainter *pa, const QRect &rect, qreal xRadius, qreal yRadius, const QColor &sc, qreal radius, const QPoint &offset)
{
    QPixmap shadow;
    qreal scale = pa->paintEngine()->paintDevice()->devicePixelRatioF();
    QRect shadow_rect = rect;

    shadow_rect.setTopLeft(shadow_rect.topLeft() + offset);

    xRadius *= scale;
    yRadius *= scale;
    radius *= scale;

    const QString &key = QString("dtk-shadow-%1x%2-%3-%4").arg(xRadius).arg(yRadius).arg(sc.name()).arg(radius);

    if (!QPixmapCache::find(key, shadow)) {
        QImage shadow_base(QSize(xRadius * 3, yRadius * 3), QImage::Format_ARGB32_Premultiplied);
        shadow_base.fill(0);
        QPainter pa(&shadow_base);

        pa.setBrush(sc);
        pa.setPen(Qt::NoPen);
        pa.drawRoundedRect(shadow_base.rect(), xRadius, yRadius);
        pa.end();

        shadow_base = dropShadow(QPixmap::fromImage(shadow_base), radius, sc);
        shadow = QPixmap::fromImage(shadow_base);
        QPixmapCache::insert(key, shadow);
    }

    const QMargins margins(xRadius + radius, yRadius + radius, xRadius + radius, yRadius + radius);
    QImage new_shadow = borderImage(shadow, margins, shadow_rect.size() * scale, QImage::Format_ARGB32_Premultiplied);
//    QPainter pa_shadow(&new_shadow);
//    pa_shadow.setCompositionMode(QPainter::CompositionMode_Clear);
//    pa_shadow.setPen(Qt::NoPen);
//    pa_shadow.setBrush(Qt::transparent);
//    pa_shadow.setRenderHint(QPainter::Antialiasing);
//    pa_shadow.drawRoundedRect((new_shadow.rect() - QMargins(radius, radius, radius, radius)).translated(-offset), xRadius, yRadius);
//    pa_shadow.end();
    new_shadow.setDevicePixelRatio(scale);
    pa->drawImage(shadow_rect.topLeft(), new_shadow);
}

void drawShadow(QPainter *pa, const QRect &rect, const QPainterPath &path, const QColor &sc, int radius, const QPoint &offset)
{
    QPixmap shadow;
    qreal scale = pa->paintEngine()->paintDevice()->devicePixelRatioF();
    QRect shadow_rect = rect;

    shadow_rect.setTopLeft(rect.topLeft() + offset);
    radius *= scale;

    QImage shadow_base(shadow_rect.size() * scale, QImage::Format_ARGB32_Premultiplied);
    shadow_base.fill(0);
    shadow_base.setDevicePixelRatio(scale);

    QPainter paTmp(&shadow_base);
    paTmp.setBrush(sc);
    paTmp.setPen(Qt::NoPen);
    paTmp.drawPath(path);
    paTmp.end();
    shadow_base = dropShadow(QPixmap::fromImage(shadow_base), radius, sc);
    shadow = QPixmap::fromImage(shadow_base);
    shadow.setDevicePixelRatio(scale);

    pa->drawPixmap(shadow_rect, shadow);
}

void drawFork(QPainter *pa, const QRectF &rect, const QColor &color, int width)
{
    QPen pen;
    pen.setWidth(width);
    pen.setColor(color);

    pa->setRenderHint(QPainter::Antialiasing, true);
    pa->setPen(pen);
    pa->setBrush(Qt::NoBrush);

    drawForkElement(pa, rect);
}

void drawRoundedRect(QPainter *pa, const QRect &rect, qreal xRadius, qreal yRadius, Corners corners, Qt::SizeMode mode)
{
    QRectF r = rect.normalized();

    if (r.isNull())
        return;

    if (mode == Qt::AbsoluteSize) {
        qreal w = r.width() / 2;
        qreal h = r.height() / 2;

        if (w == 0) {
            xRadius = 0;
        } else {
            xRadius = 100 * qMin(xRadius, w) / w;
        }
        if (h == 0) {
            yRadius = 0;
        } else {
            yRadius = 100 * qMin(yRadius, h) / h;
        }
    } else {
        if (xRadius > 100)                          // fix ranges
            xRadius = 100;

        if (yRadius > 100)
            yRadius = 100;
    }

    if (xRadius <= 0 || yRadius <= 0) {             // add normal rectangle
        pa->drawRect(r);
        return;
    }

    QPainterPath path;
    qreal x = r.x();
    qreal y = r.y();
    qreal w = r.width();
    qreal h = r.height();
    qreal rxx2 = w * xRadius / 100;
    qreal ryy2 = h * yRadius / 100;

    path.arcMoveTo(x, y, rxx2, ryy2, 180);

    if (corners & TopLeftCorner) {
        path.arcTo(x, y, rxx2, ryy2, 180, -90);
    } else {
        path.lineTo(r.topLeft());
    }

    if (corners & TopRightCorner) {
        path.arcTo(x + w - rxx2, y, rxx2, ryy2, 90, -90);
    } else {
        path.lineTo(r.topRight());
    }

    if (corners & BottomRightCorner) {
        path.arcTo(x + w - rxx2, y + h - ryy2, rxx2, ryy2, 0, -90);
    } else {
        path.lineTo(r.bottomRight());
    }

    if (corners & BottomLeftCorner) {
        path.arcTo(x, y + h - ryy2, rxx2, ryy2, 270, -90);
    } else {
        path.lineTo(r.bottomLeft());
    }

    path.closeSubpath();
    pa->drawPath(path);
}

void drawMark(QPainter *pa, const QRectF &rect, const QColor &boxInside, const QColor &boxOutside, const int penWidth, const int outLineLeng)
{
    QPen pen(boxInside);
    pen.setWidth(penWidth);
    pa->setPen(pen);
    pen.setJoinStyle(Qt::RoundJoin);
    pa->setRenderHint(QPainter::Antialiasing, true);

    drawMarkElement(pa, rect);

    if (outLineLeng == 0)
        return;

    double xWide = (rect.width() / 2.0);
    int yHigh = rect.height();
    double length = sqrt(pow(xWide, 2) + pow(yHigh, 2));
    double x = rect.right() + (outLineLeng / length) * xWide;
    double y = rect.y() - (outLineLeng / length) * yHigh;

    pen.setColor(boxOutside);
    pa->setPen(pen);
    pa->drawLine(QPointF(rect.topRight()), QPointF(x, y));
}

void drawBorder(QPainter *pa, const QRectF &rect, const QBrush &brush, int borderWidth, int radius)
{
    pa->setPen(QPen(brush, borderWidth, Qt::SolidLine));
    pa->setBrush(Qt::NoBrush);
    pa->setRenderHint(QPainter::Antialiasing);
    pa->drawRoundedRect(rect.adjusted(1, 1, -1, -1), radius, radius) ;
}

void drawArrow(QPainter *pa, const QRectF &rect, const QColor &color, Qt::ArrowType arrow, int width)
{
    QPen pen;
    pen.setWidth(width);
    pen.setColor(color);
    pa->setRenderHint(QPainter::Antialiasing, true);
    pa->setPen(pen);
    pa->setBrush(Qt::NoBrush);

    drawArrowElement(arrow, pa, rect);
}

void drawPlus(QPainter *painter, const QRectF &rect, const QColor &color, qreal width)
{
    QPen pen = color;
    pen.setWidthF(width);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    drawIncreaseElement(painter, rect);
}

void drawSubtract(QPainter *painter, const QRectF &rect, const QColor &color, qreal width)
{
    QPen pen = color;
    pen.setWidthF(width);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    drawDecreaseElement(painter, rect);
}

void drawForkElement(QPainter *pa, const QRectF &rect)
{
    pa->drawLine(rect.topLeft(), rect.bottomRight());
    pa->drawLine(rect.bottomLeft(), rect.topRight());
}

void drawDecreaseElement(QPainter *pa, const QRectF &rect)
{
    qreal centerY = rect.center().y();

    pa->drawLine(QPointF(rect.left(), centerY), QPointF(rect.right(), centerY));
}

void drawIncreaseElement(QPainter *pa, const QRectF &rect)
{
    qreal centerX = rect.center().x();
    qreal centerY = rect.center().y();

    pa->drawLine(QPointF(rect.x(), centerY), QPointF(rect.right(), centerY));
    pa->drawLine(QPointF(centerX, rect.y()), QPointF(centerX, rect.bottom()));
}

void drawMarkElement(QPainter *pa, const QRectF &rect)
{
    pa->drawLine(rect.x(), rect.center().y(), rect.center().x(), rect.bottom());
    pa->drawLine(rect.center().x(), rect.bottom(), rect.right(), rect.y());
}

void drawSelectElement(QPainter *pa, const QRectF &rect)
{
    qreal radius = rect.width() / 15.0;
    QPointF pointff = rect.center();

    pa->setBrush(QBrush(pa->pen().color()));
    pa->drawEllipse(pointff, radius, radius);
    pa->drawEllipse(pointff - QPointF(radius * 3.5, 0), radius, radius);
    pa->drawEllipse(pointff + QPointF(radius * 3.5, 0), radius, radius);
}

void drawExpandElement(QPainter *pa, const QRectF &rect)
{
    drawArrowUp(pa, rect);
}

void drawReduceElement(QPainter *pa, const QRectF &rect)
{
    drawArrowDown(pa, rect);
}

void drawLockElement(QPainter *pa, const QRectF &rect)
{
    qreal width = rect.width() / 5.0;
    qreal height = rect.height() / 5.0;
    qreal y = rect.y();
    qreal x = rect.x();

    QRectF topRect(x + width, y, width * 3, height * 3);
    QRectF bottomRect(x, y + height * 2, rect.width(), rect.height() - height * 2);
    QPainterPath path;

    path.arcMoveTo(topRect, 0);
    path.arcTo(topRect, 0, 180);

    path.addRect(bottomRect);
    pa->drawPath(path);
}

void drawUnlockElement(QPainter *pa, const QRectF &rect)
{
    qreal width = rect.width() / 5.0;
    qreal height = rect.height() / 5.0;
    qreal y = rect.y();
    qreal x = rect.x();

    QRectF topRect(x + width * 3, y, rect.width() - width * 3, height * 3);
    QRectF bottomRect(x + width, y + height * 2, rect.width() - width * 2, rect.height() - height * 2);
    QPainterPath path;

    path.arcMoveTo(topRect, 0);
    path.arcTo(topRect, 0, 180);
    path.addRect(bottomRect);
    pa->drawPath(path);
}

void drawArrowEnter(QPainter *pa, const QRectF &rect)
{
    drawArrowRight(pa, rect);
}

void drawArrowLeave(QPainter *pa, const QRectF &rect)
{
    drawArrowLeft(pa, rect);
}

void drawArrowNext(QPainter *pa, const QRectF &rect)
{
    QRectF content_rect(rect.x() + rect.width() / 2, rect.y(), rect.width() / 2, rect.height());
    qreal y = rect.center().y();

    drawArrowElement(Qt::RightArrow, pa, content_rect);
    pa->drawLine(rect.x(), y, rect.bottom(), y);
}

void drawArrowPrev(QPainter *pa, const QRectF &rect)
{
    QRectF content_rect(rect.x(), rect.y(), rect.width() / 2, rect.height());
    qreal y = rect.center().y();

    drawArrowElement(Qt::LeftArrow, pa, content_rect);
    pa->drawLine(rect.x(), y, rect.bottom(), y);
}

void drawCloseButton(QPainter *pa, const QRectF &rect)
{
    pa->setRenderHint(QPainter::Antialiasing);
    QPen pen = pa->pen();
    QPen new_pen = pa->pen();
    QColor border_color = pen.color();
    border_color.setAlphaF(0.1);
    new_pen.setColor(border_color);
    pa->setPen(new_pen);
    qreal pen_extent = pen.widthF() / 2;
    pa->drawEllipse(rect.adjusted(pen_extent, pen_extent, -pen_extent, -pen_extent));
    QRectF content_rect(0, 0, rect.width() / 3, rect.height() / 3);
    content_rect.moveCenter(rect.center());
    pa->setPen(pen);
    drawForkElement(pa, content_rect);
}

void drawDeleteButton(QPainter *pa, const QRectF &rect)
{
    const QPen pen = pa->pen();
    pa->setPen(Qt::NoPen);

    QPainterPath path;
    path.addEllipse(rect);

    QRectF hRect(rect.x(), rect.y(), rect.width() / 2, 1);

    hRect.moveCenter(rect.center());

    path.addRect(hRect);

    pa->fillPath(path, QColor("#ff6a6a"));
}

void drawAddButton(QPainter *pa, const QRectF &rect)
{
    const QPen pen = pa->pen();
    pa->setPen(Qt::NoPen);

    QPainterPath path;
    path.addEllipse(rect);

    QRectF hRect(rect.x(), rect.y(), rect.width() / 2, 1);
    QRectF vRect(rect.x(), rect.y(), 1, rect.height() / 2);

    hRect.moveCenter(rect.center());
    vRect.moveCenter(rect.center());

    path.addRect(hRect);
    path.addRect(vRect);

    QRectF center(0, 0, 1, 1);
    center.moveCenter(rect.center());
    path.addEllipse(center);

    pa->fillPath(path, QColor("#48bf00"));
}

void drawTitleBarMenuButton(QPainter *pa, const QRectF &rect)
{
    const QPen pen = pa->pen();
    pa->setPen(Qt::NoPen);
    pa->drawRect(rect);
    QRect content_rect(0, 0, rect.width() / 5, rect.height() / 5);
    content_rect.moveCenter(rect.center().toPoint());
    pa->setPen(pen);

    pa->setRenderHint(QPainter::Antialiasing, pa->device()->devicePixelRatioF() > 1.0);
    pa->drawLine(content_rect.x(), content_rect.y(), content_rect.topRight().x() - 2, content_rect.topRight().y());
    pa->drawLine(content_rect.bottomLeft(), content_rect.bottomRight());

    qreal y = content_rect.center().y();
    pa->drawLine(content_rect.x(), y, content_rect.topRight().x(), y);
}

void drawTitleBarMinButton(QPainter *pa, const QRectF &rect)
{
    const QPen pen = pa->pen();
    pa->setPen(Qt::NoPen);
    pa->drawRect(rect);
    QRect content_rect(0, 0, rect.width() / 5, rect.height() / 5);
    content_rect.moveCenter(rect.center().toPoint());
    pa->setPen(pen);
    pa->setRenderHint(QPainter::Antialiasing, pa->device()->devicePixelRatioF() > 1.0);
    drawDecreaseElement(pa, content_rect);
}

void drawTitleBarMaxButton(QPainter *pa, const QRectF &rect)
{
    const QPen pen = pa->pen();
    pa->setPen(Qt::NoPen);
    pa->drawRect(rect);
    QRect content_rect(0, 0, rect.width() / 5, rect.height() / 6);
    content_rect.moveCenter(rect.center().toPoint());
    pa->setPen(pen);
    pa->setRenderHint(QPainter::Antialiasing, pa->device()->devicePixelRatioF() > 1.0);
    pa->drawRect(content_rect);
}

void drawTitleBarCloseButton(QPainter *pa, const QRectF &rect)
{
    const QPen pen = pa->pen();
    pa->setPen(Qt::NoPen);
    pa->drawRect(rect);
    QRectF content_rect(0, 0, rect.width() / 5, rect.height() / 5);
    content_rect.moveCenter(rect.center());
    pa->setPen(pen);
    pa->setRenderHint(QPainter::Antialiasing, pa->device()->devicePixelRatioF() > 1.0);
    drawForkElement(pa, content_rect);
}

void drawTitleBarNormalButton(QPainter *pa, const QRectF &rect)
{
    const QPen pen = pa->pen();
    pa->setPen(Qt::NoPen);
    pa->drawRect(rect);
    QRect content_rect(0, 0, rect.width() / 5, rect.height() / 5);
    content_rect.moveCenter(rect.center().toPoint());
    pa->setPen(pen);

    pa->setRenderHint(QPainter::Antialiasing, pa->device()->devicePixelRatioF() > 1.0);
    pa->drawRect(content_rect.x(), content_rect.y() + 2, content_rect.width() - 2, content_rect.height() - 2);
    pa->drawLine(content_rect.x() + 2, content_rect.y(), content_rect.right(), content_rect.y());
    pa->drawLine(content_rect.right() + 1, content_rect.y(), content_rect.right() + 1, content_rect.bottom() - 2);
}

void drawArrowUp(QPainter *pa, const QRectF &rect)
{
    QRectF ar(0, 0, rect.width(), rect.height() / 2);
    ar.moveCenter(rect.center());
    drawArrowElement(Qt::UpArrow, pa, ar);
}

void drawArrowDown(QPainter *pa, const QRectF &rect)
{
    QRectF ar(0, 0, rect.width(), rect.height() / 2);
    ar.moveCenter(rect.center());
    drawArrowElement(Qt::DownArrow, pa, ar);
}

void drawArrowLeft(QPainter *pa, const QRectF &rect)
{
    QRectF ar(0, 0, rect.width() / 2, rect.height());
    ar.moveCenter(rect.center());
    drawArrowElement(Qt::LeftArrow, pa, ar);
}

void drawArrowRight(QPainter *pa, const QRectF &rect)
{
    QRectF ar(0, 0, rect.width() / 2, rect.height());
    ar.moveCenter(rect.center());
    drawArrowElement(Qt::RightArrow, pa, ar);
}

void drawArrowBack(QPainter *pa, const QRectF &rect)
{
    drawArrowLeft(pa, rect);
}

void drawArrowForward(QPainter *pa, const QRectF &rect)
{
    drawArrowRight(pa, rect);
}

void drawLineEditClearButton(QPainter *pa, const QRectF &rect)
{
    drawCloseButton(pa, rect);
}

void drawIndicatorUnchecked(QPainter *pa, const QRectF &rect)
{
    pa->drawEllipse(rect);
}

void drawIndicatorChecked(QPainter *pa, const QRectF &rect)
{
    QRectF mark(0, 0, rect.width() / 2, rect.height() / 2);
    mark.moveCenter(rect.center());
    QStyleOption opt;
    QColor brush = opt.palette.color(DPalette::Highlight);
    pa->setPen(Qt::NoPen);
    pa->setBrush(brush);

    pa->drawEllipse(rect);
    drawMark(pa, mark, opt.palette.color(DPalette::Window), opt.palette.color(DPalette::Window), 2, 0);
}

void drawArrowElement(Qt::ArrowType arrow, QPainter *pa, const QRectF &rect)
{
    const QPointF center = rect.center();

    switch (arrow) {
    case Qt::UpArrow: {
        pa->drawLine(QPointF(center.x(), rect.y()), rect.bottomLeft());
        pa->drawLine(QPointF(center.x(), rect.y()), rect.bottomRight());
        break;
    }
    case Qt::LeftArrow: {
        pa->drawLine(QPointF(rect.x(), center.y()), rect.bottomRight());
        pa->drawLine(QPointF(rect.x(), center.y()), rect.topRight());
        break;
    }
    case Qt::DownArrow: {
        pa->drawLine(QPointF(center.x(), rect.bottom()), rect.topLeft());
        pa->drawLine(QPointF(center.x(), rect.bottom()), rect.topRight());
        break;
    }
    case Qt::RightArrow: {
        pa->drawLine(QPointF(rect.right(), center.y()), rect.topLeft());
        pa->drawLine(QPointF(rect.right(), center.y()), rect.bottomLeft());
        break;
    }
    default:
        break;
    }
}

}

/*!
 * \~chinese \class DStyle
 * \~chinese \brief DStyle提供了一个修改风格的类
 * \~chinese \sa QCommonStyle()
 */

/*!
 * \~chinese \enum DStyle::PrimitiveElement
 * \~chinese \sa DStyle::PrimitiveElement QStyle::PrimitiveElement
 *
 * \~chinese \var DStyle::PrimitiveElement DStyle::PE_ItemBackground
 * \~chinese item背景颜色
 *
 * \~chinese \var DStyle::PrimitiveElement DStyle::PE_IconButtonPanel
 * \~chinese DIconButton 面板
 *
 * \~chinese \var DStyle::PrimitiveElement DStyle::PE_IconButtonIcon
 * \~chinese DIconButton 不同状态下的背景颜色
 *
 * \~chinese \var DStyle::PrimitiveElement DStyle::PE_Icon
 * \~chinese DIconButton
 *
 * \~chinese \var DStyle::PrimitiveElement DStyle::PE_SwitchButtonGroove
 * \~chinese DSwitchButton 面板
 *
 * \~chinese \var DStyle::PrimitiveElement DStyle::PE_SwitchButtonHandle
 * \~chinese DSwitchButton 滑块面板
 *
 * \~chinese \var DStyle::PrimitiveElement DStyle::PE_FloatingWidget
 * \~chinese DFloatingWidget 面板
*/

/*!
 * \~chinese \enum DStyle::StyleState
 * \~chinese \sa DStyle::StyleState 记录控件状态的值
 *
 * \~chinese \var DStyle::StyleState DStyle::SS_NormalState
 * \~chinese 正常状态
 *
 * \~chinese \var DStyle::StyleState DStyle::SS_HoverState
 * \~chinese 悬停状态
 *
 * \~chinese \var DStyle::StyleState DStyle::SS_PressState
 * \~chinese 按下状态
 *
 *\~chinese \var DStyle::StyleState DStyle::SS_StateCustomBase
 * \~chinese 自定义子元素的基值
 *
 * \~chinese \var DStyle::StyleState DStyle::StyleState_Mask
 * \~chinese 状态掩码
 *
 * \~chinese \var DStyle::StyleState DStyle::SS_CheckedFlag
 * \~chinese 选定标志
 *
 * \~chinese \var DStyle::StyleState DStyle::SS_SelectedFlag
 * \~chinese 选中状态
 *
 *  * \~chinese \var DStyle::StyleState DStyle::SS_FocusFlag
 * \~chinese 焦点状态
 *
 * \~chinese \var DStyle::StyleState DStyle::SS_FlagCustomBase
 * \~chinese 自定义子元素的基值
*/

/*!
 * \~chinese \enum DStyle::PixelMetric
 * \~chinese \sa DStyle::PixelMetric QStyle::PixelMetric
 *
 * \~chinese \var DStyle::PixelMetric DStyle::PM_FocusBorderWidth
 * \~chinese 控件焦点状态的边框宽度
 *
 * \~chinese \var DStyle::PixelMetric DStyle::PM_FocusBorderSpacing
 * \~chinese 控件内容和border之间的间隔
 *
 * \~chinese \var DStyle::PixelMetric DStyle::PM_FrameRadius
 * \~chinese 控件的圆角大小
 *
 * \~chinese \var DStyle::PixelMetric DStyle::PM_ShadowRadius
 * \~chinese 控件阴影效果的半径
 *
 * \~chinese \var DStyle::PixelMetric DStyle::PM_ShadowHOffset
 * \~chinese 阴影在水平方向的偏移
 *
 * \~chinese \var DStyle::PixelMetric DStyle::PM_ShadowVOffset
 * \~chinese 阴影在竖直方向的偏移
 *
 * \~chinese \var DStyle::PixelMetric DStyle::PM_FrameMargins
 * \~chinese 控件的margins区域，控件内容 = 控件大小 - FrameMargins
 *
 * \~chinese \var DStyle::PixelMetric DStyle::PM_IconButtonIconSize
 * \~chinese 控件大小
 *
 * \~chinese \var DStyle::PixelMetric DStyle::PM_TopLevelWindowRadius
 * \~chinese 窗口的圆角大小
 *
 * \~chinese \var DStyle::PixelMetric DStyle::PM_SwitchButtonHandleWidth
 * \~chinese 控件开关的宽度
 *
 * \~chinese \var DStyle::PixelMetric DStyle::PM_SwithcButtonHandleHeight
 * \~chinese 控件开关的高度
 *
 * \~chinese \var DStyle::PixelMetric DStyle::PM_FloatingWidgetRadius
 * \~chinese (基类)的圆角半径:控件内容-Radius < 控件内容 < 控件显示大小
 *
 * \~chinese \var DStyle::PixelMetric DStyle::PM_FloatingWidgetShadowRadius
 * \~chinese (基类)的阴影Radius区域:控件内容 < 控件内容+阴影margins < 控件内容+阴影margins+阴影Radius = 控件显示大小
 *
 * \~chinese \var DStyle::PixelMetric DStyle::PM_FloatingWidgetShadowMargins
 * \~chinese (基类)阴影的宽度 = 控件显示大小 - 阴影Radius - 控件内容
 *
 * \~chinese \var DStyle::PixelMetric DStyle::PM_FloatingWidgetShadowHOffset
 * \~chinese (基类)的阴影水平偏移
 *
 * \~chinese \var DStyle::PixelMetric DStyle::PM_ContentsMargins
 * \~chinese (基类)内容的边距（一般只用于左右边距）
 *
 * \~chinese \var DStyle::PixelMetric DStyle::PM_ContentsSpacing
 * \~chinese 内容的间距（可用于列表项中每一项的距离）
 *
 * \~chinese \var DStyle::PixelMetric DStyle::PM_ButtonMinimizedSize
 * \~chinese 按钮控件的最小大小
*/

/*!
 * \~chinese \enum DStyle::StandardPixmap
 * \~chinese \sa DStyle::StandardPixmap QStyle::StandardPixmap
 *
 * \~chinese \var DStyle::StandardPixmap DStyle::P_DecreaseElement
 * \~chinese 减少（-）
 * \~chinese \var DStyle::StandardPixmap DStyle::SP_IncreaseElement
 * \~chinese 增加（+）
 * \~chinese \var DStyle::StandardPixmap DStyle::SP_MarkElement
 * \~chinese 对勾
 * \~chinese \var DStyle::StandardPixmap DStyle::SP_SelectElement
 * \~chinese 选择（...）
 * \~chinese \var DStyle::StandardPixmap DStyle::SP_EditElement
 * \~chinese 编辑
 * \~chinese \var DStyle::StandardPixmap DStyle::SP_ExpandElement
 * \~chinese 展开
 * \~chinese \var DStyle::StandardPixmap DStyle::SP_ReduceElement
 * \~chinese 收缩
 * \~chinese \var DStyle::StandardPixmap DStyle::SP_LockElement
 * \~chinese 锁定
 * \~chinese \var DStyle::StandardPixmap DStyle::SP_UnlockElement
 * \~chinese 解锁
 * \~chinese \var DStyle::StandardPixmap DStyle::SP_MediaVolumeLowElement
 * \~chinese 音量
 * \~chinese \var DStyle::StandardPixmap DStyle::SP_MediaVolumeHighElement
 * \~chinese 满音量
 * \~chinese \var DStyle::StandardPixmap DStyle::SP_MediaVolumeMutedElement
 * \~chinese 静音
 * \~chinese \var DStyle::StandardPixmap DStyle::SP_MediaVolumeLeftElement
 * \~chinese 左声道
 * \~chinese \var DStyle::StandardPixmap DStyle::SP_MediaVolumeRightElement
 * \~chinese 右声道
 * \~chinese \var DStyle::StandardPixmap DStyle::SP_ArrowEnter
 * \~chinese 进入
 * \~chinese \var DStyle::StandardPixmap DStyle::SP_ArrowLeave
 * \~chinese 离开
 * \~chinese \var DStyle::StandardPixmap DStyle::SP_ArrowNext
 * \~chinese 下一页
 * \~chinese \var DStyle::StandardPixmap DStyle::SP_ArrowPrev
 * \~chinese 上一页
 * \~chinese \var DStyle::StandardPixmap DStyle::SP_ShowPassword
 * \~chinese 显示密码
 * \~chinese \var DStyle::StandardPixmap DStyle::SP_HidePassword
 * \~chinese 隐藏密码
 * \~chinese \var DStyle::StandardPixmap DStyle::SP_CloseButton
 * \~chinese 关闭按钮（X）
 * \~chinese \var DStyle::StandardPixmap DStyle::SP_IndicatorMajuscule
 * \~chinese 大写标识
 * \~chinese \var DStyle::StandardPixmap DStyle::SP_IndicatorSearch
 * \~chinese 搜索标识（放大镜）
 * \~chinese \var DStyle::StandardPixmap DStyle::SP_IndicatorUnchecked
 * \~chinese 搜索标识（对应对勾的选中状态）
 * \~chinese \var DStyle::StandardPixmap DStyle::SP_IndicatorChecked
 * \~chinese 搜索标识（对勾）
 *
 * \~chinese \var DStyle::StandardPixmap DStyle::SP_DeleteButton
 * \~chinese 删除按钮
 *
 * \~chinese \var DStyle::StandardPixmap DStyle::SP_AddButton
 * \~chinese 新增按钮
*/

/*!
 * \~chinese \brief DStyle::DStyle
 */
DStyle::DStyle()
{

}

/*!
 * \~chinese \brief DStyle::drawPrimitive
 * \~chinese QStyle::drawPrimitive()
 */
void DStyle::drawPrimitive(const QStyle *style, DStyle::PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w)
{
    DStyleHelper dstyle(style);

    switch (pe) {
    case PE_ItemBackground: {
        if (const DStyleOptionBackgroundGroup *vopt = qstyleoption_cast<const DStyleOptionBackgroundGroup *>(opt)) {
            QColor color = vopt->dpalette.color(DPalette::ItemBackground);

            if (dstyle.dstyle()) {
                color = dstyle.dstyle()->generatedBrush(vopt, color, vopt->dpalette.currentColorGroup(), DPalette::ItemBackground).color();
            }

            if (color.alpha() == 0) {
                return;
            }

            int frame_radius = dstyle.pixelMetric(PM_FrameRadius, opt, w);
            p->setBrush(color);
            p->setPen(Qt::NoPen);
            p->setRenderHint(QPainter::Antialiasing);

            if (vopt->directions != Qt::Horizontal && vopt->directions != Qt::Vertical) {
                p->drawRoundedRect(vopt->rect, frame_radius, frame_radius);
                break;
            }

            switch (vopt->position) {
            case DStyleOptionBackgroundGroup::OnlyOne:
                p->drawRoundedRect(vopt->rect, frame_radius, frame_radius);
                break;
            case DStyleOptionBackgroundGroup::Beginning: {
                if (vopt->directions == Qt::Horizontal) {
                    DDrawUtils::drawRoundedRect(p, vopt->rect, frame_radius, frame_radius,
                                                DDrawUtils::TopLeftCorner | DDrawUtils::BottomLeftCorner);
                } else {
                    DDrawUtils::drawRoundedRect(p, vopt->rect, frame_radius, frame_radius,
                                                DDrawUtils::TopLeftCorner | DDrawUtils::TopRightCorner);
                }

                break;
            }
            case DStyleOptionBackgroundGroup::End:
                if (vopt->directions == Qt::Horizontal) {
                    DDrawUtils::drawRoundedRect(p, vopt->rect, frame_radius, frame_radius,
                                                DDrawUtils::TopRightCorner | DDrawUtils::BottomRightCorner);
                } else {
                    DDrawUtils::drawRoundedRect(p, vopt->rect, frame_radius, frame_radius,
                                                DDrawUtils::BottomLeftCorner | DDrawUtils::BottomRightCorner);
                }

                break;
            case DStyleOptionBackgroundGroup::Middle:
                p->setRenderHint(QPainter::Antialiasing, false);
                p->drawRect(vopt->rect);
                break;
            default:
                break;
            }

            return;
        }
        break;
    }
    case PE_IconButtonPanel: {
        if (const DStyleOptionButton *btn = qstyleoption_cast<const DStyleOptionButton *>(opt)) {
            DStyleHelper dstyle(style);

            if (btn->features & DStyleOptionButton::FloatingButton) {
                int frame_margins = 3;
                const QMargins margins(frame_margins, frame_margins, frame_margins, frame_margins);
                QRect shadow_rect = opt->rect + margins;
                const QRect content_rect = opt->rect - margins;
                QColor color = dstyle.getColor(opt, QPalette::Button);

                qreal frame_radius = content_rect.width() / 2.0;
                int shadow_radius = 5;
                int shadow_xoffset = 0;
                int shadow_yoffset = 2;

                shadow_rect.setTopLeft(shadow_rect.topLeft() + QPoint(shadow_xoffset, shadow_yoffset));
                shadow_rect.setWidth(qMin(shadow_rect.width(), shadow_rect.height()));
                shadow_rect.setHeight(qMin(shadow_rect.width(), shadow_rect.height()));
                shadow_rect.moveCenter(opt->rect.center() + QPoint(shadow_xoffset / 2.0, shadow_yoffset / 2.0));

                DDrawUtils::drawShadow(p, shadow_rect, frame_radius, frame_radius,
                                       DStyle::adjustColor(color, 0, 0, +30), shadow_radius, QPoint(0, 0));

                p->setPen(Qt::NoPen);
                p->setBrush(color);
                p->setRenderHint(QPainter::Antialiasing);
                p->drawEllipse(content_rect);
            } else {
                style->drawControl(CE_PushButtonBevel, opt, p, w);
            }
        }
        break;
    }
    case PE_IconButtonIcon: {
        if (const DStyleOptionButton *btn = qstyleoption_cast<const DStyleOptionButton *>(opt)) {
            DStyleHelper dstyle(style);
            DStyleOptionIcon icon_option;

            icon_option.QStyleOption::operator =(*opt);
            icon_option.icon = btn->icon;
            icon_option.dpalette = btn->dpalette;

            QPalette pa = opt->palette;

            if (btn->features & DStyleOptionButton::TitleBarButton) {
                if (!(opt->state & (State_MouseOver | State_Sunken))) {
                    pa.setBrush(QPalette::Background, Qt::transparent);
                }

                if (opt->state & State_Sunken) {
                    pa.setBrush(QPalette::Foreground, opt->palette.highlight());
                } else {
                    pa.setBrush(QPalette::Foreground, opt->palette.buttonText());
                }
            } else {
                pa.setBrush(QPalette::Background, dstyle.generatedBrush(opt, pa.button(), pa.currentColorGroup(), QPalette::Button));
                pa.setBrush(QPalette::Foreground, dstyle.generatedBrush(opt, pa.buttonText(), pa.currentColorGroup(), QPalette::ButtonText));
            }

            icon_option.palette = pa;
            icon_option.rect.setSize(btn->iconSize);
            icon_option.rect.moveCenter(QRect(opt->rect).center());

            dstyle.drawPrimitive(PE_Icon, &icon_option, p, w);
        }
        break;
    }
    case PE_Icon: {
        if (const DStyleOptionIcon *icon_opt = qstyleoption_cast<const DStyleOptionIcon *>(opt)) {
            if (icon_opt->icon.isNull()) {
                return;
            }

            auto *data = const_cast<DStyleOptionIcon *>(icon_opt)->icon.data_ptr();

            if (DStyledIconEngine *engine = dynamic_cast<DStyledIconEngine *>(data->engine)) {
                engine->paint(p, opt->palette, opt->rect);
            } else {
                QIcon::Mode mode = QIcon::Normal;
                QIcon::State state = QIcon::Off;

                if (!(opt->state & QStyle::State_Enabled)) {
                    mode = QIcon::Disabled;
                } else if (opt->state & QStyle::State_Selected) {
                    mode = QIcon::Selected;
                } else if (opt->state & QStyle::State_MouseOver) {
                    mode = QIcon::Active;
                }

                if (opt->state & QStyle::State_On) {
                    state = QIcon::On;
                }

                p->setBrush(opt->palette.background());
                p->setPen(QPen(opt->palette.foreground(), 1));
                icon_opt->icon.paint(p, opt->rect, Qt::AlignCenter, mode, state);
            }
        }
        break;
    }
    case PE_SwitchButtonGroove: {
        if (const DStyleOptionButton *btn = qstyleoption_cast<const DStyleOptionButton *>(opt)) {
            QRect rectGroove = btn->rect;
            int frame_radius = dstyle.pixelMetric(DStyle::PM_FrameRadius, opt, w);

            p->setRenderHint(QPainter::Antialiasing);
            p->setPen(Qt::NoPen);
            p->setBrush(dstyle.getColor(opt, QPalette::Button));
            p->drawRoundedRect(rectGroove, frame_radius, frame_radius);
        }
        break;
    }
    case PE_SwitchButtonHandle: {
        if (const DStyleOptionButton *btn = qstyleoption_cast<const DStyleOptionButton *>(opt)) {
            QRect rectHandle = btn->rect;
            int frame_radius = dstyle.pixelMetric(DStyle::PM_FrameRadius, opt, w);
            p->setRenderHint(QPainter::Antialiasing);
            p->setPen(Qt::NoPen);

            if (btn->state & State_On) {
                p->setBrush(dstyle.getColor(opt, QPalette::Highlight));
            } else {
                p->setBrush(dstyle.getColor(opt, QPalette::ButtonText));
            }

            p->drawRoundedRect(rectHandle, frame_radius, frame_radius);
        }
        break;
    }
    case PE_FloatingWidget: {
        if (const DStyleOptionFloatingWidget *btn = qstyleoption_cast<const DStyleOptionFloatingWidget *>(opt)) {
            DStyleHelper dstyle(style);
            int shadowRadius = dstyle.pixelMetric(PM_FloatingWidgetShadowRadius, opt, w);    //18
            int frameRadius = dstyle.pixelMetric(PM_FloatingWidgetRadius, opt, w);           //18
            int offsetX = dstyle.pixelMetric(PM_FloatingWidgetShadowHOffset, opt, w);        //0
            int offsetY = dstyle.pixelMetric(PM_FloatingWidgetShadowVOffset, opt, w);        //6
            int shadowMargins = dstyle.pixelMetric(PM_FloatingWidgetShadowMargins, opt, w) * 2;

            //绘画 矩形(图标icon+text+btn+icon)和外面一小圈frameRadius/2的 合在一起的矩形
            p->setRenderHint(QPainter::Antialiasing);
            QMargins shadow_margin(shadowMargins, shadowMargins, shadowMargins, shadowMargins);

            //先绘画阴影
            DDrawUtils::drawShadow(p, btn->rect + shadow_margin, frameRadius, frameRadius, QColor(0, 0, 0, 0.25 * 255), shadowRadius, QPoint(offsetX, offsetY));
            //再绘画上面的待显示区域
            p->setPen(QPen(btn->dpalette.frameShadowBorder(), 1));
            p->setBrush(btn->noBackground ? Qt::NoBrush : p->background());
            p->drawRoundedRect(opt->rect, frameRadius, frameRadius);
        }
        break;
    }
    default:
        break;
    }
}

/*!
 * \~chinese \brief DStyle::drawControl
 * \~chinese \sa QStyle::drawControl()
 */
void DStyle::drawControl(const QStyle *style, DStyle::ControlElement ce, const QStyleOption *opt, QPainter *p, const QWidget *w)
{
    switch (ce) {
    case CE_IconButton: {
        if (const DStyleOptionButton *btn = qstyleoption_cast<const DStyleOptionButton *>(opt)) {
            DStyleHelper dstyle(style);

            if (!(btn->features & DStyleOptionButton::Flat)) {
                dstyle.drawPrimitive(PE_IconButtonPanel, opt, p, w);
            }

            if ((!btn->text.isEmpty() && (btn->features & DStyleOptionButton::FloatingButton))) {
                QStyleOptionButton subopt = *btn;
                subopt.rect = style->subElementRect(SE_PushButtonContents, opt, w);

                if (!(btn->features & QStyleOptionButton::Flat))
                    subopt.palette.setBrush(QPalette::ButtonText, subopt.palette.highlightedText());

                style->drawControl(CE_PushButtonLabel, &subopt, p, w);
            } else {
                DStyleOptionButton new_opt = *btn;
                new_opt.rect = dstyle.subElementRect(SE_IconButtonIcon, opt, w);
                dstyle.drawPrimitive(PE_IconButtonIcon, &new_opt, p, w);
            }

            if (btn->state & State_HasFocus) {
                if (btn->features & DStyleOptionButton::FloatingButton) {
                    int border_width = dstyle.pixelMetric(PM_FocusBorderWidth, opt, w);
                    QColor color = dstyle.getColor(opt, QPalette::Highlight);

                    p->setPen(QPen(color, border_width, Qt::SolidLine));
                    p->setBrush(Qt::NoBrush);
                    p->setRenderHint(QPainter::Antialiasing);
                    p->drawEllipse(QRectF(opt->rect).adjusted(1, 1, -1, -1));
                } else if (btn->features & DStyleOptionButton::TitleBarButton) {
                    QStyleOption option = *opt;
                    option.rect.adjust(6, 6, -6, -6);
                    style->drawPrimitive(PE_FrameFocusRect, &option, p, w);
                } else {
                    style->drawPrimitive(PE_FrameFocusRect, opt, p, w);
                }
            }
        }
        break;
    }
    case CE_SwitchButton: {
        if (const DStyleOptionButton *btn = qstyleoption_cast<const DStyleOptionButton *>(opt)) {
            DStyleHelper dstyle(style);
            DStyleOptionButton option = *btn;
            option.dpalette = btn->dpalette;
            option.rect = dstyle.subElementRect(SE_SwitchButtonGroove, opt, w);
            dstyle.drawPrimitive(PE_SwitchButtonGroove, &option, p, w);
            option.rect = dstyle.subElementRect(SE_SwitchButtonHandle, opt, w);
            dstyle.drawPrimitive(PE_SwitchButtonHandle, &option, p, w);
        }
        break;
    }
    case CE_FloatingWidget: {
        if (const DStyleOptionFloatingWidget *btn = qstyleoption_cast<const DStyleOptionFloatingWidget *>(opt)) {
            DStyleHelper dstyle(style);
            DStyleOptionFloatingWidget option = *btn;
            option.dpalette = btn->dpalette;
            option.rect = dstyle.subElementRect(SE_FloatingWidget, opt, w);
            dstyle.drawPrimitive(PE_FloatingWidget, &option, p, w);
        }
        break;
    }
    case CE_ButtonBoxButton: {
        if (const DStyleOptionButton *btn = qstyleoption_cast<const DStyleOptionButton *>(opt)) {
            DStyleHelper dstyle(style);
            dstyle.drawControl(CE_ButtonBoxButtonBevel, btn, p, w);
            DStyleOptionButton subopt = *btn;
            subopt.rect = dstyle.subElementRect(SE_ButtonBoxButtonContents, btn, w);
            dstyle.drawControl(CE_ButtonBoxButtonLabel, &subopt, p, w);
            if (btn->state & State_HasFocus) {
                QStyleOptionFocusRect fropt;
                fropt.QStyleOption::operator=(*btn);
                fropt.rect = dstyle.subElementRect(SE_ButtonBoxButtonFocusRect, btn, w);
                style->drawPrimitive(PE_FrameFocusRect, &fropt, p, w);
            }
        }
        break;
    }
    case CE_ButtonBoxButtonBevel: {
        if (const DStyleOptionButtonBoxButton *btn = qstyleoption_cast<const DStyleOptionButtonBoxButton *>(opt)) {
            bool checked = btn->state & State_On;
            bool disable = !(btn->state & State_Enabled);
            bool hover = btn->state & State_MouseOver;
            bool press = btn->state & State_Sunken;

            // normal状态时不进行任何绘制，此时可直接使用button box的背景
            if (!checked && !disable && !hover && !press) {
                return;
            }

            DStyleHelper dstyle(style);
            const QColor &background = dstyle.getColor(opt, checked ? QPalette::Highlight : QPalette::Button);
            p->setBrush(background);
            p->setPen(QPen(dstyle.getColor(btn, DPalette::FrameBorder), 1));
            int radius = dstyle.pixelMetric(PM_FrameRadius, opt, w);
            int margins = dstyle.pixelMetric(PM_FrameMargins, opt, w);
            DStyleOptionButtonBoxButton::ButtonPosition pos = btn->position;

            if (btn->state & State_HasFocus) {
                pos = DStyleOptionButtonBoxButton::OnlyOne;
            }

            switch (pos) {
            case DStyleOptionButtonBoxButton::Beginning: {
                p->setRenderHint(QPainter::Antialiasing);
                QRect rect;

                if (btn->orientation == Qt::Horizontal) {
                    rect = opt->rect.adjusted(margins, margins, 0, -margins);
                    DDrawUtils::drawRoundedRect(p, rect, radius, radius, DDrawUtils::TopLeftCorner | DDrawUtils::BottomLeftCorner);
                } else {
                    rect = opt->rect.adjusted(margins, margins, -margins, 0);
                    DDrawUtils::drawRoundedRect(p, rect, radius, radius, DDrawUtils::TopLeftCorner | DDrawUtils::TopRightCorner);
                }

                break;
            }
            case DStyleOptionButtonBoxButton::Middle: {
                QRect rect;

                if (btn->orientation == Qt::Horizontal)
                    rect = opt->rect.adjusted(0, margins, 0, -margins);
                else
                    rect = opt->rect.adjusted(margins, 0, -margins, 0);

                p->drawRect(rect);
                break;
            }
            case DStyleOptionButtonBoxButton::End: {
                p->setRenderHint(QPainter::Antialiasing);
                QRect rect;

                if (btn->orientation == Qt::Horizontal) {
                    rect = opt->rect.adjusted(0, margins, -margins, -margins);
                    DDrawUtils::drawRoundedRect(p, rect, radius, radius, DDrawUtils::TopRightCorner | DDrawUtils::BottomRightCorner);
                } else {
                    rect = opt->rect.adjusted(margins, 0, -margins, -margins);
                    DDrawUtils::drawRoundedRect(p, rect, radius, radius, DDrawUtils::BottomLeftCorner | DDrawUtils::BottomRightCorner);
                }

                break;
            }
            case DStyleOptionButtonBoxButton::OnlyOne: {
                QRect rect = opt->rect.adjusted(margins, margins, -margins, -margins);
                p->setRenderHint(QPainter::Antialiasing);
                p->drawRoundedRect(rect, radius, radius);
                break;
            }
            default:
                break;
            }
        }
        break;
    }
    case CE_ButtonBoxButtonLabel: {
        style->drawControl(CE_PushButtonLabel, opt, p, w);
        break;
    }
    case CE_TextButton: {
        if (const QStyleOptionButton *option = qstyleoption_cast<const QStyleOptionButton *>(opt)) {
            DStyleHelper dstyle(style);
            p->setPen(dstyle.getColor(option, QPalette::Highlight));
            p->drawText(opt->rect, option->text);
        }
        break;
    }
    default:
        break;
    }
}

/*!
 * \~chinese \brief DStyle::pixelMetric
 * \~chinese \sa QStyle::pixelMetric()
 */
int DStyle::pixelMetric(const QStyle *style, DStyle::PixelMetric m, const QStyleOption *opt, const QWidget *widget)
{
    DStyleHelper dstyle(style);

    switch (m) {
    case PM_FocusBorderWidth: Q_FALLTHROUGH();
    case PM_FocusBorderSpacing:
        return 2;
    case PM_FrameRadius:
        if (widget) {
            const QVariant &radius_value = widget->property("_d_dtk_frameRadius");
            bool ok = false;
            int radius = radius_value.toInt(&ok);
            if (ok && radius >= 0) {
               return radius;
            }
        }
        return 8;
    case PM_TopLevelWindowRadius:
        return 18;
    case PM_ShadowRadius:
        return 0;
    case PM_ShadowHOffset:
        return 0;
    case PM_ShadowVOffset:
        return 1;
    case PM_FrameMargins: {
        int shadow_radius = dstyle.pixelMetric(PM_ShadowRadius, opt, widget);
        int shadow_xoffset = dstyle.pixelMetric(PM_ShadowHOffset, opt, widget);
        int shadow_yoffset = dstyle.pixelMetric(PM_ShadowVOffset, opt, widget);
        int margins = shadow_radius + qMax(shadow_xoffset, shadow_yoffset);

        return margins;
    }
    case PM_IconButtonIconSize: {
        if (const DStyleOptionButton *btn = qstyleoption_cast<const DStyleOptionButton *>(opt)) {
            if (btn->features & DStyleOptionButton::FloatingButton) {
                return 20;
            }
        }
        return 12;
    }
    case PM_SwitchButtonHandleWidth:
        return 30;
    case PM_SwithcButtonHandleHeight:
        return 24;
    case PM_FloatingWidgetRadius: {
        if (const DStyleOptionFloatingWidget *wid = qstyleoption_cast<const DStyleOptionFloatingWidget *>(opt)) {
            if (wid->frameRadius != -1)
                return wid->frameRadius;
        }
        return dstyle.pixelMetric(PM_TopLevelWindowRadius, opt, widget);
    }
    case PM_FloatingWidgetShadowRadius:
        return 8;
    case PM_FloatingWidgetShadowHOffset:
        return 0;
    case PM_FloatingWidgetShadowVOffset:
        return 4;
    case PM_FloatingWidgetShadowMargins: {
        int shadow_radius = dstyle.pixelMetric(PM_FloatingWidgetShadowRadius, opt, widget);
        int shadow_hoffset = dstyle.pixelMetric(PM_FloatingWidgetShadowHOffset, opt, widget);
        int shadow_voffset = dstyle.pixelMetric(PM_FloatingWidgetShadowVOffset, opt, widget);

        return (shadow_radius +  qMax(shadow_hoffset, shadow_voffset)) / 2;
    }
    case PM_ContentsMargins:
    case PM_ContentsSpacing:
        return 10;
    case PM_ButtonMinimizedSize:
        return 36;
    default:
        break;
    }

    return -1;
}

/*!
 * \~chinese \brief DStyle::subElementRect
 * \~chinese \sa QStyle::subElementRect()
 */
QRect DStyle::subElementRect(const QStyle *style, DStyle::SubElement r, const QStyleOption *opt, const QWidget *widget)
{
    Q_UNUSED(widget)

    switch (r) {
    case SE_IconButtonIcon:
        if (const DStyleOptionButton *btn = qstyleoption_cast<const DStyleOptionButton *>(opt)) {
            if (btn->features & DStyleOptionButton::FloatingButton) {
                QRect icon_rect(opt->rect);
                icon_rect.setWidth(opt->rect.width() * 0.75);
                icon_rect.setHeight(opt->rect.height() * 0.75);
                icon_rect.moveCenter(opt->rect.center());

                return icon_rect;
            }
        }

        return opt->rect;
    case SE_SwitchButtonGroove: {
        if (const DStyleOptionButton *btn = qstyleoption_cast<const DStyleOptionButton *>(opt)) {
            return btn->rect;
        }
        break;
    }
    case SE_SwitchButtonHandle: {
        if (const DStyleOptionButton *btn = qstyleoption_cast<const DStyleOptionButton *>(opt)) {
            DStyleHelper dstyle(style);
            int handleWidth = dstyle.pixelMetric(PM_SwitchButtonHandleWidth, opt, widget);
            int handleHeight = dstyle.pixelMetric(PM_SwithcButtonHandleHeight, opt, widget);
            QRect rectHandle(0, 0, handleWidth, handleHeight);

            if (btn->state & QStyle::State_On) {
                rectHandle.moveRight(opt->rect.right());
            } else {
                rectHandle.moveLeft(opt->rect.left());
            }

            return rectHandle;
        }
        break;
    }
    case SE_FloatingWidget: {
        if (const DStyleOptionFloatingWidget *btn = qstyleoption_cast<const DStyleOptionFloatingWidget *>(opt)) {
            DStyleHelper dstyle(style);
            QRect rect = btn->rect;
            int shadowMarge = dstyle.pixelMetric(PM_FloatingWidgetShadowMargins, opt, widget);
            QRect rectBtn = rect.adjusted(shadowMarge, shadowMarge, -shadowMarge, -shadowMarge);

            return rectBtn;
        }
        break;
    }
    case SE_ButtonBoxButtonContents:
        return style->subElementRect(SE_PushButtonContents, opt, widget);
    case SE_ButtonBoxButtonFocusRect:
        return style->subElementRect(SE_PushButtonFocusRect, opt, widget);
    default:
        break;
    }

    return QRect();
}

/*!
 * \~chinese \brief DStyle::sizeFromContents
 * \~chinese \sa QStyle::sizeFromContents()
 */
QSize DStyle::sizeFromContents(const QStyle *style, DStyle::ContentsType ct, const QStyleOption *opt, const QSize &contentsSize, const QWidget *widget)
{
    Q_UNUSED(widget)

    switch (ct) {
    case CT_IconButton:
        if (const DStyleOptionButton *btn = qstyleoption_cast<const DStyleOptionButton *>(opt)) {
            if (btn->features & DStyleOptionButton::FloatingButton) {
                return btn->iconSize * 2.5;
            }

            if (btn->features & DStyleOptionButton::Flat) {
                return contentsSize.expandedTo(btn->iconSize);
            }

            return style->sizeFromContents(CT_PushButton, opt, btn->iconSize, widget);
        }
        Q_FALLTHROUGH();
    case CT_SwitchButton: {
        DStyleHelper dstyle(style);
        int w = dstyle.pixelMetric(PM_SwitchButtonHandleWidth, opt, widget);
        int h = dstyle.pixelMetric(PM_SwithcButtonHandleHeight, opt, widget);
        QSize size(qMax(contentsSize.width(), w * 5 / 3), qMax(contentsSize.height(), h));

        return size;
    }
    case CT_FloatingWidget: {
        DStyleHelper dstyle(style);
        int margins = dstyle.pixelMetric(PM_FloatingWidgetShadowMargins, opt, widget);
        int window_radius = dstyle.pixelMetric(PM_FloatingWidgetRadius, opt, widget);
        QSize size(2 * margins + qMax(2 * window_radius, contentsSize.width() + window_radius),
                   2 * margins + qMax(2 * window_radius,  contentsSize.height()));
        return size;
    }
    case CT_ButtonBoxButton: {
        QSize size = style->sizeFromContents(CT_PushButton, opt, contentsSize, widget);

        if (const DStyleOptionButtonBoxButton *btn = qstyleoption_cast<const DStyleOptionButtonBoxButton *>(opt)) {
            if (btn->text.isEmpty()) {
                // 只有图标时高度至少和宽度一致
                size.setHeight(qMax(size.width(), size.height()));
            }

            int frame_margin = DStyleHelper(style).pixelMetric(PM_FrameMargins, opt, widget);
            switch (btn->position) {
            case DStyleOptionButtonBoxButton::Beginning:
            case DStyleOptionButtonBoxButton::End:
                size.rwidth() -= frame_margin;
                break;
            case DStyleOptionButtonBoxButton::Middle:
                size.rwidth() -= 2 * frame_margin;
                break;
            default:
                break;
            }
        }

        return size;
    }
    default:
        break;
    }

    return contentsSize;
}

/*!
 * \~chinese \brief DStyle::standardIcon
 * \~chinese \sa QStyle::standardIcon()
 */
QIcon DStyle::standardIcon(const QStyle *style, DStyle::StandardPixmap st, const QStyleOption *opt, const QWidget *widget)
{
    Q_UNUSED(opt)
    Q_UNUSED(style)
    Q_UNUSED(widget)

#define CASE_ICON(Value) \
case SP_##Value: { \
        DStyledIconEngine *icon_engine = new DStyledIconEngine(DDrawUtils::draw##Value, QStringLiteral(#Value)); \
        return QIcon(icon_engine);}

    switch (st) {
    CASE_ICON(SelectElement)

    case SP_IndicatorUnchecked:
        return QIcon::fromTheme("unselected_indicator");
    case SP_IndicatorChecked: {
        const QIcon &icon = QIcon::fromTheme("selected_indicator");
        DStyledIconEngine *icon_engine = new DStyledIconEngine(std::bind(DStyledIconEngine::drawIcon, icon, std::placeholders::_1, std::placeholders::_2), QStringLiteral("IndicatorChecked"));
        icon_engine->setFrontRole(widget, DPalette::Highlight);
        return QIcon(icon_engine);
    }
    case SP_DeleteButton:
        return QIcon::fromTheme("list_delete");
    case SP_AddButton:
        return QIcon::fromTheme("list_add");
    case SP_ForkElement:
        return QIcon::fromTheme("fork_indicator");
    case SP_CloseButton:
        return QIcon::fromTheme("window-close_round");
    case SP_DecreaseElement:
        return QIcon::fromTheme("button_reduce");
    case SP_IncreaseElement:
        return QIcon::fromTheme("button_add");
    case SP_MarkElement:
        return QIcon::fromTheme("mark_indicator");
    case SP_UnlockElement:
        return QIcon::fromTheme("unlock_indicator");
    case SP_LockElement:
        return QIcon::fromTheme("lock_indicator");
    case SP_ExpandElement:
        return QIcon::fromTheme("go-up");
    case SP_ReduceElement:
        return QIcon::fromTheme("go-down");
    case SP_ArrowEnter:
        return style->standardIcon(SP_ArrowForward);
    case SP_ArrowNext:
        return QIcon::fromTheme("next_indicator");
    case SP_ArrowLeave:
        return style->standardIcon(SP_ArrowBack);
    case SP_ArrowPrev:
        return QIcon::fromTheme("prev_indicator");
    case SP_EditElement:
        return QIcon::fromTheme("edit");
    case SP_MediaVolumeLowElement:
        return QIcon::fromTheme("audio-volume-low");
    case SP_MediaVolumeHighElement:
        return QIcon::fromTheme("audio-volume-medium");
    case SP_MediaVolumeMutedElement:
        return QIcon::fromTheme("audio-volume-muted");
    case SP_MediaVolumeLeftElement:
        return QIcon::fromTheme("audio-volume-left");
    case SP_MediaVolumeRightElement:
        return QIcon::fromTheme("audio-volume-right");
    case SP_IndicatorMajuscule:
        return QIcon::fromTheme("caps_lock");
    case SP_ShowPassword:
        return QIcon::fromTheme("password_show");
    case SP_HidePassword:
        return QIcon::fromTheme("password_hide");
    case SP_IndicatorSearch:
        return QIcon::fromTheme("search_indicator");
    default:
        break;
    }

    return QIcon();
}

/*!
 * \~chinese \brief DStyle::styleHint
 * \~chinese \sa QStyle::styleHint()
 */
int DStyle::styleHint(QStyle::StyleHint sh, const QStyleOption *opt, const QWidget *w, QStyleHintReturn *shret) const
{
    switch (sh) {
    case SH_ScrollBar_MiddleClickAbsolutePosition:
    case SH_FontDialog_SelectAssociatedText:
    case SH_Menu_KeyboardSearch:
    case SH_Menu_Scrollable:
    case SH_Menu_SloppySubMenus:
    case SH_ComboBox_ListMouseTracking:
    case SH_Menu_MouseTracking:
    case SH_MenuBar_MouseTracking:
    case SH_Menu_FillScreenWithScroll:
    case SH_ItemView_ChangeHighlightOnFocus:
    case SH_TabBar_PreferNoArrows:
    case SH_ComboBox_Popup:
    case SH_Slider_StopMouseOverSlider:
    case SH_SpinBox_AnimateButton:
    case SH_SpinControls_DisableOnBounds:
    case SH_Menu_FadeOutOnHide:
    case SH_ItemView_ShowDecorationSelected:
    case SH_ScrollBar_Transient:
    case SH_TitleBar_ShowToolTipsOnButtons:
        return true;
    case SH_SpinBox_ButtonsInsideFrame:
    case SH_ScrollBar_LeftClickAbsolutePosition:
    case SH_Slider_SnapToValue:
    case SH_Menu_AllowActiveAndDisabled:
    case SH_BlinkCursorWhenTextSelected:
    case SH_UnderlineShortcut:
    case SH_ItemView_PaintAlternatingRowColorsForEmptyArea:
    case SH_ComboBox_AllowWheelScrolling:
        return false;
    case SH_Header_ArrowAlignment:
        return Qt::AlignVCenter | Qt::AlignRight;
    case SH_Menu_SubMenuPopupDelay:
        return 100;
    case SH_ToolTipLabel_Opacity:
        return 255;
    case SH_RequestSoftwareInputPanel:
        return RSIP_OnMouseClickAndAlreadyFocused;
    case SH_ItemView_ScrollMode:
        return QAbstractItemView::ScrollPerPixel;
    case SH_Widget_Animation_Duration:
        return 300;
    case SH_Button_FocusPolicy:
        return Qt::TabFocus;
    case SH_ScrollView_FrameOnlyAroundContents:
        return false;
    default:
        break;
    }

    return QCommonStyle::styleHint(sh, opt, w, shret);
}

/*!
 * \~chinese \brief DStyle::standardPalette 获取标准调色板
 * \~chinese \return 调色板
 */
QPalette DStyle::standardPalette() const
{
    QPalette pa = DGuiApplicationHelper::instance()->standardPalette(DGuiApplicationHelper::LightType);
    // 将无效的颜色fallback到QCommonStyle提供的palette，且在resolve操作中将detach pa对象
    // 防止在QApplication initSystemPalette的setSystemPalette中获取到一个和 QGuiApplicationPrivate::platformTheme()->palette()
    // 一样的QPalette对象，这样将导致QApplicationPrivate::setPalette_helper中将 app_pal 和新的QPalette对比时认为他们没有变化
    return pa.resolve(QCommonStyle::standardPalette());
}

DStyle::StyleState DStyle::getState(const QStyleOption *option)
{
    DStyle::StyleState state = DStyle::SS_NormalState;

    if (!option->state.testFlag(DStyle::State_Enabled)) {
        return DStyle::SS_NormalState;
    }

    if (option->state.testFlag(DStyle::State_Sunken)) {
        state = DStyle::SS_PressState;
    } else if (option->state.testFlag(DStyle::State_MouseOver)) {
        state = DStyle::SS_HoverState;
    }

    return state;
}

static DStyle::StateFlags getFlags(const QStyleOption *option)
{
    DStyle::StateFlags flags = 0;

    if (option->state.testFlag(DStyle::State_On)) {
        flags |= DStyle::SS_CheckedFlag;
    }

    if (option->state.testFlag(DStyle::State_Selected)) {
        flags |= DStyle::SS_SelectedFlag;
    }

    if (option->state.testFlag(DStyle::State_HasFocus)) {
        flags |= DStyle::SS_FocusFlag;
    }

    return flags;
}

/*!
 * \~chinese \brief DStyle::drawPrimitive
 * \~chinese \sa QStyle::drawPrimitive()
 */
void DStyle::drawPrimitive(QStyle::PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w) const
{
    switch (pe) {
    case PE_IndicatorArrowUp:
        p->setPen(QPen(opt->palette.foreground(), 1));
        return DDrawUtils::drawArrowUp(p, opt->rect);
    case PE_IndicatorArrowDown:
        p->setPen(QPen(opt->palette.foreground(), 1));
        return DDrawUtils::drawArrowDown(p, opt->rect);
    case PE_IndicatorArrowRight:
        p->setPen(QPen(opt->palette.foreground(), 1));
        return DDrawUtils::drawArrowRight(p, opt->rect);
    case PE_IndicatorArrowLeft:
        p->setPen(QPen(opt->palette.foreground(), 1));
        return DDrawUtils::drawArrowLeft(p, opt->rect);
    case PE_IndicatorHeaderArrow:
        if (const QStyleOptionHeader *header = qstyleoption_cast<const QStyleOptionHeader *>(opt)) {
            p->setPen(QPen(opt->palette.foreground(), 1));
            if (header->sortIndicator & QStyleOptionHeader::SortUp) {
                return DDrawUtils::drawArrowElement(Qt::DownArrow, p, opt->rect);
            } else if (header->sortIndicator & QStyleOptionHeader::SortDown) {
                return DDrawUtils::drawArrowElement(Qt::UpArrow, p, opt->rect);
            }
        }
        return;
    default:
        break;
    }

    if (Q_UNLIKELY(pe < QStyle::PE_CustomBase)) {
        return QCommonStyle::drawPrimitive(pe, opt, p, w);
    }

    drawPrimitive(this, static_cast<PrimitiveElement>(pe), opt, p, w);
}

/*!
 * \~chinese \brief DStyle::drawControl
 * \~chinese \sa QStyle::drawControl()
 */
void DStyle::drawControl(QStyle::ControlElement ce, const QStyleOption *opt, QPainter *p, const QWidget *w) const
{
    if (Q_UNLIKELY(ce < QStyle::CE_CustomBase)) {
        return QCommonStyle::drawControl(ce, opt, p, w);
    }

    drawControl(this, static_cast<ControlElement>(ce), opt, p, w);
}

/*!
 * \~chinese \brief DStyle::pixelMetric
 * \~chinese \sa QStyle::pixelMetric()
 */
int DStyle::pixelMetric(QStyle::PixelMetric m, const QStyleOption *opt, const QWidget *widget) const
{
    switch (m) {
    case PM_ButtonDefaultIndicator:
    case PM_ButtonShiftHorizontal:
    case PM_ButtonShiftVertical:
    case PM_FocusFrameVMargin:
    case PM_FocusFrameHMargin:
    case PM_MenuBarPanelWidth:
    case PM_MenuDesktopFrameWidth:
        return 0;
    case PM_ButtonMargin:
    case PM_DefaultChildMargin:
        return pixelMetric(PM_FrameRadius, opt, widget);
    case PM_DefaultFrameWidth:
        return 1;
    case PM_DefaultLayoutSpacing:
        return 5;
    case PM_DefaultTopLevelMargin:
        return pixelMetric(PM_TopLevelWindowRadius, opt, widget) / 2;
    case PM_MenuBarItemSpacing:
        return 6;
    case PM_IndicatorWidth:
    case PM_IndicatorHeight:
    case PM_ExclusiveIndicatorHeight:
    case PM_ExclusiveIndicatorWidth:
        return 14;
    case PM_ScrollBarSliderMin:
        return 36;
    case PM_SliderLength:
    case PM_ScrollBarExtent:
        return 20;
    case PM_SliderControlThickness:
        return 24;
    case PM_MenuBarHMargin:
        return 10;
    case PM_MenuBarVMargin:
        return 6;
    case PM_SliderTickmarkOffset:
        return 14;
    case PM_MenuHMargin:
        return 0;
    case PM_MenuVMargin:
        return pixelMetric(PM_TopLevelWindowRadius, opt, widget);
    case PM_SmallIconSize:
        return 14;
    case PM_ButtonIconSize:
        return 16;
    case PM_ListViewIconSize:
    case PM_LargeIconSize:
        return 24;
    case PM_IconViewIconSize:
        return 32;
    case PM_ScrollView_ScrollBarOverlap:
        return true;
    default:
        break;
    }

    if (Q_UNLIKELY(m < QStyle::PM_CustomBase)) {
        return QCommonStyle::pixelMetric(m, opt, widget);
    }

    return pixelMetric(this, static_cast<PixelMetric>(m), opt, widget);
}

/*!
 * \~chinese \brief DStyle::subElementRect
 * \~chinese \sa QStyle::subElementRect()
 */
QRect DStyle::subElementRect(QStyle::SubElement r, const QStyleOption *opt, const QWidget *widget) const
{
    if (Q_UNLIKELY(r < QStyle::SE_CustomBase)) {
        return QCommonStyle::subElementRect(r, opt, widget);
    }

    return subElementRect(this, static_cast<DStyle::SubElement>(r), opt, widget);
}

/*!
 * \~chinese \brief DStyle::sizeFromContents
 * \~chinese \sa QStyle::sizeFromContents()
 */
QSize DStyle::sizeFromContents(QStyle::ContentsType ct, const QStyleOption *opt, const QSize &contentsSize, const QWidget *widget) const
{
    if (Q_UNLIKELY(ct < QStyle::CT_CustomBase)) {
        return QCommonStyle::sizeFromContents(ct, opt, contentsSize, widget);
    }

    return sizeFromContents(this, static_cast<DStyle::ContentsType>(ct), opt, contentsSize, widget);
}

/*!
 * \~chinese \brief DStyle::standardIcon
 * \~chinese \param \sa QStyle::standardIcon()
 */
QIcon DStyle::standardIcon(QStyle::StandardPixmap st, const QStyleOption *opt, const QWidget *widget) const
{
    switch (st) {
        CASE_ICON(TitleBarMenuButton)
        CASE_ICON(TitleBarMinButton)
        CASE_ICON(TitleBarMaxButton)
        CASE_ICON(TitleBarCloseButton)
        CASE_ICON(TitleBarNormalButton)
    case SP_LineEditClearButton:
            return QIcon::fromTheme("button_edit-clear");
        break;
    default:
        break;
    }

    if (st < QStyle::SP_CustomBase) {
        return QCommonStyle::standardIcon(st, opt, widget);
    }

    return standardIcon(this, static_cast<DStyle::StandardPixmap>(st), opt, widget);
}

/*!
 * \~chinese \brief DStyle::generatedBrush获取一个加工后的画刷
 * \~chinese \param \sa option QStyleOption
 * \~chinese \param base 画刷
 * \~chinese \param \sa cg QPalette::ColorGroup
 * \~chinese \param \sa role QPalette::ColorRole
 * \~chinese \return 加工后的画刷
*/
QBrush DStyle::generatedBrush(const QStyleOption *option, const QBrush &base, QPalette::ColorGroup cg, QPalette::ColorRole role) const
{
    return generatedBrush(getState(option), option, base, cg, role);
}

/*!
 * \~chinese \brief DStyle::generatedBrush获取一个加工后的画刷
 * \~chinese \param \sa state DStyle::StyleState
 * \~chinese \param \sa option QStyleOption
 * \~chinese \param base 画刷
 * \~chinese \param \sa cg QPalette::ColorGroup
 * \~chinese \param \sa role QPalette::ColorRole
 * \~chinese \return 加工后的画刷
 */
QBrush DStyle::generatedBrush(DStyle::StyleState state, const QStyleOption *option, const QBrush &base, QPalette::ColorGroup cg, QPalette::ColorRole role) const
{
    if (auto proxy = qobject_cast<const DStyle *>(this->proxy())) {
        return proxy->generatedBrush(getFlags(option) | state, base, cg, role, option);
    }

    return generatedBrush(getFlags(option) | state, base, cg, role, option);
}

/*!
 * \~chinese \brief DStyle::generatedBrush获取一个加工后的画刷
 * \~chinese \param \sa flags DStyle::StateFlags
 * \~chinese \param base 画刷
 * \~chinese \param \sa cg QPalette::ColorGroup
 * \~chinese \param \sa role QPalette::ColorRole
 * \~chinese \param \sa option QStyleOption
 * \~chinese \return 加工后的画刷
 */
QBrush DStyle::generatedBrush(StateFlags flags, const QBrush &base, QPalette::ColorGroup cg, QPalette::ColorRole role, const QStyleOption *option) const
{
    Q_UNUSED(cg)

    QColor colorNew = base.color();

    if (!colorNew.isValid())
        return base;

    if ((flags & StyleState_Mask)  == SS_HoverState) {
        switch (role) {
        case QPalette::Button:
        case QPalette::Light:
        case QPalette::Dark:
            colorNew = adjustColor(colorNew, 0, 0, -10, 0, 0, 0, 0);
            break;
        case QPalette::Highlight:
            colorNew = adjustColor(colorNew, 0, 0, +20);
            break;
        case QPalette::ButtonText: {
            DGuiApplicationHelper::ColorType type = DGuiApplicationHelper::toColorType(option->palette);
            colorNew = adjustColor(colorNew, 0, 0, type == DGuiApplicationHelper::DarkType ? 20 : -50);
            break;
        }
        case QPalette::HighlightedText:
            colorNew = adjustColor(colorNew, 0, 0, 20);
            break;
        default:
            break;
        }

        return colorNew;
    } else if ((flags & StyleState_Mask) == SS_PressState) {
        QColor hightColor = option->palette.highlight().color();
        hightColor.setAlphaF(0.1);

        switch (role) {
        case QPalette::Button:
        case QPalette::Light: {
            colorNew = adjustColor(colorNew, 0, 0, -20, 0, 0, +20, 0);
            colorNew = blendColor(colorNew, hightColor);
            break;
        }
        case QPalette::Dark: {
            colorNew = adjustColor(colorNew, 0, 0, -15, 0, 0, +20, 0);
            colorNew = blendColor(colorNew, hightColor);
            break;
        }
        case QPalette::Highlight:
            colorNew = adjustColor(colorNew, 0, 0, -10);
            break;
        case QPalette::ButtonText:
            return option->palette.highlight();
        case QPalette::HighlightedText:
            colorNew = adjustColor(colorNew, 0, 0, 0, 0, 0, 0, -40);
            break;
        default:
            break;
        }

        return colorNew;
    }

    return base;
}

/*!
 * \~chinese \brief DStyle::generatedBrush获取一个加工后的画刷
 * \~chinese \param \sa option QStyleOption
 * \~chinese \param base 画刷
 * \~chinese \param \sa cg QPalette::ColorGroup
 * \~chinese \param \sa type DPalette::ColorType
 * \~chinese \return 加工后的画刷
 */
QBrush DStyle::generatedBrush(const QStyleOption *option, const QBrush &base, QPalette::ColorGroup cg, DPalette::ColorType type) const
{
    return generatedBrush(getState(option), option, base, cg, type);
}

/*!
 * \~chinese \brief DStyle::generatedBrush获取一个加工后的画刷
 * \~chinese \param \sa state DStyle::StyleState
 * \~chinese \param \sa option QStyleOption
 * \~chinese \param base 画刷
 * \~chinese \param \sa cg QPalette::ColorGroup
 * \~chinese \param \sa type DPalette::ColorType
 * \~chinese \return 加工后的画刷
 */
QBrush DStyle::generatedBrush(DStyle::StyleState state, const QStyleOption *option, const QBrush &base, QPalette::ColorGroup cg, DPalette::ColorType type) const
{
    if (auto proxy = qobject_cast<const DStyle *>(this->proxy())) {
        return proxy->generatedBrush(getFlags(option) | state, base, cg, type, option);
    }

    return generatedBrush(getFlags(option) | state, base, cg, type, option);
}

/*!
 * \~chinese \brief DStyle::generatedBrush获取一个加工后的画刷
 * \~chinese \param \sa flags DStyle::StateFlags
 * \~chinese \param base画刷
 * \~chinese \param \sa cg QPalette::ColorGroup
 * \~chinese \param \sa type DPalette::ColorType
 * \~chinese \param \sa option QStyleOption()
 * \~chinese \return 加工后的画刷
 */
QBrush DStyle::generatedBrush(StateFlags flags, const QBrush &base, QPalette::ColorGroup cg, DPalette::ColorType type, const QStyleOption *option) const
{
    Q_UNUSED(cg)
    Q_UNUSED(option)

    QColor colorNew = base.color();

    if (!colorNew.isValid())
        return base;

    if ((flags & StyleState_Mask)  == SS_HoverState) {
        switch (type) {
        case DPalette::LightLively:
            colorNew = adjustColor(colorNew, 0, 0, +30, 0, 0, 0, 0);
            break;
        case DPalette::DarkLively:
            colorNew = adjustColor(colorNew, 0, 0, +10, 0, 0, 0, 0);
            break;
        case DPalette::ItemBackground: {
            DGuiApplicationHelper::ColorType ct = DGuiApplicationHelper::toColorType(colorNew);
            colorNew = ct == DGuiApplicationHelper::LightType ? adjustColor(colorNew, 0, 0, -10, 0, 0, 0, +10)
                       : adjustColor(colorNew, 0, 0, +10, 0, 0, 0, +10);
            break;
        }
        case DPalette::TextWarning: {
            colorNew = adjustColor(colorNew, 0, 0, -10);
            break;
        }
        default:
            break;
        }

        return colorNew;
    } else if ((flags & StyleState_Mask) == SS_PressState) {
        switch (type) {
        case DPalette::LightLively:
            colorNew = adjustColor(colorNew, 0, 0, -30, 0, 0, 0, 0);
            break;
        case DPalette::DarkLively:
            colorNew = adjustColor(colorNew, 0, 0, -20, 0, 0, 0, 0);
            break;
        case DPalette::TextWarning: {
            colorNew = adjustColor(colorNew, 0, 0, -30);
            break;
        }
        default:
            break;
        }

        return colorNew;
    } else if ((flags & StyleState_Mask) == SS_NormalState) {
        switch (type) {
        case DPalette::LightLively:
            colorNew = adjustColor(colorNew, 0, 0, +40, 0, 0, 0, 0);
            break;
        case DPalette::DarkLively:
            colorNew = adjustColor(colorNew, 0, 0, +20, 0, 0, 0, 0);
            break;
        default:
            break;
        }

        return colorNew;
    }

    return base;
}

#if QT_CONFIG(itemviews)
/*!
 * \~chinese \brief DStyle::viewItemTextLayout视图项文本布局
 * \~chinese \param textLayout文本布局
 * \~chinese \param lineWidth 宽度
 * \~chinese \return 文本内容所占大小
 */
QSizeF DStyle::viewItemTextLayout(QTextLayout &textLayout, int lineWidth)
{
    qreal height = 0;
    qreal widthUsed = 0;
    textLayout.beginLayout();
    while (true) {
        QTextLine line = textLayout.createLine();
        if (!line.isValid())
            break;
        line.setLineWidth(lineWidth);
        line.setPosition(QPointF(0, height));
        height += line.height();
        widthUsed = qMax(widthUsed, line.naturalTextWidth());
    }
    textLayout.endLayout();
    return QSizeF(widthUsed, height);
}

/*!
 * \~chinese \brief DStyle::viewItemSize 视图项大小
 * \~chinese \param\sa style QStyle()
 * \~chinese \param\sa option QStyleOptionViewItem()
 * \~chinese \param \sa role Qt::ItemDataRole
 * \~chinese \return 文本内容所占大小
 */
QSize DStyle::viewItemSize(const QStyle *style, const QStyleOptionViewItem *option, int role)
{
    const QWidget *widget = option->widget;
    switch (role) {
    case Qt::CheckStateRole:
        if (option->features & QStyleOptionViewItem::HasCheckIndicator)
            return QSize(style->pixelMetric(QStyle::PM_IndicatorWidth, option, widget),
                         style->pixelMetric(QStyle::PM_IndicatorHeight, option, widget));
        break;
    case Qt::DisplayRole:
        if (option->features & QStyleOptionViewItem::HasDisplay) {
            QTextOption textOption;
            textOption.setWrapMode(QTextOption::WordWrap);
            QTextLayout textLayout(option->text, option->font);
            textLayout.setTextOption(textOption);
            const bool wrapText = option->features & QStyleOptionViewItem::WrapText;
            int spacing = DStyleHelper(style).pixelMetric(DStyle::PM_ContentsSpacing, option, widget);
            QRect bounds = option->rect;
            switch (option->decorationPosition) {
            case QStyleOptionViewItem::Left:
            case QStyleOptionViewItem::Right: {
                if (wrapText && bounds.isValid()) {
                    int width = bounds.width();
                    if (option->features & QStyleOptionViewItem::HasDecoration)
                        width -= option->decorationSize.width() + spacing;
                    bounds.setWidth(width);
                } else
                    bounds.setWidth(QFIXED_MAX);
                break;
            }
            case QStyleOptionViewItem::Top:
            case QStyleOptionViewItem::Bottom:
                if (wrapText)
                    bounds.setWidth(bounds.isValid() ? bounds.width() - spacing : option->decorationSize.width());
                else
                    bounds.setWidth(QFIXED_MAX);
                break;
            default:
                break;
            }

            if (wrapText && option->features & QStyleOptionViewItem::HasCheckIndicator)
                bounds.setWidth(bounds.width() - style->pixelMetric(QStyle::PM_IndicatorWidth) - spacing);

            const int lineWidth = bounds.width();
            const QSizeF size = viewItemTextLayout(textLayout, lineWidth);
            return QSize(qCeil(size.width()), qCeil(size.height()));
        }
        break;
    case Qt::DecorationRole:
        if (option->features & QStyleOptionViewItem::HasDecoration) {
            return option->decorationSize;
        }
        break;
    default:
        break;
    }

    return QSize(0, 0);
}

/*!
 * \~chinese \brief DStyle::viewItemLayout视图项组合(文字 + 图标 + 点击区域)
 * \~chinese \param \sa style QStyle()
 * \~chinese \param \sa opt QStyleOptionViewItem()
 * \~chinese \param pixmapRect 图标位置大小
 * \~chinese \param textRect 文字区域位置大小
 * \~chinese \param checkRect 点击区域位置大小
 * \~chinese \param sizehint 是否开启
 */
void DStyle::viewItemLayout(const QStyle *style, const QStyleOptionViewItem *opt,  QRect *pixmapRect, QRect *textRect, QRect *checkRect, bool sizehint)
{
    Q_ASSERT(checkRect && pixmapRect && textRect);
    *pixmapRect = QRect(QPoint(0, 0), viewItemSize(style, opt, Qt::DecorationRole));
    *textRect = QRect(QPoint(0, 0), viewItemSize(style, opt, Qt::DisplayRole));
    *checkRect = QRect(QPoint(0, 0), viewItemSize(style, opt, Qt::CheckStateRole));

    const DStyleHelper dstyle(style);
    const QWidget *widget = opt->widget;
    const bool hasCheck = checkRect->isValid();
    const bool hasPixmap = pixmapRect->isValid();
    const bool hasText = textRect->isValid();
    const int spacing = dstyle.pixelMetric(DStyle::PM_ContentsSpacing, opt, widget);
    const int x = opt->rect.left();
    const int y = opt->rect.top();
    int w, h;

    if (textRect->height() == 0 && (!hasPixmap || !sizehint)) {
        //if there is no text, we still want to have a decent height for the item sizeHint and the editor size
        textRect->setHeight(opt->fontMetrics.height());
    }

    QSize pm(0, 0);
    if (hasPixmap) {
        pm = pixmapRect->size();
    }
    if (sizehint) {
        h = qMax(checkRect->height(), qMax(textRect->height(), pm.height()));
        if (opt->decorationPosition == QStyleOptionViewItem::Left
                || opt->decorationPosition == QStyleOptionViewItem::Right) {
            w = textRect->width() + pm.width();

            if (hasText) {
                w += spacing; // add spacing
            }
        } else {
            w = qMax(textRect->width(), pm.width());
        }

        int cw = 0;
        QRect check;
        if (hasCheck) {
            cw = checkRect->width();
            w += cw + spacing;

            if (opt->direction == Qt::RightToLeft) {
                check.setRect(x + w - cw, y, cw, h);
            } else {
                check.setRect(x, y, cw, h);
            }
        }

        QRect display;
        QRect decoration;
        switch (opt->decorationPosition) {
        case QStyleOptionViewItem::Top: {
            h = textRect->height();

            if (opt->direction == Qt::RightToLeft) {
                decoration.setRect(x, y, w - cw, pm.height());
                display.setRect(x, y + pm.height() + spacing, w - cw, h);
            } else {
                decoration.setRect(x + cw, y, w - cw, pm.height());
                display.setRect(x + cw, y + pm.height() + spacing, w - cw, h);
            }
            break;
        }
        case QStyleOptionViewItem::Bottom: {
            h = textRect->height() + pm.height();

            if (opt->direction == Qt::RightToLeft) {
                display.setRect(x, y, w - cw, textRect->height());
                decoration.setRect(x, y + textRect->height() + spacing, w - cw, h - textRect->height());
            } else {
                display.setRect(x + cw, y, w - cw, textRect->height());
                decoration.setRect(x + cw, y + textRect->height() + spacing, w - cw, h - textRect->height());
            }
            break;
        }
        case QStyleOptionViewItem::Left: {
            if (opt->direction == Qt::LeftToRight) {
                decoration.setRect(x + cw, y, pm.width(), h);
                display.setRect(decoration.right() + spacing, y, w - pm.width() - cw, h);
            } else {
                display.setRect(x, y, w - pm.width() - cw, h);
                decoration.setRect(display.right() + spacing, y, pm.width(), h);
            }
            break;
        }
        case QStyleOptionViewItem::Right: {
            if (opt->direction == Qt::LeftToRight) {
                display.setRect(x + cw, y, w - pm.width() - cw, h);
                decoration.setRect(display.right() + spacing, y, pm.width(), h);
            } else {
                decoration.setRect(x, y, pm.width(), h);
                display.setRect(decoration.right() + spacing, y, w - pm.width() - cw, h);
            }
            break;
        }
        default:
            qWarning("doLayout: decoration position is invalid");
            decoration = *pixmapRect;
            break;
        }

        *checkRect = check;
        *pixmapRect = decoration;
        *textRect = display;
    } else {
        w = opt->rect.width();
        h = opt->rect.height();

        *pixmapRect = QStyle::alignedRect(opt->direction, opt->decorationAlignment,
                                          pixmapRect->size(), opt->rect);

        QRect display = opt->rect;

        switch (opt->decorationPosition) {
        case QStyleOptionViewItem::Top: {
            int residue_height = opt->rect.bottom() - pixmapRect->bottom();

            // 空间不够时，要改变图标的位置来腾出空间
            if (textRect->height() > residue_height) {
                pixmapRect->moveTop(qMax(0, pixmapRect->top() - textRect->height() + residue_height));
            }

            display.setTop(pixmapRect->bottom() + spacing);
            break;
        }
        case QStyleOptionViewItem::Bottom: {
            int residue_height = pixmapRect->top() - opt->rect.top();

            // 空间不够时，要改变图标的位置来腾出空间
            if (textRect->height() > residue_height) {
                pixmapRect->moveBottom(qMin(opt->rect.bottom(), pixmapRect->bottom() + textRect->height() - residue_height));
            }

            display.setBottom(pixmapRect->top() - spacing);
            break;
        }
        case QStyleOptionViewItem::Left:
        case QStyleOptionViewItem::Right:
            if (opt->decorationPosition == QStyleOptionViewItem::Left
                    && opt->direction == Qt::LeftToRight) {
                int residue_width = pixmapRect->left() - opt->rect.left();

                // 空间不够时，要改变图标的位置来腾出空间
                if (textRect->width() > residue_width) {
                    pixmapRect->moveLeft(qMax(opt->rect.left(), pixmapRect->left() - textRect->width() + residue_width));
                }

                display.setLeft(pixmapRect->right() + spacing);
            } else {
                int residue_width = opt->rect.right() - pixmapRect->left();

                // 空间不够时，要改变图标的位置来腾出空间
                if (textRect->width() > residue_width) {
                    pixmapRect->moveRight(qMin(opt->rect.right(), pixmapRect->right() + textRect->width() - residue_width));
                }

                display.setRight(pixmapRect->left() - spacing);
            }
        default:
            break;
        }

        if (opt->features & QStyleOptionViewItem::HasCheckIndicator) {
            *checkRect = QStyle::alignedRect(opt->direction, Qt::AlignRight | Qt::AlignVCenter, checkRect->size(), display);
        }

//        *textRect = QStyle::alignedRect(opt->direction, opt->displayAlignment, textRect->size(), display);

        if (opt->features & QStyleOptionViewItem::HasCheckIndicator)
            display.setRight(checkRect->left() - spacing);

        *textRect = display;
    }
}

/*!
 * \~chinese \brief DStyle::viewItemLayout
 * \~chinese \sa DStyle::viewItemLayout()
 */
void DStyle::viewItemLayout(const QStyleOptionViewItem *opt, QRect *pixmapRect, QRect *textRect, QRect *checkRect, bool sizehint) const
{
    viewItemLayout(this, opt, pixmapRect, textRect, checkRect, sizehint);
}

/*!
 * \~chinese \brief DStyle::viewItemDrawText视图项文字
 * \~chinese \param \sa style QStyle()
 * \~chinese \param p 画家
 * \~chinese \param \sa option QStyleOptionViewItem()
 * \~chinese \param rect 文字大小
 * \~chinese \return 布局大小
 */
QRect DStyle::viewItemDrawText(const QStyle *style, QPainter *p, const QStyleOptionViewItem *option, const QRect &rect)
{
    Q_UNUSED(style)
    QRect textRect = rect;
    const bool wrapText = option->features & QStyleOptionViewItem::WrapText;
    QTextOption textOption;
    textOption.setWrapMode(wrapText ? QTextOption::WordWrap : QTextOption::ManualWrap);
    textOption.setTextDirection(option->direction);
    textOption.setAlignment(QStyle::visualAlignment(option->direction, option->displayAlignment));
    QTextLayout textLayout(option->text, option->font);
    textLayout.setTextOption(textOption);

    viewItemTextLayout(textLayout, textRect.width());

    QString elidedText;
    qreal height = 0;
    qreal width = 0;
    int elidedIndex = -1;
    const int lineCount = textLayout.lineCount();
    for (int j = 0; j < lineCount; ++j) {
        const QTextLine line = textLayout.lineAt(j);
        if (j + 1 <= lineCount - 1) {
            const QTextLine nextLine = textLayout.lineAt(j + 1);
            if ((nextLine.y() + nextLine.height()) > textRect.height()) {
                int start = line.textStart();
                int length = line.textLength() + nextLine.textLength();
                const QStackTextEngine engine(textLayout.text().mid(start, length), option->font);
                elidedText = engine.elidedText(option->textElideMode, textRect.width());
                height += line.height();
                width = textRect.width();
                elidedIndex = j;
                break;
            }
        }
        if (line.naturalTextWidth() > textRect.width()) {
            int start = line.textStart();
            int length = line.textLength();
            const QStackTextEngine engine(textLayout.text().mid(start, length), option->font);
            elidedText = engine.elidedText(option->textElideMode, textRect.width());
            height += line.height();
            width = textRect.width();
            elidedIndex = j;
            break;
        }
        width = qMax<qreal>(width, line.width());
        height += line.height();
    }

    const QRect layoutRect = QStyle::alignedRect(option->direction, option->displayAlignment,
                                                 QSize(int(width), int(height)), textRect);
    const QPointF position = layoutRect.topLeft();
    for (int i = 0; i < lineCount; ++i) {
        const QTextLine line = textLayout.lineAt(i);
        if (i == elidedIndex) {
            qreal x = position.x() + line.x();
            qreal y = position.y() + line.y() + line.ascent();
            p->save();
            p->setFont(option->font);
            p->drawText(QPointF(x, y), elidedText);
            p->restore();
            break;
        }
        line.draw(p, position);
    }

    return layoutRect;
}

/*!
 * \~chinese \brief DStyle::viewItemDrawText
 * \~chinese \sa DStyle::viewItemDrawText()
 */
QRect DStyle::viewItemDrawText(QPainter *p, const QStyleOptionViewItem *option, const QRect &rect) const
{
    return viewItemDrawText(this, p, option, rect);
}
#endif


/*!
 * \~chinese \class DStyledIconEngine
 * \~chinese \brief DStyledIconEngine一个修改的 QIconEngine 类
*/

void DStyledIconEngine::drawIcon(const QIcon &icon, QPainter *pa, const QRectF &rect)
{
    icon.paint(pa, rect.toRect());
}

/*!
 * \~chinese \brief DStyledIconEngine::DStyledIconEngine
 * \~chinese \param drawFun
 * \~chinese \param iconName
 */
DStyledIconEngine::DStyledIconEngine(DrawFun drawFun, const QString &iconName)
    : QIconEngine()
    , m_drawFun(drawFun)
    , m_iconName(iconName)
{
    m_painterRole = DPalette::NoRole;
    m_widget = nullptr;
}

/*!
 * \~chinese \brief DStyledIconEngine::bindDrawFun活页夹
 * \~chinese \param drawFun
 */
void DStyledIconEngine::bindDrawFun(DrawFun drawFun)
{
    m_drawFun = drawFun;
}

/*!
 * \~chinese \brief DStyledIconEngine::setIconName设置icon名称
 * \~chinese \param name 名称
 */
void DStyledIconEngine::setIconName(const QString &name)
{
    m_iconName = name;
}

/*!
 * \~chinese \brief DStyledIconEngine::pixmap
 * \~chinese \sa QIconEngine::pixmap()
 */
QPixmap DStyledIconEngine::pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state)
{
    QImage image(size, QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::transparent);
    QPainter pa(&image);
    paint(&pa, QRect(QPoint(0, 0), size), mode, state);
    pa.end();

    return QPixmap::fromImage(image);
}

/*!
 * \~chinese \brief DStyledIconEngine::paint 重载paint
 * \~chinese \param painter画家
 * \~chinese \param palette调色板
 * \~chinese \param \sa rect QIconEngine::paint()
 */
void DStyledIconEngine::paint(QPainter *painter, const QPalette &palette, const QRectF &rect)
{
    if (!m_drawFun)
        return;

    painter->setBrush(palette.background());
    painter->setPen(QPen(palette.foreground(), painter->pen().widthF()));

    m_drawFun(painter, rect);
}

/*!
 * \~chinese \brief DStyledIconEngine::paint
 * \~chinese \sa QIconEngine::paint()
 */
void DStyledIconEngine::paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state)
{
    Q_UNUSED(mode)
    Q_UNUSED(state)

    if (m_painterRole != QPalette::NoRole) {
        if (m_widget) {
            painter->setPen(m_widget->palette().brush(m_painterRole).color());
            painter->setBrush(m_widget->palette().brush(m_painterRole));
        } else {
            painter->setPen(qApp->palette().brush(m_painterRole).color());
            painter->setBrush(qApp->palette().brush(m_painterRole));
        }
    }

    m_drawFun(painter, rect);
}

/*!
 * \~chinese \brief DStyledIconEngine::clone
 * \~chinese \sa QIconEngine::clone()
 */
QIconEngine *DStyledIconEngine::clone() const
{
    return new DStyledIconEngine(m_drawFun, m_iconName);
}

void DStyledIconEngine::setFrontRole(const QWidget *widget, QPalette::ColorRole role)
{
    m_painterRole = role;
    m_widget = widget;
}

void DStyledIconEngine::virtual_hook(int id, void *data)
{
    if (id == IconNameHook) {
        *reinterpret_cast<QString *>(data) = m_iconName;
    } else if (id == IsNullHook) {
        *reinterpret_cast<bool *>(data) = !m_drawFun;
    }

    return QIconEngine::virtual_hook(id, data);
}

DWIDGET_END_NAMESPACE
