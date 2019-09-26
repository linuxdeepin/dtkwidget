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
    : QWidget(parent)
    , DObject(*new DLineEditPrivate(this))
{
    D_D(DLineEdit);
    d->init();
}

DLineEdit::~DLineEdit()
{

}

QLineEdit *DLineEdit::lineEdit() const
{
    D_DC(DLineEdit);
    return d->lineEdit;
}

DLineEdit::DLineEdit(DLineEditPrivate &q, QWidget *parent)
    : QWidget(parent)
    , DObject(q)
{
    d_func()->init();
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

    if (list.isEmpty())
        return;


    d->leftWidget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(d->leftWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    d->hLayout->insertWidget(0, d->leftWidget);

    QList<QWidget *>::const_iterator itor;

    for (itor = list.constBegin(); itor != list.constEnd(); itor++) {
        layout->addWidget(*itor);
    }

    d->leftWidget->adjustSize();
}

void DLineEdit::setRightWidgets(const QList<QWidget *> &list)
{
    Q_D(DLineEdit);

    if (d->rightWidget != nullptr) {
        d->rightWidget->hide();
        d->rightWidget->deleteLater();
        d->rightWidget = nullptr;
    }

    if (list.isEmpty())
        return;

    d->rightWidget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(d->rightWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    d->hLayout->addWidget(d->rightWidget);
    QList<QWidget *>::const_iterator itor;

    for (itor = list.constBegin(); itor != list.constEnd(); itor++) {
        layout->addWidget(*itor);
    }

    d->rightWidget->adjustSize();
}

void DLineEdit::setLeftWidgetsVisible(bool visible)
{
    Q_D(DLineEdit);
    d->leftWidget->setVisible(visible);
}

void DLineEdit::setRightWidgetsVisible(bool visible)
{
    Q_D(DLineEdit);
    d->rightWidget->setVisible(visible);
}

void DLineEdit::setClearButtonEnabled(bool enable)
{
    Q_D(DLineEdit);
    d->lineEdit->setClearButtonEnabled(enable);
}

bool DLineEdit::isClearButtonEnabled() const
{
    D_DC(DLineEdit);
    return  d->lineEdit->isClearButtonEnabled();
}

void DLineEdit::setText(const QString &text)
{
    D_D(DLineEdit);
    d->lineEdit->setText(text);
}

QString DLineEdit::text()
{
    D_DC(DLineEdit);
    return d->lineEdit->text();

}

QLineEdit::EchoMode DLineEdit::echoMode() const
{
    D_DC(DLineEdit);
    return d->lineEdit->echoMode();
}

void DLineEdit::setEchoMode(QLineEdit::EchoMode mode)
{
    D_D(DLineEdit);
    d->lineEdit->setEchoMode(mode);
}

bool DLineEdit::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched)

    if (event->type() == QEvent::FocusIn) {
        Q_EMIT focusChanged(true);
    } else if (event->type() == QEvent::FocusOut) {
        Q_EMIT focusChanged(false);
    }

    return QWidget::eventFilter(watched, event);
}

DLineEditPrivate::DLineEditPrivate(DLineEdit *q)
    : DObjectPrivate(q)
    , leftWidget(nullptr)
    , rightWidget(nullptr)
    , lineEdit(nullptr)
    , hLayout(nullptr)
{

}

void DLineEditPrivate::init()
{
    Q_Q(DLineEdit);

    hLayout = new QHBoxLayout(q);
    lineEdit = new QLineEdit;

    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->addWidget(lineEdit);

    lineEdit->installEventFilter(q);

    q->lineEdit()->setClearButtonEnabled(true);

    q->connect(lineEdit, &QLineEdit::textChanged, q, &DLineEdit::textChanged);
    q->connect(lineEdit, &QLineEdit::textEdited, q, &DLineEdit::textEdited);
    q->connect(lineEdit, &QLineEdit::cursorPositionChanged, q, &DLineEdit::cursorPositionChanged);
    q->connect(lineEdit, &QLineEdit::returnPressed, q, &DLineEdit::returnPressed);
    q->connect(lineEdit, &QLineEdit::editingFinished, q, &DLineEdit::editingFinished);
    q->connect(lineEdit, &QLineEdit::selectionChanged, q, &DLineEdit::selectionChanged);
}

DWIDGET_END_NAMESPACE
