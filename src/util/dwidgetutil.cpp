// SPDX-FileCopyrightText: 2017 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dwidgetutil.h"
#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QTextLayout>
#include <QApplication>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QDesktopWidget>
#endif
#include <QScreen>

QT_BEGIN_NAMESPACE
//extern Q_WIDGETS_EXPORT void qt_blurImage(QImage &blurImage, qreal radius, bool quality, int transposed = 0);
extern Q_WIDGETS_EXPORT void qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

QImage dropShadow(const QPixmap &px, qreal radius, const QColor &color)
{
    if (px.isNull()) {
        return QImage();
    }

    QSize size = px.size();

    QImage tmp(size + QSize(radius * 2, radius * 2), QImage::Format_ARGB32_Premultiplied);
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

    if (color == QColor(Qt::black)) {
        return blurred;
    }

    tmp = blurred;

    // blacken the image...
    tmpPainter.begin(&tmp);
    tmpPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    tmpPainter.fillRect(tmp.rect(), color);
    tmpPainter.end();

    return tmp;
}

void moveToCenter(QWidget *w)
{
    Q_ASSERT(w != nullptr);
#if QT_VERSION < QT_VERSION_CHECK(5, 11, 0)
    QDesktopWidget *dw = QApplication::desktop();
    QRect parentRect = dw->availableGeometry(dw->primaryScreen());
#else
    auto parentRect = QGuiApplication::primaryScreen()->availableGeometry();
#endif

    if (w->parentWidget()) {
        parentRect = w->parentWidget()->geometry();
    }

    w->move(parentRect.center() - w->rect().center());
}

QIcon getCircleIcon(const QPixmap &pixmap, int diameter)
{
    QPixmap output(QSize(diameter, diameter));
    output.fill(Qt::transparent);
    QPainter painter(&output);
    // 抗锯齿 + 平滑边缘处理
    painter.setRenderHints(QPainter::Antialiasing, true);
    painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
    painter.setPen(Qt::NoPen);

    QPainterPath path;
    path.addEllipse(0, 0, diameter, diameter);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, diameter, diameter, pixmap);
    return QIcon(output);
}

QIcon getCircleIcon(const QIcon &icon, int diameter)
{
    QPixmap pixmap = icon.pixmap(QSize(diameter, diameter));
    return getCircleIcon(pixmap, diameter);
}

// 取自Qt源码qpixmapfilter.cpp 945行
void grayScale(const QImage &image, QImage &dest, const QRect &rect)
{
    QRect destRect = rect;
    QRect srcRect = rect;
    if (rect.isNull()) {
        srcRect = dest.rect();
        destRect = dest.rect();
    }
    if (&image != &dest) {
        destRect.moveTo(QPoint(0, 0));
    }

    const unsigned int *data = reinterpret_cast<const unsigned int *>(image.bits());
    unsigned int *outData = reinterpret_cast<unsigned int *>(dest.bits());

    if (dest.size() == image.size() && image.rect() == srcRect) {
        // a bit faster loop for grayscaling everything
        int pixels = dest.width() * dest.height();
        for (int i = 0; i < pixels; ++i) {
            int val = qGray(data[i]);
            outData[i] = qRgba(val, val, val, qAlpha(data[i]));
        }
    } else {
        int yd = destRect.top();
        for (int y = srcRect.top(); y <= srcRect.bottom() && y < image.height(); y++) {
            data = reinterpret_cast<const unsigned int *>(image.scanLine(y));
            outData = reinterpret_cast<unsigned int *>(dest.scanLine(yd++));
            int xd = destRect.left();
            for (int x = srcRect.left(); x <= srcRect.right() && x < image.width(); x++) {
                int val = qGray(data[x]);
                outData[xd++] = qRgba(val, val, val, qAlpha(data[x]));
            }
        }
    }
}

DWIDGET_END_NAMESPACE
