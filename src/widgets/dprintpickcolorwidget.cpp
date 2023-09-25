// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dprintpickcolorwidget.h"

#include "diconbutton.h"
#include "dlabel.h"
#include "dlineedit.h"
#include "dapplication.h"
#include "dslider.h"
#include "dfloatingwidget.h"
#include "diconbutton.h"
#include "qbuttongroup.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QKeyEvent>
#include <DWindowManagerHelper>
#include <DIconTheme>

#define PICKCOLOR_RADIUS 8
const int IMAGE_HEIGHT = 10;

DWIDGET_BEGIN_NAMESPACE
DGUI_USE_NAMESPACE
/*!
  \brief ColorButton::ColorButton 取色框颜色选择按钮
 */
ColorButton::ColorButton(QColor color, QWidget *parent)
    : DPushButton(parent)
    , m_color(color)
{
    setFixedSize(34, 34);
    setCheckable(true);
    QObject::connect(this, &ColorButton::clicked, this, [=] {
        this->setChecked(true);
        Q_EMIT this->selectColorButton(m_color);
    });
}

void ColorButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen;
    pen.setWidth(1);
    pen.setColor(QColor(0, 0, 0, 55));
    painter.setBrush(QBrush(m_color));
    painter.setPen(pen);
    painter.drawRoundedRect(QRect(3, 3, this->width() - 6,
                                  this->height() - 6),
                            PICKCOLOR_RADIUS, PICKCOLOR_RADIUS);
    if (isChecked()) {
        painter.setBrush(QBrush());
        QPen borderPen;
        borderPen.setWidth(2);
        borderPen.setColor("#008eff");
        painter.setPen(borderPen);
        painter.drawRoundedRect(QRect(1, 1, this->width() - 2,
                                      this->height() - 2),
                                PICKCOLOR_RADIUS, PICKCOLOR_RADIUS);
    }
}

DPrintPickColorWidget::DPrintPickColorWidget(QWidget *parent)
    : DWidget(parent)
    , pinterface(nullptr)
{
    initUI();
    initConnection();
}

void DPrintPickColorWidget::initUI()
{
    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    QGridLayout *btnLayout = new QGridLayout;
    colorList
        << QColor("#f52000") << QColor("#ff5d00") << QColor("#f8cb00") << QColor("#23c400") << QColor("#00a48a") << QColor("#0081ff") << QColor("#3c02d7") << QColor("#6a00b5")
        << QColor("#FFFFFF") << QColor("#e3e3e3") << QColor("#c9c9c9") << QColor("#adadad") << QColor("#6f6f6f") << QColor("#404040") << QColor("#1b1b1b") << QColor("#000000");
    btnGroup = new QButtonGroup(this);
    for (int i = 0; i <= colorList.count() - 1; i++) {
        // cppcheck-suppress arrayIndexOutOfBounds
        ColorButton *bn = new ColorButton(colorList[i]);
        bn->setFocusPolicy(Qt::NoFocus);
        btnlist.append(bn);
        btnGroup->setExclusive(true);
        btnGroup->addButton(bn, i);
        btnLayout->addWidget(bn, i / 8, i % 8);
    }
    QHBoxLayout *valueLayout = new QHBoxLayout;
    DLabel *valueLabel = new DLabel(qApp->translate("PickColorWidget", "Color"));
    valueLineEdit = new DLineEdit;
    valueLineEdit->setClearButtonEnabled(false);
    valueLineEdit->lineEdit()->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9A-Fa-f]{6,8}"), this));
    valueLayout->setContentsMargins(0, 0, 0, 0);
    valueLayout->addWidget(valueLabel);
    valueLayout->addSpacing(5);
    valueLayout->addWidget(valueLineEdit);
    QHBoxLayout *rgbPickColorLayout = new QHBoxLayout;
    DLabel *rgbLabel = new DLabel("RGB");
    rEdit = new DLineEdit;
    gEdit = new DLineEdit;
    bEdit = new DLineEdit;
    rEdit->setFixedWidth(57);
    gEdit->setFixedWidth(55);
    bEdit->setFixedWidth(55);
    rEdit->setClearButtonEnabled(false);
    gEdit->setClearButtonEnabled(false);
    bEdit->setClearButtonEnabled(false);
    pickColorBtn = new DIconButton(this);
    pickColorBtn->setFixedSize(55, 36);
    pickColorBtn->setIcon(DIconTheme::findQIcon("dorpper_normal"));
    pickColorBtn->setIconSize(QSize(32, 32));
    pickColorBtn->setEnabled(DWindowManagerHelper::instance()->hasComposite());
    rgbPickColorLayout->addWidget(rgbLabel);
    rgbPickColorLayout->addWidget(rEdit);
    rgbPickColorLayout->addWidget(gEdit);
    rgbPickColorLayout->addWidget(bEdit);
    rgbPickColorLayout->addWidget(pickColorBtn);

    colorLabel = new ColorLabel(this);
    colorLabel->setFixedSize(285, 140);
    colorSlider = new ColorSlider(this);

    mainlayout->addSpacing(15);
    mainlayout->addLayout(btnLayout);
    mainlayout->addLayout(valueLayout);
    mainlayout->addLayout(rgbPickColorLayout);
    mainlayout->addWidget(colorLabel);
    mainlayout->addWidget(colorSlider);
    mainlayout->addSpacing(10);
}

void DPrintPickColorWidget::initConnection()
{
    for (ColorButton *btn : btnlist) {
        connect(btn, &ColorButton::selectColorButton, this, [=](QColor color) {
            this->setRgbEdit(color, true);
        });
    }

    connect(colorSlider, &ColorSlider::valueChanged, colorLabel, [=](int val) {
        colorLabel->setHue(val);
    });

    connect(pickColorBtn, &DPushButton::clicked, this, [=] {
        if (!pinterface) {
            pinterface = new QDBusInterface("com.deepin.Picker", "/com/deepin/Picker", "com.deepin.Picker", QDBusConnection::sessionBus());
            connect(pinterface, SIGNAL(colorPicked(QString, QString)), this, SLOT(slotColorPick(QString, QString)));
        }

        pinterface->call("StartPick", QString("%1").arg(qApp->applicationPid()));
    });

    connect(colorLabel, &ColorLabel::pickedColor, this, [=](QColor color) {
        this->setRgbEdit(color);
    });

    connect(valueLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotEditColor(QString)));
    connect(DWindowManagerHelper::instance(), &DWindowManagerHelper::hasCompositeChanged, this, [this]() {
        this->pickColorBtn->setEnabled(DWindowManagerHelper::instance()->hasComposite());
    });
}

/*!
  \brief PickColorWidget::setRgbEdit 将QColor转化为RGB形式显示
  \a color 颜色 btnColor 判断是否为按钮的颜色
 */
void DPrintPickColorWidget::setRgbEdit(QColor color, bool btnColor)
{
    rEdit->setText(QString("%1").arg(color.red()));
    gEdit->setText(QString("%1").arg(color.green()));
    bEdit->setText(QString("%1").arg(color.blue()));
    convertColor(color, btnColor);
}

/*!
  \brief PickColorWidget::convertColor 将QColor转化为详细颜色
  \a color 颜色 btnColor 判断是否为按钮的颜色
 */
void DPrintPickColorWidget::convertColor(QColor color, bool btnColor)
{
    QString colorName = "";
    if (color.name().contains("#")) {
        colorName = color.name().split("#").last();
    }
    if (!btnColor) {
        if (color.isValid()) {
            if (colorList.contains(color)) {
                btnlist[colorList.indexOf(color)]->setChecked(true);
            } else {
                btnGroup->setExclusive(false);
                for (ColorButton *btn : btnlist) {
                    btn->setChecked(false);
                }
                btnGroup->setExclusive(true);
            }
        }
    }
    valueLineEdit->blockSignals(true);
    valueLineEdit->setText(colorName);
    valueLineEdit->blockSignals(false);
    Q_EMIT this->selectColorButton(color);
}

/*!
  \brief PickColorWidget::slotColorPick 调起deepin-picer取色窗口
  \a  uuid 当前进程号  colorName 颜色名称
 */
void DPrintPickColorWidget::slotColorPick(QString uuid, QString colorName)
{
    if (uuid == QString("%1").arg(qApp->applicationPid())) {
        setRgbEdit(QColor(colorName));
    }
}

/*!
  \brief PickColorWidget::slotEditColor 颜色输入框变化判断是否是按钮的颜色
  \a  str 颜色色号
 */
void DPrintPickColorWidget::slotEditColor(QString str)
{
    if (str.length() < 6) {
        return;
    }
    str.insert(0, "#");
    QColor color(str);
    if (color.isValid()) {
        if (colorList.contains(color)) {
            btnlist[colorList.indexOf(color)]->setChecked(true);
            Q_EMIT btnlist[colorList.indexOf(color)]->clicked();

        } else {
            btnGroup->setExclusive(false);
            for (ColorButton *btn : btnlist) {
                btn->setChecked(false);
            }
            btnGroup->setExclusive(true);
        }
    }
}

DPrintPickColorWidget::~DPrintPickColorWidget()
{
    delete pinterface;
}

/*!
  \brief ColorLabel::ColorLabel 提供取色的Label
 */
ColorLabel::ColorLabel(DWidget *parent)
    : DLabel(parent)
    , m_pressed(false)
    , m_tipPoint(this->rect().center())
{
    setMouseTracking(true);
    connect(this, &ColorLabel::clicked, this, [=] {
        pickColor(m_clickedPos);
    });
}

/*!
  \brief ColorLabel::getColor 获取Label上取到的颜色
  \a  qreal h, qreal s, qreal v 位置
  \return  QColor 取到的颜色
 */
QColor ColorLabel::getColor(qreal h, qreal s, qreal v)
{
    int hi = int(h / 60) % 6;
    qreal f = h / 60 - hi;

    qreal p = v * (1 - s);
    qreal q = v * (1 - f * s);
    qreal t = v * (1 - (1 - f) * s);

    if (hi == 0) {
        return QColor(std::min(int(255 * p), 255), std::min(int(255 * q), 255), std::min(int(255 * v), 255));
    } else if (hi == 1) {
        return QColor(std::min(int(255 * t), 255), std::min(int(255 * p), 255), std::min(int(255 * v), 255));
    } else if (hi == 2) {
        return QColor(std::min(int(255 * v), 255), std::min(int(255 * p), 255), int(255 * q));
    } else if (hi == 3) {
        return QColor(std::min(int(255 * v), 255), std::min(int(255 * t), 255), std::min(int(255 * p), 255));
    } else if (hi == 4) {
        return QColor(std::min(int(255 * q), 255), std::min(int(255 * v), 255), std::min(int(255 * p), 255));
    } else {
        return QColor(std::min(int(255 * p), 255), std::min(int(255 * v), 255), std::min(int(255 * t), 255));
    }
}

/*!
  \brief ColorLabel::setHue 设置取色Label的基础颜色
 */
void ColorLabel::setHue(int hue)
{
    m_hue = hue;
    update();
}

/*!
  \brief ColorLabel::pickColor 选择要水印的颜色
  \a  pos 选取颜色的位置
 */
void ColorLabel::pickColor(QPoint pos)
{
    if (pos.x() < 0 || pos.y() < 0 || pos.x() >= this->width() || pos.y() >= this->height()) {
        return;
    }

    QPixmap pickPixmap;
    pickPixmap = this->grab(QRect(0, 0, this->width(), this->height()));
    QImage pickImg = pickPixmap.toImage();

    if (!pickImg.isNull()) {
        QRgb pickRgb = pickImg.pixel(pos);
        m_pickedColor = QColor(qRed(pickRgb), qGreen(pickRgb), qBlue(pickRgb));
    } else {
        m_pickedColor = QColor(0, 0, 0);
    }
    Q_EMIT pickedColor(m_pickedColor);
}

/*!
  \brief ColorLabel::pickColorCursor 绘制点击时的鼠标光标样式
 */
QCursor ColorLabel::pickColorCursor()
{
    int tipWidth = 11;
    QPixmap cursorPix = QPixmap(QSize(tipWidth, tipWidth));
    cursorPix.fill(QColor(Qt::transparent));

    QPen whitePen;
    whitePen.setWidth(1);
    whitePen.setCapStyle(Qt::FlatCap);
    whitePen.setJoinStyle(Qt::RoundJoin);
    whitePen.setColor(QColor(255, 255, 255, 255));

    QPen blackPen;
    blackPen.setWidth(1);
    blackPen.setCapStyle(Qt::FlatCap);
    blackPen.setJoinStyle(Qt::RoundJoin);
    blackPen.setColor(QColor(0, 0, 0, 125));

    QPainter painter(&cursorPix);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(blackPen);
    painter.drawEllipse(cursorPix.rect().center(), tipWidth / 2 - 1, tipWidth / 2 - 1);
    painter.drawEllipse(cursorPix.rect().center(), tipWidth / 2 - 3, tipWidth / 2 - 3);
    painter.setPen(whitePen);
    painter.drawEllipse(cursorPix.rect().center(), tipWidth / 2 - 2, tipWidth / 2 - 2);

    return QCursor(cursorPix, -1, -1);
}

void ColorLabel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QImage backgroundImage(this->width(), this->height(), QImage::Format_ARGB32);
    for (qreal s = 0; s < this->width(); s++) {
        for (qreal v = 0; v < this->height(); v++) {
            QColor penColor = getColor(m_hue, s / this->width(), v / this->height());
            if (!penColor.isValid()) {
                continue;
            }
            backgroundImage.setPixelColor(int(s), this->height() - 1 - int(v), penColor);
        }
    }

    painter.drawImage(this->rect(), backgroundImage);
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
void ColorLabel::enterEvent(QEvent *e)
#else
void ColorLabel::enterEvent(QEnterEvent *e)
#endif
{
    m_lastCursor = this->cursor();
    qApp->setOverrideCursor(pickColorCursor());
    QLabel::enterEvent(e);
}

void ColorLabel::leaveEvent(QEvent *e)
{
    qApp->setOverrideCursor(m_lastCursor);
    QLabel::leaveEvent(e);
}

void ColorLabel::mousePressEvent(QMouseEvent *e)
{
    m_pressed = true;
    m_tipPoint = this->mapFromGlobal(cursor().pos());
    QLabel::mousePressEvent(e);
}

void ColorLabel::mouseMoveEvent(QMouseEvent *e)
{
    if (m_pressed) {
        m_tipPoint = this->mapFromGlobal(cursor().pos());
        pickColor(m_tipPoint);
    }
    update();
    QLabel::mouseMoveEvent(e);
}

void ColorLabel::mouseReleaseEvent(QMouseEvent *e)
{
    if (m_pressed) {
        m_clickedPos = e->pos();
        Q_EMIT clicked();
    }
    m_pressed = false;
    QLabel::mouseReleaseEvent(e);
}

ColorLabel::~ColorLabel()
{
}

/*!
  \brief ColorSlider::ColorSlider 选取取色Label的基础颜色
 */
ColorSlider::ColorSlider(QWidget *parent)
    : QSlider(parent)
{
    setMinimum(0);
    setMaximum(359);
    setOrientation(Qt::Horizontal);
    this->setFixedSize(QSize(285, 14));
    QRect rect = this->rect();
    m_backgroundImage = QImage(rect.width(), IMAGE_HEIGHT, QImage::Format_ARGB32);

    for (qreal s = 0; s < m_backgroundImage.width(); s++) {
        for (qreal v = 1; v <= m_backgroundImage.height(); v++) {
            QColor penColor = getColor(qreal(s / rect.width() * maximum()), 1, 1);
            if (!penColor.isValid()) {
                continue;
            }
            m_backgroundImage.setPixelColor(std::min(int(s), rect.width()), m_backgroundImage.height() - int(v), penColor);
        }
    }
}

ColorSlider::~ColorSlider()
{
}

/*!
  \brief ColorSlider::getColor 获取基础颜色
 */
QColor ColorSlider::getColor(qreal h, qreal s, qreal v)
{
    int hi = int(h / 60) % 6;
    qreal f = h / 60 - hi;
    qreal p = v * (1 - s);
    qreal q = v * (1 - f * s);
    qreal t = v * (1 - (1 - f) * s);
    if (q < 0) {
        q = 0;
    }
    QColor color;
    if (hi == 0) {
        color = QColor(std::min(int(255 * p), 255), std::min(int(255 * q), 255), std::min(int(255 * v), 255));
    } else if (hi == 1) {
        color = QColor(std::min(int(255 * t), 255), std::min(int(255 * p), 255), std::min(int(255 * v), 255));
    } else if (hi == 2) {
        color = QColor(std::min(int(255 * v), 255), std::min(int(255 * p), 255), int(255 * q));
    } else if (hi == 3) {
        color = QColor(std::min(int(255 * v), 255), std::min(int(255 * t), 255), std::min(int(255 * p), 255));
    } else if (hi == 4) {
        color = QColor(std::min(int(255 * q), 255), std::min(int(255 * v), 255), std::min(int(255 * p), 255));
    } else {
        color = QColor(std::min(int(255 * p), 255), std::min(int(255 * v), 255), std::min(int(255 * t), 255));
    }
    return color;
}

void ColorSlider::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev)
    QRect rect = this->rect();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.drawImage(QRect(rect.x(), rect.y() + 2, rect.width(),
                            IMAGE_HEIGHT),
                      m_backgroundImage);

    const int offset = 3;
    const qreal k = (qreal)(value() - minimum()) / (maximum() - minimum());
    qreal x = ((rect.width() - 2 * offset) * k) + offset;

    QPen pen;
    pen.setWidth(1);
    pen.setColor(QColor(0, 0, 0, 51));
    painter.setPen(pen);
    painter.setBrush(QBrush(Qt::white));
    painter.drawRect(QRectF(QPointF(x - offset, rect.top()), QPointF(x + offset, rect.bottom())));
}

DWIDGET_END_NAMESPACE
