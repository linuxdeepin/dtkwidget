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

#include "dimagebutton.h"
#include "dconstants.h"
#include "dthememanager.h"
#include "private/dimagebutton_p.h"

#include <QMouseEvent>
#include <QEvent>
#include <QIcon>
#include <QApplication>
#include <QImageReader>

DWIDGET_BEGIN_NAMESPACE

/*!
 * \class DImageButton
 * \brief The DImageButton class provides an easy way to create image based buttons.
 *
 * A DImageButton has four states: normal, hover, pressed and checked, developers
 * should provide at least the normal state image to get it to work properly.
 */

/*!
 * \brief DImageButton::DImageButton constructs an instance of DImageButton
 * \param parent is the parent widget that the button will be attached to.
 */
DImageButton::DImageButton(QWidget *parent)
    : QLabel(parent)
    , DObject(*new DImageButtonPrivate(this))
{
    D_THEME_INIT_WIDGET(DImageButton, checked);

    D_D(DImageButton);

    d->updateIcon();
}

/*!
 * \brief DImageButton::DImageButton This is an overloaded function.
 *
 * It's convinient to provide the images that used at the initialize stage.
 *
 * \param normalPic is the path of image to be used as the normal state.
 * \param hoverPic is the path of image to be used as the hover state.
 * \param pressPic is the path of image to be used as the pressed state.
 * \param parent is the parent widget that the button will be attached to.
 */
DImageButton::DImageButton(const QString &normalPic, const QString &hoverPic, const QString &pressPic, QWidget *parent)
    : QLabel(parent)
    , DObject(*new DImageButtonPrivate(this))
{
    D_THEME_INIT_WIDGET(DImageButton, checked);

    D_D(DImageButton);

    if (!normalPic.isEmpty()) {
        d->m_normalPic = normalPic;
    }
    if (!hoverPic.isEmpty()) {
        d->m_hoverPic = hoverPic;
    }
    if (!pressPic.isEmpty()) {
        d->m_pressPic = pressPic;
    }

    setCheckable(false);

    d->updateIcon();
}

/*!
 * \brief DImageButton::DImageButton This is an overloaded function.
 *
 * It's convinient to provide the images that used at the initialize stage.
 *
 * \param normalPic is the path of image to be used as the normal state.
 * \param hoverPic is the path of image to be used as the hover state.
 * \param pressPic is the path of image to be used as the pressed state.
 * \param checkedPic is the path of image to be used as the checked state.
 * \param parent is the parent widget that the button will be attached to.
 */
DImageButton::DImageButton(const QString &normalPic, const QString &hoverPic,
                           const QString &pressPic, const QString &checkedPic, QWidget *parent)
    : QLabel(parent)
    , DObject(*new DImageButtonPrivate(this))
{
    D_THEME_INIT_WIDGET(DImageButton, checked);

    D_D(DImageButton);

    if (!normalPic.isEmpty()) {
        d->m_normalPic = normalPic;
    }
    if (!hoverPic.isEmpty()) {
        d->m_hoverPic = hoverPic;
    }
    if (!pressPic.isEmpty()) {
        d->m_pressPic = pressPic;
    }
    if (!checkedPic.isEmpty()) {
        d->m_checkedPic = checkedPic;
    }

    setCheckable(true);

    d->updateIcon();
}

DImageButton::~DImageButton()
{
}

void DImageButton::enterEvent(QEvent *event)
{
    D_D(DImageButton);

    setCursor(Qt::PointingHandCursor);

    if (!d->m_isChecked && isEnabled()) {
        d->setState(Hover);
    }

    event->accept();
    //QLabel::enterEvent(event);
}

void DImageButton::leaveEvent(QEvent *event)
{
    D_D(DImageButton);

    if (!d->m_isChecked && isEnabled()) {
        d->setState(Normal);
    }

    event->accept();
    //QLabel::leaveEvent(event);
}

void DImageButton::mousePressEvent(QMouseEvent *event)
{
    D_D(DImageButton);

    if (event->button() != Qt::LeftButton) {
        return;
    }

    d->setState(Press);

    event->accept();
    //QLabel::mousePressEvent(event);
}

void DImageButton::mouseReleaseEvent(QMouseEvent *event)
{
    D_D(DImageButton);

    if (!rect().contains(event->pos())) {
        return;
    }

    if (d->m_isCheckable) {
        d->m_isChecked = !d->m_isChecked;
        if (d->m_isChecked) {
            d->setState(Checked);
        } else {
            d->setState(Normal);
        }
    } else {
        d->setState(Hover);
    }

    event->accept();
    //QLabel::mouseReleaseEvent(event);

    if (event->button() == Qt::LeftButton) {
        Q_EMIT clicked();
    }
}

void DImageButton::mouseMoveEvent(QMouseEvent *event)
{
    D_D(DImageButton);

    if (!d->m_isCheckable && !rect().contains(event->pos())) {
        d->setState(Normal);
    }
}

void DImageButton::setEnabled(bool enabled)
{
    D_D(DImageButton);
    if (!enabled) {
        d->setState(Disabled);
    } else {
        d->setState(Normal);
    }

    QWidget::setEnabled(enabled);
}

void DImageButton::setDisabled(bool disabled)
{
    setEnabled(!disabled);
}

void DImageButton::setCheckable(bool flag)
{
    D_D(DImageButton);

    d->m_isCheckable = flag;

    if (!d->m_isCheckable) {
        d->setState(Normal);
    }
}

void DImageButton::setChecked(bool flag)
{
    D_D(DImageButton);

    if (d->m_isCheckable == false) {
        return;
    }

    d->m_isChecked = flag;
    if (d->m_isChecked) {
        d->setState(Checked);
    } else {
        d->setState(Normal);
    }

    emit checkedChanged(d->m_isChecked);
}

/*!
 * \property DImageButton::checked
 * \brief This property indicates the DImageButton is in checked state of not.
 */

bool DImageButton::isChecked() const
{
    D_DC(DImageButton);

    return d->m_isChecked;
}

/*!
 * \property DImageButton::isCheckable
 * \brief This property indicates the DImageButton can be checked or not.
 */

bool DImageButton::isCheckable() const
{
    D_DC(DImageButton);

    return d->m_isCheckable;
}

void DImageButton::setNormalPic(const QString &normalPicPixmap)
{
    D_D(DImageButton);

    d->m_normalPic = normalPicPixmap;
    d->updateIcon();
}

void DImageButton::setHoverPic(const QString &hoverPicPixmap)
{
    D_D(DImageButton);

    d->m_hoverPic = hoverPicPixmap;
    d->updateIcon();
}

void DImageButton::setPressPic(const QString &pressPicPixmap)
{
    D_D(DImageButton);

    d->m_pressPic = pressPicPixmap;
    d->updateIcon();
}

void DImageButton::setCheckedPic(const QString &checkedPicPixmap)
{
    D_D(DImageButton);

    d->m_checkedPic = checkedPicPixmap;
    d->updateIcon();
}

void DImageButton::setDisabledPic(const QString &disabledPicPixmap)
{
    D_D(DImageButton);

    d->m_disabledPic = disabledPicPixmap;
    d->updateIcon();
}

/*!
 * \property DImageButton::getNormalPic
 * \brief This property holds the path of the image used as the normal state.
 */
const QString DImageButton::getNormalPic() const
{
    D_DC(DImageButton);

    return d->m_normalPic;
}

/*!
 * \property DImageButton::getHoverPic
 * \brief This property holds the path of the image used as the hover state.
 */
const QString DImageButton::getHoverPic() const
{
    D_DC(DImageButton);

    return d->m_hoverPic;
}

/*!
 * \property DImageButton::getPressPic
 * \brief This property holds the path of the image used as the pressed state.
 */
const QString DImageButton::getPressPic() const
{
    D_DC(DImageButton);

    return d->m_pressPic;
}

/*!
 * \property DImageButton::getCheckedPic
 * \brief This property holds the path of the image used as the checked state.
 */
const QString DImageButton::getCheckedPic() const
{
    D_DC(DImageButton);

    return d->m_checkedPic;
}

const QString DImageButton::getDisabledPic() const
{
    D_DC(DImageButton);

    return d->m_disabledPic;
}

/*!
 * \brief DImageButton::setState change the image by state
 * \param state: see DImageButton::State
 */
void DImageButton::setState(DImageButton::State state)
{
    D_D(DImageButton);

    d->setState(state);
}


/*!
 * \brief DImageButton::getState
 * \return the state that the DImageButton is in.
 */
DImageButton::State DImageButton::getState() const
{
    D_DC(DImageButton);

    return d->m_state;
}

DImageButton::DImageButton(DImageButtonPrivate &q, QWidget *parent)
    : QLabel(parent)
    , DObject(q)
{
    D_THEME_INIT_WIDGET(DImageButton, checked);

    D_D(DImageButton);

    d->updateIcon();
}

DImageButtonPrivate::DImageButtonPrivate(DImageButton *qq)
    : DObjectPrivate(qq)
{

}

DImageButtonPrivate::~DImageButtonPrivate()
{

}

void DImageButtonPrivate::updateIcon()
{
    D_Q(DImageButton);

    switch (m_state) {
    case DImageButton::Hover:
        if (!m_hoverPic.isEmpty()) { q->setPixmap(loadPixmap(m_hoverPic)); }
        break;
    case DImageButton::Press:
        if (!m_pressPic.isEmpty()) { q->setPixmap(loadPixmap(m_pressPic)); }
        break;
    case DImageButton::Checked:
        if (!m_checkedPic.isEmpty()) { q->setPixmap(loadPixmap(m_checkedPic)); }
        break;
    case DImageButton::Disabled:
        if (!m_disabledPic.isEmpty()) { q->setPixmap(loadPixmap(m_disabledPic)); }
        break;
    default:
        if (!m_normalPic.isEmpty()) { q->setPixmap(loadPixmap(m_normalPic)); }
        break;
    }

    q->setAlignment(Qt::AlignCenter);

    Q_EMIT q->stateChanged();
}

void DImageButtonPrivate::setState(DImageButton::State state)
{
    if (m_state == state) {
        return;
    }

    m_state = state;

    updateIcon();
}

QPixmap DImageButtonPrivate::loadPixmap(const QString &path)
{
    D_Q(DImageButton);

    qreal ratio = 1.0;

    const qreal devicePixelRatio = q->devicePixelRatioF();

    QPixmap pixmap;

    if (!qFuzzyCompare(ratio, devicePixelRatio)) {
        QImageReader reader;
        reader.setFileName(qt_findAtNxFile(path, devicePixelRatio, &ratio));
        if (reader.canRead()) {
            reader.setScaledSize(reader.size() * (devicePixelRatio / ratio));
            pixmap = QPixmap::fromImage(reader.read());
            pixmap.setDevicePixelRatio(devicePixelRatio);
        }
    } else {
        pixmap.load(path);
    }

    return pixmap;
}

DWIDGET_END_NAMESPACE
