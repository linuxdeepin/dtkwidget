/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#include "dstyleoption.h"

#include "dlineedit.h"
#include "private/dlineedit_p.h"

DWIDGET_BEGIN_NAMESPACE

/*!
 * \~english \class DStyleOption
 * \brief DStyleOption is the base class of deepin style option, use for dxcb plugins
 * \param widget
 */

/*!
 * \~chinese \class DStyleOption
 * \brief dcxb插件通过DStyleOption来控制控件的样式
 * \param widget
 */


/*!
 * \~english \class DStyleOptionSuggestButton
 * \brief DStyleOptionSuggestButton handle the style option of Dtk::Widget::DSuggestButton
 * \param widget
 */
/*!
 * \~english \enum DStyleOptionSuggestButton::ButtonFeature
 * \brief This enum extend types of features a Dtk::Widget::DSuggestButton can have
 */
/*!
 * \~english \var DStyleOptionSuggestButton::SuggestButton
 * \brief Default Button
 */

/*!
 * \~chinese \class DStyleOptionSuggestButton
 * \brief Dtk::Widget::DSuggestButton使用的样式配置
 * \param widget
 */
/*!
 * \~chinese \enum DStyleOptionSuggestButton::ButtonFeature
 * \brief Dtk::Widget::DSuggestButton扩展的样式
 */
/*!
 * \~chinese \var DStyleOptionSuggestButton::SuggestButton
 * \brief 默认样式
 */


/*!
 * \~english \class DStyleOptionLineEdit
 * \brief DStyleOptionLineEdit handle the style option of Dtk::Widget::DLineEdit
 * \param widget
 */
/*!
 * \~english \var DStyleOptionLineEdit::None
 * \brief Default none style
 */
/*!
 * \~english \var DStyleOptionLineEdit::Alert
 * \brief Alert style
 */
/*!
 * \~english \var DStyleOptionLineEdit::IconButton
 * \brief Edit with an icon
 */


/*!
 * \~chinese \class DStyleOptionLineEdit
 * \brief Dtk::Widget::DLineEdit使用的样式配置
 * \param widget
 */
/*!
 * \~chinese \enum DStyleOptionLineEdit::LineEditFeature
 * \brief Dtk::Widget::DSuggestButton扩展的样式
 */
/*!
 * \~chinese \var DStyleOptionLineEdit::None
 * \brief 不使用自定义样式，保持Qt默认样式。
 */
/*!
 * \~chinese \var DStyleOptionLineEdit::Alert
 * \brief 警告状态样式
 */
/*!
 * \~chinese \var DStyleOptionLineEdit::IconButton
 * \brief 包含图标按钮的编辑框样式
 */


/*!
 * \~english \brief Init style with widget.
 * \param widget
 */
/*!
 * \~chinese \brief 控件初始化样式时调用，可以用于重写控件样式。
 * \param widget
 */
void DStyleOption::init(QWidget *widget)
{
    Q_UNUSED(widget)
}

/*!
 * \~english \brief DStyleOptionSuggestButton::init set style option for Dtk::Widget::DSuggestButton
 * \param widget
 * \sa Dtk::Widget::DSuggestButton
 */
/*!
 * \~chinese \brief 初始化Dtk::Widget::DSuggestButton样式，设置为 DStyleOptionSuggestButton::SuggestButton 。
 * \param widget
 * \sa Dtk::Widget::DSuggestButton
 */
void DStyleOptionSuggestButton::init(QWidget *widget)
{
    Q_UNUSED(widget)

    features |= QStyleOptionButton::ButtonFeature(SuggestButton);
}

/*!
 * \~english \brief DStyleOptionSuggestButton::init set style option for Dtk::Widget::DLineEdit
 * \param widget
 * \sa Dtk::Widget::DLineEdit
 */
/*!
 * \~chinese \brief 根据Dtk::Widget::DLineEdit控件状态初始化样式
 * \param widget
 * \sa Dtk::Widget::DLineEdit
 */
void DStyleOptionLineEdit::init(QWidget *widget)
{
    if (const DLineEdit *edit = qobject_cast<DLineEdit*>(widget)) {
        if (edit->isAlert()) {
            features |= Alert;
        }

        if (edit->iconVisible()) {
            features |= IconButton;
            iconButtonRect = edit->d_func()->m_rightIcon->geometry();
        }
    }
}

DWIDGET_END_NAMESPACE
