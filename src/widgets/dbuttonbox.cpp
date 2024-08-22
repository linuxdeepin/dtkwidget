// SPDX-FileCopyrightText: 2019 - 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dbuttonbox.h"
#include "private/dbuttonbox_p.h"
#include <private/qabstractbutton_p.h>
#include "dstyleoption.h"
#include "dstyle.h"

#include <QButtonGroup>
#include <QAbstractButton>
#include <QHBoxLayout>
#include <QStyleOptionButton>
#include <QStylePainter>
#include <QVariantAnimation>

#include <DGuiApplicationHelper>

DWIDGET_BEGIN_NAMESPACE

constexpr int HOVER_ANI_DURATION = 150;
constexpr int CHECK_ANI_DURATION = 200;
constexpr qreal HOVER_BACKGROUND_SCALE = 0.8;
constexpr int SHADOW_HEIGHT = 2;

class DButtonBoxButtonPrivate : public DCORE_NAMESPACE::DObjectPrivate
{
public:
    DButtonBoxButtonPrivate(DButtonBoxButton *qq)
        : DObjectPrivate(qq)
    {

    }

    qint64 iconType = -1;
    DDciIcon dciIcon;
};

/*!
@~english
  @class Dtk::Widget::DButtonBoxButton
  \inmodule dtkwidget
  @brief The DButtonBoxButton class provides buttons with modified styles.

  The DButtonBoxButton class provides functions to set icon buttons
  
  @code code example
  DButtonBoxButton *btn1 = new DButtonBoxButton("btn1");
  btn1->setIcon(QStyle::StandardPixmap::SP_ArrowUp);
  @endcode
  @image html DButtonBoxButton.png
 */

/*!
@~english
  @brief DButtonBoxButton constructor.

  @param[in] text is the name of button
  @param[in] parent is passed to parent widget QAbstractButton constructor
 */
DButtonBoxButton::DButtonBoxButton(const QString &text, QWidget *parent)
    : QAbstractButton(parent)
    , DObject(*new DButtonBoxButtonPrivate(this))
{
    setText(text);
}

/*!
@~english
  @brief DButtonBoxButton constructor.

  @param[in] icon
  @param[in] text is passed to DButtonBoxButton constructor
  @param[in] parent is passed to DButtonBoxButton constructor
 */
DButtonBoxButton::DButtonBoxButton(const QIcon &icon, const QString &text, QWidget *parent)
    : DButtonBoxButton(text, parent)
{
    setIcon(icon);
}

/*!
@~english
  @brief DButtonBoxButton constructor.

  @param[in] iconType is QStyle standard icon
  @param[in] text is passed to DButtonBoxButton constructor
  @param[in] parent is passed to DButtonBoxButton constructor
 */
DButtonBoxButton::DButtonBoxButton(QStyle::StandardPixmap iconType, const QString &text, QWidget *parent)
    : DButtonBoxButton(static_cast<DStyle::StandardPixmap>(iconType), text, parent)
{

}

/*!
@~english
  @brief DButtonBoxButton constructor.

  @param[in] iconType is DtkStule standard icon
  @param[in] text is passed to DButtonBoxButton constructor
  @param[in] parent is passed to DButtonBoxButton constructor
 */
DButtonBoxButton::DButtonBoxButton(DStyle::StandardPixmap iconType, const QString &text, QWidget *parent)
    : DButtonBoxButton(text, parent)
{
    d_func()->iconType = static_cast<qint64>(iconType);
}

DButtonBoxButton::DButtonBoxButton(const DDciIcon &dciIcon, const QString &text, QWidget *parent)
    : DButtonBoxButton(text, parent)
{
    setIcon(dciIcon);
}

/*!
@~english
  @brief set button icon.

  @param[in] icon
 */
void DButtonBoxButton::setIcon(const QIcon &icon)
{
    D_D(DButtonBoxButton);

    d->iconType = -1;
    QAbstractButton::setIcon(icon);
}

/*!
@~english
  @brief set button icon.
  @overload

  @param[in] iconType is QStyle standard icon
 */
void DButtonBoxButton::setIcon(QStyle::StandardPixmap iconType)
{
    D_D(DButtonBoxButton);

    d->iconType = iconType;
    QAbstractButton::setIcon(style()->standardIcon(iconType, nullptr, this));
}

/*!
@~english
  @brief set button icon.
  @overload

  @param[in] iconType is DtkStyle standard icon
 */
void DButtonBoxButton::setIcon(DStyle::StandardPixmap iconType)
{
    D_D(DButtonBoxButton);

    d->iconType = iconType;
    QAbstractButton::setIcon(DStyleHelper(style()).standardIcon(iconType, nullptr, this));
}

void DButtonBoxButton::setIcon(const DDciIcon &icon)
{
    D_D(DButtonBoxButton);

    d->dciIcon = icon;
}

DDciIcon DButtonBoxButton::dciIcon() const
{
    D_DC(DButtonBoxButton);

    return d->dciIcon;
}

/*!
@~english
  @brief return the size of icon.

  @return size of icon.
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
@~english
  @brief DButtonBoxButton::sizeHint.

  @return sizehint of icon.

  @sa QAbstractButton::sizeHint()
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

    d->sizeHint = (dstyle
                       .sizeFromContents(DStyle::CT_ButtonBoxButton, &opt,
                                         QSize(w, h), this)
                       .expandedTo(
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                           QSize{0,0}));
#else
                           QApplication::globalStrut()));
#endif
    return d->sizeHint;
}

/*!
@~english
  @brief DButtonBoxButton::minimumSizeHint.

  @return the minimum sizehint of icon.
  @sa QAbstractButton::minimumSizeHint()
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

    D_DC(DButtonBoxButton);
    if (!d->dciIcon.isNull()) {
        option->dciIcon = d->dciIcon;
        option->features |= QStyleOptionButton::ButtonFeature(DStyleOptionButton::HasDciIcon);
    }

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
    option.palette.setColor(QPalette::HighlightedText, this->palette().highlight().color());
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
    , m_hoverId(-1)
    , m_checkedId(-1)
    , m_pressId(-1)
    , m_hoverAnimation(new QVariantAnimation(qq))
    , m_checkMoveAnimation(new QVariantAnimation(qq))
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
    q->connect(m_hoverAnimation, &QVariantAnimation::valueChanged, q, [q]() {
        q->update();
    });
    q->connect(m_checkMoveAnimation, &QVariantAnimation::valueChanged, q, [q]() {
        q->update();
    });
    m_hoverAnimation->setDuration(HOVER_ANI_DURATION);
    m_checkMoveAnimation->setDuration(CHECK_ANI_DURATION);

    layout = new QHBoxLayout(q);
    layout->setContentsMargins(0, 0, 0, 0);
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
@~english
  @class Dtk::Widget::DButtonBox
  \inmodule dtkwidget
  @brief The DButtonBox class is similar to the QButtonGroup class.

  DButtonBox provides orientation for buttons
  
  @code code example
  DButtonBox box;
  DButtonBoxButton *btn1 = new DButtonBoxButton("btn1");
  DButtonBoxButton *btn2 = new DButtonBoxButton("btn2");
  btn1->setIcon(QStyle::StandardPixmap::SP_ArrowUp);
  box.setButtonList({btn1, btn2}, true);
  @endcode
  @image html DButtonBox.png

  @sa Dtk::Widget::DButtonBoxButton
 */

/*!
@~english
  @brief DButtonBox constructor.

  @param[in] parent is passed to parent widget QWidget constructor
 */
DButtonBox::DButtonBox(QWidget *parent)
    : QWidget(parent)
    , DObject(*new DButtonBoxPrivate(this))
{
    d_func()->init();
}

/*!
@~english
  @brief return the button box orientation.

  @sa Qt::Orientation
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
@~english
  @brief set the button box orientation.

  @param[in] orientation 
  @sa Qt::Orientation
 */
void DButtonBox::setOrientation(Qt::Orientation orientation)
{
    D_D(DButtonBox);
    Q_ASSERT(d->group->buttons().isEmpty());

    d->layout->setDirection(orientation == Qt::Horizontal ? QBoxLayout::LeftToRight : QBoxLayout::TopToBottom);
}

/*!
@~english
  @brief DButtonBox::setButtonList set the button into Qlists.

  @param[in] list is the Qlist that storing buttons
  @param[in] checkable is whether the button is selectable
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
        button->installEventFilter(this);
    }
}

/*!
@~english
  @brief DButtonBox::buttonList.

  @return button list.
  @sa QButtonGroup::buttons()
 */
QList<QAbstractButton *> DButtonBox::buttonList() const
{
    D_DC(DButtonBox);

    return d->group->buttons();
}

/*!
@~english
  @brief DButtonBox::checkedButton.

  @return checked button.
  @sa QButtonGroup::checkedButton()
 */
QAbstractButton *DButtonBox::checkedButton() const
{
    D_DC(DButtonBox);

    return d->group->checkedButton();
}

/*!
@~english
  @brief DButtonBox::button

  @param[in] id is the button id，same as the param set in DButtonBox::setId.
  @return return the corresponding button if it exists, return nullptr if not.

  @sa QButtonGroup::button()
 */
QAbstractButton *DButtonBox::button(int id) const
{
    D_DC(DButtonBox);

    return d->group->button(id);
}

/*!
@~english
  @brief set button id .

  @param[in] button to be set id
  @sa QButtonGroup::setId()
 */
void DButtonBox::setId(QAbstractButton *button, int id)
{
    D_D(DButtonBox);

    d->group->setId(button, id);
}

/*!
@~english
  @brief return the button id .

  @param[in] button 
  @sa QButtonGroup::id()
 */
int DButtonBox::id(QAbstractButton *button) const
{
    D_DC(DButtonBox);

    return d->group->id(button);
}
/*!
@~english
  @brief return the id of the checked button .

  @return the checked button id .
  @sa QButtonGroup
 */
int DButtonBox::checkedId() const
{
    D_DC(DButtonBox);

    return d->group->checkedId();
}

void DButtonBox::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    D_D(DButtonBox);

    QStylePainter p(this);
    QStyleOptionButton opt;
    p.setRenderHint(QPainter::Antialiasing);
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

    bool isDarkType = DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::DarkType;
    int radius = DStyle::pixelMetric(style(), DStyle::PM_FrameRadius);
    QColor background;
    if (d->m_hoverId >= 0 && d->m_hoverId < buttonList().size()) {
        if (isDarkType) {
            background = "#141414";
            background.setAlphaF(0.2);
        } else {
            background = Qt::black;
            background.setAlphaF(0.1);
        }

        p.setPen(Qt::NoPen);
        p.setBrush(background);

        auto rect = d->m_hoverAnimation->currentValue().toRect();
        p.drawRoundedRect(rect, radius, radius);

        if (isDarkType) {                                                      // 深色模式需要绘制上下阴影
            QPainterPath rectPath;
            rectPath.addRoundedRect(rect, radius, radius);

            QRect excludeRect = rect;
            excludeRect.setHeight(rect.height() - SHADOW_HEIGHT);
            QPainterPath bottomShadowPath;
            bottomShadowPath.addRoundedRect(excludeRect, radius, radius);
            bottomShadowPath = rectPath.subtracted(bottomShadowPath);

            background.setAlphaF(0.5);
            p.setBrush(background);
            p.drawPath(bottomShadowPath);                                      // 下阴影

            excludeRect.moveBottom(rect.bottom());
            QPainterPath topShadowPath;
            topShadowPath.addRoundedRect(excludeRect, radius, radius);
            topShadowPath = rectPath.subtracted(topShadowPath);

            background = Qt::white;
            background.setAlphaF(0.1);
            p.setBrush(background);
            p.drawPath(topShadowPath);                                         // 上阴影
        }
    }

    if (d->m_pressId >= 0 && d->m_pressId < buttonList().size()) {
        background = Qt::black;
        background.setAlphaF(isDarkType ? 0.15 : 0.2);

        p.setBrush(background);
        p.setPen(Qt::NoPen);
        p.drawRoundedRect(d->m_hoverAnimation->currentValue().toRect(), radius, radius);
    }

    if (d->m_checkedId >= 0 && d->m_checkedId < buttonList().size()) {
        background = Qt::black;
        background.setAlphaF(isDarkType ? 0.3 : 0.1);
        p.setBrush(background);
        p.setPen(Qt::NoPen);

        QRect rect;
        if (d->m_checkMoveAnimation->currentValue().toRect().isValid())
            rect = d->m_checkMoveAnimation->currentValue().toRect();
        else
            rect = buttonList().at(d->m_checkedId)->geometry();
        p.drawRoundedRect(rect, radius, radius);

        p.setPen(Qt::NoPen);
        QColor shadowColor = Qt::black;
        shadowColor.setAlphaF(0.2);
        p.setBrush(shadowColor);

        QPainterPath rectPath;
        rectPath.addRoundedRect(rect, radius, radius);

        QRect excludeRect = rect;
        excludeRect.setHeight(rect.height() - SHADOW_HEIGHT);
        QPainterPath shadowPath;
        shadowPath.addRoundedRect(excludeRect, radius, radius);
        shadowPath = rectPath.subtracted(shadowPath);

        if (isDarkType) {
            background.setAlphaF(0.5);
            p.setBrush(background);
        }
        p.drawPath(shadowPath);
    }
}

bool DButtonBox::eventFilter(QObject *o, QEvent *e)
{
    D_D(DButtonBox);
    for (int i = 0; i < buttonList().size(); ++i) {
        if (o == buttonList().at(i)) {
            DStyleOptionButtonBoxButton option;
            dynamic_cast<DButtonBoxButton *>(o)->initStyleOption(&option);
            if (option.state.testFlag(QStyle::State_On)) {
                if (d->m_checkedId == i)
                    return false;

                if (d->m_checkedId >= 0 && d->m_checkedId < buttonList().size()) {
                    d->m_checkMoveAnimation->setStartValue(buttonList().at(d->m_checkedId)->geometry());
                    d->m_checkedId = i;
                    d->m_checkMoveAnimation->setEndValue(buttonList().at(d->m_checkedId)->geometry());
                } else {
                    d->m_checkedId = i;
                    d->m_checkMoveAnimation->setStartValue(0);
                    d->m_checkMoveAnimation->setEndValue(0);
                }
                d->m_checkMoveAnimation->start();
                update();
            }
            if (e->type() == QEvent::HoverEnter) {
                if (d->m_hoverId == i)
                    return false;

                d->m_hoverId = i;

                if (d->m_hoverId < 0 || d->m_hoverId >= buttonList().size())
                    return false;

                QRect smallRect = buttonList().at(d->m_hoverId)->geometry();
                smallRect.setSize(QSize(smallRect.width() * HOVER_BACKGROUND_SCALE, smallRect.height() * HOVER_BACKGROUND_SCALE));
                smallRect.moveCenter(buttonList().at(d->m_hoverId)->geometry().center());
                d->m_hoverAnimation->setStartValue(smallRect);
                d->m_hoverAnimation->setEndValue(buttonList().at(d->m_hoverId)->geometry());
                d->m_hoverAnimation->start();
                update();
            } else if (e->type() == QEvent::HoverLeave) {
                d->m_hoverId = -1;
                update();
            } else if (e->type() == QEvent::MouseButtonPress) {
                if (d->m_pressId == i)
                    return false;

                d->m_pressId = i;
                d->m_hoverId = -1;
                update();
            } else if (e->type() == QEvent::MouseButtonRelease) {
                d->m_pressId = -1;
                update();
            } else if (e->type() == QEvent::Resize) {
                d->m_hoverId = -1;
                if (d->m_checkedId >= 0 && d->m_checkedId < buttonList().size()) {
                    d->m_checkMoveAnimation->setStartValue(buttonList().at(d->m_checkedId)->geometry());
                    d->m_checkMoveAnimation->setEndValue(buttonList().at(d->m_checkedId)->geometry());
                }
            }
        }
    }
    return QWidget::eventFilter(o, e);
}

DWIDGET_END_NAMESPACE
