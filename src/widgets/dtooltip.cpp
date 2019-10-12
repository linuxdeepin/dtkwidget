#include "dtooltip.h"
#include "dstyle.h"

#include <DPlatformWindowHandle>

#include <QDebug>
#include <QEvent>
#include <QTimer>

DWIDGET_BEGIN_NAMESPACE

DToolTip::DToolTip(const QString &text, bool completionClose)
    : DTipLabel(text)
{
    if (completionClose) {
        setAttribute(Qt::WA_DeleteOnClose);
    }
}

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

void DToolTip::show(const QPoint &pos, int duration)
{
    QTimer::singleShot(duration, this, &DTipLabel::close);

    DTipLabel::show(pos);
}

DWIDGET_END_NAMESPACE
