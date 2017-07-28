/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DUTILITY_H
#define DUTILITY_H

#include "dtkwidget_global.h"

#include <QImage>
#include <QColor>
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

void moveToCenter(QWidget *w);

DWIDGET_END_NAMESPACE

#endif // DUTILITY_H
