// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dwatermarkwidget.h"

#include <DObjectPrivate>
#include <DWidgetUtil>

#include <QPainter>
#include <QResizeEvent>
#include <QDebug>

#include <cmath>

DCORE_USE_NAMESPACE
DTK_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

DWIDGET_USE_NAMESPACE

class WaterMarkDataPrivate: public DTK_CORE_NAMESPACE::DObjectPrivate
{
protected:
    explicit WaterMarkDataPrivate(WaterMarkData *parent)
        : DObjectPrivate(parent)
    {
    }

private:
    WaterMarkData::WaterMarkType type = WaterMarkData::None;            // 水印类型
    WaterMarkData::WaterMarkLayout layout = WaterMarkData::Center;      // 水印布局
    qreal scaleFactor = 1.0;                                            // 整体大小缩放系数
    int spacing = 0;                                                    // 间距
    int lineSpacing = 0;                                                // 行间距
    QString text;                                                       // 文本水印内容
    QFont font;                                                         // 文本水印字体
    QColor color;                                                       // 文本水印颜色
    qreal rotation = 0;                                                 // 水印旋转角度(0~360)
    qreal opacity = 1;                                                  // 水印透明度(0~1)
    QImage image;                                                       // 图片水印中的图片
    bool grayScale = true;                                              // 是否灰度化图片

    D_DECLARE_PUBLIC(WaterMarkData)
};

/*!
  \class Dtk::Widget::WaterMarkData
  \inmodule dtkwidget
  \brief Watermark information structure
  \a parent
 */
WaterMarkData::WaterMarkData()
    : DObject(*new WaterMarkDataPrivate(this))
{

}

/*!
  \class Dtk::Widget::WaterMarkData
  \inmodule dtkwidget
  \brief coping constructing function
  \a parent
 */
WaterMarkData::WaterMarkData(const WaterMarkData &p)
 : DObject(*new WaterMarkDataPrivate(this))
{
    D_D(WaterMarkData);
    auto pd = p.d_func();

    d->type = pd->type;
    d->layout = pd->layout;
    d->scaleFactor = pd->scaleFactor;
    d->spacing = pd->spacing;
    d->lineSpacing = pd->lineSpacing;
    d->text = pd->text;
    d->font = pd->font;
    d->color = pd->color;
    d->rotation = pd->rotation;
    d->opacity = pd->opacity;
    d->image = pd->image;
    d->grayScale = pd->grayScale;
}

/*!
  \class Dtk::Widget::WaterMarkData
  \inmodule dtkwidget
  \brief Assignment constructor
  \a parent
 */
WaterMarkData &WaterMarkData::operator=(const WaterMarkData &p)
{
    D_D(WaterMarkData);

    if (&p == this)
        return *this;

    auto pd = p.d_func();
    d->type = pd->type;
    d->layout = pd->layout;
    d->scaleFactor = pd->scaleFactor;
    d->spacing = pd->spacing;
    d->lineSpacing = pd->lineSpacing;
    d->text = pd->text;
    d->font = pd->font;
    d->color = pd->color;
    d->rotation = pd->rotation;
    d->opacity = pd->opacity;
    d->image = pd->image;
    d->grayScale = pd->grayScale;

    return *this;
}

/*!
  \brief get the current watermark type
 */
WaterMarkData::WaterMarkType WaterMarkData::type() const
{
    D_DC(WaterMarkData);
    return d->type;
}

/*!
  \brief set the current waternark 
  \a type
 */
void WaterMarkData::setType(WaterMarkType type)
{
    D_D(WaterMarkData);
    d->type = type;
}

/*!
  \brief get the current watermark layout
 */
WaterMarkData::WaterMarkLayout WaterMarkData::layout() const
{
    D_DC(WaterMarkData);
    return d->layout;
}

/*!
  \brief set the current waternark 
  \a layout
 */
void WaterMarkData::setLayout(WaterMarkLayout layout)
{
    D_D(WaterMarkData);
    d->layout = layout;
}

/*!
  \brief get the current watermark scale factor
 */
qreal WaterMarkData::scaleFactor() const
{
    D_DC(WaterMarkData);
    return d->scaleFactor;
}

/*!
  \brief set the current waternark 
  \a scaleFactor
 */
void WaterMarkData::setScaleFactor(qreal scaleFactor)
{
    D_D(WaterMarkData);
    d->scaleFactor = scaleFactor;
}

/*!
  \brief get the current watermark spacing
 */
int WaterMarkData::spacing() const
{
    D_DC(WaterMarkData);
    return d->spacing;
}

/*!
  \brief set the current waternark \a spacing
 */
void WaterMarkData::setSpacing(int spacing)
{
    D_D(WaterMarkData);
    d->spacing = spacing;
}

/*!
  \brief get the current watermark line spacing
 */
int WaterMarkData::lineSpacing() const
{
    D_DC(WaterMarkData);
    return d->lineSpacing;
}

/*!
  \brief set the current waternark 
  \a lineSpacing
 */
void WaterMarkData::setLineSpacing(int lineSpacing)
{
    D_D(WaterMarkData);
    d->lineSpacing = lineSpacing;
}

/*!
  \brief get the current watermark text
 */
QString WaterMarkData::text() const
{
    D_DC(WaterMarkData);
    return d->text;
}

/*!
  \brief set the current waternark 
  \a text
 */
void WaterMarkData::setText(const QString &text)
{
    D_D(WaterMarkData);
    d->text = text;
}

/*!
  \brief get the current watermark font
 */
QFont WaterMarkData::font() const
{
    D_DC(WaterMarkData);
    return d->font;
}

/*!
  \brief set the current waternark 
  \a font
 */
void WaterMarkData::setFont(const QFont &font)
{
    D_D(WaterMarkData);
    d->font = font;
}

/*!
  \brief get the current watermark color
 */
QColor WaterMarkData::color() const
{
    D_DC(WaterMarkData);
    return d->color;
}

/*!
  \brief set the current waternark 
  \a color
 */
void WaterMarkData::setColor(const QColor &color)
{
    D_D(WaterMarkData);
    d->color = color;
}

/*!
  \brief get the current watermark rotation(0~360)
 */
qreal WaterMarkData::rotation() const
{
    D_DC(WaterMarkData);
    return d->rotation;
}

/*!
  \brief set the current waternark (0~360)
  \a rotation
 */
void WaterMarkData::setRotation(qreal rotation)
{
    D_D(WaterMarkData);
    d->rotation = rotation;
}

/*!
  \brief get the current watermark opacity(0~1.0)
 */
qreal WaterMarkData::opacity() const
{
    D_DC(WaterMarkData);
    return d->opacity;
}

/*!
  \brief set the current waternark (0~1.0)
  \a opacity
 */
void WaterMarkData::setOpacity(qreal opacity)
{
    D_D(WaterMarkData);
    d->opacity = opacity;
}

/*!
  \brief get the current watermark image
 */
QImage WaterMarkData::image() const
{
    D_DC(WaterMarkData);
    return d->image;
}

/*!
  \brief set the current waternark \a image
 */
void WaterMarkData::setImage(const QImage &image)
{
    D_D(WaterMarkData);
    d->image = image;
}

/*!
  \brief get the current watermark grayscale of image
 */
bool WaterMarkData::grayScale() const
{
    D_DC(WaterMarkData);
    return d->grayScale;
}

/*!
  \brief set the current waternark \a grayScale of image,default value is true
 */
void WaterMarkData::setGrayScale(bool grayScale)
{
    D_D(WaterMarkData);
    d->grayScale = grayScale;
}

static QImage createTextureImage(const WaterMarkData &data, qreal deviceScale)
{
    QImage texture;

    switch (data.type()) {
    case WaterMarkData::WaterMarkType::None:
        return texture;
    case WaterMarkData::WaterMarkType::Text:
    {
        // 缩放处理
        QFont font = data.font();
        if (!(font.styleStrategy() & QFont::PreferAntialias))
            font.setStyleStrategy(QFont::PreferAntialias);
        font.setPointSize(qRound(font.pointSize() * data.scaleFactor() * deviceScale));

        // 边距计算
        QFontMetrics fm(font);
        QSize textSize = fm.size(Qt::TextSingleLine, data.text());
        int realLineSpacing = qMax(qMin(textSize.width(), textSize.height()), data.lineSpacing());
        QSize spaceSize = QSize(qMax(0, data.spacing()), realLineSpacing) * deviceScale;

        // 绘制纹理
        texture = QImage(textSize + spaceSize, QImage::Format_ARGB32);
        texture.fill(Qt::transparent);
        QPainter tp;
        tp.begin(&texture);
        tp.setFont(font);
        tp.setPen(data.color());
        tp.setBrush(Qt::NoBrush);
        tp.setRenderHint(QPainter::TextAntialiasing);
        tp.drawText(texture.rect(), Qt::AlignCenter, data.text());
        tp.end();
    }
        break;
    case WaterMarkData::WaterMarkType::Image:
    {
        // 缩放处理
        QImage img = data.image();
        if (data.grayScale()) {
            DWIDGET_NAMESPACE::grayScale(data.image(), img, data.image().rect());
        }

        img = img.scaledToWidth(qRound(img.width() * data.scaleFactor() * deviceScale));
        int realLineSpacing = qMax(qMin(img.width(), img.height()), data.lineSpacing());
        texture = QImage(img.size() + QSize(qMax(0, data.spacing()), data.lineSpacing() < 0 ? realLineSpacing : data.lineSpacing())
                         , QImage::Format_ARGB32);
        texture.fill(Qt::transparent);

        // 拷贝所有像素到一张添加了边距和行间距的图中
        for (int i = 0; i < img.width(); ++i) {
            for (int j = 0; j < img.height(); ++j) {
                texture.setPixelColor(i, j, img.pixelColor(i, j));
            }
        }
    }
        break;
    }
    return texture;
}

void drawWaterTexture(QPainter &painter, const QImage &texture, qreal rotation, const QRect &paintRect)
{
    painter.save();
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);

    // 旋转画刷,避免调整父界面大小时水印内容有移动
    QBrush b;
    b.setTextureImage(texture);
    QPoint center = texture.rect().center();
    QTransform t = QTransform()
            .translate(center.x(), center.y())
            .rotate(rotation)
            .translate(-center.x(), -center.y());
    b.setTransform(t);

//    painter.setPen(Qt::NoPen);
    painter.setBrush(b);
    painter.drawRect(paintRect);
    painter.restore();
}

class DWaterMarkWidgetPrivate: public DTK_CORE_NAMESPACE::DObjectPrivate
{
protected:
    explicit DWaterMarkWidgetPrivate(DWaterMarkWidget *parent)
        : DObjectPrivate(parent)
    {

    }

private:
    void init();

    WaterMarkData data;
    QImage textureImage;

    D_DECLARE_PUBLIC(DWaterMarkWidget)
};

void DWaterMarkWidgetPrivate::init()
{
    D_Q(DWaterMarkWidget);

    q->setObjectName("DWaterMarkWidget");
    q->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    q->setFocusPolicy(Qt::NoFocus);
}

/*!
  \class Dtk::Widget::DWaterMarkWidget
  \inmodule dtkwidget
  \brief The watermark class will cover the set parent interface and dynamically adjust the size following the parent interface.
  \a parent
 */
DWaterMarkWidget::DWaterMarkWidget(QWidget *parent)
    : QWidget(parent)
    , DObject(*new DWaterMarkWidgetPrivate(this))
{
    D_D(DWaterMarkWidget);
    d->init();

    if (parent)
        parent->installEventFilter(this);
}

/*!
  \brief get the current setting
 */
const WaterMarkData &DWaterMarkWidget::data()
{
    D_DC(DWaterMarkWidget);
    return d->data;
}

/*!
  \brief set the current setting \a data
 */
void DWaterMarkWidget::setData(const WaterMarkData &data)
{
    D_D(DWaterMarkWidget);

    d->data = data;
    d->textureImage = createTextureImage(d->data, devicePixelRatioF());

    update();
}

void DWaterMarkWidget::paintEvent(QPaintEvent *)
{
    D_D(DWaterMarkWidget);

    qreal deviceScale = devicePixelRatioF();

    QPainter painter(this);
    painter.setOpacity(d->data.opacity());

    switch (d->data.type()) {
    case WaterMarkData::WaterMarkType::None:
        return;
    case WaterMarkData::WaterMarkType::Text: {
        // 居中处理
        if (d->data.layout() == WaterMarkData::Center) {
            // 缩放处理
            QFont font = d->data.font();
            if (!(font.styleStrategy() & QFont::PreferAntialias))
                font.setStyleStrategy(QFont::PreferAntialias);
            font.setPointSize(qRound(d->data.font().pointSize() * d->data.scaleFactor() * deviceScale));

            auto center = rect().center();
            painter.translate(center.x(), center.y());
            painter.rotate(d->data.rotation());
            painter.translate(-center.x(), -center.y());

            painter.save();
            painter.setRenderHint(QPainter::TextAntialiasing);
            painter.setFont(font);
            painter.setPen(d->data.color());
            painter.drawText(rect(), Qt::AlignCenter, d->data.text());
            painter.restore();
        } else {
            drawWaterTexture(painter, d->textureImage, d->data.rotation(), rect());
        }
        break;
    }
    case WaterMarkData::WaterMarkType::Image:
    {
        if (d->data.image().isNull() || qFuzzyCompare(d->data.scaleFactor(), 0))
            return;

        // 居中处理
        if (d->data.layout() == WaterMarkData::Center) {
            // 缩放处理
            QImage img = d->data.image();
            if (d->data.grayScale())
                DWIDGET_NAMESPACE::grayScale(d->data.image(), img, d->data.image().rect());
            img = img.scaledToWidth(qRound(img.width() * d->data.scaleFactor() * deviceScale));
            QSize size = img.size() / img.devicePixelRatio();
            int imgWidth = size.width();
            int imgHeight = size.height();

            auto center = rect().center();
            painter.translate(center.x(), center.y());
            painter.rotate(d->data.rotation());
            painter.translate(-center.x(), -center.y());

            QPointF leftTop(rect().center().x() - imgWidth / 2.0, rect().center().y() - imgHeight / 2.0);
            painter.drawImage(leftTop, img);
        } else {
            drawWaterTexture(painter, d->textureImage, d->data.rotation(), rect());
        }
        break;
    }
    }
}

bool DWaterMarkWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched != parent())
        return false;

    // 保持和父界面尺寸一致
    if (event->type() == QEvent::Resize) {
        QResizeEvent *e = static_cast<QResizeEvent *>(event);
        resize(e->size());
    }

    return QWidget::eventFilter(watched, event);
}

DWIDGET_END_NAMESPACE
