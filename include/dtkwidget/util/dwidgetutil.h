// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DUTILITY_H
#define DUTILITY_H

#include <dtkwidget_global.h>

#include <QImage>
#include <QColor>
#include <QIcon>
#include <QTextOption>

DWIDGET_BEGIN_NAMESPACE

QImage dropShadow(const QPixmap &px, qreal radius, const QColor &color = Qt::black);

QStringList wordWrapText(const QString &text, int width,
                         QTextOption::WrapMode wrapMode,
                         int *lineCount = 0);

QStringList elideText(const QString &text, const QSize &size,
                      const QFontMetrics &fontMetrics,
                      QTextOption::WrapMode wordWrap,
                      Qt::TextElideMode mode,
                      int flags = 0);

QIcon getCircleIcon(const QPixmap &pixmap, int diameter = 36);
QIcon getCircleIcon(const QIcon &icon, int diameter = 36);

void moveToCenter(QWidget *w);

DWIDGET_END_NAMESPACE

#endif // DUTILITY_H
