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

#include "dlineedit.h"
#include "dthememanager.h"
#include "private/dlineedit_p.h"
#include "darrowrectangle.h"
#include "dstyleoption.h"

#include <QHBoxLayout>
#include <QResizeEvent>
#include <QWidgetAction>

#define private public
#ifndef slots
#define slots Q_SLOTS
#endif
#include <private/qlineedit_p.h>
#undef private

DWIDGET_BEGIN_NAMESPACE

/*!
 * \~english \class DLineEdit
 * \~english \brief The DLineEdit class provides a styled QLineEdit.
 *
 * \~english DLineEdit has an optional action button (DImageButton) at the right side which can be used
 * \~english to provide extra user interaction, for example: to change the echo mode of
 * the line edit.
 *
 * \~english Also, DLineEdit can be set on or off alert mode, warning the user of some
 * errors.
 */

/*!
 * \~english \brief DLineEdit::DLineEdit constructs an instance of DLineEdit.
 * \~english \param parent is passed to QLineEdit constructor.
 */

/*!
 * \~chinese \class DLineEdit
 * \~chinese \brief DLineEdit提供了一个修改过的QLineEdit样式
 *
 * \~chinese DLineEdit右侧提供了可选的动作按钮，可以使用额外的用户交互，例如: 改变密码显示
 */

/*!
 * \~chinese \brief DLineEdit的构造函数
 *
 * \~chinese \param parent 调用QLineEdit的构造函数
 */
DLineEdit::DLineEdit(QWidget *parent)
    : QLineEdit(parent),
      DObject(*new DLineEditPrivate(this))
{
    Q_D(DLineEdit);
    d->init();
}

DLineEdit::DLineEdit(DLineEditPrivate &q, QWidget *parent)
    : QLineEdit(parent),
      DObject(q)
{
    Q_D(DLineEdit);
    d->init();
}

/*!
 * \~chinese \brief 设置是否显示警告
 *
 * \~chinese @param isAlert 是否显示警告
 */
void DLineEdit::setAlert(bool isAlert)
{
    Q_D(DLineEdit);

    if (isAlert == d->m_isAlert) {
        return;
    }

    d->m_isAlert = isAlert;

    update();

    Q_EMIT alertChanged(isAlert);
}

/*!
 * \~english\property DLineEdit::alert
 * \~english \brief This property shows whether the line edit is in alert mode or not.
 *
 * There'll be a extra frame colored in orage like color showing if the alert
 * mode is on, to remind the user that the input is wrong.
 */

/*!
 * \~chinese \property DLineEdit::alert
 * \~chinese \brief 该属性会返回当前是否处于警告模式
 *
 * 将会有一个警告的颜色在额外的边框上显示，如果警告模式开启，将会提示用户输入错误
 */

/*!
 * \~chinese \brief 返回当前是否处于警告模式
 *
 * 将会有一个警告的颜色在额外的边框上显示，如果警告模式开启，将会提示用户输入错误
 * @return true
 * @return false
 */
bool DLineEdit::isAlert() const
{
    D_DC(DLineEdit);

    return d->m_isAlert;
}

/*!
 * \~chinese \brief 设置的文本会在警告模式下显示
 *
 * @param text 警告的文本
 * @param duration 显示的时间长度
 */
void DLineEdit::showAlertMessage(const QString &text, int duration)
{
    D_D(DLineEdit);

    if (!d->tooltip) {
        d->tooltip = new DArrowRectangle(DArrowRectangle::ArrowTop, this);
        d->tooltip->setObjectName("AlertTooltip");

        QLabel *label = new QLabel(d->tooltip);

        label->setWordWrap(true);
        label->setMaximumWidth(width());
        d->tooltip->setContent(label);
        d->tooltip->setBackgroundColor(DThemeManager::instance()->theme(this) == "light" ? Qt::white : Qt::black);
        d->tooltip->setArrowX(15);
        d->tooltip->setArrowHeight(5);

        QTimer::singleShot(duration, d->tooltip, [d] {
            d->tooltip->deleteLater();
            d->tooltip = Q_NULLPTR;
        });
    }

    QLabel *label = qobject_cast<QLabel *>(d->tooltip->getContent());

    if (!label) {
        return;
    }

    label->setText(text);
    label->adjustSize();

    const QPoint &pos = mapToGlobal(QPoint(15, height()));

    d->tooltip->show(pos.x(), pos.y());
}

/*!
 * \~chinese \brief 隐藏警告的消息框
 *
 */
void DLineEdit:: hideAlertMessage()
{
    Q_D(DLineEdit);

    if (d->tooltip) {
        d->tooltip->hide();
    }
}

/*!
 * \~chinese \brief 设置图标是否可见
 *
 * @param visible
 */
void DLineEdit::setIconVisible(bool visible)
{
    Q_D(DLineEdit);

    if (visible == d->m_rightIcon->isVisible()) {
        return;
    }

    d->m_rightIcon->setVisible(visible);

    if (visible) {

        addAction(d->m_iconAction, TrailingPosition);
#ifndef Q_OS_WIN
        QLineEditPrivate *d_d = reinterpret_cast<QLineEditPrivate *>(d_ptr.data());
        if (d_d->trailingSideWidgets.size() > 1) {
            if ((*(d_d->trailingSideWidgets.end() - 1)).action == d->m_iconAction) {
                d_d->trailingSideWidgets.insert(d_d->trailingSideWidgets.begin(), *d_d->trailingSideWidgets.erase(d_d->trailingSideWidgets.end() - 1));
                QResizeEvent resize_event(size(), size());
                qApp->sendEvent(this, &resize_event);
            }
        }
#endif
    } else {
        removeAction(d->m_iconAction);
    }
}

/*!
 * \~english \property DLineEdit::iconVisible
 * \~english \brief This property holds whether the action button can be seen.
 */

/*!
 * \~chinese \property DLineEdit::iconVisible
 * \~chinese 这个属性将会决定动作按钮的图标是否可见
 */
bool DLineEdit::iconVisible() const
{
    D_DC(DLineEdit);

    return d->m_rightIcon->isVisible();
}

/*!
 * \~english \property DLineEdit::normalIcon
 * \~english \brief This property holds the image used as the normal state of the action button.
 */

/*!
 * \~chinese \property DLineEdit::normalIcon
 * \~chinese 该属性返回normal状态的图标
 */
QString DLineEdit::normalIcon() const
{
    D_DC(DLineEdit);

    return d->m_rightIcon->getNormalPic();
}

/*!
 * \~chinese \brief 设置normal状态的图标
 *
 * @param normalIcon
 */
void DLineEdit::setNormalIcon(const QString &normalIcon)
{
    Q_D(DLineEdit);

    d->m_rightIcon->setNormalPic(normalIcon);
}

/*!
 * \~english \property DLineEdit::hoverIcon
 * \~english \brief This property holds the image used as the hover state of the action button.
 */

/*!
 * \~chinese \property DLineEdit::hoverIcon
 * \~chinese 该属性返回鼠标在动作按钮上时，按钮的图标
 */
QString DLineEdit::hoverIcon() const
{
    D_DC(DLineEdit);

    return d->m_rightIcon->getHoverPic();
}

/*!
 * \~chinese \brief 设置鼠标在动作按钮上时，按钮的图标
 *
 * @param hoverIcon 鼠标在动作按钮上时，按钮的图标的路径
 */
void DLineEdit::setHoverIcon(const QString &hoverIcon)
{
    Q_D(DLineEdit);

    d->m_rightIcon->setHoverPic(hoverIcon);
}

/*!
 * \~english \property DLineEdit::pressIcon
 * \~english \brief This property holds the image used as the pressed state of the action button.
 */

/*!
 * \~chinese \property DLineEdit::pressIcon
 * \~chinese 该属性返回鼠标按下时动作按钮的图标
 */
QString DLineEdit::pressIcon() const
{
    D_DC(DLineEdit);

    return d->m_rightIcon->getPressPic();
}

void DLineEdit::setLeftWidgets(const QList<QWidget *> &list)
{
    Q_D(DLineEdit);

    if (d->leftWidget != nullptr) {
        d->leftWidget->hide();
        d->leftWidget->deleteLater();
        d->leftWidget = nullptr;
    }

    if (list.isEmpty()) {
        const QMargins &margins = contentsMargins();
        setContentsMargins(0, margins.top(), margins.right(), margins.bottom());

        return;
    }

    d->leftWidget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(d->leftWidget);
    layout->setContentsMargins(0, 0, 5, 0);

    QList<QWidget *>::const_iterator itor;

    for (itor = list.constBegin(); itor != list.constEnd(); itor++) {
        layout->addWidget(*itor, 0, Qt::AlignVCenter);
    }

    d->leftWidget->adjustSize();

    int leftWidth = d->leftWidget->width();
    d->leftWidget->resize(leftWidth, height());

    const QMargins &margins = contentsMargins();
    setContentsMargins(leftWidth, margins.top(), margins.right(), margins.bottom());
}

void DLineEdit::setRightWidgets(const QList<QWidget *> &list)
{
    Q_D(DLineEdit);

    if (d->rightWidget != nullptr) {
        d->rightWidget->hide();
        d->rightWidget->deleteLater();
        d->rightWidget = nullptr;
    }

    if (list.isEmpty()) {
        const QMargins &margins = contentsMargins();
        setContentsMargins(margins.top(), margins.left(), 0, margins.bottom());

        return;
    }

    d->rightWidget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(d->rightWidget);
    layout->setContentsMargins(5, 0, 0, 0);
    QList<QWidget *>::const_iterator itor;

    for (itor = list.constBegin(); itor != list.constEnd(); itor++) {
        layout->addWidget(*itor, 0, Qt::AlignVCenter);
    }

    d->rightWidget->adjustSize();

    int rightWidth = d->rightWidget->width();

    d->rightWidget->resize(rightWidth, height());
    d->rightWidget->setGeometry(width() - rightWidth, 0, rightWidth, height());
    const QMargins &margins = contentsMargins();
    setContentsMargins(margins.left(), margins.top(), rightWidth, margins.bottom());
}

/*!
 * \~chinese \brief 设置鼠标按下时动作按钮的图标
 *
 * @param pressIcon 鼠标按下时动作按钮的图标路径
 */
void DLineEdit::setPressIcon(const QString &pressIcon)
{
    Q_D(DLineEdit);

    d->m_rightIcon->setPressPic(pressIcon);
}

void DLineEdit::focusInEvent(QFocusEvent *e)
{
    Q_EMIT focusChanged(true);
    QLineEdit::focusInEvent(e);
}

void DLineEdit::focusOutEvent(QFocusEvent *e)
{
    Q_EMIT focusChanged(false);
    QLineEdit::focusOutEvent(e);
}

void DLineEdit::resizeEvent(QResizeEvent *e)
{
    QLineEdit::resizeEvent(e);

    Q_EMIT sizeChanged(e->size());

    D_D(DLineEdit);

    d->m_rightIcon->setFixedHeight(e->size().height() - 2);

    if (d->rightWidget != nullptr) {
        d->rightWidget->setGeometry(width() - d->rightWidget->width(), 0,
                                    d->rightWidget->width(), height());
    }

    if (d->leftWidget != nullptr) {
        d->leftWidget->resize(d->leftWidget->width(), height());
    }
}

bool DLineEdit::eventFilter(QObject *watched, QEvent *event)
{
    D_D(DLineEdit);

    if (watched == d->m_rightIcon) {
        if (event->type() == QEvent::Move) {
            d->m_rightIcon->move(width() - d->m_rightIcon->width() - 1, 1);
        }
    }

    return false;
}

DLineEditPrivate::DLineEditPrivate(DLineEdit *q)
    : DObjectPrivate(q)
    , leftWidget(nullptr)
    , rightWidget(nullptr)
{
}

void DLineEditPrivate::init()
{
    Q_Q(DLineEdit);
    m_rightIcon = new DImageButton(q);
    m_rightIcon->setObjectName("IconButton");
    m_rightIcon->installEventFilter(q);

    m_iconAction = new QWidgetAction(q);
    m_iconAction->setDefaultWidget(m_rightIcon);
    m_rightIcon->hide();

    q->connect(m_rightIcon, &DImageButton::clicked, q, &DLineEdit::iconClicked);
}

DWIDGET_END_NAMESPACE

#include "moc_dlineedit.cpp"
