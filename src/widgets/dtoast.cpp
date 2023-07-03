// SPDX-FileCopyrightText: 2016 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dtoast.h"

#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)

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

/*!
  \class Dtk::Widget::DToast
  \inmodule dtkwidget
  \brief 提供应用内通知的 Tooltip，类似安卓的 Toast.

  \image html DToast.gif
  \a parent
 */

DToast::DToast(QWidget *parent) :
    QFrame(parent), DObject(*new DToastPrivate(this))
{
    D_D(DToast);
    d->initUI();
}

DToast::~DToast()
{

}

/*!
  \brief 显示的内容
 */
QString DToast::text() const
{
    D_DC(DToast);
    return d->textLabel->text();
}

/*!
  \brief 显示的图标
 */
QIcon DToast::icon() const
{
    D_DC(DToast);
    return d->icon;
}

/*!
  \brief 显示时长
 */
int DToast::duration() const
{
    D_DC(DToast);
    return d->duration;
}

/*!
  \brief 不透明度
 */
qreal DToast::opacity() const
{
    D_DC(DToast);
    return d->effect->opacity();
}

/*!
  \brief 设置文本内容

  \a text 文本内容.
 */
void DToast::setText(QString text)
{
    D_D(DToast);
    d->textLabel->setVisible(true);
    d->textLabel->setText(text);
}

/*!
  \brief 设置图标
  \a iconfile 从资源中加载图标
 */
void DToast::setIcon(QString iconfile)
{
    D_D(DToast);
    d->icon = QIcon(iconfile);
    d->iconLabel->setVisible(true);
    d->iconLabel->setPixmap(DHiDPIHelper::loadNxPixmap(iconfile));
}

/*!
  \brief 设置图标
  \a icon QIcon 对象导入图标
  \a defaultSize 设置图标大小
 */
void DToast::setIcon(QIcon icon, QSize defaultSize)
{
    D_D(DToast);
    d->icon = icon;
    d->iconLabel->setVisible(true);
    d->iconLabel->setPixmap(d->icon.pixmap(icon.actualSize(defaultSize)));
}

/*!
  \brief 设置显示时长
  \a duration
 */
void DToast::setDuration(int duration)
{
    D_D(DToast);
    d->duration = duration;
}

/*!
  \brief 设置不透明度
  \a opacity
 */
void DToast::setOpacity(qreal opacity)
{
    D_D(DToast);
    d->effect->setOpacity(opacity);
    update();
}

/*!
  \brief 弹出
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
  \brief 隐藏并销毁
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

#endif
