/*
 * Copyright (C) 2016 ~ 2017 Deepin Technology Co., Ltd.
 *
 * Author:     Iceyer <me@iceyer.net>
 *
 * Maintainer: Iceyer <me@iceyer.net>
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
#include "dtoast.h"

#include <DObjectPrivate>

#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QHBoxLayout>
#include <QLabel>
#include <QIcon>

#include "dthememanager.h"
#include "dgraphicsgloweffect.h"
#include "dhidpihelper.h"

DWIDGET_BEGIN_NAMESPACE

class DToastPrivate: public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    DToastPrivate(DToast *qq);

    QIcon   icon;
    QLabel  *iconLabel = Q_NULLPTR;
    QLabel  *textLabel = Q_NULLPTR;

    int     duration = 2000;

    QPropertyAnimation  *animation  = Q_NULLPTR;
    DGraphicsGlowEffect *effect     = Q_NULLPTR;

    void initUI();
private:
    D_DECLARE_PUBLIC(DToast)
};

DToast::DToast(QWidget *parent) :
    QFrame(parent), DObject(*new DToastPrivate(this))
{
    D_D(DToast);
    DThemeManager::registerWidget(this);
    d->initUI();
}

DToast::~DToast()
{

}

QString DToast::text() const
{
    D_DC(DToast);
    return d->textLabel->text();
}

QIcon DToast::icon() const
{
    D_DC(DToast);
    return d->icon;
}

int DToast::duration() const
{
    D_DC(DToast);
    return d->duration;
}

qreal DToast::opacity() const
{
    D_DC(DToast);
    return d->effect->opacity();
}

void DToast::setText(QString text)
{
    D_D(DToast);
    d->textLabel->setVisible(true);
    d->textLabel->setText(text);
}

void DToast::setIcon(QString iconfile)
{
    D_D(DToast);
    d->icon = QIcon(iconfile);
    d->iconLabel->setVisible(true);
    d->iconLabel->setPixmap(DHiDPIHelper::loadNxPixmap(iconfile));
}

void DToast::setIcon(QIcon icon, QSize defaultSize)
{
    D_D(DToast);
    d->icon = icon;
    d->iconLabel->setVisible(true);
    d->iconLabel->setPixmap(d->icon.pixmap(icon.actualSize(defaultSize)));
}

void DToast::setDuration(int duration)
{
    D_D(DToast);
    d->duration = duration;
}

void DToast::setOpacity(qreal opacity)
{
    D_D(DToast);
    d->effect->setOpacity(opacity);
    update();
}

void DToast::pop()
{
    Q_D(DToast);

    adjustSize();
    show();

    if (d->animation) {
        return;
    }

    int _duration = d->duration < 0 ? 2000 : d->duration;

    d->animation = new QPropertyAnimation(this, "opacity");
    d->animation->setDuration(_duration);
    d->animation->setStartValue(0);
    d->animation->setKeyValueAt(0.4, 1.0);
    d->animation->setKeyValueAt(0.8, 1.0);
    d->animation->setEndValue(0);
    d->animation->start();
    d->animation->connect(d->animation, &QPropertyAnimation::finished,
    this, [ = ]() {
        hide();
        d->animation->deleteLater();
        d->animation = Q_NULLPTR;
    });
}

void DToast::pack()
{
    Q_D(DToast);
    hide();
    if (d->animation) {
        d->animation->stop();
        d->animation->deleteLater();
        d->animation = Q_NULLPTR;
    }
}

void DToast::showEvent(QShowEvent *event)
{
    Q_EMIT visibleChanged(true);
    return QWidget::showEvent(event);
}

void DToast::hideEvent(QHideEvent *event)
{
    Q_EMIT visibleChanged(false);
    return QWidget::hideEvent(event);
}

DToastPrivate::DToastPrivate(DToast *qq)
    : DObjectPrivate(qq)
{

}

void DToastPrivate::initUI()
{
    D_Q(DToast);
    q->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    q->setWindowFlags(q->windowFlags() | Qt::WindowStaysOnTopHint);

    auto layout = new QHBoxLayout(q);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(12);

    iconLabel = new QLabel;
    iconLabel->setVisible(false);
    textLabel = new QLabel;
    textLabel->setVisible(false);

    layout->addWidget(iconLabel);
    layout->addWidget(textLabel);

    effect = new DGraphicsGlowEffect(q);
    effect->setBlurRadius(20.0);
    effect->setColor(QColor(0, 0, 0, 255 / 10));
    effect->setOffset(0, 0);
    q->setGraphicsEffect(effect);
    q->hide();
}


DWIDGET_END_NAMESPACE
