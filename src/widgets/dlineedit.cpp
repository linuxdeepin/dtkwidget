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

#include <DToolTip>
#include <DPalette>

#define private public
#ifndef slots
#define slots Q_SLOTS
#endif
#include <private/qlineedit_p.h>
#undef private

DWIDGET_BEGIN_NAMESPACE

/*!
 * \~chinese \class DLineEdit
 * \~chinese \brief DLineEdit一个聚合 QLineEdit 的输入框
 * \~chinese \li DLineEdit提供了向输入框左右两端插入控件的函数
 * \~chinese \li DLineEdit提供了带警告颜色的输入框
 * \~chinese \li DLineEdit提供了带文本警告消息的输入框
 */

/*!
 * \~chinese \brief DLineEdit的构造函数
 * \~chinese \param parent参数被发送到 QWidget 构造函数。
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

/*!
 * \~chinese \brief DLineEdit::lineEdit返回 QLineEdit 对象
 * \~chinese \row 若 DLineEdit 不满足输入框的使用需求，请用此函数抛出的对象
 * \~chinese \return
 */
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
 * \~chinese \brief DLineEdit::setAlert设置是否开启警告模式
 * \~chinese \row 警告模式，开启警告模式，输入框内将显示警告颜色
 * \~chinese \param isAlert是否开启警告模式
 */
void DLineEdit::setAlert(bool isAlert)
{
    Q_D(DLineEdit);

    if (isAlert == d->m_isAlert) {
        return;
    }

    d->m_isAlert = isAlert;

    DPalette p = palette();

    if (isAlert) {
        p.setColor(QPalette::Button, QColor(241, 57, 50, qRound(0.15 * 255)));
        d->lineEdit->setPalette(p);
    } else {
        d->lineEdit->setPalette(QPalette());
    }
    update();

    Q_EMIT alertChanged(isAlert);
}

/*!
 * \~chinese \brief DLineEdit::alert返回当前是否处于警告模式
 */
bool DLineEdit::isAlert() const
{
    D_DC(DLineEdit);

    return d->m_isAlert;
}

/*!
 * \~chinese \brief DLineEdit::showAlertMessage显示警告消息
 * \~chinese \row 显示指定的文本消息，超过指定时间后警告消息消失.
 * \~chinese \row \note 时间参数为-1时，警告消息将一直存在
 * \~chinese \param text警告的文本
 * \~chinese \param duration显示的时间长度，单位毫秒
 */
void DLineEdit::showAlertMessage(const QString &text, int duration)
{
    D_D(DLineEdit);

    if (!d->tooltip) {
        d->tooltip = new DToolTip(text);
        d->tooltip->setObjectName("AlertTooltip");
        d->tooltip->setForegroundRole(DPalette::TextWarning);

        d->frame = new DFloatingWidget;
        d->frame->setFramRadius(DStyle::pixelMetric(style(), DStyle::PM_FrameRadius));
        d->frame->setBackgroundRole(QPalette::ToolTipBase);
        d->frame->setWidget(d->tooltip);
        d->frame->setParent(parentWidget());
    }

    d->tooltip->setText(text);
    int w = DStyle::pixelMetric(style(), DStyle::PM_FloatingWidgetShadowMargins);
    d->frame->move(geometry().x() - w, geometry().y() + lineEdit()->height() + lineEdit()->y());
    d->frame->show();
    d->frame->adjustSize();
    d->frame->raise();

    if (duration <= 0)
        return;

    QTimer::singleShot(duration, d->frame, &QWidget::close);
}

/*!
 * \~chinese \brief DLineEdit:: hideAlertMessage隐藏警告消息框
 */

void DLineEdit:: hideAlertMessage()
{
    Q_D(DLineEdit);

    if (d->frame) {
        d->frame->hide();
    }
}

/*!
 * \~chinese \brief DLineEdit::setLeftWidgets向输入框左侧添加控件
 * \~chinese \row 将 QList 里的控件插入到输入框的左侧
 * \~chinese \row \note 多次调用，只有最后一次调用生效
 * \~chinese \param list存储控件的列表
 */

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

/*!
 * \~chinese \brief DLineEdit::setRightWidgets向输入框右侧添加控件
 * \~chinese \row 将 QList 里的控件插入到输入框的右侧
 * \~chinese \row \note 多次调用，只有最后一次调用生效
 * \~chinese \param list存储控件的列表
 */

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

/*!
 * \~chinese \brief DLineEdit::setLeftWidgetsVisible是否隐藏输入框左侧控件
 * \~chinese \param visible是否隐藏
 */
void DLineEdit::setLeftWidgetsVisible(bool visible)
{
    Q_D(DLineEdit);
    d->leftWidget->setVisible(visible);
}
/**
 * \~chinese \brief DLineEdit::setRightWidgetsVisible是否隐藏输入框右侧控件
 * \~chinese \param visible是否隐藏
 */
void DLineEdit::setRightWidgetsVisible(bool visible)
{
    Q_D(DLineEdit);
    d->rightWidget->setVisible(visible);
}

/*!
 * \~chinese \brief DLineEdit::setClearButtonEnabled
 * \~chinese \sa QLineEdit::setClearButtonEnabled()
 */
void DLineEdit::setClearButtonEnabled(bool enable)
{
    Q_D(DLineEdit);
    d->lineEdit->setClearButtonEnabled(enable);
}

/*!
 * \~chinese \brief DLineEdit::isClearButtonEnabled
 * \~shinese \sa QLineEdit::isClearButtonEnabled()
 */
bool DLineEdit::isClearButtonEnabled() const
{
    D_DC(DLineEdit);
    return  d->lineEdit->isClearButtonEnabled();
}

/*!
 * \~chinese \brief DLineEdit::setText
 * \~chinese \sa QLineEdit::setText()
 */
void DLineEdit::setText(const QString &text)
{
    D_D(DLineEdit);
    d->lineEdit->setText(text);
}

/*!
 * \~chinese \brief DLineEdit::text
 * \~chinese \sa QLineEdit::text()
 */
QString DLineEdit::text()
{
    D_DC(DLineEdit);
    return d->lineEdit->text();
}

/*!
 * \~chinese \brief DLineEdit::clear
 * \~chinese \sa QLineEdit::clear();
 */
void DLineEdit::clear()
{
    D_D(DLineEdit);
    return d->lineEdit->clear();
}

/*!
 * \~chinese \brief DLineEdit::echoMode
 * \~chinese \sa QLineEdit::echoMode();
 */
QLineEdit::EchoMode DLineEdit::echoMode() const
{
    D_DC(DLineEdit);
    return d->lineEdit->echoMode();
}

/*!
 * \~chinese \brief DLineEdit::setEchoMode
 * \~chinese \sa QLineEdit::setEchoMode()
 */
void DLineEdit::setEchoMode(QLineEdit::EchoMode mode)
{
    D_D(DLineEdit);
    d->lineEdit->setEchoMode(mode);
}

/*!
 * \~chinese \brief DLineEdit::eventFilter
 * \~chinese \row 该过滤器不做任何过滤动作，但会监控输入框的焦点状态，并发送信号 focusChanged()。
 */
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

bool DLineEdit::event(QEvent *e)
{
    D_D(DLineEdit);
    if (e->type() == QEvent::Move || e->type() == QEvent::Resize) {
        if (d->frame) {
            int w = DStyle::pixelMetric(style(), DStyle::PM_FloatingWidgetShadowMargins);
            d->frame->move(geometry().x() - w, geometry().y() + lineEdit()->height() + lineEdit()->y());
        }
    }
    return QWidget::event(e);
}

DLineEditPrivate::DLineEditPrivate(DLineEdit *q)
    : DObjectPrivate(q)
    , tooltip(nullptr)
    , frame(nullptr)
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
