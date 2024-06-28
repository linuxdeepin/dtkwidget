// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
  \brief 设置 widget 对象, 若是之前此处已经有对象, 会先清理旧的对象, 再将此处的新对象放到此处
  \param[in] widget 将 widget 添加到 DFloatingWidget 的布局里面
 */
void DFloatingWidget::setWidget(QWidget *widget)
{
    D_D(DFloatingWidget);

    if (!d->layout) {
        d->layout = new QHBoxLayout(this);
        d->layout->setContentsMargins(0, 0, 0, 0);
    }

    for (int i = 0; i < d->layout->count(); ++i) {
        delete d->layout->takeAt(i);
    }

    d->layout->addWidget(widget);
    d->adjustPalette();
}

/*!
  \brief DFloatingWidget::setFramRadius 设置边框的圆角大小
  \a radius 圆角大小
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
  \brief 构造函数
  \param[in] dd 是 DFloatingWidgetPrivate 的对象引用
  \param[in] parent 其父对象
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
  \brief 构造函数
  \param[in] parent 是本类的父控件
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
  \brief 主事件处理程序, 用来处理感兴趣的事件
  \param[in] event`父控件或者外设传入进来的 event 事件消息
  \return 事件消息是否被处理的结果
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
            // 背景圆角使用控件的圆角，如果控件圆角未设置则使用默认值
            int radius = d->framRadius == -1 ? DStyleHelper(style()).pixelMetric(DStyle::PM_TopLevelWindowRadius)
                                             : d->framRadius;

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
  \brief 初始化风格属性
  \param[in] option 自带一些风格属性传入, 其上面增加和删除一些其他属性
 */
void DFloatingWidget::initStyleOption(DStyleOptionFloatingWidget *option) const
{
    option->init(this);
    option->initFrom(this);
    option->noBackground = blurBackgroundIsEnabled();
    option->frameRadius = d_func()->framRadius;
}

/*!
  \brief 获取背景模糊状态
  \return 返回背景模糊状态
 */
bool DFloatingWidget::blurBackgroundIsEnabled() const
{
    D_DC(DFloatingWidget);

    return d->background;
}

/*!
  \brief 获取模糊背景功能的对象
  \return 返回模糊背景功能的对象
 */
DBlurEffectWidget *DFloatingWidget::blurBackground() const
{
    D_DC(DFloatingWidget);

    return d->background;
}

/*!
  \brief 设置模糊背景启动
  \param[in] blurBackgroundEnabled 是否启动模糊背景效果
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

        // 背景圆角使用控件的圆角，如果控件圆角未设置则使用默认值
        int radius = d->framRadius == -1 ? DStyleHelper(style()).pixelMetric(DStyle::PM_TopLevelWindowRadius)
                                         : d->framRadius;

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
