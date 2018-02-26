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
#include <DObjectPrivate>
#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QMenu>
#include <QStyleFactory>
#include <QWheelEvent>
#include <QtMath>
#include "dhidpihelper.h"

DCORE_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

class DSimpleListViewPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    DSimpleListViewPrivate(DSimpleListView *parent) : DObjectPrivate(parent)
        {

        }

    QList<DSimpleListItem*> getSearchItems(QList<DSimpleListItem*> items);
    int getItemsTotalHeight();
    int getTopRenderOffset();
    void sortItemsByColumn(int column, bool descendingSort);

    DSimpleListItem *lastHoverItem;
    DSimpleListItem *lastSelectItem;
    DSimpleListItem *drawHoverItem;
    DSimpleListItem *mouseHoverItem;
    DSimpleListItem *mousePressItem;
    DSimpleListItem *mouseReleaseItem;
    QList<DSimpleListItem*> *listItems;
    QList<DSimpleListItem*> *renderItems;
    QList<DSimpleListItem*> *selectionItems;
    QList<QString> columnTitles;
    QList<SortAlgorithm> *sortingAlgorithms;
    QList<bool> *sortingOrderes;
    QList<int> columnWidths;
    QString searchContent;
    QTimer *hideScrollbarTimer;
    SearchAlgorithm searchAlgorithm;
    bool defaultSortingOrder;
    bool mouseAtScrollArea;
    bool mouseDragScrollbar;
    bool drawFrame;
    bool drawHover;
    int alwaysVisibleColumn;
    int clipRadius;
    int defaultSortingColumn;
    int hideScrollbarDuration;
    int lastHoverColumnIndex;
    int oldRenderOffset;
    int renderOffset;
    int rowHeight;
    int scrollDistance;
    int scrollStartY;
    int scrollUnit;
    int scrollbarDefaultWidth;
    int scrollbarDragWidth;
    int scrollbarMinHeight;
    int scrollbarPadding;
    int titleArrowPadding;
    int titleHeight;
    int titleHoverColumn;
    int titlePadding;
    int titlePressColumn;

    D_DECLARE_PUBLIC(DSimpleListView)
};

DSimpleListView::DSimpleListView(QWidget *parent) : QWidget(parent), DObject(*new DSimpleListViewPrivate(this))
{
    D_D(DSimpleListView);

    // Init.
    installEventFilter(this);   // add event filter
    setMouseTracking(true);    // make MouseMove can response

    d->scrollDistance = 0;
    d->renderOffset = 0;
    d->titleHeight = 0;
    d->titleArrowPadding = 4;
    d->titlePadding = 14;
    d->rowHeight = 36;

    d->searchContent = "";
    d->searchAlgorithm = NULL;

    d->titleHoverColumn = -1;
    d->titlePressColumn = -1;

    arrowUpDarkNormalImage = DHiDPIHelper::loadNxPixmap(":/images/dark/images/arrow_up_normal.svg");
    arrowUpDarkHoverImage = DHiDPIHelper::loadNxPixmap(":/images/dark/images/arrow_up_hover.svg");
    arrowUpDarkPressImage = DHiDPIHelper::loadNxPixmap(":/images/dark/images/arrow_up_press.svg");
    arrowDownDarkNormalImage = DHiDPIHelper::loadNxPixmap(":/images/dark/images/arrow_down_normal.svg");
    arrowDownDarkHoverImage = DHiDPIHelper::loadNxPixmap(":/images/dark/images/arrow_down_hover.svg");
    arrowDownDarkPressImage = DHiDPIHelper::loadNxPixmap(":/images/dark/images/arrow_down_press.svg");

    arrowUpLightNormalImage = DHiDPIHelper::loadNxPixmap(":/images/light/images/arrow_up_normal.svg");
    arrowUpLightHoverImage = DHiDPIHelper::loadNxPixmap(":/images/light/images/arrow_up_hover.svg");
    arrowUpLightPressImage = DHiDPIHelper::loadNxPixmap(":/images/light/images/arrow_up_press.svg");
    arrowDownLightNormalImage = DHiDPIHelper::loadNxPixmap(":/images/light/images/arrow_down_normal.svg");
    arrowDownLightHoverImage = DHiDPIHelper::loadNxPixmap(":/images/light/images/arrow_down_hover.svg");
    arrowDownLightPressImage = DHiDPIHelper::loadNxPixmap(":/images/light/images/arrow_down_press.svg");

    arrowUpNormalImage = arrowUpLightNormalImage;
    arrowUpHoverImage = arrowUpLightHoverImage;
    arrowUpPressImage = arrowUpLightPressImage;
    arrowDownNormalImage = arrowDownLightNormalImage;
    arrowDownHoverImage = arrowDownLightHoverImage;
    arrowDownPressImage = arrowDownLightPressImage;

    d->listItems = new QList<DSimpleListItem*>();
    d->renderItems = new QList<DSimpleListItem*>();
    d->selectionItems = new QList<DSimpleListItem*>();
    d->lastSelectItem = NULL;
    d->lastHoverItem = NULL;
    d->lastHoverColumnIndex = -1;
    d->drawHoverItem = NULL;
    d->mouseHoverItem = NULL;
    d->mousePressItem = NULL;
    d->mouseReleaseItem = NULL;

    d->mouseAtScrollArea = false;
    d->mouseDragScrollbar = false;
    d->drawFrame = false;
    d->drawHover = false;
    d->scrollbarDefaultWidth = 4;
    d->scrollbarDragWidth = 8;
    d->scrollbarMinHeight = 30;
    d->scrollbarPadding = 4;
    d->hideScrollbarDuration = 2000;

    d->oldRenderOffset = 0;
    d->clipRadius = 0;

    d->hideScrollbarTimer = NULL;

    d->sortingAlgorithms = new QList<SortAlgorithm>();
    d->sortingOrderes = new QList<bool>();
}

DSimpleListView::~DSimpleListView()
{
    D_D(DSimpleListView);

    delete d->lastHoverItem;
    delete d->lastSelectItem;
    delete d->drawHoverItem;
    delete d->mouseHoverItem;
    delete d->mousePressItem;
    delete d->mouseReleaseItem;
    delete d->listItems;
    delete d->renderItems;
    delete d->selectionItems;
    delete d->sortingAlgorithms;
    delete d->sortingOrderes;
    delete d->hideScrollbarTimer;
}

void DSimpleListView::setRowHeight(int height)
{
    D_D(DSimpleListView);

    d->rowHeight = height;
    d->scrollUnit = d->rowHeight;
}

void DSimpleListView::setColumnTitleInfo(QList<QString> titles, QList<int> widths, int height)
{
    D_D(DSimpleListView);

    // Set column titles.
    d->columnTitles = titles;

    // Calcuate column title widths.
    d->columnWidths.clear();

    QFont font;
    font.setPointSize(titleSize);
    QFontMetrics fm(font);

    for (int i = 0; i < widths.length(); i++) {
        if (widths[i] == -1) {
            d->columnWidths << widths[i];
        } else {
            int renderTitleWidth = fm.width(titles[i]) + d->titlePadding + arrowUpNormalImage.width() / arrowUpNormalImage.devicePixelRatio() + d->titleArrowPadding * 2;
            d->columnWidths << std::max(widths[i], renderTitleWidth);
        }
    }

    // Set title height.
    d->titleHeight = height;
}

void DSimpleListView::setColumnHideFlags(QList<bool> toggleHideFlags, int visibleColumnIndex)
{
    D_D(DSimpleListView);

    Q_ASSERT_X(toggleHideFlags.contains(false), "toggleHideFlags", "at least have one 'false' in list.");
    Q_ASSERT_X(toggleHideFlags.count() == d->columnTitles.count(), "toggleHideFlags", "hide flags length is not same as titles list.");

    d->alwaysVisibleColumn = visibleColumnIndex;

    columnVisibles.clear();
    for (int i = 0; i < toggleHideFlags.count(); i++) {
        columnVisibles.append(toggleHideFlags[i]);
    }
}

void DSimpleListView::setColumnSortingAlgorithms(QList<SortAlgorithm> *algorithms, int sortColumn, bool descendingSort)
{
    D_D(DSimpleListView);

    // Add sort algorithms.
    d->sortingAlgorithms = algorithms;

    for (int i = 0; i < d->sortingAlgorithms->count(); i++) {
        d->sortingOrderes->append(false);
    }

    // If sort column is -1, don't sort default.
    d->defaultSortingColumn = sortColumn;
    d->defaultSortingOrder = descendingSort;
}

void DSimpleListView::setSearchAlgorithm(SearchAlgorithm algorithm)
{
    D_D(DSimpleListView);

    d->searchAlgorithm = algorithm;
}

void DSimpleListView::setClipRadius(int radius)
{
    D_D(DSimpleListView);

    d->clipRadius = radius;
}

void DSimpleListView::setFrame(bool enableFrame, QColor color, double opacity)
{
    D_D(DSimpleListView);

    d->drawFrame = enableFrame;
    frameColor = color;
    frameOpacity = opacity;
}

void DSimpleListView::setHoverEffect(bool enableHoverEffect)
{
    D_D(DSimpleListView);

    d->drawHover = enableHoverEffect;
}

void DSimpleListView::addItems(QList<DSimpleListItem*> items)
{
    D_D(DSimpleListView);

    // Add item to list.
    d->listItems->append(items);
    QList<DSimpleListItem*> searchItems = d->getSearchItems(items);
    d->renderItems->append(searchItems);

    // If user has click title to sort, sort items after add items to list.
    if (d->defaultSortingColumn != -1) {
        d->sortItemsByColumn(d->defaultSortingColumn, d->defaultSortingOrder);
    }

    // Repaint after add items.
    repaint();
}

void DSimpleListView::clearItems()
{
    D_D(DSimpleListView);

    // NOTE:
    // We need delete items in QList before clear QList to avoid *MEMORY LEAK* .
    qDeleteAll(d->listItems->begin(), d->listItems->end());
    d->listItems->clear();
    d->renderItems->clear();
}

void DSimpleListView::addSelections(QList<DSimpleListItem*> items, bool recordLastSelection)
{
    D_D(DSimpleListView);

    // Add item to selection list.
    d->selectionItems->append(items);

    // Record last selection item to make selected operation continuously.
    if (recordLastSelection && d->selectionItems->count() > 0) {
        d->lastSelectItem = d->selectionItems->last();
    }
}

void DSimpleListView::clearSelections(bool clearLastSelection)
{
    D_D(DSimpleListView);

    // Clear selection list.
    d->selectionItems->clear();

    if (clearLastSelection) {
        d->lastSelectItem = NULL;
    }
}

void DSimpleListView::refreshItems(QList<DSimpleListItem*> items)
{
    D_D(DSimpleListView);

    // Init.
    QList<DSimpleListItem*> *newSelectionItems = new QList<DSimpleListItem*>();
    DSimpleListItem *newLastSelectionItem = NULL;
    DSimpleListItem *newLastHoverItem = NULL;

    // Save selection items and last selection item.
    for (DSimpleListItem *item:items) {
        for (DSimpleListItem *selectionItem:*d->selectionItems) {
            if (item->sameAs(selectionItem)) {
                newSelectionItems->append(item);
                break;
            }
        }
    }

    if (d->lastSelectItem != NULL) {
        for (DSimpleListItem *item:items) {
            if (item->sameAs(d->lastSelectItem)) {
                newLastSelectionItem = item;
                break;
            }
        }
    }

    if (d->lastHoverItem != NULL) {
        for (DSimpleListItem *item:items) {
            if (item->sameAs(d->lastHoverItem)) {
                newLastHoverItem = item;
                break;
            }
        }
    }
    d->lastHoverItem = NULL;

    // Update items.
    clearItems();
    d->listItems->append(items);
    QList<DSimpleListItem*> searchItems = d->getSearchItems(items);
    d->renderItems->append(searchItems);

    // Sort once if default sort column hasn't init.
    if (d->defaultSortingColumn != -1) {
        d->sortItemsByColumn(d->defaultSortingColumn, d->defaultSortingOrder);
    }

    // Restore selection items and last selection item.
    clearSelections();
    addSelections(*newSelectionItems, false);
    d->lastSelectItem = newLastSelectionItem;
    d->lastHoverItem = newLastHoverItem;

    // Keep scroll position.
    d->renderOffset = adjustRenderOffset(d->renderOffset);

    // Render.
    repaint();
}

void DSimpleListView::search(QString content)
{
    D_D(DSimpleListView);

    if (content == "" && d->searchContent != content) {
        d->searchContent = content;

        d->renderItems->clear();
        d->renderItems->append(*d->listItems);
    } else {
        d->searchContent = content;

        QList<DSimpleListItem*> searchItems = d->getSearchItems(*d->listItems);
        d->renderItems->clear();
        d->renderItems->append(searchItems);
    }

    repaint();
}

void DSimpleListView::selectAllItems()
{
    D_D(DSimpleListView);

    // Record old render offset to control scrollbar whether display.
    d->oldRenderOffset = d->renderOffset;

    // Select all items.
    clearSelections();
    addSelections(*d->renderItems);

    // Scroll to top.
    d->renderOffset = d->getTopRenderOffset();

    // Repaint.
    repaint();
}

void DSimpleListView::selectFirstItem()
{
    D_D(DSimpleListView);

    // Record old render offset to control scrollbar whether display.
    d->oldRenderOffset = d->renderOffset;

    // Select first item.
    clearSelections();

    QList<DSimpleListItem*> items = QList<DSimpleListItem*>();
    items << d->renderItems->first();
    addSelections(items);

    // Scroll to top.
    d->renderOffset = d->getTopRenderOffset();

    // Repaint.
    repaint();
}

void DSimpleListView::selectLastItem()
{
    D_D(DSimpleListView);

    // Record old render offset to control scrollbar whether display.
    d->oldRenderOffset = d->renderOffset;

    // Select last item.
    clearSelections();

    QList<DSimpleListItem*> items = QList<DSimpleListItem*>();
    items << d->renderItems->last();
    addSelections(items);

    // Scroll to bottom.
    d->renderOffset = getBottomRenderOffset();

    // Repaint.
    repaint();
}

void DSimpleListView::selectPrevItem()
{
    D_D(DSimpleListView);

    selectPrevItemWithOffset(1);
}

void DSimpleListView::selectNextItem()
{
    D_D(DSimpleListView);

    selectNextItemWithOffset(1);
}

void DSimpleListView::shiftSelectPageDown()
{
    D_D(DSimpleListView);

    shiftSelectNextItemWithOffset(getScrollAreaHeight() / d->rowHeight);
}

void DSimpleListView::shiftSelectPageUp()
{
    D_D(DSimpleListView);

    shiftSelectPrevItemWithOffset(getScrollAreaHeight() / d->rowHeight);
}

void DSimpleListView::shiftSelectToEnd()
{
    D_D(DSimpleListView);

    // Select last item if nothing selected yet.
    if (d->selectionItems->empty()) {
        selectLastItem();
    }
    // Select items from last selected item to last item.
    else {
        // Found last selected index and do select operation.
        int lastSelectionIndex = d->renderItems->indexOf(d->lastSelectItem);
        shiftSelectItemsWithBound(lastSelectionIndex, d->renderItems->count() - 1);

        // Scroll to bottom.
        d->renderOffset = getBottomRenderOffset();

        // Repaint.
        repaint();
    }
}

void DSimpleListView::shiftSelectToHome()
{
    D_D(DSimpleListView);

    // Select first item if nothing selected yet.
    if (d->selectionItems->empty()) {
        selectFirstItem();
    }
    // Select items from last selected item to first item.
    else {
        // Found last selected index and do select operation.
        int lastSelectionIndex = d->renderItems->indexOf(d->lastSelectItem);
        shiftSelectItemsWithBound(0, lastSelectionIndex);

        // Scroll to top.
        d->renderOffset = d->getTopRenderOffset();

        // Repaint.
        repaint();
    }
}

void DSimpleListView::shiftSelectToNext()
{
    D_D(DSimpleListView);

    shiftSelectNextItemWithOffset(1);
}

void DSimpleListView::shiftSelectToPrev()
{
    D_D(DSimpleListView);

    shiftSelectPrevItemWithOffset(1);
}

void DSimpleListView::scrollPageDown()
{
    D_D(DSimpleListView);

    selectNextItemWithOffset(getScrollAreaHeight() / d->rowHeight);
}

void DSimpleListView::scrollPageUp()
{
    D_D(DSimpleListView);

    selectPrevItemWithOffset(getScrollAreaHeight() / d->rowHeight);
}

void DSimpleListView::ctrlScrollPageUp()
{
    D_D(DSimpleListView);

    d->renderOffset = adjustRenderOffset(d->renderOffset - getScrollAreaHeight());

    repaint();
}

void DSimpleListView::ctrlScrollPageDown()
{
    D_D(DSimpleListView);

    d->renderOffset = adjustRenderOffset(d->renderOffset + getScrollAreaHeight());

    repaint();
}

void DSimpleListView::ctrlScrollToHome()
{
    D_D(DSimpleListView);

    d->renderOffset = d->getTopRenderOffset();

    repaint();
}

void DSimpleListView::ctrlScrollToEnd()
{
    D_D(DSimpleListView);

    d->renderOffset = getBottomRenderOffset();

    repaint();
}

void DSimpleListView::leaveEvent(QEvent * event){
    hideScrollbar();

    QWidget::leaveEvent(event);
}

void DSimpleListView::hideScrollbar()
{
    D_D(DSimpleListView);

    // Record old render offset to control scrollbar whether display.
    d->mouseAtScrollArea = false;
    d->oldRenderOffset = d->renderOffset;

    repaint();
}

bool DSimpleListView::eventFilter(QObject *, QEvent *)
{
    return false;
}

void DSimpleListView::keyPressEvent(QKeyEvent *keyEvent)
{
    D_D(DSimpleListView);

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
    D_D(DSimpleListView);

    // Scroll if mouse drag at scrollbar.
    if (d->mouseDragScrollbar) {
        int barHeight = getScrollbarHeight();
        d->renderOffset = adjustRenderOffset((mouseEvent->y() - barHeight / 2 - d->titleHeight) / (getScrollAreaHeight() * 1.0) * d->getItemsTotalHeight());

        repaint();
    }
    // Update scrollbar status with mouse position.
    else if (isMouseAtScrollArea(mouseEvent->x()) != d->mouseAtScrollArea) {
        d->mouseAtScrollArea = isMouseAtScrollArea(mouseEvent->x());
        repaint();
    }
    // Otherwise to check titlebar arrow status.
    else {
        bool atTitleArea = isMouseAtTitleArea(mouseEvent->y());

        if (atTitleArea) {
            int hoverColumn = -1;

            if (d->sortingAlgorithms->count() != 0 && d->sortingAlgorithms->count() == d->columnTitles.count() && d->sortingOrderes->count() == d->columnTitles.count()) {
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

            if (hoverColumn != d->titleHoverColumn) {
                d->titleHoverColumn = hoverColumn;

                repaint();
            }
        } else {
            int hoverItemIndex = (d->renderOffset + mouseEvent->y() - d->titleHeight) / d->rowHeight;

            // NOTE: hoverItemIndex may be less than 0, we need check index here.
            if (hoverItemIndex >= 0 && hoverItemIndex < (*d->renderItems).length()) {
                DSimpleListItem *item = (*d->renderItems)[hoverItemIndex];

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

                if (d->drawHover && (d->drawHoverItem == NULL || !item->sameAs(d->drawHoverItem))) {
                    d->drawHoverItem = item;

                    repaint();
                }

                // Emit mouseHoverChanged signal.
                mouseHoverChanged(d->mouseHoverItem, item, columnCounter, QPoint(mouseEvent->x() - columnRenderX, mouseEvent->y() - hoverItemIndex * d->rowHeight));
                d->mouseHoverItem = item;

                if (d->lastHoverItem == NULL || !item->sameAs(d->lastHoverItem) || columnCounter != d->lastHoverColumnIndex) {
                    d->lastHoverItem = item;
                    d->lastHoverColumnIndex = columnCounter;

                    changeHoverItem(this->mapToGlobal(mouseEvent->pos()), d->lastHoverItem, columnCounter);
                }
            }
        }
    }
}

void DSimpleListView::mousePressEvent(QMouseEvent *mouseEvent)
{
    D_D(DSimpleListView);

    setFocus();

    bool atTitleArea = isMouseAtTitleArea(mouseEvent->y());
    bool atScrollArea = isMouseAtScrollArea(mouseEvent->x());

    // Sort items with column's sorting algorithms when click on title area.
    if (atTitleArea) {
        if (mouseEvent->button() == Qt::LeftButton) {
            if (d->sortingAlgorithms->count() != 0 && d->sortingAlgorithms->count() == d->columnTitles.count() && d->sortingOrderes->count() == d->columnTitles.count()) {
                // Calcuate title widths;
                QList<int> renderWidths = getRenderWidths();

                int columnCounter = 0;
                int columnRenderX = 0;
                for (int renderWidth:renderWidths) {
                    if (renderWidth > 0) {
                        if (mouseEvent->x() > columnRenderX && mouseEvent->x() < columnRenderX + renderWidth) {
                            // If switch other column, default order is from top to bottom.
                            if (columnCounter != d->defaultSortingColumn) {
                                (*d->sortingOrderes)[columnCounter] = true;
                            }
                            // If user click same column, just switch reverse order.
                            else {
                                (*d->sortingOrderes)[columnCounter] = !(*d->sortingOrderes)[columnCounter];
                            }

                            d->defaultSortingColumn = columnCounter;
                            d->defaultSortingOrder = (*d->sortingOrderes)[columnCounter];

                            changeSortingStatus(d->defaultSortingColumn, d->defaultSortingOrder);

                            d->sortItemsByColumn(columnCounter, (*d->sortingOrderes)[columnCounter]);

                            if (columnCounter != d->titlePressColumn) {
                                d->titlePressColumn = columnCounter;
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
            if (columnVisibles.count() == d->columnTitles.count()) {
                QMenu *menu = new QMenu();
                menu->setStyle(QStyleFactory::create("dlight"));

                for (int i = 0; i < columnVisibles.count(); i++) {
                    if (i != d->alwaysVisibleColumn) {
                        QAction *action = new QAction(menu);
                        action->setText(d->columnTitles[i]);

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
            d->mouseDragScrollbar = true;
        }
        // Scroll if click out of scrollbar area.
        else {
            d->renderOffset = adjustRenderOffset((mouseEvent->y() - barHeight / 2 - d->titleHeight) / (getScrollAreaHeight() * 1.0) * d->getItemsTotalHeight());
            repaint();
        }
    }
    // Select items.
    else {
        int pressItemIndex = (d->renderOffset + mouseEvent->y() - d->titleHeight) / d->rowHeight;

        if (pressItemIndex >= d->renderItems->count()) {
            clearSelections();

            repaint();
        } else {
            if (mouseEvent->button() == Qt::LeftButton) {
                if (pressItemIndex < d->renderItems->count()) {
                    // Scattered selection of items when press ctrl modifier.
                    if (mouseEvent->modifiers() == Qt::ControlModifier) {
                        DSimpleListItem *item = (*d->renderItems)[pressItemIndex];

                        if (d->selectionItems->contains(item)) {
                            d->selectionItems->removeOne(item);
                        } else {
                            QList<DSimpleListItem*> items = QList<DSimpleListItem*>();
                            items << item;
                            addSelections(items);
                        }
                    }
                    // Continuous selection of items when press shift modifier.
                    else if ((mouseEvent->modifiers() == Qt::ShiftModifier) && !d->selectionItems->empty()) {
                        int lastSelectionIndex = d->renderItems->indexOf(d->lastSelectItem);
                        int selectionStartIndex = std::min(pressItemIndex, lastSelectionIndex);
                        int selectionEndIndex = std::max(pressItemIndex, lastSelectionIndex);

                        shiftSelectItemsWithBound(selectionStartIndex, selectionEndIndex);
                    }
                    // Just select item under mouse if user not press any modifier.
                    else {
                        clearSelections();

                        QList<DSimpleListItem*> items = QList<DSimpleListItem*>();
                        items << (*d->renderItems)[pressItemIndex];
                        addSelections(items);
                    }

                    // Emit mousePressChanged signal.
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
                    mousePressChanged((*d->renderItems)[pressItemIndex], columnCounter, QPoint(mouseEvent->x() - columnRenderX, mouseEvent->y() - pressItemIndex * d->rowHeight));

                    repaint();
                }
            } else if (mouseEvent->button() == Qt::RightButton) {
                DSimpleListItem *pressItem = (*d->renderItems)[pressItemIndex];
                bool pressInSelectionArea = false;

                for (DSimpleListItem *item : *d->selectionItems) {
                    if (item == pressItem) {
                        pressInSelectionArea = true;

                        break;
                    }
                }

                if (!pressInSelectionArea && pressItemIndex < d->renderItems->length()) {
                    clearSelections();

                    QList<DSimpleListItem*> items = QList<DSimpleListItem*>();
                    items << (*d->renderItems)[pressItemIndex];
                    addSelections(items);

                    repaint();
                }

                if (d->selectionItems->length() > 0) {
                    rightClickItems(this->mapToGlobal(mouseEvent->pos()), *d->selectionItems);
                }
            }
        }
    }
}

void DSimpleListView::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
    D_D(DSimpleListView);

    if (d->mouseDragScrollbar) {
        // Reset mouseDragScrollbar.
        d->mouseDragScrollbar = false;

        repaint();
    } else {
        if (d->titlePressColumn != -1) {
            d->titlePressColumn = -1;
            repaint();
        }
    }

    // Emit mouseReleaseChanged signal.
    int releaseItemIndex = (d->renderOffset + mouseEvent->y() - d->titleHeight) / d->rowHeight;
        
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
    mouseReleaseChanged((*d->renderItems)[releaseItemIndex], columnCounter, QPoint(mouseEvent->x() - columnRenderX, mouseEvent->y() - releaseItemIndex * d->rowHeight));
}

void DSimpleListView::wheelEvent(QWheelEvent *event)
{
    D_D(DSimpleListView);

    if (event->orientation() == Qt::Vertical) {
        // Record old render offset to control scrollbar whether display.
        d->oldRenderOffset = d->renderOffset;

        qreal scrollStep = event->angleDelta().y() / 120.0;
        d->renderOffset = adjustRenderOffset(d->renderOffset - scrollStep * d->scrollUnit);

        repaint();
    }

    event->accept();
}

void DSimpleListView::paintEvent(QPaintEvent *)
{
    D_D(DSimpleListView);

    // Init.
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Calcuate title widths;
    QList<int> renderWidths = getRenderWidths();

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setOpacity(0.05);

    int penWidth = 1;
    QPainterPath framePath;
    framePath.addRoundedRect(QRect(rect().x() + penWidth, rect().y() + penWidth, rect().width() - penWidth * 2, rect().height() - penWidth * 2), d->clipRadius, d->clipRadius);
    painter.setClipPath(framePath);

    // Draw title.
    if (d->titleHeight > 0) {
        QPainterPath titlePath;
        titlePath.addRect(QRectF(rect().x(), rect().y(), rect().width(), d->titleHeight));
        painter.setOpacity(titleAreaOpacity);
        painter.fillPath(titlePath, QColor(titleAreaColor));
    }

    int renderY = 0;
    int renderHeight = 0;
    if (d->titleHeight > 0) {
        int columnCounter = 0;
        int columnRenderX = 0;
        for (int renderWidth:renderWidths) {
            if (renderWidth > 0) {
                painter.setOpacity(1);

                QFont font = painter.font() ;
                font.setPointSize(titleSize);
                painter.setFont(font);

                painter.setPen(QPen(QColor(titleColor)));
                painter.drawText(QRect(columnRenderX + d->titlePadding, 0, renderWidth, d->titleHeight), Qt::AlignVCenter | Qt::AlignLeft, d->columnTitles[columnCounter]);

                columnRenderX += renderWidth;

                if (columnCounter < renderWidths.size() - 1) {
                    painter.setOpacity(0.05);
                    QPainterPath separatorPath;
                    separatorPath.addRect(QRectF(rect().x() + columnRenderX - 1, rect().y() + 4, 1, d->titleHeight - 8));
                    painter.fillPath(separatorPath, QColor(titleLineColor));
                }

                // Draw sort arrow.
                if (d->defaultSortingColumn == columnCounter) {
                    painter.setOpacity(1);
                    int arrowX = rect().x() + columnRenderX - d->titleArrowPadding - arrowUpNormalImage.width() / arrowUpNormalImage.devicePixelRatio();
                    int arrowY = rect().y() + (d->titleHeight - arrowDownNormalImage.height() / arrowUpNormalImage.devicePixelRatio()) / 2;

                    if (d->defaultSortingOrder) {
                        if (d->titlePressColumn == d->defaultSortingColumn) {
                            painter.drawPixmap(QPoint(arrowX, arrowY), arrowDownPressImage);
                        } else if (d->titleHoverColumn == d->defaultSortingColumn) {
                            painter.drawPixmap(QPoint(arrowX, arrowY), arrowDownHoverImage);
                        } else {
                            painter.drawPixmap(QPoint(arrowX, arrowY), arrowDownNormalImage);
                        }
                    } else {
                        if (d->titlePressColumn == d->defaultSortingColumn) {
                            painter.drawPixmap(QPoint(arrowX, arrowY), arrowUpPressImage);
                        } else if (d->titleHoverColumn == d->defaultSortingColumn) {
                            painter.drawPixmap(QPoint(arrowX, arrowY), arrowUpHoverImage);
                        } else {
                            painter.drawPixmap(QPoint(arrowX, arrowY), arrowUpNormalImage);
                        }
                    }
                }
            }
            columnCounter++;
        }

        renderY += d->titleHeight;
        renderHeight += d->titleHeight;
    }

    // Draw background.
    painter.setOpacity(backgroundOpacity);
    QPainterPath backgroundPath;
    backgroundPath.addRect(QRectF(rect().x(), rect().y() + d->titleHeight, rect().width(), rect().height() - d->titleHeight));
    painter.fillPath(backgroundPath, QColor(backgroundColor));

    // Draw context.
    QPainterPath scrollAreaPath;
    scrollAreaPath.addRect(QRectF(rect().x(), rect().y() + d->titleHeight, rect().width(), getScrollAreaHeight()));

    int rowCounter = 0;
    for (DSimpleListItem *item:*d->renderItems) {
        if (rowCounter > ((d->renderOffset - d->rowHeight) / d->rowHeight)) {
            // Clip item rect.
            QPainterPath itemPath;
            itemPath.addRect(QRect(0, renderY + rowCounter * d->rowHeight - d->renderOffset, rect().width(), d->rowHeight));
            painter.setClipPath((framePath.intersected(scrollAreaPath)).intersected(itemPath));

            // Draw item backround.
            bool isSelect = d->selectionItems->contains(item);
            painter.save();
            item->drawBackground(QRect(0, renderY + rowCounter * d->rowHeight - d->renderOffset, rect().width(), d->rowHeight), &painter, rowCounter, isSelect);
            painter.restore();

            // Draw hover effect.
            if (d->drawHover && d->drawHoverItem != NULL && item->sameAs(d->drawHoverItem)) {
                item->drawHover(QRect(0, renderY + rowCounter * d->rowHeight - d->renderOffset, rect().width(), d->rowHeight), &painter);
            }

            // Draw item foreground.
            int columnCounter = 0;
            int columnRenderX = 0;
            for (int renderWidth:renderWidths) {
                if (renderWidth > 0) {
                    painter.save();
                    item->drawForeground(QRect(columnRenderX, renderY + rowCounter * d->rowHeight - d->renderOffset, renderWidth, d->rowHeight), &painter, columnCounter, rowCounter, isSelect);
                    painter.restore();

                    columnRenderX += renderWidth;
                }
                columnCounter++;
            }

            renderHeight += d->rowHeight;

            if (renderHeight > rect().height()) {
                break;
            }
        }

        rowCounter++;
    }

    // Keep clip area.
    painter.setClipPath(framePath);

    // Draw search tooltip.
    if (d->searchContent != "" && d->renderItems->size() == 0) {
        painter.setOpacity(1);
        painter.setPen(QPen(QColor(searchColor)));

        QFont font = painter.font() ;
        font.setPointSize(20);
        painter.setFont(font);

        painter.drawText(QRect(rect().x(), rect().y() + d->titleHeight, rect().width(), rect().height() - d->titleHeight), Qt::AlignCenter, tr("No search result"));
    }

    // Draw frame.
    if (d->drawFrame) {
        QPen framePen;
        framePen.setColor(frameColor);
        painter.setOpacity(frameOpacity);
        painter.drawPath(framePath);
    }

    // Draw scrollbar.
    if (d->mouseAtScrollArea) {
        paintScrollbar(&painter);
    } else if (d->oldRenderOffset != d->renderOffset) {
        paintScrollbar(&painter);

        startScrollbarHideTimer();
    }
}

void DSimpleListView::paintScrollbar(QPainter *painter)
{
    D_D(DSimpleListView);

    if (d->getItemsTotalHeight() > getScrollAreaHeight()) {
        // Init scrollbar opacity with scrollbar status.
        qreal barOpacitry = 0;
        qreal barFrameOpacitry = 0;

        // Press.
        if (d->mouseDragScrollbar) {
            barOpacitry = scrollbarPressOpacity;
            barFrameOpacitry = scrollbarFramePressOpacity;
        } else {
            // Hover.
            if (d->mouseAtScrollArea) {
                barOpacitry = scrollbarHoverOpacity;
                barFrameOpacitry = scrollbarFrameHoverOpacity;
            }
            // Normal.
            else {
                barOpacitry = scrollbarNormalOpacity;
                barFrameOpacitry = scrollbarFrameNormalOpacity;
            }
        }

        int barWidth = (d->mouseAtScrollArea || d->mouseDragScrollbar) ? d->scrollbarDragWidth : d->scrollbarDefaultWidth;
        int barRadius = 4;

        int barY = getScrollbarY();
        int barHeight = getScrollbarHeight();

        painter->setOpacity(barOpacitry);
        QPainterPath path;
        path.addRoundedRect(
            QRectF(rect().x() + rect().width() - barWidth - d->scrollbarPadding,
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
    D_D(DSimpleListView);

    // Record old render offset to control scrollbar whether display.
    d->oldRenderOffset = d->renderOffset;

    if (d->selectionItems->empty()) {
        selectFirstItem();
    } else {
        int lastIndex = 0;
        for (DSimpleListItem *item:*d->selectionItems) {
            int index = d->renderItems->indexOf(item);
            if (index > lastIndex) {
                lastIndex = index;
            }
        }

        if (lastIndex != -1) {
            lastIndex = std::min(d->renderItems->count() - 1, lastIndex + scrollOffset);

            clearSelections(false);

            QList<DSimpleListItem*> items = QList<DSimpleListItem*>();
            items << (*d->renderItems)[lastIndex];

            addSelections(items);

            int itemIndex = lastIndex + 1;
            int itemOffset = adjustRenderOffset(itemIndex * d->rowHeight - rect().height() + d->titleHeight);
            if (((d->renderOffset + getScrollAreaHeight()) / d->rowHeight) < itemIndex) {
                d->renderOffset = itemOffset;
            }

            repaint();
        }
    }
}

void DSimpleListView::selectPrevItemWithOffset(int scrollOffset)
{
    D_D(DSimpleListView);

    // Record old render offset to control scrollbar whether display.
    d->oldRenderOffset = d->renderOffset;

    if (d->selectionItems->empty()) {
        selectFirstItem();
    } else {
        int firstIndex = d->renderItems->count();
        for (DSimpleListItem *item:*d->selectionItems) {
            int index = d->renderItems->indexOf(item);
            if (index < firstIndex) {
                firstIndex = index;
            }
        }

        if (firstIndex != -1) {
            firstIndex = std::max(0, firstIndex - scrollOffset);

            clearSelections();

            QList<DSimpleListItem*> items = QList<DSimpleListItem*>();
            items << (*d->renderItems)[firstIndex];

            addSelections(items);

            int itemIndex = firstIndex - 1;
            int itemOffset = adjustRenderOffset(itemIndex * d->rowHeight + d->titleHeight);
            if ((d->renderOffset / d->rowHeight) > itemIndex) {
                d->renderOffset = itemOffset;
            }

            repaint();
        }
    }
}

void DSimpleListView::shiftSelectItemsWithBound(int selectionStartIndex, int selectionEndIndex)
{
    D_D(DSimpleListView);

    // Note: Shift operation always selection bound from last selection index to current index.
    // So we don't need *clear* lastSelectionIndex for keep shift + button is right logic.
    clearSelections(false);
    QList<DSimpleListItem*> items = QList<DSimpleListItem*>();
    int index = 0;
    for (DSimpleListItem *item:*d->renderItems) {
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
    D_D(DSimpleListView);

    // Record old render offset to control scrollbar whether display.
    d->oldRenderOffset = d->renderOffset;

    if (d->selectionItems->empty()) {
        selectFirstItem();
    } else {
        int firstIndex = d->renderItems->count();
        int lastIndex = 0;
        for (DSimpleListItem *item:*d->selectionItems) {
            int index = d->renderItems->indexOf(item);

            if (index < firstIndex) {
                firstIndex = index;
            }

            if (index > lastIndex) {
                lastIndex = index;
            }
        }

        if (firstIndex != -1) {
            int lastSelectionIndex = d->renderItems->indexOf(d->lastSelectItem);
            int selectionStartIndex, selectionEndIndex;

            if (lastIndex == lastSelectionIndex) {
                selectionStartIndex = std::max(0, firstIndex - scrollOffset);
                selectionEndIndex = lastSelectionIndex;
            } else {
                selectionStartIndex = firstIndex;
                selectionEndIndex = std::max(0, lastIndex - scrollOffset);
            }

            shiftSelectItemsWithBound(selectionStartIndex, selectionEndIndex);

            if (d->renderOffset / d->rowHeight >= selectionStartIndex) {
                d->renderOffset = adjustRenderOffset((selectionStartIndex - 1) * d->rowHeight + d->titleHeight);
            }

            repaint();
        }
    }
}

void DSimpleListView::shiftSelectNextItemWithOffset(int scrollOffset)
{
    D_D(DSimpleListView);

    // Record old render offset to control scrollbar whether display.
    d->oldRenderOffset = d->renderOffset;

    if (d->selectionItems->empty()) {
        selectFirstItem();
    } else {
        int firstIndex = d->renderItems->count();
        int lastIndex = 0;
        for (DSimpleListItem *item:*d->selectionItems) {
            int index = d->renderItems->indexOf(item);

            if (index < firstIndex) {
                firstIndex = index;
            }

            if (index > lastIndex) {
                lastIndex = index;
            }
        }

        if (firstIndex != -1) {
            int lastSelectionIndex = d->renderItems->indexOf(d->lastSelectItem);
            int selectionStartIndex, selectionEndIndex;

            if (firstIndex == lastSelectionIndex) {
                selectionStartIndex = firstIndex;
                selectionEndIndex = std::min(d->renderItems->count() - 1, lastIndex + scrollOffset);
            } else {
                selectionStartIndex = std::min(d->renderItems->count() - 1, firstIndex + scrollOffset);
                selectionEndIndex = lastIndex;
            }

            shiftSelectItemsWithBound(selectionStartIndex, selectionEndIndex);

            if ((d->renderOffset + rect().height()) / d->rowHeight <= selectionEndIndex + 1) {
                d->renderOffset = adjustRenderOffset((selectionEndIndex + 1) * d->rowHeight + d->titleHeight - rect().height());
            }


            repaint();
        }
    }
}

QList<int> DSimpleListView::getRenderWidths()
{
    D_D(DSimpleListView);

    QList<int> renderWidths;
    if (d->columnWidths.length() > 0) {
        if (d->columnWidths.contains(-1)) {
            for (int i = 0; i < d->columnWidths.count(); i++) {
                if (d->columnWidths[i] != -1) {
                    if (columnVisibles[i]) {
                        renderWidths << d->columnWidths[i];
                    } else {
                        renderWidths << 0;
                    }
                } else {
                    if (columnVisibles[i]) {
                        int totalWidthOfOtherColumns = 0;

                        for (int j = 0; j < d->columnWidths.count(); j++) {
                            if (d->columnWidths[j] != -1 && columnVisibles[j]) {
                                totalWidthOfOtherColumns += d->columnWidths[j];
                            }
                        }

                        renderWidths << rect().width() - totalWidthOfOtherColumns;
                    } else {
                        renderWidths << 0;
                    }
                }
            }
        } else {
            for (int i = 0; i < d->columnWidths.count(); i++) {
                if (columnVisibles[i]) {
                    renderWidths << d->columnWidths[i];
                } else {
                    renderWidths << 0;
                }
            }
        }
    }
    // Return widget width if user don't set column withs throught function 'setColumnTitleInfo'.
    // Avoid listview don't draw item's foregound cause by emptry 'columnWidths'.
    else {
        renderWidths << rect().width();
    }

    return renderWidths;
}

bool DSimpleListView::isMouseAtScrollArea(int x)
{
    D_D(DSimpleListView);

    return (x > rect().x() + rect().width() - d->scrollbarDragWidth) && (x < rect().x() + rect().width());
}

bool DSimpleListView::isMouseAtTitleArea(int y)
{
    D_D(DSimpleListView);

    return (y > rect().y() && y < rect().y() + d->titleHeight);
}

int DSimpleListView::adjustRenderOffset(int offset)
{
    return std::max(0, std::min(offset, getBottomRenderOffset()));
}

int DSimpleListViewPrivate::getItemsTotalHeight()
{
    return renderItems->count() * rowHeight;
}

int DSimpleListView::getScrollAreaHeight()
{
    D_D(DSimpleListView);

    return rect().height() - d->titleHeight;
}

int DSimpleListView::getScrollbarY()
{
    D_D(DSimpleListView);

    return static_cast<int>((d->renderOffset / (d->getItemsTotalHeight() * 1.0)) * getScrollAreaHeight() + d->titleHeight);
}

int DSimpleListView::getScrollbarHeight()
{
    D_D(DSimpleListView);

    return std::max(static_cast<int>(getScrollAreaHeight() / (d->getItemsTotalHeight() * 1.0) * rect().height()), d->scrollbarMinHeight);
}

int DSimpleListViewPrivate::getTopRenderOffset()
{
    return 0;
}

QList<DSimpleListItem*> DSimpleListViewPrivate::getSearchItems(QList<DSimpleListItem*> items)
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
    D_D(DSimpleListView);

    int itemsHeight = d->getItemsTotalHeight();
    if (itemsHeight > rect().height() - d->titleHeight) {
        return d->getItemsTotalHeight() - rect().height() + d->titleHeight;
    } else {
        return 0;
    }
}

void DSimpleListViewPrivate::sortItemsByColumn(int column, bool descendingSort)
{
    if (sortingAlgorithms->count() != 0 && sortingAlgorithms->count() == columnTitles.count() && sortingOrderes->count() == columnTitles.count()) {
        qSort(renderItems->begin(), renderItems->end(), [&](const DSimpleListItem *item1, const DSimpleListItem *item2) {
                return (*sortingAlgorithms)[column](item1, item2, descendingSort);
            });
    }
}

void DSimpleListView::startScrollbarHideTimer()
{
    D_D(DSimpleListView);

    if (d->hideScrollbarTimer != NULL) {
        d->hideScrollbarTimer->stop();
    }

    d->hideScrollbarTimer = new QTimer();
    connect(d->hideScrollbarTimer, SIGNAL(timeout()), this, SLOT(hideScrollbar()));
    d->hideScrollbarTimer->start(d->hideScrollbarDuration);
}

DWIDGET_END_NAMESPACE
