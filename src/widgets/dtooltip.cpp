#include "dtooltip.h"
#include "dstyle.h"

#include <DPlatformWindowHandle>

#include <QDebug>
#include <QEvent>
#include <QTimer>

DWIDGET_BEGIN_NAMESPACE

/*!
 * \~chinese \class DToolTip
 * \~chinese \brief DToolTip类似于 QToolTip 功能的类
 * \~chinese \li DToolTip提供了将 DToolTip 显示在指定位置的函数
 */

/*!
 * \~chinese \brief DToolTip的构造函数
 * \~chinese \param text文本信息
 * \~chinese \param completionClose提示消息消失后是否销毁
 */
DToolTip::DToolTip(const QString &text, bool completionClose)
    : DTipLabel(text)
{
    if (completionClose) {
        setAttribute(Qt::WA_DeleteOnClose);
    }
}

/*!
 * \~chinese \brief DToolTip::sizeHint
 * \~chinese \sa QLabel::sizeHint()
 */
QSize DToolTip::sizeHint() const
{
    int radius = DStyleHelper(style()).pixelMetric(DStyle::PM_FrameRadius);
    QSize fontSize = fontMetrics().size(Qt::TextSingleLine, text());

    fontSize.setWidth(fontSize.width() + radius);

    return fontSize;
}

bool DToolTip::event(QEvent *e)
{
    if (e->type() == QEvent::Polish) {
        DPlatformWindowHandle handle(this);
        int radius = DStyleHelper(style()).pixelMetric(DStyle::PM_FrameRadius);
        handle.setWindowRadius(radius);
        setContentsMargins(radius / 2, 0, radius / 2, 0);
    }

    return QLabel::event(e);
}

/*!
 * \chinese \brief DToolTip::show设置提示消息出现的位置
 * \chinese \param pos提示消息位置
 * \chinese \param duration提示消息存在时间，单位毫秒
 */
void DToolTip::show(const QPoint &pos, int duration)
{
    QTimer::singleShot(duration, this, &DTipLabel::close);

    DTipLabel::show(pos);
}

DWIDGET_END_NAMESPACE
