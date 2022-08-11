// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dclipeffectwidget.h"
#include <DObjectPrivate>

#include <QEvent>
#include <QBackingStore>
#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>
#include <QDebug>

#include <qpa/qplatformbackingstore.h>

DWIDGET_BEGIN_NAMESPACE

class DClipEffectWidgetPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    DClipEffectWidgetPrivate(DClipEffectWidget *qq);

    void updateImage();

    QImage image;
    QRectF imageGeometry;
    QPainterPath path;
    QMargins margins;

    QWidgetList parentList;

    Q_DECLARE_PUBLIC(DClipEffectWidget)
};

DClipEffectWidgetPrivate::DClipEffectWidgetPrivate(DClipEffectWidget *qq)
    : DObjectPrivate(qq)
{

}

/*!
  \class Dtk::Widget::DClipEffectWidget
  \inmodule dtkwidget
  \brief 用于裁剪窗口的绘制内容.
  
  支持使用 QPainterPath 设置一个区域，位于区域外的窗口内容将被裁剪。被裁剪的对象
  是此控件的父控件, 且 DClipEffectWidget 会自动填充整个父控件，另外可以通过设置
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
  A为父控件区域， DClipEffectWidget 控件区域和父控件相同，B为有效的被裁剪区域，C
  为 clipPath 区域，m为 margins ，则被裁剪掉的区域为： B - C，裁掉的部分将显示
  下层控件内容，如果下层没有其它控件，将显示主窗口背景。
  
  此控件不接收任何输入事件，且不接受焦点
  \sa Qt::WA_TransparentForMouseEvents Qt::NoFocus
  \sa DGraphicsClipEffect
 */

/*!
  \fn void DClipEffectWidget::marginsChanged(QMargins margins)
  这个信号在 \a margins 改变时被发送
  */

/*!
  \fn void DClipEffectWidget::clipPathChanged(QPainterPath clipPath)
  这个信号在 \a clipPath 改变时被发送
  */

/*!
  \brief 构造 DClipEffectWidget 对象，和普通 QWidget 一样，可以传入一个 QWidget 对象
  指针作为其父对象
  \a parent 父对象
  \warning 必须要传入一个有效的父对象，将此控件作为顶级窗口没有任何意义
 */
DClipEffectWidget::DClipEffectWidget(QWidget *parent)
    : QWidget(parent)
    , DObject(*new DClipEffectWidgetPrivate(this))
{
    Q_ASSERT(parent);

    setAttribute(Qt::WA_TransparentForMouseEvents);
    setFocusPolicy(Qt::NoFocus);
}

/*!
  \property DClipEffectWidget::margins
  \brief 用于调整控件的被裁剪区域，可以和 clipPath 组合更加灵活的裁剪控件
 */
QMargins DClipEffectWidget::margins() const
{
    D_DC(DClipEffectWidget);

    return d->margins;
}

/*!
  \property DClipEffectWidget::clipPath
  \brief 被裁剪的区域，控件真实裁剪区域 = clipPath & (rect() - margins)
  \warning clipPath 必须是一个封闭的环状路径
  \sa QPainterPath::closeSubpath
 */
QPainterPath DClipEffectWidget::clipPath() const
{
    D_DC(DClipEffectWidget);

    return d->path;
}

/*!
  \a margins
 */
void DClipEffectWidget::setMargins(QMargins margins)
{
    D_D(DClipEffectWidget);

    if (d->margins == margins)
        return;

    d->margins = margins;
    update();

    Q_EMIT marginsChanged(margins);
}

/*!
  \a path
 */
void DClipEffectWidget::setClipPath(const QPainterPath &path)
{
    D_D(DClipEffectWidget);

    if (d->path == path)
        return;

    d->path = path;
    d->image = QImage();

    Q_EMIT clipPathChanged(d->path);

    update();
}

inline QRectF multiply(const QRectF &rect, qreal scale)
{
    return QRectF(rect.topLeft() * scale, rect.size() * scale);
}

inline QRectF divide(const QRectF &rect, qreal scale)
{
    return multiply(rect, 1.0 / scale);
}

bool DClipEffectWidget::eventFilter(QObject *watched, QEvent *event)
{
    D_D(DClipEffectWidget);

    if (event->type() == QEvent::Move) {
        d->image = QImage();
    }

    if (watched != parent())
        return false;

    if (event->type() == QEvent::Paint) {
        const QPoint &offset = mapTo(window(), QPoint(0, 0));
        const QImage &image = window()->backingStore()->handle()->toImage();
        qreal scale = devicePixelRatioF();

        d->imageGeometry = QRectF(image.rect()) & multiply(QRect(offset, size()), scale);

        if (d->image.isNull() || d->imageGeometry.size() != d->image.size()) {
            d->image = image.copy(d->imageGeometry.toRect());
            d->image.setDevicePixelRatio(scale);
        } else {
            QPaintEvent *e = static_cast<QPaintEvent*>(event);
            QPainter p;
            // 此控件位置一直为 0,0，且大小和父控件一致，所以offset也是父控件相对于顶级窗口的偏移
            const QRectF &rect = QRectF(image.rect()) & multiply(e->rect().translated(offset), scale);

            d->image.setDevicePixelRatio(image.devicePixelRatio());

            p.begin(&d->image);
            p.setCompositionMode(QPainter::CompositionMode_Source);
            p.drawImage(rect.topLeft() - d->imageGeometry.topLeft(), image.copy(rect.toRect()));
            p.end();

            d->image.setDevicePixelRatio(scale);
        }
    } else if (event->type() == QEvent::Resize) {
        resize(parentWidget()->size());
    }

    return false;
}

void DClipEffectWidget::paintEvent(QPaintEvent *event)
{
    D_DC(DClipEffectWidget);

    if (d->image.isNull())
        return;

    qreal devicePixelRatio = devicePixelRatioF();
    const QRectF &rect = QRectF(event->rect()) & QRectF(this->rect()).marginsRemoved(d->margins);
    const QPoint &offset = mapTo(window(), QPoint(0, 0));
    const QRectF &imageRect = multiply(rect, devicePixelRatio) & d->imageGeometry.translated(-offset * devicePixelRatio);

    if (!imageRect.isValid())
        return;

    QPainter p(this);
    QPainterPath newPath;

    newPath.addRect(this->rect());
    newPath -= d->path;

    p.setRenderHint(QPainter::Antialiasing);
    p.setClipPath(newPath);
    p.setCompositionMode(QPainter::CompositionMode_Source);
    p.drawImage(imageRect.topLeft() / devicePixelRatio, d->image, imageRect);
}

void DClipEffectWidget::resizeEvent(QResizeEvent *event)
{
    D_D(DClipEffectWidget);

    d->image = QImage();

    QWidget::resizeEvent(event);
}

void DClipEffectWidget::showEvent(QShowEvent *event)
{
    D_D(DClipEffectWidget);

    d->parentList.clear();

    QWidget *pw = parentWidget();

    while (pw && !pw->isTopLevel()) {
        d->parentList << pw;

        pw->installEventFilter(this);
        pw = pw->parentWidget();
    }

    resize(parentWidget()->size());

    QWidget::showEvent(event);
}

void DClipEffectWidget::hideEvent(QHideEvent *event)
{
    D_D(DClipEffectWidget);

    for (QWidget *w : d->parentList)
        w->removeEventFilter(this);

    d->parentList.clear();

    QWidget::hideEvent(event);
}

DWIDGET_END_NAMESPACE
