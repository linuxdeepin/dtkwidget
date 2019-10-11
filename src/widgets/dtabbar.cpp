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
#include <QMouseEvent>
#include <QStyleOption>
#include <QStylePainter>
#include <QHBoxLayout>
#include <QDrag>
#include <QMimeData>
#include <QDragMoveEvent>
#include <QTimer>
#include <QDebug>

#include <private/qtabbar_p.h>
#define private public
#define protected public
#include <private/qdnd_p.h>
#include <private/qsimpledrag_p.h>
#include <private/qshapedpixmapdndwindow_p.h>
#undef private
#undef protected

#include "dapplication.h"
#include "diconbutton.h"

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
        startDragDistance = qApp->startDragDistance();
        maskColor = flashColor = QColor(0, 0, 255, 125);

        addButton = new DIconButton(DStyle::SP_IncreaseElement, qq);
        addButton->setObjectName("AddButton");

        connect(addButton, &DIconButton::clicked,
                qq, &DTabBar::tabAddRequested);
        connect(this, &QTabBar::tabMoved, this, [this] (int from, int to) {
            tabMinimumSize.move(from, to);
            tabMaximumSize.move(from, to);

            if (dd()->validIndex(ghostTabIndex)) {
                if (from == ghostTabIndex)
                    ghostTabIndex = to;
                else if (to == ghostTabIndex)
                    ghostTabIndex = from;
            }
        });

        setAcceptDrops(true);
        setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);

        QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d_ptr));

        leftScrollButton = new QToolButton(qq);
        rightScrollButton = new QToolButton(qq);

        leftScrollButton->setVisible(d->leftB->isVisible());
        leftScrollButton->setAutoRepeat(true);
        leftScrollButton->setArrowType(Qt::LeftArrow);
        leftScrollButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        rightScrollButton->setVisible(d->rightB->isVisible());
        rightScrollButton->setAutoRepeat(true);
        rightScrollButton->setArrowType(Qt::RightArrow);
        rightScrollButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

        d->leftB->setFixedSize(0, 0);
        d->leftB->installEventFilter(this);
        d->rightB->setFixedSize(0, 0);
        d->rightB->installEventFilter(this);

        connect(leftScrollButton, &QToolButton::clicked, d->leftB, &QToolButton::click);
        connect(rightScrollButton, &QToolButton::clicked, d->rightB, &QToolButton::click);

        QHBoxLayout *layout = new QHBoxLayout(qq);

        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        layout->addWidget(leftScrollButton);
        layout->addWidget(rightScrollButton);
        layout->addWidget(this);
        layout->addWidget(addButton);
        layout->addStretch();

        qq->setFocusProxy(this);

        connect(this, &DTabBarPrivate::currentChanged, qq, &DTabBar::currentChanged);
        connect(this, &DTabBarPrivate::tabCloseRequested, qq, &DTabBar::tabCloseRequested);
        connect(this, &DTabBarPrivate::tabMoved, qq, &DTabBar::tabMoved);
        connect(this, &DTabBarPrivate::tabBarClicked, qq, &DTabBar::tabBarClicked);
        connect(this, &DTabBarPrivate::tabBarDoubleClicked, qq, &DTabBar::tabBarDoubleClicked);
    }

    void moveTabOffset(int index, int offset)
    {
        QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d_ptr));

        if (!d->validIndex(index))
            return;
        d->tabList[index].dragOffset = offset;
        layoutTab(index); // Make buttons follow tab
        update();
    }

    struct TabBarAnimation : public QVariantAnimation {
        TabBarAnimation(QTabBarPrivate::Tab *t,
                        QTabBarPrivate *_priv,
                        DTabBarPrivate *_dpriv)
            : tab(t), priv(_priv), dpriv(_dpriv)
        { setEasingCurve(QEasingCurve::InOutQuad); }

        void updateCurrentValue(const QVariant &current) Q_DECL_OVERRIDE
        {
            dpriv->moveTabOffset(priv->tabList.indexOf(*tab), current.toInt());
        }

        void updateState(State, State newState) Q_DECL_OVERRIDE
        {
            if (newState == Stopped) dpriv->moveTabFinished(priv->tabList.indexOf(*tab));
        }

    private:
        //these are needed for the callbacks
        QTabBarPrivate::Tab *tab;
        QTabBarPrivate *priv;
        DTabBarPrivate *dpriv;
    };

    bool eventFilter(QObject *watched, QEvent *event) override;

    QSize minimumSizeHint() const override;

    void paintEvent(QPaintEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void dragEnterEvent(QDragEnterEvent *e) override;
    void dragLeaveEvent(QDragLeaveEvent *e) override;
    void dragMoveEvent(QDragMoveEvent *e) override;
    void dropEvent(QDropEvent *e) override;
    void showEvent(QShowEvent *e) override;

    QSize tabSizeHint(int index) const override;
    QSize minimumTabSizeHint(int index) const override;

    void tabInserted(int index) override;
    void tabRemoved(int index) override;
    void tabLayoutChange() override;

    void initStyleOption(QStyleOptionTab *option, int tabIndex) const;

     QTabBarPrivate *dd() const;

    Q_SLOT void startDrag(int tabIndex);

    void setupMovableTab();
    void updateMoveingTabPosition(const QPoint &mouse);
    void setupDragableTab();
    void slide(int from, int to);
    void layoutTab(int index);
    void moveTabFinished(int index);
    void layoutWidgets(int start = 0);
    void makeVisible(int index);
    void autoScrollTabs(const QPoint &mouse);
    void stopAutoScrollTabs();
    void ensureScrollTabsAnimation();

    void startTabFlash();

    void setDragingFromOther(bool v);
    int tabInsertIndexFromMouse(QPoint pos);

    void startMove(int index);
    void stopMove();

    QList<QSize> tabMinimumSize;
    QList<QSize> tabMaximumSize;
    bool visibleAddButton = true;
    DIconButton *addButton;
    QPointer<QDrag> drag;
    bool dragable = false;
    int startDragDistance;
    // 有从其它地方drag过来的标签页需要处理
    bool dragingFromOther = false;
    // 记录当前drag过来的对象是否可以当做新标签页插入
    bool canInsertFromDrag = false;
    // 为true忽略drag move事件
    bool ignoreDragEvent = false;

    QColor maskColor;
    QColor flashColor;
    // 要闪动绘制的Tab
    qreal opacityOnFlash = 1;
    int flashTabIndex = -1;

    QToolButton *leftScrollButton;
    QToolButton *rightScrollButton;

    class FullWidget : public QWidget {
    public:
        explicit FullWidget(QWidget *parent = 0)
            : QWidget(parent) {}

        void paintEvent(QPaintEvent *) override {
            QPainter pa(this);

            pa.fillRect(rect(), color);
        }

        QColor color;
    } *topFullWidget = nullptr;

    QVariantAnimation *scrollTabAnimation = nullptr;
    // 备份启动tab move时的QTabBarPrivate中的这两个值
    int scrollOffset;
    QPoint dragStartPosition;

    int ghostTabIndex = -1;
};

void DTabBarPrivate::startDrag(int tabIndex)
{
    Qt::DropAction action = drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction);

    Q_EMIT q_func()->dragEnd(action);

    if (action == Qt::IgnoreAction) {
        Q_EMIT q_func()->tabReleaseRequested(tabIndex);
    } else if (drag->target() != this) {
        if (DTabBarPrivate *tbp = qobject_cast<DTabBarPrivate*>(drag->target()))
            Q_EMIT q_func()->tabDroped(tabIndex, action, tbp->q_func());
        else
            Q_EMIT q_func()->tabDroped(tabIndex, action, drag->target());
    }

    drag->setProperty("_d_DTabBarPrivate_drity", true);

    QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d_ptr));

    // Be safe!
    if (d->dragInProgress && d->pressedIndex != -1) {
        d->hoverRect = QRect();
        moveTabFinished(d->pressedIndex);
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
    QPainter p(&grabImage);
    p.initFrom(this);

    QStyleOptionTab tab;
    initStyleOption(&tab, d->pressedIndex);
    tab.rect.moveTopLeft(QPoint(taboverlap, 0));
    q_func()->paintTab(&p, d->pressedIndex, tab);

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
}

void DTabBarPrivate::updateMoveingTabPosition(const QPoint &mouse)
{
    QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d_ptr));

    bool vertical = verticalTabs(d->shape);
    int dragDistance;
    if (vertical) {
        dragDistance = (mouse.y() - d->dragStartPosition.y());
    } else {
        dragDistance = (mouse.x() - d->dragStartPosition.x());
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

void DTabBarPrivate::setupDragableTab()
{
    if (!dragable)
        return;

    D_Q(DTabBar);

    QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d_ptr));
    QStyleOptionTab opt;
    initStyleOption(&opt, d->pressedIndex);

    QMimeData *mime_data = q->createMimeDataFromTab(d->pressedIndex, opt);

    if (!mime_data)
        return;

    if (drag)
        drag->deleteLater();

    drag = new QDrag(this);

    QPoint hotspot = drag->hotSpot();
    const QPixmap &grabImage = q->createDragPixmapFromTab(d->pressedIndex, opt, &hotspot);

    drag->setPixmap(grabImage);
    drag->setMimeData(mime_data);
    drag->setHotSpot(hotspot);

    qRegisterMetaType<Qt::DropAction>();

    QMetaObject::invokeMethod(this, "startDrag", Qt::QueuedConnection, Q_ARG(int, d->pressedIndex));
    QMetaObject::invokeMethod(q, "dragStarted", Qt::QueuedConnection);
    QMetaObject::invokeMethod(q, "dragActionChanged", Qt::QueuedConnection, Q_ARG(Qt::DropAction, Qt::IgnoreAction));

    connect(drag, &QDrag::actionChanged, q, &DTabBar::dragActionChanged);
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

    QTabBarPrivate::Tab *tab = &d->tabList[to];

    if (!tab->animation)
        tab->animation = reinterpret_cast<QTabBarPrivate::Tab::TabBarAnimation*>(new TabBarAnimation(tab, d, this));
    tab->animation->setStartValue(tab->dragOffset);
    tab->animation->setEndValue(0);
    tab->animation->setEasingCurve(QEasingCurve::OutCubic);
    tab->animation->setDuration(ANIMATION_DURATION);
    tab->animation->start();
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

void DTabBarPrivate::makeVisible(int index)
{
    QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d_ptr));

    if (!d->validIndex(index))
        return;

    const QRect tabRect = d->tabList.at(index).rect;
    const int oldScrollOffset = d->scrollOffset;
    const bool horiz = !verticalTabs(d->shape);
    const int extra_width = 2 * qMax(style()->pixelMetric(QStyle::PM_TabBarScrollButtonWidth, 0, this),
                                     QApplication::globalStrut().width());
    const int available = (horiz ? width() : height()) - extra_width;
    const int start = horiz ? tabRect.left() : tabRect.top();
    const int end = horiz ? tabRect.right() : tabRect.bottom();
    if (start < d->scrollOffset) // too far left
        d->scrollOffset = start - (index ? 8 : 0);
    else if (end > d->scrollOffset + available) // too far right
        d->scrollOffset = end - available + 1;

    d->leftB->setEnabled(d->scrollOffset > 0);
    const int last = horiz ? d->tabList.last().rect.right() : d->tabList.last().rect.bottom();
    d->rightB->setEnabled(last - d->scrollOffset >= available);
    if (oldScrollOffset != d->scrollOffset) {
        layoutWidgets();
    }
}

void DTabBarPrivate::autoScrollTabs(const QPoint &mouse)
{
    const QRect &rect = this->rect();

    if (!rect.contains(mouse)) {
        return stopAutoScrollTabs();
    }

    QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d_ptr));
    bool vertical = verticalTabs(d->shape);
    const QSize &size_hint = this->sizeHint();
    int scroll_distance = qMin(50, vertical ? rect.height() / 3 : rect.width() / 3);

    int start = 0;
    int end = 0;
    int scroll_end = 0;
    int scroll_speed = 0;

    if (vertical) {
        end = size_hint.height() - rect.height();

        if (mouse.y() < scroll_distance) {
            if (d->scrollOffset <= start)
                return;

            // to up
            scroll_end = start;
            scroll_speed = mouse.y();
        } else if (mouse.y() > rect.height() - scroll_distance) {
            if (d->scrollOffset >= end)
                return;

            // to down
            scroll_end = end;
            scroll_speed = rect.height() - mouse.y();
        } else {
            return stopAutoScrollTabs();
        }
    } else {
        end = size_hint.width() - rect.width();

        if (mouse.x() < scroll_distance) {
            if (d->scrollOffset <= start)
                return;

            // to left
            scroll_end = start;
            scroll_speed = mouse.x();
        } else if (mouse.x() > rect.width() - scroll_distance) {
            if (d->scrollOffset >= end)
                return;

            // to right
            scroll_end = end;
            scroll_speed = rect.width() - mouse.x();
        } else {
            return stopAutoScrollTabs();
        }
    }

    ensureScrollTabsAnimation();

    if (scrollTabAnimation->state() == QVariantAnimation::Running)
        scrollTabAnimation->stop();

    scrollTabAnimation->setDuration(qreal(qMax(qAbs(scroll_speed), 10)) / scroll_distance * (qAbs(scroll_end - d->scrollOffset) / 150) * 1000);
    scrollTabAnimation->setStartValue(d->scrollOffset);
    scrollTabAnimation->setEndValue(scroll_end);
    scrollTabAnimation->start();
}

void DTabBarPrivate::stopAutoScrollTabs()
{
    if (!scrollTabAnimation)
        return;

    scrollTabAnimation->stop();
    scrollTabAnimation->deleteLater();
    scrollTabAnimation = nullptr;
}

void DTabBarPrivate::ensureScrollTabsAnimation()
{
    if (scrollTabAnimation) {
        return;
    }

    scrollTabAnimation = new QVariantAnimation(this);
    connect(scrollTabAnimation, &QVariantAnimation::valueChanged,
            this, [this] (const QVariant &value) {
        if (scrollTabAnimation->state() != QVariantAnimation::Running)
            return;
        bool ok = false;
        int v = value.toInt(&ok);

        if (!ok)
            return;

        QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d_ptr));

        d->scrollOffset = v;

        if (d->dragInProgress && d->validIndex(d->pressedIndex)) {
            if (verticalTabs(d->shape)) {
                d->dragStartPosition.setY(dragStartPosition.y() + scrollOffset - v);
            } else {
                d->dragStartPosition.setX(dragStartPosition.x() + scrollOffset - v);
            }

            const QPoint bak_dragStartPosition = d->dragStartPosition;

            updateMoveingTabPosition(mapFromGlobal(QCursor::pos()));

            if (bak_dragStartPosition != d->dragStartPosition) {
                dragStartPosition = d->dragStartPosition;
                scrollOffset = d->scrollOffset;
            }
        } else {
            layoutWidgets();
            update();
        }
    });

    connect(scrollTabAnimation, &QVariantAnimation::finished,
            this, [this] {
        QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d_ptr));

        d->leftB->setEnabled(d->scrollOffset > 0);
        d->rightB->setEnabled(d->scrollOffset < sizeHint().width() - width());
    });
}

void DTabBarPrivate::startTabFlash()
{
    QVariantAnimation *animation = new QVariantAnimation(this);

    animation->setStartValue(1.0);
    animation->setEndValue(0.0);
    animation->setDuration(500);
    animation->setLoopCount(3);

    connect(animation, &QVariantAnimation::valueChanged, this, [this] (const QVariant &v) {
        opacityOnFlash = v.toReal();
        update();
    });
    connect(animation, &QVariantAnimation::finished, this, [this, animation] {
        animation->deleteLater();
        flashTabIndex = -1;
    });
    connect(animation, &QVariantAnimation::currentLoopChanged, this, [animation] (int loop) {
        if (loop % 2) {
            animation->setStartValue(1.0);
            animation->setEndValue(0.0);
        } else {
            animation->setStartValue(0.0);
            animation->setEndValue(1.0);
        }
    });

    animation->start();
}

void DTabBarPrivate::setDragingFromOther(bool v)
{
    if (v == dragingFromOther)
        return;

    dragingFromOther = v;

    if (!v) {
        if (topFullWidget) {
            topFullWidget->hide();
            topFullWidget->deleteLater();
            topFullWidget = nullptr;
        }

        return;
    }

    D_Q(DTabBar);

    if (!topFullWidget)
        topFullWidget = new FullWidget(q);

    topFullWidget->color = maskColor;
    topFullWidget->resize(q->size());
    topFullWidget->show();
    topFullWidget->raise();
}

int DTabBarPrivate::tabInsertIndexFromMouse(QPoint pos)
{
    if (pos.y() == height())
        pos.setY(pos.y() - 1);

    if (pos.x() == width())
        pos.setX(pos.x() - 1);

    int current = tabAt(pos);

    QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d_ptr));
    bool vertical = verticalTabs(this->shape());

    if (!d->validIndex(current)){
        if (vertical)
            current = pos.y() >= height() ? count() - 1 : 0;
        else
            current = pos.x() >= width() ? count() - 1 : 0;
    }

    const QRect &current_rect = tabRect(current);
    const QPoint &center = current_rect.center();

    if (vertical) {
        return pos.y() <= center.y() ? current : current + 1;
    } else {
        return pos.x() <= center.x() ? current : current + 1;
    }
}

void DTabBarPrivate::startMove(int index)
{
    if (dd()->dragInProgress)
        return;

    dd()->pressedIndex = index;
    makeVisible(index);

    const QRect &index_rect = tabRect(index);

    dd()->dragStartPosition = index_rect.center();

    QVariantAnimation *mouse_animation = new QVariantAnimation(this);

    mouse_animation->setDuration(100);
    mouse_animation->setEasingCurve(QEasingCurve::OutSine);
    mouse_animation->setStartValue(QCursor::pos());
    mouse_animation->setEndValue(mapToGlobal(index_rect.center()));

    connect(mouse_animation, &QVariantAnimation::valueChanged, this, [] (const QVariant &value) {
        const QPoint pos = value.toPoint();

        QCursor::setPos(pos.x(), pos.y());
    });

    connect(mouse_animation, &QVariantAnimation::finished, this, [this, mouse_animation] {
        mouse_animation->deleteLater();
        ignoreDragEvent = false;
    });

    ignoreDragEvent = true;
    mouse_animation->start();
}

void DTabBarPrivate::stopMove()
{
    QMouseEvent event(QEvent::MouseButtonRelease, mapFromGlobal(QCursor::pos()),
                      Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    bool movable = isMovable();
    setMovable(true);
    mouseReleaseEvent(&event);
    setMovable(movable);
}

bool DTabBarPrivate::eventFilter(QObject *watched, QEvent *event)
{
    QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d_ptr));

    if (watched == d->leftB) {
        switch (event->type()) {
        case QEvent::Show:
            leftScrollButton->show();
            break;
        case QEvent::Hide:
            leftScrollButton->hide();
            break;
        case QEvent::EnabledChange:
            leftScrollButton->setEnabled(d->leftB->isEnabled());
            break;
        case QEvent::UpdateRequest:
            leftScrollButton->setArrowType(d->leftB->arrowType());
            break;
        default:
            break;
        }
    } else if (watched == d->rightB) {
        switch (event->type()) {
        case QEvent::Show:
            rightScrollButton->show();
            // ###(zccrs): 当主窗口从最大化还原后，此button的大小会被布局改为 0x40 （原因不明）
            QTimer::singleShot(100, rightScrollButton, [this] {
                rightScrollButton->resize(rightScrollButton->sizeHint().width(), rightScrollButton->height());
            });
            break;
        case QEvent::Hide:
            rightScrollButton->hide();
            break;
        case QEvent::EnabledChange:
            rightScrollButton->setEnabled(d->rightB->isEnabled());
            break;
        case QEvent::UpdateRequest:
            rightScrollButton->setArrowType(d->rightB->arrowType());
            break;
        default:
            break;
        }
    }

    return QTabBar::eventFilter(watched, event);
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

    QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d_ptr));

    QStyleOptionTabBarBase optTabBase;
    QTabBarPrivate::initStyleBaseOption(&optTabBase, this, size());

    QStylePainter p(this);
    int selected = -1;
//    int cut = -1;
//    bool rtl = optTabBase.direction == Qt::RightToLeft;
    bool vertical = verticalTabs(d->shape);
//    QStyleOptionTab cutTab;
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
        // 强制让文本居中
        tab.rightButtonSize = QSize();
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
//        if (((!vertical && (!rtl && tab.rect.left() < 0)) || (rtl && tab.rect.right() > width()))
//            || (vertical && tab.rect.top() < 0)) {
//            cut = i;
//            cutTab = tab;
//        }
        // Don't bother drawing a tab if the entire tab is outside of the visible tab bar.
        if ((!vertical && (tab.rect.right() < 0 || tab.rect.left() > width()))
            || (vertical && (tab.rect.bottom() < 0 || tab.rect.top() > height())))
            continue;

        optTabBase.tabBarRect |= tab.rect;
        if (i == selected)
            continue;

        q->paintTab(&p, i, tab);

        if (i == flashTabIndex) {
            p.setOpacity(opacityOnFlash);
            p.fillRect(tab.rect, flashColor);
            p.setOpacity(1);
        }
    }

    // Draw the selected tab last to get it "on top"
    if (selected >= 0) {
        QStyleOptionTab tab;
        initStyleOption(&tab, selected);
        // 强制让文本居中
        tab.rightButtonSize = QSize();
        if (d->paintWithOffsets && d->tabList[selected].dragOffset != 0) {
            if (vertical) {
                tab.rect.moveTop(tab.rect.y() + d->tabList[selected].dragOffset);
            } else {
                tab.rect.moveLeft(tab.rect.x() + d->tabList[selected].dragOffset);
            }
        }
        if (!d->dragInProgress) {
            q->paintTab(&p, selected, tab);

            if (selected == flashTabIndex) {
                p.setOpacity(opacityOnFlash);
                p.fillRect(tab.rect, flashColor);
                p.setOpacity(1);
            }
        } else {
            int taboverlap = style()->pixelMetric(QStyle::PM_TabBarTabOverlap, 0, this);
            d->movingTab->setGeometry(tab.rect.adjusted(-taboverlap, 0, taboverlap, 0));
        }
    }

    // Only draw the tear indicator if necessary. Most of the time we don't need too.
//    if (d->leftB->isVisible() && cut >= 0) {
//        cutTab.rect = rect();
//        cutTab.rect = style()->subElementRect(QStyle::SE_TabBarTearIndicator, &cutTab, this);
//        p.drawPrimitive(QStyle::PE_IndicatorTabTear, cutTab);
//    }
}

void DTabBarPrivate::mouseMoveEvent(QMouseEvent *event)
{
    QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d_ptr));

    if (!d->movable && !d->validIndex(ghostTabIndex))
        return;

    // Be safe!
    if (d->pressedIndex != -1
        && event->buttons() == Qt::NoButton)
        moveTabFinished(d->pressedIndex);

    int offset_x = qAbs(event->x() - d->dragStartPosition.x());
    int offset_y = qAbs(event->y() - d->dragStartPosition.y());
    bool valid_pressed_index = d->validIndex(d->pressedIndex);

    if (verticalTabs(d->shape)) {
        std::swap(offset_x, offset_y);
    }

    // Start drag
    if (drag && drag->property("_d_DTabBarPrivate_drity").toBool()) {
        drag->deleteLater();
    }

    if (!drag && valid_pressed_index) {
        if (offset_y > startDragDistance && !d->validIndex(ghostTabIndex)) {
            setupDragableTab();
        }
    }

    // Start move
    if (!d->dragInProgress && valid_pressed_index) {
        if (offset_x > startDragDistance) {
            d->dragInProgress = true;
            setupMovableTab();

            scrollOffset = d->scrollOffset;
            dragStartPosition = d->dragStartPosition;
        }
    }

    if (event->buttons() == Qt::LeftButton
        // 此条件会导致横向拖动时卡顿
        // && offset_x > startDragDistance
        && valid_pressed_index) {

        updateMoveingTabPosition(event->pos());

        scrollOffset = d->scrollOffset;
        dragStartPosition = d->dragStartPosition;

        // Auto scroll tabs
        autoScrollTabs(event->pos());
    }
}

void DTabBarPrivate::mouseReleaseEvent(QMouseEvent *e)
{
    QTabBar::mouseReleaseEvent(e);

    stopAutoScrollTabs();
}

void DTabBarPrivate::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->source() == this) {
        e->acceptProposedAction();

        QMouseEvent event(QEvent::MouseMove, e->posF(),
                          Qt::LeftButton, e->mouseButtons(),
                          e->keyboardModifiers());

        mouseMoveEvent(&event);
    } else {
        int index = tabInsertIndexFromMouse(e->pos());

        if (q_func()->canInsertFromMimeData(index, e->mimeData())) {
            setDragingFromOther(true);
            e->acceptProposedAction();
        }
    }
}

void DTabBarPrivate::dragLeaveEvent(QDragLeaveEvent *e)
{
    setDragingFromOther(false);
    stopAutoScrollTabs();

    QTabBar::dragLeaveEvent(e);

    QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d_ptr));

    // clean hover state
    d->hoverRect = QRect();
}

void DTabBarPrivate::dragMoveEvent(QDragMoveEvent *e)
{
    if (e->source() == this) {
        e->acceptProposedAction();

        QMouseEvent event(QEvent::MouseMove, e->posF(),
                          Qt::LeftButton, e->mouseButtons(),
                          e->keyboardModifiers());

        mouseMoveEvent(&event);
    } else {
        autoScrollTabs(e->pos());

        int index = tabInsertIndexFromMouse(e->pos());

        if (q_func()->canInsertFromMimeData(index, e->mimeData())) {
            setDragingFromOther(true);
            e->acceptProposedAction();
        }
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
    } else {
        setDragingFromOther(false);

        int index = tabInsertIndexFromMouse(e->pos());

        if (q_func()->canInsertFromMimeData(index, e->mimeData())) {
            e->acceptProposedAction();
            e->setDropAction(Qt::MoveAction);
            q_func()->insertFromMimeData(index, e->mimeData());
        }
    }

    stopAutoScrollTabs();
}

void DTabBarPrivate::showEvent(QShowEvent *e)
{
    updateGeometry();

    QTabBar::showEvent(e);
}

QSize DTabBarPrivate::tabSizeHint(int index) const
{
    D_QC(DTabBar);

    if (qApp->buildDtkVersion() > DTK_VERSION_CHECK(2, 0, 8, 1))
        return q->tabSizeHint(index);

    return q->DTabBar::tabSizeHint(index);
}

QSize DTabBarPrivate::minimumTabSizeHint(int index) const
{
    D_QC(DTabBar);

    const QSize &min = qApp->buildDtkVersion() > DTK_VERSION_CHECK(2, 0, 8, 1) ? q->minimumTabSizeHint(index) : q->DTabBar::minimumTabSizeHint(index);

    if (min.isValid())
        return min;

    QSize size = QTabBar::tabSizeHint(index);
    const QSize &max = qApp->buildDtkVersion() > DTK_VERSION_CHECK(2, 0, 8, 1) ? q->maximumTabSizeHint(index) : q->DTabBar::maximumTabSizeHint(index);

    if (max.isValid()) {
        size.setWidth(qMin(size.width(), max.width()));
        size.setHeight(qMin(size.height(), max.height()));
    }

    return size;
}

void DTabBarPrivate::tabInserted(int index)
{
    D_Q(DTabBar);

    if (qApp->buildDtkVersion() > DTK_VERSION_CHECK(2, 0, 8, 1))
        q->tabInserted(index);
    else
        q->DTabBar::tabInserted(index);
}

void DTabBarPrivate::tabRemoved(int index)
{
    D_Q(DTabBar);

    if (qApp->buildDtkVersion() > DTK_VERSION_CHECK(2, 0, 8, 1))
        q->tabRemoved(index);
    else
        q->DTabBar::tabRemoved(index);
}

void DTabBarPrivate::tabLayoutChange()
{
    D_Q(DTabBar);

    if (qApp->buildDtkVersion() > DTK_VERSION_CHECK(2, 0, 8, 1))
        q->tabLayoutChange();
    else
        q->DTabBar::tabLayoutChange();
}

void DTabBarPrivate::initStyleOption(QStyleOptionTab *option, int tabIndex) const
{
    QTabBar::initStyleOption(option, tabIndex);
    // 因为没有背景，因此使用高亮色作为文本颜色
    option->palette.setBrush(QPalette::HighlightedText, option->palette.highlight());
}

QTabBarPrivate *DTabBarPrivate::dd() const
{
    return reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d_ptr));
}

/*!
 * \~chinese \class DTabBar
 * \~chinese \brief DTabBar 是一个标签栏控件，是在 QTabBar 基础之上进行扩展增强。
 *
 * \~chinese 增加了许多特性，比如：新建按钮，支持向外拖放事件...该控件目前被深度编辑器中所使用。
 */

/*!
 * \~chinese \fn DTabBar::currentChanged()
 * \~chinese \brief 当前索引位置改变
 * \~chinese \fn DTabBar::tabCloseRequested()
 * \~chinese \brief 点击关闭后发出
 * \~chinese \fn DTabBar::tabMoved()
 * \~chinese \brief 索引位置发生变化
 * \~chinese \fn DTabBar::tabIsInserted()
 * \~chinese \brief 新标签已插入
 * \~chinese \fn DTabBar::tabIsRemoved()
 * \~chinese \brief 标签已删除
 * \~chinese \fn DTabBar::tabBarClicked()
 * \~chinese \brief 点击索引标签
 * \~chinese \fn DTabBar::tabBarDoubleClicked()
 * \~chinese \brief 双击索引标签
 * \~chinese \fn DTabBar::tabAddRequested()
 * \~chinese \brief 新标签加入
 * \~chinese \fn DTabBar::tabReleaseRequested()
 * \~chinese \brief 索引标签鼠标释放
 * \~chinese \fn DTabBar::dragActionChanged()
 * \~chinese \brief 鼠标形状发生变化
 * \~chinese \fn DTabBar::tabDroped()
 * \~chinese \brief 标签拖放
 * \~chinese \fn DTabBar::dragStarted()
 * \~chinese \brief 开始拖起
 * \~chinese \fn DTabBar::dragEnd()
 * \~chinese \brief 拖放结束
 */

DTabBar::DTabBar(QWidget *parent)
    : QWidget(parent)
    , DObject(*new DTabBarPrivate(this))
{
    setAcceptDrops(true);
}

/*!
 * \~chinese \brief 设置索引标签的最小尺寸
 */
void DTabBar::setTabMinimumSize(int index, const QSize &size)
{
    D_D(DTabBar);

    if (d->tabMinimumSize.at(index) == size)
        return;

    d->tabMinimumSize[index] = size;

    QTabBarPrivate *dd = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d->d_ptr));

    dd->layoutDirty = true;
}

/*!
 * \~chinese \brief 设置索引标签的最大尺寸
 */
void DTabBar::setTabMaximumSize(int index, const QSize &size)
{
    D_D(DTabBar);

    if (d->tabMaximumSize.at(index) == size)
        return;

    d->tabMaximumSize[index] = size;

    QTabBarPrivate *dd = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d->d_ptr));

    dd->layoutDirty = true;
}

/*!
 * \~chinese \brief 新建标签栏按钮是否可见
 */
bool DTabBar::visibleAddButton() const
{
    D_DC(DTabBar);

    return d->visibleAddButton;
}

/*!
 * \~chinese \brief 标签栏形状
 */
QTabBar::Shape DTabBar::shape() const
{
    return d_func()->shape();
}

/*!
 * \~chinese \brief 设置标签栏形状
 */
void DTabBar::setShape(QTabBar::Shape shape)
{
    D_D(DTabBar);

    bool old_vertical = verticalTabs(d_func()->shape());
    bool new_vertical = verticalTabs(shape);

    d->setShape(shape);

    if (old_vertical != new_vertical) {
        if (QBoxLayout *layout = qobject_cast<QBoxLayout*>(this->layout())) {
            if (new_vertical)
                layout->setDirection(QBoxLayout::TopToBottom);
            else
                layout->setDirection(QBoxLayout::LeftToRight);
        }

        if (new_vertical) {
            d->leftScrollButton->setArrowType(Qt::UpArrow);
            d->rightScrollButton->setArrowType(Qt::DownArrow);
            d->leftScrollButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            d->rightScrollButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            d->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
            d->addButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            d->addButton->setFixedHeight(48);
            d->addButton->setMinimumWidth(0);
            d->addButton->setMaximumWidth(9999);
        } else {
            d->leftScrollButton->setArrowType(Qt::LeftArrow);
            d->rightScrollButton->setArrowType(Qt::RightArrow);
            d->leftScrollButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
            d->rightScrollButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
            d->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
            d->addButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
            d->addButton->setFixedWidth(48);
            d->addButton->setMinimumHeight(0);
            d->addButton->setMaximumHeight(9999);
        }
    }
}

/*!
 * \~chinese \brief 添加一个新的标签，返回新标签的索引
 */
int DTabBar::addTab(const QString &text)
{
    return d_func()->addTab(text);
}

/*!
 * \~chinese \brief 添加一个新的标签，包含图标、文本
 */
int DTabBar::addTab(const QIcon &icon, const QString &text)
{
    return d_func()->addTab(icon, text);
}

/*!
 * \~chinese \brief 插入一个文本标签
 */
int DTabBar::insertTab(int index, const QString &text)
{
    return d_func()->insertTab(index, text);
}

/*!
 * \~chinese \brief 插入一个图标文本标签
 */
int DTabBar::insertTab(int index, const QIcon &icon, const QString &text)
{
    return insertTab(index, icon, text);
}

/*!
 * \~chinese \brief 移除标签索引位置
 */
void DTabBar::removeTab(int index)
{
    d_func()->removeTab(index);
}

/*!
 * \~chinese \brief 移动标签索引位置
 */
void DTabBar::moveTab(int from, int to)
{
    d_func()->moveTab(from, to);
}

/*!
 * \~chinese \brief 是否启用标签索引位置
 */
bool DTabBar::isTabEnabled(int index) const
{
    return d_func()->isTabEnabled(index);
}

/*!
 * \~chinese \brief 设置标签索引位置启用状态
 */
void DTabBar::setTabEnabled(int index, bool enable)
{
    d_func()->setTabEnabled(index, enable);
}

/*!
 * \~chinese \brief 返回标签索引位置文本
 */
QString DTabBar::tabText(int index) const
{
    return d_func()->tabText(index);
}

/*!
 * \~chinese \brief 设置标签索引位置文本
 */
void DTabBar::setTabText(int index, const QString &text)
{
    d_func()->setTabText(index, text);
}

/*!
 * \~chinese \brief 返回标签索引位置图标
 */
QIcon DTabBar::tabIcon(int index) const
{
    return d_func()->tabIcon(index);
}

/*!
 * \~chinese \brief 设置标签索引位置图标
 */
void DTabBar::setTabIcon(int index, const QIcon &icon)
{
    d_func()->setTabIcon(index, icon);
}

/*!
 * \~chinese \brief 缩略模式
 */
Qt::TextElideMode DTabBar::elideMode() const
{
    return d_func()->elideMode();
}

/*!
 * \~chinese \brief 设置缩略模式
 */
void DTabBar::setElideMode(Qt::TextElideMode mode)
{
    d_func()->setElideMode(mode);
}

#ifndef QT_NO_TOOLTIP

/*!
 * \~chinese \brief 设置标签索引位置提示文本
 */
void DTabBar::setTabToolTip(int index, const QString &tip)
{
    d_func()->setTabToolTip(index, tip);
}

/*!
 * \~chinese \brief 返回标签索引位置提示文本
 */
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

/*!
 * \~chinese \brief 设置标签索引位置数据
 */
void DTabBar::setTabData(int index, const QVariant &data)
{
    d_func()->setTabData(index, data);
}

/*!
 * \~chinese \brief 返回标签索引位置数据
 */
QVariant DTabBar::tabData(int index) const
{
    return d_func()->tabData(index);
}

/*!
 * \~chinese \brief 返回标签索引位置矩形
 */
QRect DTabBar::tabRect(int index) const
{
    QRect rect = d_func()->tabRect(index);

    rect.moveTopLeft(d_func()->mapToParent(rect.topLeft()));

    return rect;
}

/*!
 * \~chinese \brief 根据位置返回标签索引位置
 */
int DTabBar::tabAt(const QPoint &pos) const
{
    return d_func()->tabAt(d_func()->mapFromParent(pos));
}

/*!
 * \~chinese \brief 当前的标签索引位置
 */
int DTabBar::currentIndex() const
{
    return d_func()->currentIndex();
}

/*!
 * \~chinese \brief 标签总数
 */
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

/*!
 * \~chinese \brief 图标大小
 */
QSize DTabBar::iconSize() const
{
    return d_func()->iconSize();
}

/*!
 * \~chinese \brief 设置图标大小
 */
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

/*!
 * \~chinese \brief 标签栏是否可关闭
 */
bool DTabBar::tabsClosable() const
{
    return d_func()->tabsClosable();
}

/*!
 * \~chinese \brief 设置标签栏是否可关闭
 */
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

bool DTabBar::isDragable() const
{
    return d_func()->dragable;
}

void DTabBar::setDragable(bool dragable)
{
    d_func()->dragable = dragable;
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

int DTabBar::startDragDistance() const
{
    return d_func()->startDragDistance;
}

QColor DTabBar::maskColor() const
{
    return d_func()->maskColor;
}

QColor DTabBar::flashColor() const
{
    return d_func()->flashColor;
}

QWindow *DTabBar::dragIconWindow() const
{
    for (QWindow *w : qGuiApp->allWindows()) {
        if (w->inherits("QShapedPixmapWindow"))
            return w;
    }

    return nullptr;
}

/*!
 * \~chinese \brief 设置当前标签索引位置
 */
void DTabBar::setCurrentIndex(int index)
{
    d_func()->setCurrentIndex(index);
}

/*!
 * \~chinese \brief 设置新建标签按钮是否显示
 */
void DTabBar::setVisibleAddButton(bool visibleAddButton)
{
    D_D(DTabBar);

    d->visibleAddButton = visibleAddButton;
    d->addButton->setVisible(visibleAddButton);
}

/*!
 * \~chinese \brief 设置开始拖动距离
 */
void DTabBar::setStartDragDistance(int startDragDistance)
{
    d_func()->startDragDistance = startDragDistance;
}

/*!
 * \~chinese \brief 设置颜色
 */
void DTabBar::setMaskColor(QColor maskColor)
{
    d_func()->maskColor = maskColor;
}

void DTabBar::setFlashColor(QColor flashColor)
{
    d_func()->flashColor = flashColor;
}

void DTabBar::startDrag(int index)
{
    D_D(DTabBar);

    d->dd()->pressedIndex = index;
    d->setupDragableTab();
}

void DTabBar::stopDrag(Qt::DropAction action)
{
    if (QBasicDrag *drag = dynamic_cast<QBasicDrag*>(QDragManager::self()->m_platformDrag)) {
        drag->cancel();
        drag->m_executed_drop_action = action;

        if (drag->m_eventLoop)
            drag->m_eventLoop->quit();
    }
}

void DTabBar::dragEnterEvent(QDragEnterEvent *e)
{
    D_D(DTabBar);

    if (e->source() == d)
        return QWidget::dragEnterEvent(e);

    int index = d->tabInsertIndexFromMouse(d->mapFromParent(e->pos()));

    if (canInsertFromMimeData(index, e->mimeData())) {
        d->setDragingFromOther(true);
        e->acceptProposedAction();

        // 插入一个虚拟的标签
        if (e->source() != d) {
            d->ghostTabIndex = index;
            insertFromMimeDataOnDragEnter(index, e->mimeData());
            // 延时启动startMove， 此时tabbar的大小还没有更新
            QTimer::singleShot(10, [d, index] {
                d->startMove(index);
            });
        }
    }
}

void DTabBar::dragLeaveEvent(QDragLeaveEvent *e)
{
    Q_UNUSED(e)
    D_D(DTabBar);

    d->setDragingFromOther(false);
    d->stopAutoScrollTabs();

    if (d->dd()->validIndex(d->ghostTabIndex)) {
        d->stopMove();
        d->removeTab(d->ghostTabIndex);
        d->ghostTabIndex = -1;
    }
}

void DTabBar::dragMoveEvent(QDragMoveEvent *e)
{
    D_D(DTabBar);

    if (d->ignoreDragEvent)
        return;

    if (e->source() == d)
        return QWidget::dragMoveEvent(e);

    int index = d->dd()->validIndex(d->ghostTabIndex) ? d->ghostTabIndex : d->tabInsertIndexFromMouse(d->mapFromParent(e->pos()));
    bool canInsert = false;

    if (canInsertFromMimeData(index, e->mimeData())) {
        d->setDragingFromOther(true);
        e->acceptProposedAction();
        canInsert = true;
    } else if (d->dd()->validIndex(d->ghostTabIndex)) {
        d->stopMove();
        d->removeTab(d->ghostTabIndex);
        d->ghostTabIndex = -1;
    }

    if (e->source() != d) {
        if (canInsert) {
            if (d->dd()->validIndex(d->ghostTabIndex)) {
                QMouseEvent event(QEvent::MouseMove, d->mapFromParent(e->pos()),
                                  Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
                d->mouseMoveEvent(&event);
            } else {
                d->ghostTabIndex = index;
                insertFromMimeDataOnDragEnter(index, e->mimeData());
                d->startMove(index);
            }
        } else {
            d->autoScrollTabs(d->mapFromParent(e->pos()));
        }
    }
}

void DTabBar::dropEvent(QDropEvent *e)
{
    D_D(DTabBar);

    if (e->source() == d)
        return QWidget::dropEvent(e);

    d->setDragingFromOther(false);
    d->stopAutoScrollTabs();

    int index = d->tabInsertIndexFromMouse(d->mapFromParent(e->pos()));

    if (canInsertFromMimeData(index, e->mimeData())) {
        e->acceptProposedAction();
        e->setDropAction(Qt::MoveAction);

        if (d->dd()->validIndex(d->ghostTabIndex)) {
            d->stopMove();
            {
                QSignalBlocker blocker(this);
                Q_UNUSED(blocker)
                d->removeTab(d->ghostTabIndex);
            }
            Q_EMIT tabIsRemoved(d->ghostTabIndex);

            insertFromMimeData(d->ghostTabIndex, e->mimeData());
            d->ghostTabIndex = -1;
        } else {
            insertFromMimeData(index, e->mimeData());
        }
    }
}

void DTabBar::resizeEvent(QResizeEvent *e)
{
    D_D(DTabBar);

    if (d->topFullWidget) {
        d->topFullWidget->resize(e->size());
        d->topFullWidget->raise();
    }

    return QWidget::resizeEvent(e);
}

void DTabBar::startTabFlash(int index)
{
    d_func()->flashTabIndex = index;
    d_func()->makeVisible(d_func()->flashTabIndex);
    d_func()->startTabFlash();
}

void DTabBar::paintTab(QPainter *painter, int index, const QStyleOptionTab &option) const
{
    Q_UNUSED(index)

    style()->drawControl(QStyle::CE_TabBarTab, &option, painter, this);
}

QPixmap DTabBar::createDragPixmapFromTab(int index, const QStyleOptionTab &option, QPoint *hotspot) const
{
    Q_UNUSED(hotspot)

    QPixmap grabImage(option.rect.size() * devicePixelRatioF());
    grabImage.setDevicePixelRatio(devicePixelRatioF());
    grabImage.fill(Qt::transparent);

    QStyleOptionTab tab = option;

    int taboverlap = style()->pixelMetric(QStyle::PM_TabBarTabOverlap, 0, this);

    tab.rect.moveTopLeft(QPoint(taboverlap, 0));

    QPainter p(&grabImage);
    p.initFrom(d_func());
    paintTab(&p, index, tab);

    return grabImage;
}

QMimeData *DTabBar::createMimeDataFromTab(int index, const QStyleOptionTab &option) const
{
    Q_UNUSED(index)
    Q_UNUSED(option)

    QMimeData *data = new QMimeData();

    data->setText(tabText(index));
    data->setData("deepin/dtkwidget-DTabBar-tab", QByteArray());

    return data;
}

bool DTabBar::canInsertFromMimeData(int index, const QMimeData *source) const
{
    Q_UNUSED(index)

    return source->hasFormat("deepin/dtkwidget-DTabBar-tab");
}

void DTabBar::insertFromMimeData(int index, const QMimeData *source)
{
    startTabFlash(insertTab(index, source->text()));
}

void DTabBar::insertFromMimeDataOnDragEnter(int index, const QMimeData *source)
{
    startTabFlash(insertTab(index, source->text()));
}

void DTabBar::tabInserted(int index)
{
    D_D(DTabBar);

    d->tabMaximumSize.insert(index, QSize());
    d->tabMinimumSize.insert(index, QSize());

    d->QTabBar::tabInserted(index);

    Q_EMIT tabIsInserted(index);
}

void DTabBar::tabLayoutChange()
{
    D_D(DTabBar);

    d->QTabBar::tabLayoutChange();
}

void DTabBar::tabRemoved(int index)
{
    D_D(DTabBar);

    d->tabMaximumSize.removeAt(index);
    d->tabMinimumSize.removeAt(index);

    d->QTabBar::tabRemoved(index);

    Q_EMIT tabIsRemoved(index);
}

QSize DTabBar::tabSizeHint(int index) const
{
    D_DC(DTabBar);

    QSize size = d->QTabBar::tabSizeHint(index);

    QTabBarPrivate *dd = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d->d_ptr));
    bool is_vertical = verticalTabs(dd->shape);

    if (is_vertical) {
        size.setWidth(qMax(size.width(), d->width()));
    } else {
        size.setHeight(qMax(size.height(), d->height()));
    }

    const QSize &min = qApp->buildDtkVersion() > DTK_VERSION_CHECK(2, 0, 8, 1) ? minimumTabSizeHint(index) : DTabBar::minimumTabSizeHint(index);
    const QSize &max = qApp->buildDtkVersion() > DTK_VERSION_CHECK(2, 0, 8, 1) ? maximumTabSizeHint(index) : DTabBar::maximumTabSizeHint(index);

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

    return d->tabMinimumSize.value(index);
}

QSize DTabBar::maximumTabSizeHint(int index) const
{
    D_DC(DTabBar);

    return d->tabMaximumSize.value(index);
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
