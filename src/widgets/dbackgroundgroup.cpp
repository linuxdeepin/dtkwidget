#include "dbackgroundgroup.h"
#include "dstyleoption.h"
#include <QBoxLayout>

#include <QStylePainter>
#include <QStyleOptionButton>

DWIDGET_BEGIN_NAMESPACE

void DBackgroundGroup::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    DStyleOptionBackgroundGroup::BackgroundDirection direction;
    if (QBoxLayout* boxLayout = qobject_cast<QBoxLayout*>(layout())) {
        if (boxLayout->direction() == QBoxLayout::LeftToRight ||
            boxLayout->direction() == QBoxLayout::RightToLeft) {
            direction = DStyleOptionBackgroundGroup::horizontal;
        }
        else {
            direction = DStyleOptionBackgroundGroup::vertical;
        }

        QList<QWidget*> items = findChildren<QWidget*>();
        QList<QPair<QWidget*, DStyleOptionBackgroundGroup::ItemBackgroundPosition>> itemStyleOptions;
        itemStyleOptions.reserve(items.size());

        auto createOption = [=] (QWidget* obj, DStyleOptionBackgroundGroup::ItemBackgroundPosition position) {
            return  QPair<QWidget*, DStyleOptionBackgroundGroup::ItemBackgroundPosition>(obj, position);
        };

        switch (items.size()) {
        case 0:
            itemStyleOptions << createOption(nullptr, DStyleOptionBackgroundGroup::Invalid);
            break;
        default: {
            QWidget* first = items.first();
            QWidget* last = items.last();

            if (first == last) {
                itemStyleOptions << createOption(first, DStyleOptionBackgroundGroup::OnlyOne);
                break;
            }

            items.removeOne(first);
            items.removeOne(last);

            itemStyleOptions << createOption(first, DStyleOptionBackgroundGroup::Beginning);

            for (auto it = items.cbegin(); it != items.cend(); ++it) {
                itemStyleOptions << createOption(*it, DStyleOptionBackgroundGroup::Middle);
            }

            itemStyleOptions << createOption(last, DStyleOptionBackgroundGroup::End);
        }
            break;
        }

        QStylePainter painter(this);
        for (QPair<QWidget*, DStyleOptionBackgroundGroup::ItemBackgroundPosition> pair : itemStyleOptions) {
            DStyleOptionBackgroundGroup option;
            if (!pair.first) continue;
            option.init(pair.first);
            option.direction = direction;
            option.position = pair.second;
            painter.drawPrimitive(static_cast<QStyle::PrimitiveElement>(DStyleOption::PE_BACKGROUND), option);
        }
    }
}

DWIDGET_END_NAMESPACE
