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
