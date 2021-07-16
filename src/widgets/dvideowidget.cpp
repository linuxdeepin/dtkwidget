/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#include "dvideowidget.h"
#include <DObjectPrivate>

#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>
#include <QCamera>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QPainter>
#include <QPainterPath>
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

/*!
  \internal
  \class Dtk::Widget::VideoFormatProxy
  \inmodule dtkwidget
  \brief DVideoWidget使用的封装视频帧的代理类
 */

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
  \class Dtk::Widget::DVideoWidget
  \inmodule dtkwidget
  \brief The DVideoWidget class provides a widget which presents video produced
  by a media object.
  \brief DVideoWidget类提供了呈现视频的小部件.
  
  Since the last frame that played is preserved, it always has better user
  experience looping some media. Also it supports video flip (both vertically
  or horizontally), video scale and rounded video clip.
  
  It's usually used just to play video animations like the one presented in
  dde-zone settings page. If you want to play video or other media objects,
  please refer to QVideoWidget for better performance or support.
  

  为了给循环播放提供更好的用户体验，视频的最后一帧将会被保留，同时还支持视频垂直或水平翻转，
  视频缩放和圆形视频编辑。
  
  将QMediaPlayer添加到DVideoWidget中，DVideoWidget封装了QVideoWidget来提供视频或图像的输出。
  \note 注意: 一次只能将一个QMediaPlayer连接到DVideoWidget中。
  
  \code
  DVideoWidget *videoWidget = new DVideoWidget(this);
  QMediaPlayer *mediaPlayer = new QMediaPlayer(this);
  QMediaPlaylist *list = new QMediaPlaylist(this);
  list->addMedia(QUrl("qrc:/test.mp4"));
  list->setPlaybackMode(QMediaPlaylist::Loop);
  videoWidget->setVideoOutput(mediaPlayer);
  videoWidget->play();
  \endcode
 */

/*!
  \fn void DVideoWidget::mirroredHorizontalChanged(bool mirroredHorizontal)
  \brief 当前的视频或画面发生水平翻转时会发出该信号
 */
/*!
  \fn void DVideoWidget::mirroredVerticalChanged(bool mirroredVertical)
  \brief 当前的视频或画面发生垂直翻转时会发出该信号
 */
/*!
  \fn void DVideoWidget::scaleChanged(qreal scale)
  \brief 当相对于视频或画面的原始大小的比例发生变化时会发出该信号
 */
/*!
  \fn void DVideoWidget::brightnessChanged(int brightness)
  \brief 当前的视频或画面的亮度发生变化时会发出该信号
 */
/*!
  \fn void DVideoWidget::contrastChanged(int contrast)
  \brief 当前的视频或画面的对比度发生变化时会发出该信号
 */
/*!
  \fn void DVideoWidget::hueChanged(int hue)
  \brief 当前的视频或画面的色彩度发生变化时会发出该信号
 */
/*!
  \fn void DVideoWidget::saturationChanged(int saturation)
  \brief 当前视频或画面的饱和度发生变化时会发出该信号
 */
/*!
  \fn void DVideoWidget::roundChanged(bool round)
  \brief 当前开启和关闭圆形效果时会发出该信号
 */

/*!
  \brief DVideoWidget的构造函数
  
  \a parent
 */
DVideoWidget::DVideoWidget(QWidget *parent)
    : QWidget(parent)
    , DObject(*new DVideoWidgetPrivate(this))
{

}

/*!
  \property DVideoWidget::mirroredHorizontal
  \brief indicates whether the video is horizontally flipped.
  \brief 返回当前视频或画面是否水平翻转画面.
  
  \sa DVideoWidget::setMirroredHorizontal()
  \sa DVideoWidget::mirroredHorizontalChanged()
  
  \return bool 是否水平翻转画面
 */
bool DVideoWidget::mirroredHorizontal() const
{
    D_DC(DVideoWidget);

    return d->mirroredHorizontal;
}

/*!
  \property DVideoWidget::mirroredVertical
  \brief indicates whether the video is vertically flipped.
  \brief 返回当前视频或画面是否垂直翻转画面.
  
  \sa DVideoWidget::setMirroredVertical()
  \sa DVideoWidget::mirroredVerticalChanged()
  
  \return bool 是否垂直翻转
 */
bool DVideoWidget::mirroredVertical() const
{
    D_DC(DVideoWidget);

    return d->mirroredVertical;
}

/*!
  \brief DVideoWidget::paint paints a specific QVideoFrame onto the widget.
  \a frame is the target video frame.
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
  \property DVideoWidget::scale

  \brief the scale ratio used to paint the video frames.
  \brief 返回当前视频或画面相对于原始大小的缩放比例.
  
  \sa DVideoWidget::setScale()
  \sa DVideoWidget::scaleChanged()
  
  \return qreal 相对于原始大小的缩放比例
 */
qreal DVideoWidget::scale() const
{
    D_DC(DVideoWidget);

    return d->scale;
}

/*!
  \property DVideoWidget::aspectRatioMode

  \brief 返回当前视频或画面的宽高比模式.
  \brief holds the current aspect ratio.
  
  \sa DVideoWidget::setAspectRatioMode()
  
  \return Qt::AspectRatioMode
 */
Qt::AspectRatioMode DVideoWidget::aspectRatioMode() const
{
    D_DC(DVideoWidget);

    return d->aspectRatioMode;
}

/*!
  \brief 设置当前的视频或画面在HiDPI下的缩放系数
  
  \a ratio
  
  \sa DVideoWidget::sourceVideoPixelRatio()
 */
void DVideoWidget::setSourceVideoPixelRatio(const qreal ratio)
{
    D_D(DVideoWidget);

    d->ratio = ratio;
}

/*!
  \brief 返回视频在HiDPI下的缩放系数
  
  \sa DVideoWidget::setSourceVideoPixelRatio()
  
  \return qreal
 */
qreal DVideoWidget::sourceVideoPixelRatio() const
{
    D_DC(DVideoWidget);

    return d->ratio;
}

/*!
  \property DVideoWidget::brightness
  \brief returns the brightness adjust setting.
  
  \property DVideoWidget::brightness
  \brief 返回当前视频或画面的画面亮度
  
  \note 该属性暂未实现
  \note This property is not implemented.
  
  \sa DVideoWidget::setBrightness()
  \sa DVideoWidget::brightnessChanged()
 */
int DVideoWidget::brightness() const
{
    D_DC(DVideoWidget);

    return d->brightness;
}

/*!
  \property DVideoWidget::contrast

  \brief returns the contrast adjust setting.
  \brief 返回当前的视频或画面的对比度
  
  \note This property is not implemented.
  \note 该属性尚未实现
  
  \sa DVideoWidget::setContrast()
  \sa DVideoWidget::contrastChanged()
 */
int DVideoWidget::contrast() const
{
    D_DC(DVideoWidget);

    return d->contrast;
}

/*!
  \property DVideoWidget::hue
  \brief returns the hue adjust setting.
  \brief 返回当前视频或画面的色调
  
  \note This property is not implemented.
  \note 该属性尚未实现
  
  \sa DVideoWidget::setHue()
  \sa DVideoWidget::hueChanged()
 */
int DVideoWidget::hue() const
{
    D_DC(DVideoWidget);

    return d->hue;
}

/*!
  \property DVideoWidget::saturation
  \brief This property holds an adjustment to the saturation of displayed video.
  \brief 返回当前的视频或画面的饱和度.
  
  \note This property is not implemented.
  \note 该属性尚未实现
  
  \sa DVideoWidget::setSaturation()
  \sa DVideoWidget::saturationChanged()
 */
int DVideoWidget::saturation() const
{
    D_DC(DVideoWidget);

    return d->saturation;
}

/*!
  \brief DVideoWidget::currentFrame
  \brief 返回当前的画面帧

  \return the current frame displaying.
 */
const QVideoFrame *DVideoWidget::currentFrame() const
{
    D_DC(DVideoWidget);

    if (d->formatProxy) {
        return &d->formatProxy->currentFrame();
    }

    return NULL;
}

/*!
  \brief 捕获当前的画面
  \brief DVideoWidget::capture grabs the current frame.

  \return a QPixmap representing the current frame.
  \return QPixmap 当前的画面帧
 */
QPixmap DVideoWidget::capture()
{
    return grab();
}

/*!
  \property DVideoWidget::round

  \brief 控制绘制的视频是否为圆形
  \brief controls whether the painted video frame are rounded or not.
  
  \sa DVideoWidget::setRound()
  \sa DVideoWidget::roundChanged()
  
  \return boo 是否为圆形
 */
bool DVideoWidget::round() const
{
    D_DC(DVideoWidget);

    return d->round;
}

/*!
  \brief 设置要跟踪的QCamera源
  \brief DVideoWidget::setSource sets a QCamera source to be tracked.

  \a source is the target camera source.
 */
void DVideoWidget::setSource(QCamera *source)
{
    Q_ASSERT(source);
    D_DC(DVideoWidget);

    source->setCaptureMode(QCamera::CaptureStillImage);
    source->setViewfinder(d->formatProxy);
}

/*!
  \brief 设置要跟踪的QMediaPlayer源
  \brief DVideoWidget::setSource sets a QMediaPlayer source to be played.


  \a source is the target media player source.
 */
void DVideoWidget::setSource(QMediaPlayer *source)
{
    Q_ASSERT(source);
    D_D(DVideoWidget);

    source->setVideoOutput(d->formatProxy);
    d->player = source;
}

/*!
  \brief 设置是否开启当前视频或画面水平翻转
  
  \a mirroredHorizontal
  
  \sa DVideoWidget::mirroredHorizontal()
  \sa DVideoWidget::mirroredHorizontalChanged()
 */
void DVideoWidget::setMirroredHorizontal(bool mirroredHorizontal)
{
    D_D(DVideoWidget);

    if (d->mirroredHorizontal == mirroredHorizontal)
        return;

    d->mirroredHorizontal = mirroredHorizontal;
    Q_EMIT mirroredHorizontalChanged(mirroredHorizontal);
}

/*!
  \brief 设置是否开启当前的视频或画面的垂直翻转
  
  \a mirroredVertical
  
  \sa DVideoWidget::mirroredVertical()
  \sa DVideoWidget::mirroredVerticalChanged()
 */
void DVideoWidget::setMirroredVertical(bool mirroredVertical)
{
    D_D(DVideoWidget);

    if (d->mirroredVertical == mirroredVertical)
        return;

    d->mirroredVertical = mirroredVertical;
    Q_EMIT mirroredVerticalChanged(mirroredVertical);
}

/*!
  \brief 设置相对于原始视频或画面大小的缩放
  
  \sa DVideoWidget::scale()
  \sa DVideoWidget::scaleChanged()
  
  \a scale
 */
void DVideoWidget::setScale(qreal scale)
{
    D_D(DVideoWidget);

    if (d->scale == scale)
        return;

    d->scale = scale;
    Q_EMIT scaleChanged(scale);
}

/*!
  \brief 设置当前的视频或画面的缩放模式
  
  \a mode
  
  \sa DVideoWidget::aspectRatioMode()
 */
void DVideoWidget::setAspectRatioMode(Qt::AspectRatioMode mode)
{
    D_D(DVideoWidget);

    d->aspectRatioMode = mode;
}

/*!
  \brief 设置当前的视频或画面的亮度
  
  \a brightness
  
  \sa DVideoWidget::brightness()
  \sa DVideoWidget::brightnessChanged()
 */
void DVideoWidget::setBrightness(int brightness)
{
    D_D(DVideoWidget);

    if (d->brightness == brightness)
        return;

    d->brightness = brightness;
    Q_EMIT brightnessChanged(brightness);
}

/*!
  \brief 设置当前视频或画面的对比度
  
  \a contrast
  
  \sa DVideoWidget::contrast()
  \sa DVideoWidget::contrastChanged()
 */
void DVideoWidget::setContrast(int contrast)
{
    D_D(DVideoWidget);

    if (d->contrast == contrast)
        return;

    d->contrast = contrast;
    Q_EMIT contrastChanged(contrast);
}

/*!
  \brief 设置当前视频或画面的色彩度
  
  \a hue
  
  \sa DVideoWidget::hue()
  \sa DVideoWidget::hueChanged()
 */
void DVideoWidget::setHue(int hue)
{
    D_D(DVideoWidget);

    if (d->hue == hue)
        return;

    d->hue = hue;
    Q_EMIT hueChanged(hue);
}

/*!
  \brief 设置当前的视频或画面的饱和度
  
  \a saturation
  
  \sa DVideoWidget::saturation()
  \sa DVideoWidget::saturationChanged()
 */
void DVideoWidget::setSaturation(int saturation)
{
    D_D(DVideoWidget);

    if (d->saturation == saturation)
        return;

    d->saturation = saturation;
    Q_EMIT saturationChanged(saturation);
}

/*!
  \brief 设置是否开启视频圆形
  
  \a round
  
  \sa DVideoWidget::round()
  \sa DVideoWidget::roundChanged()
 */
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
