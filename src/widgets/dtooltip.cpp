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
  \brief 设置 tooltip 的文本格式.

  通过给定 \a format , 将 DStyle 内部中的 ToolTip
  文本格式设置为 \a format 指定的样式。

  \sa Qt::TextFormat
 */
void DToolTip::setToolTipTextFormat(Qt::TextFormat format)
{
    DToolTipStatic::textFormat = format;
}
/*!
  \brief 获取 tooltip 文本格式.

  \return 返回 DStyle 内部使用的 ToolTip 文本格式。

  \sa Qt::TextFormat
 */
Qt::TextFormat DToolTip::toolTipTextFormat()
{
    return DToolTipStatic::textFormat;
}

/*!
 * @brief DToolTip::setToolTipShowMode
 * @param widget widget to set ToolTip show mode
 * @param mode ToolTip show mode
 */
void DToolTip::setToolTipShowMode(QWidget *widget, ToolTipShowMode mode)
{
    widget->setProperty("_d_dtk_toolTipMode", mode);
}

/*!
 * @brief DToolTip::toolTipShowMode
 * @param widget widget to get ToolTip show mode
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
    for (const QString &paragraph : qAsConst(paragraphs))
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
            toolTip.append(toolTipLayout.text().midRef(currentLine.textStart(), currentLine.textLength()));
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
  \class Dtk::Widget::DToolTip
  \inmodule dtkwidget
  \brief DToolTip类似于 QToolTip 功能的类.

  DToolTip提供了将 DToolTip 显示在指定位置的函数
 */

/*!
  \brief DToolTip的构造函数.

  \a text 文本信息
  \a completionClose 提示消息消失后是否销毁
 */
DToolTip::DToolTip(const QString &text, bool completionClose)
    : DTipLabel(text)
{
    if (completionClose) {
        setAttribute(Qt::WA_DeleteOnClose);
    }
}

/*!
  \brief DToolTip::sizeHint.

  \sa QLabel::sizeHint()
 */
QSize DToolTip::sizeHint() const
{
    int radius = DStyleHelper(style()).pixelMetric(DStyle::PM_FrameRadius);
    QSize fontSize = fontMetrics().size(Qt::TextSingleLine, text());

    fontSize.setWidth(fontSize.width() + radius);

    return fontSize;
}

/*!
  \brief DToolTip::show设置提示消息出现的位置.

  \a pos 提示消息位置
  \a duration 提示消息存在时间，单位毫秒
 */
void DToolTip::show(const QPoint &pos, int duration)
{
    QTimer::singleShot(duration, this, &DTipLabel::close);

    DTipLabel::show(pos);
}

DWIDGET_END_NAMESPACE
