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

#include "dswitchbutton.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QDebug>
#include <QApplication>
#include <QIcon>
#include "dthememanager.h"
#include "private/dswitchbutton_p.h"

DWIDGET_BEGIN_NAMESPACE

/*!
 * \class DSwitchButton
 * \brief The DSwitchButton class provides switch like widget.
 *
 * User can put the switch on/off the turn some feature on/off.
 *
 * It's inspired by UISwitch of Apple,
 * see https://developer.apple.com/documentation/uikit/uiswitch.
 */

/*!
 * \brief DSwitchButton::DSwitchButton constructs an instance of DSwitchButton.
 * \param parent is passed to QFrame constructor.
 */
DSwitchButton::DSwitchButton(QWidget *parent) :
    QFrame(parent),
    DObject(*new DSwitchButtonPrivate(this))
{
    setObjectName("DSwitchButton");

    setMaximumSize(38, 18);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    D_THEME_INIT_WIDGET(DSwitchButton);

    D_D(DSwitchButton);

    d->init();

    connect(d->m_innerAnimation, &QVariantAnimation::valueChanged, [&](){
        this->update();
    });
}

/*!
 * \property DSwitchButton::checked
 * \brief This property holds whether the switch is on or off.
 *
 * True if the switch is on, otherwise false.
 */
bool DSwitchButton::checked() const
{
    D_DC(DSwitchButton);

    return d->m_checked;
}

int DSwitchButton::animationDuration() const
{
    D_DC(DSwitchButton);

    return d->m_innerAnimation->duration();
}

QEasingCurve::Type DSwitchButton::animationType() const
{
    D_DC(DSwitchButton);

    return d->m_innerAnimation->easingCurve().type();
}

double DSwitchButton::animationStartValue() const
{
    D_DC(DSwitchButton);

    return d->m_animationStartValue;
}

double DSwitchButton::animationEndValue() const
{
    D_DC(DSwitchButton);

    return d->m_animationEndValue;
}

QString DSwitchButton::disabledImageSource() const
{
    D_DC(DSwitchButton);

    return d->m_disabledImageSource;
}

QString DSwitchButton::enabledImageSource() const
{
    D_DC(DSwitchButton);

    return d->m_enabledImageSource;
}

QSize DSwitchButton::sizeHint() const
{
    return maximumSize();
}

void DSwitchButton::setChecked(bool arg)
{
    D_D(DSwitchButton);

    if (d->m_checked != arg) {
        d->m_checked = arg;

        if(arg){
            d->m_innerAnimation->setStartValue(d->m_animationStartValue);
            d->m_innerAnimation->setEndValue(d->m_animationEndValue);
        }else{
            d->m_innerAnimation->setStartValue(d->m_animationEndValue);
            d->m_innerAnimation->setEndValue(d->m_animationStartValue);
        }
        d->m_innerAnimation->start();

        Q_EMIT checkedChanged(arg);
    }
}

void DSwitchButton::setAnimationDuration(int arg)
{
    D_D(DSwitchButton);

    d->m_innerAnimation->setDuration(arg);
}

void DSwitchButton::setAnimationType(QEasingCurve::Type arg)
{
    D_D(DSwitchButton);

    d->m_innerAnimation->setEasingCurve(arg);
}

bool DSwitchButton::setDisabledImageSource(const QString &arg)
{
    D_D(DSwitchButton);

    d->m_disabledImageSource = arg;
    bool ok = false;

    d->m_disabledImage = d->loadPixmap(arg, ok);

    return ok;
}

bool DSwitchButton::setEnabledImageSource(const QString &arg)
{
    D_D(DSwitchButton);

    d->m_enabledImageSource = arg;

    bool ok = false;

    d->m_enabledImage = d->loadPixmap(arg, ok);

    return ok;
}

void DSwitchButton::setAnimationStartValue(double animationStartValue)
{
    D_D(DSwitchButton);

    d->m_animationStartValue = animationStartValue;
}

void DSwitchButton::setAnimationEndValue(double animationEndValue)
{
    D_D(DSwitchButton);

    d->m_animationEndValue = animationEndValue;
}

void DSwitchButton::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    D_D(DSwitchButton);

    QPixmap m_innerImage;

    if(isEnabled()){
        m_innerImage = d->m_enabledImage;
    }else{
        m_innerImage = d->m_disabledImage;
    }

    if(m_innerImage.isNull())
        return;

    QPainter p(this);
    p.setRenderHints(QPainter::Antialiasing);

    const qreal ratio = m_innerImage.devicePixelRatioF();

    QPainterPath path;
    path.addRoundedRect(rect(),
                        (m_innerImage.height() / 2.0) / ratio,
                        (m_innerImage.height() / 2.0) / ratio);
    path.closeSubpath();

    p.setClipPath(path);

    double m_innerImageX = 0;

    if(d->m_innerAnimation->state() == QVariantAnimation::Stopped){
        if(!d->m_checked){
            m_innerImageX = d->m_animationStartValue;
        }
    }else{
        m_innerImageX = d->m_innerAnimation->currentValue().toDouble();
    }

    p.drawPixmap(m_innerImageX, 0, m_innerImage);
}

void DSwitchButton::mousePressEvent(QMouseEvent *e)
{
    D_D(DSwitchButton);

    if(e->button() == Qt::LeftButton){
        setChecked(!d->m_checked);
        e->accept();
    }
}

DSwitchButtonPrivate::DSwitchButtonPrivate(DSwitchButton *qq)
    : DObjectPrivate(qq)
{
    init();
}

DSwitchButtonPrivate::~DSwitchButtonPrivate()
{
    m_innerAnimation->deleteLater();
}

void DSwitchButtonPrivate::init()
{
    m_checked = false;
    m_innerAnimation = new QVariantAnimation;
    m_animationStartValue = 0;
    m_animationEndValue = 0;
}

QPixmap DSwitchButtonPrivate::loadPixmap(const QString &path, bool &ok)
{
    D_Q(DSwitchButton);

    QPixmap pixmap;

    qreal ratio = 1.0;

    const qreal devicePixel = q->devicePixelRatioF();

    if (!qFuzzyCompare(ratio, devicePixel)) {
        ok = pixmap.load(qt_findAtNxFile(path, devicePixel, &ratio));
        pixmap = pixmap.scaled(devicePixel / ratio * pixmap.width(),
                               devicePixel / ratio * pixmap.height(),
                               Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        pixmap.setDevicePixelRatio(devicePixel);
    } else {
        ok = pixmap.load(path);
    }

    return pixmap;
}

DWIDGET_END_NAMESPACE
