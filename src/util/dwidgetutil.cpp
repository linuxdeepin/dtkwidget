/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "dwidgetutil.h"

#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QTextLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <qpa/qplatformwindow.h>

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

    QDesktopWidget *dw = QApplication::desktop();
    QRect parentRect = dw->availableGeometry(dw->primaryScreen());

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

bool startMoveWindow(QWindow *window, QPoint pos)
{
    if (Q_UNLIKELY(!window || !window->isVisible() || !window->handle()))
        return false;

#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    return window->handle()->startSystemMove(pos);
#else
    Q_UNUSED(pos);
    return window->handle()->startSystemMove();
#endif
}

DWIDGET_END_NAMESPACE
