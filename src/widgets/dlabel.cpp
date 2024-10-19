// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dlabel.h"
#include "private/dlabel_p.h"
#include "dtooltip.h"

#include <DPaletteHelper>

#include <private/qlabel_p.h>

DWIDGET_BEGIN_NAMESPACE

/*!
@~english
  \class Dtk::Widget::DLabel
  \inmodule dtkwidget
  @brief DLabel a re-implementation QLabel.

  DLabel provides a function to display the DLabel at a specified location
  DLabel provides a function to change the font color
 */

/*!
@~english
  @brief Constructor for DLabel.
  \a parent The argument is sent to the QLabel constructor.
 */
DLabel::DLabel(QWidget *parent, Qt::WindowFlags f)
    : QLabel(parent, f)
    , DObject(*new DLabelPrivate(this))
{
    D_D(DLabel);
    d->init();
}

/*!
@~english
  @brief Constructor for DLabel.
  \a text Text message
  \a parent Specifying the parent object.
 */
DLabel::DLabel(const QString &text, QWidget *parent)
    : QLabel(text, parent)
    , DObject(*new DLabelPrivate(this))
{
    D_D(DLabel);
    d->init();
}

DLabel::~DLabel()
{

}

/*!
@~english
  @brief DLabel::setForegroundRole The font color displayed
  \a role Font color（QPalette::ColorRole）
 */
void DLabel::setForegroundRole(QPalette::ColorRole role)
{
    D_D(DLabel);

    d->color = DPalette::NoType;
    QLabel::setForegroundRole(role);
}

/*!
@~english
  @brief DLabel::setForegroundRole The font color displayed
  \a color Font color
 */
void DLabel::setForegroundRole(DPalette::ColorType color)
{
    D_D(DLabel);
    d->color = color;
}

/*!
@~english
  @brief DLabel::setElideMode Set the mode of ellipsis display
  \a elideMode Omitted schema enumeration
 */
void DLabel::setElideMode(Qt::TextElideMode elideMode)
{
    D_D(DLabel);
    if (d->elideMode == elideMode) {
        return;
    }
    d->elideMode = elideMode;
    update();
}

/*!
@~english
  @brief DLabel::elideMode Gets the pattern of the ellipsis
  \return Returns the pattern of ellipses
 */
Qt::TextElideMode DLabel::elideMode() const
{
    D_DC(DLabel);
    return d->elideMode;
}

/*!
@~english
  @brief DLabel::DLabel Constructor function
  \a dd Private class member variables
  \a parent Parent control
 */
DLabel::DLabel(DLabelPrivate &dd, QWidget *parent)
    : QLabel(parent)
    , DObject(dd)
{
    dd.init();
}

/*!
@~english
  @brief DLabel::initPainter Initialization painter
  \a painter painter parameter
 */
void DLabel::initPainter(QPainter *painter) const
{
    D_DC(DLabel);
    QLabel::initPainter(painter);
    if (d->color != DPalette::NoType) {
        QBrush color = DPaletteHelper::instance()->palette(this).brush(d->color);
        painter->setPen(QPen(color.color()));
    }
}

/*!
@~english
  @brief DLabel::paintEvent
  \a event Message event
  \sa QLabel::paintEvent()
 */
void DLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QLabelPrivate *d = static_cast<QLabelPrivate*>(d_ptr.data());
    QStyle *style = QWidget::style();
    QPainter painter(this);
    drawFrame(&painter);
    QRect cr = contentsRect();
    cr.adjust(d->margin, d->margin, -d->margin, -d->margin);
    int align = QStyle::visualAlignment(d->isTextLabel ? DLabelPrivate::textDirection(d)
                                                       : layoutDirection(), QFlag(d->align));

#if QT_CONFIG(movie)
    if (d->movie && !d->movie->currentPixmap().isNull()) {
        if (d->scaledcontents)
            style->drawItemPixmap(&painter, cr, align, d->movie->currentPixmap().scaled(cr.size()));
        else
            style->drawItemPixmap(&painter, cr, align, d->movie->currentPixmap());
    }
    else
#endif
    if (d->isTextLabel) {
        QRectF lr = DLabelPrivate::layoutRect(d).toAlignedRect();
        QStyleOption opt;
        opt.initFrom(this);

        if (d->control) {
#ifndef QT_NO_SHORTCUT
            const bool underline = (bool)style->styleHint(QStyle::SH_UnderlineShortcut, 0, this, 0);
            if (d->shortcutId != 0
                && underline != d->shortcutCursor.charFormat().fontUnderline()) {
                QTextCharFormat fmt;
                fmt.setFontUnderline(underline);
                d->shortcutCursor.mergeCharFormat(fmt);
            }
#endif
            DLabelPrivate::ensureTextLayouted(d);
            QAbstractTextDocumentLayout::PaintContext context;
            // Adjust the palette
            context.palette = opt.palette;

            if (d_func()->color != DPalette::NoType) {
                context.palette.setBrush(QPalette::Text, DPaletteHelper::instance()->palette(this).brush(d_func()->color));
            } else if (foregroundRole() != QPalette::Text && isEnabled()) {
                context.palette.setColor(QPalette::Text, context.palette.color(foregroundRole()));
            }

            painter.save();
            painter.translate(lr.topLeft());
            painter.setClipRect(lr.translated(-lr.x(), -lr.y()));
            d->control->setPalette(context.palette);
            d->control->drawContents(&painter, QRectF(), this);
            painter.restore();
        } else {
            int flags = align | (DLabelPrivate::textDirection(d) == Qt::LeftToRight ? Qt::TextForceLeftToRight
                                                                                    : Qt::TextForceRightToLeft);
            if (d->hasShortcut) {
                flags |= Qt::TextShowMnemonic;
                if (!style->styleHint(QStyle::SH_UnderlineShortcut, &opt, this))
                    flags |= Qt::TextHideMnemonic;
            }

            QPalette palette = opt.palette;

            if (d_func()->color != DPalette::NoType) {
                palette.setBrush(foregroundRole(), DPaletteHelper::instance()->palette(this).brush(d_func()->color));
            }

            QString text = d->text;
            if (elideMode() != Qt::ElideNone) {
                const QFontMetrics fm(fontMetrics());
                text = fm.elidedText(text, elideMode(), width(), Qt::TextShowMnemonic);
            }
            const DToolTip::ToolTipShowMode &toolTipShowMode = DToolTip::toolTipShowMode(this);
            if (toolTipShowMode != DToolTip::Default) {
                const bool showToolTip = (toolTipShowMode == DToolTip::AlwaysShow)
                        || ((toolTipShowMode == DToolTip::ShowWhenElided) && (d->text != text));
                if (DToolTip::needUpdateToolTip(this, showToolTip)) {
                    QString toolTip;
                    if (showToolTip) {
                        QTextOption textOption;
                        textOption.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
                        textOption.setTextDirection(opt.direction);
                        textOption.setAlignment(Qt::Alignment(align));
                        toolTip = DToolTip::wrapToolTipText(d->text, textOption);
                    }
                    this->setToolTip(toolTip);
                    DToolTip::setShowToolTip(this, showToolTip);
                }
            }
            style->drawItemText(&painter, lr.toRect(), flags, palette, isEnabled(), text, foregroundRole());
        }
    } else
#ifndef QT_NO_PICTURE
    if (d->picture) {
        QRect br = d->picture->boundingRect();
        int rw = br.width();
        int rh = br.height();
        if (d->scaledcontents) {
            painter.save();
            painter.translate(cr.x(), cr.y());
            painter.scale((double)cr.width()/rw, (double)cr.height()/rh);
            painter.drawPicture(-br.x(), -br.y(), *d->picture);
            painter.restore();
        } else {
            int xo = 0;
            int yo = 0;
            if (align & Qt::AlignVCenter)
                yo = (cr.height()-rh)/2;
            else if (align & Qt::AlignBottom)
                yo = cr.height()-rh;
            if (align & Qt::AlignRight)
                xo = cr.width()-rw;
            else if (align & Qt::AlignHCenter)
                xo = (cr.width()-rw)/2;
            painter.drawPicture(cr.x()+xo-br.x(), cr.y()+yo-br.y(), *d->picture);
        }
    } else
#endif
    if (d->pixmap && !d->pixmap->isNull()) {
        QPixmap pix;
        if (d->scaledcontents) {
            QSize scaledSize = cr.size() * devicePixelRatioF();
            if (!d->scaledpixmap || d->scaledpixmap->size() != scaledSize) {
#if QT_VERSION < QT_VERSION_CHECK(6, 8, 0)
                if (!d->cachedimage)
#if QT_VERSION < QT_VERSION_CHECK(6, 4, 2)
                    d->cachedimage = new QImage(d->pixmap->toImage());
                delete d->scaledpixmap;
#else
                    d->cachedimage = QImage(d->pixmap->toImage());
                d->scaledpixmap.reset();
#endif
                QImage scaledImage =
                    d->cachedimage->scaled(scaledSize,
#else
                d->scaledpixmap.reset();
                d->scaledpixmap = d->pixmap->scaled(scaledSize,
#endif
                                           Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
#if QT_VERSION < QT_VERSION_CHECK(6, 8, 0)
#if QT_VERSION < QT_VERSION_CHECK(6, 4, 2)
                d->scaledpixmap = new QPixmap(QPixmap::fromImage(scaledImage));
#else
                d->scaledpixmap = QPixmap(QPixmap::fromImage(scaledImage));
#endif
#endif
                d->scaledpixmap->setDevicePixelRatio(devicePixelRatioF());
            }
            pix = *d->scaledpixmap;
        } else
            pix = *d->pixmap;
        QStyleOption opt;
        opt.initFrom(this);
        if (!isEnabled())
            pix = style->generatedIconPixmap(QIcon::Disabled, pix, &opt);
        style->drawItemPixmap(&painter, cr, align, pix);
    }
}

DLabelPrivate::DLabelPrivate(DLabel *q)
    : DObjectPrivate(q)
{
}

void DLabelPrivate::init()
{

}

Qt::LayoutDirection DLabelPrivate::textDirection(QLabelPrivate *d)
{
    if (d->control) {
        QTextOption opt = d->control->document()->defaultTextOption();
        return opt.textDirection();
    }

    return d->text.isRightToLeft() ? Qt::RightToLeft : Qt::LeftToRight;
}

QRectF DLabelPrivate::documentRect(QLabelPrivate *d)
{
    QLabel *q = qobject_cast<QLabel*>(d->q_ptr);
    Q_ASSERT_X(d->isTextLabel, "documentRect", "document rect called for label that is not a text label!");
    QRect cr = q->contentsRect();
    cr.adjust(d->margin, d->margin, -d->margin, -d->margin);
    const int align = QStyle::visualAlignment(d->isTextLabel ? textDirection(d)
                                                             : q->layoutDirection(), QFlag(d->align));
    int m = d->indent;
    if (m < 0 && q->frameWidth()) // no indent, but we do have a frame
        m = q->fontMetrics().horizontalAdvance(QLatin1Char('x')) / 2 - d->margin;
    if (m > 0) {
        if (align & Qt::AlignLeft)
            cr.setLeft(cr.left() + m);
        if (align & Qt::AlignRight)
            cr.setRight(cr.right() - m);
        if (align & Qt::AlignTop)
            cr.setTop(cr.top() + m);
        if (align & Qt::AlignBottom)
            cr.setBottom(cr.bottom() - m);
    }
    return cr;
}

QRectF DLabelPrivate::layoutRect(QLabelPrivate *d)
{
    QRectF cr = documentRect(d);
    if (!d->control)
        return cr;
    ensureTextLayouted(d);
    // Caculate y position manually
    qreal rh = d->control->document()->documentLayout()->documentSize().height();
    qreal yo = 0;
    if (d->align & Qt::AlignVCenter)
        yo = qMax((cr.height()-rh)/2, qreal(0));
    else if (d->align & Qt::AlignBottom)
        yo = qMax(cr.height()-rh, qreal(0));
    return QRectF(cr.x(), yo + cr.y(), cr.width(), cr.height());
}

void DLabelPrivate::ensureTextLayouted(QLabelPrivate *d)
{
    if (d->textLayoutDirty) {
        if (d->textDirty) {
            if (d->control) {
                QTextDocument *doc = d->control->document();
                if (d->textDirty) {
        #ifndef QT_NO_TEXTHTMLPARSER
        #if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
                    if (d->textformat == Qt::TextFormat::RichText)
        #else
                    if (d->isRichText)
        #endif
                        doc->setHtml(d->text);
                    else
                        doc->setPlainText(d->text);
        #else
                    doc->setPlainText(d->text);
        #endif
                    doc->setUndoRedoEnabled(false);

        #ifndef QT_NO_SHORTCUT
                    if (d->hasShortcut) {
                        // Underline the first character that follows an ampersand (and remove the others ampersands)
                        int from = 0;
                        bool found = false;
                        QTextCursor cursor;
                        while (!(cursor = d->control->document()->find((QLatin1String("&")), from)).isNull()) {
                            cursor.deleteChar(); // remove the ampersand
                            cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
                            from = cursor.position();
                            if (!found && cursor.selectedText() != QLatin1String("&")) { //not a second &
                                found = true;
                                d->shortcutCursor = cursor;
                            }
                        }
                    }
        #endif
                }
            }
            d->textDirty = false;
        }

        if (d->control) {
            QTextDocument *doc = d->control->document();
            QTextOption opt = doc->defaultTextOption();

            opt.setAlignment(QFlag(d->align));

            if (d->align & Qt::TextWordWrap)
                opt.setWrapMode(QTextOption::WordWrap);
            else
                opt.setWrapMode(QTextOption::ManualWrap);

            doc->setDefaultTextOption(opt);

            QTextFrameFormat fmt = doc->rootFrame()->frameFormat();
            fmt.setMargin(0);
            doc->rootFrame()->setFrameFormat(fmt);
            doc->setTextWidth(DLabelPrivate::documentRect(d).width());
        }
        d->textLayoutDirty = false;
    }
}

DWIDGET_END_NAMESPACE
