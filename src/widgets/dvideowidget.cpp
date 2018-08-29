/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#include "dvideowidget.h"
#include <DObjectPrivate>

#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>
#include <QCamera>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QPainter>
#include <QPointer>

DWIDGET_BEGIN_NAMESPACE

class VideoFormatProxy : public QAbstractVideoSurface
{
    Q_OBJECT

public:
    VideoFormatProxy(QObject *parent);
    QVideoFrame& currentFrame() const;
protected:
    bool present(const QVideoFrame &frame);
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
                QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;
    bool isFormatSupported(const QVideoSurfaceFormat &format) const;
private:
    QVideoFrame m_currentFrame;
    QVideoFrame m_lastFrame;

    friend class DVideoWidget;

Q_SIGNALS:
    void currentFrameChanged();
};

VideoFormatProxy::VideoFormatProxy(QObject *parent):
    QAbstractVideoSurface(parent)
{
}

QVideoFrame& VideoFormatProxy::currentFrame() const
{
    return const_cast<QVideoFrame&>(m_currentFrame);
}

bool VideoFormatProxy::present(const QVideoFrame &frame)
{
    m_currentFrame = frame;

    if (frame.isValid())
        m_lastFrame = frame;

    Q_EMIT currentFrameChanged();
    return true;
}

QList<QVideoFrame::PixelFormat> VideoFormatProxy::supportedPixelFormats(QAbstractVideoBuffer::HandleType) const
{
    return QList<QVideoFrame::PixelFormat>()
                     << QVideoFrame::Format_RGB32
                     << QVideoFrame::Format_ARGB32
                     << QVideoFrame::Format_ARGB32_Premultiplied
                     << QVideoFrame::Format_RGB565
                     << QVideoFrame::Format_RGB555;
}

bool VideoFormatProxy::isFormatSupported(const QVideoSurfaceFormat &format) const
{
    return QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat()) != QImage::Format_Invalid;
}

class DVideoWidgetPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    DVideoWidgetPrivate(DVideoWidget *qq);

    VideoFormatProxy *formatProxy;
    QPointer<QMediaPlayer> player;

    bool mirroredHorizontal = false;
    bool mirroredVertical = false;
    qreal scale = 1;
    Qt::AspectRatioMode aspectRatioMode = Qt::KeepAspectRatio;
    int brightness = 0;
    int contrast = 0;
    int hue = 0;
    int saturation = 0;
    bool round = false;
    qreal ratio = 1;

    D_DECLARE_PUBLIC(DVideoWidget)
};

DVideoWidgetPrivate::DVideoWidgetPrivate(DVideoWidget *qq)
    : DObjectPrivate(qq)
    , formatProxy(new VideoFormatProxy(qq))
{
    qq->connect(formatProxy, &VideoFormatProxy::currentFrameChanged,
                qq, static_cast<void (DVideoWidget::*)()>(&DVideoWidget::repaint));
}

/*!
 * \class DVideoWidget
 * \brief The DVideoWidget class provides a widget which presents video produced
 * by a media object.
 *
 * Since the last frame that played is preserved, it always has better user
 * experience looping some media. Also it supports video flip (both vertically
 * or horizontally), video scale and rounded video clip.
 *
 * It's usually used just to play video animations like the one presented in
 * dde-zone settings page. If you want to play video or other media objects,
 * please refer to QVideoWidget for better preformance or support.
 *
 * \param parent is the parent widget to be attached on.
 */

DVideoWidget::DVideoWidget(QWidget *parent)
    : QWidget(parent)
    , DObject(*new DVideoWidgetPrivate(this))
{

}

/*!
 * \property DVideoWidget::mirroredHorizontal
 * \brief indicates whether the video is horizontally flipped.
 */
bool DVideoWidget::mirroredHorizontal() const
{
    D_DC(DVideoWidget);

    return d->mirroredHorizontal;
}

/*!
 * \property DVideoWidget::mirroredVertical
 * \brief indicates whether the video is vertically flipped.
 */
bool DVideoWidget::mirroredVertical() const
{
    D_DC(DVideoWidget);

    return d->mirroredVertical;
}

/*!
 * @brief DVideoWidget::paint paints a specific QVideoFrame onto the widget.
 * @param frame is the target video frame.
 */
void DVideoWidget::paint(const QVideoFrame &frame)
{
    D_DC(DVideoWidget);

    QPainter painter(this);

    QImage image(
                frame.bits(),
                frame.width(),
                frame.height(),
                frame.bytesPerLine(),
                QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat()));

    painter.drawImage(0, 0, image.mirrored(d->mirroredHorizontal, d->mirroredVertical));
}

/*!
 * \property DVideoWidget::scale
 * \brief the scale ratio used to paint the video frames.
 */
qreal DVideoWidget::scale() const
{
    D_DC(DVideoWidget);

    return d->scale;
}

/*!
 * \property DVideoWidget::aspectRatioMode
 * \brief holds the current aspect ratio.
 */
Qt::AspectRatioMode DVideoWidget::aspectRatioMode() const
{
    D_DC(DVideoWidget);

    return d->aspectRatioMode;
}

void DVideoWidget::setSourceVideoPixelRatio(const qreal ratio)
{
    D_D(DVideoWidget);

    d->ratio = ratio;
}

qreal DVideoWidget::sourceVideoPixelRatio() const
{
    D_DC(DVideoWidget);

    return d->ratio;
}

/*!
 * \property DVideoWidget::brightness
 * \brief returns the brightness adjust setting.
 *
 * \note
 * This property is not implemented.
 */
int DVideoWidget::brightness() const
{
    D_DC(DVideoWidget);

    return d->brightness;
}

/*!
 * \property DVideoWidget::contrast
 * \brief returns the contrast adjust setting.
 *
 * \note
 * This property is not implemented.
 */
int DVideoWidget::contrast() const
{
    D_DC(DVideoWidget);

    return d->contrast;
}

/*!
 * \property DVideoWidget::hue
 * \brief returns the hue adjust setting.
 *
 * \note
 * This property is not implemented.
 */
int DVideoWidget::hue() const
{
    D_DC(DVideoWidget);

    return d->hue;
}

/*!
 * \property DVideoWidget::saturation
 * \brief This property holds an adjustment to the saturation of displayed video.
 *
 * \note
 * This property is not implemented.
 */
int DVideoWidget::saturation() const
{
    D_DC(DVideoWidget);

    return d->saturation;
}

/**
 * @brief DVideoWidget::currentFrame
 * @return the current frame displaying.
 */
const QVideoFrame *DVideoWidget::currentFrame() const
{
    D_DC(DVideoWidget);

    if (d->formatProxy) {
        return &d->formatProxy->currentFrame();
    }

    return NULL;
}

/**
 * @brief DVideoWidget::capture grabs the current frame.
 * @return a QPixmap representing the current frame.
 */
QPixmap DVideoWidget::capture()
{
    return grab();
}

/*!
 * \property DVideoWidget::round
 * \brief controls whether the painted video frame are rounded or not.
 */
bool DVideoWidget::round() const
{
    D_DC(DVideoWidget);

    return d->round;
}

/**
 * @brief DVideoWidget::setSource sets a QCamera source to be tracked.
 * @param source is the target camera source.
 */
void DVideoWidget::setSource(QCamera *source)
{
    Q_ASSERT(source);
    D_DC(DVideoWidget);

    source->setCaptureMode(QCamera::CaptureStillImage);
    source->setViewfinder(d->formatProxy);
}

/**
 * @brief DVideoWidget::setSource sets a QMediaPlayer source to be played.
 * @param source is the target media player source.
 */
void DVideoWidget::setSource(QMediaPlayer *source)
{
    Q_ASSERT(source);
    D_D(DVideoWidget);

    source->setVideoOutput(d->formatProxy);
    d->player = source;
}

void DVideoWidget::setMirroredHorizontal(bool mirroredHorizontal)
{
    D_D(DVideoWidget);

    if (d->mirroredHorizontal == mirroredHorizontal)
        return;

    d->mirroredHorizontal = mirroredHorizontal;
    Q_EMIT mirroredHorizontalChanged(mirroredHorizontal);
}

void DVideoWidget::setMirroredVertical(bool mirroredVertical)
{
    D_D(DVideoWidget);

    if (d->mirroredVertical == mirroredVertical)
        return;

    d->mirroredVertical = mirroredVertical;
    Q_EMIT mirroredVerticalChanged(mirroredVertical);
}

void DVideoWidget::setScale(qreal scale)
{
    D_D(DVideoWidget);

    if (d->scale == scale)
        return;

    d->scale = scale;
    Q_EMIT scaleChanged(scale);
}

void DVideoWidget::setAspectRatioMode(Qt::AspectRatioMode mode)
{
    D_D(DVideoWidget);

    d->aspectRatioMode = mode;
}

void DVideoWidget::setBrightness(int brightness)
{
    D_D(DVideoWidget);

    if (d->brightness == brightness)
        return;

    d->brightness = brightness;
    Q_EMIT brightnessChanged(brightness);
}

void DVideoWidget::setContrast(int contrast)
{
    D_D(DVideoWidget);

    if (d->contrast == contrast)
        return;

    d->contrast = contrast;
    Q_EMIT contrastChanged(contrast);
}

void DVideoWidget::setHue(int hue)
{
    D_D(DVideoWidget);

    if (d->hue == hue)
        return;

    d->hue = hue;
    Q_EMIT hueChanged(hue);
}

void DVideoWidget::setSaturation(int saturation)
{
    D_D(DVideoWidget);

    if (d->saturation == saturation)
        return;

    d->saturation = saturation;
    Q_EMIT saturationChanged(saturation);
}

void DVideoWidget::setRound(bool round)
{
    D_D(DVideoWidget);

    if (d->round == round)
        return;

    d->round = round;
    Q_EMIT roundChanged(round);
}

void DVideoWidget::paintEvent(QPaintEvent *)
{
    D_DC(DVideoWidget);

    QPainter painter(this);

    const QMediaPlaylist *pl = d->player ? d->player->playlist() : NULL;
    bool loop = pl && (pl->playbackMode() == QMediaPlaylist::Loop || pl->playbackMode() == QMediaPlaylist::CurrentItemInLoop);
    QVideoFrame frame = (!loop || d->formatProxy->m_currentFrame.isValid()) ? d->formatProxy->m_currentFrame : d->formatProxy->m_lastFrame;

    frame.map(QAbstractVideoBuffer::ReadOnly);
    QImage image(
                frame.bits(),
                frame.width(),
                frame.height(),
                frame.bytesPerLine(),
                QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat()));
    frame.unmap();

    if (image.isNull()) {
        return;
    }

    image.setDevicePixelRatio(d->ratio);
    image = image.scaled(size() * d->scale * d->ratio, d->aspectRatioMode, Qt::SmoothTransformation);
    image = image.mirrored(d->mirroredHorizontal, d->mirroredVertical);

    if (d->round) {
        QPainterPath path;
        int diameter = qMin(width(), height());
        path.addEllipse(width()/2.0-diameter/2.0, height()/2.0-diameter/2.0, diameter, diameter);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        painter.setClipPath(path);
    }

    painter.drawImage(QRectF(rect()).center() - QRectF(image.rect()).center() / d->ratio, image);
}

DWIDGET_END_NAMESPACE

#include "dvideowidget.moc"
