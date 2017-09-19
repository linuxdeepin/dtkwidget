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

#ifndef DVIDEOWIDGET_H
#define DVIDEOWIDGET_H

#include "dtkwidget_global.h"
#include "dobject.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
class QCamera;
class QMediaPlayer;
class QVideoFrame;
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

class DVideoWidgetPrivate;
class DVideoWidget : public QWidget, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(bool mirroredHorizontal READ mirroredHorizontal WRITE setMirroredHorizontal NOTIFY mirroredHorizontalChanged)
    Q_PROPERTY(bool mirroredVertical READ mirroredVertical WRITE setMirroredVertical NOTIFY mirroredVerticalChanged)
    Q_PROPERTY(qreal scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(Qt::AspectRatioMode aspectRatioMode READ aspectRatioMode WRITE setAspectRatioMode)
    Q_PROPERTY(int brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged)
    Q_PROPERTY(int contrast READ contrast WRITE setContrast NOTIFY contrastChanged)
    Q_PROPERTY(int hue READ hue WRITE setHue NOTIFY hueChanged)
    Q_PROPERTY(int saturation READ saturation WRITE setSaturation NOTIFY saturationChanged)
    Q_PROPERTY(bool round READ round WRITE setRound NOTIFY roundChanged)

public:
    explicit DVideoWidget(QWidget *parent = 0);

    bool mirroredHorizontal() const;
    bool mirroredVertical() const;
    void paint(const QVideoFrame& frame);
    qreal scale() const;
    Qt::AspectRatioMode aspectRatioMode() const;

    int brightness() const;
    int contrast() const;
    int hue() const;
    int saturation() const;

    const QVideoFrame* currentFrame() const;

    QPixmap capture();

    bool round() const;

Q_SIGNALS:
    void mirroredHorizontalChanged(bool mirroredHorizontal);
    void mirroredVerticalChanged(bool mirroredVertical);
    void scaleChanged(qreal scale);
    void brightnessChanged(int brightness);
    void contrastChanged(int contrast);
    void hueChanged(int hue);
    void saturationChanged(int saturation);
    void roundChanged(bool round);

public Q_SLOTS:
    void setSource(QCamera *source);
    void setSource(QMediaPlayer *source);
    void setMirroredHorizontal(bool mirroredHorizontal);
    void setMirroredVertical(bool mirroredVertical);

    void setScale(qreal scale);
    void setAspectRatioMode(Qt::AspectRatioMode mode);
    void setBrightness(int brightness);
    void setContrast(int contrast);
    void setHue(int hue);
    void setSaturation(int saturation);
    void setRound(bool round);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    D_DECLARE_PRIVATE(DVideoWidget)
};

DWIDGET_END_NAMESPACE

#endif // DVIDEOWIDGET_H
