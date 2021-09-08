/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "dbuttonbox.h"
#include "private/dbuttonbox_p.h"
#include "dstyleoption.h"
#include "dstyle.h"

#include <QButtonGroup>
#include <QAbstractButton>
#include <QHBoxLayout>
#include <QStyleOptionButton>
#include <QStylePainter>
#include <private/qabstractbutton_p.h>

DWIDGET_BEGIN_NAMESPACE

class DButtonBoxButtonPrivate : public DCORE_NAMESPACE::DObjectPrivate
{
public:
    DButtonBoxButtonPrivate(DButtonBoxButton *qq)
        : DObjectPrivate(qq)
    {

    }

    qint64 iconType = -1;
};

/*!
  \class Dtk::Widget::DButtonBoxButton
  \inmodule dtkwidget
  \brief DButtonBoxButton 提供了修改了样式的按钮.

  DButtonBoxButton 提供了设置图标的按钮的函数
  
  \note 代码示例
  \note DButtonBoxButton *btn1 = new DButtonBoxButton("btn1");
  \note btn1->setIcon(QStyle::StandardPixmap::SP_ArrowUp);
  \image DButtonBoxButton.png
  \sa Dtk::Widget::DButtonBox
 */

/*!
  \brief DButtonBoxButton 构造函数.

  \a text 按钮名称
  \a parent 参数被发送到 QAbstractButton 构造函数
 */
DButtonBoxButton::DButtonBoxButton(const QString &text, QWidget *parent)
    : QAbstractButton(parent)
    , DObject(*new DButtonBoxButtonPrivate(this))
{
    setText(text);
}

/*!
  \brief DButtonBoxButton构造函数.

  \a icon 图标
  \a text 参数被发送到 DButtonBoxButton 构造函数
  \a parent 参数被发送到 DButtonBoxButton 构造函数
 */
DButtonBoxButton::DButtonBoxButton(const QIcon &icon, const QString &text, QWidget *parent)
    : DButtonBoxButton(text, parent)
{
    setIcon(icon);
}

/*!
  \brief DButtonBoxButton 构造函数.

  \a iconType Qt标准图标
  \a text 参数被发送到 DButtonBoxButton 构造函数
  \a parent 参数被发送到 DButtonBoxButton 构造函数
 */
DButtonBoxButton::DButtonBoxButton(QStyle::StandardPixmap iconType, const QString &text, QWidget *parent)
    : DButtonBoxButton(static_cast<DStyle::StandardPixmap>(iconType), text, parent)
{

}

/*!
  \brief DButtonBoxButton 构造函数.

  \a iconType 标准图标
  \a text 参数被发送到 DButtonBoxButton 构造函数
  \a parent 参数被发送到 DButtonBoxButton 构造函数
 */
DButtonBoxButton::DButtonBoxButton(DStyle::StandardPixmap iconType, const QString &text, QWidget *parent)
    : DButtonBoxButton(text, parent)
{
    d_func()->iconType = static_cast<qint64>(iconType);
}

/*!
  \brief 设置按钮图标.

  \a icon 图标
 */
void DButtonBoxButton::setIcon(const QIcon &icon)
{
    D_D(DButtonBoxButton);

    d->iconType = -1;
    QAbstractButton::setIcon(icon);
}

/*!
  \brief 设置按钮图标.
  \overload

  \a iconType Qt标准图标
 */
void DButtonBoxButton::setIcon(QStyle::StandardPixmap iconType)
{
    D_D(DButtonBoxButton);

    d->iconType = iconType;
    QAbstractButton::setIcon(style()->standardIcon(iconType, nullptr, this));
}

/*!
  \brief 设置按钮图标.
  \overload

  \a iconType 标准图标
 */
void DButtonBoxButton::setIcon(DStyle::StandardPixmap iconType)
{
    D_D(DButtonBoxButton);

    d->iconType = iconType;
    QAbstractButton::setIcon(DStyleHelper(style()).standardIcon(iconType, nullptr, this));
}

/*!
  \brief 返回图标大小.

  \return 返回图标大小.
 */
QSize DButtonBoxButton::iconSize() const
{
    QAbstractButtonPrivate *bp = static_cast<QAbstractButtonPrivate*>(QAbstractButton::d_ptr.data());

    if (bp->iconSize.isValid()) {
        return bp->iconSize;
    }

    DStyleHelper dstyle(style());
    DStyleOptionButton opt;
    opt.initFrom(this);
    int size = dstyle.pixelMetric(DStyle::PM_IconButtonIconSize, &opt, this);

    if (Q_LIKELY(size > 0)) {
        return QSize(size, size);
    }

    return QAbstractButton::iconSize();
}

/*!
  \brief DButtonBoxButton::sizeHint.

  \return 按钮的推荐大小.

  \sa QAbstractButton::sizeHint()
 */
QSize DButtonBoxButton::sizeHint() const
{
    QAbstractButtonPrivate *d = static_cast<QAbstractButtonPrivate*>(QAbstractButton::d_ptr.data());

    if (d->sizeHint.isValid())
        return d->sizeHint;

    ensurePolished();

    int w = 0, h = 0;

    DStyleOptionButtonBoxButton opt;
    initStyleOption(&opt);

    if (!icon().isNull()) {
        int ih = opt.iconSize.height();
        int iw = opt.iconSize.width() + 4;
        w += iw;
        h = qMax(h, ih);
    }

    QString s(text());
    bool empty = s.isEmpty();
    if (empty)
        s = QStringLiteral("XXXX");
    QFontMetrics fm = fontMetrics();
    QSize sz = fm.size(Qt::TextShowMnemonic, s);
    if(!empty || !w)
        w += sz.width();
    if(!empty || !h)
        h = qMax(h, sz.height());
    opt.rect.setSize(QSize(w, h)); // PM_MenuButtonIndicator depends on the height

    DStyleHelper dstyle(style());

    d->sizeHint = (dstyle.sizeFromContents(DStyle::CT_ButtonBoxButton, &opt, QSize(w, h), this).
                  expandedTo(QApplication::globalStrut()));
    return d->sizeHint;
}

/*!
  \brief DButtonBoxButton::minimumSizeHint.

  \return 按钮的最小大小策略.
  \sa QAbstractButton::minimumSizeHint()
 */
QSize DButtonBoxButton::minimumSizeHint() const
{
    return sizeHint();
}

void DButtonBoxButton::initStyleOption(DStyleOptionButtonBoxButton *option) const
{
    option->initFrom(this);
    option->init(this);
    option->features = QStyleOptionButton::Flat;

    if (isDown())
        option->state |= QStyle::State_Sunken;
    if (isChecked())
        option->state |= QStyle::State_On;

    option->text = text();
    option->icon = icon();
    option->iconSize = iconSize();

    if (DButtonBox *p = qobject_cast<DButtonBox*>(parent())) {
        option->orientation = p->orientation();
        option->position = p->d_func()->getButtonPosition(this);
    }
}

void DButtonBoxButton::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    DStylePainter p(this);
    DStyleOptionButtonBoxButton option;
    initStyleOption(&option);
    p.drawControl(DStyle::CE_ButtonBoxButton, option);
}

void DButtonBoxButton::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Enter:
    case Qt::Key_Return:
        if (hasFocus()) {
            click();
            break;
        }
        Q_FALLTHROUGH();
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Up:
    case Qt::Key_Down:
        if (DButtonBox *p = qobject_cast<DButtonBox *>(parent())) {
            if (p->focusWidget() == this) {
                int index = p->buttonList().indexOf(this);

                if (p->orientation() == Qt::Horizontal) {
                    if (event->key() == Qt::Key_Right)
                        p->buttonList().at((index + 1) >= p->buttonList().length() ? 0 : index + 1)->setFocus();
                    else if (event->key() == Qt::Key_Left)
                        p->buttonList().at((index - 1) < 0 ? p->buttonList().length() - 1 : index - 1)->setFocus();
                } else {
                    if (event->key() == Qt::Key_Down)
                        p->buttonList().at((index + 1) >= p->buttonList().length() ? 0 : index + 1)->setFocus();
                    else if ((event->key() == Qt::Key_Up))
                        p->buttonList().at((index - 1) < 0 ? p->buttonList().length() - 1 : index - 1)->setFocus();
                }
            }
        }
        break;
    default:
        QAbstractButton::keyPressEvent(event);
    }
}

bool DButtonBoxButton::event(QEvent *e)
{
    if (e->type() == QEvent::Polish) {
        D_DC(DButtonBoxButton);

        if (d->iconType >= 0) {
            if (d->iconType > static_cast<qint64>(QStyle::SP_CustomBase)) {
                DStyleHelper dstyle(style());
                setIcon(dstyle.standardIcon(static_cast<DStyle::StandardPixmap>(d->iconType), nullptr, this));
            } else {
                setIcon(style()->standardIcon(static_cast<QStyle::StandardPixmap>(d->iconType), nullptr, this));
            }
        }
    }

    return QAbstractButton::event(e);
}

DButtonBoxPrivate::DButtonBoxPrivate(DButtonBox *qq)
    : DObjectPrivate(qq)
{

}

void DButtonBoxPrivate::init()
{
    D_Q(DButtonBox);

    q->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed, QSizePolicy::PushButton));

    group = new QButtonGroup(q);
    q->connect(group, SIGNAL(buttonClicked(QAbstractButton*)), q, SIGNAL(buttonClicked(QAbstractButton*)));
    q->connect(group, SIGNAL(buttonPressed(QAbstractButton*)), q, SIGNAL(buttonPressed(QAbstractButton*)));
    q->connect(group, SIGNAL(buttonReleased(QAbstractButton*)), q, SIGNAL(buttonReleased(QAbstractButton*)));
    q->connect(group, SIGNAL(buttonToggled(QAbstractButton*, bool)), q, SIGNAL(buttonToggled(QAbstractButton*, bool)));

    layout = new QHBoxLayout(q);
    layout->setMargin(0);
    layout->setSpacing(0);
}

DStyleOptionButtonBoxButton::ButtonPosition DButtonBoxPrivate::getButtonPosition(const DButtonBoxButton *button) const
{
    D_QC(DButtonBox);

    DStyleOptionButtonBoxButton::ButtonPosition position;
    auto list = q->buttonList();
    bool reverse = q->layoutDirection() == Qt::RightToLeft && q->orientation() == Qt::Horizontal;

    if (list.count() == 1) {
        position = DStyleOptionButtonBoxButton::OnlyOne;
    } else if (button == list.first()) {
        position = reverse ? DStyleOptionButtonBoxButton::End : DStyleOptionButtonBoxButton::Beginning;
    } else if (button == list.last()) {
        position = reverse ? DStyleOptionButtonBoxButton::Beginning : DStyleOptionButtonBoxButton::End;
    } else {
        position = DStyleOptionButtonBoxButton::Middle;
    }

    return position;
}

/*!
  \class Dtk::Widget::DButtonBox
  \inmodule dtkwidget
  \brief DButtonBox 类似于 QButtonGroup 的类.

  DButtonBox 提供了对按钮的布局方向
  
  \code 示例代码
  DButtonBox box;
  DButtonBoxButton *btn1 = new DButtonBoxButton("btn1");
  DButtonBoxButton *btn2 = new DButtonBoxButton("btn2");
  btn1->setIcon(QStyle::StandardPixmap::SP_ArrowUp);
  box.setButtonList({btn1, btn2}, true);
  \endcode
  \image DButtonBox.png

  \sa Dtk::Widget::DButtonBoxButton
 */

/*!
  \brief DButtonBox::DButtonBox 构造函数.

  \a parent 参数被发送到 QWidget 构造函数
 */
DButtonBox::DButtonBox(QWidget *parent)
    : QWidget(parent)
    , DObject(*new DButtonBoxPrivate(this))
{
    d_func()->init();
}

/*!
  \brief DButtonBox::orientation 返回布局方向.

  \sa Qt::Orientation
 */
Qt::Orientation DButtonBox::orientation() const
{
    D_DC(DButtonBox);

    QBoxLayout::Direction ld = d->layout->direction();

    if (ld == QBoxLayout::LeftToRight || ld == QBoxLayout::RightToLeft) {
        return Qt::Horizontal;
    }

    return Qt::Vertical;
}

/*!
  \brief DButtonBox::setOrientation 设置布局方向.

  \a orientation \sa Qt::Orientation
 */
void DButtonBox::setOrientation(Qt::Orientation orientation)
{
    D_D(DButtonBox);
    Q_ASSERT(d->group->buttons().isEmpty());

    d->layout->setDirection(orientation == Qt::Horizontal ? QBoxLayout::LeftToRight : QBoxLayout::TopToBottom);
}

/*!
  \brief DButtonBox::setButtonList将按钮进添加到组中.

  \a list 存储按钮的 QList 列表
  \a checkable 按钮是否可选中
 */
void DButtonBox::setButtonList(const QList<DButtonBoxButton *> &list, bool checkable)
{
    D_D(DButtonBox);

    for (QAbstractButton *button : d->group->buttons()) {
        d->group->removeButton(button);
        d->layout->removeWidget(button);
    }

    for (int i = 0; i < list.count(); ++i) {
        QAbstractButton *button = list.at(i);

        button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        d->layout->addWidget(button);
        d->group->addButton(button);

        button->setCheckable(checkable);
    }
}

/*!
  \brief DButtonBox::buttonList.

  \return 返回按钮列表.
  \sa QButtonGroup::buttons()
 */
QList<QAbstractButton *> DButtonBox::buttonList() const
{
    D_DC(DButtonBox);

    return d->group->buttons();
}

/*!
  \brief DButtonBox::checkedButton.

  \return 返回 checked 状态的按钮.
  \sa QButtonGroup::checkedButton()
 */
QAbstractButton *DButtonBox::checkedButton() const
{
    D_DC(DButtonBox);

    return d->group->checkedButton();
}

/*!
  \brief DButtonBox::button

  \a id 按钮的ID，与 DButtonBox::setId 设置的参数一致.
  \return 返回对应\a id 的按钮，不存在返回 nullptr .

  \sa QButtonGroup::button()
 */
QAbstractButton *DButtonBox::button(int id) const
{
    D_DC(DButtonBox);

    return d->group->button(id);
}

/*!
  \brief 设置按钮 id .

  \a button 用于设置 \a id 的按钮。
  \sa QButtonGroup::setId()
 */
void DButtonBox::setId(QAbstractButton *button, int id)
{
    D_D(DButtonBox);

    d->group->setId(button, id);
}

/*!
  \brief 返回按钮 id .

  \a button 对应的按钮.
  \sa QButtonGroup::id()
 */
int DButtonBox::id(QAbstractButton *button) const
{
    D_DC(DButtonBox);

    return d->group->id(button);
}
/*!
  \brief 返回已选中按钮 id .

  \return 返回已选中按钮 id .
  \sa QButtonGroup
 */
int DButtonBox::checkedId() const
{
    D_DC(DButtonBox);

    return d->group->checkedId();
}

void DButtonBox::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    QStylePainter p(this);
    QStyleOptionButton opt;
    opt.state = QStyle::State_None;
    opt.rect = rect();
    opt.direction = layoutDirection();
    opt.palette = palette();
    opt.styleObject = this;
    opt.fontMetrics = fontMetrics();

    if (isEnabled()) {
        opt.state |= QStyle::State_Enabled;
    } else if (isActiveWindow()) {
        opt.state |= QStyle::State_Active;
    }

    p.drawControl(QStyle::CE_PushButtonBevel, opt);
}

DWIDGET_END_NAMESPACE
