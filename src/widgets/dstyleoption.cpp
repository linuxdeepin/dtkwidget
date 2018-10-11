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
 * \class DStyleOption
 * \brief DStyleOption is the base class of deepin style option
 * \param widget
 */

/*!
 * \class DStyleOptionSuggestButton
 * \brief DStyleOptionSuggestButton handle the style option of Dtk::Widget::DSuggestButton
 * \param widget
 */

/*!
 * \class DStyleOptionLineEdit
 * \brief DStyleOptionLineEdit handle the style option of Dtk::Widget::DLineEdit
 * \param widget
 */


/*!
 * \brief Init style with widget
 * \param widget
 */
void DStyleOption::init(QWidget *widget)
{
    Q_UNUSED(widget)
}

/*!
 * \brief DStyleOptionSuggestButton::init set style option for Dtk::Widget::DSuggestButton
 * \param widget
 * \sa Dtk::Widget::DSuggestButton
 */
void DStyleOptionSuggestButton::init(QWidget *widget)
{
    Q_UNUSED(widget)

    features |= QStyleOptionButton::ButtonFeature(SuggestButton);
}

/*!
 * \brief DStyleOptionSuggestButton::init set style option for Dtk::Widget::DLineEdit
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
