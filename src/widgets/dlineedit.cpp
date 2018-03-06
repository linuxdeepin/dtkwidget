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
 * \class DLineEdit
 * \brief The DLineEdit class provides a styled QLineEdit.
 *
 * DLineEdit has an optional action button (DImageButton) at the right side which can be used
 * to provide extra user interaction, for example: to change the echo mode of
 * the line edit.
 *
 * Also, DLineEdit can be set on or off alert mode, warning the user of some
 * errors.
 */

/*!
 * \brief DLineEdit::DLineEdit constructs an instance of DLineEdit.
 * \param parent is passed to QLineEdit constructor.
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

void DLineEdit::setAlert(bool isAlert)
{
    Q_D(DLineEdit);

    if (isAlert == d->m_isAlert) {
        return;
    }

    d->m_isAlert = isAlert;

    Q_EMIT alertChanged(isAlert);
}

/*!
 * \property DLineEdit::alert
 * \brief This property shows whether the line edit is in alert mode or not.
 *
 * There'll be a extra frame colored in orage like color showing if the alert
 * mode is on, to remind the user that the input is wrong.
 */
bool DLineEdit::isAlert() const
{
    D_DC(DLineEdit);

    return d->m_isAlert;
}

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

void DLineEdit:: hideAlertMessage()
{
    Q_D(DLineEdit);

    if (d->tooltip) {
        d->tooltip->hide();
    }
}

void DLineEdit::setIconVisible(bool visible)
{
    Q_D(DLineEdit);

    if (visible == d->m_rightIcon->isVisible()) {
        return;
    }

    d->m_rightIcon->setVisible(visible);

    if (visible) {
        QLineEditPrivate *d_d = reinterpret_cast<QLineEditPrivate*>(d_ptr.data());

        addAction(d->m_iconAction, TrailingPosition);

        if (d_d->trailingSideWidgets.size() > 1) {
            if ((*(d_d->trailingSideWidgets.end() - 1)).action == d->m_iconAction) {
                d_d->trailingSideWidgets.insert(d_d->trailingSideWidgets.begin(), *d_d->trailingSideWidgets.erase(d_d->trailingSideWidgets.end() - 1));
                QResizeEvent resize_event(size(), size());
                qApp->sendEvent(this, &resize_event);
            }
        }
     } else {
        removeAction(d->m_iconAction);
    }
}

/*!
 * \property DLineEdit::iconVisible
 * \brief This property holds whether the action button can be seen.
 */
bool DLineEdit::iconVisible() const
{
    D_DC(DLineEdit);

    return d->m_rightIcon->isVisible();
}

/*!
 * \property DLineEdit::normalIcon
 * \brief This property holds the image used as the normal state of the action button.
 */
QString DLineEdit::normalIcon() const
{
    D_DC(DLineEdit);

    return d->m_rightIcon->getNormalPic();
}

void DLineEdit::setNormalIcon(const QString &normalIcon)
{
    Q_D(DLineEdit);

    d->m_rightIcon->setNormalPic(normalIcon);
}

/*!
 * \property DLineEdit::hoverIcon
 * \brief This property holds the image used as the hover state of the action button.
 */
QString DLineEdit::hoverIcon() const
{
    D_DC(DLineEdit);

    return d->m_rightIcon->getHoverPic();
}

void DLineEdit::setHoverIcon(const QString &hoverIcon)
{
    Q_D(DLineEdit);

    d->m_rightIcon->setHoverPic(hoverIcon);
}

/*!
 * \property DLineEdit::pressIcon
 * \brief This property holds the image used as the pressed state of the action button.
 */
QString DLineEdit::pressIcon() const
{
    D_DC(DLineEdit);

    return d->m_rightIcon->getPressPic();
}

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
