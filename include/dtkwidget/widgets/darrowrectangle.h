// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DARROWRECTANGLE_H
#define DARROWRECTANGLE_H

#include <QDesktopWidget>
#include <QWidget>
#include <QLabel>
#include <QTextLine>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QTimer>
#include <QDebug>

#include <DObject>
#include <dtkwidget_global.h>
#include <DThemeManager>
#include <dgraphicsgloweffect.h>
#include <DBlurEffectWidget>

DWIDGET_BEGIN_NAMESPACE

class DPlatformWindowHandle;
class DArrowRectanglePrivate;
class LIBDTKWIDGETSHARED_EXPORT DArrowRectangle : public QWidget, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DArrowRectangle)
    D_DECLARE_PRIVATE(DArrowRectangle)

public:

    enum ArrowDirection {
        ArrowLeft,
        ArrowRight,
        ArrowTop,
        ArrowBottom
    };

    enum FloatMode {
        FloatWindow,
        FloatWidget,
    };

    explicit DArrowRectangle(ArrowDirection direction, QWidget *parent = nullptr);
    explicit DArrowRectangle(ArrowDirection direction, FloatMode floatMode, QWidget *parent = nullptr);
    ~DArrowRectangle() override;

    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
    Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth)
    Q_PROPERTY(int radius READ radius WRITE setRadius)
    Q_PROPERTY(int arrowWidth READ arrowWidth WRITE setArrowWidth)
    Q_PROPERTY(int arrowHeight READ arrowHeight WRITE setArrowHeight)
    Q_PROPERTY(int arrowX READ arrowX WRITE setArrowX)
    Q_PROPERTY(int arrowY READ arrowY WRITE setArrowY)
    Q_PROPERTY(int margin READ margin WRITE setMargin)
    Q_PROPERTY(ArrowDirection arrowDirection READ arrowDirection WRITE setArrowDirection)
    Q_PROPERTY(qreal shadowXOffset READ shadowXOffset WRITE setShadowXOffset)
    Q_PROPERTY(qreal shadowYOffset READ shadowYOffset WRITE setShadowYOffset)
    Q_PROPERTY(qreal shadowBlurRadius READ shadowBlurRadius WRITE setShadowBlurRadius)

    int radius() const;
    bool radiusForceEnabled() const;
    int arrowHeight() const;
    int arrowWidth() const;
    int arrowX() const;
    int arrowY() const;
    int margin() const;
    int borderWidth() const;
    QColor borderColor() const;
    QColor backgroundColor() const;
    ArrowDirection arrowDirection() const;

    void setRadius(int value);
    void setRadiusForceEnable(bool enable);
    void setArrowHeight(int value);
    void setArrowWidth(int value);
    void setArrowX(int value);
    void setArrowY(int value);
    void setMargin(int value);
    void setBorderWidth(int borderWidth);
    void setBorderColor(const QColor &borderColor);
    void setBackgroundColor(const QColor &backgroundColor);
    void setBackgroundColor(DBlurEffectWidget::MaskColorType type);
    void setArrowDirection(ArrowDirection value);
    void setWidth(int value);
    void setHeight(int value);

    virtual void show(int x, int y);

    void setContent(QWidget *content);
    QWidget *getContent() const;
    void resizeWithContent();
    void move(int x, int y);
    QSize getFixedSize();

    qreal shadowXOffset() const;
    qreal shadowYOffset() const;
    qreal shadowBlurRadius() const;

    void setShadowBlurRadius(const qreal &shadowBlurRadius);
    void setShadowXOffset(const qreal &shadowXOffset);
    void setShadowYOffset(const qreal &shadowYOffset);

    void setLeftRightRadius(bool enable);
    void setRadiusArrowStyleEnable(bool enable);

Q_SIGNALS:
    void windowDeactivate() const;

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;
    bool event(QEvent *e) Q_DECL_OVERRIDE;
};

DWIDGET_END_NAMESPACE

#endif // DARROWRECTANGLE_H

