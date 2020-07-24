/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
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
#include "dcrumbedit.h"
#include "dobject_p.h"
#include "DStyle"

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

/*!
    \~english \class DCrumbTextFormat
    \~english \brief Text format for Tags/Crumbs inside DCrumbEdit

    \~english DCrumbTextFormat comes with properties to indicate the look of a tag/crumb inside a DCrumbEdit.

    \~english The text property is the main property which indicate the tag itself. Other properties include
    tag color, background color and background radius are also available to use.

    \~english Using DCrumbEdit::makeTextFormat() to create a DCrumbTextFormat object is always recommended,
    if you are going to craft your own DCrumbTextFormat object, don't forget to call the
    DCrumbTextFormat::setObjectType function(normally value should be `QTextFormat::UserObject + 1`).

    \~english \sa DCrumbEdit
*/

/*!
    \~chinese \class DCrumbTextFormat
    \~chinese \brief 表示标签输入控件所含标签的文字样式

    \~chinese DCrumbTextFormat 包含了表示标签输入控件 DCrumbEdit 内所包含的标签的各种样式属性。

    \~chinese 文字属性为标识一个标签的唯一重要属性，其他可用的属性包含标签背景颜色，标签文字颜色以及标签圆角程度等可供使用。

    \~chinese 总是建议通过 DCrumbEdit::makeTextFormat() 来构造一个可供使用的 DCrumbTextFormat 对象，若希望自行构造，
    请留意使用 DCrumbTextFormat::setObjectType 来设置必要的对象类型 (通常应为 `QTextFormat::UserObject + 1`).

    \~chinese \sa DCrumbEdit
*/

DCrumbTextFormat::DCrumbTextFormat()
    : QTextCharFormat(QTextFormat(QTextFormat::InvalidFormat))
{

}

/*!
 * \~chinese \brief DCrumbTextFormat::tagColor
 * \~chinese \return 返回标签的颜色
 */
QColor DCrumbTextFormat::tagColor() const
{
    return colorProperty(QTextFormat::UserProperty);
}

/*!
 * \~chinese \brief DCrumbTextFormat::setTagColor 设置标签颜色
 * \~chinese \param color 标签的颜色
 */
void DCrumbTextFormat::setTagColor(const QColor &color)
{
    setProperty(QTextFormat::UserProperty, color);
}

/*!
 * \~chinese \brief DCrumbTextFormat::text
 * \~chinese \return 返回标签的文本内容
 */
QString DCrumbTextFormat::text() const
{
    return stringProperty(QTextFormat::UserProperty + 1);
}

/*!
 * \~chinese \brief DCrumbTextFormat::setText 设置文本内容（标签名称）
 * \~chinese \param text 文本内容
 */
void DCrumbTextFormat::setText(const QString &text)
{
    setProperty(QTextFormat::UserProperty + 1, text);
}

/*!
 * \~chinese \brief DCrumbTextFormat::textColor
 * \~chinese \return 返回文本的颜色
 */
QColor DCrumbTextFormat::textColor() const
{
    return colorProperty(QTextFormat::UserProperty + 2);
}

/*!
 * \~chinese \brief DCrumbTextFormat::setTextColor 设置文本颜色
 * \~chinese \param color 文本的颜色
 */
void DCrumbTextFormat::setTextColor(const QColor &color)
{
    setProperty(QTextFormat::UserProperty + 2, color);
}

/*!
 * \~chinese \brief DCrumbTextFormat::background
 * \~chinese \return 返回背景画刷
 */
QBrush DCrumbTextFormat::background() const
{
    return brushProperty(QTextFormat::UserProperty + 3);
}

/*!
 * \~chinese \brief DCrumbTextFormat::setBackground 设置背景
 * \~chinese \param background 背景画刷
 */
void DCrumbTextFormat::setBackground(const QBrush &background)
{
    setProperty(QTextFormat::UserProperty + 3, background);
}

/*!
 * \~chinese \brief DCrumbTextFormat::backgroundRadius
 * \~chinese \return 返回背景的圆角
 */
int DCrumbTextFormat::backgroundRadius() const
{
    return intProperty(QTextFormat::UserProperty + 4);
}

/*!
 * \~chinese \brief DCrumbTextFormat::setBackgroundRadius 设置背景圆角程度
 * \~chinese \param radius 圆角的大小
 */
void DCrumbTextFormat::setBackgroundRadius(int radius)
{
    setProperty(QTextFormat::UserProperty + 4, radius);
}

DCrumbTextFormat::DCrumbTextFormat(int objectType)
{
    setObjectType(objectType);

    setBackground(getGradientBrush(static_cast<Background>(qrand() % 12)));
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
        widgetTop = new QWidget(qq);
        widgetBottom = new QWidget(qq);
        widgetLeft = new QWidget(qq);
        widgetRight = new QWidget(qq);
        crumbRadius = DStyle::pixelMetric(qq->style(), DStyle::PM_FrameRadius);
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
        return QSizeF(font_metrics.width(crumb_format.text()) + font_metrics.height() + radius + 2, font_metrics.height() + 2);

    return QSizeF(font_metrics.width(crumb_format.text()) + 2 * radius + 2, font_metrics.height() + 2 + TopMargin *2);
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
    \~english \class DCrumbEdit
    \~english \brief Tag Edit Widget

    \~english \image html DCrumbEdit.png

    \~english DCrumbEdit provide a edit widget as a container for user entered Tags (or say, Crumbs), and allow user edit
    the existed tags inside this widget. Tags comes with different text, each of the tags inside the same DCrumbEdit should
    have different text.

    \~english The tag, or say crumb, use its text as an identifier, and it comes with other properties like tag color and etc.

    \~english DCrumbEdit allowed user add a tag by providing a simple QString or provide a custom DCrumbTextFormat object.
    Refer to appendCrumb() and insertCrumb() to see the usage. To get a list of tags/crumbs, see crumbList()

    \~english \sa DCrumbTextFormat
*/

/*!
    \~chinese \class DCrumbEdit
    \~chinese \brief 标签输入控件。

    \~chinese \image html DCrumbEdit.png

    \~chinese DCrumbEdit 提供了一个编辑框，编辑框内可包含若干个标签，并允许通过用户输入来编辑这些标签。编辑框所包含的标签可通过标签的文字
    内容确定唯一的标签，即编辑框所包含的标签内容（字符串值）均不重复。

    \~chinese 编辑框中包含的标签的主体为字符串，除文字外，每个标签可以对应其不同的颜色等样式，对于可供使用的样式请参见 DCrumbTextFormat 。

    \~chinese DCrumbEdit 允许通过给定字符串的形式便捷的向其中添加一个默认样式的标签，也可以通过自行构建标签文字样式。请参见 insertCrumb()
    和 appendCrumb() 的不同重载。可以通过 crumbList() 获得当前标签输入控件中包含的标签列表。

    \~chinese \sa DCrumbTextFormat
*/

DCrumbEdit::DCrumbEdit(QWidget *parent)
    : QTextEdit(parent)
    , DObject(*new DCrumbEditPrivate(this))
{
    Q_D(DCrumbEdit);

    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    viewport()->setAutoFillBackground(false);
    setFrameShape(QFrame::NoFrame);

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
 * \~english \brief Insert new tag/crumb to the crumb edit widget at the given position.
 *
 * \~english \param format a DCrumbTextFormat.
 * \~english \param pos position to be inserted.
 *
 * \~english Insert new tag/crumb with the provided DCrumbTextFormat at the given position. To create a ready-to-use
 * DCrumbTextFormat object, call makeTextFormat().
 *
 * \~english \return true if we appended a new tag into the crumb edit widget.
 *
 * \~english \sa makeTextFormat(), insertCrumb()
 */

/*!
 * \~chinese \brief 像标签编辑控件中插入新的标签
 *
 * \~chinese \param format 一个 DCrumbTextFormat 对象.
 * \~chinese \param pos 待插入的位置.
 *
 * \~chinese 通过给定的 DCrumbTextFormat 对象所描述的属性创建新的标签并追加到标签编辑控件中。若要创建一个直接可供使用的 DCrumbTextFormat
 * 对象，可以考虑使用 makeTextFormat() 。
 *
 * \~chinese \return 插入标签成功时返回 true，否则返回 false.
 *
 * \~chinese \sa makeTextFormat(), insertCrumb()
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
 * \~english \brief Append new tag/crumb to the crumb edit widget.
 *
 * \~english \param format a DCrumbTextFormat.
 *
 * \~english Append new tag/crumb with the provided DCrumbTextFormat. To create a ready-to-use DCrumbTextFormat object,
 * call makeTextFormat().
 *
 * \~english \return true if we appended a new tag into the crumb edit widget.
 *
 * \~english \sa makeTextFormat(), insertCrumb()
 */

/*!
 * \~chinese \brief 像标签编辑控件中追加新的标签
 *
 * \~chinese \param format 一个 DCrumbTextFormat 对象.
 *
 * \~chinese 通过给定的 DCrumbTextFormat 对象所描述的属性创建新的标签并追加到标签编辑控件中。若要创建一个直接可供使用的 DCrumbTextFormat
 * 对象，可以考虑使用 makeTextFormat() 。
 *
 * \~chinese \return 插入标签成功时返回 true，否则返回 false.
 *
 * \~chinese \sa makeTextFormat(), insertCrumb()
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
 * \~english \brief Insert new tag/crumb to the crumb edit widget.
 *
 * \~english \param text of the tag/crumb.
 * \~english \param pos position to be inserted.
 *
 * \~english Insert new tag/crumb with the provided text to the given position.
 *
 * \~english \return true if we appended a new tag into the crumb edit widget.
 *
 * \~english \sa appendCrumb()
 */

/*!
 * \~chinese \brief 像标签编辑控件中插入新的标签
 *
 * \~chinese \param text 标签的文字内容.
 * \~chinese \param pos 待插入的位置.
 *
 * \~chinese 通过给定的文字创建新的标签并插入到标签编辑控件中的指定位置。
 *
 * \~chinese \return 插入标签成功时返回 true，否则返回 false.
 *
 * \~chinese \sa appendCrumb()
 */
bool DCrumbEdit::insertCrumb(const QString &text, int pos)
{
    DCrumbTextFormat format = makeTextFormat();

    format.setText(text);

    return insertCrumb(format, pos);
}

/*!
 * \~english \brief Append new tag/crumb to the crumb edit widget.
 *
 * \~english \param text of the tag/crumb.
 *
 * \~english Append new tag/crumb with the provided text.
 *
 * \~english \return true if we appended a new tag into the crumb edit widget.
 *
 * \~english \sa insertCrumb()
 */

/*!
 * \~chinese \brief 像标签编辑控件中追加新的标签
 *
 * \~chinese \param text 标签的文字内容.
 *
 * \~chinese 通过给定的文字创建新的标签并追加到标签编辑控件中。
 *
 * \~chinese \return 插入标签成功时返回 true，否则返回 false.
 *
 * \~chinese \sa insertCrumb()
 */
bool DCrumbEdit::appendCrumb(const QString &text)
{
    DCrumbTextFormat format = makeTextFormat();

    format.setText(text);

    return appendCrumb(format);
}

/*!
 * \~chinese \brief 检查当前控件是否已经包含了 \a text 标签
 * \~chinese \param text 待检查的标签文字名称
 * \~chinese \return 如果包含，返回 true
 */
bool DCrumbEdit::containCrumb(const QString &text) const
{
    D_DC(DCrumbEdit);

    return d->formats.contains(text);
}

/*!
 * \~english \brief Get a list of crumb/tags inside the crumb edit widget.
 */

/*!
 * \~chinese \brief 取得标签输入控件内已包含的标签列表
 */
QStringList DCrumbEdit::crumbList() const
{
    D_DC(DCrumbEdit);

    return d->formatList;
}

/*!
 * \~english \brief Get the DCrumbTextFormat matched the given tag \a text name.
 */

/*!
 * \~chinese \brief 根据标签名称取得对应的标签样式属性 DCrumbTextFormat 对象
 */
DCrumbTextFormat DCrumbEdit::crumbTextFormat(const QString &text) const
{
    D_DC(DCrumbEdit);

    return d->formats.value(text);
}

/*!
 * \~english \brief create a ready-to-use DCrumbTextFormat object.
 * \~english \return the created DCrumbTextFormat object
 */

/*!
 * \~chinese \brief 创建一个直接可用的 DCrumbTextFormat 对象.
 * \~chinese \return 创建的 DCrumbTextFormat 对象
 */
DCrumbTextFormat DCrumbEdit::makeTextFormat() const
{
    D_DC(DCrumbEdit);

    DCrumbTextFormat format(d->objectType);

    format.setFontFamily(font().family());
    format.setBackgroundRadius(d->crumbRadius);

    return format;
}

/*!
 * \~english \brief create a ready-to-use DCrumbTextFormat object with the given color.
 *
 * \~english Create a ready-to-use DCrumbTextFormat object with the given color, and set its text
 * as the color name.
 *
 * \~english \return the created DCrumbTextFormat object
 */

/*!
 * \~chinese \brief 根据所给的标签类型（颜色）创建一个直接可用的 DCrumbTextFormat 对象.
 *
 * 将根据所给的（颜色）类型创建一个新的 DCrumbTextFormat 对象，并将其内文字属性设为颜色的名称。
 *
 * \~chinese \return 创建的 DCrumbTextFormat 对象
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
 * \~chinese \brief 设置或取消设置标签编辑控件的只读状态
 */
void DCrumbEdit::setCrumbReadOnly(bool crumbReadOnly)
{
    D_D(DCrumbEdit);

    d->crumbReadOnly = crumbReadOnly;
}

/*!
 * \~chinese \brief 设置控件圆角角度
 */
void DCrumbEdit::setCrumbRadius(int crumbRadius)
{
    D_D(DCrumbEdit);

    d->crumbRadius = crumbRadius;
}

/*!
 * \~chinese \brief 设置分割符
 *
 * \~chinese 设置表示划分标签边界的分割字符，可用于一次性输入多个以给定分隔符分割的标签
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
    }

    return QTextEdit::event(e);
}

/*!\reimp */
void DCrumbEdit::paintEvent(QPaintEvent *event)
{
    QPainter p(viewport());

    QStyleOptionFrame panel;
    initStyleOption(&panel);
    panel.rect = viewport()->rect();
    style()->drawPrimitive(QStyle::PE_PanelLineEdit, &panel, &p, this);

    QTextEdit::paintEvent(event);
}

/*!\reimp */
void DCrumbEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        D_D(DCrumbEdit);

        bool result = d->makeCrumb();
        if (!result)
            event->ignore();

    } else if (event->key() == Qt::Key_Escape) {
        event->ignore();
    } else {
        QTextEdit::keyPressEvent(event);
    }
}

/*!\reimp */
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

/*!\reimp */
void DCrumbEdit::focusOutEvent(QFocusEvent *event)
{
    D_D(DCrumbEdit);

    d->makeCrumb();

    QTextEdit::focusOutEvent(event);
}

/*!\reimp */
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

/*!\reimp */
bool DCrumbEdit::canInsertFromMimeData(const QMimeData *source) const
{
    if (source->hasFormat("deepin/dtkwidget-DCrumbTextFormat-data"))
        return true;

    return QTextEdit::canInsertFromMimeData(source);
}

/*!\reimp */
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
