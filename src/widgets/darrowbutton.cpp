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

#include "darrowbutton.h"
#include "dthememanager.h"

DWIDGET_BEGIN_NAMESPACE

ArrowButtonIcon::ArrowButtonIcon(QWidget *parent) :
    QLabel(parent)
{
    setAttribute(Qt::WA_TransparentForMouseEvents);
}

void ArrowButtonIcon::setArrowDirection(int direction)
{
    m_direction = direction;
    style()->unpolish(this);
    style()->polish(this);// force a stylesheet recomputation
}

void ArrowButtonIcon::setButtonState(int state)
{
    m_buttonState = state;
    style()->unpolish(this);
    style()->polish(this);// force a stylesheet recomputation
}

int ArrowButtonIcon::arrowDirection() const
{
    return m_direction;
}

int ArrowButtonIcon::buttonState() const
{
    return m_buttonState;
}

/*!
    \~chinese \class DArrowButton
    \~chinese \brief 可以使用 DArrowButton 类快速创建箭头形状的按钮。

    \~chinese \image html DArrowButton.png

    \~chinese DArrowButton 提供了快速的方式创建包含箭头标识的按钮，并允许通过 setArrowDirection() 设置箭头方向来直接改按钮的箭头图标的方向。
    此外，还可以通过 \ref arrowButtonDirection 和 \ref arrowButtonState 属性获取和修改箭头按钮的状态。

    \~chinese \sa QLabel
*/

/*!
    \~english \class DArrowButton
    \~english \brief DArrowButton allowed you create button with arrow icon conveniently.

    \~english \image html DArrowButton.png

    \~english You can use DArrowButton to create button with arrow icon, and it also allowed you update the arrow direction by
    calling setArrowDirection() . You can also update arrow state via arrowButtonDirection and arrowButtonState property.

    \~english \sa QLabel
*/

/*!
 * \~english \brief Construct a new DArrowButton with DArrowButton::ArrowDown direction
 * \~chinese \brief 构造一个 DArrowButton 箭头按钮，默认箭头方向向下
 */
DArrowButton::DArrowButton(QWidget *parent)
    : QLabel(parent)
{
    DThemeManager::registerWidget(this);

    setFixedSize(IMAGE_BUTTON_WIDTH, BUTTON_HEIGHT);

    m_normalLabel = new ArrowButtonIcon(this);
    m_normalLabel->setObjectName("ArrowButtonIconNormal");
    m_normalLabel->setFixedSize(size());
    m_normalLabel->move(0, 0);

    m_hoverLabel = new ArrowButtonIcon(this);
    m_hoverLabel->setObjectName("ArrowButtonIconHover");
    m_hoverLabel->setFixedSize(size());
    m_hoverLabel->move(0, 0);
    m_hoverLabel->setVisible(false);

    m_pressLabel = new ArrowButtonIcon(this);
    m_pressLabel->setObjectName("ArrowButtonIconPress");
    m_pressLabel->setFixedSize(size());
    m_pressLabel->move(0, 0);
    m_pressLabel->setVisible(false);

    setArrowDirection(DArrowButton::ArrowDown);
}

/*!
 * \~english \brief Set arrow direction of the button
 * \~chinese \brief 设置按钮的箭头方向
 */
void DArrowButton::setArrowDirection(ArrowDirection direction)
{
    m_arrowDirection = direction;
    updateIconDirection(direction);
}

/*!
 * \~english \brief Get the arrow direction of the button.
 * \~chinese \brief 获取箭头方向
 */
int DArrowButton::arrowDirection() const
{
    return m_arrowDirection;
}

/*!
 * \~english \brief Set the button state.
 * \~chinese \brief 设置按钮状态
 */
void DArrowButton::setButtonState(ArrowButtonState state)
{
    m_buttonState = state;
    updateIconState(state);
}

/*!
 * \~english \brief Get the button state
 * \~chinese \brief 获得按钮状态
 */
int DArrowButton::buttonState() const
{
    return m_buttonState;
}

void DArrowButton::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)

    Q_EMIT mousePress();
    m_normalLabel->setVisible(false);
    m_hoverLabel->setVisible(false);
    m_pressLabel->setVisible(true);

    setButtonState(DArrowButton::ArrowStatePress);
}

void DArrowButton::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)

    Q_EMIT mouseRelease();
    m_normalLabel->setVisible(true);
    m_hoverLabel->setVisible(false);
    m_pressLabel->setVisible(false);

    setButtonState(DArrowButton::ArrowStateNormal);
}

/* \reimp */
void DArrowButton::enterEvent(QEvent *)
{
    Q_EMIT mouseEnter();
}

/* \reimp */
void DArrowButton::leaveEvent(QEvent *)
{
    Q_EMIT mouseLeave();
}

void DArrowButton::initButtonState()
{
//    QStateMachine * machine = new QStateMachine(this);
//    QState * hoverState = new QState(machine);
//    hoverState->assignProperty(this,"pos",QPoint(0,0));
//    QState * hideState = new QState(machine);
//    hideState->assignProperty(this,"pos",QPoint(0,height()));

//    machine->setInitialState(hoverState);

//    QPropertyAnimation *sa = new QPropertyAnimation(this, "pos");
//    sa->setDuration(200);
//    sa->setEasingCurve(QEasingCurve::InSine);
//    connect(sa,&QPropertyAnimation::finished,this,&Panel::hasShown);

//    QPropertyAnimation *ha = new QPropertyAnimation(this, "pos");
//    ha->setDuration(200);
//    ha->setEasingCurve(QEasingCurve::InSine);
//    connect(ha,&QPropertyAnimation::finished,this,&Panel::hasHidden);

//    QSignalTransition *ts1 = hoverState->addTransition(this,SIGNAL(startHide()), hideState);
//    ts1->addAnimation(ha);
//    connect(ts1,&QSignalTransition::triggered,[=](int value = 2){
//        m_HSManager->SetState(value);
//    });
//    QSignalTransition *ts2 = hideState->addTransition(this,SIGNAL(startShow()),hoverState);
//    ts2->addAnimation(sa);
//    connect(ts2,&QSignalTransition::triggered,[=](int value = 0){
//        m_HSManager->SetState(value);
//    });

//    machine->start();
}

/*!
 * \enum DArrowButton::ArrowDirection
 *
 * \~english \brief The ArrowDirection enum indicate the direction of the arrow icon in the arrown button
 * \~chinese \brief DArrowButton::ArrowDirection 表示箭头图标的方向。
 *
 * \var DArrowButton::ArrowDirection DArrowButton::ArrowUp
 * \~english Up direction
 * \~chinese 箭头朝上
 *
 * \var DArrowButton::ArrowDirection DArrowButton::ArrowDown
 * \~english Down direction
 * \~chinese 箭头朝下
 *
 * \var DArrowButton::ArrowDirection DArrowButton::ArrowLeft
 * \~english Left direction
 * \~chinese 箭头朝左
 *
 * \var DArrowButton::ArrowDirection DArrowButton::ArrowRight
 * \~english Right direction
 * \~chinese 箭头朝右
 */

/*!
 * \enum DArrowButton::ArrowButtonState
 *
 * \~english \brief The ArrowDirection enum indicate the direction of the arrow icon in the arrown button
 * \~chinese \brief DArrowButton::ArrowDirection 表示箭头图标的方向。
 *
 * \var DArrowButton::ArrowButtonState DArrowButton::ArrowStateNormal
 * \~english Normal state
 * \~chinese 普通状态
 *
 * \var DArrowButton::ArrowButtonState DArrowButton::ArrowStateHover
 * \~english Mouse hover state
 * \~chinese 鼠标在按钮上方悬停状态
 *
 * \var DArrowButton::ArrowButtonState DArrowButton::ArrowStatePress
 * \~english Button got pressed state
 * \~chinese 按钮被按下状态
 */

/*!
 * \~english \brief Update arrow direction of the button
 * \~chinese \brief 更新箭头按钮中箭头的方向
 */
void DArrowButton::updateIconDirection(ArrowDirection direction)
{
    m_normalLabel->setArrowDirection(direction);
    m_hoverLabel->setArrowDirection(direction);
    m_pressLabel->setArrowDirection(direction);
}

/*!
 * \~english \brief Update icon state of the button
 * \~chinese \brief 更新箭头按钮中图标的状态
 */
void DArrowButton::updateIconState(ArrowButtonState state)
{
    m_normalLabel->setButtonState(state);
    m_hoverLabel->setButtonState(state);
    m_pressLabel->setButtonState(state);
}

DWIDGET_END_NAMESPACE
