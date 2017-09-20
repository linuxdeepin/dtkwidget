/* -*- Mode: C++; indent-tabs-mode: nil; tab-width: 4 -*-
 * -*- coding: utf-8 -*-
 *
 * Copyright (C) 2011 ~ 2017 Deepin, Inc.
 *               2011 ~ 2017 Wang Yong
 *
 * Author:     Wang Yong <wangyong@deepin.com>
 * Maintainer: Wang Yong <wangyong@deepin.com>
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

#include "dsimplelistview.h"
#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QMenu>
#include <QStyleFactory>
#include <QWheelEvent>
#include <QtMath>

DWIDGET_BEGIN_NAMESPACE

DSimpleListView::DSimpleListView(QWidget *parent) : QWidget(parent)
{
    // Init.
    installEventFilter(this);   // add event filter
    setMouseTracking(true);    // make MouseMove can response

    scrollDistance = 0;
    renderOffset = 0;
    titleHeight = 0;
    titleArrowPadding = 4;
    titlePadding = 14;

    searchContent = "";
    searchAlgorithm = NULL;

    titleHoverColumn = -1;
    titlePressColumn = -1;

    arrowUpDarkNormalImage = QImage(":/images/dark/images/arrow_up_normal.png");
    arrowUpDarkHoverImage = QImage(":/images/dark/images/arrow_up_hover.png");
    arrowUpDarkPressImage = QImage(":/images/dark/images/arrow_up_press.png");
    arrowDownDarkNormalImage = QImage(":/images/dark/images/arrow_down_normal.png");
    arrowDownDarkHoverImage = QImage(":/images/dark/images/arrow_down_hover.png");
    arrowDownDarkPressImage = QImage(":/images/dark/images/arrow_down_press.png");

    arrowUpLightNormalImage = QImage(":/images/light/images/arrow_up_normal.png");
    arrowUpLightHoverImage = QImage(":/images/light/images/arrow_up_hover.png");
    arrowUpLightPressImage = QImage(":/images/light/images/arrow_up_press.png");
    arrowDownLightNormalImage = QImage(":/images/light/images/arrow_down_normal.png");
    arrowDownLightHoverImage = QImage(":/images/light/images/arrow_down_hover.png");
    arrowDownLightPressImage = QImage(":/images/light/images/arrow_down_press.png");

    arrowUpNormalImage = arrowUpLightNormalImage;
    arrowUpHoverImage = arrowUpLightHoverImage;
    arrowUpPressImage = arrowUpLightPressImage;
    arrowDownNormalImage = arrowDownLightNormalImage;
    arrowDownHoverImage = arrowDownLightHoverImage;
    arrowDownPressImage = arrowDownLightPressImage;

    listItems = new QList<DSimpleListItem*>();
    renderItems = new QList<DSimpleListItem*>();
    selectionItems = new QList<DSimpleListItem*>();
    lastSelectItem = NULL;
    lastHoverItem = NULL;
    lastHoverColumnIndex = -1;

    mouseAtScrollArea = false;
    mouseDragScrollbar = false;
    scrollbarDefaultWidth = 4;
    scrollbarDragWidth = 8;
    scrollbarMinHeight = 30;
    scrollbarPadding = 4;
    hideScrollbarDuration = 2000;

    oldRenderOffset = 0;
    clipRadius = 8;

    hideScrollbarTimer = NULL;

    sortingAlgorithms = new QList<SortAlgorithm>();
    sortingOrderes = new QList<bool>();
}

DSimpleListView::~DSimpleListView()
{
    delete lastHoverItem;
    delete lastSelectItem;
    delete listItems;
    delete renderItems;
    delete selectionItems;
    delete sortingAlgorithms;
    delete sortingOrderes;
    delete hideScrollbarTimer;
}

void DSimpleListView::setRowHeight(int height)
{
    rowHeight = height;
    scrollUnit = rowHeight;
}

void DSimpleListView::setColumnTitleInfo(QList<QString> titles, QList<int> widths, int height)
{
    // Set column titles.
    columnTitles = titles;

    // Calcuate column title widths.
    columnWidths.clear();
    QFont font;
    font.setPointSize(titleSize);
    QFontMetrics fm(font);

    for (int i = 0; i < widths.length(); i++) {
        if (widths[i] == -1) {
            columnWidths << widths[i];
        } else {
            int renderTitleWidth = fm.width(titles[i]) + titlePadding + arrowUpNormalImage.width() + titleArrowPadding * 2;
            columnWidths << std::max(widths[i], renderTitleWidth);
        }
    }

    // Set title height.
    titleHeight = height;
}

void DSimpleListView::setColumnHideFlags(QList<bool> toggleHideFlags, int visibleColumnIndex)
{
    Q_ASSERT_X(toggleHideFlags.contains(false), "toggleHideFlags", "at least have one 'false' in list.");
    Q_ASSERT_X(toggleHideFlags.count() == columnTitles.count(), "toggleHideFlags", "hide flags length is not same as titles list.");

    alwaysVisibleColumn = visibleColumnIndex;

    columnVisibles.clear();
    for (int i = 0; i < toggleHideFlags.count(); i++) {
        columnVisibles.append(toggleHideFlags[i]);
    }
}

void DSimpleListView::setColumnSortingAlgorithms(QList<SortAlgorithm> *algorithms, int sortColumn, bool descendingSort)
{
    // Add sort algorithms.
    sortingAlgorithms = algorithms;

    for (int i = 0; i < sortingAlgorithms->count(); i++) {
        sortingOrderes->append(false);
    }

    // If sort column is -1, don't sort default.
    defaultSortingColumn = sortColumn;
    defaultSortingOrder = descendingSort;
}

void DSimpleListView::setSearchAlgorithm(SearchAlgorithm algorithm)
{
    searchAlgorithm = algorithm;
}

void DSimpleListView::setClipRadius(int radius)
{
    clipRadius = radius;
}


void DSimpleListView::addItems(QList<DSimpleListItem*> items)
{
    // Add item to list.
    listItems->append(items);
    QList<DSimpleListItem*> searchItems = getSearchItems(items);
    renderItems->append(searchItems);

    // If user has click title to sort, sort items after add items to list.
    if (defaultSortingColumn != -1) {
        sortItemsByColumn(defaultSortingColumn, defaultSortingOrder);
    }
}

void DSimpleListView::clearItems()
{
    // NOTE:
    // We need delete items in QList before clear QList to avoid *MEMORY LEAK* .
    qDeleteAll(listItems->begin(), listItems->end());
    listItems->clear();
    renderItems->clear();
}

void DSimpleListView::addSelections(QList<DSimpleListItem*> items, bool recordLastSelection)
{
    // Add item to selection list.
    selectionItems->append(items);

    // Record last selection item to make selected operation continuously.
    if (recordLastSelection && selectionItems->count() > 0) {
        lastSelectItem = selectionItems->last();
    }
}

void DSimpleListView::clearSelections(bool clearLastSelection)
{
    // Clear selection list.
    selectionItems->clear();

    if (clearLastSelection) {
        lastSelectItem = NULL;
    }
}

void DSimpleListView::refreshItems(QList<DSimpleListItem*> items)
{
    // Init.
    QList<DSimpleListItem*> *newSelectionItems = new QList<DSimpleListItem*>();
    DSimpleListItem *newLastSelectionItem = NULL;
    DSimpleListItem *newLastHoverItem = NULL;

    // Save selection items and last selection item.
    for (DSimpleListItem *item:items) {
        for (DSimpleListItem *selectionItem:*selectionItems) {
            if (item->sameAs(selectionItem)) {
                newSelectionItems->append(item);
                break;
            }
        }
    }

    if (lastSelectItem != NULL) {
        for (DSimpleListItem *item:items) {
            if (item->sameAs(lastSelectItem)) {
                newLastSelectionItem = item;
                break;
            }
        }
    }

    if (lastHoverItem != NULL) {
        for (DSimpleListItem *item:items) {
            if (item->sameAs(lastHoverItem)) {
                newLastHoverItem = item;
                break;
            }
        }
    }
    lastHoverItem = NULL;

    // Update items.
    clearItems();
    listItems->append(items);
    QList<DSimpleListItem*> searchItems = getSearchItems(items);
    renderItems->append(searchItems);

    // Sort once if default sort column hasn't init.
    if (defaultSortingColumn != -1) {
        sortItemsByColumn(defaultSortingColumn, defaultSortingOrder);
    }

    // Restore selection items and last selection item.
    clearSelections();
    addSelections(*newSelectionItems, false);
    lastSelectItem = newLastSelectionItem;
    lastHoverItem = newLastHoverItem;

    // Keep scroll position.
    renderOffset = adjustRenderOffset(renderOffset);

    // Render.
    repaint();
}

void DSimpleListView::search(QString content)
{
    if (content == "" && searchContent != content) {
        searchContent = content;

        renderItems->clear();
        renderItems->append(*listItems);
    } else {
        searchContent = content;

        QList<DSimpleListItem*> searchItems = getSearchItems(*listItems);
        renderItems->clear();
        renderItems->append(searchItems);
    }

    repaint();
}

void DSimpleListView::selectAllItems()
{
    // Record old render offset to control scrollbar whether display.
    oldRenderOffset = renderOffset;

    // Select all items.
    clearSelections();
    addSelections(*renderItems);

    // Scroll to top.
    renderOffset = getTopRenderOffset();

    // Repaint.
    repaint();
}

void DSimpleListView::selectFirstItem()
{
    // Record old render offset to control scrollbar whether display.
    oldRenderOffset = renderOffset;

    // Select first item.
    clearSelections();

    QList<DSimpleListItem*> items = QList<DSimpleListItem*>();
    items << renderItems->first();
    addSelections(items);

    // Scroll to top.
    renderOffset = getTopRenderOffset();

    // Repaint.
    repaint();
}

void DSimpleListView::selectLastItem()
{
    // Record old render offset to control scrollbar whether display.
    oldRenderOffset = renderOffset;

    // Select last item.
    clearSelections();

    QList<DSimpleListItem*> items = QList<DSimpleListItem*>();
    items << renderItems->last();
    addSelections(items);

    // Scroll to bottom.
    renderOffset = getBottomRenderOffset();

    // Repaint.
    repaint();
}

void DSimpleListView::selectPrevItem()
{
    selectPrevItemWithOffset(1);
}

void DSimpleListView::selectNextItem()
{
    selectNextItemWithOffset(1);
}

void DSimpleListView::shiftSelectPageDown()
{
    shiftSelectNextItemWithOffset(getScrollAreaHeight() / rowHeight);
}

void DSimpleListView::shiftSelectPageUp()
{
    shiftSelectPrevItemWithOffset(getScrollAreaHeight() / rowHeight);
}

void DSimpleListView::shiftSelectToEnd()
{
    // Select last item if nothing selected yet.
    if (selectionItems->empty()) {
        selectLastItem();
    }
    // Select items from last selected item to last item.
    else {
        // Found last selected index and do select operation.
        int lastSelectionIndex = renderItems->indexOf(lastSelectItem);
        shiftSelectItemsWithBound(lastSelectionIndex, renderItems->count() - 1);

        // Scroll to bottom.
        renderOffset = getBottomRenderOffset();

        // Repaint.
        repaint();
    }
}

void DSimpleListView::shiftSelectToHome()
{
    // Select first item if nothing selected yet.
    if (selectionItems->empty()) {
        selectFirstItem();
    }
    // Select items from last selected item to first item.
    else {
        // Found last selected index and do select operation.
        int lastSelectionIndex = renderItems->indexOf(lastSelectItem);
        shiftSelectItemsWithBound(0, lastSelectionIndex);

        // Scroll to top.
        renderOffset = getTopRenderOffset();

        // Repaint.
        repaint();
    }
}

void DSimpleListView::shiftSelectToNext()
{
    shiftSelectNextItemWithOffset(1);
}

void DSimpleListView::shiftSelectToPrev()
{
    shiftSelectPrevItemWithOffset(1);
}

void DSimpleListView::scrollPageDown()
{
    selectNextItemWithOffset(getScrollAreaHeight() / rowHeight);
}

void DSimpleListView::scrollPageUp()
{
    selectPrevItemWithOffset(getScrollAreaHeight() / rowHeight);
}

void DSimpleListView::ctrlScrollPageUp()
{
    renderOffset = adjustRenderOffset(renderOffset - getScrollAreaHeight());

    repaint();
}

void DSimpleListView::ctrlScrollPageDown()
{
    renderOffset = adjustRenderOffset(renderOffset + getScrollAreaHeight());

    repaint();
}

void DSimpleListView::ctrlScrollToHome()
{
    renderOffset = getTopRenderOffset();

    repaint();
}

void DSimpleListView::ctrlScrollToEnd()
{
    renderOffset = getBottomRenderOffset();

    repaint();
}

void DSimpleListView::leaveEvent(QEvent * event){
    hideScrollbar();

    QWidget::leaveEvent(event);
}

void DSimpleListView::hideScrollbar()
{
    // Record old render offset to control scrollbar whether display.
    mouseAtScrollArea = false;
    oldRenderOffset = renderOffset;

    repaint();
}

bool DSimpleListView::eventFilter(QObject *, QEvent *)
{
    return false;
}

void DSimpleListView::keyPressEvent(QKeyEvent *keyEvent)
{
    if (keyEvent->key() == Qt::Key_Home) {
        if (keyEvent->modifiers() == Qt::ControlModifier) {
            ctrlScrollToHome();
        } else if (keyEvent->modifiers() == Qt::ShiftModifier) {
            shiftSelectToHome();
        }else {
            selectFirstItem();
        }
    } else if (keyEvent->key() == Qt::Key_End) {
        if (keyEvent->modifiers() == Qt::ControlModifier) {
            ctrlScrollToEnd();
        } else if (keyEvent->modifiers() == Qt::ShiftModifier) {
            shiftSelectToEnd();
        } else {
            selectLastItem();
        }
    } else if (keyEvent->key() == Qt::Key_Up) {
        if (keyEvent->modifiers() == Qt::ShiftModifier) {
            shiftSelectToPrev();
        } else {
            selectPrevItem();
        }
    } else if (keyEvent->key() == Qt::Key_Down) {
        if (keyEvent->modifiers() == Qt::ShiftModifier) {
            shiftSelectToNext();
        } else {
            selectNextItem();
        }
    } else if (keyEvent->key() == Qt::Key_PageUp) {
        if (keyEvent->modifiers() == Qt::ControlModifier) {
            ctrlScrollPageUp();
        } else if (keyEvent->modifiers() == Qt::ShiftModifier) {
            shiftSelectPageUp();
        } else {
            scrollPageUp();
        }
    } else if (keyEvent->key() == Qt::Key_PageDown) {
        if (keyEvent->modifiers() == Qt::ControlModifier) {
            ctrlScrollPageDown();
        } else if (keyEvent->modifiers() == Qt::ShiftModifier) {
            shiftSelectPageDown();
        } else {
            scrollPageDown();
        }
    } else if (keyEvent->key() == Qt::Key_A) {
        if (keyEvent->modifiers() == Qt::ControlModifier) {
            selectAllItems();
        }
    }
}

void DSimpleListView::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    // Scroll if mouse drag at scrollbar.
    if (mouseDragScrollbar) {
        int barHeight = getScrollbarHeight();
        renderOffset = adjustRenderOffset((mouseEvent->y() - barHeight / 2 - titleHeight) / (getScrollAreaHeight() * 1.0) * getItemsTotalHeight());

        repaint();
    }
    // Update scrollbar status with mouse position.
    else if (isMouseAtScrollArea(mouseEvent->x()) != mouseAtScrollArea) {
        mouseAtScrollArea = isMouseAtScrollArea(mouseEvent->x());
        repaint();
    }
    // Otherwise to check titlebar arrow status.
    else {
        bool atTitleArea = isMouseAtTitleArea(mouseEvent->y());

        if (atTitleArea) {
            int hoverColumn = -1;

            if (sortingAlgorithms->count() != 0 && sortingAlgorithms->count() == columnTitles.count() && sortingOrderes->count() == columnTitles.count()) {
                // Calcuate title widths;
                QList<int> renderWidths = getRenderWidths();

                int columnCounter = 0;
                int columnRenderX = 0;
                for (int renderWidth:renderWidths) {
                    if (renderWidth > 0) {
                        if (mouseEvent->x() > columnRenderX && mouseEvent->x() < columnRenderX + renderWidth) {
                            hoverColumn = columnCounter;

                            break;
                        }

                        columnRenderX += renderWidth;
                    }

                    columnCounter++;
                }
            }

            if (hoverColumn != titleHoverColumn) {
                titleHoverColumn = hoverColumn;

                repaint();
            }
        } else {
            int hoverItemIndex = (renderOffset + mouseEvent->y() - titleHeight) / rowHeight;

            // NOTE: hoverItemIndex may be less than 0, we need check index here.
            if (hoverItemIndex >= 0 && hoverItemIndex < (*renderItems).length()) {
                DSimpleListItem *item = (*renderItems)[hoverItemIndex];

                QList<int> renderWidths = getRenderWidths();

                int columnCounter = 0;
                int columnRenderX = 0;
                for (int renderWidth:renderWidths) {
                    if (renderWidth > 0) {
                        if (mouseEvent->x() > columnRenderX && mouseEvent->x() < columnRenderX + renderWidth) {
                            break;
                        }

                        columnRenderX += renderWidth;
                    }

                    columnCounter++;
                }

                if (lastHoverItem == NULL || !item->sameAs(lastHoverItem) || columnCounter != lastHoverColumnIndex) {
                    lastHoverItem = item;
                    lastHoverColumnIndex = columnCounter;

                    changeHoverItem(this->mapToGlobal(mouseEvent->pos()), lastHoverItem, columnCounter);
                }
            }
        }
    }
}

void DSimpleListView::mousePressEvent(QMouseEvent *mouseEvent)
{
    setFocus();

    bool atTitleArea = isMouseAtTitleArea(mouseEvent->y());
    bool atScrollArea = isMouseAtScrollArea(mouseEvent->x());

    // Sort items with column's sorting algorithms when click on title area.
    if (atTitleArea) {
        if (mouseEvent->button() == Qt::LeftButton) {
            if (sortingAlgorithms->count() != 0 && sortingAlgorithms->count() == columnTitles.count() && sortingOrderes->count() == columnTitles.count()) {
                // Calcuate title widths;
                QList<int> renderWidths = getRenderWidths();

                int columnCounter = 0;
                int columnRenderX = 0;
                for (int renderWidth:renderWidths) {
                    if (renderWidth > 0) {
                        if (mouseEvent->x() > columnRenderX && mouseEvent->x() < columnRenderX + renderWidth) {
                            // If switch other column, default order is from top to bottom.
                            if (columnCounter != defaultSortingColumn) {
                                (*sortingOrderes)[columnCounter] = true;
                            }
                            // If user click same column, just switch reverse order.
                            else {
                                (*sortingOrderes)[columnCounter] = !(*sortingOrderes)[columnCounter];
                            }

                            defaultSortingColumn = columnCounter;
                            defaultSortingOrder = (*sortingOrderes)[columnCounter];

                            changeSortingStatus(defaultSortingColumn, defaultSortingOrder);

                            sortItemsByColumn(columnCounter, (*sortingOrderes)[columnCounter]);

                            if (columnCounter != titlePressColumn) {
                                titlePressColumn = columnCounter;
                            }

                            repaint();
                            break;
                        }

                        columnRenderX += renderWidth;
                    }

                    columnCounter++;
                }
            }
        } else if (mouseEvent->button() == Qt::RightButton) {
            if (columnVisibles.count() == columnTitles.count()) {
                QMenu *menu = new QMenu();
                menu->setStyle(QStyleFactory::create("dlight"));

                for (int i = 0; i < columnVisibles.count(); i++) {
                    if (i != alwaysVisibleColumn) {
                        QAction *action = new QAction(menu);
                        action->setText(columnTitles[i]);

                        action->setCheckable(true);
                        action->setChecked(columnVisibles[i]);

                        connect(action, &QAction::triggered, this, [this, action, i] {
                                columnVisibles[i] = !columnVisibles[i];

                                changeColumnVisible(i, columnVisibles[i], columnVisibles);

                                repaint();
                            });

                        menu->addAction(action);
                    }
                }

                menu->exec(this->mapToGlobal(mouseEvent->pos()));
            }
        }
    }
    // Scroll when click on scrollbar area.
    else if (atScrollArea) {
        int barHeight = getScrollbarHeight();
        int barY = getScrollbarY();

        // Flag mouseDragScrollbar when click on scrollbar.
        if (mouseEvent->y() > barY && mouseEvent->y() < barY + barHeight) {
            mouseDragScrollbar = true;
        }
        // Scroll if click out of scrollbar area.
        else {
            renderOffset = adjustRenderOffset((mouseEvent->y() - barHeight / 2 - titleHeight) / (getScrollAreaHeight() * 1.0) * getItemsTotalHeight());
            repaint();
        }
    }
    // Select items.
    else {
        int pressItemIndex = (renderOffset + mouseEvent->y() - titleHeight) / rowHeight;

        if (pressItemIndex >= renderItems->count()) {
            clearSelections();

            repaint();
        } else {
            if (mouseEvent->button() == Qt::LeftButton) {
                if (pressItemIndex < renderItems->count()) {
                    // Scattered selection of items when press ctrl modifier.
                    if (mouseEvent->modifiers() == Qt::ControlModifier) {
                        DSimpleListItem *item = (*renderItems)[pressItemIndex];

                        if (selectionItems->contains(item)) {
                            selectionItems->removeOne(item);
                        } else {
                            QList<DSimpleListItem*> items = QList<DSimpleListItem*>();
                            items << item;
                            addSelections(items);
                        }
                    }
                    // Continuous selection of items when press shift modifier.
                    else if ((mouseEvent->modifiers() == Qt::ShiftModifier) && !selectionItems->empty()) {
                        int lastSelectionIndex = renderItems->indexOf(lastSelectItem);
                        int selectionStartIndex = std::min(pressItemIndex, lastSelectionIndex);
                        int selectionEndIndex = std::max(pressItemIndex, lastSelectionIndex);

                        shiftSelectItemsWithBound(selectionStartIndex, selectionEndIndex);
                    }
                    // Just select item under mouse if user not press any modifier.
                    else {
                        clearSelections();

                        QList<DSimpleListItem*> items = QList<DSimpleListItem*>();
                        items << (*renderItems)[pressItemIndex];
                        addSelections(items);
                    }

                    repaint();
                }
            } else if (mouseEvent->button() == Qt::RightButton) {
                DSimpleListItem *pressItem = (*renderItems)[pressItemIndex];
                bool pressInSelectionArea = false;

                for (DSimpleListItem *item : *selectionItems) {
                    if (item == pressItem) {
                        pressInSelectionArea = true;

                        break;
                    }
                }

                if (!pressInSelectionArea && pressItemIndex < renderItems->length()) {
                    clearSelections();

                    QList<DSimpleListItem*> items = QList<DSimpleListItem*>();
                    items << (*renderItems)[pressItemIndex];
                    addSelections(items);

                    repaint();
                }

                if (selectionItems->length() > 0) {
                    rightClickItems(this->mapToGlobal(mouseEvent->pos()), *selectionItems);
                }
            }
        }
    }
}

void DSimpleListView::mouseReleaseEvent(QMouseEvent *)
{
    if (mouseDragScrollbar) {
        // Reset mouseDragScrollbar.
        mouseDragScrollbar = false;

        repaint();
    } else {
        if (titlePressColumn != -1) {
            titlePressColumn = -1;
            repaint();
        }
    }
}

void DSimpleListView::wheelEvent(QWheelEvent *event)
{
    if (event->orientation() == Qt::Vertical) {
        // Record old render offset to control scrollbar whether display.
        oldRenderOffset = renderOffset;

        qreal scrollStep = event->angleDelta().y() / 120.0;
        renderOffset = adjustRenderOffset(renderOffset - scrollStep * scrollUnit);

        repaint();
    }

    event->accept();
}

void DSimpleListView::paintEvent(QPaintEvent *)
{
    // Init.
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Calcuate title widths;
    QList<int> renderWidths = getRenderWidths();

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setOpacity(0.05);

    int penWidth = 1;
    QPainterPath framePath;
    framePath.addRoundedRect(QRect(rect().x() + penWidth, rect().y() + penWidth, rect().width() - penWidth * 2, rect().height() - penWidth * 2), clipRadius, clipRadius);
    painter.setClipPath(framePath);

    // Draw title.
    QPainterPath titlePath;
    titlePath.addRect(QRectF(rect().x(), rect().y(), rect().width(), titleHeight));
    painter.setOpacity(titleAreaOpacity);
    painter.fillPath(titlePath, QColor(titleAreaColor));

    int renderY = 0;
    int renderHeight = 0;
    if (titleHeight > 0) {
        int columnCounter = 0;
        int columnRenderX = 0;
        for (int renderWidth:renderWidths) {
            if (renderWidth > 0) {
                painter.setOpacity(1);

                QFont font = painter.font() ;
                font.setPointSize(titleSize);
                painter.setFont(font);

                painter.setPen(QPen(QColor(titleColor)));
                painter.drawText(QRect(columnRenderX + titlePadding, 0, renderWidth, titleHeight), Qt::AlignVCenter | Qt::AlignLeft, columnTitles[columnCounter]);

                columnRenderX += renderWidth;

                if (columnCounter < renderWidths.size() - 1) {
                    painter.setOpacity(0.05);
                    QPainterPath separatorPath;
                    separatorPath.addRect(QRectF(rect().x() + columnRenderX - 1, rect().y() + 4, 1, titleHeight - 8));
                    painter.fillPath(separatorPath, QColor(titleLineColor));
                }

                // Draw sort arrow.
                if (defaultSortingColumn == columnCounter) {
                    painter.setOpacity(1);
                    int arrowX = rect().x() + columnRenderX - titleArrowPadding - arrowUpNormalImage.width();
                    int arrowY = rect().y() + (titleHeight - arrowDownNormalImage.height()) / 2;

                    if (defaultSortingOrder) {
                        if (titlePressColumn == defaultSortingColumn) {
                            painter.drawImage(QPoint(arrowX, arrowY), arrowDownPressImage);
                        } else if (titleHoverColumn == defaultSortingColumn) {
                            painter.drawImage(QPoint(arrowX, arrowY), arrowDownHoverImage);
                        } else {
                            painter.drawImage(QPoint(arrowX, arrowY), arrowDownNormalImage);
                        }
                    } else {
                        if (titlePressColumn == defaultSortingColumn) {
                            painter.drawImage(QPoint(arrowX, arrowY), arrowUpPressImage);
                        } else if (titleHoverColumn == defaultSortingColumn) {
                            painter.drawImage(QPoint(arrowX, arrowY), arrowUpHoverImage);
                        } else {
                            painter.drawImage(QPoint(arrowX, arrowY), arrowUpNormalImage);
                        }
                    }
                }
            }
            columnCounter++;
        }

        renderY += titleHeight;
        renderHeight += titleHeight;
    }

    // Draw background.
    painter.setOpacity(backgroundOpacity);
    QPainterPath backgroundPath;
    backgroundPath.addRect(QRectF(rect().x(), rect().y() + titleHeight, rect().width(), rect().height() - titleHeight));
    painter.fillPath(backgroundPath, QColor(backgroundColor));

    // Draw context.
    QPainterPath scrollAreaPath;
    scrollAreaPath.addRect(QRectF(rect().x(), rect().y() + titleHeight, rect().width(), getScrollAreaHeight()));

    int rowCounter = 0;
    for (DSimpleListItem *item:*renderItems) {
        if (rowCounter > ((renderOffset - rowHeight) / rowHeight)) {
            // Clip item rect.
            QPainterPath itemPath;
            itemPath.addRect(QRect(0, renderY + rowCounter * rowHeight - renderOffset, rect().width(), rowHeight));
            painter.setClipPath((framePath.intersected(scrollAreaPath)).intersected(itemPath));

            // Draw item backround.
            bool isSelect = selectionItems->contains(item);
            painter.save();
            item->drawBackground(QRect(0, renderY + rowCounter * rowHeight - renderOffset, rect().width(), rowHeight), &painter, rowCounter, isSelect);
            painter.restore();

            // Draw item foreground.
            int columnCounter = 0;
            int columnRenderX = 0;
            for (int renderWidth:renderWidths) {
                if (renderWidth > 0) {
                    painter.save();
                    item->drawForeground(QRect(columnRenderX, renderY + rowCounter * rowHeight - renderOffset, renderWidth, rowHeight), &painter, columnCounter, rowCounter, isSelect);
                    painter.restore();

                    columnRenderX += renderWidth;
                }
                columnCounter++;
            }

            renderHeight += rowHeight;

            if (renderHeight > rect().height()) {
                break;
            }
        }

        rowCounter++;
    }

    // Keep clip area.
    painter.setClipPath(framePath);

    // Draw search tooltip.
    if (searchContent != "" && renderItems->size() == 0) {
        painter.setOpacity(1);
        painter.setPen(QPen(QColor(searchColor)));

        QFont font = painter.font() ;
        font.setPointSize(20);
        painter.setFont(font);

        painter.drawText(QRect(rect().x(), rect().y() + titleHeight, rect().width(), rect().height() - titleHeight), Qt::AlignCenter, tr("No search result"));
    }

    // Draw frame.
    QPen framePen;
    framePen.setColor(frameColor);

    painter.setOpacity(frameOpacity);
    painter.drawPath(framePath);

    // Draw scrollbar.
    if (mouseAtScrollArea) {
        paintScrollbar(&painter);
    } else if (oldRenderOffset != renderOffset) {
        paintScrollbar(&painter);

        startScrollbarHideTimer();
    }
}

void DSimpleListView::paintScrollbar(QPainter *painter)
{
    if (getItemsTotalHeight() > getScrollAreaHeight()) {
        // Init scrollbar opacity with scrollbar status.
        qreal barOpacitry = 0;
        qreal barFrameOpacitry = 0;

        // Press.
        if (mouseDragScrollbar) {
            barOpacitry = scrollbarPressOpacity;
            barFrameOpacitry = scrollbarFramePressOpacity;
        } else {
            // Hover.
            if (mouseAtScrollArea) {
                barOpacitry = scrollbarHoverOpacity;
                barFrameOpacitry = scrollbarFrameHoverOpacity;
            }
            // Normal.
            else {
                barOpacitry = scrollbarNormalOpacity;
                barFrameOpacitry = scrollbarFrameNormalOpacity;
            }
        }

        int barWidth = (mouseAtScrollArea || mouseDragScrollbar) ? scrollbarDragWidth : scrollbarDefaultWidth;
        int barRadius = 4;

        int barY = getScrollbarY();
        int barHeight = getScrollbarHeight();

        painter->setOpacity(barOpacitry);
        QPainterPath path;
        path.addRoundedRect(
            QRectF(rect().x() + rect().width() - barWidth - scrollbarPadding,
                   barY + barRadius,
                   barWidth,
                   barHeight - barRadius * 2), barRadius, barRadius);
        painter->fillPath(path, QColor(scrollbarColor));

        QPen pen;
        pen.setColor(QColor(scrollbarColor));
        pen.setWidth(1);
        painter->setOpacity(barFrameOpacitry);
        painter->setPen(pen);
        painter->drawPath(path);
    }
}

void DSimpleListView::selectNextItemWithOffset(int scrollOffset)
{
    // Record old render offset to control scrollbar whether display.
    oldRenderOffset = renderOffset;

    if (selectionItems->empty()) {
        selectFirstItem();
    } else {
        int lastIndex = 0;
        for (DSimpleListItem *item:*selectionItems) {
            int index = renderItems->indexOf(item);
            if (index > lastIndex) {
                lastIndex = index;
            }
        }

        if (lastIndex != -1) {
            lastIndex = std::min(renderItems->count() - 1, lastIndex + scrollOffset);

            clearSelections(false);

            QList<DSimpleListItem*> items = QList<DSimpleListItem*>();
            items << (*renderItems)[lastIndex];

            addSelections(items);

            int itemIndex = lastIndex + 1;
            int itemOffset = adjustRenderOffset(itemIndex * rowHeight - rect().height() + titleHeight);
            if (((renderOffset + getScrollAreaHeight()) / rowHeight) < itemIndex) {
                renderOffset = itemOffset;
            }

            repaint();
        }
    }
}

void DSimpleListView::selectPrevItemWithOffset(int scrollOffset)
{
    // Record old render offset to control scrollbar whether display.
    oldRenderOffset = renderOffset;

    if (selectionItems->empty()) {
        selectFirstItem();
    } else {
        int firstIndex = renderItems->count();
        for (DSimpleListItem *item:*selectionItems) {
            int index = renderItems->indexOf(item);
            if (index < firstIndex) {
                firstIndex = index;
            }
        }

        if (firstIndex != -1) {
            firstIndex = std::max(0, firstIndex - scrollOffset);

            clearSelections();

            QList<DSimpleListItem*> items = QList<DSimpleListItem*>();
            items << (*renderItems)[firstIndex];

            addSelections(items);

            int itemIndex = firstIndex - 1;
            int itemOffset = adjustRenderOffset(itemIndex * rowHeight + titleHeight);
            if ((renderOffset / rowHeight) > itemIndex) {
                renderOffset = itemOffset;
            }

            repaint();
        }
    }
}

void DSimpleListView::shiftSelectItemsWithBound(int selectionStartIndex, int selectionEndIndex)
{
    // Note: Shift operation always selection bound from last selection index to current index.
    // So we don't need *clear* lastSelectionIndex for keep shift + button is right logic.
    clearSelections(false);
    QList<DSimpleListItem*> items = QList<DSimpleListItem*>();
    int index = 0;
    for (DSimpleListItem *item:*renderItems) {
        if (index >= selectionStartIndex && index <= selectionEndIndex) {
            items << item;
        }

        index++;
    }

    // Note: Shift operation always selection bound from last selection index to current index.
    // So we don't need *record* lastSelectionIndex for keep shift + button is right logic.
    addSelections(items, false);
}

void DSimpleListView::shiftSelectPrevItemWithOffset(int scrollOffset)
{
    // Record old render offset to control scrollbar whether display.
    oldRenderOffset = renderOffset;

    if (selectionItems->empty()) {
        selectFirstItem();
    } else {
        int firstIndex = renderItems->count();
        int lastIndex = 0;
        for (DSimpleListItem *item:*selectionItems) {
            int index = renderItems->indexOf(item);

            if (index < firstIndex) {
                firstIndex = index;
            }

            if (index > lastIndex) {
                lastIndex = index;
            }
        }

        if (firstIndex != -1) {
            int lastSelectionIndex = renderItems->indexOf(lastSelectItem);
            int selectionStartIndex, selectionEndIndex;

            if (lastIndex == lastSelectionIndex) {
                selectionStartIndex = std::max(0, firstIndex - scrollOffset);
                selectionEndIndex = lastSelectionIndex;
            } else {
                selectionStartIndex = firstIndex;
                selectionEndIndex = std::max(0, lastIndex - scrollOffset);
            }

            shiftSelectItemsWithBound(selectionStartIndex, selectionEndIndex);

            renderOffset = adjustRenderOffset((selectionStartIndex - 1) * rowHeight + titleHeight);

            repaint();
        }
    }
}

void DSimpleListView::shiftSelectNextItemWithOffset(int scrollOffset)
{
    // Record old render offset to control scrollbar whether display.
    oldRenderOffset = renderOffset;

    if (selectionItems->empty()) {
        selectFirstItem();
    } else {
        int firstIndex = renderItems->count();
        int lastIndex = 0;
        for (DSimpleListItem *item:*selectionItems) {
            int index = renderItems->indexOf(item);

            if (index < firstIndex) {
                firstIndex = index;
            }

            if (index > lastIndex) {
                lastIndex = index;
            }
        }

        if (firstIndex != -1) {
            int lastSelectionIndex = renderItems->indexOf(lastSelectItem);
            int selectionStartIndex, selectionEndIndex;

            if (firstIndex == lastSelectionIndex) {
                selectionStartIndex = firstIndex;
                selectionEndIndex = std::min(renderItems->count() - 1, lastIndex + scrollOffset);
            } else {
                selectionStartIndex = std::min(renderItems->count() - 1, firstIndex + scrollOffset);
                selectionEndIndex = lastIndex;
            }

            shiftSelectItemsWithBound(selectionStartIndex, selectionEndIndex);

            renderOffset = adjustRenderOffset((selectionEndIndex + 1) * rowHeight + titleHeight - rect().height());

            repaint();
        }
    }
}

QList<int> DSimpleListView::getRenderWidths()
{
    QList<int> renderWidths;
    if (columnWidths.contains(-1)) {
        for (int i = 0; i < columnWidths.count(); i++) {
            if (columnWidths[i] != -1) {
                if (columnVisibles[i]) {
                    renderWidths << columnWidths[i];
                } else {
                    renderWidths << 0;
                }
            } else {
                if (columnVisibles[i]) {
                    int totalWidthOfOtherColumns = 0;

                    for (int j = 0; j < columnWidths.count(); j++) {
                        if (columnWidths[j] != -1 && columnVisibles[j]) {
                            totalWidthOfOtherColumns += columnWidths[j];
                        }
                    }

                    renderWidths << rect().width() - totalWidthOfOtherColumns;
                } else {
                    renderWidths << 0;
                }
            }
        }
    } else {
        for (int i = 0; i < columnWidths.count(); i++) {
            if (columnVisibles[i]) {
                renderWidths << columnWidths[i];
            } else {
                renderWidths << 0;
            }
        }
    }

    return renderWidths;
}

bool DSimpleListView::isMouseAtScrollArea(int x)
{
    return (x > rect().x() + rect().width() - scrollbarDragWidth) && (x < rect().x() + rect().width());
}

bool DSimpleListView::isMouseAtTitleArea(int y)
{
    return (y > rect().y() && y < rect().y() + titleHeight);
}

int DSimpleListView::adjustRenderOffset(int offset)
{
    return std::max(0, std::min(offset, getBottomRenderOffset()));
}

int DSimpleListView::getItemsTotalHeight()
{
    return renderItems->count() * rowHeight;
}

int DSimpleListView::getScrollAreaHeight()
{
    return rect().height() - titleHeight;
}

int DSimpleListView::getScrollbarY()
{
    return static_cast<int>((renderOffset / (getItemsTotalHeight() * 1.0)) * getScrollAreaHeight() + titleHeight);
}

int DSimpleListView::getScrollbarHeight()
{
    return std::max(static_cast<int>(getScrollAreaHeight() / (getItemsTotalHeight() * 1.0) * rect().height()), scrollbarMinHeight);
}

int DSimpleListView::getTopRenderOffset()
{
    return 0;
}

QList<DSimpleListItem*> DSimpleListView::getSearchItems(QList<DSimpleListItem*> items)
{
    if (searchContent == "" || searchAlgorithm == NULL) {
        return items;
    } else {
        QList<DSimpleListItem*> *searchItems = new QList<DSimpleListItem*>();

        for (DSimpleListItem *item : items) {
            if (searchAlgorithm(item, searchContent)) {
                searchItems->append(item);
            }
        }

        return *searchItems;
    }
}

int DSimpleListView::getBottomRenderOffset()
{
    int itemsHeight = getItemsTotalHeight();
    if (itemsHeight > rect().height() - titleHeight) {
        return getItemsTotalHeight() - rect().height() + titleHeight;
    } else {
        return 0;
    }
}

void DSimpleListView::sortItemsByColumn(int column, bool descendingSort)
{
    if (sortingAlgorithms->count() != 0 && sortingAlgorithms->count() == columnTitles.count() && sortingOrderes->count() == columnTitles.count()) {
        qSort(renderItems->begin(), renderItems->end(), [&](const DSimpleListItem *item1, const DSimpleListItem *item2) {
                return (*sortingAlgorithms)[column](item1, item2, descendingSort);
            });
    }
}

void DSimpleListView::startScrollbarHideTimer()
{
    if (hideScrollbarTimer != NULL) {
        hideScrollbarTimer->stop();
    }

    hideScrollbarTimer = new QTimer();
    connect(hideScrollbarTimer, SIGNAL(timeout()), this, SLOT(hideScrollbar()));
    hideScrollbarTimer->start(hideScrollbarDuration);
}

DWIDGET_END_NAMESPACE
