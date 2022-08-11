// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QDebug>
#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>
#include <QApplication>

#include "dsegmentedcontrol.h"
#include "dthememanager.h"
#include "dobject_p.h"
#include "dgraphicsclipeffect.h"

DCORE_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

class DSegmentedControlPrivate : public DObjectPrivate
{
public:
    DSegmentedControlPrivate(DSegmentedControl *qq)
        : DObjectPrivate(qq)
        , highlight(new DSegmentedHighlight(qq))
        , clipEffect(new DGraphicsClipEffect(qq))
        , hLayout(new QHBoxLayout(qq))
        , highlightMoveAnimation(new QPropertyAnimation(highlight, "pos", qq))
        , currentIndex(-1)
    {
        qq->setGraphicsEffect(clipEffect);
    }

    DSegmentedHighlight *highlight;
    // 此处不适用DClipEffectWidget是因为无法解决Qt浮点数高分屏缩放产生的细线问题
//    DClipEffectWidget *clipEffect;
    DGraphicsClipEffect *clipEffect;
    QHBoxLayout *hLayout;
    QPropertyAnimation *highlightMoveAnimation;
    int currentIndex;
    QList<QToolButton*> tabList;

    D_DECLARE_PUBLIC(DSegmentedControl)
};

/*!
  \class Dtk::Widget::DSegmentedHighlight
  \inmodule dtkwidget
  \obsolete
  \brief DSegmentedHighlight 类代表了 DSegmentedControl 中当前选中（即高亮）的部分.

  请使用 Dtk::Widget::DButtonBox 代替。
 */

/*!
  \brief DSegmentedHighlight::DSegmentedHighlight 为 DSegmentedHighlight 类的构造函数.

  \a parent 制定了控件的父控件。
  \note 一般使用者无需使用自己构造 DSegmentedHighlight 对象。
 */
DSegmentedHighlight::DSegmentedHighlight(QWidget *parent) :
    QToolButton(parent)
{
}

/*!
  \class Dtk::Widget::DSegmentedControl
  \inmodule dtkwidget
  \brief DSegmentedControl 类提供了一种类似标签页的控件.

  它由一系列的 Segment 构成，每一个 Segment 可以被设置标题和图标，也可以被选中，
  它们之间的选中为互斥关系。
  DSegmentedControl 更像是脱离于 QTabWidget 工作的 QTabBar 。
  
  它可以跟 QStackedLayout 搭配使用，达到切换标签页的功能，例如：
  \image html segmentedcontrol.png
 */

/*!
  \fn void DSegmentedControl::currentChanged(int index)
  \brief 信号在当前选中（高亮）的 Segment 发生改变时触发.

  \a index 当前标签的索引.
 */

/*!
  \fn void DSegmentedControl::currentTitleChanged(QString title)
  \brief 信号在当前选中（高亮）的 Segment 发生改变时触发.

  \a title 改变的标签标题.
 */

/*!
  \fn void DSegmentedControl::animationDurationChanged(int animationDuration)
  \brief 信号在高亮动画的动画周期发生改变时触发.

  \a animationDuration 改变的动画周期.
 */

/*!
  \brief DSegmentedControl::DSegmentedControl 为 DSegmentedControl 类的构造函数。
  \a parent 制定了控件的父控件。
 */
DSegmentedControl::DSegmentedControl(QWidget *parent)
    : QWidget(parent)
    , DObject(*new DSegmentedControlPrivate(this))
{
    setObjectName("DSegmentedControl");
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    D_D(DSegmentedControl);

    d->hLayout->setSpacing(1);
    d->hLayout->setMargin(0);
    d->hLayout->setObjectName("TabBar");
    d->highlight->setObjectName("Highlight");
    d->highlight->installEventFilter(this);

    d->highlightMoveAnimation->setDuration(100);
    d->highlightMoveAnimation->setEasingCurve(QEasingCurve::InCubic);
}

/*!
  \property DSegmentedControl::count
  \brief count 属性代表了控件中 Segment 的数量。
 */
int DSegmentedControl::count() const
{
    D_DC(DSegmentedControl);

    return d->tabList.count();
}

/*!
  \brief DSegmentedControl::highlight 用于获取当前选中（高亮）的 Segment 。
  \return 控件中当前选中（高亮）的 Segmnet 。
 */
const DSegmentedHighlight *DSegmentedControl::highlight() const
{
    D_DC(DSegmentedControl);

    return d->highlight;
}

/*!
  \property DSegmentedControl::currentIndex
  \brief currentIndex 属性指示当前选中 Segment 在所有 Segment 中的位置。
 */
int DSegmentedControl::currentIndex() const
{
    D_DC(DSegmentedControl);

    return d->currentIndex;
}

/*!
  \brief DSegmentedControl::at 用于获取在特定位置上的 Segment。
  Segment 目前由 QToolButton 充当。
  \a index 为所要获取的 Segment 的索引。
  \return 指定索引位置的 Segment。
 */
QToolButton *DSegmentedControl::at(int index) const
{
    D_DC(DSegmentedControl);

    return d->tabList[index];
}

/*!
  \brief DSegmentedControl::getText 用于获取在特定位置上 Segment 的标题。
  \a index 为所要获取的 Segment 的索引。
  \return 指定索引位置上 Segment 的标题。
 */
QString DSegmentedControl::getText(int index) const
{
    const QToolButton *button = at(index);

    if(button){
        return button->text();
    }

    return "";
}

/*!
  \brief DSegmentedControl::getIcon 用于获取在特定位置上 Segment 的图标。
  \a index 为所要获取的 Segment 的索引。
  \return 指定索引位置上 Segment 的图标。
 */
QIcon DSegmentedControl::getIcon(int index) const
{
    return at(index)->icon();
}

/*!
  \property DSegmentedControl::animationDuration
  \brief animationDuration 属性用于控制控件中高亮部分移动动画的时间周期。
 */
int DSegmentedControl::animationDuration() const
{
    D_DC(DSegmentedControl);

    return d->highlightMoveAnimation->duration();
}

/*!
  \brief DSegmentedControl::indexByTitle 用于通过 Segment 的标题内容获取
  Segment 的索引。
  \a title 指定 Segment 的标题内容。
  \return 指定 Segment 的索引。
 */
int DSegmentedControl::indexByTitle(const QString &title) const
{
    D_DC(DSegmentedControl);

    int i=0;
    Q_FOREACH (QToolButton *button, d->tabList) {
        if(button->text() == title)
            return i;
        ++i;
    }

    return -1;
}

/*!
  \property DSegmentedControl::animationType
  \brief animationType 属性用于控制控件中高亮部分移动动画的动画曲线类型。
 */
QEasingCurve::Type DSegmentedControl::animationType() const
{
    D_DC(DSegmentedControl);

    return d->highlightMoveAnimation->easingCurve().type();
}

/*!
  \brief DSegmentedControl::addSegmented 为控件添加 Segment，并指定标题。
  \a title 为 Segment 的标题。
  \return 新增加的 Segment 的索引。
  
  
  \sa insertSegmented, removeSegmented
 */
int DSegmentedControl::addSegmented(const QString &title)
{
    D_D(DSegmentedControl);

    insertSegmented(d->hLayout->count(), title);

    return d->hLayout->count()-1;
}

/*!
  \brief DSegmentedControl::addSegmented 为控件添加 Segment，并指定标题和图标。
  \a icon 为 Segment 的图标。
  \a title 为 Segment 的标题。
  \return 新增加的 Segment 的索引。
  
  \sa insertSegmented, removeSegmented
 */
int DSegmentedControl::addSegmented(const QIcon &icon, const QString &title)
{
    D_D(DSegmentedControl);

    insertSegmented(d->hLayout->count(), icon, title);

    return d->hLayout->count()-1;
}

/*!
  \brief DSegmentedControl::addSegmented 添加多个 Segment。
  \a titleList 多个 Segment 的标题列表。
  
  
  \sa insertSegmented, removeSegmented
 */
void DSegmentedControl::addSegmented(const QStringList &titleList)
{
    Q_FOREACH (const QString &title, titleList) {
        addSegmented(title);
    }
}

/*!
  \brief DSegmentedControl::addSegmented 添加多个 Segment。
  \a titleList 为多个 Segment 的标题列表。
  \a iconList 为多个 Segment 的图标列表。
  
  
  \sa insertSegmented, removeSegmented
 */
void DSegmentedControl::addSegmented(const QList<QIcon> &iconList, const QStringList &titleList)
{
    for(int i=0;i<titleList.count();++i){
        addSegmented(iconList[i], titleList[i]);
    }
}

/*!
  \brief DSegmentedControl::insertSegmented 用于在特定位置插入一个新的 Segment。
  \a index 指定要插入 Segment 的位置。
  \a title 指定要插入 Segment 的标题。
  
  
  \sa addSegmented, removeSegmented
 */
void DSegmentedControl::insertSegmented(int index, const QString &title)
{
    insertSegmented(index, QIcon(), title);
}

/*!
  \brief DSegmentedControl::insertSegmented 用于在特定位置插入一个新的 Segment。
  \a index 指定要插入 Segment 的位置。
  \a title 指定要插入 Segment 的标题。
  
  
  \sa addSegmented, removeSegmented
 */
void DSegmentedControl::insertSegmented(int index, const QIcon &icon, const QString &title)
{
    D_D(DSegmentedControl);

    QToolButton *button = new QToolButton();

    d->tabList.insert(index, button);

    button->setObjectName("Segmented");
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    button->setText(title);
    button->setIcon(icon);

    connect(button, &QToolButton::clicked, this, &DSegmentedControl::buttonClicked);
    d->hLayout->insertWidget(index, button);

    if(d->currentIndex == -1){
        setCurrentIndex(0);
    }

    button->installEventFilter(this);
}

/*!
  \brief DSegmentedControl::removeSegmented 用于移除特定位置上的 Segment。
  \a index 指定了要移除的 Segment 的索引。
 */
void DSegmentedControl::removeSegmented(int index)
{
    D_D(DSegmentedControl);

    if(index == d->currentIndex)
        setCurrentIndex(-1);

    delete d->hLayout->takeAt(index);

    QToolButton *button = at(index);
    d->tabList.removeAt(index);
    if(button)
        button->deleteLater();
}

/*!
  \brief DSegmentedControl::clear 清除所有 Segment 。
 */
void DSegmentedControl::clear()
{
    D_D(DSegmentedControl);

    for(int i=0; i<count(); ++i){
        delete d->hLayout->takeAt(i);

        QToolButton *button = at(i);
        if(button)
            button->deleteLater();
    }

    d->tabList.clear();
}

bool DSegmentedControl::setCurrentIndex(int currentIndex)
{
    D_D(DSegmentedControl);

    if(currentIndex == d->currentIndex)
        return true;

    if(currentIndex<0||currentIndex>count()-1){
        qErrnoWarning("index range over!");
        return false;
    }

    d->currentIndex = currentIndex;

    Q_FOREACH (QToolButton *button, d->tabList) {
        button->setEnabled(true);
    }

    at(currentIndex)->setFocus();
    at(currentIndex)->setEnabled(false);

    updateHighlightGeometry();

    Q_EMIT currentChanged(currentIndex);
    Q_EMIT currentTitleChanged(at(currentIndex)->text());

    return true;
}

/*!
  \brief DSegmentedControl::setCurrentIndexByTitle 设置标题为指定标题的 Segment 为当前选中（高亮）的 Segment。
  \a title 为指定的标题。
  \return 是否设置成功， true 为设置成功，否则为 false 。
 */
bool DSegmentedControl::setCurrentIndexByTitle(const QString &title)
{
    return setCurrentIndex(indexByTitle(title));
}

/*!
  \brief DSegmentedControl::setText 用于给特定位置的 Segment 设置标题。
  \a index 为指定 Segment 的索引。
  \a title 为要设定的标题内容。
 */
void DSegmentedControl::setText(int index, const QString &title)
{
    at(index)->setText(title);
}

/*!
  \brief DSegmentedControl::setIcon 用于给特定位置的 Segment 设置图标。
  \a index 为指定 Segment 的索引。
  \a icon 为要设定的图标。
 */
void DSegmentedControl::setIcon(int index, const QIcon &icon)
{
    at(index)->setIcon(icon);
}

void DSegmentedControl::setAnimationDuration(int animationDuration)
{
    D_D(DSegmentedControl);

    d->highlightMoveAnimation->setDuration(animationDuration);
}

void DSegmentedControl::setAnimationType(QEasingCurve::Type animationType)
{
    D_D(DSegmentedControl);

    d->highlightMoveAnimation->setEasingCurve(animationType);
}

bool DSegmentedControl::eventFilter(QObject *obj, QEvent *e)
{
    D_D(DSegmentedControl);

    if(d->currentIndex < 0)
        return false;

    QWidget *w = at(d->currentIndex);

    if (obj == w) {
        if(e->type() == QEvent::Resize){
            updateHighlightGeometry(false);
        }
    }

    return false;
}

void DSegmentedControl::resizeEvent(QResizeEvent *event)
{
    QPainterPath clip_path;

    clip_path.addRoundedRect(QRect(QPoint(0, 0), event->size()), 4, 4);

    D_D(DSegmentedControl);

    d->clipEffect->setClipPath(clip_path);

    return QWidget::resizeEvent(event);
}

void DSegmentedControl::updateHighlightGeometry(bool animation)
{
    D_D(DSegmentedControl);

    if (d->currentIndex < 0)
        return;

    QRect tmp = at(d->currentIndex)->geometry();

    d->highlight->resize(tmp.size());

    if (d->currentIndex == 0) {
        tmp.setX(0);
    }

    tmp.setY(0);

    if (d->highlight->pos() == tmp.topLeft())
        return;

    if (animation) {
        d->highlightMoveAnimation->setStartValue(d->highlight->pos());
        d->highlightMoveAnimation->setEndValue(tmp.topLeft());
        d->highlightMoveAnimation->start();
    } else {
        d->highlight->move(tmp.topLeft());
    }
}

void DSegmentedControl::buttonClicked()
{
    D_D(DSegmentedControl);

    int i = d->tabList.indexOf(qobject_cast<QToolButton*>(sender()));

    if(i>=0){
        setCurrentIndex(i);
    }
}

DWIDGET_END_NAMESPACE
