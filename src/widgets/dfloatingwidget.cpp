/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zhangwengeng <zhangwengeng_cm@deepin.com>
 *
 * Maintainer: zhangwengeng <zhangwengeng_cm@deepin.com>
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

#include "dfloatingwidget.h"
#include "private/dfloatingwidget_p.h"
#include "dstyleoption.h"
#include "dstyle.h"
#include "dblureffectwidget.h"

#include <QHBoxLayout>
#include <QEvent>
#include <QDebug>
#include <QPaintEvent>

DWIDGET_BEGIN_NAMESPACE

DFloatingWidgetPrivate::DFloatingWidgetPrivate(DFloatingWidget *qq)
    : DObjectPrivate(qq)
{
}

DFloatingWidgetPrivate::~DFloatingWidgetPrivate()
{
}

void DFloatingWidgetPrivate::init()
{
}

void DFloatingWidgetPrivate::adjustPalette()
{
    if (!layout || layout->count() == 0)
        return;

    QWidget *content = layout->itemAt(0)->widget();

    if (background) {
        D_Q(DFloatingWidget);
        // 开启模糊背景后应到调整调色板
        QPalette pa = q->palette();
        pa.setBrush(QPalette::Button, pa.brush(QPalette::Base));
        pa.setBrush(QPalette::Light, pa.brush(QPalette::Base));
        pa.setBrush(QPalette::Dark, pa.brush(QPalette::Base));

        content->setPalette(pa);
    } else {
        content->setPalette(QPalette());
    }
}

/*!
 * \~chinese \brief 设置 widget 对象, 若是之前此处已经有对象, 会先清理旧的对象, 再将此处的新对象放到此处
 * \~chinese \param[in] widget 将 widget 添加到 DFloatingWidget 的布局里面
 */
void DFloatingWidget::setWidget(QWidget *widget)
{
    D_D(DFloatingWidget);

    if (!d->layout) {
        d->layout = new QHBoxLayout(this);
        d->layout->setMargin(0);
    }

    for (int i = 0; i < d->layout->count(); ++i) {
        delete d->layout->takeAt(i);
    }

    d->layout->addWidget(widget);
    d->adjustPalette();
}

/*!
 * \~chinese \brief DFloatingWidget::setFramRadius 设置边框的圆角大小
 * \~chinese \param radius 圆角大小
 */
void DFloatingWidget::setFramRadius(int radius)
{
    D_D(DFloatingWidget);
    if (d->framRadius == radius)
        return;
    d->framRadius = radius;
    update();
}

/*!
 * \~chinese \brief 构造函数
 * \~chinese \param[in] dd 是 DFloatingWidgetPrivate 的对象引用
 * \~chinese \param[in] parent 其父对象
 */
DFloatingWidget::DFloatingWidget(DFloatingWidgetPrivate &dd, QWidget *parent)
    : QWidget(parent)
    , DObject(dd)
{
    D_D(DFloatingWidget);
    d->init();
    setAutoFillBackground(false);
}

/*!
 * \~chinese \brief 构造函数
 * \~chinese \param[in] parent 是本类的父控件
 */
DFloatingWidget::DFloatingWidget(QWidget *parent)
    : DFloatingWidget(*new DFloatingWidgetPrivate(this), parent)
{

}

QSize DFloatingWidget::sizeHint() const
{
    D_DC(DFloatingWidget);

    QSize size = d->layout ? d->layout->sizeHint() : QSize(0, 0);
    DStyleHelper dstyle(style());
    DStyleOptionFloatingWidget opt;
    opt.frameRadius = d_func()->framRadius;
    size = dstyle.sizeFromContents(DStyle::CT_FloatingWidget, &opt, size, this);

    return size;
}

void DFloatingWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    // 更新模糊背景的源图片，在此处更新是为了避免阴影部分会进入到模糊控件的源图片
    D_D(DFloatingWidget);

    if (d->background) {
        d->background->updateBlurSourceImage(e->region().translated(-d->background->pos()) & d->background->rect());
    }

    DStylePainter painter(this);
    DStyleOptionFloatingWidget opt;
    initStyleOption(&opt);
    painter.drawControl(DStyle::CE_FloatingWidget, opt);
}

/*!
 * \~chinese \brief 主事件处理程序, 用来处理感兴趣的事件
 * \~chinese \param[in] event`父控件或者外设传入进来的 event 事件消息
 * \~chinese \return 事件消息是否被处理的结果
 */
bool DFloatingWidget::event(QEvent *event)
{
    D_D(DFloatingWidget);

    switch (event->type()) {
    case QEvent::Polish: {
        DStyleHelper dstyle(style());
        int margins = dstyle.pixelMetric(DStyle::PM_FloatingWidgetShadowMargins, nullptr, this);
        setContentsMargins(margins, margins, margins, margins);

        if (d->background) {
            int radius = DStyleHelper(style()).pixelMetric(DStyle::PM_TopLevelWindowRadius);

            d->background->setBlurRectXRadius(radius);
            d->background->setBlurRectYRadius(radius);
            d->background->setGeometry(contentsRect());

            d->adjustPalette();
        }
        break;
    }
    case QEvent::PaletteChange: {
        d->adjustPalette();
        break;
    }
    case QEvent::Resize: {
        if (d->background) {
            d->background->setGeometry(contentsRect());
        }
        break;
    }
    default:
        break;
    }

    return QWidget::event(event);
}

/*!
 * \~chinese \brief 初始化风格属性
 * \~chinese \param[in] option 自带一些风格属性传入, 其上面增加和删除一些其他属性
 */
void DFloatingWidget::initStyleOption(DStyleOptionFloatingWidget *option) const
{
    option->init(this);
    option->initFrom(this);
    option->noBackground = blurBackgroundIsEnabled();
    option->frameRadius = d_func()->framRadius;
}

/*!
 * \~chinese \brief 获取背景模糊状态
 * \~chinese \return 返回背景模糊状态
 */
bool DFloatingWidget::blurBackgroundIsEnabled() const
{
    D_DC(DFloatingWidget);

    return d->background;
}

/*!
 * \~chinese \brief 获取模糊背景功能的对象
 * \~chinese \return 返回模糊背景功能的对象
 */
DBlurEffectWidget *DFloatingWidget::blurBackground() const
{
    D_DC(DFloatingWidget);

    return d->background;
}

/*!
 * \~chinese \brief 设置模糊背景启动
 * \~chinese \param[in] blurBackgroundEnabled 是否启动模糊背景效果
 */
void DFloatingWidget::setBlurBackgroundEnabled(bool blurBackgroundEnabled)
{
    D_D(DFloatingWidget);

    if (bool(d->background) == blurBackgroundEnabled)
        return;

    if (blurBackgroundEnabled) {
        d->background = new DBlurEffectWidget(this);
        d->background->setGeometry(contentsRect());
        d->background->lower();
        d->background->setFocusPolicy(Qt::NoFocus);
        d->background->setAttribute(Qt::WA_TransparentForMouseEvents);
        d->background->show();

        int radius = DStyleHelper(style()).pixelMetric(DStyle::PM_TopLevelWindowRadius);

        d->background->setBlurRectXRadius(radius);
        d->background->setBlurRectYRadius(radius);
        d->background->setBlendMode(DBlurEffectWidget::InWidgetBlend);

        connect(d->background, &DBlurEffectWidget::blurSourceImageDirtied,
                this, static_cast<void(DFloatingWidget::*)()>(&DFloatingWidget::update));
    } else {
        d->background->hide();
        d->background->deleteLater();
    }

    d->adjustPalette();
}

DWIDGET_END_NAMESPACE
