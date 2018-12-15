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

#include "dtextbutton.h"
#include "dthememanager.h"

/**
 * \~chinese \class DTextButton
 * \~chinese \brief 一个 DDE 风格按钮控件。
 *
 * \~chinese 继承自 QPushButton 并修改了样式使其更符合 DDE 风格, 其他功能与 QPushButton 一致, 可将其直接作为 QPushButton 使用
 */

DWIDGET_BEGIN_NAMESPACE

/*!
 * \brief 构造一个 DTextButton 实例。
 * \param text 按钮需要显示的文字
 * \param parent 用于作为按钮的父控件
 */
DTextButton::DTextButton(const QString & text, QWidget * parent) :
    QPushButton(text, parent)
{
    DThemeManager::registerWidget(this);
}

DTextButton::~DTextButton()
{

}

DWIDGET_END_NAMESPACE
