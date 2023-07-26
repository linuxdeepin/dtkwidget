// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "danchors.h"

#include "denhancedwidget.h"

DWIDGET_BEGIN_NAMESPACE

/*!
@~english
  @class Dtk::Widget::DAnchorsBase
  \inmodule dtkwidget
  
  @brief DAnchorsBase provides a way to specify the relationship between a QWidget and other QWidgets to determine its position.
  
  In addition to the more traditional layout approach, DtkWidget also provides a way to use anchored concept layout controls (
  Similar to the anchors property in QQuickItem), each control can be thought of as having a set of six invisible anchors
  Lines ": left, horizontalCenter, right, top, verticalCenter, and bottom, as shown:
  @image html edges_anchors.png
  Using DAnchors allows QWidgets to determine relationships based on these anchor lines, such as:
  @code
  DAnchors<QLabel> rect1(new QLabel("rect1"));
  DAnchors<QLabel> rect2(new QLabel("rect2"));
  
  rect2.setLeft(rect1.right());
  @endcode
  The left edge of rect2 will then line up with the right edge of rect1:
  @image html edge1.png
  You can also set multiple anchor lines at the same time:
  @code
  DAnchors<QLabel> rect1(new QLabel("rect1"));
  DAnchors<QLabel> rect2(new QLabel("rect2"));
  
  rect2.setTop(rect1.bottom());
  rect2.setLeft(rect1.right());
  @endcode
  @image html edge3.png
  Anchor layout can be used in multiple controls at the same time, and the controls only need to meet the following conditions:
  \a Controls are siblings, or anchored controls are parent controls
  \a Anchoring relations cannot be bound cyclically
  \section1 margin_offset
  The anchor system allows you to set the spacing between "anchor lines", and each control has a set of four margins corresponding to the "anchor lines" :
  leftMargin, rightMargin, topMargin, and bottomMargin and two offsets:
  horizontalCenterOffset and verticalCenterOffset.
  @image html margins_anchors.png
  Here's an example of a left margin:
  @code
  DAnchors<QLabel> rect1(new QLabel("rect1"));
  DAnchors<QLabel> rect2(new QLabel("rect2"));
  
  rect2.setLeftMargin(5);
  rect2.setLeft(rect1.right());
  @endcode
  The left border of rect2 is 5 pixels from the right border of rect1:
  @image html edge2.png
  @note margin applies only to the anchor point you set, not to the control itself, if it is set
  margin, but no corresponding anchor is set, which has no effect on the control itself. The value of margin does
  Is a negative value, which determines the direction of the margin (inner margin or outer margin).
  
  In addition to setting anchors based on "anchor lines," there are also setCenterIn and setFill, which are special
  Implementation of.
  
  \section1 loop_anchor
  Suppose DAnchorsBase a1, a2; a1.setRight(a2.left());  I'm going to say between a1 and a2
  The logic for whether there is a loop binding is:
  Try to change the value of the right boundary of a1. If the value of the left boundary of a2 changes after the update, it is considered to cause a loop tie
  Sure, otherwise it is considered not to exist
 */

/*!
@~english
  @property DAnchorsBase::target
  @brief The control object to which the anchor function is bound
  @note Read only
  */

/*!
@~english
  @property DAnchorsBase::enabled
  @brief Controls whether anchoring is enabled. 
  A false value simply means that the position of the control will not be updated based on changes to its various properties, 
  but the anchoring relationship is not broken
  @note Readable and writable
  */

/*!
@~english
  @property DAnchorsBase::anchors
  @brief A pointer to itself
  @note Read only
  */

/*!
@~english
  @property DAnchorsBase::top
  @brief Information about bounding anchor lines on the target control
  @note Only binding to top verticalCenter bottom
  @note Assigning a value to a property does not change its own value, but sets a binding to the anchor
  @note Readable and writable
  */

/*!
@~english
  @property DAnchorsBase::bottom
  @note Only binding to top verticalCenter bottom
  @brief Information about the anchor line under the target control
  @note Assigning a value to a property does not change its own value, but sets a binding to the anchor
  @note Readable and writable
  */

/*!
@~english
  @property DAnchorsBase::left
  @note Only binding to left horizontalCenter right
  @brief Information about the anchor line on the left boundary of the target control
  @note Assigning a value to a property does not change its own value, but sets a binding to the anchor
  @note Readable and writable
  */

/*!
@~english
  @property DAnchorsBase::right
  @note Only binding to left horizontalCenter right
  @brief Information about the anchor line on the right border of the target control
  @note Assigning a value to a property does not change its own value, but sets a binding to the anchor
  @note Readable and writable
  */

/*!
@~english
  @property DAnchorsBase::horizontalCenter
  @note Only binding to left horizontalCenter right
  @brief target controls information about horizontal anchor lines
  @note Assigning a value to a property does not change its own value, but sets a binding to the anchor
  @note Readable and writable
  */

/*!
@~english
  @property DAnchorsBase::verticalCenter
  @note Only binding to top verticalCenter bottom
  @brief target controls vertical anchor line information
  @note Assigning a value to a property does not change its own value, but sets a binding to the anchor
  @note Readable and writable
  */

/*!
@~english
  @property DAnchorsBase::fill
  @brief target control fills the target object
  @note Readable and writable
  */

/*!
@~english
  @property DAnchorsBase::centerIn
  @brief target control's centered target object
  @note Readable and writable
  */

/*!
@~english
  @property DAnchorsBase::margins
  @brief The margin of the top, bottom, left, and right anchor lines. This value has lower priority than the margin value specific to each anchor line
  @note Readable and writable
  */

/*!
@~english
  @property DAnchorsBase::topMargin
  @brief The margin of the upper "anchor line" has higher priority than the margins
  @note Readable and writable
  */

/*!
@~english
  @property DAnchorsBase::bottomMargin
  @brief The margin of the lower "anchor line" has higher priority than the margins
  @note Readable and writable
  */

/*!
@~english
  @property DAnchorsBase::leftMargin
  @brief The margin of the left "anchor line" has higher priority than the margins
  @note Readable and writable
  */

/*!
@~english
  @property DAnchorsBase::rightMargin
  @brief The margin of the right "anchor line" has higher priority than the margins
  @note Readable and writable
  */

/*!
@~english
  @property DAnchorsBase::horizontalCenterOffset
  @brief The offset of the horizontal "anchor line"
  @note Readable and writable
  */

/*!
@~english
  @property DAnchorsBase::verticalCenterOffset
  @brief The offset of the vertical anchor line
  @note Readable and writable
  */

/*!
@~english
  @enum Dtk::Widget::DAnchorsBase::AnchorError
  DAnchorsBase::AnchorError Types of errors that can occur in the process of setting anchor information
  
  \value NoError
  There were no errors in setting up the anchor
  
  \value Conflict
  Indicates that the anchoring relationship is in conflict with an existing relationship, 
  for example, fill and centerIn cannot be set at the same time
  
  \value TargetInvalid
  Indicates that the target control is invalid when the anchor relationship is set
  
  \value PointInvalid
  Indicates an error in the anchor information when setting the anchor relationship, 
  such as when setting Qt::AnchorLeft to Qt::AnchorTop
  
  \value LoopBind
  The anchored relation representing the setting and the existing relation form a cyclic binding
 */

/*!
@~english
  @fn void DAnchorsBase::enabledChanged(bool enabled)
  @brief The signal is sent when the value of the \a enabled attribute changes
 */
/*!
@~english
  @fn void DAnchorsBase::topChanged(const DAnchorInfo *top)
  @brief The signal is sent when the value of the \a top attribute changes
 */
/*!
@~english
  @fn void DAnchorsBase::bottomChanged(const DAnchorInfo *bottom)
  @brief The signal is sent when the value of the \a bottom attribute changes
 */
/*!
@~english
  @fn void DAnchorsBase::leftChanged(const DAnchorInfo *left)
  @brief The signal is sent when the value of the \a left attribute changes
 */
/*!
@~english
  @fn void DAnchorsBase::rightChanged(const DAnchorInfo *right)
  @brief The signal is sent when the value of the \a right attribute changes
 */
/*!
@~english
  @fn void DAnchorsBase::horizontalCenterChanged(const DAnchorInfo *horizontalCenter)
  @brief The signal is sent when the value of the \a horizontalCenter attribute changes
 */
/*!
@~english
  @fn void DAnchorsBase::verticalCenterChanged(const DAnchorInfo *verticalCenter)
  @brief The signal is sent when the value of the \a verticalCenter attribute changes
 */
/*!
@~english
  @fn void DAnchorsBase::fillChanged(QWidget *fill)
  @brief The signal is sent when the value of the \a fill attribute changes
 */
/*!
@~english
  @fn void DAnchorsBase::centerInChanged(QWidget *centerIn)
  @brief The signal is sent when the value of the \a centerIn attribute changes
 */
/*!
@~english
  @fn void DAnchorsBase::marginsChanged(int margins)
  @brief The signal is sent when the value of the \a margins attribute changes
 */
/*!
@~english
  @fn void DAnchorsBase::topMarginChanged(int topMargin)
  @brief The signal is sent when the value of the \a topMargin attribute changes
 */
/*!
@~english
  @fn void DAnchorsBase::bottomMarginChanged(int bottomMargin)
  @brief The signal is sent when the value of the \a bottomMargin attribute changes
 */
/*!
@~english
  @fn void DAnchorsBase::leftMarginChanged(int leftMargin)
  @brief The signal is sent when the value of the \a leftMargin attribute changes
 */
/*!
@~english
  @fn DAnchorsBase::rightMarginChanged(int rightMargin)
  @brief The signal is sent when the value of the \a rightMargin attribute changes
 */
/*!
@~english
  @fn DAnchorsBase::horizontalCenterOffsetChanged(int horizontalCenterOffset)
  @brief The signal is sent when the value of the \a horizontalCenterOffset attribute changes
 */
/*!
@~english
  @fn DAnchorsBase::verticalCenterOffsetChanged(int verticalCenterOffset)
  @brief The signal is sent when the value of the \a verticalCenterOffset attribute changes
 */

/*!
@~english
  @class Dtk::Widget::DAnchors
  \inmodule dtkwidget
  @brief DAnchors is a template class that holds a control pointer on top of DAnchorsBase,
  Using a control in conjunction with an anchor is like attaching an anchor property to the control itself.
  
  The "->", "*", and "&" operators are overloaded to make the DAnchors layer transparent and minimize the difference between using DAnchors and directly using QWidget* objects.
 */

/*!
@~english
  @class Dtk::Widget::DAnchorInfo
  \inmodule dtkwidget
  @brief DAnchorInfo is used to record the anchoring information of the "anchor line" : the DAnchorsBase object being anchored, 
  the type of anchoring, the information of the target "anchor line".
  
  Each anchor line is associated with a DAnchorInfo object. In general, 
  when using anchor layouts, you only need to care about the binding of the "anchor lines" and not about the data stored in DAnchorInfo.
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
public:
    DAnchorsBasePrivate(DAnchorsBase *qq): q_ptr(qq) {}
    ~DAnchorsBasePrivate()
    {
        delete top;
        delete bottom;
        delete left;
        delete right;
        delete horizontalCenter;
        delete verticalCenter;
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
@~english
  @brief Construct the DAnchorsBase object, passing in the w object and binding it to a new DAnchorsBase object
  \a w Controls that need to use anchored relationships
  @note The anchoring relationship for w does not disappear with the destruction of the DAnchorsBase object for this construction.
  This constructor may implicitly construct a new DAnchorsBase object for actual implementation, function execution
  Will first check if there is already a DAnchorsBase object bound to the w object and create one if not
  A new DAnchorsBase object is bound to it; otherwise, an existing object is used. Implicitly created DAnchorsBase
  Object is automatically destroyed when the corresponding QWidget object is destroyed.
  @sa target() clearAnchors() getAnchorBaseByWidget()
 */
DAnchorsBase::DAnchorsBase(QWidget *w):
    QObject(w)
{
    init(w);
}

/*!
@~english
  @brief On destruction, the DAnchorsBase object is determined to be bound to the target, 
  and if so, the binding is removed from the mapping table
  @warning The DAnchorsBasePrivate object may be explicitly shared between multiple DAnchorsBase objects, 
  so the corresponding DAnchorsBasePrivate object is not necessarily destroyed after the destruction of the DAnchorsBase
  @sa QExplicitlySharedDataPointer
 */
DAnchorsBase::~DAnchorsBase()
{
    DAnchorsBasePrivate::removeWidgetAnchorsBase(target(), this);
}

QWidget *DAnchorsBase::target() const
{
    Q_D(const DAnchorsBase);

    if (!d->extendWidget)
        return 0;

    return d->extendWidget->target();
}

/*!
@~english
  @brief Returns the extension object of the target control.
  This object provides additional change signals to the QWidget object related to the size and position of the widget
  @return
  @sa Dtk::Widget::DEnhancedWidget
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
@~english
  @brief Errors generated during anchoring will clear this state before a new anchoring function is called, 
  and each time the anchoring function is called, the return value of this function can be used to determine whether the anchoring setup was successful
  @return
  @sa errorString()
 */
DAnchorsBase::AnchorError DAnchorsBase::errorCode() const
{
    Q_D(const DAnchorsBase);

    return d->errorCode;
}

/*!
@~english
  @brief A textual description of the errorCode
  @return
  @sa errorCode
 */
QString DAnchorsBase::errorString() const
{
    Q_D(const DAnchorsBase);

    return d->errorString;
}

/*!
@~english
  @brief Returns true if this info sets the anchor object, false otherwise
  @code
  DAnchors<QWidget> w1;
  DAnchors<QWidget> w2;
  
  w1.setLeft(w2.right());
  
  qDebug() << w1.isBinding(w1.left()) << w2.isBinding(w2.right());
  @endcode
  Print the content as：ture false
  \a info
  @return
 */
bool DAnchorsBase::isBinding(const DAnchorInfo *info) const
{
    return info->targetInfo;
}

/*!
@~english
  @brief A static function that allows you to set the anchoring relationship between two objects directly. 
  Calling this function may implicitly create a DAnchorsBase object
  \a w Control object to anchor
  \a p Anchor line/anchor point to be anchored
  \a target The target object to anchor
  \a point The target anchor line/anchor point for anchoring
  @return It returns true if anchoring was successful and false otherwise
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
@~english
  @brief Clears all anchoring relationships associated with control w, including any associations that anchor w or are anchored by w. Will be direct
  The DAnchorsBase object corresponding to w is destroyed
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
@~english
  @brief Returns the DAnchorsBase object bound to w
  \a w
  @return If there is no corresponding anchored object for w, then null is returned
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
@~english
  @brief Set the anchoring rules for the DAnchorsBase::target object
  @note A corresponding DAnchorsBase object may be created implicitly for the target control
  \a p Set the anchor rule for which anchor line/anchor point of the current control
  \a target The anchored target control
  \a point The target anchor line/anchor point for anchoring
  @return It returns true if the setting was successful and false otherwise
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
@~english
  @brief Call the other overloaded functions with the target() in fill as an argument
  \a fill
  @return
 */
bool DAnchorsBase::setFill(DAnchorsBase *fill)
{
    return setFill(fill->target());
}

/*!
@~english
  @brief Call other overloaded functions with target() in centerIn as an argument
  \a centerIn
  @return
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
@~english
  @brief Move the upper boundary of the target control to the arg position
  \a arg The location to move to
 */
void DAnchorsBase::moveTop(int arg)
{
    MOVE_POS(Top)
}

/*!
@~english
  @brief Move the lower boundary of the target control to arg
  \a arg The location to move to
 */
void DAnchorsBase::moveBottom(int arg)
{
    MOVE_POS(Bottom)
}

/*!
@~english
  @brief Move the left edge of the target control to arg
  \a arg The location to move to
 */
void DAnchorsBase::moveLeft(int arg)
{
    MOVE_POS(Left)
}

/*!
@~english
  @brief Move the right edge of the target control to arg
  \a arg The location to move to
 */
void DAnchorsBase::moveRight(int arg)
{
    MOVE_POS(Right)
}

/*!
@~english
  @brief Move the horizontal midline of the target control to arg
  \a arg The location to move to
 */
void DAnchorsBase::moveHorizontalCenter(int arg)
{
    MOVE_POS(HorizontalCenter)
}

/*!
@~english
  @brief Move the middle vertical line of the target control to arg
  \a arg The location to move to
 */
void DAnchorsBase::moveVerticalCenter(int arg)
{
    MOVE_POS(VerticalCenter)
}

/*!
@~english
  @brief Move the top boundary of the target control to arg
  \a arg The location to move to
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
