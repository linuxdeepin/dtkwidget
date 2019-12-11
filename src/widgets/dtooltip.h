#ifndef DTOOLTIP_H
#define DTOOLTIP_H

#include <DTipLabel>
#include <dobject.h>
#include <dtkwidget_global.h>

DWIDGET_BEGIN_NAMESPACE

class DToolTip : public DTipLabel
{
    Q_OBJECT

public:
    explicit DToolTip(const QString &text, bool completionClose = true);

    QSize sizeHint() const override;
    void show(const QPoint &pos, int duration);
};

DWIDGET_END_NAMESPACE

#endif // DTOOLTIP_H
