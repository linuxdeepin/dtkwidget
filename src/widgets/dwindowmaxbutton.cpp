/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#include <DObjectPrivate>

#include "dthememanager.h"
#include "dwindowmaxbutton.h"
#include "private/dimagebutton_p.h"
#include <QStyle>

DWIDGET_BEGIN_NAMESPACE

class DWindowMaxButtonPrivate : public DImageButtonPrivate{
public:
    DWindowMaxButtonPrivate(DWindowMaxButton* qq):DImageButtonPrivate(qq) {
        m_isMaximized = false;
    }

private:
    bool m_isMaximized;
    Q_DECLARE_PUBLIC(DWindowMaxButton)
};

/*!
 * \~english \class DWindowMaxButton
 * \~english \brief The DWindowMaxButton class is used as the unified window maximize button.
 *
 * \~english It's actually a special DImageButton which has the appearance of maximize button.
 *
 * \~chinese \class DWindowMaxButton
 * \~chinese \brief DWindowMaxButton 类是 DTK 窗口统一的最大化/恢复按钮控件。
 * \~chinese 它有两种状态，一种表示窗口已经进入最大化状态，点击按钮窗口恢复为普通状态；
 * \~chinese 另一种表示窗口处于普通状态，点击按钮窗口进入最大化状态。
 *
 * \sa isMaximized
 */

/*!
 * \~chinese \brief DWindowMaxButton::DWindowMaxButton 是 DWindowMaxButton 的构造
 * \~chinese 函数，返回 DWindowMaxButton 对象，普通程序一般无需使用。
 * \~chinese \param parent 为创建对象的父控件。
 */
DWindowMaxButton::DWindowMaxButton(QWidget * parent) :
    DImageButton(*new DWindowMaxButtonPrivate(this), parent)
{
    DThemeManager::registerWidget(this, QStringList({"isMaximized"}));

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

/*!
 * \~chinese \fn DWindowMaxButton::maximizedChanged
 * \~chinese \brief isMaximizedChanged 信号在 isMaximized 属性发生变化时触发。
 */

/*!
 * \~english \property DWindowMaxButton::isMaximized
 * \~english \brief isMaximized indicates whether the button state is maximized or not.
 *
 * \~english It's true if it's set to maximized, otherwise false.
 *
 * \~chinese \property DWindowMaxButton::isMaximized
 * \~chinese \brief isMaximized 属性标示了这个按钮当前所处于的状态。
 * \~chinese 值为 true 表示按钮处于最大化状态，false 表示窗口处于正常状态。
 */
bool DWindowMaxButton::isMaximized() const
{
    D_DC(DWindowMaxButton);

    return d->m_isMaximized;
}

void DWindowMaxButton::setMaximized(bool isMaximized)
{
    D_D(DWindowMaxButton);

    if (d->m_isMaximized == isMaximized)
        return;

    d->m_isMaximized = isMaximized;
    Q_EMIT maximizedChanged(isMaximized);
}

DWIDGET_END_NAMESPACE


