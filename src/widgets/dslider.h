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

#ifndef DSLIDER_H
#define DSLIDER_H

#include <QSlider>

#include "dtkwidget_global.h"

DWIDGET_BEGIN_NAMESPACE

class DSliderPrivate;
class LIBDTKWIDGETSHARED_EXPORT DSlider : public QSlider
{
    Q_OBJECT
    Q_PROPERTY(int handleType READ handleType)
    Q_PROPERTY(QColor tipColor READ tipColor WRITE setTipColor)
    Q_PROPERTY(QColor scaleColor READ scaleColor WRITE setScaleColor)
    Q_PROPERTY(bool hoverShowValue READ hoverShowValue WRITE setHoverShowValue)
    Q_PROPERTY(QColor hoverValueColor READ hoverValueColor WRITE setHoverValueColor)
    Q_PROPERTY(int hoverShowValueInterval READ hoverShowValueInterval WRITE setHoverShowValueInterval)

public:
    /*!
     * \brief The HandleType enum contains the handle types can be used by DSlider.
     */
    enum HandleType {
        SharpHandler, /*!< Rectangle with one side replaced by an arrow, like handle on verniers */
        RoundHandle /*!< Round shape handle */
    };

    DSlider(QWidget *parent = 0);
    DSlider(Qt::Orientation orientation, QWidget *parent = 0);
    ~DSlider();

    int handleType() const;
    void setHandleType(HandleType handleType);

    QString leftTip() const;
    void setLeftTip(const QString &leftTip);

    QString rightTip() const;
    void setRightTip(const QString &rightTip);

    QColor tipColor() const;
    void setTipColor(const QColor &tipColor);

    QColor scaleColor() const;
    void setScaleColor(const QColor &scaleColor);

    void addScale(int value);
    void removeScale(int value);

    bool hoverShowValue() const;
    QColor hoverValueColor() const;
    int hoverShowValueInterval() const;

public Q_SLOTS:
    void setHoverShowValue(bool hoverShowValue);
    void setHoverValueColor(QColor hoverValueColor);
    void setHoverShowValueInterval(int hoverShowValueInterval);

protected:
    DSlider(DSliderPrivate &d);
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

private Q_SLOTS:
    void hoverTimout();

private:
    DSliderPrivate *d_ptr;

    void init();

    Q_DECLARE_PRIVATE(DSlider)
    Q_DISABLE_COPY(DSlider)
};

DWIDGET_END_NAMESPACE

#endif // DSLIDER_H
