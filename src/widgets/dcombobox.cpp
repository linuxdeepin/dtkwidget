/*
* Copyright (C) 2021 ~ 2021 Uniontech Software Technology Co.,Ltd.
*
* Author:     Ye ShanShan <yeshanshan@uniontech.com>
*
* Maintainer: Ye ShanShan <yeshanshan@uniontech.com>>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
#include <QDesktopWidget>
#include <QLayout>
#include <QScrollBar>
#include <QHeaderView>
#include <QScreen>
#include <QStack>

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
 * \~chinese \brief 获得控件屏幕矩形位置，QDesktopWidgetPrivate改为QApplication::desktop()访问应用位置。
 */
QRect DComboBoxPrivate::popupGeometry()
{
    D_Q(DComboBox);
    bool useFullScreenForPopupMenu = false;
    if (const QPlatformTheme *theme = QGuiApplicationPrivate::platformTheme())
        useFullScreenForPopupMenu = theme->themeHint(QPlatformTheme::UseFullScreenForPopupMenu).toBool();
    return useFullScreenForPopupMenu ?
                QApplication::desktop()->screenGeometry(q) :
                QApplication::desktop()->availableGeometry(q);
}

/*!
 * \~chinese \brief
 * Computes a size hint based on the maximum width
 * for the items in the combobox.
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
        const int textWidth = fontMetrics.width(q->itemText(i));
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
 * \~chinese \class DComboBox
 * \~chinese \brief DComboBox 重写QComboBox, 提供一个下拉列表供用户选择
 *
 * \~chinese \note\row 代码示例
 * \~chinese \note\row DComboBox *btn1 = new DComboBox("btn1");
 * \~chinese \note\row btn1->addItem("ComboBox");
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
 * \~chinese \brief 重写QComboBox::showPopup, 按照maxVisibleItems()来限制最大显示高度，其值默认为16，
 * 可通过setMaxVisibleItems()来改变最大显示高度，这个功能使得setMaximumHeight()接口无效。
 */
void DComboBox::showPopup()
{
    D_D(DComboBox);

    QComboBox::showPopup();

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
    // 小于 16 的时候使用 qt 默认的，直接返回，避免显示多余的空白
    if (getRowCount() <= maxVisibleItems()) {
        return;
    }
    //获取下拉视图容器
    if (QComboBoxPrivateContainer *container = this->findChild<QComboBoxPrivateContainer *>()) {
        // 重置最大高度

        QStyle * const style = this->style();
        QStyleOptionComboBox opt;
        initStyleOption(&opt);
        const bool usePopup = style->styleHint(QStyle::SH_ComboBox_Popup, &opt, this);

        QRect listRect(style->subControlRect(QStyle::CC_ComboBox, &opt,
                                             QStyle::SC_ComboBoxListBoxPopup, this));
        QRect screen = d->popupGeometry();

        QPoint below = mapToGlobal(listRect.bottomLeft());
        int belowHeight = screen.bottom() - below.y();
        QPoint above = mapToGlobal(listRect.topLeft());
        int aboveHeight = above.y() - screen.y();
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
            int marginTop, marginBottom;
            container->getContentsMargins(0, &marginTop, 0, &marginBottom);
            heightMargin += marginTop + marginBottom;

            //add the frame of the view
            view()->getContentsMargins(0, &marginTop, 0, &marginBottom);
            marginTop += static_cast<QAbstractScrollAreaPrivate *>(QObjectPrivate::get(view()))->top;
            marginBottom += static_cast<QAbstractScrollAreaPrivate *>(QObjectPrivate::get(view()))->bottom;
            heightMargin += marginTop + marginBottom;

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

        //takes account of the minimum/maximum size of the container
//        listRect.setSize( listRect.size().expandedTo(container->minimumSize())
//                          .boundedTo(container->maximumSize()));

        // make sure the widget fits on screen
        if (boundToScreen) {
            if (listRect.width() > screen.width() )
                listRect.setWidth(screen.width());
            if (mapToGlobal(listRect.bottomRight()).x() > screen.right()) {
                below.setX(screen.x() + screen.width() - listRect.width());
                above.setX(screen.x() + screen.width() - listRect.width());
            }
            if (mapToGlobal(listRect.topLeft()).x() < screen.x() ) {
                below.setX(screen.x());
                above.setX(screen.x());
            }
        }

        QScrollBar *sb = view()->horizontalScrollBar();
        Qt::ScrollBarPolicy policy = view()->horizontalScrollBarPolicy();
        bool needHorizontalScrollBar = (policy == Qt::ScrollBarAsNeeded || policy == Qt::ScrollBarAlwaysOn)
                && sb->minimum() < sb->maximum();
        if (needHorizontalScrollBar) {
            listRect.adjust(0, 0, 0, sb->height());
        }

        // 提前修改container高度，否则 view()->visualRect(view()->currentIndex())的位置计算错误
        container->setMaximumHeight(listRect.height());

        if (usePopup) {
            // Position horizontally.
            listRect.moveLeft(above.x());

            // Position vertically so the curently selected item lines up
            // with the combo box.
            const QRect currentItemRect = view()->visualRect(view()->currentIndex());
            const int offset = listRect.top() - currentItemRect.top();
            listRect.moveTop(above.y() + offset - listRect.top());

            // Clamp the listRect height and vertical position so we don't expand outside the
            // available screen geometry.This may override the vertical position, but it is more
            // important to show as much as possible of the popup.
            const int height = !boundToScreen ? listRect.height() : qMin(listRect.height(), screen.height());
            listRect.setHeight(height);

            if (boundToScreen) {
                if (listRect.top() < screen.top())
                    listRect.moveTop(screen.top());
                if (listRect.bottom() > screen.bottom())
                    listRect.moveBottom(screen.bottom());
            }
        } else if (!boundToScreen || listRect.height() <= belowHeight) {
            listRect.moveTopLeft(below);
        } else if (listRect.height() <= aboveHeight) {
            listRect.moveBottomLeft(above);
        } else if (belowHeight >= aboveHeight) {
            listRect.setHeight(belowHeight);
            listRect.moveTopLeft(below);
        } else {
            listRect.setHeight(aboveHeight);
            listRect.moveBottomLeft(above);
        }

        container->setGeometry(listRect);
    }

}

DWIDGET_END_NAMESPACE
