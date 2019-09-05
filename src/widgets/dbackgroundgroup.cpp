#include "dbackgroundgroup.h"
#include "dstyleoption.h"
#include "dstyle.h"
#include "dobject_p.h"

#include <QBoxLayout>
#include <QStylePainter>
#include <QEvent>

DWIDGET_BEGIN_NAMESPACE

class DBackgroundGroupPrivate : public DCORE_NAMESPACE::DObjectPrivate
{
public:
    DBackgroundGroupPrivate(DBackgroundGroup *qq)
        : DObjectPrivate(qq)
    {

    }

    void updateOptions()
    {
        D_QC(DBackgroundGroup);

        QList<QWidget*> items = q->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
        QList<QPair<QWidget*, DStyleOptionBackgroundGroup::ItemBackgroundPosition>> itemStyleOptions;
        itemStyleOptions.reserve(items.size());

        if (items.isEmpty()) {
            itemStyleOptions << qMakePair(nullptr, DStyleOptionBackgroundGroup::Invalid);
        } else {
            QWidget* first = items.first();
            QWidget* last = items.last();

            if (first == last) {
                itemStyleOptions << qMakePair(first, DStyleOptionBackgroundGroup::OnlyOne);
            } else {
                items.removeOne(first);
                items.removeOne(last);

                if (q->layoutDirection() == Qt::LeftToRight) {
                    itemStyleOptions << qMakePair(first, DStyleOptionBackgroundGroup::Beginning);
                } else {
                    itemStyleOptions << qMakePair(first, DStyleOptionBackgroundGroup::End);
                }

                for (auto it = items.cbegin(); it != items.cend(); ++it) {
                    itemStyleOptions << qMakePair(*it, DStyleOptionBackgroundGroup::Middle);
                }

                if (q->layoutDirection() == Qt::LeftToRight) {
                    itemStyleOptions << qMakePair(last, DStyleOptionBackgroundGroup::End);
                } else {
                    itemStyleOptions << qMakePair(last, DStyleOptionBackgroundGroup::Beginning);
                }
            }
        }

        this->itemStyleOptions = itemStyleOptions;
    }

    Qt::Orientations direction;
    QList<QPair<QWidget*, DStyleOptionBackgroundGroup::ItemBackgroundPosition>> itemStyleOptions;
    QMargins itemMargins;
    bool useWidgetBackground = true;

    D_DECLARE_PUBLIC(DBackgroundGroup)
};

DBackgroundGroup::DBackgroundGroup(QLayout *layout, QWidget *parent)
    : QWidget(parent)
    , DObject(*new DBackgroundGroupPrivate(this))
{
    setLayout(layout);
    // 默认使用窗口背景作为item背景
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(false);

    if (QBoxLayout* boxLayout = qobject_cast<QBoxLayout*>(layout)) {
        if (boxLayout->direction() == QBoxLayout::LeftToRight ||
            boxLayout->direction() == QBoxLayout::RightToLeft) {
            d_func()->direction = Qt::Horizontal;
        } else {
            d_func()->direction = Qt::Vertical;
        }
    }
}

QMargins DBackgroundGroup::itemMargins() const
{
    D_DC(DBackgroundGroup);

    return d->itemMargins;
}

bool DBackgroundGroup::useWidgetBackground() const
{
    D_DC(DBackgroundGroup);

    return d->useWidgetBackground;
}

void DBackgroundGroup::setItemMargins(QMargins itemMargins)
{
    D_D(DBackgroundGroup);

    d->itemMargins = itemMargins;
    update();
}

void DBackgroundGroup::setUseWidgetBackground(bool useWidgetBackground)
{
    D_D(DBackgroundGroup);

    if (d->useWidgetBackground == useWidgetBackground)
        return;

    d->useWidgetBackground = useWidgetBackground;
    Q_EMIT useWidgetBackgroundChanged(useWidgetBackground);
}

void DBackgroundGroup::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    DStylePainter painter(this);
    D_DC(DBackgroundGroup);

    for (auto pair : d->itemStyleOptions) {
        DStyleOptionBackgroundGroup option;
        if (!pair.first) continue;
        option.init(pair.first);
        option.rect += d->itemMargins;
        option.directions = d->direction;
        option.position = pair.second;

        if (d->useWidgetBackground) {
            option.dpalette.setBrush(DPalette::ItemBackground, palette().brush(backgroundRole()));
        }

        painter.drawPrimitive(DStyle::PE_ItemBackground, option);
    }
}

bool DBackgroundGroup::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::ChildAdded:
    case QEvent::ChildRemoved: {
        QChildEvent *ce = static_cast<QChildEvent*>(event);

        if (!ce->child()->isWidgetType()) {
            break;
        } else {
            Q_FALLTHROUGH();
        }
    }
    case QEvent::LayoutDirectionChange: {
        D_D(DBackgroundGroup);
        d->updateOptions();
        break;
    }
    default:
        break;
    }

    return QWidget::event(event);
}

DWIDGET_END_NAMESPACE
