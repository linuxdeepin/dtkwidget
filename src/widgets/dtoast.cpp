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

    QBrush  background  = Qt::white;
    int     radius      = 4;
    QColor  borderColor = QColor(0, 0, 0, 255 / 10);
    QIcon   icon;
    QLabel  *iconLabel = Q_NULLPTR;
    QLabel  *textLabel = Q_NULLPTR;

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
    D_THEME_INIT_WIDGET(DToast);
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

qreal DToast::opacity() const
{
    D_DC(DToast);
    return d->effect->opacity();
}

QColor Dtk::Widget::DToast::borderColor() const
{
    D_DC(DToast);
    return d->borderColor;
}

QBrush DToast::background() const
{
    D_DC(DToast);
    return d->background;
}

int DToast::radius() const
{
    D_DC(DToast);
    return d->radius;

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

void DToast::paintEvent(QPaintEvent *)
{
    D_D(DToast);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

    // draw border outer
    auto outer = true;
    auto radius = d->radius;
    auto penWidthf = 1.0;
    auto background =  d->background;
    auto borderColor = d->borderColor;
    auto margin = 2.0;
    auto shadowMargins = QMarginsF(margin, margin, margin, margin);

    // draw background
    auto backgroundRect = QRectF(rect()).marginsRemoved(shadowMargins);
    QPainterPath backgroundPath;
    backgroundPath.addRoundedRect(backgroundRect, radius, radius);
    painter.fillPath(backgroundPath, background);

    // draw border
    QPainterPath borderPath;
    QRectF borderRect = QRectF(rect());
    auto borderRadius = d->radius;//radius;
    QMarginsF borderMargin(penWidthf / 2, penWidthf / 2, penWidthf / 2, penWidthf / 2);
    if (outer) {
        borderRadius += penWidthf / 2;
        borderRect = borderRect.marginsAdded(borderMargin).marginsRemoved(shadowMargins);
    } else {
        borderRadius -= penWidthf / 2;
        borderRect = borderRect.marginsRemoved(borderMargin).marginsRemoved(shadowMargins);
    }
    borderPath.addRoundedRect(borderRect, borderRadius, borderRadius);
    QPen borderPen(borderColor);
    borderPen.setWidthF(penWidthf);
    painter.strokePath(borderPath, borderPen);
}

void DToast::setBorderColor(QColor borderColor)
{
    D_D(DToast);
    d->borderColor = borderColor;
}

void DToast::setOpacity(qreal opacity)
{
    D_D(DToast);
    d->effect->setOpacity(opacity);
    update();
}

void DToast::setBackground(QBrush background)
{
    D_D(DToast);
    d->background = background;
}

void DToast::setRadius(int radius)
{
    D_D(DToast);
    d->radius = radius;
}

void DToast::pop()
{
    Q_D(DToast);

    adjustSize();
    show();

    if (d->animation) {
        return;
    }

    d->animation = new QPropertyAnimation(this, "opacity");
    d->animation->setDuration(2000);
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

DToastPrivate::DToastPrivate(DToast *qq)
    : DObjectPrivate(qq)
{

}

void DToastPrivate::initUI()
{
    D_Q(DToast);
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
