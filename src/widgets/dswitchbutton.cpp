// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dswitchbutton.h"
#include "private/dswitchbutton_p.h"

#include <DStyleOptionButton>
#include <DStyle>
#include <DDciIcon>
#include <DGuiApplicationHelper>

#include <QApplication>

DWIDGET_BEGIN_NAMESPACE

constexpr int DCI_ICON_SIZE = 120;

/*!
@~english
  @brief DSwitchButton::DSwitchButton implements a switch button
  \a parent
 */
DSwitchButton::DSwitchButton(QWidget *parent)
    : QAbstractButton(parent)
    , DObject(*new DSwitchButtonPrivate(this))
{
    D_D(DSwitchButton);

    d->init();
}

/*!
@~english
  @brief DSwitchButton::sizeHint initializes the control rectangle size (before drawing)
  @return Control rectangle size
 */
QSize DSwitchButton::sizeHint() const
{
    QSize size(0, 0);
    DStyleHelper dstyle(style());
    size = dstyle.sizeFromContents(DStyle::CT_SwitchButton, nullptr, QSize(0, 0), this);

    return size;
}

/*!
@~english
  @brief DSwitchButton::paintEvent Painting treatment
  \a e Painting event
  @sa QWidget::paintEvent()
 */
void DSwitchButton::paintEvent(QPaintEvent *e)
{
    D_D(DSwitchButton);
    Q_UNUSED(e);

    DStylePainter painter(this);
    DStyleOptionButton opt;
    initStyleOption(&opt);
    painter.drawControl(DStyle::CE_SwitchButton, opt);

    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.drawImage(rect().adjusted(4, -8, -4, 8), d->player.currentImage());          // 为了显示按钮的阴影所留的空白
}

/*!
@~english
  @brief DSwitchButton::initStyleOption Initializes the abstract button object (for inheritance), which is later used for DStylePainter painting DStyle::CE_SwitchButton enumeration
  \a option The abstract style button object is initialized
 */
void DSwitchButton::initStyleOption(DStyleOptionButton *option) const
{
    if (!option)
        return;

    option->init(this);
    option->initFrom(this);

    if (isChecked()) {
        option->state |= QStyle::State_On;
    } else {
        option->state |= QStyle::State_Off;
    }

    // 针对switch button禁用hover状态
    option->state &= ~QStyle::State_MouseOver;

    if (hasFocus()) {
        option->state |= QStyle::State_HasFocus;
    }

    //按照设计要求: 这里需要预留绘制focusRect的区域
    option->rect.adjust(4, 4, -4, -4);
}

DSwitchButtonPrivate::DSwitchButtonPrivate(DSwitchButton *qq)
    : DObjectPrivate(qq)
{

}

DSwitchButtonPrivate::~DSwitchButtonPrivate()
{

}

void DSwitchButtonPrivate::init()
{
    checked = false;
    animationStartValue = 0;
    animationEndValue = 1;

    D_Q(DSwitchButton);

    q->setObjectName("DSwitchButton");
    q->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    q->setCheckable(true);
    
    auto initPlayer= [this, q]() {
        DDciIcon icon = !checked ? DDciIcon::fromTheme("switch_on") : DDciIcon::fromTheme("switch_off");
        player.setIcon(icon);
        player.setMode(DDciIcon::Mode::Normal);
        auto palette = DDciIconPalette::fromQPalette(q->palette());
        player.setPalette(palette);
        player.setDevicePixelRatio(qApp->devicePixelRatio());
        player.setIconSize(DCI_ICON_SIZE);
        player.setTheme(DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::DarkType
                        ? DDciIcon::Dark : DDciIcon::Light);
    };

    initPlayer();

    q->connect(q, &DSwitchButton::toggled, q, [q, this](bool ckd) {
        if (checked == ckd)
            return;

        checked = ckd;
        DDciIcon icon = checked ? DDciIcon::fromTheme("switch_on") : DDciIcon::fromTheme("switch_off");
        player.setIcon(icon);
        player.play(DDciIcon::Mode::Normal);

        Q_EMIT q->checkedChanged(checked);
    });

    q->connect(&player, &DDciIconPlayer::updated, q, [q]() {
        q->update();
    });

    q->connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, q, initPlayer);
}

DWIDGET_END_NAMESPACE
