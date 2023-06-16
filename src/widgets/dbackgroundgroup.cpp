// SPDX-FileCopyrightText: 2022 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dbackgroundgroup.h"
#include "dstyleoption.h"
#include "dstyle.h"
#include "dobject_p.h"
#include "dsizemode.h"

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
        // fix-bug-36867：隐藏的 widget 不应该在列表中占用位置
        for (auto it = items.begin(); it != items.end();) {
            if (!*it || !(*it)->isVisible())
                it = items.erase(it);
            else
                ++it;
        }

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

    void updateLayoutSpacing();

    Qt::Orientations direction;
    QList<QPair<QWidget*, DStyleOptionBackgroundGroup::ItemBackgroundPosition>> itemStyleOptions;
    QMargins itemMargins;
    int itemSpacing = -1;
    bool useWidgetBackground = true;
    QPalette::ColorRole role = QPalette::Base;
    D_DECLARE_PUBLIC(DBackgroundGroup)
};

/*!
@~english
  \class Dtk::Widget::DBackgroundGroup
  \inmodule dtkwidget
  @brief DBackgroundGroup provides the ability to change the layout control to a rounded border(view the layout as a whole).

  @note sample code
  @code
  QWidget w;
  QHBoxLayout* mainlayout = new QHBoxLayout(&w);
  mainlayout->addWidget(new QPushButton("btn"));
  QHBoxLayout *hlayout = new QHBoxLayout;
  DBackgroundGroup *group = new DBackgroundGroup(hlayout,&w);
  mainlayout->addWidget(group);
  hlayout->addWidget(new QFrame);
  hlayout->addWidget(new QFrame);
  @endcode
  @image html DBackgroundGroup.png
 */

/*!
@~english
  @brief DBackgroundGroup::DBackgroundGroup constructor function
  \a layout Layout Object
  \a parent parameters are sent to QWidget constructor function
 */
DBackgroundGroup::DBackgroundGroup(QLayout *layout, QWidget *parent)
    : QWidget(parent)
    , DObject(*new DBackgroundGroupPrivate(this))
{
    // 默认使用窗口背景作为item背景
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(false);
    setLayout(layout);
}

/*!
@~english
  @brief DBackgroundGroup::itemMargins returns the margin of the control within the layout
  \return control margins within the layout
 */
QMargins DBackgroundGroup::itemMargins() const
{
    D_DC(DBackgroundGroup);

    return d->itemMargins;
}

/*!
@~english
  @brief DBackgroundGroup::useWidgetBackground whether to use the QWidget background color
  \return whether to use the QWidget background color
 */
bool DBackgroundGroup::useWidgetBackground() const
{
    D_DC(DBackgroundGroup);

    return d->useWidgetBackground;
}

/*!
@~english
  @brief DBackgroundGroup::setLayout setting the Layout
  \a layout layout
 */
void DBackgroundGroup::setLayout(QLayout *layout)
{
    QWidget::setLayout(layout);

    if (!layout)
        return;

    if (QBoxLayout* boxLayout = qobject_cast<QBoxLayout*>(layout)) {
        if (boxLayout->direction() == QBoxLayout::LeftToRight ||
            boxLayout->direction() == QBoxLayout::RightToLeft) {
            d_func()->direction = Qt::Horizontal;
        } else {
            d_func()->direction = Qt::Vertical;
        }
    }

    // 从布局中同步margins数据
    setItemMargins(layout->contentsMargins());
}

void DBackgroundGroup::setBackgroundRole(QPalette::ColorRole role)
{
    D_D(DBackgroundGroup);
    d->role = role;
}

QPalette::ColorRole DBackgroundGroup::backgroundRole() const
{
    D_DC(DBackgroundGroup);
    return d->role;
}

/*!
@~english
  @brief DBackgroundGroup::setItemMargins set the margins of the control within the layout
  \a itemMargins control margins within the layout
 */
void DBackgroundGroup::setItemMargins(QMargins itemMargins)
{
    D_D(DBackgroundGroup);

    d->itemMargins = itemMargins;
    d->updateLayoutSpacing();
}

/*!
@~english
  @brief DBackgroundGroup::setItemSpacing set the distance between the layout internal controls
  \a spacing distance
 */
void DBackgroundGroup::setItemSpacing(int spacing)
{
    D_D(DBackgroundGroup);

    d->itemSpacing = spacing;
    d->updateLayoutSpacing();
}

/*!
@~english
  @brief DBackgroundGroup::setUseWidgetBackground set whether to use the QWidget background color,and send the useWidgetBackgroundChanged signal
  \a useWidgetBackground whether to use QWidget background color
 */
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

        if (!pair.first->isVisible())
            continue;

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
        }
        update(); //重绘全部区域
        Q_FALLTHROUGH();
    }
    case QEvent::Show:
    case QEvent::LayoutDirectionChange:
    case QEvent::LayoutRequest: {
        D_D(DBackgroundGroup);
        d->updateOptions();
        break;
    }
    case QEvent::StyleChange: {
        D_D(DBackgroundGroup);
        d->updateLayoutSpacing();
    } break;
    default:
        break;
    }

    return QWidget::event(event);
}

void DBackgroundGroupPrivate::updateLayoutSpacing()
{
    D_Q(DBackgroundGroup);

    QLayout *layout = q->layout();
    QBoxLayout::Direction lo = QBoxLayout::LeftToRight;

    if (QBoxLayout *l = qobject_cast<QBoxLayout*>(layout)) {
        lo = l->direction();
    }

    int spacing = itemSpacing;

    if (spacing < 0) {
        if(lo == QBoxLayout::LeftToRight || lo == QBoxLayout::RightToLeft)
            spacing = q->style()->pixelMetric(QStyle::PM_LayoutHorizontalSpacing, nullptr, q);
        else
            spacing = q->style()->pixelMetric(QStyle::PM_LayoutVerticalSpacing, nullptr, q);
    }

    if (lo == QBoxLayout::LeftToRight || lo == QBoxLayout::RightToLeft) {
        layout->setSpacing(itemMargins.left() + itemMargins.right() + spacing);
    } else {
        layout->setSpacing(itemMargins.top() + itemMargins.bottom() + spacing);
    }
}

DWIDGET_END_NAMESPACE
