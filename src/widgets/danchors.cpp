/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     kirigaya <kirigaya@mkacg.com>
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

#include "danchors.h"

#include "denhancedwidget.h"

DWIDGET_BEGIN_NAMESPACE

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

DAnchorsBase::DAnchorsBase(QWidget *w):
    QObject(w)
{
    init(w);
}

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

DAnchorsBase::AnchorError DAnchorsBase::errorCode() const
{
    Q_D(const DAnchorsBase);

    return d->errorCode;
}

QString DAnchorsBase::errorString() const
{
    Q_D(const DAnchorsBase);

    return d->errorString;
}

bool DAnchorsBase::isBinding(const DAnchorInfo *info) const
{
    return info->targetInfo;
}

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

void DAnchorsBase::clearAnchors(const QWidget *w)
{
    DAnchorsBase *base = DAnchorsBasePrivate::getWidgetAnchorsBase(w);
    if (base) {
        delete base;
    }
}

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

bool DAnchorsBase::setFill(DAnchorsBase *fill)
{
    return setFill(fill->target());
}

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

void DAnchorsBase::moveTop(int arg)
{
    MOVE_POS(Top)
}

void DAnchorsBase::moveBottom(int arg)
{
    MOVE_POS(Bottom)
}

void DAnchorsBase::moveLeft(int arg)
{
    MOVE_POS(Left)
}

void DAnchorsBase::moveRight(int arg)
{
    MOVE_POS(Right)
}

void DAnchorsBase::moveHorizontalCenter(int arg)
{
    MOVE_POS(HorizontalCenter)
}

void DAnchorsBase::moveVerticalCenter(int arg)
{
    MOVE_POS(VerticalCenter)
}

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
