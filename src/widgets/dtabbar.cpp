/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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
#include "dtabbar.h"
#include "dobject_p.h"

#include <QPainter>
#include <QApplication>
#include <QMouseEvent>
#include <QStyleOption>
#include <QStylePainter>
#include <QHBoxLayout>
#include <QDrag>
#include <QMimeData>
#include <QDragMoveEvent>
#include <QDebug>

#include <private/qtabbar_p.h>

DWIDGET_BEGIN_NAMESPACE

inline static bool verticalTabs(QTabBar::Shape shape)
{
    return shape == QTabBar::RoundedWest
           || shape == QTabBar::RoundedEast
           || shape == QTabBar::TriangularWest
           || shape == QTabBar::TriangularEast;
}

class DMovableTabWidget : public QWidget
{
public:
    explicit DMovableTabWidget(QWidget *parent = Q_NULLPTR);
    void setPixmap(const QPixmap &pixmap);

protected:
    void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;

private:
    QPixmap m_pixmap;
};

DMovableTabWidget::DMovableTabWidget(QWidget *parent)
    : QWidget(parent)
{

}

void DMovableTabWidget::setPixmap(const QPixmap &pixmap)
{
    m_pixmap = pixmap;
    update();
}

void DMovableTabWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    QPainter pa(this);

    pa.drawPixmap(0, 0, m_pixmap);
}

class DTabBarPrivate : public DObjectPrivate
{
    D_DECLARE_PUBLIC(DTabBar)

public:
    DTabBarPrivate(DTabBar* qq)
        : DObjectPrivate(qq) {
        addButton = new QToolButton(qq);
        addButton->setIcon(QIcon(":/images/light/images/spinner_increase_normal.png"));

        qq->connect(addButton, &QToolButton::clicked,
                    qq, &DTabBar::tabAddRequested);
        qq->connect(qq, &DTabBar::tabMoved, qq, [this] (int from, int to) {
            tabMinimumSize.move(from, to);
            tabMaximumSize.move(from, to);
        });
    }
    ~DTabBarPrivate()
    {

    }

    void _q_startDrag(int tabIndex);

    void setupMovableTab();
    void slide(int from, int to);
    void layoutTab(int index);
    void moveTab(int index, int offset);
    void moveTabFinished(int index);
    void layoutWidgets(int start = 0);

    QList<QSize> tabMinimumSize;
    QList<QSize> tabMaximumSize;
    bool visibleAddButton = true;
    QToolButton *addButton;
    QPointer<QDrag> drag;
};

void DTabBarPrivate::_q_startDrag(int tabIndex)
{
    Qt::DropAction action = drag->exec(Qt::MoveAction);

    if (action != Qt::MoveAction) {
        Q_EMIT q_func()->tabReleaseRequested(tabIndex);
    }
}

void DTabBarPrivate::setupMovableTab()
{
    D_Q(DTabBar);
    QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(q->d_ptr));

    if (!d->movingTab)
        d->movingTab = reinterpret_cast<QMovableTabWidget*>(new DMovableTabWidget(q));

    int taboverlap = q->style()->pixelMetric(QStyle::PM_TabBarTabOverlap, 0 ,q);
    QRect grabRect = q->tabRect(d->pressedIndex);
    grabRect.adjust(-taboverlap, 0, taboverlap, 0);

    QPixmap grabImage(grabRect.size() * q->devicePixelRatioF());
    grabImage.setDevicePixelRatio(q->devicePixelRatioF());
    grabImage.fill(Qt::transparent);
    QStylePainter p(&grabImage, q);
    p.initFrom(q);

    QStyleOptionTab tab;
    q->initStyleOption(&tab, d->pressedIndex);
    tab.rect.moveTopLeft(QPoint(taboverlap, 0));
    p.drawControl(QStyle::CE_TabBarTab, tab);
    p.end();

    reinterpret_cast<DMovableTabWidget*>(d->movingTab)->setPixmap(grabImage);
    d->movingTab->setGeometry(grabRect);
    d->movingTab->raise();

    // Re-arrange widget order to avoid overlaps
    if (d->tabList[d->pressedIndex].leftWidget)
        d->tabList[d->pressedIndex].leftWidget->raise();
    if (d->tabList[d->pressedIndex].rightWidget)
        d->tabList[d->pressedIndex].rightWidget->raise();
    if (d->leftB)
        d->leftB->raise();
    if (d->rightB)
        d->rightB->raise();
    d->movingTab->setVisible(true);

    drag = new QDrag(q);
    QMimeData *mime_data = new QMimeData();

    drag->setPixmap(grabImage);
    drag->setMimeData(mime_data);

    QMetaObject::invokeMethod(q, "_q_startDrag", Qt::QueuedConnection, Q_ARG(int, d->pressedIndex));
}

void DTabBarPrivate::slide(int from, int to)
{
    D_Q(DTabBar);
    QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(q->d_ptr));

    if (from == to
            || !d->validIndex(from)
            || !d->validIndex(to))
        return;
    bool vertical = verticalTabs(d->shape);
    int preLocation = vertical ? q->tabRect(from).y() : q->tabRect(from).x();
    q->setUpdatesEnabled(false);
    q->moveTab(from, to);
    q->setUpdatesEnabled(true);
    int postLocation = vertical ? q->tabRect(to).y() : q->tabRect(to).x();
    int length = postLocation - preLocation;

    QTabBarPrivate::Tab *tab = &d->tabList[to];

    d->tabList[to].dragOffset -= length;

    if (!d->isAnimated()) {
        moveTabFinished(d->tabList.indexOf(*tab));
        return;
    }

//    if (!tab->animation)
//        tab->animation = new QTabBarPrivate::Tab::TabBarAnimation(tab, d);
//    tab->animation->setStartValue(tab->dragOffset);
//    tab->animation->setEndValue(0);
//    tab->animation->setDuration(ANIMATION_DURATION);
//    tab->animation->start();
    moveTab(to, 0);
    moveTabFinished(to);
}

void DTabBarPrivate::layoutTab(int index)
{
    Q_ASSERT(index >= 0);
    D_Q(DTabBar);
    QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(q->d_ptr));

    QTabBarPrivate::Tab &tab = d->tabList[index];
    bool vertical = verticalTabs(d->shape);
    if (!(tab.leftWidget || tab.rightWidget))
        return;

    QStyleOptionTab opt;
    q->initStyleOption(&opt, index);
    if (tab.leftWidget) {
        QRect rect = q->style()->subElementRect(QStyle::SE_TabBarTabLeftButton, &opt, q);
        QPoint p = rect.topLeft();
        if ((index == d->pressedIndex) || d->paintWithOffsets) {
            if (vertical)
                p.setY(p.y() + d->tabList[index].dragOffset);
            else
                p.setX(p.x() + d->tabList[index].dragOffset);
        }
        tab.leftWidget->move(p);
    }
    if (tab.rightWidget) {
        QRect rect = q->style()->subElementRect(QStyle::SE_TabBarTabRightButton, &opt, q);
        QPoint p = rect.topLeft();
        if ((index == d->pressedIndex) || d->paintWithOffsets) {
            if (vertical)
                p.setY(p.y() + tab.dragOffset);
            else
                p.setX(p.x() + tab.dragOffset);
        }
        tab.rightWidget->move(p);
    }
}

void DTabBarPrivate::moveTab(int index, int offset)
{
    D_Q(DTabBar);
    QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(q->d_ptr));

    if (!d->validIndex(index))
        return;
    d->tabList[index].dragOffset = offset;
    layoutTab(index); // Make buttons follow tab
    q->update();
}

void DTabBarPrivate::moveTabFinished(int index)
{
    D_Q(DTabBar);
    QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(q->d_ptr));
    bool cleanup = (d->pressedIndex == index) || (d->pressedIndex == -1) || !d->validIndex(index);
    bool allAnimationsFinished = true;
#ifndef QT_NO_ANIMATION
    for(int i = 0; allAnimationsFinished && i < d->tabList.count(); ++i) {
        const QTabBarPrivate::Tab &t = d->tabList.at(i);
        if (t.animation && t.animation->state() == QAbstractAnimation::Running)
            allAnimationsFinished = false;
    }
#endif //QT_NO_ANIMATION
    if (allAnimationsFinished && cleanup) {
        if(d->movingTab)
            d->movingTab->setVisible(false); // We might not get a mouse release
        for (int i = 0; i < d->tabList.count(); ++i) {
            d->tabList[i].dragOffset = 0;
        }
        if (d->pressedIndex != -1 && d->movable) {
            d->pressedIndex = -1;
            d->dragInProgress = false;
            d->dragStartPosition = QPoint();
        }
        layoutWidgets();
    } else {
        if (!d->validIndex(index))
            return;
        d->tabList[index].dragOffset = 0;
    }
    q->update();
}

void DTabBarPrivate::layoutWidgets(int start)
{
    D_Q(DTabBar);
    for (int i = start; i < q->count(); ++i) {
        layoutTab(i);
    }
}

DTabBar::DTabBar(QWidget *parent)
    : QTabBar(parent)
    , DObject(*new DTabBarPrivate(this))
{
    setAcceptDrops(true);
    setChangeCurrentOnDrag(true);
}

void DTabBar::setTabMinimumSize(int index, const QSize &size)
{
    D_D(DTabBar);

    d->tabMinimumSize[index] = size;
}

void DTabBar::setTabMaximumSize(int index, const QSize &size)
{
    D_D(DTabBar);

    d->tabMaximumSize[index] = size;
}

QSize DTabBar::sizeHint() const
{
    QSize size = QTabBar::sizeHint();

    if (visibleAddButton()) {
        D_DC(DTabBar);

        size.setWidth(size.width() + d->addButton->sizeHint().width());
    }

    return size;
}

bool DTabBar::visibleAddButton() const
{
    D_DC(DTabBar);

    return d->visibleAddButton;
}

void DTabBar::setVisibleAddButton(bool visibleAddButton)
{
    D_D(DTabBar);

    d->visibleAddButton = visibleAddButton;
    d->addButton->setVisible(visibleAddButton);
}

void DTabBar::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d_ptr));

    QStyleOptionTabBarBase optTabBase;
    QTabBarPrivate::initStyleBaseOption(&optTabBase, this, size());

    QStylePainter p(this);
    int selected = -1;
    int cut = -1;
    bool rtl = optTabBase.direction == Qt::RightToLeft;
    bool vertical = verticalTabs(d->shape);
    QStyleOptionTab cutTab;
    selected = d->currentIndex;
    if (d->dragInProgress)
        selected = d->pressedIndex;

    for (int i = 0; i < d->tabList.count(); ++i)
         optTabBase.tabBarRect |= tabRect(i);

    optTabBase.selectedTabRect = tabRect(selected);

    if (d->drawBase)
        p.drawPrimitive(QStyle::PE_FrameTabBarBase, optTabBase);

    for (int i = 0; i < d->tabList.count(); ++i) {
        QStyleOptionTab tab;
        initStyleOption(&tab, i);
        if (d->paintWithOffsets && d->tabList[i].dragOffset != 0) {
            if (vertical) {
                tab.rect.moveTop(tab.rect.y() + d->tabList[i].dragOffset);
            } else {
                tab.rect.moveLeft(tab.rect.x() + d->tabList[i].dragOffset);
            }
        }
        if (!(tab.state & QStyle::State_Enabled)) {
            tab.palette.setCurrentColorGroup(QPalette::Disabled);
        }
        // If this tab is partially obscured, make a note of it so that we can pass the information
        // along when we draw the tear.
        if (((!vertical && (!rtl && tab.rect.left() < 0)) || (rtl && tab.rect.right() > width()))
            || (vertical && tab.rect.top() < 0)) {
            cut = i;
            cutTab = tab;
        }
        // Don't bother drawing a tab if the entire tab is outside of the visible tab bar.
        if ((!vertical && (tab.rect.right() < 0 || tab.rect.left() > width()))
            || (vertical && (tab.rect.bottom() < 0 || tab.rect.top() > height())))
            continue;

        optTabBase.tabBarRect |= tab.rect;
        if (i == selected)
            continue;

        p.drawControl(QStyle::CE_TabBarTab, tab);
    }

    // Draw the selected tab last to get it "on top"
    if (selected >= 0) {
        QStyleOptionTab tab;
        initStyleOption(&tab, selected);
        if (d->paintWithOffsets && d->tabList[selected].dragOffset != 0) {
            if (vertical)
                tab.rect.moveTop(tab.rect.y() + d->tabList[selected].dragOffset);
            else
                tab.rect.moveLeft(tab.rect.x() + d->tabList[selected].dragOffset);
        }
        if (!d->dragInProgress)
            p.drawControl(QStyle::CE_TabBarTab, tab);
        else {
            int taboverlap = style()->pixelMetric(QStyle::PM_TabBarTabOverlap, 0, this);
            d->movingTab->setGeometry(tab.rect.adjusted(-taboverlap, 0, taboverlap, 0));
        }
    }

    // Only draw the tear indicator if necessary. Most of the time we don't need too.
    if (d->leftB->isVisible() && cut >= 0) {
        cutTab.rect = rect();
        cutTab.rect = style()->subElementRect(QStyle::SE_TabBarTearIndicator, &cutTab, this);
        p.drawPrimitive(QStyle::PE_IndicatorTabTear, cutTab);
    }
}

void DTabBar::mouseMoveEvent(QMouseEvent *event)
{
    const QRect rect_hint(QPoint(0, 0), sizeHint());

    if (!rect_hint.contains(event->pos())) {
        event->ignore();

        return;
    }

    QTabBarPrivate *qd = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d_ptr));
    D_D(DTabBar);

    if (!qd->movable)
        return;

    // Be safe!
    if (qd->pressedIndex != -1
        && event->buttons() == Qt::NoButton)
        d->moveTabFinished(qd->pressedIndex);

    // Start drag
    if (!qd->dragInProgress && qd->pressedIndex != -1) {
        if ((event->pos() - qd->dragStartPosition).manhattanLength() > QApplication::startDragDistance()) {
            qd->dragInProgress = true;
            d->setupMovableTab();
        }
    }

    int offset = (event->pos() - qd->dragStartPosition).manhattanLength();

    if (event->buttons() == Qt::LeftButton
        && offset > QApplication::startDragDistance()
        && qd->validIndex(qd->pressedIndex)) {
        bool vertical = verticalTabs(qd->shape);
        int dragDistance;
        if (vertical) {
            dragDistance = (event->pos().y() - qd->dragStartPosition.y());
        } else {
            dragDistance = (event->pos().x() - qd->dragStartPosition.x());
        }
        qd->tabList[qd->pressedIndex].dragOffset = dragDistance;

        QRect startingRect = tabRect(qd->pressedIndex);
        if (vertical)
            startingRect.moveTop(startingRect.y() + dragDistance);
        else
            startingRect.moveLeft(startingRect.x() + dragDistance);

        int overIndex;
        if (dragDistance < 0)
            overIndex = tabAt(startingRect.topLeft());
        else
            overIndex = tabAt(startingRect.topRight());

        if (overIndex != qd->pressedIndex && overIndex != -1) {
            int offset = 1;
            if (isRightToLeft() && !vertical)
                offset *= -1;
            if (dragDistance < 0) {
                dragDistance *= -1;
                offset *= -1;
            }
            for (int i = qd->pressedIndex;
                 offset > 0 ? i < overIndex : i > overIndex;
                 i += offset) {
                QRect overIndexRect = tabRect(overIndex);
                int needsToBeOver = (vertical ? overIndexRect.height() : overIndexRect.width()) / 2;
                if (dragDistance > needsToBeOver)
                    d->slide(i + offset, qd->pressedIndex);
            }
        }
        // Buttons needs to follow the dragged tab
        d->layoutTab(qd->pressedIndex);

        update();
    }
}

void DTabBar::resizeEvent(QResizeEvent *e)
{
    D_D(DTabBar);

    d->addButton->move(d->addButton->x(), (height() - d->addButton->height()) / 2);

    QTabBar::resizeEvent(e);
}

void DTabBar::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->source() == this) {
        e->acceptProposedAction();

        QMouseEvent event(QEvent::MouseMove, e->posF(),
                          Qt::LeftButton, e->mouseButtons(),
                          e->keyboardModifiers());

        mouseMoveEvent(&event);
    }
}

void DTabBar::dragMoveEvent(QDragMoveEvent *e)
{
    if (e->source() == this) {
        e->acceptProposedAction();

        QMouseEvent event(QEvent::MouseMove, e->posF(),
                          Qt::LeftButton, e->mouseButtons(),
                          e->keyboardModifiers());

        mouseMoveEvent(&event);
    }
}

void DTabBar::dropEvent(QDropEvent *e)
{
    if (e->source() == this) {
        e->acceptProposedAction();

        QMouseEvent event(QEvent::MouseButtonRelease, e->posF(),
                          Qt::LeftButton, e->mouseButtons(),
                          e->keyboardModifiers());

        mouseReleaseEvent(&event);
    }
}

QSize DTabBar::tabSizeHint(int index) const
{
    D_DC(DTabBar);

    QSize size = QTabBar::tabSizeHint(index);

    if (index >= d->tabMaximumSize.count())
        return size;

    const QSize &min = d->tabMinimumSize.at(index);
    const QSize &max = d->tabMaximumSize.at(index);

    size.setWidth(qMax(size.width(), min.width()));
    size.setHeight(qMax(size.height(), min.height()));

    if (max.isValid()) {
        size.setWidth(qMin(size.width(), max.width()));
        size.setHeight(qMin(size.height(), max.height()));
    }

    return size;
}

QSize DTabBar::minimumTabSizeHint(int index) const
{
    D_DC(DTabBar);

    const QSize &min = d->tabMinimumSize.value(index);

    if (min.isValid())
        return min;

    return QTabBar::tabSizeHint(index);
}

QSize DTabBar::maximumTabSizeHint(int index) const
{
    D_DC(DTabBar);

    const QSize &max = d->tabMaximumSize.value(index);

    if (max.isValid())
        return max;

    return QTabBar::tabSizeHint(index);
}

void DTabBar::tabInserted(int index)
{
    D_D(DTabBar);

    d->tabMaximumSize.insert(index, QSize());
    d->tabMinimumSize.insert(index, QSize());

    QTabBar::tabInserted(index);
}

void DTabBar::tabRemoved(int index)
{
    D_D(DTabBar);

    d->tabMaximumSize.removeAt(index);
    d->tabMinimumSize.removeAt(index);

    QTabBar::tabInserted(index);
}

void DTabBar::tabLayoutChange()
{
    QTabBar::tabLayoutChange();

    D_D(DTabBar);

    const QSize &size = this->size();
    const QSize &size_hint = QTabBar::sizeHint();

    d->addButton->move(qMin(size_hint.width(), size.width() - d->addButton->width()),
                       (size.height() - d->addButton->height()) / 2);
}

DWIDGET_END_NAMESPACE

#include "moc_dtabbar.cpp"
