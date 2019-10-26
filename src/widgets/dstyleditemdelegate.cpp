/*
 * Copyright (C) 2017 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "dstyleditemdelegate.h"
#include "dobject_p.h"
#include "dstyleoption.h"
#include "dapplicationhelper.h"

#include <QDebug>
#include <QApplication>
#include <QTextLayout>
#include <DStyle>
#include <QPainter>

Q_DECLARE_METATYPE(QMargins)

DWIDGET_BEGIN_NAMESPACE

class DViewItemActionPrivate : public DCORE_NAMESPACE::DObjectPrivate
{
public:
    DViewItemActionPrivate(DViewItemAction *qq)
        : DObjectPrivate(qq)
    {

    }

    Qt::Alignment alignment;
    QSize iconSize;
    QSize maxSize;
    bool clickable;
    QWidget *widget = nullptr;

    qint8 colorType = -1;
    qint8 colorRole = -1;
    qint8 fontSize = -1;
};

class DStyledItemDelegatePrivate : public DCORE_NAMESPACE::DObjectPrivate
{
public:
    DStyledItemDelegatePrivate(DStyledItemDelegate *qq)
        : DObjectPrivate(qq)
    {

    }

    static QSize actionSize(const DViewItemAction *action, const QSize &max, const QSize &fallbackIconSize, int spacing)
    {
        if (action->widget()) {
            return action->widget()->size();
        }

        const QString &text = action->text();
        QSize icon_size = action->iconSize();

        if (!icon_size.isValid() && !action->icon().isNull()) {
            icon_size = fallbackIconSize;
        }

        QSize max_size;

        max_size.setWidth(max.width() >= 0 ? max.width() : QWIDGETSIZE_MAX);
        max_size.setHeight(max.height() >= 0 ? max.height() : QWIDGETSIZE_MAX);

        if (text.isEmpty()) {
            return icon_size.boundedTo(max_size);
        }

        QFontMetrics fm(action->font());
        const QSize &text_size = fm.size(0, text);

        if (!icon_size.isValid())
            spacing = 0;

        return QSize(icon_size.width() + text_size.width() + spacing, qMax(icon_size.height(), text_size.height()));
    }

    static QSize displayActionSize(const DViewItemAction *action, const QStyle *style, const QStyleOptionViewItem &option)
    {
        QStyleOptionViewItem item;
        item.text = action->text();
        item.features = option.features | QStyleOptionViewItem::HasDisplay;
        item.font = action->font();
        item.fontMetrics = QFontMetrics(item.font);
        item.rect = option.rect;
        item.decorationPosition = option.decorationPosition;
        item.decorationSize = option.decorationSize;

        return DStyle::viewItemSize(style, &item, Qt::DisplayRole);
    }

    static QList<QRect> doActionsLayout(QRect base, const QList<DViewItemAction*> &list, Qt::Orientation orientation,
                                        Qt::LayoutDirection direction, const QSize &fallbackIconSize, QSize *bounding)
    {
        if (list.isEmpty()) {
            if (bounding) {
                bounding->setWidth(0);
                bounding->setHeight(0);
            }

            return {};
        }

        QList<QRect> result_list;
        int max_width = 0;
        int max_height = 0;
        QList<QSize> size_list;
        size_list.reserve(list.size());
        int spacing = DStyleHelper(qApp->style()).pixelMetric(DStyle::PM_ContentsSpacing);

        if (orientation == Qt::Horizontal) {
            // 记录上中下三列的宽度
            int top_width = 0;
            int center_width = 0;
            int bottom_width = 0;

            // 先找到最大宽度
            for (const DViewItemAction *action : list) {
                size_list.append(actionSize(action, action->maximumSize(), fallbackIconSize, spacing));

                if (action->alignment().testFlag(Qt::AlignVCenter)) {
                    center_width += size_list.last().width();
                } else if (action->alignment().testFlag(Qt::AlignBottom)) {
                    bottom_width += size_list.last().width();
                } else {
                    top_width += size_list.last().width();
                }
            }

            max_width = qBound(qMax(top_width, center_width), bottom_width, base.width()) + (list.size() -  1) * spacing;
            base.setWidth(max_width);

            QRect top_rect = base;
            QRect center_rect = base;
            QRect bottom_rect = base;

            // 开始布局
            for (int i = 0; i < list.size(); ++i) {
                const DViewItemAction *action = list.at(i);

                if (action->alignment().testFlag(Qt::AlignVCenter)) {
                    result_list << QStyle::alignedRect(direction, action->alignment(), size_list.at(i), center_rect);
                    center_rect.setLeft(result_list.last().right() + spacing);
                } else if (action->alignment().testFlag(Qt::AlignBottom)) {
                    result_list << QStyle::alignedRect(direction, action->alignment(), size_list.at(i), bottom_rect);
                    bottom_rect.setLeft(result_list.last().right() + spacing);
                } else {
                    result_list << QStyle::alignedRect(direction, action->alignment(), size_list.at(i), top_rect);
                    top_rect.setLeft(result_list.last().right() + spacing);
                }

                if (bounding)
                    max_height = qMax(max_height, result_list.last().bottom() - base.top() + 1);
            }
        } else {
            // 记录左中右三列的宽度
            int left_height = 0;
            int center_height = 0;
            int right_height = 0;

            // 先找到最大高度
            for (const DViewItemAction *action : list) {
                size_list.append(actionSize(action, action->maximumSize(), fallbackIconSize, spacing));

                if (action->alignment().testFlag(Qt::AlignHCenter)) {
                    center_height += size_list.last().height();
                } else if (action->alignment().testFlag(Qt::AlignRight)) {
                    right_height += size_list.last().height();
                } else {
                    left_height += size_list.last().height();
                }
            }

            max_height = qBound(qMax(left_height, center_height), right_height, base.height()) + (list.size() -  1) * spacing;
            base.setHeight(max_height);

            QRect left_rect = base;
            QRect center_rect = base;
            QRect right_rect = base;

            // 开始布局
            for (int i = 0; i < list.size(); ++i) {
                const DViewItemAction *action = list.at(i);

                if (action->alignment().testFlag(Qt::AlignHCenter)) {
                    result_list << QStyle::alignedRect(direction, action->alignment(), size_list.at(i), center_rect);
                    center_rect.setTop(result_list.last().bottom() + spacing);
                } else if (action->alignment().testFlag(Qt::AlignRight)) {
                    result_list << QStyle::alignedRect(direction, action->alignment(), size_list.at(i), right_rect);
                    right_rect.setTop(result_list.last().bottom() + spacing);
                } else {
                    result_list << QStyle::alignedRect(direction, action->alignment(), size_list.at(i), left_rect);
                    left_rect.setTop(result_list.last().bottom() + spacing);
                }

                if (bounding)
                    max_height = qMax(max_height, result_list.last().bottom() - base.top() + 1);
            }
        }

        if (bounding) {
            bounding->setWidth(max_width);
            bounding->setHeight(max_height);
        }

        return result_list;
    }

    static void drawAction(QPainter *pa, const QStyleOptionViewItem &option, const QRect &rect, const DViewItemAction *action, int spacing)
    {
        if (!action->isVisible()) {
            return;
        }

        if (action->widget()) {
            if (action->widget()->geometry() != rect) {
                action->widget()->setGeometry(rect);
            }

            return;
        }

        QSize icon_size(action->iconSize());

        if (!icon_size.isValid() && !action->icon().isNull()) {
            icon_size = option.decorationSize;
        }

        // 图标的绘制用也可能会使用这些颜色
        QPalette::ColorGroup cg = option.state & QStyle::State_Enabled
                              ? QPalette::Normal : QPalette::Disabled;
        if (cg == QPalette::Normal && !(option.state & QStyle::State_Active))
            cg = QPalette::Inactive;

        if (option.state & QStyle::State_Selected) {
            pa->setPen(option.palette.color(cg, QPalette::HighlightedText));
        } else {
            if (action->textColorType() > 0) {
                pa->setPen(QPen(DApplicationHelper::instance()->palette(option.widget).brush(cg, action->textColorType()), 1));
            } else {
                QPalette::ColorRole role = action->textColorRole() > 0 ? action->textColorRole() : QPalette::Text;
                pa->setPen(QPen(option.palette.brush(cg, role), 1));
            }
        }

        // draw icon
        if (icon_size.isValid()) {
            const QIcon &icon = action->icon();
            QRect icon_rect(QPoint(0, 0), icon_size);

            icon_rect.moveCenter(rect.center());
            icon_rect.moveLeft(rect.left());
            icon.paint(pa, icon_rect);
        }

        // draw text
        if (!action->text().isEmpty()) {
            QRect text_rect = rect;

            if (!icon_size.isValid()) {
                spacing = 0;
            }

            text_rect.setLeft(text_rect.left() + icon_size.width() + spacing);
            pa->setFont(action->font());
            pa->drawText(text_rect, Qt::AlignVCenter, action->text());
        }
    }

    static QSize drawActions(QPainter *pa, const QStyleOptionViewItem &option, const QVariant &value, Qt::Edge edge, QList<QPair<QAction*, QRect>> *clickableActionRect)
    {
        const DViewItemActionList &actionList = qvariant_cast<DViewItemActionList>(value);
        const Qt::Orientation orientation = (edge == Qt::TopEdge || edge == Qt::BottomEdge) ? Qt::Vertical : Qt::Horizontal;
        QSize bounding;
        const QList<QRect> &list = doActionsLayout(option.rect, actionList, orientation, option.direction, option.decorationSize, &bounding);
        QPoint origin(0, 0);
        int spacing = DStyleHelper(qApp->style()).pixelMetric(DStyle::PM_ContentsSpacing);

        switch (edge) {
        case Qt::BottomEdge:
            origin = QPoint(0, option.rect.height() - bounding.height());
            break;
        case Qt::RightEdge:
            origin = QPoint(option.rect.width() - bounding.width(), 0);
            break;
        default:
            break;
        }

        for (int i = 0; i < list.count(); ++i) {
            DViewItemAction *action = actionList.at(i);
            const QRect &rect = list.at(i).translated(origin);

            drawAction(pa, option, rect, action, spacing);

            if (action->isClickable()) {
                clickableActionRect->append(qMakePair(action, rect));
            }
        }

        return bounding;
    }

    DStyledItemDelegate::BackgroundType backgroundType;
    QMargins margins;
    QSize itemSize;
    int itemSpacing = 0;
    QMap<QModelIndex, QList<QPair<QAction*, QRect>>> clickableActionMap;
    QAction *pressedAction = nullptr;
};

DViewItemAction::DViewItemAction(Qt::Alignment alignment, const QSize &iconSize, const QSize &maxSize, bool clickable)
    : QAction(nullptr)
    , DObject(*new DViewItemActionPrivate(this))
{
    D_D(DViewItemAction);

    d->alignment = alignment;
    d->iconSize = iconSize;
    d->maxSize = maxSize;
    d->clickable = clickable;
}

DViewItemAction::DViewItemAction(Qt::Alignment alignment, const QSize &iconSize,
                                 const QSize &maxSize, bool clickable, QObject *parent)
    : DViewItemAction(alignment, iconSize, maxSize, clickable)
{
    if (parent) {
        qWarning() << "setting parent for a DViewItemAction object is no longer supported.";
    }
}

Qt::Alignment DViewItemAction::alignment() const
{
    D_DC(DViewItemAction);

    return d->alignment;
}

QSize DViewItemAction::iconSize() const
{
    D_DC(DViewItemAction);

    return d->iconSize;
}

QSize DViewItemAction::maximumSize() const
{
    D_DC(DViewItemAction);

    return d->maxSize;
}

void DViewItemAction::setTextColorRole(DPalette::ColorType role)
{
    D_D(DViewItemAction);

    d->colorRole = -1;
    d->colorType = role;
}

void DViewItemAction::setTextColorRole(QPalette::ColorRole role)
{
    D_D(DViewItemAction);

    d->colorType = -1;
    d->colorRole = role;
}

DPalette::ColorType DViewItemAction::textColorType() const
{
    D_DC(DViewItemAction);

    return static_cast<DPalette::ColorType>(d->colorType);
}

DPalette::ColorRole DViewItemAction::textColorRole() const
{
    D_DC(DViewItemAction);

    return static_cast<DPalette::ColorRole>(d->colorRole);
}

void DViewItemAction::setFontSize(DFontSizeManager::SizeType size)
{
    D_D(DViewItemAction);

    d->fontSize = size;
}

QFont DViewItemAction::font() const
{
    D_DC(DViewItemAction);

    if (d->fontSize < 0) {
        return QAction::font();
    }

    return DFontSizeManager::instance()->get(static_cast<DFontSizeManager::SizeType>(d->fontSize), QAction::font());
}

bool DViewItemAction::isClickable() const
{
    D_DC(DViewItemAction);

    return d->clickable;
}

void DViewItemAction::setWidget(QWidget *widget)
{
    D_D(DViewItemAction);

    d->widget = widget;
}

QWidget *DViewItemAction::widget() const
{
    D_DC(DViewItemAction);

    return d->widget;
}

DStyledItemDelegate::DStyledItemDelegate(QAbstractItemView *parent)
    : QStyledItemDelegate(parent)
    , DObject(*new DStyledItemDelegatePrivate(this))
{
    //支持QAction的点击
    parent->viewport()->installEventFilter(this);

    // 初始化 background type. 注意 setBackgroundType() 中有额外的处理操作，所以不能直接简单的修改默认值
    setBackgroundType(DStyledItemDelegate::RoundedBackground);
}

void DStyledItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    D_DC(DStyledItemDelegate);

    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);
    QRect backup_opt_rect = opt.rect;

    const QWidget *widget = option.widget;
    QStyle *style = widget ? widget->style() : QApplication::style();

    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, widget);

    // draw the background
    if (d->backgroundType != NoBackground && !(opt.state & QStyle::State_Selected)) {
        DStyleOptionBackgroundGroup boption;
        boption.init(widget);
        boption.QStyleOption::operator =(opt);
        boption.position = DStyleOptionBackgroundGroup::ItemBackgroundPosition(opt.viewItemPosition);

        if (opt.backgroundBrush.style() != Qt::NoBrush) {
            boption.dpalette.setBrush(DPalette::ItemBackground, opt.backgroundBrush);
        }

        boption.rect = opt.rect;

        if (d->backgroundType != RoundedBackground) {
            boption.directions = Qt::Vertical;
        }

        style->drawPrimitive(static_cast<QStyle::PrimitiveElement>(DStyle::PE_ItemBackground), &boption, painter, widget);
    }

    // 设置内容区域
    QMargins margins = d->margins;
    const QVariant &margins_varinat = index.data(Dtk::MarginsRole);

    if (margins_varinat.isValid()) {
        margins = qvariant_cast<QMargins>(margins_varinat);
    }

    opt.rect = opt.rect.marginsRemoved(margins);
    QRect itemContentRect = opt.rect;
    QSize action_area_size(0, 0);
    QList<QPair<QAction*, QRect>> clickActionList;
    int spacing = DStyleHelper(qApp->style()).pixelMetric(DStyle::PM_ContentsSpacing);

    action_area_size = d->drawActions(painter, opt, index.data(Dtk::LeftActionListRole), Qt::LeftEdge, &clickActionList);
    itemContentRect.setLeft(itemContentRect.left() + action_area_size.width() + (action_area_size.isNull() ? 0 : spacing));

    action_area_size = d->drawActions(painter, opt, index.data(Dtk::RightActionListRole), Qt::RightEdge, &clickActionList);
    itemContentRect.setRight(itemContentRect.right() - action_area_size.width() - (action_area_size.isNull() ? 0 : spacing));

    action_area_size = d->drawActions(painter, opt, index.data(Dtk::TopActionListRole), Qt::TopEdge, &clickActionList);
    itemContentRect.setTop(itemContentRect.top() + action_area_size.height() + (action_area_size.isNull() ? 0 : spacing));

    action_area_size = d->drawActions(painter, opt, index.data(Dtk::BottomActionListRole), Qt::BottomEdge, &clickActionList);
    itemContentRect.setBottom(itemContentRect.bottom() - action_area_size.height() - (action_area_size.isNull() ? 0 : spacing));

    if (!clickActionList.isEmpty()) {
        const_cast<DStyledItemDelegatePrivate*>(d)->clickableActionMap[index] = clickActionList;
    } else {
        const_cast<DStyledItemDelegatePrivate*>(d)->clickableActionMap.remove(index);
    }

    const DViewItemActionList &text_action_list = qvariant_cast<DViewItemActionList>(index.data(Dtk::TextActionListRole));

    opt.rect = itemContentRect;
    QRect iconRect, textRect, checkRect;
    DStyle::viewItemLayout(style, &opt, &iconRect, &textRect, &checkRect, false);

    // 图标的绘制用也可能会使用这些颜色
    QPalette::ColorGroup cg = opt.state & QStyle::State_Enabled
                          ? QPalette::Normal : QPalette::Disabled;
    if (opt.state & QStyle::State_Selected) {
        painter->setPen(opt.palette.color(cg, QPalette::HighlightedText));
    } else {
        painter->setPen(opt.palette.color(cg, QPalette::Text));
    }

    // draw icon
    if (opt.features & QStyleOptionViewItem::HasDecoration) {
        QIcon::Mode mode = QIcon::Normal;
        if (!(opt.state & QStyle::State_Enabled))
            mode = QIcon::Disabled;
        else if (opt.state & QStyle::State_Selected)
            mode = QIcon::Selected;
        QIcon::State state = opt.state & QStyle::State_Open ? QIcon::On : QIcon::Off;
        opt.icon.paint(painter, iconRect, opt.decorationAlignment, mode, state);
    }

    // draw the text
    if (opt.features & QStyleOptionViewItem::HasDisplay) {
        if (opt.state & QStyle::State_Editing) {
            painter->setPen(opt.palette.color(cg, QPalette::Text));
            painter->drawRect(textRect.adjusted(0, 0, -1, -1));
        }

        if (!text_action_list.isEmpty()) {
            opt.displayAlignment &= ~Qt::AlignBottom;
            opt.displayAlignment &= ~Qt::AlignVCenter;
            opt.displayAlignment |= Qt::AlignTop;
        }

        if (text_action_list.isEmpty()) {
            DStyle::viewItemDrawText(style, painter, &opt, textRect);
        } else {
            opt.displayAlignment &= ~Qt::AlignVCenter;
            opt.displayAlignment &= ~Qt::AlignBottom;

            QRect bounding = DStyle::viewItemDrawText(style, painter, &opt, textRect);

            // draw action text
            for (const DViewItemAction *action : text_action_list) {
                textRect.setTop(bounding.bottom());
                opt.font = action->font();
                opt.text = action->text();

                if (opt.state & QStyle::State_Selected) {
                    painter->setPen(opt.palette.color(cg, QPalette::HighlightedText));
                } else if (action->textColorType() >= 0) {
                    painter->setPen(DApplicationHelper::instance()->palette(widget).color(cg, action->textColorType()));
                } else if (action->textColorRole() >= 0) {
                    painter->setPen(opt.palette.color(cg, action->textColorRole()));
                } else {
                    painter->setPen(opt.palette.color(cg, QPalette::Text));
                }

                bounding = DStyle::viewItemDrawText(style, painter, &opt, textRect);
            }
        }
    }

    // draw the check mark
    if (opt.features & QStyleOptionViewItem::HasCheckIndicator) {
        QStyleOptionViewItem option(opt);
        option.rect = checkRect;
        option.state = option.state & ~QStyle::State_HasFocus;

        switch (opt.checkState) {
        case Qt::Unchecked:
            option.state |= QStyle::State_Off;
            break;
        case Qt::PartiallyChecked:
            option.state |= QStyle::State_NoChange;
            break;
        case Qt::Checked:
            option.state |= QStyle::State_On;
            break;
        }

        style->drawPrimitive(QStyle::PE_IndicatorViewItemCheck, &option, painter, widget);
    }

    // reset rect for focus rect
    opt.rect = backup_opt_rect;

    if (opt.state & QStyle::State_HasFocus) {
        QStyleOptionFocusRect o;
        o.QStyleOption::operator=(opt);
        o.rect = style->subElementRect(QStyle::SE_ItemViewItemFocusRect, &opt, widget);
        o.state |= QStyle::State_KeyboardFocusChange;
        o.state |= QStyle::State_Item;
        QPalette::ColorGroup cg = (opt.state & QStyle::State_Enabled)
                      ? QPalette::Normal : QPalette::Disabled;
        o.backgroundColor = opt.palette.color(cg, (opt.state & QStyle::State_Selected)
                                              ? QPalette::Highlight : QPalette::Window);
        style->drawPrimitive(QStyle::PE_FrameFocusRect, &o, painter, widget);
    }
}

QSize DStyledItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    D_DC(DStyledItemDelegate);

    if (d->itemSize.isValid()) {
        return d->itemSize;
    }

    QVariant value = index.data(Qt::SizeHintRole);

    if (value.isValid())
        return qvariant_cast<QSize>(value);

    const QWidget *widget = option.widget;
    QStyle *style = widget ? widget->style() : QApplication::style();
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    QRect pixmapRect, textRect, checkRect;
    DStyle::viewItemLayout(style, &opt, &pixmapRect, &textRect, &checkRect, true);

    const DViewItemActionList &text_action_list = qvariant_cast<DViewItemActionList>(index.data(Dtk::TextActionListRole));

    for (const DViewItemAction *action : text_action_list) {
        const QSize &action_size = d->displayActionSize(action, style, opt);
        textRect.setWidth(qMax(textRect.width(), action_size.width()));
        textRect.setHeight(textRect.height() + action_size.height());
    }

    QSize size = (pixmapRect | textRect | checkRect).size();

    const DViewItemActionList &left_actions = qvariant_cast<QList<DViewItemAction*>>(index.data(Dtk::LeftActionListRole));
    const DViewItemActionList &right_actions = qvariant_cast<QList<DViewItemAction*>>(index.data(Dtk::RightActionListRole));
    const DViewItemActionList &top_actions = qvariant_cast<QList<DViewItemAction*>>(index.data(Dtk::TopActionListRole));
    const DViewItemActionList &bottom_actions = qvariant_cast<QList<DViewItemAction*>>(index.data(Dtk::BottomActionListRole));

    QSize action_area_size;
    // 获取左边区域大小
    d->doActionsLayout(QRect(0, 0, QWIDGETSIZE_MAX, size.height()), left_actions, Qt::Horizontal,
                       option.direction, option.decorationSize, &action_area_size);
    size.setHeight(qMax(size.height(), action_area_size.height()));
    size.setWidth(size.width() + action_area_size.width());

    // 获取右边区域大小
    d->doActionsLayout(QRect(0, 0, QWIDGETSIZE_MAX, size.height()), right_actions, Qt::Horizontal,
                       option.direction, option.decorationSize, &action_area_size);
    size.setHeight(qMax(size.height(), action_area_size.height()));
    size.setWidth(size.width() + action_area_size.width());

    // 获取上面区域大小
    d->doActionsLayout(QRect(0, 0, size.width(), QWIDGETSIZE_MAX), top_actions, Qt::Vertical,
                       option.direction, option.decorationSize, &action_area_size);
    size.setHeight(size.height() + action_area_size.height());
    size.setWidth(qMax(size.width(), action_area_size.width()));

    // 获取下面区域大小
    d->doActionsLayout(QRect(0, 0, size.width(), QWIDGETSIZE_MAX), bottom_actions, Qt::Vertical,
                       option.direction, option.decorationSize, &action_area_size);
    size.setHeight(size.height() + action_area_size.height());
    size.setWidth(qMax(size.width(), action_area_size.width()));

    QMargins margins = d->margins;
    const QVariant &margins_varinat = index.data(Dtk::MarginsRole);

    if (margins_varinat.isValid()) {
        margins = qvariant_cast<QMargins>(margins_varinat);
    }

    // 在item高度上添加额外空间来模拟spacing
    size.rheight() += d->itemSpacing;

    return QRect(QPoint(0, 0), size).marginsAdded(margins).size();
}

DStyledItemDelegate::BackgroundType DStyledItemDelegate::backgroundType() const
{
    D_DC(DStyledItemDelegate);

    return d->backgroundType;
}

QMargins DStyledItemDelegate::margins() const
{
    D_DC(DStyledItemDelegate);

    return d->margins;
}

QSize DStyledItemDelegate::itemSize() const
{
    D_DC(DStyledItemDelegate);

    return d->itemSize;
}

int DStyledItemDelegate::spacing() const
{
    D_DC(DStyledItemDelegate);

    return d->itemSpacing;
}

void DStyledItemDelegate::setBackgroundType(DStyledItemDelegate::BackgroundType backgroundType)
{
    D_D(DStyledItemDelegate);

    if (d->backgroundType == backgroundType)
        return;

    d->backgroundType = backgroundType;
    d->margins = QMargins();

    if (d->backgroundType != NoBackground) {
        QStyle *style = qApp->style();

        if (QWidget *w = qobject_cast<QWidget *>(parent())) {
            style = w->style();
        }

        int frame_margin = style->pixelMetric(static_cast<QStyle::PixelMetric>(DStyle::PM_FrameRadius));
        d->margins += frame_margin;
    }
}

void DStyledItemDelegate::setMargins(const QMargins margins)
{
    D_D(DStyledItemDelegate);

    d->margins = margins;
}

void DStyledItemDelegate::setItemSize(QSize itemSize)
{
    D_D(DStyledItemDelegate);

    d->itemSize = itemSize;
}

void DStyledItemDelegate::setItemSpacing(int spacing)
{
    D_D(DStyledItemDelegate);

    d->itemSpacing = spacing;
}

void DStyledItemDelegate::initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const
{
    QStyledItemDelegate::initStyleOption(option, index);

    if (index.data(Dtk::TextActionListRole).isValid()) {
        option->features |= QStyleOptionViewItem::HasDisplay;
    }

    if ((option->state & (QStyle::State_HasFocus | QStyle::State_Selected)) == (QStyle::State_HasFocus | QStyle::State_Selected)) {
        if (const QAbstractItemView *view = qobject_cast<const QAbstractItemView*>(option->widget)) {
            // item 被选中背景色和获取焦点时的border样式类似，因此单选时不再绘制焦点样式
            if (view->selectionMode() == QAbstractItemView::SingleSelection) {
                option->state &= ~QStyle::State_HasFocus;
            }
        }
    }

    D_DC(DStyledItemDelegate);
    option->rect.adjust(0, 0, 0, -d->itemSpacing);
}

bool DStyledItemDelegate::eventFilter(QObject *object, QEvent *event)
{
    switch (event->type()) {
    case QEvent::MouseButtonPress: {
        D_D(DStyledItemDelegate);
        d->pressedAction = nullptr;
        Q_FALLTHROUGH();
    }
    case QEvent::MouseButtonRelease: {
        QMouseEvent *ev = static_cast<QMouseEvent*>(event);
        D_D(DStyledItemDelegate);

        QAbstractItemView *view = qobject_cast<QAbstractItemView*>(parent());
        const QModelIndex &index = view->indexAt(ev->pos());

        for (auto action_map : d->clickableActionMap.value(index)) {
            if (action_map.first->isEnabled()
                    && action_map.second.contains(ev->pos(), true)) {
                if (event->type() == QEvent::MouseButtonRelease
                        && d->pressedAction == action_map.first) {
                    action_map.first->trigger();
                } else {
                    d->pressedAction = action_map.first;
                }

                return true;
            }
        }
        break;
    }
    default:
        break;
    }

    return QStyledItemDelegate::eventFilter(object, event);
}

static Dtk::ItemDataRole getActionPositionRole(Qt::Edge edge)
{
    switch (edge) {
    case Qt::TopEdge:
        return Dtk::TopActionListRole;
    case Qt::RightEdge:
        return Dtk::RightActionListRole;
    case Qt::LeftEdge:
        return Dtk::LeftActionListRole;
    case Qt::BottomEdge:
        return Dtk::BottomActionListRole;
    default:
        break;
    }

    return Dtk::LeftActionListRole;
}

DStandardItem::~DStandardItem()
{
    for (Qt::Edge e : {Qt::TopEdge, Qt::LeftEdge, Qt::RightEdge, Qt::BottomEdge}) {
        DViewItemActionList list = qvariant_cast<DViewItemActionList>(data(getActionPositionRole(e)));
        for (auto action : list) {
            action->deleteLater();
        }
        setData(QVariant(), getActionPositionRole(e));
    }
}

void DStandardItem::setActionList(Qt::Edge edge, const DViewItemActionList &list)
{
    QVariant value;

    if (!list.isEmpty()) {
        value = QVariant::fromValue(list);
    }

    setData(value, getActionPositionRole(edge));
}

DViewItemActionList DStandardItem::actionList(Qt::Edge edge) const
{
    return qvariant_cast<DViewItemActionList>(data(getActionPositionRole(edge)));
}

void DStandardItem::setTextActionList(const DViewItemActionList &list)
{
    QVariant value;

    if (!list.isEmpty()) {
        value = QVariant::fromValue(list);
    }

    setData(value, Dtk::TextActionListRole);
}

DViewItemActionList DStandardItem::textActionList() const
{
    return qvariant_cast<DViewItemActionList>(data(Dtk::TextActionListRole));
}

DWIDGET_END_NAMESPACE
