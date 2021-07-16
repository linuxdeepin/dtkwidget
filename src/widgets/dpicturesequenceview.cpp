/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "dpicturesequenceview.h"
#include "private/dpicturesequenceview_p.h"

#include <QGraphicsPixmapItem>
#include <QImageReader>
#include <QIcon>

DWIDGET_BEGIN_NAMESPACE

DPictureSequenceViewPrivate::DPictureSequenceViewPrivate(DPictureSequenceView *q) :
    DObjectPrivate(q)
{

}

DPictureSequenceViewPrivate::~DPictureSequenceViewPrivate()
{
    for (auto *item : pictureItemList)
    {
        scene->removeItem(item);
        delete item;
    }

    scene->deleteLater();
}

void DPictureSequenceViewPrivate::init()
{
    D_Q(DPictureSequenceView);

    scene = new QGraphicsScene(q);
    refreshTimer = new QTimer(q);
    refreshTimer->setInterval(33);

    q->setScene(scene);
    q->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    q->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    q->setFrameShape(QFrame::NoFrame);

    q->connect(refreshTimer, SIGNAL(timeout()), q, SLOT(_q_refreshPicture()));
    q->viewport()->setAccessibleName("DPictureSequenceViewport");
}

void DPictureSequenceViewPrivate::play()
{
    refreshTimer->start();
}

QPixmap DPictureSequenceViewPrivate::loadPixmap(const QString &path)
{
    D_Q(DPictureSequenceView);

    qreal ratio = 1.0;

    const qreal devicePixelRatio = q->devicePixelRatioF();

    QPixmap pixmap;

    if (!qFuzzyCompare(ratio, devicePixelRatio)) {
        QImageReader reader;
        reader.setFileName(qt_findAtNxFile(path, devicePixelRatio, &ratio));
        if (reader.canRead()) {
            reader.setScaledSize(reader.size() * (devicePixelRatio / ratio));
            pixmap = QPixmap::fromImage(reader.read());
            pixmap.setDevicePixelRatio(devicePixelRatio);
        }
    } else {
        pixmap.load(path);
    }

    return pixmap;
}

void DPictureSequenceViewPrivate::_q_refreshPicture()
{
    QGraphicsPixmapItem *item = pictureItemList.value(lastItemPos++);

    if (item)
        item->hide();

    if (lastItemPos == pictureItemList.count()) {
        lastItemPos = 0;

        if (singleShot)
            refreshTimer->stop();

        D_QC(DPictureSequenceView);

        Q_EMIT q->playEnd();
    }

    item = pictureItemList.value(lastItemPos);

    if (item)
        item->show();
}

/*!
  \class Dtk::Widget::DPictureSequenceView
  \inmodule dtkwidget

  \brief DPictureSequenceView draw a serial of picture as movie. It trigger picture update by an timer.
  \brief 将图片序列绘制为动画，通过定时器刷新来实现动画效果.
 */

/*!
  \property DPictureSequenceView::singleShot

  \brief 控制动画是否只播放一次。
  \brief Animation is just refresh one time.
 */

/*!
  \property DPictureSequenceView::speed

  \brief 动画更新时间间隔，单位为毫秒(ms)。
  \brief Update interval of refresh timer by ms.
 */

DPictureSequenceView::DPictureSequenceView(QWidget *parent) :
    QGraphicsView(parent),
    DObject(*new DPictureSequenceViewPrivate(this))
{
    D_D(DPictureSequenceView);

    d->init();
}

/*!
  \brief 通过一个URI模板来设置图片序列.
  \brief Set picture source list by a uri template an range.

  \a srcFormat 图片源模板，例如":/images/Spinner/Spinner%1.png"。
  \a srcFormat is the source uri template, just like ":/images/Spinner/Spinner%1.png".
  \a range 图片的序号范围，需要为一系列整数。
  \a range for build source uris, it make an sequence of number.
  \a fieldWidth 图片的序号转化为字符串时的宽度，通过‘0’来填充.
  \a fieldWidth string width when convert number to string, fill "0" if needed.
  \a autoScale auto resize source image to widget size, default to false.
  \a autoScale 是否自动缩放图片，默认不缩放。
 */
void DPictureSequenceView::setPictureSequence(const QString &srcFormat, const QPair<int, int> &range, const int fieldWidth, const bool autoScale)
{
    QStringList pics;

    for (int i(range.first); i != range.second; ++i)
        pics << srcFormat.arg(i, fieldWidth, 10, QChar('0'));

    setPictureSequence(pics, autoScale);
}

/*!
  \brief 通过URI列表来设置图片序列。
  \brief Set picture source list by a QStringList.

  \a sequence 图片源路径。
  \a sequence url list
  \a autoScale 是否自动缩放图片，默认不缩放。
  \a autoScale auto resize source image to widget size, default to false.
 */
void DPictureSequenceView::setPictureSequence(const QStringList &sequence, const bool autoScale)
{
    D_D(DPictureSequenceView);

    QList<QPixmap> pixmapSequence;
    for (const QString &path : sequence) {
        pixmapSequence << d->loadPixmap(path);
    }

    setPictureSequence(pixmapSequence, autoScale);
}

/*!
  \brief 通过位图数据序列来初始化图片序列.
  \brief Set picture source with pixmap array.

  \a sequence 位图数据序列。
  \a sequence image data list.
  \a autoScale 是否自动缩放图片，默认不缩放。
  \a autoScale auto resize source image to widget size, default to false.
 */
void DPictureSequenceView::setPictureSequence(const QList<QPixmap> &sequence, const bool autoScale)
{
    D_D(DPictureSequenceView);

    stop();
    d->scene->clear();
    d->pictureItemList.clear();

    for (QPixmap pixmap : sequence) {
        if (autoScale) {
            pixmap = pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }

        d->pictureItemList << d->scene->addPixmap(pixmap);
        d->pictureItemList.last()->hide();
    }

    if (!d->pictureItemList.isEmpty()) {
        d->pictureItemList.first()->show();
    }

    setStyleSheet("background-color:transparent;");
}

/*!
  \brief 开始/恢复播放.
  \brief Start/resume update timer and show animation.
 */
void DPictureSequenceView::play()
{
    D_D(DPictureSequenceView);

    d->play();
}

/*!
  \brief 暂停播放并停止在当前图片上.
  \brief Pause animation and stay on current picture.
 */
void DPictureSequenceView::pause()
{
    D_D(DPictureSequenceView);

    d->refreshTimer->stop();
}

/*!
  \brief 暂停播放并停止在初始图片上.
  \brief Stop animation and rest to first picture.
 */
void DPictureSequenceView::stop()
{
    D_D(DPictureSequenceView);

    d->refreshTimer->stop();
    if (d->pictureItemList.count() > d->lastItemPos)
        d->pictureItemList[d->lastItemPos]->hide();
    if (!d->pictureItemList.isEmpty())
        d->pictureItemList[0]->show();
    d->lastItemPos = 0;
}

int DPictureSequenceView::speed() const
{
    D_DC(DPictureSequenceView);

    return d->refreshTimer->interval();
}

void DPictureSequenceView::setSpeed(int speed)
{
    D_D(DPictureSequenceView);

    d->refreshTimer->setInterval(speed);
}

bool DPictureSequenceView::singleShot() const
{
    D_DC(DPictureSequenceView);

    return d->singleShot;
}

void DPictureSequenceView::setSingleShot(bool singleShot)
{
    D_D(DPictureSequenceView);

    d->singleShot = singleShot;
}

DWIDGET_END_NAMESPACE

#include "moc_dpicturesequenceview.cpp"
