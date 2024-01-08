// SPDX-FileCopyrightText: 2017 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dcrumbedit.h"
#include "dobject_p.h"
#include "DStyle"
#include "dsizemode.h"
#include <QRandomGenerator>
#include <QAbstractTextDocumentLayout>
#include <QPainter>
#include <QPainterPath>
#include <QTextBlock>
#include <QStyleOptionFrame>
#include <QMouseEvent>
#include <QMimeData>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTime>

#include <private/qtextdocument_p.h>

DWIDGET_BEGIN_NAMESPACE

enum Background {
    red,
    grey,
    black,
    brown,
    yellow,
    darkBlue,
    lightBlue,
    turquoise,
    lightGreen,
    oliveGreen,
    darkMagenta,
    lightMagenta
};

QBrush static getGradientBrush(Background background)
{
    QStringList gradient;
    switch (background) {
    case Background::lightGreen:
        gradient = QStringList{"#81c704","#429321"};
        break;
    case Background::lightBlue:
        gradient = QStringList{"#0090f0", "#0456b4"};
        break;
    case Background::grey:
        gradient = QStringList{"#7a7a7a", "#585858"};
        break;
    case Background::brown:
        gradient = QStringList{"#b06e1f", "#875110"};
        break;
    case Background::yellow:
        gradient = QStringList{"#fabd2b", "#d08c05"};
        break;
    case Background::turquoise:
        gradient = QStringList{"#00f06b", "#429aff"};
        break;
    case Background::darkBlue:
        gradient = QStringList{"#0055f3", "#0405b4"};
        break;
    case Background::darkMagenta:
        gradient = QStringList{"#8c00f0", "#5d04b4"};
        break;
    case Background::lightMagenta:
        gradient = QStringList{"#ec2cff", "#ad007b"};
        break;
    case Background::red:
        gradient = QStringList{"#ff7130", "#db071d"};
        break;
    case Background::oliveGreen:
        gradient = QStringList{"#91a800", "#426300"};
        break;
    case Background::black:
        gradient = QStringList{"#4e4e4e", "#000000"};
        break;
    }

    QLinearGradient lg;
    lg.setColorAt(0, QColor(gradient[0]));
    lg.setColorAt(1, QColor(gradient[1]));
    return QBrush(lg);
}

class CrumbObjectInterface : public QObject, public QTextObjectInterface
{
    Q_OBJECT
    Q_INTERFACES(QTextObjectInterface)

public:
    enum TextMargin
    {
        TopMargin = 2,
        LeftMargin = 4,
    };

    explicit CrumbObjectInterface(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    QSizeF intrinsicSize(QTextDocument *doc, int posInDocument,
                         const QTextFormat &format) Q_DECL_OVERRIDE;

    void drawObject(QPainter *painter, const QRectF &rect, QTextDocument *doc,
                    int posInDocument, const QTextFormat &format) Q_DECL_OVERRIDE;

    QBrush backgroundBrush(const QRect &rect, const QBrush &brush);
};

class DCrumbEditPanelFrame : public QWidget {
    Q_OBJECT

public:
    explicit DCrumbEditPanelFrame(QWidget *parent = nullptr)
        :QWidget(parent)
    {
        setFocusProxy(parent);
    }

protected:
    void paintEvent(QPaintEvent *event) override;
};

void DCrumbEditPanelFrame::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    QStyleOptionFrame panel;
    panel.initFrom(parentWidget());
    panel.rect = rect();
    style()->drawPrimitive(QStyle::PE_PanelLineEdit, &panel, &p, parentWidget());
    QWidget::paintEvent(event);
}

/*!
@~english
    @class Dtk::Widget::DCrumbTextFormat
    \inmodule dtkwidget

    @brief Text format for Tags/Crumbs inside DCrumbEdit.

    DCrumbTextFormat comes with properties to indicate the look of a tag/crumb inside a DCrumbEdit.

    The text property is the main property which indicate the tag itself. Other properties include
    tag color, background color and background radius are also available to use.

    Using DCrumbEdit::makeTextFormat() to create a DCrumbTextFormat object is always recommended,
    if you are going to craft your own DCrumbTextFormat object, don't forget to call the
    DCrumbTextFormat::setObjectType function(normally value should be `QTextFormat::UserObject + 1`).

    @sa DCrumbEdit
*/

DCrumbTextFormat::DCrumbTextFormat()
    : QTextCharFormat(QTextFormat(QTextFormat::InvalidFormat))
{

}

/*!
@~english
  @brief DCrumbTextFormat::tagColor return the color of the tag/crumb
  @return the color of the tag/crumb
 */
QColor DCrumbTextFormat::tagColor() const
{
    return colorProperty(QTextFormat::UserProperty);
}

/*!
@~english
  @brief DCrumbTextFormat::setTagColor set the color of the tag/crumb
  @param[in] color color of the tag/crumb
 */
void DCrumbTextFormat::setTagColor(const QColor &color)
{
    setProperty(QTextFormat::UserProperty, color);
}

/*!
@~english
  @brief DCrumbTextFormat::text return the text of the tag/crumb
  @return the text content of the tag/crumb
 */
QString DCrumbTextFormat::text() const
{
    return stringProperty(QTextFormat::UserProperty + 1);
}

/*!
@~english
  @brief DCrumbTextFormat::setText set the text content(name of the tag/crumb)
  @param[in] text the text content
 */
void DCrumbTextFormat::setText(const QString &text)
{
    setProperty(QTextFormat::UserProperty + 1, text);
}

/*!
@~english
  @brief DCrumbTextFormat::textColor return the color of the text
  @return color of the text
 */
QColor DCrumbTextFormat::textColor() const
{
    return colorProperty(QTextFormat::UserProperty + 2);
}

/*!
@~english
  @brief DCrumbTextFormat::setTextColor set the color of the text
  @param[in] color color of the text
 */
void DCrumbTextFormat::setTextColor(const QColor &color)
{
    setProperty(QTextFormat::UserProperty + 2, color);
}

/*!
@~english
  @brief DCrumbTextFormat::background return the background brush
  @return the background brush
 */
QBrush DCrumbTextFormat::background() const
{
    return brushProperty(QTextFormat::UserProperty + 3);
}

/*!
@~english
  @brief DCrumbTextFormat::setBackground set the backfround brush
  @param[in] background the backfround brush
 */
void DCrumbTextFormat::setBackground(const QBrush &background)
{
    setProperty(QTextFormat::UserProperty + 3, background);
}

/*!
@~english
  @brief DCrumbTextFormat::backgroundRadius return the background corner radius
  @return the background corner radius
 */
int DCrumbTextFormat::backgroundRadius() const
{
    return intProperty(QTextFormat::UserProperty + 4);
}

/*!
@~english
  @brief DCrumbTextFormat::setBackgroundRadius set the background corner radius
  @param[in] radius the background corner radius
 */
void DCrumbTextFormat::setBackgroundRadius(int radius)
{
    setProperty(QTextFormat::UserProperty + 4, radius);
}

DCrumbTextFormat::DCrumbTextFormat(int objectType)
{
    setObjectType(objectType);

    setBackground(getGradientBrush(static_cast<Background>(QRandomGenerator::global()->generate() % 12)));
    setTextColor(Qt::white);
#if QT_VERSION < QT_VERSION_CHECK(5, 12, 0)
    setVerticalAlignment(QTextCharFormat::AlignBaseline);
#else
    setVerticalAlignment(QTextCharFormat::AlignBottom);
#endif
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
        widgetTop = new QWidget(qq);
        widgetBottom = new QWidget(qq);
        widgetLeft = new QWidget(qq);
        widgetRight = new QWidget(qq);
        crumbRadius = DStyle::pixelMetric(qq->style(), DStyle::PM_FrameRadius);
        panelFrame = new DCrumbEditPanelFrame(qq);
        panelFrame->stackUnder(qq->viewport());

        widgetTop->setAccessibleName("DCrumbEditTopWidget");
        widgetBottom->setAccessibleName("DCrumbEditBottomWidget");
        widgetLeft->setAccessibleName("DCrumbEditLeftWidget");
        widgetRight->setAccessibleName("DCrumbEditRightWidget");
        panelFrame->setAccessibleName("DCrumbEditPanelFrame");
    }

    void registerHandler(QAbstractTextDocumentLayout *layout)
    {
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
        if (currentText == tag_text)
            format.setBackground(currentBrush);
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

        auto fmt = q->document()->documentLayout()->formatAt(mousePos);
        DCrumbTextFormat format = formats.value(fmt.stringProperty(QTextFormat::UserProperty + 1));

        currentText = format.text();
        currentBrush = format.background();

        if (format.text().isEmpty())
            return false;

        if (cursor.atEnd() && mousePos.x() > q->cursorRect().right())
            return false;

        makeCrumb();

        if (mousePos.x() < q->cursorRect().left() ||
            mousePos.y() < q->cursorRect().top())
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
    bool explicitCrumbRadius = false;
    QString splitter = ",";
    QStringList formatList;
    QMap<QString, DCrumbTextFormat> formats;

    bool dualClickMakeCrumb = false;
    QString currentText;
    QBrush currentBrush;
    DCrumbEditPanelFrame* panelFrame = nullptr;

public:
    QWidget* widgetTop;
    QWidget* widgetBottom;
    QWidget* widgetLeft;
    QWidget* widgetRight;
};

QSizeF CrumbObjectInterface::intrinsicSize(QTextDocument *doc, int posInDocument, const QTextFormat &format)
{
    Q_UNUSED(doc)
    Q_UNUSED(posInDocument)

    const DCrumbTextFormat crumb_format(format);
    const QFontMetricsF font_metrics(crumb_format.font());
    int radius = crumb_format.backgroundRadius();

    if (crumb_format.tagColor().isValid())
        return QSizeF(font_metrics.horizontalAdvance(crumb_format.text()) + font_metrics.height() + radius + 2, font_metrics.height() + 2);

    return QSizeF(font_metrics.horizontalAdvance(crumb_format.text()) + 2 * radius + 2, font_metrics.height() + 2 + TopMargin *2);
}

void CrumbObjectInterface::drawObject(QPainter *painter, const QRectF &rect,
                                      QTextDocument *doc, int posInDocument, const QTextFormat &format)
{
    Q_UNUSED(doc)
    Q_UNUSED(posInDocument)

    const QRect new_rect = rect.adjusted(LeftMargin, TopMargin, 0, -TopMargin).toRect();
    const DCrumbTextFormat crumb_format(format);
    const QFontMetricsF font_metrics(crumb_format.font());
    const int radius = crumb_format.backgroundRadius();

    QPainterPath background_path;
    QPainterPath tag_path;
    const QRectF tag_rect(new_rect.x() + 2, new_rect.y() + 2, font_metrics.height() - 4, font_metrics.height() - 4);

    tag_path.addEllipse(tag_rect);
    background_path.addRoundedRect(new_rect, radius, crumb_format.backgroundRadius());

    painter->setRenderHint(QPainter::Antialiasing);
    painter->fillPath(background_path, backgroundBrush(new_rect, crumb_format.background()));

    if (crumb_format.tagColor().isValid()) {
        painter->fillPath(tag_path, crumb_format.tagColor());

        painter->setPen(crumb_format.textColor());
        painter->drawText(new_rect.adjusted(tag_rect.width() + 2, 0, -radius, 0),
                          crumb_format.text(), Qt::AlignVCenter | Qt::AlignRight);
    } else {
        painter->setPen(crumb_format.textColor());
        painter->drawText(new_rect, Qt::AlignCenter, crumb_format.text());
    }
}

QBrush CrumbObjectInterface::backgroundBrush(const QRect &rect, const QBrush &brush)
{
    QLinearGradient *lg = static_cast<QLinearGradient *>(const_cast<QGradient*>(brush.gradient()));
    if (lg) {
        lg->setStart(QPointF(rect.x(), rect.y()));
        lg->setFinalStop(QPointF(rect.x(), rect.y() + rect.height()));
    }

    return brush;
}

/*!
@~english
    @class Dtk::Widget::DCrumbEdit
    \inmodule dtkwidget

    @brief Tag Edit Widget.

    DCrumbEdit provide a edit widget as a container for user entered Tags (or say, Crumbs), and allow user edit
    the existed tags inside this widget. Tags comes with different text, each of the tags inside the same DCrumbEdit should
    have different text.

    The tag, or say crumb, use its text as an identifier, and it comes with other properties like tag color and etc.

    DCrumbEdit allowed user add a tag by providing a simple QString or provide a custom DCrumbTextFormat object.
    Refer to appendCrumb() and insertCrumb() to see the usage. To get a list of tags/crumbs, see crumbList()

    @image html DCrumbEdit.png
    @sa DCrumbTextFormat
*/

DCrumbEdit::DCrumbEdit(QWidget *parent)
    : QTextEdit(parent)
    , DObject(*new DCrumbEditPrivate(this))
{
    Q_D(DCrumbEdit);

    int frameRadius = DStyle::pixelMetric(style(), DStyle::PM_FrameRadius);
    int margins = DStyle::pixelMetric(style(), DStyle::PM_FrameMargins);
    viewport()->setAutoFillBackground(false);
    viewport()->setAccessibleName("DCrumbViewport");
    setFrameShape(QFrame::NoFrame);
    int margin = frameRadius / 2 + margins + 2;
    setViewportMargins(margin, margin, margin, margin);

    d->widgetTop->setFixedWidth(1);
    d->widgetBottom->setFixedWidth(1);
    d->widgetLeft->setFixedHeight(1);
    d->widgetRight->setFixedHeight(1);

    addScrollBarWidget(d->widgetTop, Qt::AlignTop);
    addScrollBarWidget(d->widgetBottom, Qt::AlignBottom);
    addScrollBarWidget(d->widgetLeft, Qt::AlignLeft);
    addScrollBarWidget(d->widgetRight, Qt::AlignRight);

    d_func()->registerHandler(document()->documentLayout());

    connect(document(), SIGNAL(documentLayoutChanged()),
            this, SLOT(_q_onDocumentLayoutChanged()));
    connect(this, SIGNAL(cursorPositionChanged()),
            this, SLOT(_q_onCurrentPositionChanged()));
    connect(this, SIGNAL(textChanged()),
            this, SLOT(_q_onTextChanged()));
}

/*!
@~english
  @brief Insert new tag/crumb to the crumb edit widget at the given position.
  
  @param[in] format a DCrumbTextFormat.
  @param[in] pos position to be inserted.

  Insert new tag/crumb with the provided DCrumbTextFormat at the given position. To create a ready-to-use
  DCrumbTextFormat object, call makeTextFormat().
  
  @return true if we appended a new tag into the crumb edit widget.
  
  @sa makeTextFormat()
 */
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

/*!
@~english
  @brief Append new tag/crumb to the crumb edit widget.
  
  @param[in] format a DCrumbTextFormat.
  
  Append new tag/crumb with the provided DCrumbTextFormat. To create a ready-to-use DCrumbTextFormat object,
  call makeTextFormat().

  @return true if we appended a new tag into the crumb edit widget.
  
  @sa makeTextFormat()
 */
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

/*!
@~english
  @brief Insert new tag/crumb to the crumb edit widget.
  
  @param[in] text of the tag/crumb.
  @param[in] pos position to be inserted.
  
  Insert new tag/crumb with the provided text to the given position.
  
  @return true if we appended a new tag into the crumb edit widget.
 */
bool DCrumbEdit::insertCrumb(const QString &text, int pos)
{
    DCrumbTextFormat format = makeTextFormat();

    format.setText(text);

    return insertCrumb(format, pos);
}

/*!
@~english
  @brief Append new tag/crumb to the crumb edit widget.
  
  @param[in] text of the tag/crumb.
  
  Append new tag/crumb with the provided text.
  
  @return true if we appended a new tag into the crumb edit widget.
 */
bool DCrumbEdit::appendCrumb(const QString &text)
{
    DCrumbTextFormat format = makeTextFormat();

    format.setText(text);

    return appendCrumb(format);
}

/*!
@~english
  @brief Check whether this widget already has the tag/crumb setting param "text" as text content
  @param[in] text the text(also the name) of the tag/crumb
  @return True if the widget does have such a tag/crumb and False if not
 */
bool DCrumbEdit::containCrumb(const QString &text) const
{
    D_DC(DCrumbEdit);

    return d->formats.contains(text);
}

/*!
@~english
  @brief Get a list of crumb/tags inside the crumb edit widget.
 */
QStringList DCrumbEdit::crumbList() const
{
    D_DC(DCrumbEdit);

    return d->formatList;
}

/*!
@~english
  @brief Get the DCrumbTextFormat matched the given tag @param[in] text name.
 */
DCrumbTextFormat DCrumbEdit::crumbTextFormat(const QString &text) const
{
    D_DC(DCrumbEdit);

    return d->formats.value(text);
}

/*!
@~english
  @brief create a ready-to-use DCrumbTextFormat object.

  @return the created DCrumbTextFormat object
 */
DCrumbTextFormat DCrumbEdit::makeTextFormat() const
{
    D_DC(DCrumbEdit);

    DCrumbTextFormat format(d->objectType);

    format.setFontFamilies(font().families());
    format.setBackgroundRadius(d->crumbRadius);

    return format;
}

/*!
@~english
  @brief create a ready-to-use DCrumbTextFormat object with the given color.
  
  Create a ready-to-use DCrumbTextFormat object with the given color, and set its text
  as the color name.

  @return the created DCrumbTextFormat object
 */
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

/*!
@~english
  @brief set or cancel setting the tag\crumb to be readonly

  @param[in] crumbReadOnly True means readonly, while False means not.
 */
void DCrumbEdit::setCrumbReadOnly(bool crumbReadOnly)
{
    D_D(DCrumbEdit);

    d->crumbReadOnly = crumbReadOnly;
}

/*!
@~english
  @brief set the corner radius of the tag/crumb

  @param[in] crumbRadius the corner radius of the tag/crumb.
 */
void DCrumbEdit::setCrumbRadius(int crumbRadius)
{
    D_D(DCrumbEdit);

    d->crumbRadius = crumbRadius;
    d->explicitCrumbRadius = true;
}

/*!
@~english
  @brief set the splitter
  
  set the splitter representing the boundary of the tag/crumb.
  It can be used to input multiple tags/crumbs separated by a given splitter at one time.

  @param[in] splitter the splitter character.
 */
void DCrumbEdit::setSplitter(const QString &splitter)
{
    D_D(DCrumbEdit);

    d->splitter = splitter;
}

// is that a typo?
void DCrumbEdit::setDualClickMakeCrumb(bool flag) Q_DECL_NOEXCEPT
{
    D_D(DCrumbEdit);

    d->dualClickMakeCrumb = flag;
}

bool DCrumbEdit::event(QEvent *e)
{
    D_D(DCrumbEdit);

    if (e->type() == QEvent::Polish) {
        DStyleHelper dstyle(style());
        int frame_radius = dstyle.pixelMetric(DStyle::PM_FrameRadius, nullptr, this);
        d->widgetTop->setFixedHeight(frame_radius);
        d->widgetBottom->setFixedHeight(frame_radius);
        d->widgetLeft->setFixedWidth(frame_radius);
        d->widgetRight->setFixedWidth(frame_radius);
    } else if (e->type() == QEvent::Resize) {
        d->panelFrame->resize(size());
    } else if (e->type() == QEvent::StyleChange)  {
        int frameRadius = DStyle::pixelMetric(style(), DStyle::PM_FrameRadius);
        // update crumbRadius if not set.
        if (!d->explicitCrumbRadius) {
        d->crumbRadius = frameRadius;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        auto collection = document()->docHandle()->formatCollection();
#else
        auto collection = QTextDocumentPrivate::get(document())->formatCollection();
#endif
            for (int i = 0; i < collection->numFormats(); ++i) {
                // only update format in d->formats.
                if (!collection->format(i).hasProperty(QTextFormat::UserProperty + 1))
                    continue;
                const auto key = collection->format(i).property(QTextFormat::UserProperty + 1).toString();
                if (d->formats.contains(key))
                    collection->formats[i].setProperty(QTextFormat::UserProperty + 4, crumbRadius());
            }
            for (auto iter = d->formats.begin(); iter != d->formats.end(); iter++) {
                iter->setBackgroundRadius(crumbRadius());
            }
        }

        int margins = DStyle::pixelMetric(style(), DStyle::PM_FrameMargins);
        int margin = frameRadius / 2 + margins + 2;
        setViewportMargins(margin, margin, margin, margin);
    }

    return QTextEdit::event(e);
}

void DCrumbEdit::paintEvent(QPaintEvent *event)
{
    QTextEdit::paintEvent(event);
}

void DCrumbEdit::keyPressEvent(QKeyEvent *event)
{
    D_D(DCrumbEdit);

    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        bool result = d->makeCrumb();
        if (!result)
            event->ignore();

    } else if (event->key() == Qt::Key_Escape) {
        event->ignore();
    } else if (event == QKeySequence::SelectAll) {
        // When the text is being edited and there is already color text, the copied text will encounter an exception: use "Select All" will convert the edited text to color text
        d->makeCrumb();
        QTextEdit::keyPressEvent(event);
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

    for (QString::const_iterator ch_it = plain_text.begin(); ch_it != plain_text.end(); ++ch_it) {
        ++pos;

        if (pos >= cursor.selectionEnd())
            break;

        if (*ch_it == QChar::ObjectReplacementCharacter) {
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
        } else {
            // When there is editable text, if the mouse is selected, editable text needs to be added
            if (pos < cursor.selectionStart())
                continue;

            if (!text.isEmpty() && ((ch_it - 1) >= plain_text.begin()) && (*(ch_it - 1) == QChar::ObjectReplacementCharacter)) {
                // The previous character is not the first character and is an identifier.
                text.append(" ").append(*ch_it);
            } else {
                text.append(*ch_it);
            }
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
