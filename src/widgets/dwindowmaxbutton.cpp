/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#include "dwindowmaxbutton.h"
#include "private/diconbutton_p.h"
#include "dstyleoption.h"

DWIDGET_BEGIN_NAMESPACE

class DWindowMaxButtonPrivate : public DIconButtonPrivate {
public:
    DWindowMaxButtonPrivate(DWindowMaxButton* qq)
        : DIconButtonPrivate(qq)
    {
    }

    void setMaximized(bool isMaximized)
    {
        if (isMaximized == m_isMaximized)
            return;

        m_isMaximized = isMaximized;

        updateIcon();

        Q_EMIT q_func()->maximizedChanged(isMaximized);
    }

    void updateIcon()
    {
        auto drawFun = !m_isMaximized ? DDrawUtils::drawTitleBarMaxButton : DDrawUtils::drawTitleBarNormalButton;
        QString iconName = !m_isMaximized ? QStringLiteral("TitleBarMaxButton") : QStringLiteral("TitleBarNormalButton");

        q_func()->setIcon(QIcon (new DStyledIconEngine(drawFun, iconName)));
    }

private:
    bool m_isMaximized = false;
    Q_DECLARE_PUBLIC(DWindowMaxButton)
};

/*!
  \class Dtk::Widget::DWindowMaxButton
  \inmodule dtkwidget
  \brief The DWindowMaxButton class is used as the unified window maximize button.
  \brief DWindowMaxButton 类是 DTK 窗口统一的最大化/恢复按钮控件.
  
  It's actually a special DImageButton which has the appearance of maximize button.
  它有两种状态，一种表示窗口已经进入最大化状态，点击按钮窗口恢复为普通状态；
  另一种表示窗口处于普通状态，点击按钮窗口进入最大化状态。
  
  \sa DWindowMaxButton::isMaximized()
 */

/*!
  \brief DWindowMaxButton::DWindowMaxButton 是 DWindowMaxButton 的构造
  函数，返回 DWindowMaxButton 对象，普通程序一般无需使用。
  \a parent 为创建对象的父控件。
 */
DWindowMaxButton::DWindowMaxButton(QWidget * parent) :
    DIconButton(*new DWindowMaxButtonPrivate(this), parent)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    d_func()->updateIcon();
    setFlat(true);
}

/*!
  \fn void DWindowMaxButton::maximizedChanged(bool isMaximized)
  \brief isMaximizedChanged 信号在 isMaximized 属性发生变化时触发.

  \a isMaximized 是否时最大化状态.
 */

/*!
  \property DWindowMaxButton::isMaximized
  \brief isMaximized indicates whether the button state is maximized or not.
  
  It's true if it's set to maximized, otherwise false.
  
  \property DWindowMaxButton::isMaximized
  \brief isMaximized 属性标示了这个按钮当前所处于的状态。
  值为 true 表示按钮处于最大化状态，false 表示窗口处于正常状态。
 */
bool DWindowMaxButton::isMaximized() const
{
    D_DC(DWindowMaxButton);

    return d->m_isMaximized;
}

QSize DWindowMaxButton::sizeHint() const
{
    return iconSize();
}

void DWindowMaxButton::setMaximized(bool isMaximized)
{
    D_D(DWindowMaxButton);

    d->setMaximized(isMaximized);
}

void DWindowMaxButton::initStyleOption(DStyleOptionButton *option) const
{
    DIconButton::initStyleOption(option);

    option->features |= QStyleOptionButton::ButtonFeature(DStyleOptionButton::TitleBarButton);
}

DWIDGET_END_NAMESPACE


