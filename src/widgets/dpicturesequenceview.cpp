/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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
 * \class DPictureSequenceView
 * \brief DPictureSequenceView draw a serial of picture as movie. It trigger picture update by an timer.
 */

/*!
 * \property DPictureSequenceView::singleShot
 * \brief Animation is just refresh one time.
 */

/*!
 * \property DPictureSequenceView::speed
 * \brief Update interval of refresh timer by ms.
 */

/*!
 * \brief DPictureSequenceView::DPictureSequenceView Constructs a new DPictureSequenceView instance having the specified parent.
 * \param parent
 */
DPictureSequenceView::DPictureSequenceView(QWidget *parent) :
    QGraphicsView(parent),
    DObject(*new DPictureSequenceViewPrivate(this))
{
    D_D(DPictureSequenceView);

    d->init();
}

/*!
 * \brief DPictureSequenceView::setPictureSequence set picture source list by a uri template an range.
 * \param srcFormat is the source uri template, just like ":/images/Spinner/Spinner%1.png".
 * \param range for build source uris, it make an sequence of number.
 * \param fieldWidth string width when convert number to string, fill "0" if needed.
 * \param autoScale auto resize source image to widget size, default to false.
 */
void DPictureSequenceView::setPictureSequence(const QString &srcFormat, const QPair<int, int> &range, const int fieldWidth, const bool autoScale)
{
    QStringList pics;

    for (int i(range.first); i != range.second; ++i)
        pics << srcFormat.arg(i, fieldWidth, 10, QChar('0'));

    setPictureSequence(pics, autoScale);
}

/*!
 * \brief DPictureSequenceView::setPictureSequence set picture source list by a QStringList.
 * \param sequence url list
 * \param autoScale auto resize source image to widget size, default to false.
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
 * \brief DPictureSequenceView::setPictureSequence
 * \param sequence image data list.
 * \param autoScale auto resize source image to widget size, default to false.
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
 * \brief DPictureSequenceView::play start/resume update timer and show animation.
 */
void DPictureSequenceView::play()
{
    D_D(DPictureSequenceView);

    d->play();
}

/*!
 * \brief DPictureSequenceView::pause pause animation and stay on current picture.
 */
void DPictureSequenceView::pause()
{
    D_D(DPictureSequenceView);

    d->refreshTimer->stop();
}

/*!
 * \brief DPictureSequenceView::stop stop animation and rest to first picture.
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
