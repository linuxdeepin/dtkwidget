/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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
#include "dcrumbedit.h"
#include "dobject_p.h"

#include <QAbstractTextDocumentLayout>
#include <QPainter>
#include <QTextBlock>
#include <QStyleOptionFrame>
#include <QMouseEvent>
#include <QMimeData>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

DWIDGET_BEGIN_NAMESPACE

class CrumbObjectInterface : public QObject, public QTextObjectInterface
{
    Q_OBJECT
    Q_INTERFACES(QTextObjectInterface)

public:
    explicit CrumbObjectInterface(QObject *parent = 0)
        : QObject(parent)
    {

    }

    QSizeF intrinsicSize(QTextDocument *doc, int posInDocument,
                         const QTextFormat &format) Q_DECL_OVERRIDE;

    void drawObject(QPainter *painter, const QRectF &rect, QTextDocument *doc,
                    int posInDocument, const QTextFormat &format) Q_DECL_OVERRIDE;
};

DCrumbTextFormat::DCrumbTextFormat()
    : QTextCharFormat(QTextFormat(QTextFormat::InvalidFormat))
{

}

QColor DCrumbTextFormat::tagColor() const
{
    return colorProperty(QTextFormat::UserProperty);
}

void DCrumbTextFormat::setTagColor(const QColor &color)
{
    setProperty(QTextFormat::UserProperty, color);
}

QString DCrumbTextFormat::text() const
{
    return stringProperty(QTextFormat::UserProperty + 1);
}

void DCrumbTextFormat::setText(const QString &text)
{
    setProperty(QTextFormat::UserProperty + 1, text);
}

QColor DCrumbTextFormat::textColor() const
{
    return colorProperty(QTextFormat::UserProperty + 2);
}

void DCrumbTextFormat::setTextColor(const QColor &color)
{
    setProperty(QTextFormat::UserProperty + 2, color);
}

QBrush DCrumbTextFormat::background() const
{
    return brushProperty(QTextFormat::UserProperty + 3);
}

void DCrumbTextFormat::setBackground(const QBrush &background)
{
    setProperty(QTextFormat::UserProperty + 3, background);
}

int DCrumbTextFormat::backgroundRadius() const
{
    return intProperty(QTextFormat::UserProperty + 4);
}

void DCrumbTextFormat::setBackgroundRadius(int radius)
{
    setProperty(QTextFormat::UserProperty + 4, radius);
}

DCrumbTextFormat::DCrumbTextFormat(int objectType)
{
    setObjectType(objectType);
    setBackground(QColor("#76c1ff"));
    setTextColor(Qt::white);
    setVerticalAlignment(QTextCharFormat::AlignBaseline);
}

DCrumbTextFormat::DCrumbTextFormat(const QTextFormat &fmt)
    : QTextCharFormat(fmt)
{

}

class DCrumbEditPrivate : public DCORE_NAMESPACE::DObjectPrivate
{
    D_DECLARE_PUBLIC(DCrumbEdit)

public:
    DCrumbEditPrivate(DCrumbEdit *qq)
        : DObjectPrivate(qq)
        , object(new CrumbObjectInterface(qq))
    {

    }

    void registerHandler(QAbstractTextDocumentLayout *layout)
    {
        D_Q(DCrumbEdit);

        for (int i = QTextFormat::UserObject + 1; ; ++i) {
            if (!layout->handlerForObject(i)) {
                objectType = i;
                break;
            }
        }

        layout->registerHandler(objectType, object);
    }

    bool canAddCrumb(const QString &text) const
    {
        return !formats.contains(text);
    }

    void makeCrumb(QTextCursor &cursor, const QString &text)
    {
        const QString tag_text = text.simplified();

        if (tag_text.isEmpty())
            return;

        if (!canAddCrumb(tag_text))
            return;

        D_Q(DCrumbEdit);

        DCrumbTextFormat format = q->makeTextFormat();

        format.setText(tag_text);
        cursor.insertText(QString(QChar::ObjectReplacementCharacter), format);
    }

    bool makeCrumb()
    {
        D_Q(DCrumbEdit);

        QString text = q->toPlainText().remove(QChar::ObjectReplacementCharacter);

        if (text.isEmpty())
            return false;

        QTextCursor cursor = q->document()->find(text);

        if (cursor.isNull())
            return false;

        if (splitter.isEmpty()) {
            makeCrumb(cursor, text);

            return true;
        }

        for (const QString &tag_text : text.split(splitter)) {
            makeCrumb(cursor, tag_text);
        }

        return true;
    }

    bool editCrumb(const QPoint &mousePos)
    {
        D_Q(DCrumbEdit);

        QTextCursor cursor = q->cursorForPosition(mousePos);

        if (cursor.charFormat().objectType() != objectType)
            return false;

        DCrumbTextFormat format(cursor.charFormat());

        if (format.text().isEmpty())
            return false;

        makeCrumb();

        if (mousePos.x() < q->cursorRect().left())
            cursor.setPosition(cursor.position() - 1, QTextCursor::KeepAnchor);
        else
            cursor.setPosition(cursor.position() + 1, QTextCursor::KeepAnchor);

        cursor.insertText(format.text());

        return true;
    }

    QByteArray crumbMapToData(const QList<DCrumbTextFormat> &list) const
    {
        QJsonArray array;

        for (const DCrumbTextFormat &f : list) {
            if (!f.isValid() || f.isEmpty())
                continue;

            QJsonObject object;
            object["text"] = f.text();

            if (f.tagColor().isValid())
                object["tag color"] = f.tagColor().name();

            array.append(QJsonValue(object));
        }

        QJsonDocument document;
        document.setArray(array);

        return document.toJson(QJsonDocument::Compact);
    }

    QList<DCrumbTextFormat> crumbListFromData(const QByteArray &format) const
    {
        const QJsonDocument &document = QJsonDocument::fromJson(format);
        const QJsonArray &array = document.array();

        QList<DCrumbTextFormat> list;

        D_QC(DCrumbEdit);

        for (const QJsonValue &v : array) {
            const QJsonObject &object = v.toObject();

            if (object.isEmpty())
                continue;

            DCrumbTextFormat format = q->makeTextFormat();

            format.setText(object["text"].toString());

            if (object.contains("tag color"))
                format.setTagColor(QColor(object["tag color"].toString()));

            list << format;
        }

        return list;
    }

    void _q_onDocumentLayoutChanged()
    {
        D_Q(DCrumbEdit);

        if (!q->sender() || q->sender() == q->document())
            registerHandler(q->document()->documentLayout());
    }

    void _q_onCurrentPositionChanged()
    {
        D_QC(DCrumbEdit);

        QTextCursor cursor = q->textCursor();

        if (cursor.atEnd()) {
            return;
        }

        cursor.setPosition(cursor.position() + 1);

        if (cursor.charFormat().objectType() != objectType)
            return;

        if (q->textCursor().charFormat().objectType() == objectType)
            makeCrumb();
    }

    void _q_onTextChanged()
    {
        D_Q(DCrumbEdit);

        bool formatsChanged = false;

        QTextCursor cursor(q->document());
        QSet<QString> crumbList;
        int last_pos = 0;

        cursor.setPosition(0);
        formatList.clear();

        while (!cursor.atEnd()) {
            cursor.setPosition(last_pos + 1);

            if (last_pos == cursor.position())
                break;

            last_pos = cursor.position();

            DCrumbTextFormat format(cursor.charFormat());

            if (format.objectType() == objectType) {
                const QString &text = format.text();

                if (!text.isEmpty()) {
                    crumbList << text;
                    formatList << text;

                    if (!formats.contains(text)) {
                        formats[text] = format;
                        formatsChanged = true;

                        Q_EMIT q->crumbAdded(text);
                    }
                }
            }
        }

        Q_FOREACH(const DCrumbTextFormat &f, formats) {
            if (!crumbList.contains(f.text())) {
                formats.remove(f.text());
                formatsChanged = true;

                Q_EMIT q->crumbRemoved(f.text());
            }
        }

        if (formatsChanged)
            Q_EMIT q->crumbListChanged();
    }

    CrumbObjectInterface *object;
    int objectType;
    bool crumbReadOnly = false;
    int crumbRadius = 2;
    QString splitter = ",";
    QStringList formatList;
    QMap<QString, DCrumbTextFormat> formats;

    bool dualClickMakeCrumb = false;
};

QSizeF CrumbObjectInterface::intrinsicSize(QTextDocument *doc, int posInDocument, const QTextFormat &format)
{
    Q_UNUSED(doc)
    Q_UNUSED(posInDocument)

    const DCrumbTextFormat crumb_format(format);
    const QFontMetricsF font_metrics(crumb_format.font());
    int radius = crumb_format.backgroundRadius();

    if (crumb_format.tagColor().isValid())
        return QSizeF(font_metrics.width(crumb_format.text()) + font_metrics.height() + radius + 2, font_metrics.height() + 2);

    return QSizeF(font_metrics.width(crumb_format.text()) + 2 * radius + 2, font_metrics.height() + 2);
}

void CrumbObjectInterface::drawObject(QPainter *painter, const QRectF &rect,
                                      QTextDocument *doc, int posInDocument, const QTextFormat &format)
{
    Q_UNUSED(doc)
    Q_UNUSED(posInDocument)

    const QRectF new_rect = rect.adjusted(1, 1, -1, -1);
    const DCrumbTextFormat crumb_format(format);
    const QFontMetricsF font_metrics(crumb_format.font());
    const int radius = crumb_format.backgroundRadius();

    QPainterPath background_path;
    QPainterPath tag_path;
    const QRectF tag_rect(new_rect.x() + 2, new_rect.y() + 2, font_metrics.height() - 4, font_metrics.height() - 4);

    tag_path.addEllipse(tag_rect);
    background_path.addRoundedRect(new_rect, radius, crumb_format.backgroundRadius());

    painter->setRenderHint(QPainter::Antialiasing);
    painter->fillPath(background_path, crumb_format.background());

    if (crumb_format.tagColor().isValid()) {
        painter->fillPath(tag_path, crumb_format.tagColor());

        painter->setPen(crumb_format.textColor());
        painter->drawText(new_rect.adjusted(tag_rect.width() + 2, 0, -radius, 0),
                          crumb_format.text(), Qt::AlignVCenter | Qt::AlignRight);
    } else {
        painter->setPen(crumb_format.textColor());
        painter->drawText(new_rect.adjusted(radius, 0, -radius, 0), crumb_format.text());
    }
}

DCrumbEdit::DCrumbEdit(QWidget *parent)
    : QTextEdit(parent)
    , DObject(*new DCrumbEditPrivate(this))
{
    d_func()->registerHandler(document()->documentLayout());

    connect(document(), SIGNAL(documentLayoutChanged()),
            this, SLOT(_q_onDocumentLayoutChanged()));
    connect(this, SIGNAL(cursorPositionChanged()),
            this, SLOT(_q_onCurrentPositionChanged()));
    connect(this, SIGNAL(textChanged()),
            this, SLOT(_q_onTextChanged()));
}

bool DCrumbEdit::insertCrumb(const DCrumbTextFormat &format, int pos)
{
    if (format.text().isEmpty())
        return false;

    D_DC(DCrumbEdit);

    if (!d->canAddCrumb(format.text()))
        return false;

    QTextCursor cursor = textCursor();

    if (pos >= 0)
        cursor.setPosition(pos);

    cursor.insertText(QString(QChar::ObjectReplacementCharacter), format);

    return true;
}

bool DCrumbEdit::appendCrumb(const DCrumbTextFormat &format)
{
    if (format.text().isEmpty())
        return false;

    D_DC(DCrumbEdit);

    if (!d->canAddCrumb(format.text()))
        return false;

    QTextCursor cursor = textCursor();

    cursor.movePosition(QTextCursor::End);
    cursor.insertText(QString(QChar::ObjectReplacementCharacter), format);

    return true;
}

bool DCrumbEdit::insertCrumb(const QString &text, int pos)
{
    DCrumbTextFormat format = makeTextFormat();

    format.setText(text);

    return insertCrumb(format, pos);
}

bool DCrumbEdit::appendCrumb(const QString &text)
{
    DCrumbTextFormat format = makeTextFormat();

    format.setText(text);

    return appendCrumb(format);
}

bool DCrumbEdit::containCrumb(const QString &text) const
{
    D_DC(DCrumbEdit);

    return d->formats.contains(text);
}

QStringList DCrumbEdit::crumbList() const
{
    D_DC(DCrumbEdit);

    return d->formatList;
}

DCrumbTextFormat DCrumbEdit::crumbTextFormat(const QString &text) const
{
    D_DC(DCrumbEdit);

    return d->formats.value(text);
}

DCrumbTextFormat DCrumbEdit::makeTextFormat() const
{
    D_DC(DCrumbEdit);

    DCrumbTextFormat format(d->objectType);

    format.setFontFamily(font().family());
    format.setBackgroundRadius(d->crumbRadius);

    return format;
}

DCrumbTextFormat DCrumbEdit::makeTextFormat(DCrumbEdit::CrumbType type) const
{
    D_DC(DCrumbEdit);

    QString text;
    DCrumbTextFormat format = makeTextFormat();

    switch (type) {
    case black:
        text = tr("Black");
        break;
    case white:
        text = tr("White");
        break;
    case darkGray:
        text = tr("Dark Gray");
        break;
    case gray:
        text = tr("Gray");
        break;
    case lightGray:
        text = tr("Light Gray");
        break;
    case red:
        text = tr("Red");
        break;
    case green:
        text = tr("Green");
        break;
    case blue:
        text = tr("Blue");
        break;
    case cyan:
        text = tr("Cyan");
        break;
    case magenta:
        text = tr("Magenta");
        break;
    case yellow:
        text = tr("Yellow");
        break;
    case darkRed:
        text = tr("Dark Red");
        break;
    case darkGreen:
        text = tr("Dark Green");
        break;
    case darkBlue:
        text = tr("Dark Blue");
        break;
    case darkCyan:
        text = tr("Dark Cyan");
        break;
    case darkMagenta:
        text = tr("Dark Magenta");
        break;
    case darkYellow:
        text = tr("Dark Yellow");
        break;
    default:
        return format;
    }

    format.setTagColor(QColor(static_cast<Qt::GlobalColor>(type)));
    format.setText(text);

    return format;
}

bool DCrumbEdit::dualClickMakeCrumb() const Q_DECL_NOEXCEPT
{
    D_DC(DCrumbEdit);

    return d->dualClickMakeCrumb;
}

bool DCrumbEdit::crumbReadOnly() const
{
    D_DC(DCrumbEdit);

    return d->crumbReadOnly;
}

int DCrumbEdit::crumbRadius() const
{
    D_DC(DCrumbEdit);

    return d->crumbRadius;
}

QString DCrumbEdit::splitter() const
{
    D_DC(DCrumbEdit);

    return d->splitter;
}

void DCrumbEdit::setCrumbReadOnly(bool crumbReadOnly)
{
    D_D(DCrumbEdit);

    d->crumbReadOnly = crumbReadOnly;
}

void DCrumbEdit::setCrumbRadius(int crumbRadius)
{
    D_D(DCrumbEdit);

    d->crumbRadius = crumbRadius;
}

void DCrumbEdit::setSplitter(const QString &splitter)
{
    D_D(DCrumbEdit);

    d->splitter = splitter;
}

void DCrumbEdit::setDualClickMakeCrumb(bool flag) Q_DECL_NOEXCEPT
{
    D_D(DCrumbEdit);

    d->dualClickMakeCrumb = flag;
}

void DCrumbEdit::paintEvent(QPaintEvent *event)
{
    QPainter p(viewport());

    QStyleOptionFrame panel;
    initStyleOption(&panel);
    panel.rect = viewport()->rect();
    style()->drawPrimitive(QStyle::PE_PanelLineEdit, &panel, &p, this);

    QTextEdit::paintEvent(event);
}

void DCrumbEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        D_D(DCrumbEdit);

        d->makeCrumb();
    } else {
        QTextEdit::keyPressEvent(event);
    }
}

void DCrumbEdit::mouseDoubleClickEvent(QMouseEvent *event)
{
    D_D(DCrumbEdit);

    if (d->dualClickMakeCrumb && d->makeCrumb())
        return;

    if (crumbReadOnly())
        return QTextEdit::mouseDoubleClickEvent(event);

    if (!d->editCrumb(event->pos()))
        return QTextEdit::mouseDoubleClickEvent(event);
}

void DCrumbEdit::focusOutEvent(QFocusEvent *event)
{
    D_D(DCrumbEdit);

    d->makeCrumb();

    QTextEdit::focusOutEvent(event);
}

QMimeData *DCrumbEdit::createMimeDataFromSelection() const
{
    D_DC(DCrumbEdit);

    QMimeData *mime = new QMimeData();
    const QTextCursor &cursor = textCursor();
    QStringList::const_iterator current_format = d->formatList.constBegin();

    const QString &plain_text = toPlainText();
    const QString &selected_text = cursor.selectedText();
    int pos = -1;
    QString text;
    QList<DCrumbTextFormat> format_list;

    for (const QChar &ch : plain_text) {
        ++pos;

        if (pos >= cursor.selectionEnd())
            break;

        if (ch == QChar::ObjectReplacementCharacter) {
            if (pos < cursor.selectionStart()) {
                ++current_format;
                continue;
            }

            const DCrumbTextFormat &f = d->formats.value(*current_format);

            ++current_format;

            if (f.text().isEmpty())
                continue;

            if (!text.isEmpty())
                text.append(" ").append(f.text());
            else
                text.append(f.text());

            format_list << f;
        } else if (pos < cursor.selectionStart()) {
            text.append(ch);
        }
    }

    mime->setText(text);
    mime->setData("deepin/dtkwidget-DCrumbTextFormat-data", selected_text.toUtf8());
    mime->setData("deepin/dtkwidget-DCrumbTextFormat-list", d->crumbMapToData(format_list));

    return mime;
}

bool DCrumbEdit::canInsertFromMimeData(const QMimeData *source) const
{
    if (source->hasFormat("deepin/dtkwidget-DCrumbTextFormat-data"))
        return true;

    return QTextEdit::canInsertFromMimeData(source);
}

void DCrumbEdit::insertFromMimeData(const QMimeData *source)
{
    if (!source->hasFormat("deepin/dtkwidget-DCrumbTextFormat-data"))
        QTextEdit::insertFromMimeData(source);

    const QString &plain_text = QString::fromUtf8(source->data("deepin/dtkwidget-DCrumbTextFormat-data"));

    if (plain_text.isEmpty())
        return;

    D_DC(DCrumbEdit);

    const QList<DCrumbTextFormat> &list = d->crumbListFromData(source->data("deepin/dtkwidget-DCrumbTextFormat-list"));
    QList<DCrumbTextFormat>::const_iterator current_format = list.constBegin();
    QString text;

    textCursor().beginEditBlock();

    for (const QChar &ch : plain_text) {
        if (ch == QChar::ObjectReplacementCharacter) {
            if (!text.isEmpty()) {
                textCursor().insertText(text);
                text.clear();
            }

            insertCrumb(*current_format);
            ++current_format;
        } else {
            text.append(ch);
        }
    }

    if (!text.isEmpty())
        textCursor().insertText(text);

    textCursor().endEditBlock();
}

DWIDGET_END_NAMESPACE

#include "moc_dcrumbedit.cpp"
#include "dcrumbedit.moc"
