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
  \class Dtk::Widget::DPasswordEdit
  \inmodule dtkwidget

  \brief DPasswordEdit提供了一个让用户输入的密码框.
  \brief The DPasswordEdit class provides a widget to let user input password.
 */

/*!
  \brief DPasswordEdit的构造函数.
  \brief DPasswordEdit::DPasswordEdit constructs an instance of DPasswordEdit.
  
  \a parent is passed to DLineEdit constructor.
 */
DPasswordEdit::DPasswordEdit(QWidget *parent)
    : DLineEdit(*new DPasswordEditPrivate(this), parent)
{
    D_D(DPasswordEdit);

    d->init();
}

/*!
  \property DPasswordEdit::isEchoMode

  \brief 该属性会控制用户输入是否可见.
  \brief This property holds whether the user input should be displayed directly
  or show as dots.
  
  \sa QLineEdit::EchoMode
 */
bool DPasswordEdit::isEchoMode() const
{
    return lineEdit()->echoMode() == QLineEdit::Normal;
}

/*!
  \brief 设置显示模式
  \sa QLineEdit::EchoMode
  \a mode
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
  \brief DPasswordEdit::setEchoButtonIsVisible 设置输入密码是否可见
  \a visible　true输入密码可见　false不可见
 */
void DPasswordEdit::setEchoButtonIsVisible(bool visible)
{
    D_D(DPasswordEdit);

    d->togglePasswordVisibleButton->setVisible(visible);
    setRightWidgetsVisible(visible);
}

/*!
  \brief DPasswordEdit::echoButtonIsVisible
  \return true输入密码可见　false不可见
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
    q->lineEdit()->setAttribute(Qt::WA_InputMethodEnabled, false);

    QList<QWidget *> list;
    togglePasswordVisibleButton = new DSuggestButton;
    togglePasswordVisibleButton->setAccessibleName("DPasswordEditPasswordVisibleButton");
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
