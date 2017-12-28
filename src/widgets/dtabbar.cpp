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
#include "dimagebutton.h"
#include "dthememanager.h"

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

class DTabBarPrivate : public QTabBar, public DObjectPrivate
{
    Q_OBJECT
    D_DECLARE_PUBLIC(DTabBar)

public:
    explicit DTabBarPrivate(DTabBar* qq)
      : QTabBar(qq)
      , DObjectPrivate(qq) {
        addButton = new DImageButton(qq);
        addButton->setObjectName("AddButton");
        addButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        connect(addButton, &DImageButton::clicked,
                qq, &DTabBar::tabAddRequested);
        connect(this, &QTabBar::tabMoved, this, [this] (int from, int to) {
            tabMinimumSize.move(from, to);
            tabMaximumSize.move(from, to);
        });

        setAcceptDrops(true);
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        setChangeCurrentOnDrag(true);

        QHBoxLayout *layout = new QHBoxLayout(qq);

        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        layout->addWidget(this, Qt::AlignLeft);
        layout->addWidget(addButton, Qt::AlignVCenter);
        layout->addStretch();

        qq->setFocusProxy(this);
    }

    QSize minimumSizeHint() const override;

    void paintEvent(QPaintEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void dragEnterEvent(QDragEnterEvent *e) override;
    void dragMoveEvent(QDragMoveEvent *e) override;
    void dropEvent(QDropEvent *e) override;
    void showEvent(QShowEvent *e) override;

    QSize tabSizeHint(int index) const override;
    QSize minimumTabSizeHint(int index) const override;
    virtual QSize maximumTabSizeHint(int index) const;

    void tabInserted(int index) override;
    void tabRemoved(int index) override;

    Q_SLOT void startDrag(int tabIndex);

    void setupMovableTab();
    void slide(int from, int to);
    void layoutTab(int index);
    void moveTabFinished(int index);
    void layoutWidgets(int start = 0);

    QList<QSize> tabMinimumSize;
    QList<QSize> tabMaximumSize;
    bool visibleAddButton = true;
    DImageButton *addButton;
    QPointer<QDrag> drag;
};

void DTabBarPrivate::startDrag(int tabIndex)
{
    Qt::DropAction action = drag->exec(Qt::MoveAction);

    if (action != Qt::MoveAction) {
        Q_EMIT q_func()->tabReleaseRequested(tabIndex);
    }
}

void DTabBarPrivate::setupMovableTab()
{
    QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d_ptr));

    if (!d->movingTab)
        d->movingTab = reinterpret_cast<QMovableTabWidget*>(new DMovableTabWidget(this));

    int taboverlap = style()->pixelMetric(QStyle::PM_TabBarTabOverlap, 0, this);
    QRect grabRect = tabRect(d->pressedIndex);
    grabRect.adjust(-taboverlap, 0, taboverlap, 0);

    QPixmap grabImage(grabRect.size() * devicePixelRatioF());
    grabImage.setDevicePixelRatio(devicePixelRatioF());
    grabImage.fill(Qt::transparent);
    QStylePainter p(&grabImage, this);
    p.initFrom(this);

    QStyleOptionTab tab;
    initStyleOption(&tab, d->pressedIndex);
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

    drag = new QDrag(this);
    QMimeData *mime_data = new QMimeData();

    drag->setPixmap(grabImage);
    drag->setMimeData(mime_data);

    QMetaObject::invokeMethod(this, "startDrag", Qt::QueuedConnection, Q_ARG(int, d->pressedIndex));
}

void DTabBarPrivate::slide(int from, int to)
{
    QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d_ptr));

    if (from == to
            || !d->validIndex(from)
            || !d->validIndex(to))
        return;
    bool vertical = verticalTabs(d->shape);
    int preLocation = vertical ? tabRect(from).y() : tabRect(from).x();
    setUpdatesEnabled(false);
    moveTab(from, to);
    setUpdatesEnabled(true);
    int postLocation = vertical ? tabRect(to).y() : tabRect(to).x();
    int length = postLocation - preLocation;

    d->tabList[to].dragOffset -= length;

    if (!d->isAnimated()) {
        moveTabFinished(to);
        return;
    }

//    if (!tab->animation)
//        tab->animation = new QTabBarPrivate::Tab::TabBarAnimation(tab, d);
//    tab->animation->setStartValue(tab->dragOffset);
//    tab->animation->setEndValue(0);
//    tab->animation->setDuration(ANIMATION_DURATION);
//    tab->animation->start();

    moveTabFinished(to);
}

void DTabBarPrivate::layoutTab(int index)
{
    Q_ASSERT(index >= 0);
    QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d_ptr));

    QTabBarPrivate::Tab &tab = d->tabList[index];
    bool vertical = verticalTabs(d->shape);
    if (!(tab.leftWidget || tab.rightWidget))
        return;

    QStyleOptionTab opt;
    initStyleOption(&opt, index);
    if (tab.leftWidget) {
        QRect rect = style()->subElementRect(QStyle::SE_TabBarTabLeftButton, &opt, this);
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
        QRect rect = style()->subElementRect(QStyle::SE_TabBarTabRightButton, &opt, this);
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

void DTabBarPrivate::moveTabFinished(int index)
{
    QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d_ptr));
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
    update();
}

void DTabBarPrivate::layoutWidgets(int start)
{
    for (int i = start; i < count(); ++i) {
        layoutTab(i);
    }
}

QSize DTabBarPrivate::minimumSizeHint() const
{
    const QSize &hint = sizeHint();
    QSize size = QTabBar::minimumSizeHint();

    size.setWidth(qMin(size.width(), hint.width()));
    size.setHeight(qMin(size.height(), hint.height()));

    return size;
}

void DTabBarPrivate::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    D_Q(DTabBar);

    QPainter pa(this);

    pa.fillRect(rect(), Qt::red);

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

        if (!q->paintTab(&p, i, tab))
            p.drawControl(QStyle::CE_TabBarTab, tab);
    }

    // Draw the selected tab last to get it "on top"
    if (selected >= 0) {
        QStyleOptionTab tab;
        initStyleOption(&tab, selected);
        if (d->paintWithOffsets && d->tabList[selected].dragOffset != 0) {
            if (vertical) {
                tab.rect.moveTop(tab.rect.y() + d->tabList[selected].dragOffset);
            } else {
                tab.rect.moveLeft(tab.rect.x() + d->tabList[selected].dragOffset);
            }
        }
        if (!d->dragInProgress) {
            if (!q->paintTab(&p, selected, tab))
                p.drawControl(QStyle::CE_TabBarTab, tab);
        } else {
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

void DTabBarPrivate::mouseMoveEvent(QMouseEvent *event)
{
    QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d_ptr));

    if (!d->movable)
        return;

    // Be safe!
    if (d->pressedIndex != -1
        && event->buttons() == Qt::NoButton)
        moveTabFinished(d->pressedIndex);

    // Start drag
    if (!d->dragInProgress && d->pressedIndex != -1) {
        if ((event->pos() - d->dragStartPosition).manhattanLength() > QApplication::startDragDistance()) {
            d->dragInProgress = true;
            setupMovableTab();
        }
    }

    int offset = (event->pos() - d->dragStartPosition).manhattanLength();

    if (event->buttons() == Qt::LeftButton
        && offset > QApplication::startDragDistance()
        && d->validIndex(d->pressedIndex)) {

        bool vertical = verticalTabs(d->shape);
        int dragDistance;
        if (vertical) {
            dragDistance = (event->pos().y() - d->dragStartPosition.y());
        } else {
            dragDistance = (event->pos().x() - d->dragStartPosition.x());
        }

        if (!(d->pressedIndex == 0 && dragDistance < 0)
                && !(d->pressedIndex == d->tabList.count() - 1 && dragDistance > 0)) {
            d->tabList[d->pressedIndex].dragOffset = dragDistance;
        } else {
            d->tabList[d->pressedIndex].dragOffset = 0;
        }

        QRect startingRect = tabRect(d->pressedIndex);
        if (vertical)
            startingRect.moveTop(startingRect.y() + dragDistance);
        else
            startingRect.moveLeft(startingRect.x() + dragDistance);

        int overIndex;
        if (dragDistance < 0)
            overIndex = tabAt(startingRect.topLeft());
        else
            overIndex = tabAt(startingRect.topRight());

        if (overIndex != d->pressedIndex && overIndex != -1) {
            int offset = 1;
            if (isRightToLeft() && !vertical)
                offset *= -1;
            if (dragDistance < 0) {
                dragDistance *= -1;
                offset *= -1;
            }
            for (int i = d->pressedIndex;
                 offset > 0 ? i < overIndex : i > overIndex;
                 i += offset) {
                QRect overIndexRect = tabRect(overIndex);
                int needsToBeOver = (vertical ? overIndexRect.height() : overIndexRect.width()) / 2;
                if (dragDistance > needsToBeOver) {
                    slide(i + offset, d->pressedIndex);
                }
            }
        }

        // Buttons needs to follow the dragged tab
        layoutTab(d->pressedIndex);

        update();
    }
}

void DTabBarPrivate::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->source() == this) {
        e->acceptProposedAction();

        QMouseEvent event(QEvent::MouseMove, e->posF(),
                          Qt::LeftButton, e->mouseButtons(),
                          e->keyboardModifiers());

        mouseMoveEvent(&event);
    }
}

void DTabBarPrivate::dragMoveEvent(QDragMoveEvent *e)
{
    if (e->source() == this) {
        e->acceptProposedAction();

        QMouseEvent event(QEvent::MouseMove, e->posF(),
                          Qt::LeftButton, e->mouseButtons(),
                          e->keyboardModifiers());

        mouseMoveEvent(&event);
    }
}

void DTabBarPrivate::dropEvent(QDropEvent *e)
{
    if (e->source() == this) {
        e->acceptProposedAction();

        QMouseEvent event(QEvent::MouseButtonRelease, e->posF(),
                          Qt::LeftButton, e->mouseButtons(),
                          e->keyboardModifiers());

        mouseReleaseEvent(&event);
    }
}

void DTabBarPrivate::showEvent(QShowEvent *e)
{
    updateGeometry();

    QTabBar::showEvent(e);
}

QSize DTabBarPrivate::tabSizeHint(int index) const
{
    QSize size = QTabBar::tabSizeHint(index);

    if (index >= tabMaximumSize.count())
        return size;

    const QSize &min = tabMinimumSize.at(index);
    const QSize &max = tabMaximumSize.at(index);

    size.setWidth(qMax(size.width(), min.width()));
    size.setHeight(qMax(size.height(), min.height()));

    if (max.isValid()) {
        size.setWidth(qMin(size.width(), max.width()));
        size.setHeight(qMin(size.height(), max.height()));
    }

    return size;
}

QSize DTabBarPrivate::minimumTabSizeHint(int index) const
{
    const QSize &min = tabMinimumSize.value(index);

    if (min.isValid())
        return min;

    return QTabBar::minimumTabSizeHint(index);
}

QSize DTabBarPrivate::maximumTabSizeHint(int index) const
{
    const QSize &max = tabMaximumSize.value(index);

    if (max.isValid())
        return max;

    return QTabBar::tabSizeHint(index);
}

void DTabBarPrivate::tabInserted(int index)
{
    tabMaximumSize.insert(index, QSize());
    tabMinimumSize.insert(index, QSize());

    QTabBar::tabInserted(index);
}

void DTabBarPrivate::tabRemoved(int index)
{
    tabMaximumSize.removeAt(index);
    tabMinimumSize.removeAt(index);

    QTabBar::tabInserted(index);
}

DTabBar::DTabBar(QWidget *parent)
    : QWidget(parent)
    , DObject(*new DTabBarPrivate(this))
{
    D_THEME_INIT_WIDGET(DTabBar)
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

bool DTabBar::visibleAddButton() const
{
    D_DC(DTabBar);

    return d->visibleAddButton;
}

QTabBar::Shape DTabBar::shape() const
{
    return d_func()->shape();
}

void DTabBar::setShape(QTabBar::Shape shape)
{
    d_func()->setShape(shape);
}

int DTabBar::addTab(const QString &text)
{
    return d_func()->addTab(text);
}

int DTabBar::addTab(const QIcon &icon, const QString &text)
{
    return d_func()->addTab(icon, text);
}

int DTabBar::insertTab(int index, const QString &text)
{
    return d_func()->insertTab(index, text);
}

int DTabBar::insertTab(int index, const QIcon &icon, const QString &text)
{
    return insertTab(index, icon, text);
}

void DTabBar::removeTab(int index)
{
    d_func()->removeTab(index);
}

void DTabBar::moveTab(int from, int to)
{
    d_func()->moveTab(from, to);
}

bool DTabBar::isTabEnabled(int index) const
{
    return d_func()->isTabEnabled(index);
}

void DTabBar::setTabEnabled(int index, bool enable)
{
    d_func()->setTabEnabled(index, enable);
}

QString DTabBar::tabText(int index) const
{
    return d_func()->tabText(index);
}

void DTabBar::setTabText(int index, const QString &text)
{
    d_func()->setTabText(index, text);
}

QColor DTabBar::tabTextColor(int index) const
{
    return d_func()->tabTextColor(index);
}

void DTabBar::setTabTextColor(int index, const QColor &color)
{
    d_func()->setTabTextColor(index, color);
}

QIcon DTabBar::tabIcon(int index) const
{
    return d_func()->tabIcon(index);
}

void DTabBar::setTabIcon(int index, const QIcon &icon)
{
    d_func()->setTabIcon(index, icon);
}

Qt::TextElideMode DTabBar::elideMode() const
{
    return d_func()->elideMode();
}

void DTabBar::setElideMode(Qt::TextElideMode mode)
{
    d_func()->setElideMode(mode);
}

#ifndef QT_NO_TOOLTIP
void DTabBar::setTabToolTip(int index, const QString &tip)
{
    d_func()->setTabToolTip(index, tip);
}

QString DTabBar::tabToolTip(int index) const
{
    return d_func()->tabToolTip(index);
}
#endif

#ifndef QT_NO_WHATSTHIS
void DTabBar::setTabWhatsThis(int index, const QString &text)
{
    d_func()->setTabWhatsThis(index, text);
}

QString DTabBar::tabWhatsThis(int index) const
{
    return d_func()->tabWhatsThis(index);
}
#endif

void DTabBar::setTabData(int index, const QVariant &data)
{
    d_func()->setTabData(index, data);
}

QVariant DTabBar::tabData(int index) const
{
    return d_func()->tabData(index);
}

QRect DTabBar::tabRect(int index) const
{
    return d_func()->tabRect(index);
}

int DTabBar::tabAt(const QPoint &pos) const
{
    return d_func()->tabAt(pos);
}

int DTabBar::currentIndex() const
{
    return d_func()->currentIndex();
}

int DTabBar::count() const
{
    return d_func()->count();
}

void DTabBar::setDrawBase(bool drawTheBase)
{
    d_func()->setDrawBase(drawTheBase);
}

bool DTabBar::drawBase() const
{
    return d_func()->drawBase();
}

QSize DTabBar::iconSize() const
{
    return d_func()->iconSize();
}

void DTabBar::setIconSize(const QSize &size)
{
    d_func()->setIconSize(size);
}

bool DTabBar::usesScrollButtons() const
{
    return d_func()->usesScrollButtons();
}

void DTabBar::setUsesScrollButtons(bool useButtons)
{
    d_func()->setUsesScrollButtons(useButtons);
}

bool DTabBar::tabsClosable() const
{
    return d_func()->tabsClosable();
}

void DTabBar::setTabsClosable(bool closable)
{
    d_func()->setTabsClosable(closable);
}

void DTabBar::setTabButton(int index, QTabBar::ButtonPosition position, QWidget *widget)
{
    d_func()->setTabButton(index, position, widget);
}

QWidget *DTabBar::tabButton(int index, QTabBar::ButtonPosition position) const
{
    return d_func()->tabButton(index, position);
}

QTabBar::SelectionBehavior DTabBar::selectionBehaviorOnRemove() const
{
    return d_func()->selectionBehaviorOnRemove();
}

void DTabBar::setSelectionBehaviorOnRemove(QTabBar::SelectionBehavior behavior)
{
    d_func()->setSelectionBehaviorOnRemove(behavior);
}

bool DTabBar::expanding() const
{
    return d_func()->expanding();
}

void DTabBar::setExpanding(bool enabled)
{
    d_func()->setExpanding(enabled);
}

bool DTabBar::isMovable() const
{
    return d_func()->isMovable();
}

void DTabBar::setMovable(bool movable)
{
    d_func()->setMovable(movable);
}

bool DTabBar::documentMode() const
{
    return d_func()->documentMode();
}

void DTabBar::setDocumentMode(bool set)
{
    d_func()->setDocumentMode(set);
}

bool DTabBar::autoHide() const
{
    return d_func()->autoHide();
}

void DTabBar::setAutoHide(bool hide)
{
    d_func()->setAutoHide(hide);
}

bool DTabBar::changeCurrentOnDrag() const
{
    return d_func()->changeCurrentOnDrag();
}

void DTabBar::setChangeCurrentOnDrag(bool change)
{
    d_func()->setChangeCurrentOnDrag(change);
}

void DTabBar::setCurrentIndex(int index)
{
    d_func()->setCurrentIndex(index);
}

void DTabBar::setVisibleAddButton(bool visibleAddButton)
{
    D_D(DTabBar);

    d->visibleAddButton = visibleAddButton;
    d->addButton->setVisible(visibleAddButton);
}

bool DTabBar::paintTab(QPainter *painter, int index, const QStyleOptionTab &option)
{
    Q_UNUSED(painter)
    Q_UNUSED(index)
    Q_UNUSED(option)

    return false;
}

DTabBarPrivate *DTabBar::d_func()
{
    return dynamic_cast<DTabBarPrivate*>(d_d_ptr.data());
}

const DTabBarPrivate *DTabBar::d_func() const
{
    return dynamic_cast<const DTabBarPrivate*>(d_d_ptr.data());
}

DWIDGET_END_NAMESPACE

#include "dtabbar.moc"
