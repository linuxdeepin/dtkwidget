// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
#include <QToolTip>
#include <private/qhighdpiscaling_p.h>

#include <DApplicationHelper>

#include <private/qtabbar_p.h>
#define private public
#define protected public
#include <private/qdnd_p.h>
#include <private/qsimpledrag_p.h>
#include <private/qshapedpixmapdndwindow_p.h>
#undef private
#undef protected

#include "dpalettehelper.h"
#include "diconbutton.h"

DWIDGET_BEGIN_NAMESPACE

// TODO: Replace with verticalTabs in qtabbar_p.h when all versions of Qt support.
constexpr inline static bool dtk_verticalTabs(QTabBar::Shape shape)
{
    return shape == QTabBar::RoundedWest
           || shape == QTabBar::RoundedEast
           || shape == QTabBar::TriangularWest
           || shape == QTabBar::TriangularEast;
}

// QBoxLayout::Direction与QTabBar::Shape之间的映射
inline static QBoxLayout::Direction shapeToDirection (const QTabBar::Shape shape)
{
    if (dtk_verticalTabs(shape)) {
        return QBoxLayout::TopToBottom;
    } else {
        return QBoxLayout::LeftToRight;
    }
}

// 判断是否为横向布局
static inline bool horz(QBoxLayout::Direction dir)
{
    return dir == QBoxLayout::RightToLeft || dir == QBoxLayout::LeftToRight;
}

// TODO 提取到DStyle
static inline QColor getColor(const QStyleOption *option, DPalette::ColorType type, const QWidget *w)
{
    if (auto s = qobject_cast<DStyle*>(w->style())) {
        const DPalette &pa = DPaletteHelper::instance()->palette(w, option->palette);
        return s->generatedBrush(option, pa.brush(type), pa.currentColorGroup(), type).color();
    }
    return QColor();
}

// 获得边框线颜色，与chameleonstyle保持一致，TODO 提取到DStyle
static inline QColor getTabbarBorderColor(const QStyleOption *opt, const QWidget *w)
{
    if (DGuiApplicationHelper::instance()->paletteType() == DGuiApplicationHelper::LightType
            || (DGuiApplicationHelper::instance()->paletteType() == DGuiApplicationHelper::UnknownType
                && DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::LightType)) {
        return getColor(opt, DPalette::FrameBorder, w);
    } else if (DGuiApplicationHelper::instance()->paletteType() == DGuiApplicationHelper::DarkType
               || (DGuiApplicationHelper::instance()->paletteType() == DGuiApplicationHelper::UnknownType
                   && DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::DarkType)){
        return QColor(0, 0, 0, static_cast<int>(0.05 * 255));
    } else {
        return getColor(opt, DPalette::FrameBorder, w);
    }
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
        addButton->setAccessibleName("DTabBarAddButton");
        addButton->installEventFilter(this);

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
        setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
        setDrawBase(false);

        QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d_ptr));

        leftScrollButton = new DIconButton(DStyle::SP_ArrowLeft, qq);
        leftScrollButton->setObjectName("leftButton");
        rightScrollButton = new DIconButton(DStyle::SP_ArrowRight, qq);
        rightScrollButton->setObjectName("rightButton");

        leftScrollButton->setVisible(d->leftB->isVisible());
        leftScrollButton->setAutoRepeat(true);
        rightScrollButton->setVisible(d->rightB->isVisible());
        rightScrollButton->setAutoRepeat(true);

        d->leftB->setFixedSize(0, 0);
        d->leftB->installEventFilter(this);
        d->rightB->setFixedSize(0, 0);
        d->rightB->installEventFilter(this);

        qq->installEventFilter(this);

        connect(leftScrollButton, &DIconButton::clicked, d->leftB, &QToolButton::click);
        connect(rightScrollButton, &DIconButton::clicked, d->rightB, &QToolButton::click);

        layout = new QBoxLayout(shapeToDirection(d->shape), qq);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        layout->setContentsMargins(0, 0, 0, 0);

        leftBtnL = new QSpacerItem(0, 0);
        leftBtnR = new QSpacerItem(0, 0);
        rightBtnL = new QSpacerItem(0, 0);
        addBtnL = new QSpacerItem(0, 0);
        addBtnR = new QSpacerItem(0, 0);
        stretch = new QSpacerItem(1, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        layout->addSpacerItem(leftBtnL);
        layout->addWidget(leftScrollButton);

        layout->addSpacerItem(leftBtnR);
        layout->addWidget(this);
        layout->addSpacerItem(rightBtnL);

        layout->addWidget(rightScrollButton);
        layout->addSpacerItem(addBtnL);
        layout->addWidget(addButton);

        layout->addSpacerItem(addBtnR);
        layout->addSpacerItem(stretch);
        d->expanding = false;

        qq->setTabLabelAlignment(Qt::AlignCenter);
        updateTabAlignment();

        qq->setFocusProxy(this);

        connect(this, &DTabBarPrivate::currentChanged, this, &DTabBarPrivate::onCurrentChanged);
        connect(this, &DTabBarPrivate::tabCloseRequested, qq, &DTabBar::tabCloseRequested);
        connect(this, &DTabBarPrivate::tabMoved, qq, &DTabBar::tabMoved);
        connect(this, &DTabBarPrivate::tabBarClicked, qq, &DTabBar::tabBarClicked);
        connect(this, &DTabBarPrivate::tabBarDoubleClicked, qq, &DTabBar::tabBarDoubleClicked);
    }

    // 刷新占位符弹簧的方向及大小,(当前布局方向、是否为直角风格、滑块是否显示、添加按钮是否显示)
    void refreshSpacers()
    {
        D_Q(DTabBar);
        const int TabSpacing = DStyle::pixelMetric(style(), DStyle::PM_ContentsSpacing, nullptr) / 2;
        bool isRectType = q->property("_d_dtk_tabbartab_type").toBool();
        int ll = 0;
        int lr = 0;
        int rl = 0;
        int al = 0;
        int ar = 0;
        if (leftScrollButton->isVisible()) {
            ll = isRectType ? 6 : TabSpacing;
            lr = isRectType ? 6 : TabSpacing;
        }

        /* 右箭头、添加按钮都显示，则右键投左右边距6，添加按钮左右边距10
         * 右箭头、添加按钮都不显示，则没有边距
         * 只显示右键，左右边距为6
         * 只显示添加按钮，左右边距为10
         */
        if (rightScrollButton->isVisible()) {
            rl = isRectType ? 6 : TabSpacing;
            const int AddButtonLeftMargin = 6;
            al = AddButtonLeftMargin;
        }
        if (addButton->isVisible()) {
            const int AddButtonLeftMargin = 10;
            al = AddButtonLeftMargin;
            const int AddButtonRightMargin = 10;
            ar = AddButtonRightMargin;
        }

        // 根据布局方向设置不同弹簧策略
        if (horz(shapeToDirection(shape()))) {
            leftBtnL->changeSize(ll, 0);
            leftBtnR->changeSize(lr, 0);
            rightBtnL->changeSize(rl, 0);
            addBtnL->changeSize(al, 0);
            addBtnR->changeSize(ar, 0);
        } else {
            leftBtnL->changeSize(0, ll);
            leftBtnR->changeSize(0, lr);
            rightBtnL->changeSize(0, rl);
            addBtnL->changeSize(0, al);
            addBtnR->changeSize(0, ar);
        }
        layout->invalidate();
    }

    ~DTabBarPrivate() override
    {
        if (stretchIsRemove && stretch) {
            delete stretch;
        }
    }

    // 更新布局内的控件的对齐方式
    void updateTabAlignment()
    {
        Qt::Alignment tabAlignment = dtk_verticalTabs(shape()) ?
                    Qt::AlignHCenter : Qt::AlignVCenter;

        for (int i = 0; i < layout->count(); i++) {
            QLayoutItem *item = layout->itemAt(i);
            item->setAlignment(tabAlignment);
        }
        layout->invalidate();
    }

    void moveTabOffset(int index, int offset)
    {
        QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d_ptr));

        if (!d->validIndex(index))
            return;
        d->at(index)->dragOffset = offset;
        layoutTab(index); // Make buttons follow tab
        update();
    }

    // 直角Tabbar，绘制边框延长线。
    /*
        ----      ----
            xxxxxx
        ----      ----
     */
    void drawDTabbarExtendLine()
    {
        D_Q(DTabBar);

        if (!q->property("_d_dtk_tabbartab_type").toBool()) {
            return;
        }

        QStyleOptionTabBarBase optTabBase;
        QTabBarPrivate::initStyleBaseOption(&optTabBase, this, q->size());

        QStylePainter p(q);
        QStyleOption* opt = &optTabBase;
        p.setPen(getTabbarBorderColor(opt, this));

        const QRect dtabbarRect = q->rect();
        const QRect qtabbarRect = this->rect();

        // 不用区分是否竖直，线坐标一致，
        p.drawLine(dtabbarRect.topLeft(), mapTo(q, qtabbarRect.topLeft()));
        p.drawLine(dtabbarRect.bottomLeft(), mapTo(q, qtabbarRect.bottomLeft()));

        p.drawLine(mapTo(q, qtabbarRect.topRight()), dtabbarRect.topRight());
        p.drawLine(mapTo(q, qtabbarRect.bottomRight()), dtabbarRect.bottomRight());
    }

    struct TabBarAnimation : public QVariantAnimation {
        TabBarAnimation(QTabBarPrivate::Tab *t,
                        QTabBarPrivate *_priv,
                        DTabBarPrivate *_dpriv)
            : tab(t), priv(_priv), dpriv(_dpriv)
        { setEasingCurve(QEasingCurve::InOutQuad); }

        void updateCurrentValue(const QVariant &current) Q_DECL_OVERRIDE
        {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            dpriv->moveTabOffset(priv->tabList.indexOf(*tab), current.toInt());
#else
            dpriv->moveTabOffset(priv->tabList.indexOf(tab), current.toInt());
#endif
        }

        void updateState(State, State newState) Q_DECL_OVERRIDE
        {
            if (newState == Stopped)
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                dpriv->moveTabFinished(priv->tabList.indexOf(*tab));
#else
                dpriv->moveTabFinished(priv->tabList.indexOf(tab));
#endif
        }

    private:
        //these are needed for the callbacks
        QTabBarPrivate::Tab *tab;
        QTabBarPrivate *priv;
        DTabBarPrivate *dpriv;
    };

    bool eventFilter(QObject *watched, QEvent *event) override;
    bool event(QEvent *e) override;

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

    Q_SLOT void startDrag();

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

    Q_INVOKABLE void startMove(int index);
    void stopMove();

    void onCurrentChanged(int current);
    void updateCloseButtonVisible();

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

    bool mousePress = false;
    bool stretchIsRemove = false;

    QColor maskColor;
    QColor flashColor;
    // 要闪动绘制的Tab
    qreal opacityOnFlash = 1;
    int flashTabIndex = -1;

    DIconButton *leftScrollButton;
    DIconButton *rightScrollButton;
    QBoxLayout *layout;
    QSpacerItem *stretch;
    QSpacerItem *leftBtnL;
    QSpacerItem *leftBtnR;
    QSpacerItem *rightBtnL;
    QSpacerItem *addBtnL;
    QSpacerItem *addBtnR;

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

void DTabBarPrivate::startDrag()
{
    QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d_ptr));
    Qt::DropAction action = drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction);

    Q_EMIT q_func()->dragEnd(action);

    if (action == Qt::IgnoreAction) {
        Q_EMIT q_func()->tabReleaseRequested(d->pressedIndex);
    } else if (drag->target() != this) {
        if (DTabBarPrivate *tbp = qobject_cast<DTabBarPrivate*>(drag->target()))
            Q_EMIT q_func()->tabDroped(d->pressedIndex, action, tbp->q_func());
        else
            Q_EMIT q_func()->tabDroped(d->pressedIndex, action, drag->target());
    }

    drag->setProperty("_d_DTabBarPrivate_drity", true);

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
    if (dtk_verticalTabs(shape()))
        grabRect.adjust(0, -taboverlap, 0, taboverlap);
    else
        grabRect.adjust(-taboverlap, 0, taboverlap, 0);

    QPixmap grabImage(grabRect.size() * devicePixelRatioF());
    grabImage.setDevicePixelRatio(devicePixelRatioF());
    grabImage.fill(Qt::transparent);
    QPainter p(&grabImage);
#if QT_VERSION< QT_VERSION_CHECK(5, 13, 0)
    p.initFrom(this);
#else
    p.begin(this);
#endif

    QStyleOptionTab tab;
    initStyleOption(&tab, d->pressedIndex);
    tab.position = QStyleOptionTab::OnlyOneTab;
    if (dtk_verticalTabs(shape()))
        tab.rect.moveTopLeft(QPoint(0, taboverlap));
    else
        tab.rect.moveTopLeft(QPoint(taboverlap, 0));
    // 强制让文本居中
    tab.rightButtonSize = QSize();
    q_func()->paintTab(&p, d->pressedIndex, tab);
    p.end();

    reinterpret_cast<DMovableTabWidget*>(d->movingTab)->setPixmap(grabImage);
    d->movingTab->setGeometry(grabRect);
    d->movingTab->raise();

    // Re-arrange widget order to avoid overlaps
    if (d->at(d->pressedIndex)->leftWidget)
        d->at(d->pressedIndex)->leftWidget->raise();
    if (d->at(d->pressedIndex)->rightWidget)
        d->at(d->pressedIndex)->rightWidget->raise();

    if (d->leftB)
        d->leftB->raise();
    if (d->rightB)
        d->rightB->raise();
    d->movingTab->setVisible(true);
}

void DTabBarPrivate::updateMoveingTabPosition(const QPoint &mouse)
{
    QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d_ptr));

    bool vertical = dtk_verticalTabs(d->shape);
    int dragDistance;
    if (vertical) {
        dragDistance = (mouse.y() - d->dragStartPosition.y());
    } else {
        dragDistance = (mouse.x() - d->dragStartPosition.x());
    }

    if (!(d->pressedIndex == 0 && dragDistance < 0)
            && !(d->pressedIndex == d->tabList.count() - 1 && dragDistance > 0)) {
        d->at(d->pressedIndex)->dragOffset = dragDistance;
    } else {
        d->at(d->pressedIndex)->dragOffset = 0;
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
    // 强制让文本居中
    opt.rightButtonSize = QSize();

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

    if (window()->windowHandle() && window()->windowHandle()->screen())
        hotspot = QHighDpiScaling::mapPositionFromNative(hotspot, window()->windowHandle()->screen()->handle());

    drag->setHotSpot(hotspot);

    qRegisterMetaType<Qt::DropAction>();


    //task34370
    //该函数于事件循环结束后调用，d->pressedIndex在事件中已经更新
    //然传入参数非更新后的参数，故去除此函数参数
    QMetaObject::invokeMethod(this, "startDrag", Qt::QueuedConnection);
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
    bool vertical = dtk_verticalTabs(d->shape);
    int preLocation = vertical ? tabRect(from).y() : tabRect(from).x();
    setUpdatesEnabled(false);
    moveTab(from, to);
    setUpdatesEnabled(true);
    int postLocation = vertical ? tabRect(to).y() : tabRect(to).x();
    int length = postLocation - preLocation;
    d->at(to)->dragOffset -= length;

    if (!d->isAnimated()) {
        moveTabFinished(to);
        return;
    }

    QTabBarPrivate::Tab *tab = d->at(to);

    if (!tab->animation)
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        tab->animation = reinterpret_cast<QTabBarPrivate::Tab::TabBarAnimation*>(new TabBarAnimation(tab, d, this));
#else
        tab->animation.reset(reinterpret_cast<QTabBarPrivate::Tab::TabBarAnimation*>(new TabBarAnimation(tab, d, this)));
#endif
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
    QTabBarPrivate::Tab *tab = d->at(index);
    bool vertical = dtk_verticalTabs(d->shape);

    if (!(tab->leftWidget || tab->rightWidget))
        return;

    QStyleOptionTab opt;
    initStyleOption(&opt, index);

    if (tab->leftWidget) {
        QRect rect = style()->subElementRect(QStyle::SE_TabBarTabLeftButton, &opt, this);
        QPoint p = rect.topLeft();
        if ((index == d->pressedIndex) || d->paintWithOffsets) {
            if (vertical)
                p.setY(p.y() + d->at(index)->dragOffset);
            else
                p.setX(p.x() + d->at(index)->dragOffset);
        }
        tab->leftWidget->move(p);
    }

    if (tab->rightWidget) {
        QRect rect = style()->subElementRect(QStyle::SE_TabBarTabRightButton, &opt, this);
        QPoint p = rect.topLeft();
        if ((index == d->pressedIndex) || d->paintWithOffsets) {
            if (vertical)
                p.setY(p.y() + tab->dragOffset);
            else
                p.setX(p.x() + tab->dragOffset);
        }
        tab->rightWidget->move(p);
    }
}

void DTabBarPrivate::moveTabFinished(int index)
{
    QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d_ptr));
    bool cleanup = (d->pressedIndex == index) || (d->pressedIndex == -1) || !d->validIndex(index);
    bool allAnimationsFinished = true;
#ifndef QT_NO_ANIMATION
    for (int i = 0; allAnimationsFinished && i < d->tabList.count(); ++i) {
        const QTabBarPrivate::Tab *t = d->at(i);
        if (t->animation && t->animation->state() == QAbstractAnimation::Running)
            allAnimationsFinished = false;
    }
#endif //QT_NO_ANIMATION
    if (allAnimationsFinished && cleanup) {
        if(d->movingTab)
            d->movingTab->setVisible(false); // We might not get a mouse release
        for (int i = 0; i < d->tabList.count(); ++i) {
            d->at(i)->dragOffset = 0;
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
        d->at(index)->dragOffset = 0;
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
    const QRect tabRect = d->at(index)->rect;

    const int oldScrollOffset = d->scrollOffset;
    const bool horiz = !dtk_verticalTabs(d->shape);
    const int extra_width = 2 * qMax(style()->pixelMetric(QStyle::PM_TabBarScrollButtonWidth, 0, this),
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                     QApplication::globalStrut().width());
#else
                                    0);
#endif
    const int available = (horiz ? width() : height()) - extra_width;
    const int start = horiz ? tabRect.left() : tabRect.top();
    const int end = horiz ? tabRect.right() : tabRect.bottom();
    if (start < d->scrollOffset) // too far left
        d->scrollOffset = start - (index ? 8 : 0);
    else if (end > d->scrollOffset + available) // too far right
        d->scrollOffset = end - available + 1;

    d->leftB->setEnabled(d->scrollOffset > 0);

    const int last = horiz ? d->at(d->tabList.size() - 1)->rect.right()
                           : d->at(d->tabList.size() - 1)->rect.bottom();
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
    bool vertical = dtk_verticalTabs(d->shape);
    const QSize &size_hint = this->size();
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
            if (dtk_verticalTabs(d->shape)) {
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
    bool vertical = dtk_verticalTabs(this->shape());

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

    // 不要改变鼠标位置。会导致光标不受控的体验，bug-18362
//    connect(mouse_animation, &QVariantAnimation::valueChanged, this, [=] (const QVariant &value) {
//        const QPoint pos = value.toPoint();
//        QCursor::setPos(pos.x(), pos.y());
//    });

    connect(mouse_animation, &QVariantAnimation::finished, this, [this, mouse_animation] {
        mouse_animation->deleteLater();
        ignoreDragEvent = false;
    });

    ignoreDragEvent = true;
    mouse_animation->start();
}

void DTabBarPrivate::stopMove()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QMouseEvent event(QEvent::MouseButtonRelease, mapFromGlobal(QCursor::pos()),
                      QCursor::pos(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
#else
    QMouseEvent event(QEvent::MouseButtonRelease, mapFromGlobal(QCursor::pos()),
                      Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
#endif
    bool movable = isMovable();
    setMovable(true);
    mouseReleaseEvent(&event);
    setMovable(movable);
}

void DTabBarPrivate::onCurrentChanged(int current)
{
    updateCloseButtonVisible();
    D_Q(DTabBar);
    Q_EMIT q->currentChanged(current);
}

// 只允许当前标签页显示关闭按钮
void DTabBarPrivate::updateCloseButtonVisible()
{
    if (!tabsClosable())
        return;

    int current = currentIndex();

    for (int i = 0; i < this->count(); ++i) {
        QWidget *close_button = tabButton(i, QTabBar::RightSide);

        if (!close_button || close_button->metaObject()->className() != QByteArrayLiteral("CloseButton")) {
            close_button = tabButton(i, QTabBar::LeftSide);
        }

        if (!close_button || close_button->metaObject()->className() != QByteArrayLiteral("CloseButton"))
            continue;

        close_button->setVisible(i == current);
    }
}

static QIcon getArrowIcon(const QStyle *style, Qt::ArrowType type)
{
    switch (type) {
    case Qt::UpArrow:
        return style->standardIcon(QStyle::SP_ArrowUp);
    case Qt::DownArrow:
        return style->standardIcon(QStyle::SP_ArrowDown);
    case Qt::LeftArrow:
        return style->standardIcon(QStyle::SP_ArrowLeft);
    case Qt::RightArrow:
        return style->standardIcon(QStyle::SP_ArrowRight);
    default:
        break;
    }

    return QIcon();
}

bool DTabBarPrivate::eventFilter(QObject *watched, QEvent *event)
{
    QTabBarPrivate *d = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d_ptr));
    D_Q(DTabBar);

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
            leftScrollButton->setIcon(getArrowIcon(style(), d->leftB->arrowType()));
            break;
        default:
            break;
        }
    } else if (watched == d->rightB) {
        switch (event->type()) {
        case QEvent::Show:
            rightScrollButton->show();
            rightScrollButton->updateGeometry();
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
            rightScrollButton->setIcon(getArrowIcon(style(), d->rightB->arrowType()));
            break;
        default:
            break;
        }
    } else if(watched == q) {
        if (QEvent::Paint == event->type()) {
            drawDTabbarExtendLine();
        }
    }
    if (event->type() == QEvent::Show || event->type() == QEvent::Hide) {
        if (watched == d->leftB || watched == d->rightB || watched == addButton) {
            refreshSpacers();
        }
    }

    return QTabBar::eventFilter(watched, event);
}

bool DTabBarPrivate::event(QEvent *e)
{
    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(e);

    if (e->type() == QEvent::ToolTip) {
        if (const QTabBarPrivate::Tab *tab = dd()->at(currentIndex())) {
            if (!tab->toolTip.isEmpty()) {
                if(mousePress)
                    return true;
            }
        }
    } if (e->type() == QEvent::MouseButtonPress && mouseEvent->button() == Qt::LeftButton) {
        mousePress = true;
    } if (e->type() == QEvent::MouseButtonRelease && mouseEvent->button() == Qt::LeftButton) {
        mousePress = false;
    }
    return QTabBar::event(e);
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
    int cutLeft = -1;
    int cutRight = -1;
    bool vertical = dtk_verticalTabs(d->shape);
    QStyleOptionTab cutTabLeft;
    QStyleOptionTab cutTabRight;
    selected = d->currentIndex;
    if (d->dragInProgress)
        selected = d->pressedIndex;
    const QRect scrollRect = d->normalizedScrollRect();

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

        if (d->paintWithOffsets && d->at(i)->dragOffset != 0) {
            if (vertical) {
                tab.rect.moveTop(tab.rect.y() + d->at(i)->dragOffset);
            } else {
                tab.rect.moveLeft(tab.rect.x() + d->at(i)->dragOffset);
            }
        }
        if (!(tab.state & QStyle::State_Enabled)) {
            tab.palette.setCurrentColorGroup(QPalette::Disabled);
        }

        // If this tab is partially obscured, make a note of it so that we can
        // pass the information along when we draw the tear.

        QRect tabRect = d->at(i)->rect;

        int tabStart = vertical ? tabRect.top() : tabRect.left();
        int tabEnd = vertical ? tabRect.bottom() : tabRect.right();
        if (tabStart < scrollRect.left() + d->scrollOffset) {
            cutLeft = i;
            cutTabLeft = tab;
        } else if (tabEnd > scrollRect.right() + d->scrollOffset) {
            cutRight = i;
            cutTabRight = tab;
        }

        // Don't bother drawing a tab if the entire tab is outside of the visible tab bar.
        if ((!vertical && (tab.rect.right() < 0 || tab.rect.left() > width()))
            || (vertical && (tab.rect.bottom() < 0 || tab.rect.top() > height())))
            continue;

        optTabBase.tabBarRect |= tab.rect;
        if (i == selected)
            continue;

        q->paintTab(&p, i, tab);
//        p.drawControl(QStyle::CE_TabBarTab, tab); // Qt源码写法

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

        if (d->paintWithOffsets && d->at(selected)->dragOffset != 0) {
            if (vertical) {
                tab.rect.moveTop(tab.rect.y() + d->at(selected)->dragOffset);
            } else {
                tab.rect.moveLeft(tab.rect.x() + d->at(selected)->dragOffset);
            }
        }

        if (!d->dragInProgress) {
            q->paintTab(&p, selected, tab);
//            p.drawControl(QStyle::CE_TabBarTab, tab); // Qt源码写法

            if (selected == flashTabIndex) {
                p.setOpacity(opacityOnFlash);
                p.fillRect(tab.rect, flashColor);
                p.setOpacity(1);
            }
        } else {
            int taboverlap = style()->pixelMetric(QStyle::PM_TabBarTabOverlap, 0, this);
            if (dtk_verticalTabs(d->shape))
                d->movingTab->setGeometry(tab.rect.adjusted(-taboverlap, 0, taboverlap, 0));
            else
                d->movingTab->setGeometry(tab.rect.adjusted(-taboverlap, 0, taboverlap, 0));
        }
    }

    // Only draw the tear indicator if necessary. Most of the time we don't need too.
    if (d->leftB->isVisible() && cutLeft >= 0) {
        cutTabLeft.rect = rect();
        cutTabLeft.rect = style()->subElementRect(QStyle::SE_TabBarTearIndicatorLeft, &cutTabLeft, this);
        p.drawPrimitive(QStyle::PE_IndicatorTabTearLeft, cutTabLeft);
    }

    if (d->rightB->isVisible() && cutRight >= 0) {
        cutTabRight.rect = rect();
        cutTabRight.rect = style()->subElementRect(QStyle::SE_TabBarTearIndicatorRight, &cutTabRight, this);
        p.drawPrimitive(QStyle::PE_IndicatorTabTearRight, cutTabRight);
    }
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    int offset_x = qAbs(event->x() - d->dragStartPosition.x());
    int offset_y = qAbs(event->y() - d->dragStartPosition.y());
#else
    int offset_x = qAbs(event->position().x() - d->dragStartPosition.x());
    int offset_y = qAbs(event->position().y() - d->dragStartPosition.y());
#endif
    bool valid_pressed_index = d->validIndex(d->pressedIndex);

    if (dtk_verticalTabs(d->shape)) {
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
        && d->dragInProgress // update tab position only in dragging..
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

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        QMouseEvent event(QEvent::MouseMove, e->position(),
                          QCursor::pos(), Qt::LeftButton, e->buttons(),
                          e->modifiers());
#else
        QMouseEvent event(QEvent::MouseMove, e->posF(),
                          Qt::LeftButton, e->mouseButtons(),
                          e->keyboardModifiers());
#endif

        mouseMoveEvent(&event);
    } else {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        int index = tabInsertIndexFromMouse(e->position().toPoint());
#else
        int index = tabInsertIndexFromMouse(e->pos());
#endif

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

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        QMouseEvent event(QEvent::MouseMove, e->position(),
                          QCursor::pos(), Qt::LeftButton, e->buttons(),
                          e->modifiers());
#else
        QMouseEvent event(QEvent::MouseMove, e->posF(),
                          Qt::LeftButton, e->mouseButtons(),
                          e->keyboardModifiers());
#endif

        mouseMoveEvent(&event);
    } else {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        autoScrollTabs(e->position().toPoint());
#else
        autoScrollTabs(e->pos());
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        int index = tabInsertIndexFromMouse(e->position().toPoint());
#else
        int index = tabInsertIndexFromMouse(e->pos());
#endif

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

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        QMouseEvent event(QEvent::MouseButtonRelease, e->position(),
                          QCursor::pos(), Qt::LeftButton, e->buttons(),
                          e->modifiers());
#else
        QMouseEvent event(QEvent::MouseButtonRelease, e->posF(),
                          Qt::LeftButton, e->mouseButtons(),
                          e->keyboardModifiers());
#endif

        mouseReleaseEvent(&event);
    } else {
        setDragingFromOther(false);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        int index = tabInsertIndexFromMouse(e->position().toPoint());
#else
        int index = tabInsertIndexFromMouse(e->pos());
#endif

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

    return q->tabSizeHint(index);
}

QSize DTabBarPrivate::minimumTabSizeHint(int index) const
{
    D_QC(DTabBar);

    const QSize &min = q->minimumTabSizeHint(index);

    if (min.isValid())
        return min;

    QSize size = QTabBar::tabSizeHint(index);
    const QSize &max = q->maximumTabSizeHint(index);

    if (max.width() > 0) {
        size.setWidth(qMin(size.width(), max.width()));
    }

    if (max.height() > 0) {
        size.setHeight(qMin(size.height(), max.height()));
    }

    return size;
}

void DTabBarPrivate::tabInserted(int index)
{
    D_Q(DTabBar);

    q->tabInserted(index);
}

void DTabBarPrivate::tabRemoved(int index)
{
    D_Q(DTabBar);

    q->tabRemoved(index);
}

void DTabBarPrivate::tabLayoutChange()
{
    D_Q(DTabBar);

    q->tabLayoutChange();
    // 更新关闭按钮的显示
    updateCloseButtonVisible();
}

void DTabBarPrivate::initStyleOption(QStyleOptionTab *option, int tabIndex) const
{
    QTabBar::initStyleOption(option, tabIndex);
}

QTabBarPrivate *DTabBarPrivate::dd() const
{
    return reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d_ptr));
}

/*!
@~english
  @class Dtk::Widget::DTabBar
  \inmodule dtkwidget
  @brief DTabBar is a tab bar control that is an extension and enhancement of QTabBar

  Added many features, such as a new button and support for dragging and dropping events outward This control is currently used in the depth editor.
 */

/*!
@~english
  @fn void DTabBar::currentChanged(int index)
  @brief The current index position has changed

  @param[in] index Changed index value
 */

/*!
@~english
  @fn void DTabBar::tabCloseRequested(int index)
  @brief Click to close and send out

  @param[in] index Closed index value
 */

/*!
@~english
  @fn void DTabBar::tabMoved(int from, int to)
  @brief Index position has changed

  @param[in] a Initial value of change in index position from， @param[in] to Index value after position change
 */

/*!
@~english
  @fn void DTabBar::tabIsInserted(int index)
  @brief The new label has been inserted

  @param[in] index The index value of the inserted label
 */

/*!
@~english
  @fn void DTabBar::tabIsRemoved(int index)
  @brief The label has been deleted

  @param[in] index The index value of the deleted label
 */

/*!
@~english
  @fn void DTabBar::tabBarClicked(int index)
  @brief Click on the index tab

  @param[in] index Click on the index value of the label
 */

/*!
@~english
  @fn void DTabBar::tabBarDoubleClicked(int index)
  @brief Double click on the index label

  @param[in] index Double click on the index value of the tag
 */

/*!
@~english
  @fn void DTabBar::tabAddRequested()
  @brief Add a new label
 */

/*!
@~english
  @fn void DTabBar::tabReleaseRequested(int index)
  @brief Index label mouse release

  @param[in] index Index value for label release
 */

/*!
@~english
  @fn void DTabBar::dragActionChanged(Qt::DropAction action)
  @brief The shape of the mouse has changed

  @param[in] action Drag and drop actions for changes
 */

/*!
@~english
  @fn void DTabBar::tabDroped(int index, Qt::DropAction action, QObject *target)
  @brief Label drag and drop

  @param[in] index Drag and drop tag indexing， @param[in] action Drag and drop action, @param[in] target Dragged DTabBar instance
 */

/*!
@~english
  @fn void DTabBar::dragStarted()
  @brief Start dragging
 */

/*!
@~english
  @fn void DTabBar::dragEnd(Qt::DropAction action)
  @brief End dragging

  @param[in] action Drag and drop action.
 */

DTabBar::DTabBar(QWidget *parent)
    : QWidget(parent)
    , DObject(*new DTabBarPrivate(this))
{
    setAcceptDrops(true);
}

/*!
@~english
  @brief Set the minimum size of the index tag.

  @param[in] index The index value of the set tag. @param[in] size Minimum size.
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
@~english
  @brief Set the maximum size of the index tag.

  @param[in] index The index value of the set tag， @param[in] size Maximum size.
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
@~english
  @brief The new tab button is visible or not

  @return The new tab button is visible then return true.Otherwise return false.
 */
bool DTabBar::visibleAddButton() const
{
    D_DC(DTabBar);

    return d->visibleAddButton;
}

/*!
@~english
  @brief The shape of the tab bar.

  @return Return the shape of the current DTabBar instance's tab bar.
 */
QTabBar::Shape DTabBar::shape() const
{
    return d_func()->shape();
}

/*!
@~english
  @brief Set the shape of the tab bar.

  @param[in] shape Tab bar shape.
 */
void DTabBar::setShape(QTabBar::Shape shape)
{
    D_D(DTabBar);

    bool old_vertical = dtk_verticalTabs(d_func()->shape());
    bool new_vertical = dtk_verticalTabs(shape);

    d->setShape(shape);

    if (old_vertical != new_vertical) {
        // 设置对应shape上的策略
        if (new_vertical) {
            d->leftScrollButton->setIcon(getArrowIcon(style(), Qt::UpArrow));
            d->rightScrollButton->setIcon(getArrowIcon(style(), Qt::DownArrow));
            d->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        } else {
            d->leftScrollButton->setIcon(getArrowIcon(style(), Qt::LeftArrow));
            d->rightScrollButton->setIcon(getArrowIcon(style(), Qt::RightArrow));
            d->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        }
        // 重新设置Direction
        d->layout->setDirection(shapeToDirection(shape));
        d->updateTabAlignment();
    }
}

/*!
@~english
  @brief Add a new tab and return the index of the new tab.

  @param[in] text Title text of the new tab.
  @return Return the index of the new tab.
 */
int DTabBar::addTab(const QString &text)
{
    return d_func()->addTab(text);
}

/*!
@~english
  @brief Add a new tab with an icon and text.

  @param[in] icon Title icon of the new tab， @param[in] text Title text of the new tab.
  @return Return the index of the new tab.
 */
int DTabBar::addTab(const QIcon &icon, const QString &text)
{
    return d_func()->addTab(icon, text);
}

/*!
@~english
  @brief Insert a text label.

  @param[in] index The index value of the inserted label. @param[in] text Title text of the new label.

  @warning if @param[in] index exceeds the current index, a new label will be added at the end.

  @return ndex of the newly inserted label.
  @sa QTabBar::insertTab()
 */
int DTabBar::insertTab(int index, const QString &text)
{
    return d_func()->insertTab(index, text);
}

/*!
@~english
  @brief Insert an icon text label.

  @param[in] index Index value at which the label will be inserted.
  @param[in] icon Icon of the new label.
  @param[in] text Text of the new label.

  @return Index of the newly inserted label.
 */
int DTabBar::insertTab(int index, const QIcon &icon, const QString &text)
{
    return d_func()->insertTab(index, icon, text);
}

/*!
@~english
  @brief Remove the label at the specified index position.

  @param[in] index Index value of the label to be removed.
 */
void DTabBar::removeTab(int index)
{
    d_func()->removeTab(index);
}

/*!
@~english
  @brief Check if the label at the specified index position is enabled.

  @param[in] index Label index.
  @return True if the label is enabled, false otherwise.
 */
void DTabBar::moveTab(int from, int to)
{
    d_func()->moveTab(from, to);
}

/*!
@~english
  @brief 是否启用标签索引位置

  @param[in] index 标签索引值.
  @return 启用标签索引返回 true，否则返回 false.
 */
bool DTabBar::isTabEnabled(int index) const
{
    return d_func()->isTabEnabled(index);
}

/*!
@~english
  @brief Set the enabled state of the label at the specified index position.

  @param[in] index Label index.
  @param[in] enable Whether to enable the label.
 */
void DTabBar::setTabEnabled(int index, bool enable)
{
    d_func()->setTabEnabled(index, enable);
}

/*!
@~english
  @brief Return the text of the label at the specified index position.

  @param[in] index Label index.
  @return Text of the corresponding label.
 */
QString DTabBar::tabText(int index) const
{
    return d_func()->tabText(index);
}

/*!
@~english
  @brief Set the text of the label at the specified index position.

  @param[in] index Label index.
  @param[in] text Label text.
 */
void DTabBar::setTabText(int index, const QString &text)
{
    d_func()->setTabText(index, text);
}

/*!
@~english
  @brief Return the icon of the label at the specified index position.

  @param[in] index Label index.
  @return Icon of the corresponding label.
 */
QIcon DTabBar::tabIcon(int index) const
{
    return d_func()->tabIcon(index);
}

/*!
@~english
  @brief Set the icon of the label at the specified index position.

  @param[in] index label index， @param[in] icon Corresponding label icon.
 */
void DTabBar::setTabIcon(int index, const QIcon &icon)
{
    d_func()->setTabIcon(index, icon);
}

/*!
@~english
  @brief Truncation mode.

  @return Return the text truncation mode of the label.
 */
Qt::TextElideMode DTabBar::elideMode() const
{
    return d_func()->elideMode();
}

/*!
@~english
  @brief Set the truncation mode for the text.

  @param[in] mode Text truncation mode.
 */
void DTabBar::setElideMode(Qt::TextElideMode mode)
{
    d_func()->setElideMode(mode);
}

#ifndef QT_NO_TOOLTIP

/*!
@~english
  @brief Set the tooltip text of the label at the specified index position.

  @param[in] index Label index., @param[in] tip Tooltip text..
 */
void DTabBar::setTabToolTip(int index, const QString &tip)
{
    d_func()->setTabToolTip(index, tip);
}

/*!
@~english
  @brief Return the tooltip text of the label at the specified index position.

  @param[in] index label index
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
@~english
  @brief Set the data of the label at the specified index position.

  @param[in] index label index, @param[in] data label data
 */
void DTabBar::setTabData(int index, const QVariant &data)
{
    d_func()->setTabData(index, data);
}

/*!
@~english
  @brief Return the data of the label at the specified index position.

  @param[in] index label index
  @return label data
 */
QVariant DTabBar::tabData(int index) const
{
    return d_func()->tabData(index);
}

/*!
@~english
  @brief Return the rectangle of the label at the specified index position.

  @param[in] index Label index.
  @return Label rectangle.
 */
QRect DTabBar::tabRect(int index) const
{
    QRect rect = d_func()->tabRect(index);

    rect.moveTopLeft(d_func()->mapToParent(rect.topLeft()));

    return rect;
}

/*!
@~english
  @brief Return the label index position based on the position.

  @param[in] pos Coordinates of the corresponding label, which can be any coordinates within the label.
  @return Return the index value of the corresponding label.
 */
int DTabBar::tabAt(const QPoint &pos) const
{
    return d_func()->tabAt(d_func()->mapFromParent(pos));
}

/*!
@~english
  @brief Current position of the tab index.

  @return Current tab index.
 */
int DTabBar::currentIndex() const
{
    return d_func()->currentIndex();
}

/*!
@~english
  @brief the total num of labels

  @return return the total num of labels
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
@~english
  @brief the size of icon

  @return return the size of icon
 */
QSize DTabBar::iconSize() const
{
    return d_func()->iconSize();
}

/*!
@~english
  @brief set the size of icon

  @param[in] size the size of icon
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
@~english
  @brief The tab bar is closable or not

  @return Return whether the label is closable.
 */
bool DTabBar::tabsClosable() const
{
    return d_func()->tabsClosable();
}

/*!
@~english
  @brief Set whether the tab bar is closable or not.

  @param[in] closable the label is closable or not
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
    D_D(DTabBar);

    if (enabled == expanding())
        return;
    QTabBarPrivate *dd = reinterpret_cast<QTabBarPrivate *>(qGetPtrHelper(d->d_ptr));
    dd->expanding = enabled;
    auto auto_layout = d->layout;

    if (enabled) {
        auto_layout->removeItem(d->stretch);
        d->stretchIsRemove = true;
        d->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    } else {
        auto_layout->addSpacerItem(d->stretch);
        d->stretchIsRemove = false;
        d->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    }
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
@~english
  @brief 启用直角样式的 Tabbar.

  此风格适用于切换窗口的操作,多用于支持多开的应用.

  @param[in] enable true 启用，false 恢复
 */
void DTabBar::setEnabledEmbedStyle(bool enable)
{
    D_D(DTabBar);

    setProperty("_d_dtk_tabbartab_type", enable);

    int radius;
    QSize size;
    if (enable) {
        radius = DStyle::pixelMetric(style(), DStyle::PM_FloatingWidgetRadius);
        size = QSize(24, 24);
    } else {
        radius = DStyle::pixelMetric(style(), DStyle::PM_FrameRadius);
        size = d->addButton->sizeHint();
    }
    d->addButton->setMaximumSize(size);
    d->rightScrollButton->setMaximumSize(size);
    d->leftScrollButton->setMaximumSize(size);

    DStyle::setFrameRadius(d->rightScrollButton, radius);
    DStyle::setFrameRadius(d->leftScrollButton, radius);
    DStyle::setFrameRadius(d->addButton, radius);
    update();
}

/*!
@~english
  @brief 设置 tab 标签文字对齐方式.

  @param[in] alignment 对齐方式.
 */
void DTabBar::setTabLabelAlignment(Qt::Alignment alignment)
{
    setProperty("_d_dtk_tabbar_alignment", int(alignment));
}

/*!
@~english
  @brief 设置当前标签索引位置.

  @param[in] index 标签索引.
 */
void DTabBar::setCurrentIndex(int index)
{
    d_func()->setCurrentIndex(index);
}

/*!
@~english
  @brief 设置新建标签按钮是否显示.

  @param[in] visibleAddButton 新建按钮是否可视.
 */
void DTabBar::setVisibleAddButton(bool visibleAddButton)
{
    D_D(DTabBar);

    d->visibleAddButton = visibleAddButton;
    d->addButton->setVisible(visibleAddButton);
}

/*!
@~english
  @brief 设置开始拖动距离.

  @param[in] startDragDistance 开始拖动的距离.
 */
void DTabBar::setStartDragDistance(int startDragDistance)
{
    d_func()->startDragDistance = startDragDistance;
}

/*!
@~english
  @brief 设置颜色.

  @param[in] maskColor 遮罩颜色.
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

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    int index = d->tabInsertIndexFromMouse(d->mapFromParent(e->position().toPoint()));
#else
    int index = d->tabInsertIndexFromMouse(d->mapFromParent(e->pos()));
#endif

    if (canInsertFromMimeData(index, e->mimeData())) {
        d->setDragingFromOther(true);
        e->acceptProposedAction();

        // 插入一个虚拟的标签
        if (e->source() != d) {
            d->ghostTabIndex = index;
            insertFromMimeDataOnDragEnter(index, e->mimeData());
            // 延时启动startMove， 此时tabbar的大小还没有更新
            QMetaObject::invokeMethod(d, "startMove", Qt::QueuedConnection, Q_ARG(int, index));
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

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    int index = d->dd()->validIndex(d->ghostTabIndex) ? d->ghostTabIndex : d->tabInsertIndexFromMouse(d->mapFromParent(e->position().toPoint()));
#else
    int index = d->dd()->validIndex(d->ghostTabIndex) ? d->ghostTabIndex : d->tabInsertIndexFromMouse(d->mapFromParent(e->pos()));
#endif
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                QMouseEvent event(QEvent::MouseMove, d->mapFromParent(e->position().toPoint()),
                                  d->mapFromParent(e->position().toPoint()), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
#else
                QMouseEvent event(QEvent::MouseMove, d->mapFromParent(e->pos()),
                                  Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
#endif
                d->mouseMoveEvent(&event);
            } else {
                d->ghostTabIndex = index;
                insertFromMimeDataOnDragEnter(index, e->mimeData());
                d->startMove(index);
            }
        } else {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            d->autoScrollTabs(d->mapFromParent(e->position().toPoint()));
#else
            d->autoScrollTabs(d->mapFromParent(e->pos()));
#endif
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

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    int index = d->tabInsertIndexFromMouse(d->mapFromParent(e->position().toPoint()));
#else
    int index = d->tabInsertIndexFromMouse(d->mapFromParent(e->pos()));
#endif

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
#if QT_VERSION < QT_VERSION_CHECK(5, 13, 0)
    p.initFrom(d_func());
#else
    p.begin(const_cast<DTabBarPrivate *>(d_func()));
#endif
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
    bool is_vertical = dtk_verticalTabs(dd->shape);

    if (is_vertical) {
        size.setWidth(qMax(size.width(), dd->leftB->width()));
    } else {
        size.setHeight(qMax(size.height(), dd->leftB->height()));
    }

    const QSize &min = minimumTabSizeHint(index);
    const QSize &max = maximumTabSizeHint(index);

    size.setWidth(qMax(size.width(), min.width()));
    size.setHeight(qMax(size.height(), min.height()));

    if (max.width() > 0)
        size.setWidth(qMin(size.width(), max.width()));

    if (max.height() > 0)
        size.setHeight(qMin(size.height(), max.height()));

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
