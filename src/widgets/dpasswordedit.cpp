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
#include "dthememanager.h"
#include "private/dpasswordedit_p.h"

#include <DUtil>

#include <QDebug>
#include <QTimer>


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
    // This will do the trick refreshing your style sheet for you
    // after your registered property changed.
    DThemeManager::registerWidget(this, QStringList({"isEchoMode"}));

    D_D(DPasswordEdit);

    d->init();
}

/*!
 * \~english \property DPasswordEdit::isEchoMode
 * \~english \brief This property holds whether the user input should be displayed directly
 * or show as dots.
 *
 * \see QLineEdit::EchoMode
 */

/*!
 * \~chinese \property DPasswordEdit::isEchoMode
 * \~chinese \brief 该属性会控制用户输入是否可见
 *
 * \see QLineEdit::EchoMode
 */
bool DPasswordEdit::isEchoMode() const
{
    return echoMode() == Normal;
}

/*!
 * \~chinese \brief 设置显示模式
 * \see QLineEdit::EchoMode
 * @param mode
 */
void DPasswordEdit::setEchoMode(QLineEdit::EchoMode mode)
{
    if (mode != echoMode()) {
        QLineEdit::setEchoMode(mode);

        // To inform the style sheet system that our style sheet needs
        // to be recalculated.
        Q_EMIT echoModeChanged(mode == Normal);
    }
}

DPasswordEditPrivate::DPasswordEditPrivate(DPasswordEdit *q)
    : DLineEditPrivate(q)
{

}

void DPasswordEditPrivate::init()
{
    D_Q(DPasswordEdit);

    q->setEchoMode(q->Password);
    q->setIconVisible(true);

    // FIXME: DPasswordEdit instances that initialized with a parent will fail
    // to load the little eye icon if we don't do the below thing.
    DUtil::TimerSingleShot(0,  [q] { q->setStyleSheet(q->styleSheet()); });

    q->connect(q, SIGNAL(iconClicked()), q, SLOT(_q_toggleEchoMode()));
}

void DPasswordEditPrivate::_q_toggleEchoMode()
{
    D_Q(DPasswordEdit);

    if (q->isEchoMode()) {
        q->setEchoMode(q->Password);
    } else {
        q->setEchoMode(q->Normal);
    }
}

DWIDGET_END_NAMESPACE

#include "moc_dpasswordedit.cpp"
