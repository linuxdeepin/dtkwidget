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

#include "dpasswordedit.h"
#include "private/dpasswordedit_p.h"

#include <DUtil>
#include <DStyle>
#include <DSuggestButton>

#include <QDebug>
#include <QTimer>
#include <QPushButton>
#include <QLineEdit>


DWIDGET_BEGIN_NAMESPACE

/*!
 * \~english \class DPasswordEdit
 * \~english \brief The DPasswordEdit class provides a widget to let user input password.
 */

/*!
 * \~english \brief DPasswordEdit::DPasswordEdit constructs an instance of DPasswordEdit.
 * \~english \param parent is passed to DLineEdit constructor.
 */

/*!
 * \~chinese \class DPasswordEdit
 * \~chinese \brief DPasswordEdit提供了一个让用户输入的密码框
 */

/*!
 * \~chinese \brief DPasswordEdit的构造函数
 *
 * @param parent
 */
DPasswordEdit::DPasswordEdit(QWidget *parent)
    : DLineEdit(*new DPasswordEditPrivate(this), parent)
{
    D_D(DPasswordEdit);

    d->init();
}

/*!
 * \~english \property DPasswordEdit::isEchoMode
 * \~english \brief This property holds whether the user input should be displayed directly
 * or show as dots.
 *
 * \~chinese \see QLineEdit::EchoMode
 */

/*!
 * \~chinese \property DPasswordEdit::isEchoMode
 * \~chinese \brief 该属性会控制用户输入是否可见
 *
 * \~chinese \see QLineEdit::EchoMode
 */
bool DPasswordEdit::isEchoMode() const
{
    return lineEdit()->echoMode() == QLineEdit::Normal;
}

/*!
 * \~chinese \brief 设置显示模式
 * \~chinese \see QLineEdit::EchoMode
 * @param mode
 */
void DPasswordEdit::setEchoMode(QLineEdit::EchoMode mode)
{
    if (mode != echoMode()) {
        DLineEdit::setEchoMode(mode);

        // To inform the style sheet system that our style sheet needs
        // to be recalculated.
        Q_EMIT echoModeChanged(mode == QLineEdit::Normal);

        D_D(DPasswordEdit);

        if (isEchoMode()) {
            d->togglePasswordVisibleButton->setIcon(DStyle::standardIcon(style(), DStyle::SP_HidePassword));
        } else {
            d->togglePasswordVisibleButton->setIcon(DStyle::standardIcon(style(),DStyle::SP_ShowPassword));
        }
    }
}

/*!
 * \~chinese \brief DPasswordEdit::setEchoButtonIsVisible 设置输入密码是否可见
 * \~chinese \param visible　true输入密码可见　false不可见
 */
void DPasswordEdit::setEchoButtonIsVisible(bool visible)
{
    D_D(DPasswordEdit);

    d->togglePasswordVisibleButton->setVisible(visible);
    setRightWidgetsVisible(visible);
}

/*!
 * \~chinese \brief DPasswordEdit::echoButtonIsVisible
 * \~chinese \return true输入密码可见　false不可见
 */
bool DPasswordEdit::echoButtonIsVisible() const
{
    D_DC(DPasswordEdit);
    return d->togglePasswordVisibleButton->isVisible();
}

DPasswordEditPrivate::DPasswordEditPrivate(DPasswordEdit *q)
    : DLineEditPrivate(q)
{

}

void DPasswordEditPrivate::init()
{
    D_Q(DPasswordEdit);

    q->lineEdit()->setEchoMode(QLineEdit::Password);

    QList<QWidget *> list;
    togglePasswordVisibleButton = new DSuggestButton;
    togglePasswordVisibleButton->setIcon(DStyle::standardIcon(q->style(), DStyle::SP_ShowPassword));
    togglePasswordVisibleButton->setIconSize(QSize(24, 24));

    list.append(togglePasswordVisibleButton);
    q->setRightWidgets(list);

    q->connect(togglePasswordVisibleButton, SIGNAL(clicked()), q, SLOT(_q_toggleEchoMode()));
}

void DPasswordEditPrivate::_q_toggleEchoMode()
{
    D_Q(DPasswordEdit);

    if (q->isEchoMode()) {
        togglePasswordVisibleButton->setIcon(DStyle::standardIcon(q->style(),DStyle::SP_ShowPassword));
        q->setEchoMode(q->lineEdit()->Password);
    } else {
        togglePasswordVisibleButton->setIcon(DStyle::standardIcon(q->style(), DStyle::SP_HidePassword));
        q->setEchoMode(q->lineEdit()->Normal);
    }
}

DWIDGET_END_NAMESPACE

#include "moc_dpasswordedit.cpp"
