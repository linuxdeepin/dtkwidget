// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "private/dtoolbutton_p.h"

#include <DGuiApplicationHelper>
#include <DStyle>

#include <QStyleOptionButton>
#include <QStylePainter>
#include <QEvent>
#include <QDebug>
#include <QKeyEvent>

DWIDGET_BEGIN_NAMESPACE

Dtk::Widget::DToolButtonPrivate::DToolButtonPrivate(DToolButton *qq)
    : DObjectPrivate(qq)
{

}

/*!
@~english
    @class Dtk::Widget::DToolButton
    @brief Inherited from the QToolButton class, used to customize tool buttons.
 */

DToolButton::DToolButton(QWidget *parent)
    : QToolButton(parent)
    , DObject(*new DToolButtonPrivate(this))
{
    D_D(DToolButton);
    connect(this, &DToolButton::pressed, this, [d]() {
        d->m_dciPlayer.play(DDciIcon::Pressed);
    });
    connect(this, &DToolButton::released, this, [d]() {
        d->m_dciPlayer.play(DDciIcon::Normal);
    });
}

/*!
@~english
    @fn void DToolButton::paintEvent(QPaintEvent *event)
    @brief This method overrides the paintEvent method of QToolButton to draw a custom appearance for the tool button.
    @param event The paint event, which is not used.
 */

void DToolButton::paintEvent(QPaintEvent *event)
{
    D_D(DToolButton);
    Q_UNUSED(event)
    QStylePainter p(this);
    QStyleOptionToolButton opt;
    initStyleOption(&opt);

    if (!d->m_dciIcon.isNull()) {
        p.setRenderHint(QPainter::SmoothPixmapTransform);
        p.drawImage(rect(), d->m_dciPlayer.currentImage());

        if (opt.state & QStyle::State_HasFocus) {
            p.setPen(QPen(palette().highlight().color(), 2));
            p.setBrush(Qt::NoBrush);
            p.setRenderHint(QPainter::Antialiasing);
            int radius = DStyle::pixelMetric(style(), DStyle::PM_FrameRadius);
            p.drawRoundedRect(opt.rect.marginsRemoved(QMargins(1, 1, 1, 1)), radius, radius);
        }
        return;
    }

    p.drawComplexControl(QStyle::CC_ToolButton, opt);
}

/*!
@~english
    @fn void DToolButton::initStyleOption(QStyleOptionToolButton *option) const
    @brief Initializes the style options for the tool button.
    @param option A pointer to a QStyleOptionToolButton object, used to specify style options.
 */
void DToolButton::initStyleOption(QStyleOptionToolButton *option) const
{
    QToolButton::initStyleOption(option);
}

/*!
@~english
    @fn QSize DToolButton::sizeHint() const
    @brief Gets the size hint for the tool button.
    @return The size hint for the tool button, of type QSize.
 */

QSize DToolButton::sizeHint() const
{
    return QToolButton::sizeHint();
}

bool DToolButton::event(QEvent *e)
{
    D_D(DToolButton);
    if (d->m_dciIcon.isNull())
        return QToolButton::event(e);

    if (e->type() == QEvent::WindowActivate) {
        auto palette = DDciIconPalette::fromQPalette(this->palette());
        d->m_dciPlayer.setPalette(palette);
        d->m_dciPlayer.setTheme(DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::DarkType
                                ? DDciIcon::Dark : DDciIcon::Light);
        d->m_dciPlayer.setMode(DDciIcon::Normal);
    } else if (e->type() == QEvent::HoverEnter) {
        d->m_dciPlayer.play(DDciIcon::Hover);
    } else if (e->type() == QEvent::HoverLeave) {
        d->m_dciPlayer.play(DDciIcon::Normal);
    }
    return QToolButton::event(e);
}

/*!
@~english
    @fn void DToolButton::setAlignment(Qt::Alignment flag)
    @brief DToolButton:: setAlignment set the alignment of DToolButton
    @a flag alignment mode
 */
void DToolButton::setAlignment(Qt::Alignment flag)
{
    this->setProperty("_d_dtk_toolButtonAlign", QVariant(flag));
}

/*!
@~english
    @fn Qt::Alignment DToolButton::alignment() const
    @brief DToolButton:: alignment returns the current alignment of DToolButton
    @return If setAlignment is successfully set, the current alignment is returned; otherwise, Qt:: AlignLeft is returned
 */
Qt::Alignment DToolButton::alignment() const
{
    if(this->property("_d_dtk_toolButtonAlign").isValid())
        return static_cast<Qt::Alignment>(this->property("_d_dtk_toolButtonAlign").toInt());
    else
        return Qt::AlignLeft;
}

void DToolButton::setDciIcon(const DDciIcon &dciIcon)
{
    D_D(DToolButton);
    d->m_dciIcon = dciIcon;
    d->m_dciPlayer.setIcon(dciIcon);
    d->m_dciPlayer.setIconSize(120);

    connect(&d->m_dciPlayer, &DDciIconPlayer::updated, this, [this]() {
        update();
    });

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, [this, d](DGuiApplicationHelper::ColorType colorType) {
        auto palette = DDciIconPalette::fromQPalette(this->palette());
        d->m_dciPlayer.setPalette(palette);
        d->m_dciPlayer.setTheme(colorType ? DDciIcon::Dark : DDciIcon::Light);
    });
}

DWIDGET_END_NAMESPACE
