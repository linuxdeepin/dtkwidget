// SPDX-FileCopyrightText: 2016 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "shortcutedit.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QKeySequence>
#include <QKeyEvent>

namespace
{

const int MAX_WIDTH = 238;
const int MAX_HEIGHT = 22 + 2 * 2;
const double BORDER_RADIUS = 3.0;
const QColor ACTIVE_BORDER_COLOR = QColor("#2ca7f8");
const QColor NORMAL_BORDER_COLOR = QColor(0, 0, 0, 255 * 8 / 100);

}

static bool isModifiersKey(int key)
{
    QList<int> keys;
    keys << Qt::Key_Shift;
    keys << Qt::Key_Control;
    keys << Qt::Key_Alt;
    keys << Qt::Key_Meta;

    return keys.contains(key);
}

class ShortcutEditPrivate
{
public:
    ShortcutEditPrivate(ShortcutEdit *parent) : q_ptr(parent) {}

    QSize stringSize(const QString &str) const;
    QRectF drawTextRect(const QRect &lastRect, const QString &str, QPainter &painter) const;

    Qt::KeyboardModifiers   keyModifiers    = Qt::NoModifier;
    Qt::Key                 key             = Qt::Key_unknown;
    QStringList             keyStringList;
    bool                    canSet          = true;

    QColor          borderColor = NORMAL_BORDER_COLOR;

    ShortcutEdit *q_ptr;
    Q_DECLARE_PUBLIC(ShortcutEdit)
};

ShortcutEdit::ShortcutEdit(QWidget *parent) : QWidget(parent), d_ptr(new ShortcutEditPrivate(this))
{
    setFocusPolicy(Qt::ClickFocus);
}

ShortcutEdit::~ShortcutEdit()
{

}

void ShortcutEdit::setShortCut(Qt::KeyboardModifiers modifier, Qt::Key key)
{
    Q_D(ShortcutEdit);
    d->keyModifiers = modifier;
    d->key = key;

    d->keyStringList.clear();
    if (d->keyModifiers != Qt::NoModifier) {
        d->keyStringList << QKeySequence(d->keyModifiers).toString().split("+", QString::SkipEmptyParts);
    }
    if (d->key != Qt::Key_unknown) {
        d->keyStringList << QKeySequence(d->key).toString();
    }
}

void ShortcutEdit::setShortCut(const QString &sequenceString)
{
    Q_D(ShortcutEdit);

    // replcae ++ to sep_plus
    const QString sepString = "sep_str";
    const QString plusString = "sep_plus";
    QString formatSequenceString = QString(sequenceString).replace("++", plusString);
    formatSequenceString = formatSequenceString.replace("+", sepString);
    formatSequenceString = formatSequenceString.replace(plusString, sepString + "+");
    d->keyStringList = formatSequenceString.split(sepString);
}

void ShortcutEdit::paintEvent(QPaintEvent *)
{
    Q_D(const ShortcutEdit);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

    if (d->keyStringList.isEmpty()) {
        // Draw tips
        QRect tR(6, 5, width(), height());
        QFont f;
        f.setPixelSize(11);
        painter.setFont(f);
        painter.drawText(tR, tr("Please enter a new shortcut"));
    } else {
        QRectF lastRect(1, 0, 0, 0);
        for (QString key : d->keyStringList) {
            painter.save();
            lastRect = d->drawTextRect(lastRect.toRect(), key, painter);
            painter.restore();
        }
    }

    auto borderRadius = BORDER_RADIUS;
    auto penWidthf = 1.0;
    QPainterPath borderPath;
    QRectF borderRect = QRectF(rect());
    QMarginsF borderMargin(penWidthf / 2, penWidthf / 2, penWidthf / 2, penWidthf / 2);
    borderRadius -= penWidthf / 2;
    borderRect = borderRect.marginsRemoved(borderMargin);
    borderPath.addRoundedRect(borderRect, borderRadius, borderRadius);
    QPen borderPen(d->borderColor);
    borderPen.setWidthF(penWidthf);
    painter.strokePath(borderPath, borderPen);
}

void ShortcutEdit::focusInEvent(QFocusEvent *e)
{
    Q_UNUSED(e)
    Q_D(ShortcutEdit);
    d->borderColor = ACTIVE_BORDER_COLOR;
    this->update();
}

void ShortcutEdit::focusOutEvent(QFocusEvent *e)
{
    Q_UNUSED(e)
    Q_D(ShortcutEdit);
    d->borderColor = NORMAL_BORDER_COLOR;
    this->update();
}

void ShortcutEdit::keyPressEvent(QKeyEvent *e)
{
    Q_D(ShortcutEdit);

    if (e->key() != Qt::Key_Backspace) {
        if (isModifiersKey(e->key()) || ! d->canSet) {
            return;
        }
        d->canSet = true;
        if (e->key() == Qt::Key_Delete) {
            d->keyModifiers = e->modifiers();
            d->key = static_cast<Qt::Key>(e->key());
        } else {
            d->keyModifiers = e->modifiers();
            d->key = static_cast<Qt::Key>(e->key());
        }
        d->keyStringList.clear();
        if (d->keyModifiers != Qt::NoModifier) {
            d->keyStringList << QKeySequence(d->keyModifiers).toString().split("+", QString::SkipEmptyParts);
        }
        if (d->key != Qt::Key_unknown) {
            d->keyStringList << QKeySequence(d->key).toString();
        }
    } else {
        d->canSet = true;
        d->key = Qt::Key_unknown;
        d->keyModifiers = Qt::NoModifier;
        d->keyStringList.clear();
    }

    notifyShortcutChanged(d->keyModifiers, d->key);

    this->update();
}

void ShortcutEdit::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_D(ShortcutEdit);
    if (event->button() == Qt::LeftButton) {
        d->canSet = true;
        d->key = Qt::Key_unknown;
        d->keyModifiers = Qt::NoModifier;
        notifyShortcutChanged(d->keyModifiers, d->key);
    }
    this->update();
}

void ShortcutEdit::notifyShortcutChanged(Qt::KeyboardModifiers modifier, Qt::Key key)
{
    QKeySequence modifierSeq(modifier);
    QKeySequence keySeq(key);
    QString seqString = modifierSeq.toString() + keySeq.toString();
    Q_EMIT shortcutStringChanged(seqString);
    Q_EMIT shortcutChanged(modifier, key);
}

QSize ShortcutEditPrivate::stringSize(const QString &str) const
{
    QFont f;
    f.setPixelSize(10);
    QFontMetrics fm(f);
    int w = fm.boundingRect(str).width();
    int h = fm.height();

    return QSize(w, h);
}

QRectF ShortcutEditPrivate::drawTextRect(const QRect &lastRect, const QString &str, QPainter &painter) const
{
    const QSize ss = stringSize(str);
    QRectF r(lastRect.x() + lastRect.width() + 5, 2 + 2,
             ss.width() + 6 * 2, 18);

    QPainterPath bp;
    bp.addRoundedRect(r, BORDER_RADIUS, BORDER_RADIUS);
    painter.setClipPath(bp);

    painter.fillRect(r, QColor(105, 170, 255, 255 * 0.15));

    // Draw inside border
    painter.save();
    painter.setPen(QPen(QColor(95, 159, 217, 255 * 0.3), 1));
    QPainterPathStroker stroker;
    stroker.setWidth(1.0);
    stroker.setJoinStyle(Qt::RoundJoin);
    QPainterPath borderPath = stroker.createStroke(bp);
    painter.drawPath(borderPath);
    painter.restore();

    // Draw text
    QRectF tR(r.x() + 6, 5, r.width(), r.height());
    QFont f;
    f.setPixelSize(10);
    painter.setFont(f);
    painter.drawText(tR, str);

    return r;
}
