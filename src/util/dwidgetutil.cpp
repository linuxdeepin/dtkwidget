// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dtkwidget/util/dwidgetutil.h"

#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QTextLayout>
#include <QApplication>
#include <QDesktopWidget>

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

DWIDGET_END_NAMESPACE
