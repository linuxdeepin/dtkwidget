// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dgraphicsclipeffect.h"
#include <DObjectPrivate>

#include <QPainter>
#include <QPainterPath>
#include <QDebug>

DWIDGET_BEGIN_NAMESPACE

class DGraphicsClipEffectPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    DGraphicsClipEffectPrivate(DGraphicsClipEffect *qq);

    QPainterPath clipPath;
    QMargins margins;

    Q_DECLARE_PUBLIC(DGraphicsClipEffect)
};

DGraphicsClipEffectPrivate::DGraphicsClipEffectPrivate(DGraphicsClipEffect *qq)
    : DObjectPrivate(qq)
{

}

/*!
  \class Dtk::Widget::DGraphicsClipEffect
  \inmodule dtkwidget
  \brief 用于裁剪窗口的绘制内容.
  
  支持使用 QPainterPath 设置一个区域，位于区域外的窗口内容将被裁剪,可以通过设置
  margins 控制有效的裁剪区域。如下图所示：
  \raw HTML
  <pre style="font-family: FreeMono, Consolas, Menlo, 'Noto Mono', 'Courier New', Courier, monospace;line-height: 100%;">
  ┏━━━━━━━━━━━━━━┯━━━━━━━━━━━━━━┓
  ┃              │              ┃
  ┃              │              ┃
  ┃     A        m              ┃
  ┃              │              ┃
  ┃              │              ┃
  ┃       ┏┅┅┅┅┅┅┷┅┅┅┅┅┅┓       ┃
  ┃       ┋B ╭───────╮  ┋       ┃
  ┃       ┋  │       │  ┋       ┃
  ┃── m ──┋  │   C   │  ┋── m ──┃
  ┃       ┋  │       │  ┋       ┃
  ┃       ┋  ╰───────╯  ┋       ┃
  ┃       ┗┅┅┅┅┅┅┯┅┅┅┅┅┅┛       ┃
  ┃              │              ┃
  ┃              │              ┃
  ┃              m              ┃
  ┃              │              ┃
  ┃              │              ┃
  ┗━━━━━━━━━━━━━━┷━━━━━━━━━━━━━━┛
  </pre>
  \endraw
  A为被作用的控件区域，B为有效的被裁剪区域，C 为 clipPath 区域，m为
  margins ，则被裁剪掉的区域为： B - C，裁掉的部分将显示
  下层控件内容，如果下层没有其它控件，将显示主窗口背景。
  
  \warning 以这种方式实现对 QWidget 的剪切在性能上会有较大的影响，一般情况下，我们推荐使用 DClipEffectWidget
  \sa DClipEffectWidget QWidget::setGraphicsEffect
 */

/*!
  \fn void DGraphicsClipEffect::marginsChanged(QMargins margins)
  这个信号在 \a margins 改变时被发送
  */

/*!
  \fn void DGraphicsClipEffect::clipPathChanged(QPainterPath clipPath)
  这个信号在 \a clipPath 改变时被发送
  */

/*!
  \brief 构造 DGraphicsClipEffect 对象，和普通 QObject 一样，可以传入一个 QObject 对象
  指针作为其父对象
  \a parent 父对象
 */
DGraphicsClipEffect::DGraphicsClipEffect(QObject *parent)
    : QGraphicsEffect(parent)
    , DObject(*new DGraphicsClipEffectPrivate(this))
{

}

/*!
  \property DGraphicsClipEffect::margins
  \brief 用于调整目标控件的被裁剪区域，可以和 clipPath 组合更加灵活的裁剪控件
 */
QMargins DGraphicsClipEffect::margins() const
{
    D_DC(DGraphicsClipEffect);

    return d->margins;
}

/*!
  \property DGraphicsClipEffect::clipPath
  \brief 被裁剪的区域，控件真实裁剪区域 = clipPath & (boundingRect() - margins)
  \warning clipPath 必须是一个封闭的环状路径
  \sa QPainterPath::closeSubpath
 */
QPainterPath DGraphicsClipEffect::clipPath() const
{
    D_DC(DGraphicsClipEffect);

    return d->clipPath;
}

/*!
  \a margins
 */
void DGraphicsClipEffect::setMargins(const QMargins &margins)
{
    D_D(DGraphicsClipEffect);

    if (d->margins == margins)
        return;

    d->margins = margins;
    Q_EMIT marginsChanged(margins);
}

/*!
  \a clipPath
 */
void DGraphicsClipEffect::setClipPath(const QPainterPath &clipPath)
{
    D_D(DGraphicsClipEffect);

    if (d->clipPath == clipPath)
        return;

    d->clipPath = clipPath;
    Q_EMIT clipPathChanged(clipPath);
}

void DGraphicsClipEffect::draw(QPainter *painter)
{
    if (!painter->isActive()) {
        qWarning() << "DGraphicsClipEffect::draw: The painter is not active!";

        drawSource(painter);
        return;
    }

    D_D(DGraphicsClipEffect);

    QPoint offset;
    Qt::CoordinateSystem system = sourceIsPixmap() ? Qt::LogicalCoordinates : Qt::DeviceCoordinates;
    QPixmap pixmap = sourcePixmap(system, &offset, QGraphicsEffect::PadToEffectiveBoundingRect);

    if (pixmap.isNull())
        return;

    QPainter pixmapPainter(&pixmap);
    QPainterPath newPath;

    newPath.addRect(pixmap.rect().marginsRemoved(d->margins));
    newPath -= d->clipPath;
    pixmapPainter.setRenderHints(painter->renderHints() | QPainter::Antialiasing);
    pixmapPainter.setCompositionMode(QPainter::CompositionMode_Clear);
    pixmapPainter.fillPath(newPath, Qt::transparent);

    painter->save();

    if (system == Qt::DeviceCoordinates)
        painter->setWorldTransform(QTransform());

    painter->drawPixmap(offset, pixmap);
    painter->restore();
}

DWIDGET_END_NAMESPACE
