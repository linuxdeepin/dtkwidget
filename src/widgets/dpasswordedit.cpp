// SPDX-FileCopyrightText: 2015 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dpasswordedit.h"
#include "private/dpasswordedit_p.h"
#include "dsizemode.h"

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

void DPasswordEdit::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::StyleChange) {
        D_D(DPasswordEdit);
        d->togglePasswordVisibleButton->setFixedWidth(d->defaultButtonWidth());
        d->togglePasswordVisibleButton->setIconSize(d->defaultIconSize());
    }
    return DLineEdit::changeEvent(event);
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
    togglePasswordVisibleButton->setFixedWidth(defaultButtonWidth());
    togglePasswordVisibleButton->setIconSize(defaultIconSize());

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
