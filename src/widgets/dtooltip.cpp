#include "dtooltip.h"
#include "dstyle.h"

#include <DPlatformWindowHandle>

#include <QDebug>
#include <QEvent>
#include <QTimer>

DWIDGET_BEGIN_NAMESPACE

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
