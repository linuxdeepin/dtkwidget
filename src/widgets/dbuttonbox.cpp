/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
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

DButtonBoxButton::DButtonBoxButton(const QString &text, QWidget *parent)
    : QAbstractButton(parent)
    , DObject(*new DButtonBoxButtonPrivate(this))
{
    setText(text);
}

DButtonBoxButton::DButtonBoxButton(const QIcon &icon, const QString &text, QWidget *parent)
    : DButtonBoxButton(text, parent)
{
    setIcon(icon);
}

DButtonBoxButton::DButtonBoxButton(QStyle::StandardPixmap iconType, const QString &text, QWidget *parent)
    : DButtonBoxButton(static_cast<DStyle::StandardPixmap>(iconType), text, parent)
{

}

DButtonBoxButton::DButtonBoxButton(DStyle::StandardPixmap iconType, const QString &text, QWidget *parent)
    : DButtonBoxButton(text, parent)
{
    d_func()->iconType = static_cast<qint64>(iconType);
}

void DButtonBoxButton::setIcon(const QIcon &icon)
{
    D_D(DButtonBoxButton);

    d->iconType = -1;
    QAbstractButton::setIcon(icon);
}

void DButtonBoxButton::setIcon(QStyle::StandardPixmap iconType)
{
    D_D(DButtonBoxButton);

    d->iconType = iconType;
    QAbstractButton::setIcon(style()->standardIcon(iconType, nullptr, this));
}

void DButtonBoxButton::setIcon(DStyle::StandardPixmap iconType)
{
    D_D(DButtonBoxButton);

    d->iconType = iconType;
    QAbstractButton::setIcon(DStyleHelper(style()).standardIcon(iconType, nullptr, this));
}

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

DButtonBox::DButtonBox(QWidget *parent)
    : QWidget(parent)
    , DObject(*new DButtonBoxPrivate(this))
{
    d_func()->init();
}

Qt::Orientation DButtonBox::orientation() const
{
    D_DC(DButtonBox);

    QBoxLayout::Direction ld = d->layout->direction();

    if (ld == QBoxLayout::LeftToRight || ld == QBoxLayout::RightToLeft) {
        return Qt::Horizontal;
    }

    return Qt::Vertical;
}

void DButtonBox::setOrientation(Qt::Orientation orientation)
{
    D_D(DButtonBox);
    Q_ASSERT(d->group->buttons().isEmpty());

    d->layout->setDirection(orientation == Qt::Horizontal ? QBoxLayout::LeftToRight : QBoxLayout::TopToBottom);
}

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

QList<QAbstractButton *> DButtonBox::buttonList() const
{
    D_DC(DButtonBox);

    return d->group->buttons();
}

QAbstractButton *DButtonBox::checkedButton() const
{
    D_DC(DButtonBox);

    return d->group->checkedButton();
}

QAbstractButton *DButtonBox::button(int id) const
{
    D_DC(DButtonBox);

    return d->group->button(id);
}

void DButtonBox::setId(QAbstractButton *button, int id)
{
    D_D(DButtonBox);

    d->group->setId(button, id);
}

int DButtonBox::id(QAbstractButton *button) const
{
    D_DC(DButtonBox);

    return d->group->id(button);
}

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
