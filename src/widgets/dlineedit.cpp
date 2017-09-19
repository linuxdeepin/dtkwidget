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

#include <QHBoxLayout>
#include <QDebug>
#include <QResizeEvent>

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
    D_THEME_INIT_WIDGET(DLineEdit, alert);

    Q_D(DLineEdit);
    d->init();
}

DLineEdit::DLineEdit(DLineEditPrivate &q, QWidget *parent)
    : QLineEdit(parent),
      DObject(q)
{
    D_THEME_INIT_WIDGET(DLineEdit, alert);

    Q_D(DLineEdit);
    d->init();
}

void DLineEdit::setAlert(bool isAlert)
{
    Q_D(DLineEdit);

    if (isAlert == d->m_isAlert)
        return;

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

void DLineEdit::setIconVisible(bool visible)
{
    Q_D(DLineEdit);

    if (visible == d->m_rightIcon->isVisible())
        return;

    d->m_rightIcon->setVisible(visible);
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
}

DLineEditPrivate::DLineEditPrivate(DLineEdit *q)
    : DObjectPrivate(q)
{
}

void DLineEditPrivate::init()
{
    Q_Q(DLineEdit);
    m_insideFrame = new QFrame(q);
    m_insideFrame->setObjectName("LineEditInsideFrame");
    m_rightIcon = new DImageButton;
    m_rightIcon->hide();
    m_centralHLayout = new QHBoxLayout;
    m_centralHLayout->addStretch();
    m_centralHLayout->addWidget(m_rightIcon);
    m_centralHLayout->setSpacing(0);
    m_centralHLayout->setMargin(0);

    q->setLayout(m_centralHLayout);
    q->setContextMenuPolicy(Qt::NoContextMenu);

    q->connect(m_rightIcon, &DImageButton::clicked, q, &DLineEdit::iconClicked);
    q->connect(q, SIGNAL(sizeChanged(QSize)), q, SLOT(_q_resizeInsideFrame(QSize)));
}

void DLineEditPrivate::_q_resizeInsideFrame(const QSize &size)
{
    m_insideFrame->setFixedHeight(size.height() - 1);
    m_insideFrame->setFixedWidth(size.width());
}

DWIDGET_END_NAMESPACE

#include "moc_dlineedit.cpp"
