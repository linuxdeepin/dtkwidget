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

#include "dbasebutton.h"
#include "dthememanager.h"

DWIDGET_BEGIN_NAMESPACE

/*!
 * \~english \class DBaseButton
 * \~english \brief The DBaseButton class provides deepin style QPushButton.
 *
 * \~english DBaseButton will be deprecated and removed in the future, please use
 * \~english QPushButton, together with dstyle Qt style plugin, QPushButton should have
 * \~english the same visual effect as DBaseButton.
 */

/*!
 * \~chinese \class DBaseButton
 * \~chinese \brief DBaseButton 提供 deepin 风格的 QPushButton。
 *
 * \~chinese DBaseButton 将来会被弃用并删除，请使用 QPushButton
 */

/*!
 * \~english \brief DBaseButton::DBaseButton constructs an instance of DBaseButton.
 * \~english \param parent is passed to QPushButton constructor.
 */

/*!
 * \~chinese \brief DBaseButton 的构造函数
 * \~chinese \param parent
 */
DBaseButton::DBaseButton(QWidget *parent) :
    QPushButton(parent)
{
    DThemeManager::registerWidget(this);

    initInsideFrame();
}

/*!
 * \~english \brief DBaseButton::DBaseButton constructs an instance of DBaseButton.
 * \~english \param text is passed to QPushButton constructor.
 * \~english \param parent is passed to QPushButton constructor.
 */

/*!
 * \~chinese \brief DBaseButton 的重载构造函数
 * \~chinese \param text 用于初始化文本内容
 * \~chinese \param
 */
DBaseButton::DBaseButton(const QString &text, QWidget *parent) :
    QPushButton(text, parent)
{
    DThemeManager::registerWidget(this);

    initInsideFrame();
}

/*!
 * \~english \brief DBaseButton::DBaseButton constructs an instance of DBaseButton.
 * \~english \param icon is passed to QPushButton constructor.
 * \~english \param text is passed to QPushButton constructor.
 * \~english \param parent is passed to QPushButton constructor.
 */

/*!
 * \~chinese \brief DBaseButton 的重载构造函数
 * \~chinese \param icon 初始化图标
 * \~chinese \param text 初始化文本内容
 * \~chinese \param parent
 */
DBaseButton::DBaseButton(const QIcon &icon, const QString &text, QWidget *parent) :
    QPushButton(icon, text, parent)
{
    DThemeManager::registerWidget(this);

    initInsideFrame();
}

//Bypassing the problem here
//qss can't draw box-shadow
void DBaseButton::initInsideFrame()
{
    QFrame *insideFrame = new QFrame;
    insideFrame->raise();
    insideFrame->setAttribute(Qt::WA_TransparentForMouseEvents);
    insideFrame->setObjectName("ButtonInsideFrame");
    QHBoxLayout *insideLayout = new QHBoxLayout(this);
    insideLayout->setContentsMargins(0, 1, 0, 0);
    insideLayout->addWidget(insideFrame);
}

DWIDGET_END_NAMESPACE
