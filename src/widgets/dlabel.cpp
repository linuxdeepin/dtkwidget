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

#include "dlabel.h"

DWIDGET_BEGIN_NAMESPACE

/*!
 * \~english \class DLabel
 * \~english \brief The DLabel class is a direct subclass of QLabel, styled by deepin to
 * provide better style unification.
 */

/*!
 * \~chinese \class DLabel
 * \~chinese \brief DLabel 是 QLabel 的子类，为 deepin 提供更好的的风格统一。
 */

/*!
 * \~english \brief DLabel::DLabel constructs an instance of DLabel.
 * \~english \param parent is passed to QLabel constructor.
 * \~english \param f is passed to QLabel constructor.
 */

/*!
 * \~chinese \brief DLabel 的构造函数.
 * \~chinese \param parent
 * \~chinese \param f
 */
DLabel::DLabel(QWidget *parent, Qt::WindowFlags f)
    : QLabel(parent, f)
{

}

/*!
 * \~english \brief DLabel::DLabel constructs an instance of DLabel.
 * \~english \param text is used to initialize the content, passed to QLabel constructor.
 * \~english \param parent is passed to QLabel constructor.
 * \~english \param f is passed to QLabel constructor.
 */

/*!
 * \~chinese \brief DLabel 的重载构造函数
 * \~chinese \param text 用于初始化本文内容，传递给 QLabel 构造函数
 * \~chinese \param parent 初始化 QLabel 的构造函数
 * \~chinese \param f 初始化 QLabel 的构造函数
 */
DLabel::DLabel(const QString &text, QWidget *parent, Qt::WindowFlags f)
    : QLabel(text, parent, f)
{

}

DWIDGET_END_NAMESPACE
