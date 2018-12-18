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

/**
 * \~chinese \class DToast
 * \~chinese \brief 提供应用内通知的 Tooltip，类似安卓的 Toast。
 * \~chinese \image html DToast.gif
 * \~chinese \param parent
 */

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

/*!
 * \~chinese \brief 显示的内容
 */
QString DToast::text() const
{
    D_DC(DToast);
    return d->textLabel->text();
}

/*!
 * \~chinese \brief 显示的图标
 */
QIcon DToast::icon() const
{
    D_DC(DToast);
    return d->icon;
}

/*!
 * \~chinese \brief 显示时长
 */
int DToast::duration() const
{
    D_DC(DToast);
    return d->duration;
}

/*!
 * \~chinese \brief 不透明度
 */
qreal DToast::opacity() const
{
    D_DC(DToast);
    return d->effect->opacity();
}

/*!
 * \~chinese \brief 设置文本内容
 */
void DToast::setText(QString text)
{
    D_D(DToast);
    d->textLabel->setVisible(true);
    d->textLabel->setText(text);
}

/*!
 * \~chinese \brief 设置图标
 * \~chinese \param iconfile 从资源中加载图标
 */
void DToast::setIcon(QString iconfile)
{
    D_D(DToast);
    d->icon = QIcon(iconfile);
    d->iconLabel->setVisible(true);
    d->iconLabel->setPixmap(DHiDPIHelper::loadNxPixmap(iconfile));
}

/*!
 * \~chinese \brief 设置图标
 * \~chinese \param icon QIcon 对象导入图标
 * \~chinese \param defaultSize 设置图标大小
 */
void DToast::setIcon(QIcon icon, QSize defaultSize)
{
    D_D(DToast);
    d->icon = icon;
    d->iconLabel->setVisible(true);
    d->iconLabel->setPixmap(d->icon.pixmap(icon.actualSize(defaultSize)));
}

/*!
 * \~chinese \brief 设置显示时长
 * \~chinese \param duration
 */
void DToast::setDuration(int duration)
{
    D_D(DToast);
    d->duration = duration;
}

/*!
 * \~chinese \brief 设置不透明度
 * \~chinese \param opacity
 */
void DToast::setOpacity(qreal opacity)
{
    D_D(DToast);
    d->effect->setOpacity(opacity);
    update();
}

/*!
 * \~chinese \brief 弹出
 */
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

/*!
 * \~chinese \brief 隐藏并销毁
 */
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
