// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dspinner.h"

#include <QtMath>
#include <QPainter>
#include <QPainterPath>
#include <QTimer>
#include <QEvent>

#include <DObjectPrivate>

DWIDGET_BEGIN_NAMESPACE

class DSpinnerPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    explicit DSpinnerPrivate(DSpinner *qq);

    QList<QColor> createDefaultIndicatorColorList(QColor color);

    QTimer refreshTimer;

    double indicatorShadowOffset = 10;
    double currentDegree = 0.0;

    QList<QList<QColor>> indicatorColors;
    QPalette::ColorGroup colorGroup;

    D_DECLARE_PUBLIC(DSpinner)
};

DSpinnerPrivate::DSpinnerPrivate(DSpinner *qq)
    : DObjectPrivate(qq)
{

}

/*!
    \class Dtk::Widget::DSpinner
    \inmodule dtkwidget

    \brief 可以使用 DSpinner 类快速创建用于指示加载中状态的旋转等待图标动画控件.

    DSpinner 提供了一个用于指示加载中状态的旋转等待图标动画控件。在创建图标后，使用 start() 即可开始图标旋转的动画。
    \brief Use DSpinner to create a widget with spinning animation for display a waiting state.
    DSpinner provide a spinning animation widget to indicate a waiting state. Call start() to start
    the spinning animation.

    \image html DSpinner.gif
*/

/*!
  \brief Create a DSpinner widget
  \brief 创建一个 DSpinner 控件
  
  \a parent Parent widget.
  \a parent 父控件
 */
DSpinner::DSpinner(QWidget *parent) :
    QWidget(parent), DObject(*new DSpinnerPrivate(this))
{
    Q_D(DSpinner);

    d->refreshTimer.setInterval(30);
    d->colorGroup = palette().currentColorGroup();

    connect(&d->refreshTimer, &QTimer::timeout,
    this, [ = ]() {
        d->currentDegree += 14;
        update();
    });
}

DSpinner::~DSpinner()
{

}

/*!
  \brief Is the DSpinner spinning or not.
  \brief DSpinner 是否正在播放旋转动画.

  \return 正在播放返回 true，否则返回 false.
 */
bool DSpinner::isPlaying() const
{
    Q_D(const DSpinner);
    return d->refreshTimer.isActive();
}

/*!
  \brief Start spinning
  \brief 开始旋转动画
 */
void DSpinner::start()
{
    Q_D(DSpinner);
    d->refreshTimer.start();
}

/*!
  \brief Stop spinning
  \brief 停止旋转动画
 */
void DSpinner::stop()
{
    Q_D(DSpinner);
    d->refreshTimer.stop();
}

/*!
  \brief Set background \a color
  \brief 设置背景颜色
 */
void DSpinner::setBackgroundColor(QColor color)
{
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, color);
    setPalette(pal);
}

void DSpinner::paintEvent(QPaintEvent *)
{
    Q_D(DSpinner);

    if (d->colorGroup != palette().currentColorGroup()) {
        d->colorGroup = palette().currentColorGroup();
        d->indicatorColors.clear();
    }

    if (d->indicatorColors.isEmpty()) {
        for (int i = 0; i < 3; ++i)
            d->indicatorColors << d->createDefaultIndicatorColorList(palette().highlight().color());
    }

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    auto degreeCurrent = d->currentDegree * 1.0;

    auto center = QRectF(rect()).center();
    auto radius = qMin(rect().width(), rect().height()) / 2.0;
    auto indicatorRadius = radius / 2 / 2 * 1.1;
    auto indicatorDegreeDelta = 360 / d->indicatorColors.count();

    for (int i = 0; i <  d->indicatorColors.count(); ++i) {
        auto colors = d->indicatorColors.value(i);
        for (int j = 0; j < colors.count(); ++j) {
            degreeCurrent = d->currentDegree - j * d->indicatorShadowOffset + indicatorDegreeDelta * i;
            auto x = (radius - indicatorRadius) * qCos(qDegreesToRadians(degreeCurrent));
            auto y = (radius - indicatorRadius) * qSin(qDegreesToRadians(degreeCurrent));

            x = center.x() + x;
            y = center.y() + y;
            auto tl = QPointF(x - 1 * indicatorRadius, y - 1 * indicatorRadius);
            QRectF rf(tl.x(), tl.y(), indicatorRadius * 2, indicatorRadius * 2);

            QPainterPath path;
            path.addEllipse(rf);

            painter.fillPath(path, colors.value(j));
        }
    }
}

void DSpinner::changeEvent(QEvent *e)
{
    Q_D(DSpinner);

    if (e->type() == QEvent::PaletteChange)
        d->indicatorColors.clear();

    QWidget::changeEvent(e);
}

QList<QColor> DSpinnerPrivate::createDefaultIndicatorColorList(QColor color)
{
    QList<QColor> colors;
    QList<int> opacitys;
    opacitys << 100 << 30 << 15 << 10 << 5 << 4 << 3 << 2 << 1;
    for (int i = 0; i < opacitys.count(); ++i) {
        color.setAlpha(255 * opacitys.value(i) / 100);
        colors << color;
    }
    return colors;
}

DWIDGET_END_NAMESPACE
