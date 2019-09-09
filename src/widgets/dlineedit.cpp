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
#include "private/dlineedit_p.h"
#include "darrowrectangle.h"

#include <QHBoxLayout>

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
    setClearButtonEnabled(true);
}

DLineEdit::DLineEdit(DLineEditPrivate &q, QWidget *parent)
    : QLineEdit(parent),
      DObject(q)
{

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

void DLineEdit::setLeftWidgetsVisible(bool visible)
{
    Q_D(DLineEdit);
    int left = 0;
    const QMargins &margins = contentsMargins();

    if (visible) {
        d->leftWidget->adjustSize();
        left = d->leftWidget->width();
    }
    setContentsMargins(left, margins.top(), margins.right(), margins.bottom());
}

void DLineEdit::setRightWidgetsVisible(bool visible)
{
    Q_D(DLineEdit);
    int right = 0;
    const QMargins &margins = contentsMargins();

    if (visible) {
        d->rightWidget->adjustSize();
        right = d->rightWidget->width();
    }
    setContentsMargins(margins.left(), margins.top(), right, margins.bottom());
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

    if (d->rightWidget != nullptr) {
        d->rightWidget->setGeometry(width() - d->rightWidget->width(), 0,
                                    d->rightWidget->width(), height());
    }

    if (d->leftWidget != nullptr) {
        d->leftWidget->resize(d->leftWidget->width(), height());
    }
}

DLineEditPrivate::DLineEditPrivate(DLineEdit *q)
    : DObjectPrivate(q)
    , leftWidget(nullptr)
    , rightWidget(nullptr)
{
}

DWIDGET_END_NAMESPACE
