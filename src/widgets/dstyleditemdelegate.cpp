// SPDX-FileCopyrightText: 2017 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dstyleditemdelegate.h"
#include "dobject_p.h"
#include "dstyleoption.h"
#include "dpalettehelper.h"
#include "dstyle.h"
#include "dsizemode.h"

#include "dlistview.h"

#include <QDebug>
#include <QApplication>
#include <QTextLayout>
#include <DStyle>
#include <QPainter>
#include <QListView>
#include <QLineEdit>
#include <QTableView>
#include <QListWidget>
#include <QPointer>
#include <private/qlayoutengine_p.h>
#include <DGuiApplicationHelper>
#include <DDciIcon>

Q_DECLARE_METATYPE(QMargins)

DWIDGET_BEGIN_NAMESPACE

struct ActionListData : public QSharedData {
    explicit ActionListData() { }
    explicit ActionListData(const DViewItemActionList& v)
        : list(v)
    {
    }
    ~ActionListData ()
    {
        qDeleteAll(list);
    }
    DViewItemActionList list;
};

class ActionList
{
public:
    explicit ActionList() { }
    explicit ActionList(ActionListData *data)
        : m_data(data)
    {
    }
    inline bool isValid() const
    {
        return m_data;
    }
    inline const ActionListData* constData() const
    {
        return m_data.constData();
    }
private:
    friend QDataStream &operator<<(QDataStream &s, const ActionList &v);
    friend QDataStream &operator>>(QDataStream &s, ActionList &v);
    QSharedDataPointer<ActionListData> m_data;
};

QDataStream &operator<<(QDataStream &ds, const ActionList &v)
{
    quintptr data = reinterpret_cast<quintptr>(v.m_data.data());
    ds << data;
    return ds;
}

QDataStream &operator>>(QDataStream &ds, ActionList &v)
{
    quintptr data;
    ds >> data;
    v.m_data = reinterpret_cast<ActionListData *>(data);
    return ds;
}
DWIDGET_END_NAMESPACE

Q_DECLARE_METATYPE(DTK_WIDGET_NAMESPACE::ActionList)

DWIDGET_BEGIN_NAMESPACE
static void saveDViewItemActionList(QDataStream &s, const void *d)
{
    const ActionList &data = *static_cast<const ActionList*>(d);
    s << data;
}

static void loadDViewItemActionList(QDataStream &s, void *d)
{
    ActionList &data = *static_cast<ActionList*>(d);
    s >> data;
}

__attribute__((constructor))
static void registerMetaType ()
{
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    // register DViewItemActionList's stream operator to support that QMetaType can using save and load function.
    QMetaType::registerStreamOperators(QMetaTypeId<DTK_WIDGET_NAMESPACE::ActionList>::qt_metatype_id(),
                                       saveDViewItemActionList,
                                       loadDViewItemActionList);
    #endif
}

static DViewItemActionList qvariantToActionList(const QVariant &v)
{
    const ActionList &wrapper = v.value<ActionList>();
    if (wrapper.isValid())
        return wrapper.constData()->list;

    return DViewItemActionList();
}

static QVariant actionListToQVariant(const DViewItemActionList &v)
{
    ActionList wrapper(new ActionListData(v));
    return QVariant::fromValue(wrapper);
}

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
    QMargins clickMargins;
    bool clickable = false;
    DDciIcon dciIcon;
    QPointer<QWidget> widget = nullptr;

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

    // get rect in AlignVCenter layout for text.
    static QRect textAndTextActionsLayout(const QRect &textRect, const QStyle *style, const QStyleOptionViewItem &option, const QModelIndex &index)
    {
        QStyleOptionViewItem opt = option;
        opt.displayAlignment |= Qt::AlignVCenter;
        QSize size;

        if (!opt.text.isEmpty())
            size = DStyle::viewItemSize(style, &opt, Qt::DisplayRole);

        for (const DViewItemAction *action : qvariantToActionList(index.data(Dtk::TextActionListRole))) {
            const QSize &action_size = displayActionSize(action, style, opt);
            size.setWidth(qMax(size.width(), action_size.width()));
            size.setHeight(size.height() + action_size.height());
        }

        return QStyle::alignedRect(opt.direction , opt.displayAlignment, size, textRect);
    }

    static QList<QRect> doActionsLayout(QRect base, const DViewItemActionList &list, Qt::Orientation orientation,
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
                pa->setPen(QPen(DPaletteHelper::instance()->palette(option.widget).brush(cg, action->textColorType()), 1));
            } else {
                QPalette::ColorRole role = action->textColorRole() > 0 ? action->textColorRole() : QPalette::Text;
                pa->setPen(QPen(option.palette.brush(cg, role), 1));
            }
        }

        // draw icon
        if (icon_size.isValid()) {
            QRect icon_rect(QPoint(0, 0), icon_size);

            icon_rect.moveCenter(rect.center());
            icon_rect.moveLeft(rect.left());

            if (action->dciIcon().isNull()) {
                const QIcon &icon = action->icon();
                auto modeStatePair = DStyle::toIconModeState(&option);
                icon.paint(pa, icon_rect, Qt::AlignCenter, modeStatePair.first, modeStatePair.second);
            } else {
                DDciIcon dciicon = action->dciIcon();
                DDciIcon::Mode mode = DStyle::toDciIconMode(&option);
                auto appTheme = DGuiApplicationHelper::toColorType(option.palette);
                DDciIcon::Theme theme = appTheme == DGuiApplicationHelper::LightType ? DDciIcon::Light : DDciIcon::Dark;
                DDciIconPalette palette{option.palette.color(cg, QPalette::WindowText), option.palette.color(cg, QPalette::Window),
                                        option.palette.color(cg, QPalette::Highlight), option.palette.color(cg, QPalette::HighlightedText)};
                if (option.state & QStyle::State_Selected)
                    palette.setForeground(option.palette.color(cg, QPalette::HighlightedText));
                dciicon.paint(pa, icon_rect, pa->device() ? pa->device()->devicePixelRatio()
                                                          : qApp->devicePixelRatio(), theme, mode, Qt::AlignCenter, palette);
            }
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
        const DViewItemActionList &actionList = qvariantToActionList(value);
        DViewItemActionList visiable_actionList;
        for (auto action : actionList) {
            if (action->isVisible())
                visiable_actionList.append(action);
            action->setEnabled(option.state & QStyle::State_Enabled);
            if (QWidget *actionWidget = action->widget()) {
                actionWidget->setVisible(action->isVisible());
                actionWidget->setEnabled(option.state & QStyle::State_Enabled);
            }
        }

        const Qt::Orientation orientation = (edge == Qt::TopEdge || edge == Qt::BottomEdge) ? Qt::Vertical : Qt::Horizontal;
        QSize bounding;
        const QList<QRect> &list = doActionsLayout(option.rect, visiable_actionList, orientation, option.direction, option.decorationSize, &bounding);
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
            DViewItemAction *action = visiable_actionList.at(i);
            const QRect &rect = list.at(i).translated(origin);

            drawAction(pa, option, rect, action, spacing);

            if (action->isClickable()) {
                clickableActionRect->append(qMakePair(action, rect.marginsAdded(action->clickAreaMargins())));
            }
        }

        return bounding;
    }

    static DViewItemActionList allActions(const QModelIndex &index)
    {
        static const QVector<ItemDataRole> rules {
            LeftActionListRole,
                    TopActionListRole,
                    RightActionListRole,
                    BottomActionListRole,
                    TextActionListRole
        };
        DViewItemActionList results;
        for (const auto role: rules) {
            const auto &list = qvariantToActionList(index.data(role));
            if (list.isEmpty())
                continue;
            results << list;
        }
        return results;
    }

    bool readyRecordVisibleWidgetOfCurrentFrame()
    {
        // multi QEvent maybe be merged to one using postEvent
        // so we can clear cache avoid to recording multi ItemWidget.
        if (Q_UNLIKELY(hasStartRecord)) {
            currentWidgets.clear();
            return false;
        }
        hasStartRecord = true;
        return true;
    }

    void updateWidgetVisibleInUnvisualArea()
    {
        hasStartRecord = false;
        if (lastWidgets.isEmpty() && currentWidgets.isEmpty())
            return;

        for (const auto &widget : qAsConst(lastWidgets)) {
            if (currentWidgets.contains(widget))
                continue;
            if (widget && widget->isVisible())
                widget->setVisible(false);
        }

        lastWidgets.swap(currentWidgets);
        currentWidgets.clear();
    }

    void recordVisibleWidgetOfCurrentFrame(const QModelIndex &index)
    {
        // only record virsual widget when starting record.
        if (Q_UNLIKELY(!hasStartRecord))
            return;

        for (auto action : allActions(index)) {
            if (!action->isVisible())
                continue;

            if (auto widget = action->widget())
                currentWidgets.append(QPointer<QWidget>(widget));
        }
    }

    DStyledItemDelegate::BackgroundType backgroundType = DStyledItemDelegate::NoBackground;
    QMargins margins;
    QSize itemSize;
    int itemSpacing = 0;
    QMap<QModelIndex, QList<QPair<QAction*, QRect>>> clickableActionMap;
    QAction *pressedAction = nullptr;
    QList<QPointer<QWidget>> lastWidgets;
    QList<QPointer<QWidget>> currentWidgets;
    bool hasStartRecord = false;
};

/*!
  \class Dtk::Widget::DViewItemAction
  \inmodule dtkwidget
  \brief 是一个 action 活动, 其默认是不能被点击的.
  
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  
  setActionList(Qt::BottomEdge, *list)　的　BottomEdge　为水平方向的枚举值时候: LeftEdge, RightEdge,　其对应　alignment　枚举数值只有如下的有效：
  
  其参数 alignment 为如下值时候(才有效):
  \list
  \li AlignTop
  \li AlignVCenter
  \li AlignBottom
  \endlist

  此处举一个小例子, 可以自行修改相关参数, 查看结果, 理解参数:
  \code
  QMainWindow *wid = new QMainWindow();
  wid->resize(800, 600);
  
  //视图和模型
  DListView *view = new DListView(wid);
  QStandardItemModel *model = new QStandardItemModel(view);
  view->setModel(model);
  
  //标准DStandardItem item
  DStandardItem *item = new DStandardItem();
  item->setText("@1:这是同一个item 的text\n@2:这是同一个item 的text\n@3:这是同一个item 的text\n@4:这是同一个item 的text\n@5:这是同一个item 的text\n@6:这是同一个item 的text");
  
  //DViewItemAction() 的形参: 其参数 alignment = AlignLeft 或 AlignHCenter 或  AlignRight; 设置为 AlignTop, AlignVCenter, AlignBottom 也是无效的
  DViewItemAction *act1 = new DViewItemAction(Qt::AlignBottom, QSize(15, 15), QSize(50, 50));
  act1->setText("act1");
  act1->setIcon(QIcon("/home/yuanyi/Desktop/dog.jpg"));
  DViewItemAction *act2 = new DViewItemAction(Qt::AlignVCenter, QSize(15, 15), QSize());
  act2->setText("act2");
  act2->setIcon(QIcon("/home/yuanyi/Desktop/dog.jpg"));
  DViewItemAction *act3 = new DViewItemAction(Qt::AlignTop, QSize(15, 15), QSize(100, 50));
  act3->setText("act3");
  act3->setIcon(QIcon("/home/yuanyi/Desktop/dog.jpg"));
  DViewItemAction *act4 = new DViewItemAction(Qt::AlignBottom, QSize(15, 15), QSize(100, 50));
  act4->setText("act4");
  act4->setIcon(QIcon("/home/yuanyi/Desktop/dog.jpg"));
  
  DViewItemActionList *list = new DViewItemActionList();
  list->append(act1);
  list->append(act2);
  list->append(act3);
  list->append(act4);
  
  //edge实参: TopEdg, BottomEdge (主要设置前面两个参数), LeftEdge, RightEdge
  item->setActionList(Qt::LeftEdge, *list);
  model->appendRow(item);
  
  wid->setCentralWidget(view);
  wid->show();
  \endcode

  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  
  setActionList(Qt::BottomEdge, *list)　的　BottomEdge　为竖直方向的枚举值时候: TopEdge, BottomEdge,　其对应　alignment　枚举数值只有如下的有效：
  
  其参数 alignment 为如下值时候(才有效):
  \list
  \li AlignLeft
  \li AlignHCenter
  \li AlignRight
  \endlist

  此处举一个小例子, 可以自行修改相关参数, 查看结果, 理解参数:
  \code
  QMainWindow *wid = new QMainWindow();
  wid->resize(800, 600);
  
  //视图和模型
  DListView *view = new DListView(wid);
  QStandardItemModel *model = new QStandardItemModel(view);
  view->setModel(model);
  
  //标准DStandardItem item
  DStandardItem *item = new DStandardItem();
  item->setText("这是一个item 的text");
  
  //DViewItemAction() 的形参: 其参数 alignment = AlignLeft 或 AlignHCenter 或  AlignRight; 设置为 AlignTop, AlignVCenter, AlignBottom 也是无效的
  DViewItemAction *act1 = new DViewItemAction(Qt::AlignLeft, QSize(15, 15), QSize(50, 50));
  act1->setText("act1");
  act1->setIcon(QIcon("/home/yuanyi/Desktop/dog.jpg"));
  DViewItemAction *act2 = new DViewItemAction(Qt::AlignRight, QSize(15, 15), QSize());
  act2->setText("act2");
  act2->setIcon(QIcon("/home/yuanyi/Desktop/dog.jpg"));
  DViewItemAction *act3 = new DViewItemAction(Qt::AlignHCenter, QSize(15, 15), QSize(100, 50));
  act3->setText("act3");
  act3->setIcon(QIcon("/home/yuanyi/Desktop/dog.jpg"));
  DViewItemAction *act4 = new DViewItemAction(Qt::AlignLeft, QSize(15, 15), QSize(100, 50));
  act4->setText("act4");
  act4->setIcon(QIcon("/home/yuanyi/Desktop/dog.jpg"));
  
  DViewItemActionList *list = new DViewItemActionList();
  list->append(act1);
  list->append(act2);
  list->append(act3);
  list->append(act4);
  
  //edge实参: TopEdg, BottomEdge (主要设置前面两个参数), LeftEdge, RightEdge
  item->setActionList(Qt::BottomEdge, *list);
  model->appendRow(item);
  
  wid->setCentralWidget(view);
  wid->show();
  \endcode
  \a alignment 设置本控件对象(在父控件的)方向位置: 居左, 居中, 居右 (其他枚举值此处无效)
  \a iconSize 本控件的图标 icon 的大小
 */

/*!
  \brief 构造函数
  \a alignment 设置放置的方位
  \a iconSize 设置图标 icon 的大小
  \a maxSize 控件矩形最大的大小
  \a clickable 是否允许点击动作的标识
 */
DViewItemAction::DViewItemAction(Qt::Alignment alignment, const QSize &iconSize, const QSize &maxSize, bool clickable)
    : QAction(nullptr)
    , DObject(*new DViewItemActionPrivate(this))
{
    D_D(DViewItemAction);

    d->alignment = alignment;
    d->iconSize = iconSize.isValid() ? iconSize : QSize(qApp->style()->pixelMetric(DStyle::PM_IndicatorWidth), qApp->style()->pixelMetric(DStyle::PM_IndicatorHeight));
    d->maxSize = maxSize;
    d->clickable = clickable;
}

#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
DViewItemAction::DViewItemAction(Qt::Alignment alignment, const QSize &iconSize,
                                 const QSize &maxSize, bool clickable, QObject *parent)
    : DViewItemAction(alignment, iconSize, maxSize, clickable)
{
    if (parent) {
        qWarning() << "setting parent for a DViewItemAction object is no longer supported.";
    }
}
#endif

/*!
  \brief 获取 DViewItemAction 放置的(水平)位置
  \return 返回 DViewItemAction 放置的(水平)位置
 */
Qt::Alignment DViewItemAction::alignment() const
{
    D_DC(DViewItemAction);

    return d->alignment;
}

/*!
  \brief 获取图标 icon 的矩形大小
  \return 返回图标 icon 的矩形大小
 */
QSize DViewItemAction::iconSize() const
{
    D_DC(DViewItemAction);

    return d->iconSize;
}

/*!
  \brief 获取最大的矩形大小
  \return 返回最大的矩形大小
 */
QSize DViewItemAction::maximumSize() const
{
    D_DC(DViewItemAction);

    return d->maxSize;
}

/*!
  \brief 获取点击区域范围
  \return 返回点击区域范围
 */
QMargins DViewItemAction::clickAreaMargins() const
{
    D_DC(DViewItemAction);

    return d->clickMargins;
}

/*!
  \brief 设置点击区域范围(eg: 只有图标　icon　时候，会点击区域比较小， 不方便点击， 可以将点击范围设置大一点，便于点击)
  \a margins 将被设置的点击区域间隔数值
 */
void DViewItemAction::setClickAreaMargins(const QMargins &margins)
{
    D_D(DViewItemAction);

    d->clickMargins = margins;
}

/*!
  \fn void DViewItemAction::setTextColorRole(DPalette::ColorType role)
  \brief 设置文本颜色的角色枚举数值 (DPalette)
  \overload
  \a role 文本颜色的枚举值
 */
void DViewItemAction::setTextColorRole(DPalette::ColorType role)
{
    D_D(DViewItemAction);

    d->colorRole = -1;
    d->colorType = role;
}

/*!
  \fn void DViewItemAction::setTextColorRole(QPalette::ColorRole role)
  \brief 设置文本颜色的角色枚举数值 (QPalette)
  \overload
  \a role 文本颜色的枚举值
 */
void DViewItemAction::setTextColorRole(QPalette::ColorRole role)
{
    D_D(DViewItemAction);

    d->colorType = -1;
    d->colorRole = role;
}

/*!
  \brief 获取文本颜色类型
  \return 返回文本颜色的类型
 */
DPalette::ColorType DViewItemAction::textColorType() const
{
    D_DC(DViewItemAction);

    return static_cast<DPalette::ColorType>(d->colorType);
}

/*!
  \brief 获取文本颜色角色
  \return 返回文本颜色角色
 */
DPalette::ColorRole DViewItemAction::textColorRole() const
{
    D_DC(DViewItemAction);

    return static_cast<DPalette::ColorRole>(d->colorRole);
}

/*!
  \brief 设置字体大小
  \a size 将要设置的字体大小值
 */
void DViewItemAction::setFontSize(DFontSizeManager::SizeType size)
{
    D_D(DViewItemAction);

    d->fontSize = size;
}

/*!
  \brief 获取字体
  \return 返回字体
 */
QFont DViewItemAction::font() const
{
    D_DC(DViewItemAction);

    if (d->fontSize < 0) {
        return QAction::font();
    }

    return DFontSizeManager::instance()->get(static_cast<DFontSizeManager::SizeType>(d->fontSize), QAction::font());
}

/*!
  \brief 获取是否允许被点击的状态(即：如果可以被点击返回true，否则返回false)
  \return 返回是否允许被点击的状态
 */
bool DViewItemAction::isClickable() const
{
    D_DC(DViewItemAction);

    return d->clickable;
}

/*!
  \brief 设置 QWidget
  \a widget 被添加进来的 QWidget
 */
void DViewItemAction::setWidget(QWidget *widget)
{
    D_D(DViewItemAction);

    d->widget = QPointer<QWidget>(widget);
    d->widget->setVisible(false);
}

/*!
  \brief 获取 QWidget 控件对象
  \return 返回 QWidget 控件对象
 */
QWidget *DViewItemAction::widget() const
{
    D_DC(DViewItemAction);

    return d->widget;
}

void DViewItemAction::setDciIcon(const DDciIcon &dciIcon)
{
    D_D(DViewItemAction);

    d->dciIcon = dciIcon;
}

DDciIcon DViewItemAction::dciIcon() const
{
    D_DC(DViewItemAction);

    return d->dciIcon;
}

static QPalette::ColorRole getViewItemColorRole(const QModelIndex &index, int role)
{
    const QVariant &value = index.data(role);

    if (!value.isValid())
        return QPalette::NoRole;

    auto pair = qvariant_cast<QPair<int, int>>(value);

    return static_cast<QPalette::ColorRole>(pair.first);
}

static DPalette::ColorType getViewItemColorType(const QModelIndex &index, int role)
{
    const QVariant &value = index.data(role);

    if (!value.isValid())
        return DPalette::NoType;

    auto pair = qvariant_cast<QPair<int, int>>(value);

    return static_cast<DPalette::ColorType>(pair.second);
}

static QFont getViewItemFont(const QModelIndex &index, int role)
{
    const QVariant &value = index.data(role);

    if (!value.isValid()) {
        return qvariant_cast<QFont>(index.data(Qt::FontRole));
    }

    DFontSizeManager::SizeType size = static_cast<DFontSizeManager::SizeType>(qvariant_cast<int>(value));
    return DFontSizeManager::instance()->get(size);
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
    bool editing = false;

    if (auto view = qobject_cast<const QListView*>(widget)) {
        editing = !qobject_cast<const QListWidget*>(view) && view->isPersistentEditorOpen(index);
    }

    if (!editing)
        style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, widget);

    // draw the background
    if (!editing && backgroundType() != NoBackground && !(opt.state & QStyle::State_Selected)
            && !(d->backgroundType & NoNormalState && DStyle::getState(&opt) == DStyle::SS_NormalState)) {
        DStyleOptionBackgroundGroup boption;
        boption.init(widget);
        boption.QStyleOption::operator =(opt);
        boption.position = DStyleOptionBackgroundGroup::ItemBackgroundPosition(opt.viewItemPosition);

        if (opt.backgroundBrush.style() != Qt::NoBrush) {
            boption.dpalette.setBrush(DPalette::ItemBackground, opt.backgroundBrush);
        }

        boption.rect = opt.rect;

        if (backgroundType() != RoundedBackground) {
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

    const DViewItemActionList &text_action_list = qvariantToActionList(index.data(Dtk::TextActionListRole));

    opt.rect = itemContentRect;
    QRect iconRect, textRect, checkRect;
    DStyle::viewItemLayout(style, &opt, &iconRect, &textRect, &checkRect, false);

    // 图标的绘制用也可能会使用这些颜色
    QPalette::ColorGroup cg = opt.state & QStyle::State_Enabled
                          ? QPalette::Normal : QPalette::Disabled;

    if (cg == QPalette::Normal && !(opt.state & QStyle::State_Active))
        cg = QPalette::Inactive;

    if (opt.state & QStyle::State_Selected) {
        painter->setPen(opt.palette.color(cg, QPalette::HighlightedText));
    } else {
        painter->setPen(opt.palette.color(cg, QPalette::Text));
    }

    // draw icon
    if (opt.features & QStyleOptionViewItem::HasDecoration) {
        QVariant icon = index.data(Qt::DecorationRole);
        DDciIcon dciIcon;
        if (icon.canConvert<DTK_GUI_NAMESPACE::DDciIcon>())
            dciIcon = qvariant_cast<DDciIcon>(icon);

        if (dciIcon.isNull()) {
            QIcon::Mode mode = QIcon::Normal;
            if (!(opt.state & QStyle::State_Enabled))
                mode = QIcon::Disabled;
            else if (opt.state & QStyle::State_Selected)
                mode = QIcon::Selected;
            QIcon::State state = opt.state & QStyle::State_Open ? QIcon::On : QIcon::Off;
            opt.icon.paint(painter, iconRect, opt.decorationAlignment, mode, state);
        } else {
            DDciIcon::Mode mode = DStyle::toDciIconMode(&option);
            auto appTheme = DGuiApplicationHelper::toColorType(option.palette);
            DDciIcon::Theme theme = appTheme == DGuiApplicationHelper::LightType ? DDciIcon::Light : DDciIcon::Dark;
            DDciIconPalette palette{option.palette.color(cg, QPalette::WindowText), option.palette.color(cg, QPalette::Window),
                                    option.palette.color(cg, QPalette::Highlight), option.palette.color(cg, QPalette::HighlightedText)};
            if (option.state & QStyle::State_Selected)
                palette.setForeground(opt.palette.color(cg, QPalette::HighlightedText));
            dciIcon.paint(painter, iconRect, painter->device() ? painter->device()->devicePixelRatioF()
                                                               : qApp->devicePixelRatio(),
                          theme, mode, Qt::AlignCenter, palette);
        }
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

            QRect textRectbounding = d->textAndTextActionsLayout(textRect, style, opt, index);

            QRect bounding(textRectbounding.topLeft(), QSize());
            if (!opt.text.isEmpty()) {
                bounding = DStyle::viewItemDrawText(style, painter, &opt, textRectbounding);
            }

            // draw action text
            for (const DViewItemAction *action : text_action_list) {
                textRect.setTop(bounding.bottom());
                opt.font = action->font();
                opt.text = action->text();

                if (opt.state & QStyle::State_Selected) {
                    painter->setPen(opt.palette.color(cg, QPalette::HighlightedText));
                } else if (action->textColorType() >= 0) {
                    painter->setPen(DPaletteHelper::instance()->palette(widget).color(cg, action->textColorType()));
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

        style->drawPrimitive(QStyle::PE_IndicatorItemViewItemCheck, &option, painter, widget);
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

    const_cast<DStyledItemDelegatePrivate*>(d)->recordVisibleWidgetOfCurrentFrame(index);
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

    const DViewItemActionList &text_action_list = qvariantToActionList(index.data(Dtk::TextActionListRole));

    for (const DViewItemAction *action : text_action_list) {
        const QSize &action_size = d->displayActionSize(action, style, opt);
        textRect.setWidth(qMax(textRect.width(), action_size.width()));
        textRect.setHeight(textRect.height() + action_size.height());
    }

    QSize size = (pixmapRect | textRect | checkRect).size();

    const DViewItemActionList &left_actions = qvariantToActionList(index.data(Dtk::LeftActionListRole));
    const DViewItemActionList &right_actions = qvariantToActionList(index.data(Dtk::RightActionListRole));
    const DViewItemActionList &top_actions = qvariantToActionList(index.data(Dtk::TopActionListRole));
    const DViewItemActionList &bottom_actions = qvariantToActionList(index.data(Dtk::BottomActionListRole));

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
    const QListView * lv = qobject_cast<const QListView*>(option.widget);
    if (lv) {
        if (lv->flow() == QListView::LeftToRight) {
            size.rwidth() += d->itemSpacing;
        } else {
            size.rheight() += d->itemSpacing;
        }
    }

    return QRect(QPoint(0, 0), size).marginsAdded(margins).size();
}

void DStyledItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!editor)
        return;
    Q_ASSERT(index.isValid());
    const QWidget *widget = option.widget;

    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    QRect rect = opt.rect;

    if (opt.features & QStyleOptionViewItem::HasDecoration) {
        opt.showDecorationSelected = true;

        QStyle *style = widget ? widget->style() : QApplication::style();
        QRect geom = style->subElementRect(QStyle::SE_ItemViewItemDecoration, &opt, widget);
        int spacing = DStyleHelper(style).pixelMetric(DStyle::PM_ContentsSpacing, &opt, widget);

        // 排除掉图标的位置，剩下的空间全都给输入框
        switch (opt.decorationPosition) {
        case QStyleOptionViewItem::Left:
            rect.setLeft(geom.right() + spacing);
            break;
        case QStyleOptionViewItem::Right:
            rect.setRight(geom.left() - spacing);
            break;
        case QStyleOptionViewItem::Top:
            rect.setTop(geom.bottom() + spacing);
            break;
        case QStyleOptionViewItem::Bottom:
            rect.setBottom(geom.top() - spacing);
            break;
        default:
            break;
        }
    }

    // 确保输入框的高度不会超出范围
    rect.setHeight(qMin(rect.height(), editor->sizeHint().height()));

    editor->setGeometry(rect);
}

DStyledItemDelegate::BackgroundType DStyledItemDelegate::backgroundType() const
{
    D_DC(DStyledItemDelegate);
    return DStyledItemDelegate::BackgroundType(d->backgroundType & BackgroundType_Mask);
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

void DStyledItemDelegate::setBackgroundType(DStyledItemDelegate::BackgroundType type)
{
    D_D(DStyledItemDelegate);

    if (d->backgroundType == type)
        return;

    d->backgroundType = type;
    d->margins = QMargins();

    if (backgroundType() != NoBackground) {
        QStyle *style = qApp->style();

        if (QWidget *w = qobject_cast<QWidget *>(parent())) {
            style = w->style();
        }

        int frame_margin = style->pixelMetric(static_cast<QStyle::PixelMetric>(DStyle::PM_FrameRadius));
        int left_right_margin = style->pixelMetric(static_cast<QStyle::PixelMetric>(DStyle::PM_ContentsMargins));
        d->margins += QMargins(left_right_margin, frame_margin, left_right_margin, frame_margin);
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

    QVariant value = index.data(Qt::DecorationRole);
    if (value.canConvert<DDciIcon>()) {
        // The dciicon can not be set to opt.icon
        auto dciIcon = qvariant_cast<DDciIcon>(value);
        DDciIcon::Mode mode;
        if (!(option->state & QStyle::State_Enabled))
            mode = DDciIcon::Disabled;
        else if (option->state & QStyle::State_Selected)
            mode = DDciIcon::Pressed;
        else
            mode = DDciIcon::Normal;
        auto appTheme = DGuiApplicationHelper::toColorType(option->palette);
        DDciIcon::Theme theme = appTheme == DGuiApplicationHelper::LightType ? DDciIcon::Light : DDciIcon::Dark;
        int actualSize = dciIcon.actualSize(option->decorationSize.width(), theme, mode);
        // For highdpi icons actualSize might be larger than decorationSize, which we don't want. Clamp it to decorationSize.
        option->decorationSize = QSize(qMin(option->decorationSize.width(), actualSize),
                                       qMin(option->decorationSize.height(), actualSize));
    }

    if (option->viewItemPosition == QStyleOptionViewItem::ViewItemPosition::Invalid) {
        const int rowCount =  index.model()->rowCount();
        if (rowCount == 1) {
            option->viewItemPosition = QStyleOptionViewItem::ViewItemPosition::OnlyOne;
        } else if (index.row() == 0) {
            option->viewItemPosition = QStyleOptionViewItem::ViewItemPosition::Beginning;
        } else if (index.row() == rowCount - 1) {
            option->viewItemPosition = QStyleOptionViewItem::ViewItemPosition::End;
        } else {
            option->viewItemPosition = QStyleOptionViewItem::ViewItemPosition::Middle;
        }
    }

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
    const QListView * lv = qobject_cast<const QListView*>(option->widget);
    if (lv) {
        if (lv->flow() == QListView::LeftToRight) {
            option->rect.adjust(0, 0, 0 - d->itemSpacing, 0);
        } else {
            option->rect.adjust(0, 0, 0, 0 - d->itemSpacing);
        }
        if (lv->window() && lv->window()->isActiveWindow()) {
            option->state |= QStyle::State_Active;
        } else {
            option->state &= (~QStyle::State_Active);
        }
    }

    DPalette::ColorType type = getViewItemColorType(index, Dtk::ViewItemForegroundRole);

    if (type != DPalette::NoType) {
        option->palette.setBrush(QPalette::Text, DPaletteHelper::instance()->palette(option->widget).brush(type));
    } else {
        QPalette::ColorRole role = getViewItemColorRole(index, Dtk::ViewItemForegroundRole);

        if (role != QPalette::NoRole) {
            option->palette.setBrush(QPalette::Text, lv->palette().brush(role));
        }
    }

    type = getViewItemColorType(index, Dtk::ViewItemBackgroundRole);

    if (type != DPalette::NoType) {
        option->backgroundBrush = DPaletteHelper::instance()->palette(option->widget).brush(type);
    } else {
        QPalette::ColorRole role = getViewItemColorRole(index, Dtk::ViewItemBackgroundRole);

        if (role != QPalette::NoRole) {
            auto pa = option->widget ? option->widget->palette() : qApp->palette();
            option->backgroundBrush = pa.brush(role);
        }
    }

    option->font = getViewItemFont(index, Dtk::ViewItemFontLevelRole);
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
    const auto view = qobject_cast<QAbstractItemView*>(parent());
    if (event->type() == QEvent::StyleChange && view) {
        D_D(DStyledItemDelegate);
        do {
            if (d->margins.isNull())
                break;

            int frame_margin = DStyle::pixelMetric(view->style(), DStyle::PM_FrameRadius);
            int left_right_margin = DStyle::pixelMetric(view->style(), DStyle::PM_ContentsMargins);
            d->margins = QMargins(left_right_margin, frame_margin, left_right_margin, frame_margin);

            // relayout to calculate size.
            view->doItemsLayout();
        } while (false);
    }
    if (view && object == view->viewport()) {
        static const QEvent::Type UpdateWidgetVisibleEvent(
                    static_cast<QEvent::Type>(QEvent::registerEventType()));

        if (event->type() == QEvent::Paint) {
            D_D(DStyledItemDelegate);
            const QPaintEvent *pe = static_cast<QPaintEvent *>(event);
            // We only hide widgets when updating all area, it maybe also to be paint when hover,
            // and it's area is a specific part.
            if (pe->rect() == view->viewport()->rect() &&
                d->readyRecordVisibleWidgetOfCurrentFrame()) {
                auto updateEvent = new QEvent(UpdateWidgetVisibleEvent);
                qApp->postEvent(view->viewport(), updateEvent);
            }
        } else if (event->type() == UpdateWidgetVisibleEvent) {
            D_D(DStyledItemDelegate);
            d->updateWidgetVisibleInUnvisualArea();
            return true;
        }
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

/*!
  \class Dtk::Widget::DStandardItem
  \inmodule dtkwidget
  \brief 提供标准项 item,通常用于模型/视图,或模型-代理-视图里面,用以提供标准的 item 控件.

  \sa QStandardItem DViewItemAction
 */

/*!
  \brief 析构函数
 */
DStandardItem::~DStandardItem()
{
}

/*!
  \brief 设置 actionList
  \a edge 选用数值方向的枚举值, 表示设置 list 是在本对象 DStandardItem 的哪一个区域；
                  edge是相对于 item 的内容区域的，内容区域指的是 item 自身的图标和文字所在区域，也就是通过 setIcon和setText设置的内容的显示区域。
  \a list 许多 actiontem 的集合的 list 列表
 */
void DStandardItem::setActionList(Qt::Edge edge, const DViewItemActionList &list)
{
    QVariant value;

    if (!list.isEmpty()) {
        value = actionListToQVariant(list);;
    }

    auto role = getActionPositionRole(edge);
    setData(value, role);
}

/*!
  \brief 获取项 item 的集合列表 list
  \a edge edge是相对于 item 的内容区域的，内容区域指的是 item 自身的图标和文字所在区域，也就是通过 setIcon和setText设置的内容的显示区域。
  \return 返回项 item 的集合列表 list
 */
DViewItemActionList DStandardItem::actionList(Qt::Edge edge) const
{
    return qvariantToActionList(data(getActionPositionRole(edge)));
}

/*!
  \brief 设置项 item 的集合列表 \a list, 只显示有 text 的 DViewItemActionList 集合
   项 item 的集合列表 \a list, 只显示文字的 item 的集合
  \b 一个参考代码片段，使用 setTextActionList():
  
  \code
  QMainWindow *wid = new QMainWindow();
  wid->resize(800, 600);
  
  //视图和模型
  DListView *view = new DListView(wid);
  QStandardItemModel *model = new QStandardItemModel(view);
  view->setModel(model);
  
  //标准DStandardItem item
  DStandardItem *item = new DStandardItem();
  item->setText("@1:这是同一个item 的text\n@2:这是同一个item 的text\n@3:这是同一个item 的text\n@4:这是同一个item 的text\n@5:这是同一个item 的text\n@6:这是同一个item 的text");
  
  //DViewItemAction() 的形参: 其参数 alignment = AlignLeft 或 AlignHCenter 或  AlignRight; 设置为 AlignTop, AlignVCenter, AlignBottom 也是无效的
  DViewItemAction *act1 = new DViewItemAction(Qt::AlignBottom, QSize(15, 15), QSize(50, 50));
  act1->setText("act1");
  act1->setIcon(QIcon("/home/yuanyi/Desktop/dog.jpg"));
  DViewItemAction *act2 = new DViewItemAction(Qt::AlignVCenter, QSize(15, 15), QSize());
  act2->setText("act2");
  act2->setIcon(QIcon("/home/yuanyi/Desktop/dog.jpg"));
  DViewItemAction *act3 = new DViewItemAction(Qt::AlignTop, QSize(15, 15), QSize(100, 50));
  act3->setText("act3");
  act3->setIcon(QIcon("/home/yuanyi/Desktop/dog.jpg"));
  DViewItemAction *act4 = new DViewItemAction(Qt::AlignBottom, QSize(15, 15), QSize(100, 50));
  act4->setText("act4");
  act4->setIcon(QIcon("/home/yuanyi/Desktop/dog.jpg"));
  
  DViewItemActionList *list = new DViewItemActionList();
  list->append(act1);
  list->append(act2);
  list->append(act3);
  list->append(act4);
  
  item->setTextActionList(*list);
  model->appendRow(item);
  
  wid->setCentralWidget(view);
  wid->show();
  \endcode
 */
void DStandardItem::setTextActionList(const DViewItemActionList &list)
{
    QVariant value;

    if (!list.isEmpty()) {
        value = actionListToQVariant(list);;
    }

    setData(value, Dtk::TextActionListRole);
}

/*!
  \brief 返回 "扩充的文本内容列表" 数据
 */
DViewItemActionList DStandardItem::textActionList() const
{
    return qvariantToActionList(data(Dtk::TextActionListRole));
}

void DStandardItem::setTextColorRole(DPalette::ColorType role)
{
    setData(QVariant::fromValue(qMakePair((int)QPalette::NoRole, (int)role)), Dtk::ViewItemForegroundRole);
}

void DStandardItem::setTextColorRole(QPalette::ColorRole role)
{
    setData(QVariant::fromValue(qMakePair((int)role, (int)DPalette::NoType)), Dtk::ViewItemForegroundRole);
}

DPalette::ColorType DStandardItem::textColorType() const
{
    return getViewItemColorType(index(), Dtk::ViewItemForegroundRole);
}

QPalette::ColorRole DStandardItem::textColorRole() const
{
    return getViewItemColorRole(index(), Dtk::ViewItemForegroundRole);
}

void DStandardItem::setBackgroundRole(DPalette::ColorType role)
{
    setData(QVariant::fromValue(qMakePair((int)QPalette::NoRole, (int)role)), Dtk::ViewItemBackgroundRole);
}

void DStandardItem::setBackgroundRole(QPalette::ColorRole role)
{
    setData(QVariant::fromValue(qMakePair((int)role, (int)DPalette::NoType)), Dtk::ViewItemBackgroundRole);
}

DPalette::ColorType DStandardItem::backgroundType() const
{
    return getViewItemColorType(index(), Dtk::ViewItemBackgroundRole);
}

QPalette::ColorRole DStandardItem::backgroundRole() const
{
    return getViewItemColorRole(index(), Dtk::ViewItemBackgroundRole);
}

void DStandardItem::setFontSize(DFontSizeManager::SizeType size)
{
    setData((int)size, Dtk::ViewItemFontLevelRole);
}

QFont DStandardItem::font() const
{
    return getViewItemFont(index(), Dtk::ViewItemFontLevelRole);
}

void DStandardItem::setDciIcon(const DDciIcon &dciIcon)
{
    setData(QVariant::fromValue<DDciIcon>(dciIcon), Qt::DecorationRole);
}

DDciIcon DStandardItem::dciIcon() const
{
    return qvariant_cast<DDciIcon>(data(Qt::DecorationRole));
}

QStandardItem *DStandardItem::clone() const
{
    return new DStandardItem(*this);
}

DWIDGET_END_NAMESPACE
