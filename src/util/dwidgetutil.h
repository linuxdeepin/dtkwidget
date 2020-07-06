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

#ifndef DUTILITY_H
#define DUTILITY_H

#include "dtkwidget_global.h"

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
