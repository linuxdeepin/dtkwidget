// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "danchors.h"

#include "denhancedwidget.h"

DWIDGET_BEGIN_NAMESPACE

/*!
  \class Dtk::Widget::DAnchorsBase
  \inmodule dtkwidget
  
  \brief DAnchorsBase 提供了一种指定 QWidget 与其它 QWidget 之间的关系来确定
  其位置的方法.
  
  除了比较传统的布局方式之外，DtkWidget 还提供了一种使用锚定概念布局控件的方法（
  类似于 QQuickItem 中的 anchors 属性），可以认为每个控件具有一组6个不可见的“锚
  线”：left，horizontalCenter，right，top，verticalCenter和bottom，如图所示：
  \image html edges_anchors.png
  使用 DAnchors 可以让 QWidget 基于这些“锚线”来确定相互间的关系，如：
  \code
  DAnchors<QLabel> rect1(new QLabel("rect1"));
  DAnchors<QLabel> rect2(new QLabel("rect2"));
  
  rect2.setLeft(rect1.right());
  \endcode
  这样 rect2 的左边界就会和 rect1 的右边界对齐：
  \image html edge1.png
  另外还可以同时设置多个“锚线”：
  \code
  DAnchors<QLabel> rect1(new QLabel("rect1"));
  DAnchors<QLabel> rect2(new QLabel("rect2"));
  
  rect2.setTop(rect1.bottom());
  rect2.setLeft(rect1.right());
  \endcode
  \image html edge3.png
  锚定布局同时在多个控件中使用，控件之间只需要满足以下条件：
  \a 控件之间为兄弟关系，或被锚定控件为父控件
  \a 锚定关系不能循环绑定
  \section1 margin_offset 锚定的间隔和偏移
  锚定系统允许设置“锚线”之间的间距，和“锚线”一一对应，每个控件都有一组4个 margin：
  leftMargin, rightMargin, topMargin 和 bottomMargin 以及两个 offset：
  horizontalCenterOffset 和 verticalCenterOffset。
  \image html margins_anchors.png
  下面是左margin的例子：
  \code
  DAnchors<QLabel> rect1(new QLabel("rect1"));
  DAnchors<QLabel> rect2(new QLabel("rect2"));
  
  rect2.setLeftMargin(5);
  rect2.setLeft(rect1.right());
  \endcode
  rect2 的左边界相距 rect1 的右边界5个像素：
  \image html edge2.png
  \note margin 仅仅是对设置的锚点生效，并不是让控件本身增加了边距，如果设置了
  margin，但并没有设置相应的锚点，对控件本身而已是没有任何影响的。margin 的值可以
  为负数，通过值的正负来决定margin的方向（内 margin 还是外 margin）
  
  除了基于“锚线”来设置锚定外，另外还有 setCenterIn 和 setFill 这两个比较特殊的
  的实现。
  
  \section1 loop_anchor 判断循环锚定的方式
  假设 DAnchorsBase a1, a2; a1.setRight(a2.left()); 则判断 a1 和 a2 之间
  会不会存在循环绑定的逻辑为:
  尝试更改 a1 右边界的值，更新后如果 a2 左边界的值产出了变化，则认为会导致循环绑
  定，否则认为不存在
 */

/*!
  \property DAnchorsBase::target
  \brief 绑定了锚定功能的控件对象
  \note 只读
  */

/*!
  \property DAnchorsBase::enabled
  \brief 控制锚定功能是否开启，为 false 时仅仅表示不会根据控件各种属性的变化来
  来更新它的位置，但锚定关系并没有被解除
  \note 可读可写
  */

/*!
  \property DAnchorsBase::anchors
  \brief 一个指向自己的指针
  \note 只读
  */

/*!
  \property DAnchorsBase::top
  \brief target 控件上边界锚线的信息
  \note 只能和 top verticalCenter bottom 绑定
  \note 对属性赋值不会更改它自身的值，而是对此锚线设置绑定关系
  \note 可读可写
  */

/*!
  \property DAnchorsBase::bottom
  \note 只能和 top verticalCenter bottom 绑定
  \brief target 控件下边界锚线的信息
  \note 对属性赋值不会更改它自身的值，而是对此锚线设置绑定关系
  \note 可读可写
  */

/*!
  \property DAnchorsBase::left
  \note 只能和 left horizontalCenter right 绑定
  \brief target 控件左边界锚线的信息
  \note 对属性赋值不会更改它自身的值，而是对此锚线设置绑定关系
  \note 可读可写
  */

/*!
  \property DAnchorsBase::right
  \note 只能和 left horizontalCenter right 绑定
  \brief target 控件右边界锚线的信息
  \note 对属性赋值不会更改它自身的值，而是对此锚线设置绑定关系
  \note 可读可写
  */

/*!
  \property DAnchorsBase::horizontalCenter
  \note 只能和 left horizontalCenter right 绑定
  \brief target 控件水平锚线的信息
  \note 对属性赋值不会更改它自身的值，而是对此锚线设置绑定关系
  \note 可读可写
  */

/*!
  \property DAnchorsBase::verticalCenter
  \note 只能和 top verticalCenter bottom 绑定
  \brief target 控件竖直锚线的信息
  \note 对属性赋值不会更改它自身的值，而是对此锚线设置绑定关系
  \note 可读可写
  */

/*!
  \property DAnchorsBase::fill
  \brief target 控件的填充目标对象
  \note 可读可写
  */

/*!
  \property DAnchorsBase::centerIn
  \brief target 控件的居中目标对象
  \note 可读可写
  */

/*!
  \property DAnchorsBase::margins
  \brief 上下左右四条“锚线”的边距，此值的优先级低于每条“锚线”特定的 margin 值
  \note 可读可写
  */

/*!
  \property DAnchorsBase::topMargin
  \brief 上“锚线”的边距，优先级高于 margins
  \note 可读可写
  */

/*!
  \property DAnchorsBase::bottomMargin
  \brief 下“锚线”的边距，优先级高于 margins
  \note 可读可写
  */

/*!
  \property DAnchorsBase::leftMargin
  \brief 左“锚线”的边距，优先级高于 margins
  \note 可读可写
  */

/*!
  \property DAnchorsBase::rightMargin
  \brief 右“锚线”的边距，优先级高于 margins
  \note 可读可写
  */

/*!
  \property DAnchorsBase::horizontalCenterOffset
  \brief 水平“锚线”的偏移量
  \note 可读可写
  */

/*!
  \property DAnchorsBase::verticalCenterOffset
  \brief 竖直“锚线”的偏移量
  \note 可读可写
  */

/*!
  \enum Dtk::Widget::DAnchorsBase::AnchorError
  DAnchorsBase::AnchorError 设置锚定信息的过程中可能出现的错误类型
  
  \value NoError
  设置锚定的过程中没有任何错误发生
  
  \value Conflict
  表示设置的锚定关系跟已有关系存在冲突，如 fill 和 centerIn 不能同时设置
  
  \value TargetInvalid
  表示设置锚定关系时的目标控件无效
  
  \value PointInvalid
  表示设置锚定关系时的“锚线”信息错误，如把 Qt::AnchorLeft 设置到了 Qt::AnchorTop 上
  
  \value LoopBind
  表示设置的锚定关系和已有关系形成了循环绑定
 */

/*!
  \fn void DAnchorsBase::enabledChanged(bool enabled)
  \brief 信号会在 \a enabled 属性的值改变时被发送
 */
/*!
  \fn void DAnchorsBase::topChanged(const DAnchorInfo *top)
  \brief 信号会在 \a top 属性的值改变时被发送
 */
/*!
  \fn void DAnchorsBase::bottomChanged(const DAnchorInfo *bottom)
  \brief 信号会在 \a bottom 属性的值改变时被发送
 */
/*!
  \fn void DAnchorsBase::leftChanged(const DAnchorInfo *left)
  \brief 信号会在 \a left 属性的值改变时被发送
 */
/*!
  \fn void DAnchorsBase::rightChanged(const DAnchorInfo *right)
  \brief 信号会在 \a right 属性的值改变时被发送
 */
/*!
  \fn void DAnchorsBase::horizontalCenterChanged(const DAnchorInfo *horizontalCenter)
  \brief 信号会在 \a horizontalCenter 属性的值改变时被发送
 */
/*!
  \fn void DAnchorsBase::verticalCenterChanged(const DAnchorInfo *verticalCenter)
  \brief 信号会在 \a verticalCenter 属性的值改变时被发送
 */
/*!
  \fn void DAnchorsBase::fillChanged(QWidget *fill)
  \brief 信号会在 \a fill 属性的值改变时被发送
 */
/*!
  \fn void DAnchorsBase::centerInChanged(QWidget *centerIn)
  \brief 信号会在 \a centerIn 属性的值改变时被发送
 */
/*!
  \fn void DAnchorsBase::marginsChanged(int margins)
  \brief 信号会在 \a margins 属性的值改变时被发送
 */
/*!
  \fn void DAnchorsBase::topMarginChanged(int topMargin)
  \brief 信号会在 \a topMargin 属性的值改变时被发送
 */
/*!
  \fn void DAnchorsBase::bottomMarginChanged(int bottomMargin)
  \brief 信号会在 \a bottomMargin 属性的值改变时被发送
 */
/*!
  \fn void DAnchorsBase::leftMarginChanged(int leftMargin)
  \brief 信号会在 \a leftMargin 属性的值改变时被发送
 */
/*!
  \fn DAnchorsBase::rightMarginChanged(int rightMargin)
  \brief 信号会在 \a rightMargin 属性的值改变时被发送
 */
/*!
  \fn DAnchorsBase::horizontalCenterOffsetChanged(int horizontalCenterOffset)
  \brief 信号会在 \a horizontalCenterOffset 属性的值改变时被发送
 */
/*!
  \fn DAnchorsBase::verticalCenterOffsetChanged(int verticalCenterOffset)
  \brief 信号会在 \a verticalCenterOffset 属性的值改变时被发送
 */

/*!
  \class Dtk::Widget::DAnchors
  \inmodule dtkwidget
  \brief DAnchors 是一个模板类，在 DAnchorsBase 的基础上保存了一个控件指针，
  将控件和锚定绑定在一起使用，相当于把“锚线”属性附加到了控件本身.
  
  重载了 “->”、“*”、“&” 等运算符，用于把 DAnchors 这层封装透明化，尽量减少使用
  DAnchors<QWidget> 和直接使用 QWidget* 对象的区别。
 */

/*!
  \class Dtk::Widget::DAnchorInfo
  \inmodule dtkwidget
  \brief DAnchorInfo 用于记录“锚线”的锚定信息：被锚定的 DAnchorsBase 对象、
  锚定的类型、目标“锚线”的信息.
  
  每条锚线都和一个 DAnchorInfo 对象相对应。一般来说，在使用锚定布局时，只需要关心
  “锚线”的绑定关系，不用关心 DAnchorInfo 中存储的数据。
 */

class DAnchorsRect: public QRect
{
public:
    DAnchorsRect() {}
    DAnchorsRect(const QRect &rect): QRect(rect.topLeft(), rect.bottomRight()) {}

    const QRect &operator=(const QRect &rect)
    {
        setTopLeft(rect.topLeft());
        setBottomRight(rect.bottomRight());

        return *this;
    }

    inline qreal horizontalCenter()
    {
        return left() + width() / 2.0;
    }

    inline void moveHorizontalCenter(int arg)
    {
        moveLeft(arg - width() / 2.0);
    }

    inline qreal verticalCenter()
    {
        return top() + height() / 2.0;
    }

    inline void moveVerticalCenter(int arg)
    {
        moveTop(arg - height() / 2.0);
    }

    void setTop(int arg, Qt::AnchorPoint point);
    void setVerticalCenter(qreal arg, Qt::AnchorPoint point);
    void setBottom(int arg, Qt::AnchorPoint point);
    void setLeft(int arg, Qt::AnchorPoint point);
    void setHorizontalCenter(qreal arg, Qt::AnchorPoint point);
    void setRight(int arg, Qt::AnchorPoint point);
};

class DAnchorsBasePrivate : public QSharedData
{
    DAnchorsBasePrivate(DAnchorsBase *qq): q_ptr(qq) {}
    ~DAnchorsBasePrivate()
    {
        doDestory();
    }
    void tryDestory()
    {
        // TODO 应该是析构函数调用, 当ref.load() == 1时，提前释放资源。
        if (ref.load() == 1)
            doDestory();
    }
    void doDestory()
    {
#define SAFE_DELETE(pointer) \
    if (pointer) { \
        delete pointer; \
        pointer = nullptr; \
    }

        SAFE_DELETE(top);
        SAFE_DELETE(bottom);
        SAFE_DELETE(left);
        SAFE_DELETE(right);
        SAFE_DELETE(horizontalCenter);
        SAFE_DELETE(verticalCenter);

#undef SAFE_DELETE
    }

    static void setWidgetAnchorsBase(const QWidget *w, DAnchorsBase *b)
    {
        if (w) {
            DAnchorsBase *bb = widgetMap.value(w, NULL);
            if (bb) {
                bb->deleteLater();
            }
            widgetMap[w] = b;
        }
    }
    static DAnchorsBase *getWidgetAnchorsBase(const QWidget *w)
    {
        return widgetMap.value(w, NULL);
    }
    static void removeWidgetAnchorsBase(const QWidget *w, const DAnchorsBase *b)
    {
        if (w && b && widgetMap.value(w, NULL) == b) {
            widgetMap.remove(w);
        }
    }

    const DAnchorInfo *getInfoByPoint(const Qt::AnchorPoint &p) const
    {
        switch (p) {
        case Qt::AnchorTop:
            return top;
        case Qt::AnchorBottom:
            return bottom;
        case Qt::AnchorLeft:
            return left;
        case Qt::AnchorRight:
            return right;
        case Qt::AnchorHorizontalCenter:
            return horizontalCenter;
        case Qt::AnchorVerticalCenter:
            return verticalCenter;
        default:
            return NULL;
        }
    }

    bool checkInfo(const DAnchorInfo *info1, const DAnchorInfo *info2) const
    {
        const Qt::AnchorPoint &p = info2->type;

        switch (info1->type) {
        case Qt::AnchorTop://Deliberate
        case Qt::AnchorBottom://Deliberate
        case Qt::AnchorVerticalCenter:
            return (p == Qt::AnchorTop || p == Qt::AnchorBottom || p == Qt::AnchorVerticalCenter);
        case Qt::AnchorLeft://Deliberate
        case Qt::AnchorRight://Deliberate
        case Qt::AnchorHorizontalCenter:
            return (p == Qt::AnchorLeft || p == Qt::AnchorRight || p == Qt::AnchorHorizontalCenter);
        default:
            return false;
        }
    }

    bool isBindable(const DAnchorInfo *info) const
    {
        if (fill->target() || centerIn->target()) {
            return false;
        }

        Q_Q(const DAnchorsBase);

        bool tmp1 = ((int)q->isBinding(top) + (int)q->isBinding(verticalCenter) + (int)q->isBinding(bottom)) < 2;
        bool tmp2 = ((int)q->isBinding(left) + (int)q->isBinding(horizontalCenter) + (int)q->isBinding(right)) < 2;

        switch (info->type) {
        case Qt::AnchorTop://Deliberate
        case Qt::AnchorBottom://Deliberate
        case Qt::AnchorHorizontalCenter:
            return tmp1;
        case Qt::AnchorLeft://Deliberate
        case Qt::AnchorRight://Deliberate
        case Qt::AnchorVerticalCenter:
            return tmp2;
        default:
            return false;
        }
    }

    qreal getValueByInfo(const DAnchorInfo *info)
    {
        DAnchorsRect rect = info->base->target()->geometry();

        switch (info->type) {
        case Qt::AnchorTop:
            return rect.top();
        case Qt::AnchorBottom:
            return rect.bottom() + 1;
        case Qt::AnchorHorizontalCenter:
            return rect.horizontalCenter();
        case Qt::AnchorLeft:
            return rect.left();
        case Qt::AnchorRight:
            return rect.right() + 1;
        case Qt::AnchorVerticalCenter:
            return rect.verticalCenter();
        default:
            return 0;
        }
    }

    void setValueByInfo(qreal value, const DAnchorInfo *info)
    {
        if (!info) {
            return;
        }

        switch (info->type) {
        case Qt::AnchorTop:
            info->base->setTop(value, Qt::AnchorTop);
            break;
        case Qt::AnchorBottom:
            info->base->setBottom(value - 1, Qt::AnchorTop);
            break;
        case Qt::AnchorHorizontalCenter:
            info->base->setHorizontalCenter(value, Qt::AnchorLeft);
            break;
        case Qt::AnchorLeft:
            info->base->setLeft(value, Qt::AnchorRight);
            break;
        case Qt::AnchorRight:
            info->base->setRight(value - 1, Qt::AnchorLeft);
            break;
        case Qt::AnchorVerticalCenter:
            info->base->setVerticalCenter(value, Qt::AnchorLeft);
            break;
        default:
            break;
        }
    }

    qreal getTargetValueByInfo(const DAnchorInfo *info)
    {
        if (!info->targetInfo) {
            return getValueByInfo(info);
        }

        qreal value = getValueByInfo(info->targetInfo);
        bool isParent = info->base->target()->parentWidget() == info->targetInfo->base->target();
        int topValue = isParent ? -info->targetInfo->base->target()->geometry().top() : 0;
        int leftValue = isParent ? -info->targetInfo->base->target()->geometry().left() : 0;

        switch (info->type) {
        case Qt::AnchorTop: {
            int offset = topMargin == 0 ? margins : topMargin;
            return value + offset + topValue;
        }
        case Qt::AnchorBottom: {
            int offset = bottomMargin == 0 ? margins : bottomMargin;
            return value - offset + topValue - 1;
        }
        case Qt::AnchorHorizontalCenter: {
            int offset = horizontalCenterOffset;
            return value + offset + leftValue;
        }
        case Qt::AnchorLeft: {
            int offset = leftMargin == 0 ? margins : leftMargin;
            return value + offset + leftValue;
        }
        case Qt::AnchorRight: {
            int offset = rightMargin == 0 ? margins : rightMargin;
            return value - offset + leftValue - 1;
        }
        case Qt::AnchorVerticalCenter: {
            int offset = verticalCenterOffset;
            return value + offset + topValue;
        }
        default:
            return 0;
        }
    }

    const DAnchorsRect getWidgetRect(const QWidget *w) const
    {
        if (!w) {
            return DAnchorsRect();
        }

        if (extendWidget && extendWidget->target()->parentWidget() == w) {
            return w->rect();
        }

        return w->geometry();
    }

    int horizontalAnchorCount() const
    {
        Q_Q(const DAnchorsBase);

        int count = 0;
        if(q->isBinding(left))
            ++count;
        if(q->isBinding(horizontalCenter))
            ++count;
        if(q->isBinding(right))
            ++count;

        return count;
    }

    int verticalAnchorCount() const
    {
        Q_Q(const DAnchorsBase);

        int count = 0;
        if(q->isBinding(top))
            ++count;
        if(q->isBinding(verticalCenter))
            ++count;
        if(q->isBinding(bottom))
            ++count;

        return count;
    }

    DAnchorsBase *q_ptr;

    QPointer<DEnhancedWidget> extendWidget;
    DAnchorInfo *top = new DAnchorInfo(q_ptr, Qt::AnchorTop);
    DAnchorInfo *bottom = new DAnchorInfo(q_ptr, Qt::AnchorBottom);
    DAnchorInfo *left = new DAnchorInfo(q_ptr, Qt::AnchorLeft);
    DAnchorInfo *right = new DAnchorInfo(q_ptr, Qt::AnchorRight);
    DAnchorInfo *horizontalCenter = new DAnchorInfo(q_ptr, Qt::AnchorHorizontalCenter);
    DAnchorInfo *verticalCenter = new DAnchorInfo(q_ptr, Qt::AnchorVerticalCenter);
    DEnhancedWidget *fill = new DEnhancedWidget(NULL, q_ptr);
    DEnhancedWidget *centerIn = new DEnhancedWidget(NULL, q_ptr);
    int margins = 0;
    int topMargin = 0;
    int bottomMargin = 0;
    int leftMargin = 0;
    int rightMargin = 0;
    int horizontalCenterOffset = 0;
    int verticalCenterOffset = 0;
    bool alignWhenCentered = false;
    DAnchorsBase::AnchorError errorCode = DAnchorsBase::NoError;
    QString errorString;
    static QMap<const QWidget *, DAnchorsBase *> widgetMap;

    Q_DECLARE_PUBLIC(DAnchorsBase)
};

QMap<const QWidget *, DAnchorsBase *> DAnchorsBasePrivate::widgetMap;

/*!
  \brief 构造 DAnchorsBase 对象，传入的 w 对象会和一个新的 DAnchorsBase 对象
  绑定到一起
  \a w 需要使用锚定关系的控件
  \note 对 w 设置的锚定关系不会随着本次构造的 DAnchorsBase 对象的销毁而消失。
  此构造函数可能会隐式的构造一个新 DAnchorsBase 对象用于真正的功能实现，函数执行
  时会先检查当前是否已经有和 w 对象绑定的 DAnchorsBase 对象，如果没有则会创建一
  个新的 DAnchorsBase 对象与之绑定，否则使用已有的对象。隐式创建的 DAnchorsBase
  对象会在对应的 QWidget 对象被销毁时自动销毁。
  \sa target() clearAnchors() getAnchorBaseByWidget()
 */
DAnchorsBase::DAnchorsBase(QWidget *w):
    QObject(w)
{
    init(w);
}

/*!
  \brief 在析构时会判断此 DAnchorsBase 对象是否和 target 存在绑定关系，如果是
  则从映射表中移除绑定
  \warning DAnchorsBasePrivate 对象可能是在多个 DAnchorsBase 对象之间显式
  共享的，所以在销毁 DAnchorsBase 后，对应的 DAnchorsBasePrivate 对象不一定
  会被销毁
  \sa QExplicitlySharedDataPointer
 */
DAnchorsBase::~DAnchorsBase()
{
    DAnchorsBasePrivate::removeWidgetAnchorsBase(target(), this);
    /** TODO dynamic_cast会报警告`QSharedData is not polymorphic`，因为没有虚函数.
    因为由QExplicitlySharedDataPointer管理, QSharedData没有虚析构，
    导致delete QSharedData时，不会调用子类~DAnchorsBasePrivate.
    之后改成QExplicitlySharedDataPointer<DAnchorsBasePrivate> d_ptr，
    d_ptr计数为0会调用~DAnchorsBasePrivate
    */
    if (auto p = reinterpret_cast<DAnchorsBasePrivate*>(d_ptr.data())) {
        p->tryDestory();
    }
}

QWidget *DAnchorsBase::target() const
{
    Q_D(const DAnchorsBase);

    if (!d->extendWidget)
        return 0;

    return d->extendWidget->target();
}

/*!
  \brief 返回 target 控件的扩展对象。此对象为 QWidget 对象额外提供了和控件大小、
  位置相关的变化信号
  \return
  \sa Dtk::Widget::DEnhancedWidget
 */
DEnhancedWidget *DAnchorsBase::enhancedWidget() const
{
    Q_D(const DAnchorsBase);

    return d->extendWidget;
}

bool DAnchorsBase::enabled() const
{
    Q_D(const DAnchorsBase);

    if (!d->extendWidget)
        return false;

    return d->extendWidget->enabled();
}

const DAnchorsBase *DAnchorsBase::anchors() const
{
    return this;
}

const DAnchorInfo *DAnchorsBase::top() const
{
    Q_D(const DAnchorsBase);

    return d->top;
}

const DAnchorInfo *DAnchorsBase::bottom() const
{
    Q_D(const DAnchorsBase);

    return d->bottom;
}

const DAnchorInfo *DAnchorsBase::left() const
{
    Q_D(const DAnchorsBase);

    return d->left;
}

const DAnchorInfo *DAnchorsBase::right() const
{
    Q_D(const DAnchorsBase);

    return d->right;
}

const DAnchorInfo *DAnchorsBase::horizontalCenter() const
{
    Q_D(const DAnchorsBase);

    return d->horizontalCenter;
}

const DAnchorInfo *DAnchorsBase::verticalCenter() const
{
    Q_D(const DAnchorsBase);

    return d->verticalCenter;
}

QWidget *DAnchorsBase::fill() const
{
    Q_D(const DAnchorsBase);

    if (d->fill) {
        return d->fill->target();
    }

    return NULL;
}

QWidget *DAnchorsBase::centerIn() const
{
    Q_D(const DAnchorsBase);

    if (d->centerIn) {
        return d->centerIn->target();
    }

    return NULL;
}

int DAnchorsBase::margins() const
{
    Q_D(const DAnchorsBase);

    return d->margins;
}

int DAnchorsBase::topMargin() const
{
    Q_D(const DAnchorsBase);

    return d->topMargin;
}

int DAnchorsBase::bottomMargin() const
{
    Q_D(const DAnchorsBase);

    return d->bottomMargin;
}

int DAnchorsBase::leftMargin() const
{
    Q_D(const DAnchorsBase);

    return d->leftMargin;
}

int DAnchorsBase::rightMargin() const
{
    Q_D(const DAnchorsBase);

    return d->rightMargin;
}

int DAnchorsBase::horizontalCenterOffset() const
{
    Q_D(const DAnchorsBase);

    return d->horizontalCenterOffset;
}

int DAnchorsBase::verticalCenterOffset() const
{
    Q_D(const DAnchorsBase);

    return d->verticalCenterOffset;
}

int DAnchorsBase::alignWhenCentered() const
{
    Q_D(const DAnchorsBase);

    return d->alignWhenCentered;
}

/*!
  \brief 锚定过程中产生的错误，在一个新的锚定函数被调用之前会清空此错误状态，每次
  调用锚定函数后，可以通过此函数的返回值来判断锚定设置是否成功
  \return
  \sa errorString()
 */
DAnchorsBase::AnchorError DAnchorsBase::errorCode() const
{
    Q_D(const DAnchorsBase);

    return d->errorCode;
}

/*!
  \brief 对 errorCode 的文本描述信息
  \return
  \sa errorCode
 */
QString DAnchorsBase::errorString() const
{
    Q_D(const DAnchorsBase);

    return d->errorString;
}

/*!
  \brief 如果此 info 设置了锚定对象，则返回 true ，否则返回 false
  \code
  DAnchors<QWidget> w1;
  DAnchors<QWidget> w2;
  
  w1.setLeft(w2.right());
  
  qDebug() << w1.isBinding(w1.left()) << w2.isBinding(w2.right());
  \endcode
  打印内容为：ture false
  \a info
  \return
 */
bool DAnchorsBase::isBinding(const DAnchorInfo *info) const
{
    return info->targetInfo;
}

/*!
  \brief 方便用户直接设置两个对象之间锚定关系的静态函数，调用此函数可能会隐式创建
  DAnchorsBase 对象
  \a w 要锚定的控件对象
  \a p 要锚定的锚线/锚点
  \a target 锚定的目标对象
  \a point 锚定的目标锚线/锚点
  \return 如果锚定成功，则返回 true，否则返回 false
 */
bool DAnchorsBase::setAnchor(QWidget *w, const Qt::AnchorPoint &p, QWidget *target, const Qt::AnchorPoint &point)
{
    if (!w || !target) {
        return false;
    }

    DAnchorsBase *base = DAnchorsBasePrivate::getWidgetAnchorsBase(w);
    if (!base) {
        base = new DAnchorsBase(w);
    }

    return base->setAnchor(p, target, point);
}

/*!
  \brief 清除和控件 w 相关的所有锚定关系，包括锚定w或者被w锚定的任何关联。会直接
  销毁 w 对应的 DAnchorsBase 对象
  \a w
 */
void DAnchorsBase::clearAnchors(const QWidget *w)
{
    DAnchorsBase *base = DAnchorsBasePrivate::getWidgetAnchorsBase(w);
    if (base) {
        delete base;
    }
}

/*!
  \brief 返回与 w 绑定的 DAnchorsBase 对象
  \a w
  \return 如果 w 没有对应的锚定对象，则返回空
 */
DAnchorsBase *DAnchorsBase::getAnchorBaseByWidget(const QWidget *w)
{
    return DAnchorsBasePrivate::getWidgetAnchorsBase(w);
}

void DAnchorsBase::setEnabled(bool enabled)
{
    Q_D(DAnchorsBase);

    if (!d->extendWidget)
        return;

    d->extendWidget->setEnabled(enabled);
}

/*!
  \brief 为 DAnchorsBase::target 对象设置锚定规则
  \note 可能会为目标控件隐式创建其对应的 DAnchorsBase 对象
  \a p 为当前控件的哪个锚线/锚点设置锚定规则
  \a target 锚定的目标控件
  \a point 锚定的目标锚线/锚点
  \return 如果设置成功，则返回 true，否则返回 false
 */
bool DAnchorsBase::setAnchor(const Qt::AnchorPoint &p, QWidget *target, const Qt::AnchorPoint &point)
{
    if (!target) {
        return false;
    }

    DAnchorsBase *base = DAnchorsBasePrivate::getWidgetAnchorsBase(target);

    if (!base) {
        base = new DAnchorsBase(target);
    }

    const DAnchorInfo *info = base->d_func()->getInfoByPoint(point);

    switch (p) {
    case Qt::AnchorTop:
        return setTop(info);
    case Qt::AnchorBottom:
        return setBottom(info);
    case Qt::AnchorLeft:
        return setLeft(info);
    case Qt::AnchorRight:
        return setRight(info);
    case Qt::AnchorHorizontalCenter:
        return setHorizontalCenter(info);
    case Qt::AnchorVerticalCenter:
        return setVerticalCenter(info);
    default:
        return false;
    }
}

#define ANCHOR_BIND_INFO(point, Point, slotName, ...)\
    Q_D(DAnchorsBase);\
    if(*d->point == point)\
        return true;\
    DEnhancedWidget *tmp_w1 = NULL;\
    DEnhancedWidget *tmp_w2 = NULL;\
    if(d->point->targetInfo){\
        tmp_w1 = d->point->targetInfo->base->d_func()->extendWidget;\
    }\
    QStringList signalList = QString(#__VA_ARGS__).split("),");\
    if(point){\
        if(!d->isBindable(d->point)){\
            d->errorCode = Conflict;\
            d->errorString = "Conflict: CenterIn or Fill is anchored.";\
            return false;\
        }\
        if (point->base == d->q_func()){\
            d->errorCode = TargetInvalid;\
            d->errorString = "Cannot anchor widget to self.";\
            return false;\
        }else if(target()->parentWidget() != point->base->target()){\
            bool isBrother = false;\
            Q_FOREACH (const QWidget *w, target()->parentWidget()->findChildren<QWidget*>()) {\
                if(w == point->base->target()){\
                    isBrother = true;\
                    break;\
                }\
            }\
            if(!isBrother){\
                d->errorCode = TargetInvalid;\
                d->errorString = "Cannot anchor to an widget that isn't a parent or sibling.";\
                return false;\
            }\
        }\
        if(!d->checkInfo(d->point, point)){\
            d->errorCode = PointInvalid;\
            d->errorString = "Cannot anchor a vertical/horizontal edge to a horizontal/vertical edge.";\
            return false;\
        }\
        int old_pos = d->getValueByInfo(point);\
        DAnchorInfo old_info = *d->point;\
        *d->point = point;\
        slotName();\
        if(old_pos != d->getValueByInfo(point)){\
            *d->point = old_info;\
            slotName();\
            d->errorCode = PointInvalid;\
            d->errorString = "loop bind.";\
            return false;\
        }else{\
            old_pos = d->getValueByInfo(d->point);\
            int target_old_value = d->getValueByInfo(point);\
            /*保存widget当然的一些标志位，setValueByInfo仅仅是尝试设置，调用之后要复原标志位*/\
            bool moved = point->base->target()->testAttribute(Qt::WA_Moved);\
            bool resized = point->base->target()->testAttribute(Qt::WA_Resized);\
            d->setValueByInfo(target_old_value + 1, point);\
            if(old_pos != d->getValueByInfo(d->point)){\
                *d->point = old_info;\
                slotName();\
                d->setValueByInfo(target_old_value, point);\
                point->base->target()->setAttribute(Qt::WA_Moved, moved);\
                point->base->target()->setAttribute(Qt::WA_Resized, resized);\
                d->errorCode = PointInvalid;\
                d->errorString = "loop bind.";\
                return false;\
            }\
            d->setValueByInfo(target_old_value, point);\
            point->base->target()->setAttribute(Qt::WA_Moved, moved);\
            point->base->target()->setAttribute(Qt::WA_Resized, resized);\
        }\
        tmp_w2 = point->base->d_func()->extendWidget;\
        if(tmp_w1 != tmp_w2){\
            Q_FOREACH(QString str, signalList){\
                QByteArray arr = str.replace(" ", "").toLatin1();\
                if(arr.right(1) != ")") arr += ")";\
                if(tmp_w1) {\
                    disconnect(tmp_w1, QByteArray("2"+arr).data(), d->q_func(), SLOT(slotName()));\
                    disconnect(tmp_w1, SIGNAL(showed()), d->q_func(), SLOT(slotName()));\
                }\
                if(arr.size() != 13 || target()->parentWidget() != point->base->target()) {\
                    connect(tmp_w2, QByteArray("2"+arr).data(), d->q_func(), SLOT(slotName()));\
                    connect(tmp_w2, SIGNAL(showed()), d->q_func(), SLOT(slotName()));\
                }\
            }\
        }\
    }else{\
        Q_FOREACH(QString str, signalList){\
            QByteArray arr = str.replace(" ", "").toLatin1();\
            if(arr.right(1) != ")") arr += ")";\
            disconnect(tmp_w1, QByteArray("2"+arr).data(), d->q_func(), SLOT(slotName()));\
            disconnect(tmp_w1, SIGNAL(showed()), d->q_func(), SLOT(slotName()));\
        }\
        *d->point = point;\
    }\
    if((isBinding(d->right) || isBinding(d->horizontalCenter)) && d->horizontalAnchorCount() == 1)\
    {connect(d->extendWidget, SIGNAL(widthChanged(int)), d->q_func(), SLOT(updateHorizontal()));}\
    else disconnect(d->extendWidget, SIGNAL(widthChanged(int)), d->q_func(), SLOT(updateHorizontal()));\
    if((isBinding(d->bottom) || isBinding(d->verticalCenter)) && d->verticalAnchorCount() == 1)\
    {connect(d->extendWidget, SIGNAL(heightChanged(int)), d->q_func(), SLOT(updateVertical()));}\
    else disconnect(d->extendWidget, SIGNAL(heightChanged(int)), d->q_func(), SLOT(updateVertical()));\
    Q_EMIT point##Changed(d->point);\
    return true;\

#define ANCHOR_BIND_WIDGET(point, Point)\
    if(d->point->target() == point)\
        return true;\
    if(point){\
        if (point == target()){\
            d->errorCode = TargetInvalid;\
            d->errorString = "Cannot anchor widget to self.";\
            return false;\
        }else if(target()->parentWidget() != point){\
            bool isBrother = false;\
            Q_FOREACH (const QWidget *w, target()->parentWidget()->findChildren<QWidget*>()) {\
                if(w == point){\
                    isBrother = true;\
                    break;\
                }\
            }\
            if(!isBrother){\
                d->errorCode = TargetInvalid;\
                d->errorString = "Cannot anchor to an widget that isn't a parent or sibling.";\
                return false;\
            }\
        }\
        QRect old_rect = point->geometry();\
        QWidget *old_widget = d->point->target();\
        d->point->setTarget(point);\
        update##Point();\
        if(old_rect != point->geometry()){\
            d->point->setTarget(old_widget);\
            update##Point();\
            d->errorCode = PointInvalid;\
            d->errorString = "loop bind.";\
            return false;\
        }\
        DAnchorInfo *info = NULL;\
        setTop(info);setLeft(info);setRight(info);setBottom(info);setHorizontalCenter(info);setVerticalCenter(info);setCenterIn((QWidget*)NULL);\
        if(d->point == d->fill)\
            setCenterIn((QWidget*)NULL);\
        if(target()->parentWidget() == point)\
            disconnect(d->point, SIGNAL(positionChanged(QPoint)), d->q_func(), SLOT(update##Point()));\
        else connect(d->point, SIGNAL(positionChanged(QPoint)), d->q_func(), SLOT(update##Point()));\
    }\
    d->point->setTarget(point);\
    if(d->centerIn){connect(d->extendWidget, SIGNAL(sizeChanged(QSize)), d->q_func(), SLOT(updateCenterIn()));}\
    else disconnect(d->extendWidget, SIGNAL(sizeChanged(QSize)), d->q_func(), SLOT(updateCenterIn()));\
    Q_EMIT point##Changed(point);\
    return true;\

bool DAnchorsBase::setTop(const DAnchorInfo *top)
{
    ANCHOR_BIND_INFO(top, Top, updateVertical , yChanged(int), heightChanged(int))
}

bool DAnchorsBase::setBottom(const DAnchorInfo *bottom)
{
    ANCHOR_BIND_INFO(bottom, Bottom, updateVertical , yChanged(int), heightChanged(int))
}

bool DAnchorsBase::setLeft(const DAnchorInfo *left)
{
    ANCHOR_BIND_INFO(left, Left, updateHorizontal, xChanged(int), widthChanged(int))
}

bool DAnchorsBase::setRight(const DAnchorInfo *right)
{
    ANCHOR_BIND_INFO(right, Right, updateHorizontal, xChanged(int), widthChanged(int))
}

bool DAnchorsBase::setHorizontalCenter(const DAnchorInfo *horizontalCenter)
{
    ANCHOR_BIND_INFO(horizontalCenter, HorizontalCenter, updateHorizontal, xChanged(int), widthChanged(int))
}

bool DAnchorsBase::setVerticalCenter(const DAnchorInfo *verticalCenter)
{
    ANCHOR_BIND_INFO(verticalCenter, VerticalCenter, updateVertical , yChanged(int), heightChanged(int))
}

bool DAnchorsBase::setFill(QWidget *fill)
{
    Q_D(DAnchorsBase);

    ANCHOR_BIND_WIDGET(fill, Fill)
}

bool DAnchorsBase::setCenterIn(QWidget *centerIn)
{
    Q_D(DAnchorsBase);

    if (centerIn && d->fill->target()) {
        d->errorCode = Conflict;
        d->errorString = "Conflict: Fill is anchored.";
        return false;
    }

    ANCHOR_BIND_WIDGET(centerIn, CenterIn)
}

/*!
  \brief 将 fill 中的target()作为参数调用其它重载函数
  \a fill
  \return
 */
bool DAnchorsBase::setFill(DAnchorsBase *fill)
{
    return setFill(fill->target());
}

/*!
  \brief 将 centerIn 中的target()作为参数调用其它重载函数
  \a centerIn
  \return
 */
bool DAnchorsBase::setCenterIn(DAnchorsBase *centerIn)
{
    return setCenterIn(centerIn->target());
}

void DAnchorsBase::setMargins(int margins)
{
    Q_D(DAnchorsBase);

    if (d->margins == margins) {
        return;
    }

    d->margins = margins;

    if (margins != 0) {
        if (d->fill->target()) {
            updateFill();
        } else {
            updateVertical();
            updateHorizontal();
        }
    }

    Q_EMIT marginsChanged(margins);
}

void DAnchorsBase::setTopMargin(int topMargin)
{
    Q_D(DAnchorsBase);

    if (d->topMargin == topMargin) {
        return;
    }

    d->topMargin = topMargin;

    if (d->fill->target()) {
        updateFill();
    } else if (isBinding(d->top)) {
        updateVertical();
    }

    Q_EMIT topMarginChanged(topMargin);
}

void DAnchorsBase::setBottomMargin(int bottomMargin)
{
    Q_D(DAnchorsBase);

    if (d->bottomMargin == bottomMargin) {
        return;
    }

    d->bottomMargin = bottomMargin;

    if (d->fill->target()) {
        updateFill();
    } else if (isBinding(d->bottom)) {
        updateVertical();
    }

    Q_EMIT bottomMarginChanged(bottomMargin);
}

void DAnchorsBase::setLeftMargin(int leftMargin)
{
    Q_D(DAnchorsBase);

    if (d->leftMargin == leftMargin) {
        return;
    }

    d->leftMargin = leftMargin;

    if (d->fill->target()) {
        updateFill();
    } else if (isBinding(d->left)) {
        updateHorizontal();
    }

    Q_EMIT leftMarginChanged(leftMargin);
}

void DAnchorsBase::setRightMargin(int rightMargin)
{
    Q_D(DAnchorsBase);

    if (d->rightMargin == rightMargin) {
        return;
    }

    d->rightMargin = rightMargin;

    if (isBinding(d->right)) {
        updateHorizontal();
    }
    if (d->fill->target()) {
        updateFill();
    }

    Q_EMIT rightMarginChanged(rightMargin);
}

void DAnchorsBase::setHorizontalCenterOffset(int horizontalCenterOffset)
{
    Q_D(DAnchorsBase);

    if (d->horizontalCenterOffset == horizontalCenterOffset) {
        return;
    }

    d->horizontalCenterOffset = horizontalCenterOffset;

    if (isBinding(d->horizontalCenter)) {
        updateHorizontal();
    }

    Q_EMIT horizontalCenterOffsetChanged(horizontalCenterOffset);
}

void DAnchorsBase::setVerticalCenterOffset(int verticalCenterOffset)
{
    Q_D(DAnchorsBase);

    if (d->verticalCenterOffset == verticalCenterOffset) {
        return;
    }

    d->verticalCenterOffset = verticalCenterOffset;

    if (isBinding(d->verticalCenter)) {
        updateVertical();
    }

    Q_EMIT verticalCenterOffsetChanged(verticalCenterOffset);
}

void DAnchorsBase::setAlignWhenCentered(bool alignWhenCentered)
{
    Q_D(DAnchorsBase);

    if (d->alignWhenCentered == alignWhenCentered) {
        return;
    }

    d->alignWhenCentered = alignWhenCentered;
    Q_EMIT alignWhenCenteredChanged(alignWhenCentered);
}

#define SET_POS(fun)\
    DAnchorsRect rect = target()->geometry();\
    rect.set##fun(arg, point);\
    target()->setMinimumSize(QSize(0,0));\
    target()->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));\
    target()->setGeometry(rect);

#define MOVE_POS(fun)\
    DAnchorsRect rect = target()->geometry();\
    rect.move##fun(arg);\
    target()->move(rect.topLeft());

void DAnchorsBase::setTop(int arg, Qt::AnchorPoint point)
{
    SET_POS(Top)
}

void DAnchorsBase::setBottom(int arg, Qt::AnchorPoint point)
{
    SET_POS(Bottom)
}

void DAnchorsBase::setLeft(int arg, Qt::AnchorPoint point)
{
    SET_POS(Left)
}

void DAnchorsBase::setHorizontalCenter(int arg, Qt::AnchorPoint point)
{
    SET_POS(HorizontalCenter)
}

void DAnchorsBase::setVerticalCenter(int arg, Qt::AnchorPoint point)
{
    SET_POS(VerticalCenter)
}

void DAnchorsBase::setRight(int arg, Qt::AnchorPoint point)
{
    SET_POS(Right)
}

/*!
  \brief 移动 target 控件的上边界到 arg 这个位置
  \a arg 要移动到的位置
 */
void DAnchorsBase::moveTop(int arg)
{
    MOVE_POS(Top)
}

/*!
  \brief 移动 target 控件的下边界到 arg 这个位置
  \a arg 要移动到的位置
 */
void DAnchorsBase::moveBottom(int arg)
{
    MOVE_POS(Bottom)
}

/*!
  \brief 移动 target 控件的左边界到 arg 这个位置
  \a arg 要移动到的位置
 */
void DAnchorsBase::moveLeft(int arg)
{
    MOVE_POS(Left)
}

/*!
  \brief 移动 target 控件的右边界到 arg 这个位置
  \a arg 要移动到的位置
 */
void DAnchorsBase::moveRight(int arg)
{
    MOVE_POS(Right)
}

/*!
  \brief 移动 target 控件的水平中线到 arg 这个位置
  \a arg 要移动到的位置
 */
void DAnchorsBase::moveHorizontalCenter(int arg)
{
    MOVE_POS(HorizontalCenter)
}

/*!
  \brief 移动 target 控件的竖直中线到 arg 这个位置
  \a arg 要移动到的位置
 */
void DAnchorsBase::moveVerticalCenter(int arg)
{
    MOVE_POS(VerticalCenter)
}

/*!
  \brief 移动 target 控件的上边界到 arg 这个位置
  \a arg 要移动到的位置
 */
void DAnchorsBase::moveCenter(const QPoint &arg)
{
    MOVE_POS(Center)
}

#define UPDATE_GEOMETRY(p1,P1,p2,P2,p3,P3)\
    Q_D(DAnchorsBase);\
    if(isBinding(d->p1)){\
        int p1##Value = d->getTargetValueByInfo(d->p1);\
        move##P1(p1##Value);\
        if(isBinding(d->p2)){\
            qreal value = d->getTargetValueByInfo(d->p2);\
            set##P3(2 * value - p1##Value, Qt::Anchor##P1);\
        }else if(isBinding(d->p3)){\
            set##P3(d->getTargetValueByInfo(d->p3), Qt::Anchor##P1);\
        }\
    }else if(isBinding(d->p3)){\
        int p3##Value = d->getTargetValueByInfo(d->p3);\
        move##P3(p3##Value);\
        if(isBinding(d->p2)){\
            qreal value = d->getTargetValueByInfo(d->p2);\
            set##P1(2 * value - p3##Value, Qt::Anchor##P1);\
        }\
    }else if(isBinding(d->p2)){\
        move##P2(d->getTargetValueByInfo(d->p2));\
    }\

void DAnchorsBase::updateVertical()
{
    UPDATE_GEOMETRY(top, Top, verticalCenter, VerticalCenter, bottom, Bottom)
}

void DAnchorsBase::updateHorizontal()
{
    UPDATE_GEOMETRY(left, Left, horizontalCenter, HorizontalCenter, right, Right)
}

void DAnchorsBase::updateFill()
{
    Q_D(DAnchorsBase);

    QRect rect = d->getWidgetRect(d->fill->target());
    int offset = d->topMargin != 0 ? d->topMargin : d->margins;
    rect.setTop(rect.top() + offset);
    offset = d->bottomMargin != 0 ? d->bottomMargin : d->margins;
    rect.setBottom(rect.bottom() - offset);
    offset = d->leftMargin != 0 ? d->leftMargin : d->margins;
    rect.setLeft(rect.left() + offset);
    offset = d->rightMargin != 0 ? d->rightMargin : d->margins;
    rect.setRight(rect.right() - offset);

    target()->setFixedSize(rect.size());
    target()->move(rect.topLeft());
}

void DAnchorsBase::updateCenterIn()
{
    Q_D(DAnchorsBase);

    QRect rect = d->getWidgetRect(d->centerIn->target());
    moveCenter(rect.center());
}

void DAnchorsBase::init(QWidget *w)
{
    Q_D(DAnchorsBase);

    DAnchorsBase *base = DAnchorsBasePrivate::getWidgetAnchorsBase(w);

    if (base) {
        d_ptr = base->d_ptr;
    } else if (d && d->q_func() == this) {
        d->removeWidgetAnchorsBase(target(), this);
        d->setWidgetAnchorsBase(w, this);
        d->extendWidget->setTarget(w);
    } else {
        base = new DAnchorsBase(w, false);
        d_ptr = base->d_ptr;
    }
}

DAnchorsBase::DAnchorsBase(QWidget *w, bool):
    QObject(w),
    d_ptr(new DAnchorsBasePrivate(this))
{
    Q_D(DAnchorsBase);

    d->extendWidget = new DEnhancedWidget(w, this);
    connect(d->extendWidget, SIGNAL(enabledChanged(bool)), SIGNAL(enabledChanged(bool)));
    connect(d->fill, SIGNAL(sizeChanged(QSize)), SLOT(updateFill()));
    connect(d->centerIn, SIGNAL(sizeChanged(QSize)), SLOT(updateCenterIn()));
    connect(d->fill, SIGNAL(showed()), SLOT(updateFill()));
    connect(d->centerIn, SIGNAL(showed()), SLOT(updateCenterIn()));

    d->setWidgetAnchorsBase(w, this);
}

void DAnchorsRect::setTop(int arg, Qt::AnchorPoint point)
{
    if (point == Qt::AnchorVerticalCenter) {
        QRect::setBottom(bottom() + arg - top());
    }
    QRect::setTop(arg);
}

void DAnchorsRect::setVerticalCenter(qreal arg, Qt::AnchorPoint point)
{
    if (point == Qt::AnchorTop) {
        QRect::setBottom(2 * arg - top());
    } else if (point == Qt::AnchorBottom) {
        QRect::setTop(2 * arg - bottom());
    }
}

void DAnchorsRect::setBottom(int arg, Qt::AnchorPoint point)
{
    if (point == Qt::AnchorVerticalCenter) {
        setHeight(arg - bottom());
    }
    QRect::setBottom(arg);
}

void DAnchorsRect::setLeft(int arg, Qt::AnchorPoint point)
{
    if (point == Qt::AnchorHorizontalCenter) {
        setWidth(left() - arg);
    }
    QRect::setLeft(arg);
}

void DAnchorsRect::setHorizontalCenter(qreal arg, Qt::AnchorPoint point)
{
    if (point == Qt::AnchorLeft) {
        QRect::setRight(2 * arg - left());
    } else if (point == Qt::AnchorRight) {
        QRect::setLeft(2 * arg - right());
    }
}

void DAnchorsRect::setRight(int arg, Qt::AnchorPoint point)
{
    if (point == Qt::AnchorHorizontalCenter) {
        setWidth(arg - right());
    }
    QRect::setRight(arg);
}

DWIDGET_END_NAMESPACE
