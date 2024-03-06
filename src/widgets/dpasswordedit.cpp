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
@~english
  @class Dtk::Widget::DPasswordEdit
  \inmodule dtkwidget

  @brief DPasswordEdit provides a password input box for user input.
  @brief The DPasswordEdit class provides a widget to let user input password.
 */

/*!
@~english
  @brief DPasswordEdit constructor.
  @brief DPasswordEdit::DPasswordEdit constructs an instance of DPasswordEdit.
    
  @param[in] parent The parent widget passed to DLineEdit constructor.
 */

DPasswordEdit::DPasswordEdit(QWidget *parent)
    : DLineEdit(*new DPasswordEditPrivate(this), parent)
{
    D_D(DPasswordEdit);

    d->init();
}

/*!
@~english
  @property DPasswordEdit::isEchoMode

  @brief This attribute controls the visibility of user input.
  @brief This property holds whether the user input should be displayed directly
  or show as dots.
  
  @sa QLineEdit::EchoMode
 */
bool DPasswordEdit::isEchoMode() const
{
    return lineEdit()->echoMode() == QLineEdit::Normal;
}

/*!
@~english
  @brief Setting the model of display.
  @sa QLineEdit::EchoMode
  @param[in] mode
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
@~english
  @brief DPasswordEdit::setEchoButtonIsVisible Set the visibility of the button of echo mode.
  @param[in] visible "true" means the button of echo mode is visible, while "false" means the button of echo mode is not visible.
 */
void DPasswordEdit::setEchoButtonIsVisible(bool visible)
{
    D_D(DPasswordEdit);

    d->togglePasswordVisibleButton->setVisible(visible);
    setRightWidgetsVisible(visible);
}

/*!
@~english
  @brief DPasswordEdit::echoButtonIsVisible
  @return If "true", the button of echo mode is visible. If "false", the button of echo mode is not visible.
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
