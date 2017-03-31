/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
#include "dvideowidget.h"
#include "private/dobject_p.h"

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

    emit currentFrameChanged();
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

class DVideoWidgetPrivate : public DObjectPrivate
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

    D_DECLARE_PUBLIC(DVideoWidget)
};

DVideoWidgetPrivate::DVideoWidgetPrivate(DVideoWidget *qq)
    : DObjectPrivate(qq)
    , formatProxy(new VideoFormatProxy(qq))
{
    qq->connect(formatProxy, &VideoFormatProxy::currentFrameChanged,
                qq, static_cast<void (DVideoWidget::*)()>(&DVideoWidget::repaint));
}

DVideoWidget::DVideoWidget(QWidget *parent)
    : QWidget(parent)
    , DObject(*new DVideoWidgetPrivate(this))
{

}

bool DVideoWidget::mirroredHorizontal() const
{
    D_DC(DVideoWidget);

    return d->mirroredHorizontal;
}

bool DVideoWidget::mirroredVertical() const
{
    D_DC(DVideoWidget);

    return d->mirroredVertical;
}

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

qreal DVideoWidget::scale() const
{
    D_DC(DVideoWidget);

    return d->scale;
}

Qt::AspectRatioMode DVideoWidget::aspectRatioMode() const
{
    D_DC(DVideoWidget);

    return d->aspectRatioMode;
}

int DVideoWidget::brightness() const
{
    D_DC(DVideoWidget);

    return d->brightness;
}

int DVideoWidget::contrast() const
{
    D_DC(DVideoWidget);

    return d->contrast;
}

int DVideoWidget::hue() const
{
    D_DC(DVideoWidget);

    return d->hue;
}

int DVideoWidget::saturation() const
{
    D_DC(DVideoWidget);

    return d->saturation;
}

const QVideoFrame *DVideoWidget::currentFrame() const
{
    D_DC(DVideoWidget);

    if (d->formatProxy) {
        return &d->formatProxy->currentFrame();
    }

    return NULL;
}

//! 返回当前帧
//!
QPixmap DVideoWidget::capture()
{
    return grab();
}

//! 获取视频截图
//!
bool DVideoWidget::round() const
{
    D_DC(DVideoWidget);

    return d->round;
}

void DVideoWidget::setSource(QCamera *source)
{
    Q_ASSERT(source);
    D_DC(DVideoWidget);

    source->setCaptureMode(QCamera::CaptureStillImage);
    source->setViewfinder(d->formatProxy);
}

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
    emit mirroredHorizontalChanged(mirroredHorizontal);
}

void DVideoWidget::setMirroredVertical(bool mirroredVertical)
{
    D_D(DVideoWidget);

    if (d->mirroredVertical == mirroredVertical)
        return;

    d->mirroredVertical = mirroredVertical;
    emit mirroredVerticalChanged(mirroredVertical);
}

void DVideoWidget::setScale(qreal scale)
{
    D_D(DVideoWidget);

    if (d->scale == scale)
        return;

    d->scale = scale;
    emit scaleChanged(scale);
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
    emit brightnessChanged(brightness);
}

void DVideoWidget::setContrast(int contrast)
{
    D_D(DVideoWidget);

    if (d->contrast == contrast)
        return;

    d->contrast = contrast;
    emit contrastChanged(contrast);
}

void DVideoWidget::setHue(int hue)
{
    D_D(DVideoWidget);

    if (d->hue == hue)
        return;

    d->hue = hue;
    emit hueChanged(hue);
}

void DVideoWidget::setSaturation(int saturation)
{
    D_D(DVideoWidget);

    if (d->saturation == saturation)
        return;

    d->saturation = saturation;
    emit saturationChanged(saturation);
}

void DVideoWidget::setRound(bool round)
{
    D_D(DVideoWidget);

    if (d->round == round)
        return;

    d->round = round;
    emit roundChanged(round);
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
                QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat()));
    frame.unmap();

    if (image.isNull()) {
        return;
    }

    image = image.scaled(size()*d->scale, d->aspectRatioMode, Qt::SmoothTransformation);
    image = image.mirrored(d->mirroredHorizontal, d->mirroredVertical);

    if (d->round) {
        QPainterPath path;
        int diameter = qMin(width(), height());
        path.addEllipse(width()/2.0-diameter/2.0, height()/2.0-diameter/2.0, diameter, diameter);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        painter.setClipPath(path);
    }

    painter.drawImage(rect().center() - image.rect().center(), image);
}

DWIDGET_END_NAMESPACE

#include "dvideowidget.moc"
