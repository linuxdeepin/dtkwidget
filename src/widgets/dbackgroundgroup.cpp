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
  \class Dtk::Widget::DBackgroundGroup
  \inmodule dtkwidget
  \brief DBackgroundGroup提供了将布局控件改成圆角边框(将布局看成一个整体).

  \note 示例代码
  \code
  QWidget w;
  QHBoxLayout* mainlayout = new QHBoxLayout(&w);
  mainlayout->addWidget(new QPushButton("btn"));
  QHBoxLayout *hlayout = new QHBoxLayout;
  DBackgroundGroup *group = new DBackgroundGroup(hlayout,&w);
  mainlayout->addWidget(group);
  hlayout->addWidget(new QFrame);
  hlayout->addWidget(new QFrame);
  \endcode
  \image DBackgroundGroup.png
 */

/*!
  \brief DBackgroundGroup::DBackgroundGroup构造函数
  \a layout 布局对象
  \a parent 参数被发送到 QWidget 构造函数
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
  \brief DBackgroundGroup::itemMargins返回控件在布局内的边距
  \return 控件在布局内的边距
 */
QMargins DBackgroundGroup::itemMargins() const
{
    D_DC(DBackgroundGroup);

    return d->itemMargins;
}

/*!
  \brief DBackgroundGroup::useWidgetBackground是否使用 QWidget 背景颜色
  \return
 */
bool DBackgroundGroup::useWidgetBackground() const
{
    D_DC(DBackgroundGroup);

    return d->useWidgetBackground;
}

/*!
  \brief DBackgroundGroup::setLayout设置布局
  \a layout 布局
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
  \brief DBackgroundGroup::setItemMargins设置控件在布局内的边距
  \a itemMargins 控件在布局内的边距
 */
void DBackgroundGroup::setItemMargins(QMargins itemMargins)
{
    D_D(DBackgroundGroup);

    d->itemMargins = itemMargins;
    d->updateLayoutSpacing();
}

/*!
  \brief DBackgroundGroup::setItemSpacing设置布局内控件间的距离
  \a spacing 距离
 */
void DBackgroundGroup::setItemSpacing(int spacing)
{
    D_D(DBackgroundGroup);

    d->itemSpacing = spacing;
    d->updateLayoutSpacing();
}

/*!
  \brief DBackgroundGroup::setUseWidgetBackground设置是否使用 QWidget 背景颜色
  设置是否使用 QWidget 背景颜色,并发送 useWidgetBackgroundChanged 信号
  \a useWidgetBackground 是否使用 QWidget 背景颜色
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
        spacing = q->style()->pixelMetric(QStyle::PM_DefaultLayoutSpacing, nullptr, q);
    }

    if (lo == QBoxLayout::LeftToRight || lo == QBoxLayout::RightToLeft) {
        layout->setSpacing(itemMargins.left() + itemMargins.right() + spacing);
    } else {
        layout->setSpacing(itemMargins.top() + itemMargins.bottom() + spacing);
    }
}

DWIDGET_END_NAMESPACE
