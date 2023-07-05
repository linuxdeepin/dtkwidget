// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dcombobox.h"
#include "private/dcombobox_p.h"

#ifndef emit
#define emit Q_EMIT
#include <private/qcombobox_p.h>
#undef emit
#else
#include <private/qcombobox_p.h>
#endif

#include <private/qabstractscrollarea_p.h>
#include <private/qguiapplication_p.h>
#include <qpa/qplatformtheme.h>

#include <QListView>
#include <QTableView>
#include <QItemDelegate>
#include <QTreeView>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QDesktopWidget>
#endif
#include <QLayout>
#include <QScrollBar>
#include <QHeaderView>
#include <QScreen>
#include <QStack>
#include <QWindow>

DWIDGET_BEGIN_NAMESPACE

DComboBoxPrivate::DComboBoxPrivate(DComboBox *q)
    : DObjectPrivate(q)
{
}

void DComboBoxPrivate::init()
{
    D_Q(DComboBox);
    q->setMaxVisibleItems(MaxVisibleItems);
}

/*!
 * @~english
    @brief Obtain the screen rectangle of the widget. Use QScreen to access the screen geometry.
 */
QRect DComboBoxPrivate::popupGeometry()
{
    D_Q(DComboBox);
    bool useFullScreenForPopupMenu = false;
    if (const QPlatformTheme *theme = QGuiApplicationPrivate::platformTheme())
        useFullScreenForPopupMenu = theme->themeHint(QPlatformTheme::UseFullScreenForPopupMenu).toBool();
    auto screen = q->window()->windowHandle()->screen();
    return useFullScreenForPopupMenu ? screen->geometry() : screen->availableGeometry();
}

/*!
 * @~english 
    @brief Computes a size hint based on the maximum width for the items in the combobox.
 */
int DComboBoxPrivate::computeWidthHint() const
{
    D_Q(const DComboBox);

    int width = 0;
    const int count = q->count();
    const int iconWidth = q->iconSize().width() + 4;
    const QFontMetrics &fontMetrics = q->fontMetrics();

    for (int i = 0; i < count; ++i) {
#if QT_DEPRECATED_SINCE(5, 11)
        const int textWidth = fontMetrics.horizontalAdvance(q->itemText(i));
#else
        const int textWidth = fontMetrics.horizontalAdvance(q->itemText(i));
#endif
        if (q->itemIcon(i).isNull())
            width = (qMax(width, textWidth));
        else
            width = (qMax(width, textWidth + iconWidth));
    }

    QStyleOptionComboBox opt;
    q->initStyleOption(&opt);
    QSize tmp(width, 0);
    tmp = q->style()->sizeFromContents(QStyle::CT_ComboBox, &opt, tmp, q);
    return tmp.width();
}


/*!
 * @~english @class DComboBox
 * @~english @brief Override QComboBox to create a ComboBox and provide a dropdown list for the user to select from.
 *
 * @~english @note Code example.
 * @~english @code
 * DComboBox *btn1 = new DComboBox("btn1");
 * btn1->addItem("ComboBox");
 * @endcode 
 */
DComboBox::DComboBox(QWidget *parent)
    : QComboBox(parent)
    , DCORE_NAMESPACE::DObject(*new DComboBoxPrivate(this))
{
    D_D(DComboBox);
    d->init();
}

DComboBox::DComboBox(DComboBoxPrivate &dd, QWidget *parent)
    : QComboBox (parent)
    , DCORE_NAMESPACE::DObject(dd, this)
{
    D_D(DComboBox);
    d->init();
}

/*!
 * @~english 
    @brief Override QComboBox::showPopup to limit the maximum display height according to maxVisibleItems(), which has a default value of 16. The maximum display height can be changed using setMaxVisibleItems(), which makes the setMaximumHeight() interface ineffective.
 */
void DComboBox::showPopup()
{
    D_D(DComboBox);

    auto getRowCount = [=]{
        int count = 0;
        QStack<QModelIndex> toCheck;
        toCheck.push(view()->rootIndex());
#if QT_CONFIG(treeview)
        QTreeView *treeView = qobject_cast<QTreeView*>(view());
#endif
        while (!toCheck.isEmpty()) {
            QModelIndex parent = toCheck.pop();
            for (int i = 0, end = model()->rowCount(parent); i < end; ++i) {
                QModelIndex idx = model()->index(i, modelColumn(), parent);
                if (!idx.isValid())
                    continue;
#if QT_CONFIG(treeview)
                if (model()->hasChildren(idx) && treeView && treeView->isExpanded(idx))
                    toCheck.push(idx);
#endif
                ++count;
            }
        }
        return count;
    };
    // When the value of maxVisibleItems() is less than 16, use the default value of qt and return it directly to avoid displaying excess whitespace
    QComboBoxPrivateContainer *container = this->findChild<QComboBoxPrivateContainer *>();
    if (getRowCount() <= maxVisibleItems() || !container)
        return QComboBox::showPopup();

    // Calculate maximum height by maximum item size
    QStyle * const style = this->style();
    QStyleOptionComboBox opt;
    initStyleOption(&opt);
    const bool usePopup = style->styleHint(QStyle::SH_ComboBox_Popup, &opt, this);

    QRect listRect(style->subControlRect(QStyle::CC_ComboBox, &opt,
                                         QStyle::SC_ComboBoxListBoxPopup, this));
    QRect screen = d->popupGeometry();

    bool boundToScreen = !window()->testAttribute(Qt::WA_DontShowOnScreen);

    {
        int listHeight = 0;
        int count = 0;
        QStack<QModelIndex> toCheck;
        toCheck.push(view()->rootIndex());
#if QT_CONFIG(treeview)
        QTreeView *treeView = qobject_cast<QTreeView*>(view());
        if (treeView && treeView->header() && !treeView->header()->isHidden())
            listHeight += treeView->header()->height();
#endif
        while (!toCheck.isEmpty()) {
            QModelIndex parent = toCheck.pop();
            for (int i = 0, end = model()->rowCount(parent); i < end; ++i) {
                QModelIndex idx = model()->index(i, modelColumn(), parent);
                if (!idx.isValid())
                    continue;
                listHeight += view()->visualRect(idx).height();
#if QT_CONFIG(treeview)
                if (model()->hasChildren(idx) && treeView && treeView->isExpanded(idx))
                    toCheck.push(idx);
#endif
                ++count;
                if (count >= maxVisibleItems()) {
                    toCheck.clear();
                    break;
                }
            }
        }
        if (count > 1)
            listHeight += (count - 1) * container->spacing();
        listRect.setHeight(listHeight);
    }

    {
        // add the spacing for the grid on the top and the bottom;
        int heightMargin = container->topMargin()  + container->bottomMargin();

        // add the frame of the container
        const auto& containerMargin = container->contentsMargins();
        heightMargin += containerMargin.top() + containerMargin.bottom();

        //add the frame of the view
        const auto& viewMargin = view()->contentsMargins();
        heightMargin += (viewMargin.top() + static_cast<QAbstractScrollAreaPrivate *>(QObjectPrivate::get(view()))->top
                        + viewMargin.bottom() + static_cast<QAbstractScrollAreaPrivate *>(QObjectPrivate::get(view()))->bottom);

        listRect.setHeight(listRect.height() + heightMargin);
    }

    {
        // Hides or shows the scrollers when we emulate a popupmenu
        if (style->styleHint(QStyle::SH_ComboBox_Popup, &opt, this) &&
                view()->verticalScrollBar()->minimum() < view()->verticalScrollBar()->maximum()) {
            const int margin = style->pixelMetric(QStyle::PM_MenuScrollerHeight);

            bool needTop = view()->verticalScrollBar()->value()
                           > (view()->verticalScrollBar()->minimum() + container->topMargin());
            if (needTop) {
                listRect.adjust(0, 0, 0, margin);
            }

            bool needBottom = view()->verticalScrollBar()->value()
                              < (view()->verticalScrollBar()->maximum() - container->bottomMargin() - container->topMargin());
            if (needBottom) {
                listRect.adjust(0, 0, 0, margin);
            }
        }
    }

    // Add space for margin at top and bottom if the style wants it.
    if (usePopup)
        listRect.setHeight(listRect.height() + style->pixelMetric(QStyle::PM_MenuVMargin, &opt, this) * 2);

    // Make sure the popup is wide enough to display its contents.
    if (usePopup) {
        const int diff = d->computeWidthHint() - width();
        if (diff > 0)
            listRect.setWidth(listRect.width() + diff);
    }

    // make sure the widget fits on screen
    if (boundToScreen && listRect.width() > screen.width())
        listRect.setWidth(screen.width());

    QScrollBar *sb = view()->horizontalScrollBar();
    Qt::ScrollBarPolicy policy = view()->horizontalScrollBarPolicy();
    bool needHorizontalScrollBar = (policy == Qt::ScrollBarAsNeeded || policy == Qt::ScrollBarAlwaysOn)
            && sb->minimum() < sb->maximum();
    if (needHorizontalScrollBar) {
        listRect.adjust(0, 0, 0, sb->height());
    }
    container->setMaximumSize(listRect.size());
    QComboBox::showPopup();
    auto currentIndexTopLeft = view()->mapToGlobal(view()->visualRect(view()->currentIndex()).topLeft());
    int offset = mapToGlobal(rect().topLeft()).y() - currentIndexTopLeft.y();
    int newY = qMax(screen.top(), qMin(container->y() + offset, screen.bottom() - container->height()));
    container->move(container->x(), newY);
}

DWIDGET_END_NAMESPACE
