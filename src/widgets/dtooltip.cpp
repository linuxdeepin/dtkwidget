// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dtooltip.h"
#include "dstyle.h"

#include <DPlatformWindowHandle>

#include <QDebug>
#include <QEvent>
#include <QTimer>
#include <QToolTip>
#include <QTextLayout>

DWIDGET_BEGIN_NAMESPACE
namespace DToolTipStatic {
static inline void registerDToolTipMetaType()
{
    qRegisterMetaType<DToolTip::ToolTipShowMode>();
}

Q_CONSTRUCTOR_FUNCTION(registerDToolTipMetaType);

static Qt::TextFormat textFormat = Qt::TextFormat::AutoText;
}

/*!
@~english
  @brief set the Tooltip text format
  Set the Tooltip text format inside DStyle to the specified style by setting format.
  @param[in] format text format
  @sa Qt::TextFormat
 */
void DToolTip::setToolTipTextFormat(Qt::TextFormat format)
{
    DToolTipStatic::textFormat = format;
}
/*!
@~english
  @brief get the Tooltip text format
  @return Tooltip text format used inside DStyle
  @sa Qt::TextFormat
 */
Qt::TextFormat DToolTip::toolTipTextFormat()
{
    return DToolTipStatic::textFormat;
}

/*!
@~english
 * @brief DToolTip::setToolTipShowMode
 * @param[in] widget widget to set ToolTip show mode
 * @param[in] mode ToolTip show mode
 */
/*!
@~english
  @enum Dtk::Widget::DToolTip::ToolTipShowMode
  @brief The ToolTipShowMode enum indicate the ToolTip show mode.
  
  \value  NotShow
  Do not display the ToolTip.
  
  \value  AlwaysShow
  Always display the ToolTip.

  \value  ShowWhenElided
  Display the ToolTip when the text is elided.

  \value  Default
  Use the default ToolTip show mode.
 */
void DToolTip::setToolTipShowMode(QWidget *widget, ToolTipShowMode mode)
{
    widget->setProperty("_d_dtk_toolTipMode", mode);
}

/*!
@~english
 * @brief DToolTip::toolTipShowMode
 * @param[in] widget widget to get ToolTip show mode
 * @return ToolTip show mode
 */
DToolTip::ToolTipShowMode DToolTip::toolTipShowMode(const QWidget *widget)
{
    QVariant vToolTipMode = widget->property("_d_dtk_toolTipMode");
    if (vToolTipMode.isValid()) {
        return qvariant_cast<ToolTipShowMode>(vToolTipMode);
    } else {
        return ToolTipShowMode::Default;
    }
}

QString DToolTip::wrapToolTipText(QString text, QTextOption option)
{
    if (text.isEmpty()) {
        return "";
    }
    const auto MaxPixelsPerRow = DStyle::pixelMetric(nullptr, DStyle::PixelMetric::PM_ToolTipLabelWidth);
    QStringList paragraphs = text.split('\n');
    const QFont &toolTipFont = QToolTip::font();
    QString toolTip{""};
    for (const QString &paragraph : std::as_const(paragraphs))
    {
        if (paragraph.isEmpty())
            continue;
        QTextLayout toolTipLayout(paragraph, toolTipFont);
        toolTipLayout.setTextOption(option);
        qreal height = 0;
        toolTipLayout.beginLayout();
        QTextLine line = toolTipLayout.createLine();
        while(line.isValid()) {
            line.setLineWidth(MaxPixelsPerRow);
            line.setPosition({0, height});
            height += line.height();
            line = toolTipLayout.createLine();
        }
        toolTipLayout.endLayout();
        for (int i = 0; i < toolTipLayout.lineCount(); ++i) {
            const auto &currentLine = toolTipLayout.lineAt(i);
            const auto &toolTipText = toolTipLayout.text();
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            toolTip.append(toolTipText.midRef(currentLine.textStart(), currentLine.textLength()));
#else
            toolTip.append(QStringView{toolTipText}.mid(currentLine.textStart(), currentLine.textLength()));
#endif
            toolTip.append('\n');
        }
    }
    toolTip.chop(1);
    return toolTip;
}

bool DToolTip::needUpdateToolTip(const QWidget *widget, bool showToolTip)
{
    QVariant vShowToolTip = widget->property("_d_dtk_showToolTip");
    bool needUpdate = false;
    if (vShowToolTip.isValid()) {
        bool oldShowStatus = vShowToolTip.toBool();
        if (showToolTip != oldShowStatus) {
            needUpdate = true;
        }
    } else {
        needUpdate = true;
    }
    return needUpdate;
}

void DToolTip::setShowToolTip(QWidget *widget, bool showToolTip)
{
    widget->setProperty("_d_dtk_showToolTip", showToolTip);
}

/*!
@~english
  @class Dtk::Widget::DToolTip
  @inmodule dtkwidget
  @brief like QToolTip
  @details DToolTip provides a function that displays DTooltip in the specified position.
 */

/*!
@~english
  @brief DToolTip constructor
  @param[in] text text message
  @param[in] completionClose Prompt whether the tip is destroyed after disappearing.
 */
DToolTip::DToolTip(const QString &text, bool completionClose)
    : DTipLabel(text)
{
    if (completionClose) {
        setAttribute(Qt::WA_DeleteOnClose);
    }
}

/*!
@~english
  @brief DToolTip::sizeHint

  @sa QLabel::sizeHint()
 */
QSize DToolTip::sizeHint() const
{
    int radius = DStyleHelper(style()).pixelMetric(DStyle::PM_FrameRadius);
    QSize fontSize = fontMetrics().size({}, text());

    fontSize.setWidth(fontSize.width() + radius);

    return fontSize;
}

/*!
@~english
  @brief DToolTip::show set the location where the tip appears

  @param[in] pos location of tip
  @param[in] duration tip existence time, unit in millisecond
 */
void DToolTip::show(const QPoint &pos, int duration)
{
    QTimer::singleShot(duration, this, &DTipLabel::close);

    DTipLabel::show(pos);
}

DWIDGET_END_NAMESPACE
